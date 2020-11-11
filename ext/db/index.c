
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
  +------------------------------------------------------------------------+
*/

#include "db/index.h"
#include "db/indexinterface.h"
#include "db/exception.h"

#include "kernel/main.h"
#include "kernel/object.h"
#include "kernel/array.h"
#include "kernel/exception.h"
#include "kernel/fcall.h"

/**
 * Dao\Db\Index
 *
 * Allows to define indexes to be used on tables. Indexes are a common way
 * to enhance database performance. An index allows the database server to find
 * and retrieve specific rows much faster than it could do without an index
 */
zend_class_entry *dao_db_index_ce;

PHP_METHOD(Dao_Db_Index, __construct);
PHP_METHOD(Dao_Db_Index, getName);
PHP_METHOD(Dao_Db_Index, getColumns);
PHP_METHOD(Dao_Db_Index, getType);
PHP_METHOD(Dao_Db_Index, __set_state);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_db_index___construct, 0, 0, 2)
	ZEND_ARG_INFO(0, indexName)
	ZEND_ARG_INFO(0, columns)
	ZEND_ARG_INFO(0, type)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_db_index_method_entry[] = {
	PHP_ME(Dao_Db_Index, __construct, arginfo_dao_db_index___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Db_Index, getName, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Index, getColumns, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Index, getType, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Index, __set_state, arginfo___set_state, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};

/**
 * Dao\Db\Index initializer
 */
DAO_INIT_CLASS(Dao_Db_Index){

	DAO_REGISTER_CLASS(Dao\\Db, Index, db_index, dao_db_index_method_entry, 0);

	zend_declare_property_null(dao_db_index_ce, SL("_indexName"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_db_index_ce, SL("_columns"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_db_index_ce, SL("_type"), ZEND_ACC_PROTECTED);

	zend_class_implements(dao_db_index_ce, 1, dao_db_indexinterface_ce);

	return SUCCESS;
}

/**
 * Dao\Db\Index constructor
 *
 * @param string $indexName
 * @param array $columns
 * @param string|null $type
 */
PHP_METHOD(Dao_Db_Index, __construct){

	zval *index_name, *columns, *type = NULL;

	dao_fetch_params(0, 2, 1, &index_name, &columns, &type);

	if (!type) {
		type = &DAO_GLOBAL(z_null);
	}

	dao_update_property(getThis(), SL("_indexName"), index_name);
	dao_update_property(getThis(), SL("_columns"), columns);
	dao_update_property(getThis(), SL("_type"), type);
}

/**
 * Gets the index name
 *
 * @return string
 */
PHP_METHOD(Dao_Db_Index, getName){


	RETURN_MEMBER(getThis(), "_indexName");
}

/**
 * Gets the columns that comprends the index
 *
 * @return array
 */
PHP_METHOD(Dao_Db_Index, getColumns){


	RETURN_MEMBER(getThis(), "_columns");
}

/**
 * Gets the index type
 *
 * @return string
 */
PHP_METHOD(Dao_Db_Index, getType){

	zval type = {};

	dao_read_property(&type, getThis(), SL("_type"), PH_NOISY|PH_READONLY);

	if (Z_TYPE(type) == IS_STRING) {
		RETURN_CTOR(&type);
	} else {
		RETVAL_EMPTY_STRING();
	}
}

/**
 * Restore a Dao\Db\Index object from export
 *
 * @param array $data
 * @return Dao\Db\IndexInterface
 */
PHP_METHOD(Dao_Db_Index, __set_state){

	zval *data, index_name = {}, columns = {};

	dao_fetch_params(0, 1, 0, &data);

	if (!dao_array_isset_fetch_str(&index_name, data, SL("_indexName"), PH_READONLY)) {
		DAO_THROW_EXCEPTION_STR(dao_db_exception_ce, "_indexName parameter is required");
		return;
	}

	if (!dao_array_isset_fetch_str(&columns, data, SL("_columns"), PH_READONLY)) {
		DAO_THROW_EXCEPTION_STR(dao_db_exception_ce, "_columns parameter is required");
		return;
	}

	/**
	 * Return a Dao\Db\Index as part of the returning state
	 */
	object_init_ex(return_value, dao_db_index_ce);
	DAO_CALL_METHOD(NULL, return_value, "__construct", &index_name, &columns);
}
