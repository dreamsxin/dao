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

#include "mvc/model/query/builder/insert.h"
#include "mvc/model/query/builder.h"
#include "mvc/model/query/builderinterface.h"
#include "mvc/model/query/exception.h"
#include "mvc/model/metadatainterface.h"
#include "mvc/model/metadata/memory.h"
#include "mvc/model/query.h"
#include "mvc/model/query/scanner.h"
#include "di.h"
#include "diinterface.h"
#include "di/injectable.h"
#include "db/rawvalue.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/object.h"
#include "kernel/exception.h"
#include "kernel/fcall.h"
#include "kernel/concat.h"
#include "kernel/operators.h"
#include "kernel/string.h"
#include "kernel/file.h"
#include "kernel/hash.h"
#include "kernel/framework/orm.h"

#include "interned-strings.h"

/**
 * Dao\Mvc\Model\Query\Builder\Insert
 *
 *<code>
 *$resultset = Dao\Mvc\Model\Query\Builder::createInsertBuilder()
 *   ->table('Robots')
 *   ->columns(array('name'))
 *   ->values(array(array('name' => 'Google'), array('name' => 'Baidu')))
 *   ->getQuery()
 *   ->execute();
 *</code>
 */
zend_class_entry *dao_mvc_model_query_builder_insert_ce;

PHP_METHOD(Dao_Mvc_Model_Query_Builder_Insert, __construct);
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Insert, table);
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Insert, getTable);
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Insert, columns);
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Insert, getColumns);
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Insert, values);
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Insert, getValues);
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Insert, setConflict);
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Insert, _compile);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_query_builder_insert___construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, params, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_query_builder_insert_table, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, table, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_query_builder_insert_columns, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, columns, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_query_builder_insert_values, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, values, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_query_builder_insert_setconflict, 0, 0, 1)
	ZEND_ARG_INFO(0, conflict)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_mvc_model_query_builder_insert_method_entry[] = {
	PHP_ME(Dao_Mvc_Model_Query_Builder_Insert, __construct, arginfo_dao_mvc_model_query_builder_insert___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Mvc_Model_Query_Builder_Insert, table, arginfo_dao_mvc_model_query_builder_insert_table, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Query_Builder_Insert, getTable, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Query_Builder_Insert, columns, arginfo_dao_mvc_model_query_builder_insert_columns, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Query_Builder_Insert, getColumns, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Query_Builder_Insert, values, arginfo_dao_mvc_model_query_builder_insert_values, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Query_Builder_Insert, getValues, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Query_Builder_Insert, setConflict, arginfo_dao_mvc_model_query_builder_insert_setconflict, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Query_Builder_Insert, _compile, arginfo_empty, ZEND_ACC_PROTECTED)
	PHP_FE_END
};

/**
 * Dao\Mvc\Model\Query\Builder\Insert initializer
 */
