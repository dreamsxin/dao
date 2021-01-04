
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

#include "mvc/model/metadata/strategy/introspection.h"
#include "mvc/model/metadata.h"
#include "mvc/model/exception.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"
#include "kernel/object.h"
#include "kernel/concat.h"
#include "kernel/exception.h"
#include "kernel/file.h"
#include "kernel/array.h"
#include "kernel/operators.h"
#include "kernel/hash.h"

/**
 * Dao\Mvc\Model\MetaData\Strategy\Instrospection
 *
 * Queries the table meta-data in order to instrospect the model's metadata
 */
zend_class_entry *dao_mvc_model_metadata_strategy_introspection_ce;

PHP_METHOD(Dao_Mvc_Model_MetaData_Strategy_Introspection, getMetaData);
PHP_METHOD(Dao_Mvc_Model_MetaData_Strategy_Introspection, getColumnMaps);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_metadata_strategy_introspection_getmetadata, 0, 0, 2)
	ZEND_ARG_INFO(0, model)
	ZEND_ARG_INFO(0, dependencyInjector)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_metadata_strategy_introspection_getcolumnmaps, 0, 0, 2)
	ZEND_ARG_INFO(0, model)
	ZEND_ARG_INFO(0, dependencyInjector)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_mvc_model_metadata_strategy_introspection_method_entry[] = {
	PHP_ME(Dao_Mvc_Model_MetaData_Strategy_Introspection, getMetaData, arginfo_dao_mvc_model_metadata_strategy_introspection_getmetadata, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_MetaData_Strategy_Introspection, getColumnMaps, arginfo_dao_mvc_model_metadata_strategy_introspection_getcolumnmaps, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Mvc\Model\MetaData\Strategy\Introspection initializer
 */
DAO_INIT_CLASS(Dao_Mvc_Model_MetaData_Strategy_Introspection){

	DAO_REGISTER_CLASS(Dao\\Mvc\\Model\\MetaData\\Strategy, Introspection, mvc_model_metadata_strategy_introspection, dao_mvc_model_metadata_strategy_introspection_method_entry, 0);

	return SUCCESS;
}

/**
 * The meta-data is obtained by reading the column descriptions from the database information schema
 *
 * @param Dao\Mvc\ModelInterface $model
 * @param Dao\DiInterface $dependencyInjector
 * @return array
 */
PHP_METHOD(Dao_Mvc_Model_MetaData_Strategy_Introspection, getMetaData){

	zval *model, *dependency_injector, class_name = {}, schema = {}, table = {}, read_connection = {}, exists = {}, complete_table = {}, exception_message = {};
	zval columns = {}, attributes = {}, primary_keys = {}, non_primary_keys = {}, numeric_typed = {}, not_null = {}, field_types = {}, field_sizes = {}, field_bytes = {}, field_scales = {};
	zval field_bind_types = {}, automatic_create_attributes = {}, automatic_update_attributes = {}, field_default_values = {}, identity_field = {}, *column;

	dao_fetch_params(1, 2, 0, &model, &dependency_injector);

	dao_get_class(&class_name, model, 0);
	DAO_MM_ADD_ENTRY(&class_name);
	DAO_MM_CALL_METHOD(&schema, model, "getschema");
	DAO_MM_ADD_ENTRY(&schema);
	DAO_MM_CALL_METHOD(&table, model, "getsource");
	DAO_MM_ADD_ENTRY(&table);

	/**
	 * Check if the mapped table exists on the database
	 */
	DAO_MM_CALL_METHOD(&read_connection, model, "getreadconnection");
	DAO_MM_ADD_ENTRY(&read_connection);
	DAO_MM_CALL_METHOD(&exists, &read_connection, "tableexists", &table, &schema);
	DAO_MM_ADD_ENTRY(&exists);

	if (!zend_is_true(&exists)) {
		if (zend_is_true(&schema)) {
			DAO_CONCAT_VSV(&complete_table, &schema, "\".\"", &table);
			DAO_MM_ADD_ENTRY(&complete_table);
		} else {
			ZVAL_COPY_VALUE(&complete_table, &table);
		}

		/**
		 * The table not exists
		 */
		DAO_CONCAT_SVSV(&exception_message, "Table \"", &complete_table, "\" doesn't exist on database when dumping meta-data for ", &class_name);
		DAO_MM_ADD_ENTRY(&exception_message);
		DAO_MM_THROW_EXCEPTION_ZVAL(dao_mvc_model_exception_ce, &exception_message);
		return;
	}

	/**
	 * Try to describe the table
	 */
	DAO_MM_CALL_METHOD(&columns, &read_connection, "describecolumns", &table, &schema);
	DAO_MM_ADD_ENTRY(&columns);

	if (!dao_fast_count_ev(&columns)) {
		if (zend_is_true(&schema)) {
			DAO_CONCAT_VSV(&complete_table, &schema, "\".\"", &table);
			DAO_MM_ADD_ENTRY(&complete_table);
		} else {
			ZVAL_COPY_VALUE(&complete_table, &table);
		}

		/**
		 * The table not exists
		 */
		DAO_CONCAT_SVSV(&exception_message, "Cannot obtain table columns for the mapped source \"", &complete_table, "\" used in model ", &class_name);
		DAO_MM_ADD_ENTRY(&exception_message);
		DAO_MM_THROW_EXCEPTION_ZVAL(dao_mvc_model_exception_ce, &exception_message);
		return;
	}

	/**
	 * Initialize meta-data
	 */
	array_init(&attributes);
	DAO_MM_ADD_ENTRY(&attributes);
	array_init(&primary_keys);
	DAO_MM_ADD_ENTRY(&primary_keys);
	array_init(&non_primary_keys);
	DAO_MM_ADD_ENTRY(&non_primary_keys);
	array_init(&numeric_typed);
	DAO_MM_ADD_ENTRY(&numeric_typed);
	array_init(&not_null);
	DAO_MM_ADD_ENTRY(&not_null);
	array_init(&field_types);
	DAO_MM_ADD_ENTRY(&field_types);
	array_init(&field_sizes);
	DAO_MM_ADD_ENTRY(&field_sizes);
	array_init(&field_bytes);
	DAO_MM_ADD_ENTRY(&field_bytes);
	array_init(&field_scales);
	DAO_MM_ADD_ENTRY(&field_scales);
	array_init(&field_bind_types);
	DAO_MM_ADD_ENTRY(&field_bind_types);
	array_init(&automatic_create_attributes);
	DAO_MM_ADD_ENTRY(&automatic_create_attributes);
	array_init(&automatic_update_attributes);
	DAO_MM_ADD_ENTRY(&automatic_update_attributes);
	array_init(&field_default_values);
	DAO_MM_ADD_ENTRY(&field_default_values);

	ZVAL_FALSE(&identity_field);

	ZEND_HASH_FOREACH_VAL(Z_ARRVAL(columns), column) {
		zval field_name = {}, feature = {}, type = {}, size = {}, bytes = {}, scale = {}, bind_type = {}, default_value = {};

		DAO_MM_CALL_METHOD(&field_name, column, "getname");
		DAO_MM_ADD_ENTRY(&field_name);
		dao_array_append(&attributes, &field_name, PH_COPY);

		/**
		 * To mark fields as primary keys
		 */
		DAO_MM_CALL_METHOD(&feature, column, "isprimary");
		if (DAO_IS_TRUE(&feature)) {
			dao_array_append(&primary_keys, &field_name, PH_COPY);
		} else {
			dao_array_append(&non_primary_keys, &field_name, PH_COPY);
		}

		/**
		 * To mark fields as numeric
		 */
		DAO_MM_CALL_METHOD(&feature, column, "isnumeric");
		if (DAO_IS_TRUE(&feature)) {
			dao_array_update_zval_bool(&numeric_typed, &field_name, 1, 0);
		}

		/**
		 * To mark fields as not null
		 */
		DAO_MM_CALL_METHOD(&feature, column, "isnotnull");
		if (DAO_IS_TRUE(&feature)) {
			dao_array_append(&not_null, &field_name, PH_COPY);
		}

		/**
		 * To mark fields as identity columns
		 */
		DAO_MM_CALL_METHOD(&feature, column, "isautoincrement");
		if (DAO_IS_TRUE(&feature) && !zend_is_true(&identity_field)) {
			DAO_MM_ZVAL_COPY(&identity_field, &field_name);
		}

		/**
		 * To get the internal types
		 */
		DAO_MM_CALL_METHOD(&type, column, "gettype");
		dao_array_update(&field_types, &field_name, &type, 0);

		/**
		 * To get the internal size
		 */
		DAO_MM_CALL_METHOD(&size, column, "getsize");
		dao_array_update(&field_sizes, &field_name, &size, 0);

		/**
		 * To get the internal bytes
		 */
		DAO_MM_CALL_METHOD(&bytes, column, "getbytes");
		dao_array_update(&field_bytes, &field_name, &bytes, 0);

		/**
		 * To get the internal scale
		 */
		DAO_MM_CALL_METHOD(&scale, column, "getscale");
		dao_array_update(&field_scales, &field_name, &scale, 0);

		/**
		 * To mark how the fields must be escaped
		 */
		DAO_MM_CALL_METHOD(&bind_type, column, "getbindtype");
		dao_array_update(&field_bind_types, &field_name, &bind_type, 0);

		DAO_MM_CALL_METHOD(&default_value, column, "getdefaultvalue");
		if (Z_TYPE(default_value) != IS_NULL) {
			dao_array_update(&field_default_values, &field_name, &default_value, 0);
		}
	} ZEND_HASH_FOREACH_END();

	/**
	 * Create an array using the MODELS_* constants as indexes
	 */
	array_init_size(return_value, 14);
	dao_array_update_long(return_value, DAO_MVC_MODEL_METADATA_MODELS_ATTRIBUTES, &attributes, PH_COPY);
	dao_array_update_long(return_value, DAO_MVC_MODEL_METADATA_MODELS_PRIMARY_KEY, &primary_keys, PH_COPY);
	dao_array_update_long(return_value, DAO_MVC_MODEL_METADATA_MODELS_NON_PRIMARY_KEY, &non_primary_keys, PH_COPY);
	dao_array_update_long(return_value, DAO_MVC_MODEL_METADATA_MODELS_NOT_NULL, &not_null, PH_COPY);
	dao_array_update_long(return_value, DAO_MVC_MODEL_METADATA_MODELS_DATA_TYPES, &field_types, PH_COPY);
	dao_array_update_long(return_value, DAO_MVC_MODEL_METADATA_MODELS_DATA_TYPES_NUMERIC, &numeric_typed, PH_COPY);
	dao_array_update_long(return_value, DAO_MVC_MODEL_METADATA_MODELS_IDENTITY_COLUMN, &identity_field, PH_COPY);
	dao_array_update_long(return_value, DAO_MVC_MODEL_METADATA_MODELS_DATA_TYPES_BIND, &field_bind_types, PH_COPY);
	dao_array_update_long(return_value, DAO_MVC_MODEL_METADATA_MODELS_AUTOMATIC_DEFAULT_INSERT, &automatic_create_attributes, PH_COPY);
	dao_array_update_long(return_value, DAO_MVC_MODEL_METADATA_MODELS_AUTOMATIC_DEFAULT_UPDATE, &automatic_update_attributes, PH_COPY);
	dao_array_update_long(return_value, DAO_MVC_MODEL_METADATA_MODELS_DATA_DEFAULT_VALUE, &field_default_values, PH_COPY);
	dao_array_update_long(return_value, DAO_MVC_MODEL_METADATA_MODELS_DATA_SZIE, &field_sizes, PH_COPY);
	dao_array_update_long(return_value, DAO_MVC_MODEL_METADATA_MODELS_DATA_SCALE, &field_scales, PH_COPY);
	dao_array_update_long(return_value, DAO_MVC_MODEL_METADATA_MODELS_DATA_BYTE, &field_bytes, PH_COPY);
	RETURN_MM();
}

/**
 * Read the model's column map, this can't be infered
 *
 * @param Dao\Mvc\ModelInterface $model
 * @param Dao\DiInterface $dependencyInjector
 * @return array
 */
PHP_METHOD(Dao_Mvc_Model_MetaData_Strategy_Introspection, getColumnMaps){

	zval *model, *dependency_injector, ordered_column_map = {}, columns = {}, *column_name, reversed_column_map = {};
	zend_string *str_key;
	ulong idx;

	dao_fetch_params(1, 2, 0, &model, &dependency_injector);

	/**
	 * Check for a columnMap() method on the model
	 */
	if (dao_method_exists_ex(model, SL("columnmap")) == SUCCESS) {
		DAO_MM_CALL_METHOD(&ordered_column_map, model, "columnmap");
		DAO_MM_ADD_ENTRY(&ordered_column_map);
		if (Z_TYPE(ordered_column_map) != IS_ARRAY) {
			DAO_MM_THROW_EXCEPTION_STR(dao_mvc_model_exception_ce, "columnMap() not returned an array");
			return;
		}
	} else {
		array_init(&ordered_column_map);
		DAO_MM_ADD_ENTRY(&ordered_column_map);
	}
	array_init(&reversed_column_map);
	DAO_MM_ADD_ENTRY(&reversed_column_map);

	DAO_MM_CALL_METHOD(&columns, model, "getattributes");
	DAO_MM_ADD_ENTRY(&columns);

	if (Z_TYPE(columns) == IS_ARRAY) {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL(columns), column_name) {
			if (!dao_array_isset(&ordered_column_map, column_name)) {
				dao_array_update(&ordered_column_map, column_name, column_name, PH_COPY);
			}
		} ZEND_HASH_FOREACH_END();
	}

	ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL(ordered_column_map), idx, str_key, column_name) {
		zval name = {};
		if (str_key) {
			ZVAL_STR(&name, str_key);
		} else {
			ZVAL_LONG(&name, idx);
		}
		dao_array_update(&reversed_column_map, column_name, &name, PH_COPY);
	} ZEND_HASH_FOREACH_END();


	/**
	 * Store the column map
	 */
	array_init_size(return_value, 2);
	dao_array_update_long(return_value, DAO_MVC_MODEL_METADATA_MODELS_COLUMN_MAP, &ordered_column_map, PH_COPY);
	dao_array_update_long(return_value, DAO_MVC_MODEL_METADATA_MODELS_REVERSE_COLUMN_MAP, &reversed_column_map, PH_COPY);
	RETURN_MM();
}
