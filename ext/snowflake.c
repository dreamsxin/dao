
/*
  +------------------------------------------------------------------------+
  | Dao Framework                                                          |
  +------------------------------------------------------------------------+
  | Copyright (c) 2011-2014 Phalcon Team (http://www.phalconphp.com)       |
  +------------------------------------------------------------------------+
  | This source file is subject to the New BSD License that is bundled     |
  | with this package in the file docs/LICENSE.txt.                        |
  |                                                                        |
  | If you did not receive a copy of the license and are unable to         |
  | obtain it through the world-wide-web, please send an email             |
  | to license@phalconphp.com so we can send you a copy immediately.       |
  +------------------------------------------------------------------------+
  | Authors: Andres Gutierrez <andres@phalconphp.com>                      |
  |          Eduar Carvajal <eduar@phalconphp.com>                         |
  |          ZhuZongXin <dreamsxin@qq.com>                                 |
  +------------------------------------------------------------------------+
*/

#include "snowflake.h"
#include "exception.h"

#include <inttypes.h>

#include "kernel/main.h"
#include "kernel/shm.h"
#include "kernel/exception.h"
#include "kernel/object.h"
#include "kernel/operators.h"

/**
 * Dao\Snowflake
 *
 *<code>
 *  $snowflake = new Dao\Snowflake;
 *  $id = $snowflake->nextId();
 *  $desc = $snowflake->parse($id);
 *</code>
 */
zend_class_entry *dao_snowflake_ce;

PHP_METHOD(Dao_Snowflake, nextId);
PHP_METHOD(Dao_Snowflake, parse);
PHP_METHOD(Dao_Snowflake, parseID);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_snowflake_parse, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, id, IS_STRING, 0)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_snowflake_method_entry[] = {
	PHP_ME(Dao_Snowflake, nextId, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Snowflake, parse, arginfo_dao_snowflake_parse, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Snowflake, parseID, arginfo_dao_snowflake_parse, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_FE_END
};

static uint64_t get_time_in_ms()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((uint64_t)tv.tv_sec) * 1000ULL + ((uint64_t)tv.tv_usec) / 1000ULL;
}

static uint64_t till_next_ms(uint64_t last_ts)
{
    uint64_t ts;
    while((ts = get_time_in_ms()) <= last_ts);
    return ts;
}

static uint64_t key2int(char *key)
{
    uint64_t v;
    if (sscanf(key, "%" PRIdMAX, &v) == 0) {
        return 0;
    }
    return v;
}

zend_object_handlers dao_snowflake_object_handlers;
zend_object* dao_snowflake_object_create_handler(zend_class_entry *ce)
{
	dao_snowflake_object *intern = ecalloc(1, sizeof(dao_snowflake_object) + zend_object_properties_size(ce));
	intern->std.ce = ce;

	zend_object_std_init(&intern->std, ce);
	object_properties_init(&intern->std, ce);
	intern->std.handlers = &dao_snowflake_object_handlers;

	intern->shm = dao_shared_memory_open("dao_snowflake");
	if (NULL == intern->shm) {
		intern->shm = dao_shared_memory_create("dao_snowflake", sizeof(snowflake_data_t));
		if (NULL == intern->shm) {
			DAO_THROW_EXCEPTION_FORMAT(dao_exception_ce, "Failed to create shared memory (%d)", errno);
			return &intern->std;
		}

		int size = dao_shared_memory_size(intern->shm);
		if (size < sizeof(snowflake_data_t)) {
			DAO_THROW_EXCEPTION_STR(dao_exception_ce, "Shared memory size is error");
			return &intern->std;
		}

		intern->data = (snowflake_data_t*)dao_shared_memory_ptr(intern->shm);
		if (!intern->data) {
			DAO_THROW_EXCEPTION_STR(dao_exception_ce, "Failed to get shared memory");
			return &intern->std;
		}

		intern->data = (snowflake_data_t*)dao_shared_memory_ptr(intern->shm);
		intern->data->sequence  = 0;
		intern->data->timestamp = 0;
	} else {
		intern->data = (snowflake_data_t*)dao_shared_memory_ptr(intern->shm);

		int size = dao_shared_memory_size(intern->shm);
		if (size < sizeof(snowflake_data_t)) {
			DAO_THROW_EXCEPTION_STR(dao_exception_ce, "Shared memory size is error");
			return &intern->std;
		}

		intern->data = (snowflake_data_t*)dao_shared_memory_ptr(intern->shm);
		if (!intern->data) {
			DAO_THROW_EXCEPTION_STR(dao_exception_ce, "Failed to get shared memory");
			return &intern->std;
		}
	}

	return &intern->std;
}

