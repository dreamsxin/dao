
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

#include "storage/btree.h"
#include "storage/exception.h"

#include "zend_smart_str.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "kernel/file.h"
#include "kernel/exception.h"

/**
 * Dao\Storage\Btree
 *
 * It can be used to replace APC or local memstoraged.
 */
zend_class_entry *dao_storage_btree_ce;

PHP_METHOD(Dao_Storage_Btree, __construct);
PHP_METHOD(Dao_Storage_Btree, set);
PHP_METHOD(Dao_Storage_Btree, get);
PHP_METHOD(Dao_Storage_Btree, delete);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_btree___construct, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, db, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_btree_set, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_btree_get, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_btree_delete, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_storage_btree_method_entry[] = {
	PHP_ME(Dao_Storage_Btree, __construct, arginfo_dao_storage_btree___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Storage_Btree, set, arginfo_dao_storage_btree_set, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Btree, get, arginfo_dao_storage_btree_get, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Btree, delete, arginfo_dao_storage_btree_delete, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

zend_object_handlers dao_storage_btree_object_handlers;
zend_object* dao_storage_btree_object_create_handler(zend_class_entry *ce)
{
	dao_storage_btree_object *intern = ecalloc(1, sizeof(dao_storage_btree_object) + zend_object_properties_size(ce));
	intern->std.ce = ce;

	zend_object_std_init(&intern->std, ce);
	object_properties_init(&intern->std, ce);
	intern->std.handlers = &dao_storage_btree_object_handlers;

	return &intern->std;
}

void dao_storage_btree_object_free_handler(zend_object *object)
{
	dao_storage_btree_object *intern = dao_storage_btree_object_from_obj(object);

	dao_storage_btree_close(&intern->db);
}

/**
 * Dao\Storage\Btree initializer
 */
DAO_INIT_CLASS(Dao_Storage_Btree){

	DAO_REGISTER_CLASS_CREATE_OBJECT(Dao\\Storage, Btree, storage_btree, dao_storage_btree_method_entry, 0);

	zend_declare_property_null(dao_storage_btree_ce, SL("_db"), ZEND_ACC_PROTECTED);

	return SUCCESS;
}

/**
 * Dao\Storage\Btree constructor
 *
 * @param string $db
 */
PHP_METHOD(Dao_Storage_Btree, __construct)
{
	zval *db;
	dao_storage_btree_object *intern;

	dao_fetch_params(0, 1, 0, &db);

	dao_update_property(getThis(), SL("_db"), db);

	intern = dao_storage_btree_object_from_obj(Z_OBJ_P(getThis()));

	if (dao_storage_btree_open(&intern->db, Z_STRVAL_P(db)) != DAO_STORAGE_BTREE_OK){
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Failed to open Db %s", Z_STRVAL_P(db));
		return;
	}
}

/**
 * Stores storaged content
 *
 * @param string $key
 * @param string $value
 * @return string
 */
PHP_METHOD(Dao_Storage_Btree, set)
{
	zval *key, *value;
	dao_storage_btree_object *intern;

	dao_fetch_params(0, 2, 0, &key, &value);

	intern = dao_storage_btree_object_from_obj(Z_OBJ_P(getThis()));

	if (dao_storage_btree_sets(&intern->db, Z_STRVAL_P(key), Z_STRVAL_P(value)) != DAO_STORAGE_BTREE_OK){
		RETURN_FALSE;
	}

	RETURN_TRUE;
}

/**
 * Returns a storaged content
 *
 * @param string $key
 * @return mixed
 */
PHP_METHOD(Dao_Storage_Btree, get)
{
	zval *key;
	dao_storage_btree_object *intern;
	char* value;

	dao_fetch_params(0, 1, 0, &key);

	intern = dao_storage_btree_object_from_obj(Z_OBJ_P(getThis()));

	if (dao_storage_btree_gets(&intern->db, Z_STRVAL_P(key), &value) == DAO_STORAGE_BTREE_OK){
		RETURN_STRING(value);
		return;
	}

	RETURN_NULL();
}

/**
 * Returns a storaged content
 *
 * @param string|array $keys
 * @return boolean
 */
PHP_METHOD(Dao_Storage_Btree, delete)
{
	zval *key;
	dao_storage_btree_object *intern;

	dao_fetch_params(0, 1, 0, &key);

	intern = dao_storage_btree_object_from_obj(Z_OBJ_P(getThis()));

	if (dao_storage_btree_removes(&intern->db, Z_STRVAL_P(key)) != DAO_STORAGE_BTREE_OK){
		RETURN_FALSE;
	}

	RETURN_TRUE;
}
