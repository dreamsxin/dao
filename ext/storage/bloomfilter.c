
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

#include "storage/bloomfilter.h"
#include "storage/exception.h"

#include "kernel/main.h"
#include "kernel/exception.h"
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "kernel/bloomfilter.h"

/**
 * Dao\Storage\Bloomfilter
 *
 * This class defines bloomfilter entity and its description
 *
 */
zend_class_entry *dao_storage_bloomfilter_ce;

PHP_METHOD(Dao_Storage_Bloomfilter, __construct);
PHP_METHOD(Dao_Storage_Bloomfilter, add);
PHP_METHOD(Dao_Storage_Bloomfilter, check);
PHP_METHOD(Dao_Storage_Bloomfilter, reset);
PHP_METHOD(Dao_Storage_Bloomfilter, save);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_bloomfilter___construct, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, filename, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, seed, IS_LONG, 1)
	ZEND_ARG_TYPE_INFO(0, maxItems, IS_LONG, 1)
	ZEND_ARG_TYPE_INFO(0, falsePositive, IS_DOUBLE, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_bloomfilter_add, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, value, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_bloomfilter_check, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, value, IS_STRING, 0)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_storage_bloomfilter_method_entry[] = {
	PHP_ME(Dao_Storage_Bloomfilter, __construct, arginfo_dao_storage_bloomfilter___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL|ZEND_ACC_CTOR)
	PHP_ME(Dao_Storage_Bloomfilter, add, arginfo_dao_storage_bloomfilter_add, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Bloomfilter, check, arginfo_dao_storage_bloomfilter_check, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Bloomfilter, reset, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Bloomfilter, save, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

zend_object_handlers dao_storage_bloomfilter_object_handlers;
zend_object* dao_storage_bloomfilter_object_create_handler(zend_class_entry *ce)
{
	dao_storage_bloomfilter_object *intern = ecalloc(1, sizeof(dao_storage_bloomfilter_object) + zend_object_properties_size(ce));
	intern->std.ce = ce;

	zend_object_std_init(&intern->std, ce);
	object_properties_init(&intern->std, ce);
	intern->std.handlers = &dao_storage_bloomfilter_object_handlers;

	memset(&intern->bloomfilter, 0, sizeof(dao_bloomfilter));

	return &intern->std;
}

void dao_storage_bloomfilter_object_free_handler(zend_object *object)
{
	dao_storage_bloomfilter_object *intern = dao_storage_bloomfilter_object_from_obj(object);

	dao_bloomfilter_save(&intern->bloomfilter, Z_STRVAL(intern->filename));
	dao_bloomfilter_free(&intern->bloomfilter);
}

/**
 * Dao\Storage\Bloomfilter initializer
 */
DAO_INIT_CLASS(Dao_Storage_Bloomfilter){

	DAO_REGISTER_CLASS_CREATE_OBJECT(Dao\\Storage, Bloomfilter, storage_bloomfilter, dao_storage_bloomfilter_method_entry, 0);

	return SUCCESS;
}

/**
 * Dao\Storage\Bloomfilter constructor
 */
PHP_METHOD(Dao_Storage_Bloomfilter, __construct){

	zval *filename, *_seed = NULL, *_max_items = NULL, *_false_positive = NULL;
	dao_storage_bloomfilter_object *intern;
	uint32_t seed = 0, max_items = 100000;
	double false_positive = 0.00001;

	dao_fetch_params(0, 1, 3, &filename, &_seed, &_max_items, &_false_positive);

	if (_seed && Z_TYPE_P(_seed) == IS_LONG) {
		seed = Z_LVAL_P(_seed);
	}

	if (_max_items && Z_TYPE_P(_max_items) == IS_LONG && Z_LVAL_P(_max_items) > 0) {
		max_items = Z_LVAL_P(_max_items);
	}

	if (_false_positive && Z_TYPE_P(_false_positive) == IS_DOUBLE && Z_DVAL_P(_false_positive) < 1 && Z_DVAL_P(_false_positive) > 0) {
		false_positive =  Z_DVAL_P(_false_positive);
	}

	intern = dao_storage_bloomfilter_object_from_obj(Z_OBJ_P(getThis()));

	DAO_ZVAL_DUP(&intern->filename, filename);

	if (dao_bloomfilter_init(&intern->bloomfilter, seed, max_items, false_positive) != 0) {
		DAO_THROW_EXCEPTION_STR(dao_storage_exception_ce, "Create bloom filter failed");
		return;
	}

	dao_bloomfilter_load(&intern->bloomfilter, Z_STRVAL_P(filename));
}

/**
 * Add value
 *
 * @param string value
 * @return boolean
 */
PHP_METHOD(Dao_Storage_Bloomfilter, add){

	zval *value;
	dao_storage_bloomfilter_object *intern;
	int ret;

	dao_fetch_params(0, 1, 0, &value);

	intern = dao_storage_bloomfilter_object_from_obj(Z_OBJ_P(getThis()));
	ret = dao_bloomfilter_add(&intern->bloomfilter, Z_STRVAL_P(value), Z_STRLEN_P(value));
	if (ret < 0) {
		DAO_THROW_EXCEPTION_STR(dao_storage_exception_ce, "Add value error");
		return;
	}
	RETURN_TRUE;
}

/**
 * Check value
 *
 * @param string value
 * @return boolean
 */
PHP_METHOD(Dao_Storage_Bloomfilter, check){

	zval *value;
	dao_storage_bloomfilter_object *intern;

	dao_fetch_params(0, 1, 0, &value);

	intern = dao_storage_bloomfilter_object_from_obj(Z_OBJ_P(getThis()));
	if (!dao_bloomfilter_check(&intern->bloomfilter, Z_STRVAL_P(value), Z_STRLEN_P(value))) {
		RETURN_TRUE;
	}
	RETURN_FALSE;
}

/**
 * Reset
 *
 * @return boolean
 */
PHP_METHOD(Dao_Storage_Bloomfilter, reset){

	dao_storage_bloomfilter_object *intern;

	intern = dao_storage_bloomfilter_object_from_obj(Z_OBJ_P(getThis()));
	if (!dao_bloomfilter_reset(&intern->bloomfilter)) {
		RETURN_TRUE;
	}
	RETURN_FALSE;
}

/**
 * Save data to file
 *
 * @return boolean
 */
PHP_METHOD(Dao_Storage_Bloomfilter, save){

	dao_storage_bloomfilter_object *intern;

	intern = dao_storage_bloomfilter_object_from_obj(Z_OBJ_P(getThis()));
	if (!dao_bloomfilter_save(&intern->bloomfilter, Z_STRVAL(intern->filename))) {
		RETURN_TRUE;
	}
	RETURN_FALSE;
}
