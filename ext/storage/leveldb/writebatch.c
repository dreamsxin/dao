
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

#include "storage/leveldb/writebatch.h"
#include "storage/exception.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "kernel/file.h"
#include "kernel/exception.h"

/**
 * Dao\Storage\Leveldb\Writebatch
 *
 */
zend_class_entry *dao_storage_leveldb_writebatch_ce;

PHP_METHOD(Dao_Storage_Leveldb_Writebatch, put);
PHP_METHOD(Dao_Storage_Leveldb_Writebatch, delete);
PHP_METHOD(Dao_Storage_Leveldb_Writebatch, clear);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_leveldb_wirtebatch_put, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_leveldb_wirtebatch_delete, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_storage_leveldb_writebatch_method_entry[] = {
	PHP_ME(Dao_Storage_Leveldb_Writebatch, put, arginfo_dao_storage_leveldb_wirtebatch_put, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Leveldb_Writebatch, delete, arginfo_dao_storage_leveldb_wirtebatch_delete, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Leveldb_Writebatch, clear, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

zend_object_handlers dao_storage_leveldb_writebatch_object_handlers;
zend_object* dao_storage_leveldb_writebatch_object_create_handler(zend_class_entry *ce)
{
	dao_storage_leveldb_writebatch_object *intern = ecalloc(1, sizeof(dao_storage_leveldb_writebatch_object) + zend_object_properties_size(ce));
	intern->std.ce = ce;

	zend_object_std_init(&intern->std, ce);
	object_properties_init(&intern->std, ce);
	intern->std.handlers = &dao_storage_leveldb_writebatch_object_handlers;

	intern->batch = leveldb_writebatch_create();

	return &intern->std;
}

void dao_storage_leveldb_writebatch_object_free_handler(zend_object *object)
{
	dao_storage_leveldb_writebatch_object *intern = dao_storage_leveldb_writebatch_object_from_obj(object);
	if (intern->batch) {
		leveldb_writebatch_destroy(intern->batch);
		intern->batch = NULL;
	}
}

/**
 * Dao\Storage\Leveldb\Writebatch initializer
 */
DAO_INIT_CLASS(Dao_Storage_Leveldb_Writebatch){

	DAO_REGISTER_CLASS_CREATE_OBJECT(Dao\\Storage\\Leveldb, Writebatch, storage_leveldb_writebatch, dao_storage_leveldb_writebatch_method_entry, 0);

	return SUCCESS;
}

/**
 * Adds a put operation for the given key and value to the write batch
 *
 * @param string $key
 * @param string $value
 * @return mixed
 */
PHP_METHOD(Dao_Storage_Leveldb_Writebatch, put)
{
	zval *key, *value;
	dao_storage_leveldb_writebatch_object *intern;

	dao_fetch_params(0, 2, 0, &key, &value);

	intern = dao_storage_leveldb_writebatch_object_from_obj(Z_OBJ_P(getThis()));

	leveldb_writebatch_put(intern->batch, Z_STRVAL_P(key), Z_STRLEN_P(key), Z_STRVAL_P(value), Z_STRLEN_P(value));

	RETURN_TRUE;
}

/**
 * Adds a deletion operation for the given key to the write batch
 *
 * @param string $key
 * @return boolean
 */
PHP_METHOD(Dao_Storage_Leveldb_Writebatch, delete)
{
	zval *key;
	dao_storage_leveldb_writebatch_object *intern;

	dao_fetch_params(0, 1, 0, &key);

	intern = dao_storage_leveldb_writebatch_object_from_obj(Z_OBJ_P(getThis()));

	leveldb_writebatch_delete(intern->batch, Z_STRVAL_P(key), Z_STRLEN_P(key));

	RETURN_TRUE;
}

/**
 * Clears all of operations in the write batch
 *
 * @return boolean
 */
PHP_METHOD(Dao_Storage_Leveldb_Writebatch, clear)
{
	dao_storage_leveldb_writebatch_object *intern;

	intern = dao_storage_leveldb_writebatch_object_from_obj(Z_OBJ_P(getThis()));

	leveldb_writebatch_clear(intern->batch);

	RETURN_TRUE;
}