void dao_snowflake_object_free_handler(zend_object *object)
{
	dao_snowflake_object *intern = dao_snowflake_object_from_obj(object);
	if (intern->shm) {
		intern->data = NULL;
		dao_shared_memory_cleanup(intern->shm);
		intern->shm = NULL;
	}
}

/**
 * Dao\Snowflake initializer
 */
DAO_INIT_CLASS(Dao_Snowflake){

	DAO_REGISTER_CLASS_CREATE_OBJECT(Dao, Snowflake, snowflake, dao_snowflake_method_entry, 0);

	return SUCCESS;
}

/**
 * Returns a unique id value generated by the snowflake
 *
 *<code>
 *  $snowflake = new \Dao\Snowflake;
 *  $id = $snowflake->nextId();
 *</code>
 */
PHP_METHOD(Dao_Snowflake, nextId){

	dao_snowflake_object *intern;

	intern = dao_snowflake_object_from_obj(Z_OBJ_P(getThis()));
	if (intern->data != NULL) {
		if (!dao_shared_memory_lock(intern->shm)) {
			zend_long len;
			char *str = NULL;
			uint64_t ts;
			ts = get_time_in_ms();
			if (ts == intern->data->timestamp) {		
				intern->data->sequence = (intern->data->sequence + 1) & 0xFFF;
				if(intern->data->sequence == 0)
				{
					ts = till_next_ms(ts);
				}
			} else  {
				intern->data->sequence = 0;
			}	
			intern->data->timestamp = ts;		
			uint64_t id = ((ts - DAO_GLOBAL(snowflake).epoch) << 22) | ((DAO_GLOBAL(snowflake).node & 0x3FF) << 12) | intern->data->sequence;	
			dao_shared_memory_unlock(intern->shm);
			len = spprintf(&str, 0, "%" PRId64, id);
			RETVAL_STRINGL(str, len);
			efree(str);
		} else {
			RETURN_FALSE;
		}
	} else {
		RETURN_FALSE;
	}
}

/**
 * Parse a unique id value
 *
 *<code>
 *  $snowflake = new \Dao\Snowflake;
 *  $desc = $snowflake->parse($id);
 *</code>
 */
PHP_METHOD(Dao_Snowflake, parse){

	zval *value;
	uint64_t id;
	int node, ts;

	dao_fetch_params(0, 1, 0, &value);

	if(!(id = key2int(Z_STRVAL_P(value)))) {
		RETURN_FALSE;
	}
	node = (id >> 12) & 0x3FF;
	ts   = ((id >> 22) + DAO_GLOBAL(snowflake).epoch) / 1000ULL;	
	array_init(return_value);
	add_assoc_long(return_value, "node", node);
	add_assoc_double(return_value, "timestamp", ts);
}
/**
 * Parse a unique id value
 *
 *<code>
 *  $desc = \Dao\Snowflake::parseID($id);
 *</code>
 */
PHP_METHOD(Dao_Snowflake, parseID){

	zval *value;
	uint64_t id;
	int node, ts;

	dao_fetch_params(0, 1, 0, &value);

	if(!(id = key2int(Z_STRVAL_P(value)))) {
		RETURN_FALSE;
	}
	node = (id >> 12) & 0x3FF;
	ts   = ((id >> 22) + DAO_GLOBAL(snowflake).epoch) / 1000ULL;	
	array_init(return_value);
	add_assoc_long(return_value, "node", node);
	add_assoc_double(return_value, "timestamp", ts);
}
