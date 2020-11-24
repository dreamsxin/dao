
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

#include "mvc/model/resultset/complex.h"
#include "mvc/model/resultset.h"
#include "mvc/model/resultsetinterface.h"
#include "mvc/model/row.h"
#include "mvc/model/exception.h"
#include "mvc/model.h"

#include <ext/pdo/php_pdo_driver.h>

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "kernel/array.h"
#include "kernel/hash.h"
#include "kernel/concat.h"
#include "kernel/string.h"
#include "kernel/variables.h"
#include "kernel/exception.h"

#include "interned-strings.h"

/**
 * Dao\Mvc\Model\Resultset\Complex
 *
 * Complex resultsets may include complete objects and scalar values.
 * This class builds every complex row as it is required
 */
zend_class_entry *dao_mvc_model_resultset_complex_ce;

PHP_METHOD(Dao_Mvc_Model_Resultset_Complex, __construct);
PHP_METHOD(Dao_Mvc_Model_Resultset_Complex, valid);
PHP_METHOD(Dao_Mvc_Model_Resultset_Complex, toArray);
PHP_METHOD(Dao_Mvc_Model_Resultset_Complex, serialize);
PHP_METHOD(Dao_Mvc_Model_Resultset_Complex, unserialize);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_resultset_complex___construct, 0, 0, 2)
	ZEND_ARG_INFO(0, columnsTypes)
	ZEND_ARG_INFO(0, result)
	ZEND_ARG_INFO(0, cache)
	ZEND_ARG_INFO(0, sourceModel)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_mvc_model_resultset_complex_method_entry[] = {
	PHP_ME(Dao_Mvc_Model_Resultset_Complex, __construct, arginfo_dao_mvc_model_resultset_complex___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Mvc_Model_Resultset_Complex, valid, arginfo_iterator_valid, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Resultset_Complex, toArray, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Resultset_Complex, serialize, arginfo_serializable_serialize, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Resultset_Complex, unserialize, arginfo_serializable_unserialize, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Mvc\Model\Resultset\Complex initializer
 */
DAO_INIT_CLASS(Dao_Mvc_Model_Resultset_Complex){

	DAO_REGISTER_CLASS_EX(Dao\\Mvc\\Model\\Resultset, Complex, mvc_model_resultset_complex, dao_mvc_model_resultset_ce, dao_mvc_model_resultset_complex_method_entry, 0);

	zend_declare_property_null(dao_mvc_model_resultset_complex_ce, SL("_sourceModel"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_model_resultset_complex_ce, SL("_columnTypes"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_model_resultset_complex_ce, SL("_rowsModels"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_model_resultset_complex_ce, SL("_rowsObjects"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_model_resultset_complex_ce, SL("_rowsArrays"), ZEND_ACC_PROTECTED);

	return SUCCESS;
}

/**
 * Dao\Mvc\Model\Resultset\Complex constructor
 *
 * @param Dao\Mvc\ModelInterface $sourceModel
 * @param array $columnsTypes
 * @param Dao\Db\ResultInterface $result
 * @param Dao\Cache\BackendInterface $cache
 */
PHP_METHOD(Dao_Mvc_Model_Resultset_Complex, __construct){

	zval *columns_types, *result, *cache = NULL, *source_model = NULL, *count = NULL, fetch_assoc = {};

	dao_fetch_params(0, 2, 3, &columns_types, &result, &cache, &source_model, &count);

	if (!cache) {
		cache = &DAO_GLOBAL(z_null);
	}

	if (!source_model) {
		source_model = &DAO_GLOBAL(z_null);
	}

	/**
	 * Column types, tell the resultset how to build the result
	 */
	dao_update_property(getThis(), SL("_columnTypes"), columns_types);

	/**
	 * Valid resultsets are Dao\Db\ResultInterface instances
	 * FIXME: or Dao\Db\Result\Pdo?
	 */
	dao_update_property(getThis(), SL("_result"), result);

	/**
	 * Update the related cache if any
	 */
	if (Z_TYPE_P(cache) != IS_NULL) {
		dao_update_property(getThis(), SL("_cache"), cache);
	}

	dao_update_property(getThis(), SL("_sourceModel"), source_model);

	/**
	 * Resultsets type 1 are traversed one-by-one
	 */
	dao_update_property_long(getThis(), SL("_type"), DAO_MVC_MODEL_RESULTSET_TYPE_PARTIAL);

	/**
	 * If the database result is an object, change it to fetch assoc
	 */
	if (Z_TYPE_P(result) == IS_OBJECT) {
		ZVAL_LONG(&fetch_assoc, PDO_FETCH_ASSOC);
		DAO_CALL_METHOD(NULL, result, "setfetchmode", &fetch_assoc);
	}
}

/**
 * Check whether internal resource has rows to fetch
 *
 * @return boolean
 */
PHP_METHOD(Dao_Mvc_Model_Resultset_Complex, valid){

	zval key = {}, source_model = {}, type = {}, row = {}, rows = {}, hydrate_mode = {}, columns_types = {}, underscore = {}, empty_str = {};
	zval dependency_injector = {}, service_name = {}, has = {}, active_row = {}, dirty_state = {}, *column;
	zend_class_entry *ce;
	zend_string *str_key;
	ulong idx;
	int i_type, is_partial, i_hydrate_mode;

	DAO_MM_INIT();

	DAO_MM_CALL_SELF(&key, "key");
	DAO_MM_ADD_ENTRY(&key);

	dao_read_property(&source_model, getThis(), SL("_sourceModel"), PH_NOISY|PH_READONLY);
	dao_read_property(&type, getThis(), SL("_type"), PH_NOISY|PH_READONLY);
	i_type = (Z_TYPE(type) == IS_LONG) ? Z_LVAL(type) : dao_get_intval(&type);
	is_partial = (i_type == DAO_MVC_MODEL_RESULTSET_TYPE_PARTIAL);

	if (Z_TYPE(source_model) == IS_OBJECT) {
		ce = Z_OBJCE(source_model);
	} else {
		ce = dao_mvc_model_ce;
	}

	if (is_partial) {
		/**
		 * The result is bigger than 32 rows so it's retrieved one by one
		 */

		if (!dao_property_array_isset_fetch(&row, getThis(), SL("_rows"), &key, PH_READONLY)) {
			zval result = {};
			dao_read_property(&result, getThis(), SL("_result"), PH_NOISY|PH_READONLY);
			if (Z_TYPE(result) == IS_OBJECT) {
				DAO_MM_CALL_METHOD(&row, &result, "fetch");
				DAO_MM_ADD_ENTRY(&row);
			} else {
				ZVAL_FALSE(&row);
			}
			dao_update_property_array(getThis(), SL("_rows"), &key, &row);
		}
	} else {
		/**
		 * The full rows are dumped in this_ptr->rows
		 */
		dao_read_property(&rows, getThis(), SL("_rows"), PH_READONLY);
		if (Z_TYPE(rows) != IS_ARRAY) {
			zval result = {};
			dao_read_property(&result, getThis(), SL("_result"), PH_NOISY|PH_READONLY);
			if (Z_TYPE(result) == IS_OBJECT) {
				DAO_MM_CALL_METHOD(&rows, &result, "fetchall");
				DAO_MM_ADD_ENTRY(&rows);
				dao_update_property(getThis(), SL("_rows"), &rows);
			}
		}
		if (Z_TYPE(rows) == IS_ARRAY) {
			dao_array_get_current(&row, &rows);
			if (Z_TYPE(row) == IS_OBJECT) {
				zend_hash_move_forward(Z_ARRVAL(rows));
			}
		} else {
			ZVAL_FALSE(&row);
		}
	}

	/**
	 * Valid records are arrays
	 */
	if (Z_TYPE(row) == IS_ARRAY || Z_TYPE(row) == IS_OBJECT) {

		/**
		 * The result type=1 so we need to build every row
		 */
		if (!is_partial) {
			/**
			 * The row is already built so we just assign it to the activeRow
			 */
			dao_update_property(getThis(), SL("_activeRow"), &row);
			RETURN_MM_TRUE;
		}

		/**
		 * Get current hydration mode
		 */
		dao_read_property(&hydrate_mode, getThis(), SL("_hydrateMode"), PH_NOISY|PH_READONLY);
		dao_read_property(&columns_types, getThis(), SL("_columnTypes"), PH_NOISY|PH_READONLY);
		i_hydrate_mode  = dao_get_intval(&hydrate_mode);

		DAO_MM_ZVAL_STRING(&underscore, "_");
		DAO_MM_ZVAL_EMPTY_STRING(&empty_str);

		/**
		 * Each row in a complex result is a Dao\Mvc\Model\Row instance
		 */
		switch (i_hydrate_mode) {
			case 0:
			{
				zval tmp = {};
				dao_read_property(&tmp, getThis(), SL("_rowsModels"), PH_NOISY|PH_READONLY);
				if (dao_array_isset_fetch(&active_row, &tmp, &key, PH_READONLY)) {
					dao_update_property(getThis(), SL("_activeRow"), &active_row);
					RETURN_MM_TRUE;
				}
				ZVAL_STR(&service_name, IS(modelsRow));

				DAO_MM_CALL_METHOD(&dependency_injector, getThis(), "getdi");
				DAO_MM_ADD_ENTRY(&dependency_injector);
				DAO_MM_CALL_METHOD(&has, &dependency_injector, "has", &service_name);
				if (zend_is_true(&has)) {
					DAO_MM_CALL_METHOD(&active_row, &dependency_injector, "get", &service_name);
					DAO_MM_ADD_ENTRY(&active_row);
					DAO_MM_VERIFY_CLASS_EX(&active_row, dao_mvc_model_row_ce, dao_mvc_model_row_ce);
				} else {
					object_init_ex(&active_row, dao_mvc_model_row_ce);
					DAO_MM_ADD_ENTRY(&active_row);
				}
				break;

			}
			case 1:
			{
				zval tmp = {};
				dao_read_property(&tmp, getThis(), SL("_rowsArrays"), PH_NOISY|PH_READONLY);
				if (dao_array_isset_fetch(&active_row, &tmp, &key, PH_READONLY)) {
					dao_update_property(getThis(), SL("_activeRow"), &active_row);
					RETURN_MM_TRUE;
				}
				array_init(&active_row);
				DAO_MM_ADD_ENTRY(&active_row);
				break;
			}
			case 2:
			default:
			{
				zval tmp = {};
				dao_read_property(&tmp, getThis(), SL("_rowsObjects"), PH_NOISY|PH_READONLY);
				if (dao_array_isset_fetch(&active_row, &tmp, &key, PH_READONLY)) {
					dao_update_property(getThis(), SL("_activeRow"), &active_row);
					RETURN_MM_TRUE;
				}
				object_init(&active_row);
				DAO_MM_ADD_ENTRY(&active_row);
				break;
			}
		}

		/**
		 * Create every record according to the column types
		 */

		/**
		 * Set records as dirty state PERSISTENT by default
		 */
		ZVAL_LONG(&dirty_state, 0);

		ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL(columns_types), idx, str_key, column) {
			zval alias = {}, n_alias = {}, column_type = {}, source = {}, attributes = {}, column_map = {}, row_model = {}, *attribute, sql_alias = {}, value = {};

			dao_array_fetch_str(&column_type, column, SL("type"), PH_NOISY|PH_READONLY);

			if (DAO_IS_STRING(&column_type, "object")) {

				/**
				 * Object columns are assigned column by column
				 */
				dao_array_fetch_str(&source, column, SL("column"), PH_NOISY|PH_READONLY);
				dao_array_fetch_str(&attributes, column, SL("attributes"), PH_NOISY|PH_READONLY);
				dao_array_fetch_str(&column_map, column, SL("columnMap"), PH_NOISY|PH_READONLY);

				/**
				 * Assign the values from the _source_attribute notation to its real column name
				 */
				array_init(&row_model);
				DAO_MM_ADD_ENTRY(&row_model);
				ZEND_HASH_FOREACH_VAL(Z_ARRVAL(attributes), attribute) {
					zval column_alias = {}, column_value = {};
					/**
					 * Columns are supposed to be in the form _table_field
					 */
					DAO_CONCAT_VVVV(&column_alias, &underscore, &source, &underscore, attribute);

					if (!dao_array_isset_fetch(&column_value, &row, &column_alias, PH_NOISY|PH_READONLY)) {
						ZVAL_NULL(&column_value);
					}

					dao_array_update(&row_model, attribute, &column_value, PH_COPY);
					zval_ptr_dtor(&column_alias);
				} ZEND_HASH_FOREACH_END();

				/**
				 * Generate the column value according to the hydration type
				 */
				switch (i_hydrate_mode) {

					case 0: {
						zval instance = {};

						/**
						 * Get the base instance
						 */
						if (!dao_array_isset_fetch_str(&instance, column, SL("instance"), PH_READONLY)) {
							php_error_docref(NULL, E_NOTICE, "Undefined index: instance");
							ZVAL_NULL(&instance);
						}

						/**
						 * Assign the values to the attributes using a column map
						 */
						DAO_MM_CALL_CE_STATIC(&value, ce, "cloneresultmap", &instance, &row_model, &column_map, &dirty_state, &source_model);
						DAO_MM_ADD_ENTRY(&value);
						break;
					}

					default:
						/**
						 * Other kinds of hydrations
						 */
						DAO_MM_CALL_CE_STATIC(&value, ce, "cloneresultmaphydrate", &row_model, &column_map, &hydrate_mode, &source_model);
						DAO_MM_ADD_ENTRY(&value);
						break;
				}

				/**
				 * The complete object is assigned to an attribute with the name of the alias or
				 * the model name
				 */
				if (!dao_array_isset_fetch_str(&alias, column, SL("balias"), PH_READONLY)) {
					ZVAL_NULL(&alias);
				}
			} else {
				if (str_key) {
					ZVAL_STR(&alias, str_key);
				} else {
					ZVAL_LONG(&alias, idx);
				}

				/**
				 * Scalar columns are simply assigned to the result object
				 */
				if (dao_array_isset_fetch_str(&sql_alias, column, SL("sqlAlias"), PH_READONLY)) {
					if (!dao_array_isset_fetch(&value, &row, &sql_alias, PH_READONLY)) {
						ZVAL_NULL(&value);
					}
				} else if (!dao_array_isset_fetch(&value, &row, &alias, PH_READONLY)) {
					ZVAL_NULL(&value);
				}

				/**
				 * If a 'balias' is defined is not an unnamed scalar
				 */
				if (!dao_array_isset_str(column, SL("balias"))) {
					DAO_STR_REPLACE(&n_alias, &underscore, &empty_str, &alias);
					DAO_MM_ADD_ENTRY(&n_alias);
					ZVAL_COPY_VALUE(&alias, &n_alias);
				}
			}

			/**
			 * Assign the instance according to the hydration type
			 */
			if (unlikely(Z_TYPE(alias) == IS_NULL)) {
				zend_throw_exception_ex(dao_mvc_model_exception_ce, 0, "Unexpected inconsistency: attribute is NULL");
				RETURN_MM();
			}

			switch (i_hydrate_mode) {

				case 1:
					dao_array_update(&active_row, &alias, &value, PH_COPY);
					break;

				default:
					dao_update_property_zval_zval(&active_row, &alias, &value);
					break;

			}
		} ZEND_HASH_FOREACH_END();

		if (Z_TYPE(active_row) == IS_OBJECT && dao_method_exists_ex(&active_row, SL("afterfetch")) == SUCCESS) {
			DAO_MM_CALL_METHOD(NULL, &active_row, "afterfetch");
		}
		switch (i_hydrate_mode) {
			case 0:
			{
				dao_update_property_array(getThis(), SL("_rowsModels"), &key, &active_row);
				break;
			}
			case 1:
			{
				dao_update_property_array(getThis(), SL("_rowsArrays"), &key, &active_row);
				break;
			}
			case 2:
			default:
			{
				dao_update_property_array(getThis(), SL("_rowsObjects"), &key, &active_row);
				break;
			}
		}
		/**
		 * Store the generated row in this_ptr->activeRow to be retrieved by 'current'
		 */
		dao_update_property(getThis(), SL("_activeRow"), &active_row);
		RETURN_MM_TRUE;
	}

	/**
	 * There are no results to retrieve so we update this_ptr->activeRow as false
	 */
	dao_update_property_bool(getThis(), SL("_activeRow"), 0);
	RETURN_MM_FALSE;
}

/**
 * Returns a complete resultset as an array, if the resultset has a big number of rows
 * it could consume more memory than currently it does.
 *
 * @return array
 */
PHP_METHOD(Dao_Mvc_Model_Resultset_Complex, toArray){

	array_init(return_value);

	DAO_MM_INIT();

	DAO_MM_CALL_METHOD(NULL, getThis(), "rewind");

	while (1) {
		zval valid = {}, current = {}, arr = {};

		DAO_MM_CALL_METHOD(&valid, getThis(), "valid");
		if (!DAO_IS_NOT_FALSE(&valid)) {
			break;
		}

		DAO_MM_CALL_METHOD(&current, getThis(), "current");
		DAO_MM_ADD_ENTRY(&current);
		if (Z_TYPE(current) == IS_OBJECT && dao_method_exists_ex(&current, SL("toarray")) == SUCCESS) {
			DAO_MM_CALL_METHOD(&arr, &current, "toarray");
			dao_array_append(return_value, &arr, 0);
		} else {
			dao_array_append(return_value, &current, PH_COPY);
		}
		DAO_MM_CALL_METHOD(NULL, getThis(), "next");
	}
	RETURN_MM();
}

/**
 * Serializing a resultset will dump all related rows into a big array
 *
 * @return string
 */
PHP_METHOD(Dao_Mvc_Model_Resultset_Complex, serialize){

	zval records = {}, count = {}, cache = {}, column_types = {}, hydrate_mode = {}, data = {};

	/**
	 * Obtain the records as an array
	 */
	DAO_CALL_METHOD(&records, getThis(), "toarray");
	dao_fast_count(&count, &records);

	dao_read_property(&cache, getThis(), SL("_cache"), PH_NOISY|PH_READONLY);
	dao_read_property(&column_types, getThis(), SL("_columnTypes"), PH_NOISY|PH_READONLY);
	dao_read_property(&hydrate_mode, getThis(), SL("_hydrateMode"), PH_NOISY|PH_READONLY);

	array_init_size(&data, 4);
	dao_array_update_str(&data, SL("cache"), &cache, PH_COPY);
	dao_array_update_str(&data, SL("rows"), &records, 0);
	dao_array_update_str(&data, SL("count"), &count, 0);
	dao_array_update_str(&data, SL("columnTypes"), &column_types, PH_COPY);
	dao_array_update_str(&data, SL("hydrateMode"), &hydrate_mode, PH_COPY);

	dao_serialize(return_value, &data);
	zval_ptr_dtor(&data);
}

/**
 * Unserializing a resultset will allow to only works on the rows present in the saved state
 *
 * @param string $data
 */
PHP_METHOD(Dao_Mvc_Model_Resultset_Complex, unserialize){

	zval *data, resultset = {}, rows = {}, count = {}, cache = {}, column_types = {}, hydrate_mode = {};

	dao_fetch_params(0, 1, 0, &data);

	dao_update_property_long(getThis(), SL("_type"), DAO_MVC_MODEL_RESULTSET_TYPE_FULL);

	dao_unserialize(&resultset, data);
	if (Z_TYPE(resultset) != IS_ARRAY) {
		DAO_THROW_EXCEPTION_STR(dao_mvc_model_exception_ce, "Invalid serialization data");
		return;
	}

	dao_array_fetch_str(&rows, &resultset, SL("rows"), PH_NOISY|PH_READONLY);
	dao_update_property(getThis(), SL("_rows"), &rows);

	dao_array_fetch_str(&rows, &resultset, SL("count"), PH_NOISY|PH_READONLY);
	dao_update_property(getThis(), SL("_count"), &count);

	dao_array_fetch_str(&cache, &resultset, SL("cache"), PH_NOISY|PH_READONLY);
	dao_update_property(getThis(), SL("_cache"), &cache);

	dao_array_fetch_str(&column_types, &resultset, SL("columnTypes"), PH_NOISY|PH_READONLY);
	dao_update_property(getThis(), SL("_columnTypes"), &column_types);

	dao_array_fetch_str(&hydrate_mode, &resultset, SL("hydrateMode"), PH_NOISY|PH_READONLY);
	dao_update_property(getThis(), SL("_hydrateMode"), &hydrate_mode);
}