DAO_INIT_CLASS(Dao_Mvc_Model_Query_Builder_Insert){

	DAO_REGISTER_CLASS_EX(Dao\\Mvc\\Model\\Query\\Builder, Insert, mvc_model_query_builder_insert, dao_mvc_model_query_builder_ce, dao_mvc_model_query_builder_insert_method_entry, 0);

	zend_declare_property_long(dao_mvc_model_query_builder_insert_ce, SL("_type"), PHQL_T_INSERT, ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_model_query_builder_insert_ce, SL("_table"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_model_query_builder_insert_ce, SL("_columns"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_model_query_builder_insert_ce, SL("_flipColumns"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_model_query_builder_insert_ce, SL("_values"), ZEND_ACC_PROTECTED);
	zend_declare_property_bool(dao_mvc_model_query_builder_insert_ce, SL("_useColumnName"), 0, ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_model_query_builder_insert_ce, SL("_conflict"), ZEND_ACC_PROTECTED);

	zend_class_implements(dao_mvc_model_query_builder_insert_ce, 1, dao_mvc_model_query_builderinterface_ce);

	return SUCCESS;
}

/**
 * Dao\Mvc\Model\Query\Builder\Insert constructor
 *
 * @param array $params
 * @param Dao\Di $dependencyInjector
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Insert, __construct){

	zval *params = NULL, *dependency_injector = NULL, table = {}, columns = {}, values = {};

	dao_fetch_params(0, 0, 2, &params, &dependency_injector);

	/**
	 * Update the dependency injector if any
	 */
	if (dependency_injector && Z_TYPE_P(dependency_injector) != IS_NULL) {
		DAO_CALL_METHOD(NULL, getThis(), "setdi", dependency_injector);
	}

	if (params && Z_TYPE_P(params) == IS_ARRAY) {
		if (dao_array_isset_fetch_str(&table, params, SL("table"), PH_READONLY)) {
			DAO_CALL_METHOD(NULL, getThis(), "settable", &table);
		}

		if (dao_array_isset_fetch_str(&columns, params, SL("columns"), PH_READONLY)) {
			DAO_CALL_METHOD(NULL, getThis(), "setcolumns", &columns);
		}

		if (dao_array_isset_fetch_str(&values, params, SL("values"), PH_READONLY)) {
			DAO_CALL_METHOD(NULL, getThis(), "setvalues", &values);
		}
	}
}

/**
 * Sets the table to insert into
 *
 * @param string table
 * @return Dao\Mvc\Model\Query\Builder\Insert
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Insert, table){

	zval *table;

	dao_fetch_params(0, 1, 0, &table);

	dao_update_property(getThis(), SL("_table"), table);
	RETURN_THIS();
}

/**
 * Gets the table to insert into
 *
 * @return boolean
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Insert, getTable){


	RETURN_MEMBER(getThis(), "_table");
}

/**
 * Set the columns that will be inserted
 *
 * @param array $columns
 * @return Dao\Mvc\Model\Query\Builder\Insert
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Insert, columns){

	zval *columns, flip_columns = {}, *entry, data;
	zend_ulong num_idx;
	zend_string *str_idx;

	dao_fetch_params(1, 1, 0, &columns);

	array_init_size(&flip_columns, zend_hash_num_elements(Z_ARRVAL_P(columns)));
	DAO_MM_ADD_ENTRY(&flip_columns);
	ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(columns), num_idx, str_idx, entry) {
		if (Z_TYPE_P(entry) == IS_LONG) {
			if (str_idx) {
				ZVAL_STR_COPY(&data, str_idx);
			} else {
				ZVAL_LONG(&data, num_idx);
			}
			zend_hash_index_update(Z_ARRVAL(flip_columns), Z_LVAL_P(entry), &data);
		} else if (Z_TYPE_P(entry) == IS_STRING) {
			if (str_idx) {
				ZVAL_STR_COPY(&data, str_idx);
			} else {
				ZVAL_LONG(&data, num_idx);
			}
			zend_symtable_update(Z_ARRVAL(flip_columns), Z_STR_P(entry), &data);
		} else {
			php_error_docref(NULL, E_WARNING, "Columns must be STRING and INTEGER values!");
		}
	} ZEND_HASH_FOREACH_END();

	dao_update_property(getThis(), SL("_columns"), columns);
	dao_update_property(getThis(), SL("_flipColumns"), &flip_columns);

	RETURN_MM_THIS();
}

/**
 * Gets the columns that will be inserted
 *
 * @return string|array
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Insert, getColumns){


	RETURN_MEMBER(getThis(), "_columns");
}

/**
 * Sets the values to insert
 *
 * @param array $values
 * @return Dao\Mvc\Model\Query\Builder\Insert
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Insert, values){

	zval *values, use_columnname = {}, *row;

	dao_fetch_params(0, 1, 0, &values);

	if (!zend_hash_num_elements(Z_ARRVAL_P(values))) {
		DAO_THROW_EXCEPTION_STR(dao_mvc_model_query_exception_ce, "Values must be not empty");
		RETURN_FALSE;
	}

	ZVAL_TRUE(&use_columnname);
	ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(values), row) {
		if (Z_TYPE_P(row) != IS_ARRAY) {
			DAO_THROW_EXCEPTION_STR(dao_mvc_model_query_exception_ce, "Value every row must be array");
			return;
		}

		if (!dao_array_is_associative(row, 1)) {
			ZVAL_FALSE(&use_columnname);
		}
	} ZEND_HASH_FOREACH_END();

	dao_update_property(getThis(), SL("_useColumnName"), &use_columnname);
	dao_update_property(getThis(), SL("_values"), values);
	RETURN_THIS();
}

/**
 * Gets the values to insert
 *
 * @return array
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Insert, getValues){

	RETURN_MEMBER(getThis(), "_values");
}

/**
 * Sets conflict
 *
 * @param array $conflict
 * @return Dao\Mvc\Model\Query\Builder\Insert
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Insert, setConflict){

	zval *conflict;

	dao_fetch_params(0, 1, 0, &conflict);

	dao_update_property(getThis(), SL("_conflict"), conflict);
	RETURN_THIS();
}

/**
 * Returns a PHQL statement built based on the builder parameters
 *
 * @return string
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Insert, _compile){

	zval table = {}, columns = {}, values = {}, phql = {}, joined_columns = {}, hidden_param = {}, use_columnname = {};
	zval *row = NULL, insert_rows = {}, joined_rows = {}, bind_params = {}, bind_types = {};
	zend_string *str_key;
	ulong idx;

	DAO_MM_INIT();

	DAO_MM_CALL_SELF(&table, "gettable");
	DAO_MM_ADD_ENTRY(&table);

	if (DAO_IS_EMPTY(&table)) {
		DAO_MM_THROW_EXCEPTION_STR(dao_mvc_model_query_exception_ce, "Table is required");
		return;
	}

	DAO_MM_CALL_SELF(&columns, "getcolumns");
	DAO_MM_ADD_ENTRY(&columns);

	if (Z_TYPE(columns) != IS_ARRAY || !zend_hash_num_elements(Z_ARRVAL(columns))) {
		DAO_MM_THROW_EXCEPTION_STR(dao_mvc_model_query_exception_ce, "Columns must be array");
		return;
	}

	DAO_MM_CALL_SELF(&values, "getvalues");
	DAO_MM_ADD_ENTRY(&values);

	if (Z_TYPE(values) != IS_ARRAY || !zend_hash_num_elements(Z_ARRVAL(values))) {
		DAO_MM_THROW_EXCEPTION_STR(dao_mvc_model_query_exception_ce, "Values must be array");
		return;
	}

	DAO_MM_CALL_SELF(&bind_params, "getbindparams");
	DAO_MM_ADD_ENTRY(&bind_params);
	if (Z_TYPE(bind_params) != IS_ARRAY) {
		array_init(&bind_params);
		DAO_MM_ADD_ENTRY(&bind_params);
	}

	DAO_CONCAT_SVS(&phql, "INSERT INTO [", &table, "]");

	dao_fast_join_str(&joined_columns, SL("], ["), &columns);
	DAO_SCONCAT_SVS(&phql, " ([", &joined_columns, "]) VALUES ");
	zval_ptr_dtor(&joined_columns);

	dao_read_property(&hidden_param, getThis(), SL("_hiddenParamNumber"), PH_READONLY);
	dao_read_property(&use_columnname, getThis(), SL("_useColumnName"), PH_READONLY);

	array_init(&insert_rows);
	DAO_MM_ADD_ENTRY(&insert_rows);

	if (zend_is_true(&use_columnname)) {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL(values), row) {
			zval *column, insert_values = {}, joined_values = {};

			array_init(&insert_values);

			ZEND_HASH_FOREACH_VAL(Z_ARRVAL(columns), column) {
				zval key = {}, insert_value = {}, value = {};
				DAO_CONCAT_SVSV(&key, "phi_", &hidden_param, "_", column);
				DAO_CONCAT_SVS(&insert_value, " :", &key, ":");
				dao_array_append(&insert_values, &insert_value, 0);
				if (dao_array_isset_fetch(&value, row, column, PH_READONLY)) {
					dao_array_update(&bind_params, &key, &value, PH_COPY);
				} else {
					dao_array_update(&bind_params, &key, &DAO_GLOBAL(z_null), 0);
				}
				zval_ptr_dtor(&key);
			} ZEND_HASH_FOREACH_END();
			dao_increment(&hidden_param);
			dao_fast_join_str(&joined_values, SL(", "), &insert_values);
			zval_ptr_dtor(&insert_values);
			dao_array_append(&insert_rows, &joined_values, 0);
		} ZEND_HASH_FOREACH_END();
	} else {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL(values), row) {
			zval insert_values = {}, *value, joined_values = {};

			array_init(&insert_values);
			ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(row), idx, str_key, value) {
				zval column = {}, key = {}, insert_value = {};
				if (str_key) {
					ZVAL_STR(&column, str_key);
				} else {
					ZVAL_LONG(&column, idx);
				}

				DAO_CONCAT_SVSV(&key, "phi_", &hidden_param, "_", &column);
				DAO_CONCAT_SVS(&insert_value, " :", &key, ":");

				dao_array_append(&insert_values, &insert_value, 0);
				dao_array_update(&bind_params, &key, value, PH_COPY);
				zval_ptr_dtor(&key);
			} ZEND_HASH_FOREACH_END();
			dao_increment(&hidden_param);
			dao_fast_join_str(&joined_values, SL(", "), &insert_values);
			zval_ptr_dtor(&insert_values);
			dao_array_append(&insert_rows, &joined_values, 0);
		} ZEND_HASH_FOREACH_END();
	}

	dao_fast_join_str(&joined_rows, SL("), ("), &insert_rows);

	DAO_SCONCAT_SVS(&phql, "(", &joined_rows, ")");
	zval_ptr_dtor(&joined_rows);
	DAO_MM_ADD_ENTRY(&phql);

	dao_update_property(getThis(), SL("_mergeBindParams"), &bind_params);

	DAO_MM_CALL_SELF(&bind_types, "getbindtypes");
	DAO_MM_ADD_ENTRY(&bind_types);
	dao_update_property(getThis(), SL("_mergeBindTypes"), &bind_types);


	dao_update_property(getThis(), SL("_phql"), &phql);
	RETURN_MM();
}
