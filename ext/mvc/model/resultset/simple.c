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

#include "mvc/model/resultset/simple.h"
#include "mvc/model/resultset.h"
#include "mvc/model/resultsetinterface.h"
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
#include "kernel/exception.h"
#include "kernel/variables.h"

/**
 * Dao\Mvc\Model\Resultset\Simple
 *
 * Simple resultsets only contains complete objects.
 * This class builds every complete object as it is required
 */
zend_class_entry *dao_mvc_model_resultset_simple_ce;

PHP_METHOD(Dao_Mvc_Model_Resultset_Simple, __construct);
PHP_METHOD(Dao_Mvc_Model_Resultset_Simple, valid);
PHP_METHOD(Dao_Mvc_Model_Resultset_Simple, toArray);
PHP_METHOD(Dao_Mvc_Model_Resultset_Simple, serialize);
PHP_METHOD(Dao_Mvc_Model_Resultset_Simple, unserialize);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_resultset_simple___construct, 0, 0, 3)
	ZEND_ARG_INFO(0, columnMap)
	ZEND_ARG_INFO(0, model)
	ZEND_ARG_INFO(0, result)
	ZEND_ARG_INFO(0, cache)
	ZEND_ARG_INFO(0, sourceModel)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_resultset_simple_toarray, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, columns, IS_ARRAY, 1)
	ZEND_ARG_TYPE_INFO(0, mustColumn, _IS_BOOL, 1)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_mvc_model_resultset_simple_method_entry[] = {
	PHP_ME(Dao_Mvc_Model_Resultset_Simple, __construct, arginfo_dao_mvc_model_resultset_simple___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Mvc_Model_Resultset_Simple, valid, arginfo_iterator_valid, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Resultset_Simple, toArray, arginfo_dao_mvc_model_resultset_simple_toarray, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Resultset_Simple, serialize, arginfo_serializable_serialize, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Resultset_Simple, unserialize, arginfo_serializable_unserialize, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Mvc\Model\Resultset\Simple initializer
 */
DAO_INIT_CLASS(Dao_Mvc_Model_Resultset_Simple){

	DAO_REGISTER_CLASS_EX(Dao\\Mvc\\Model\\Resultset, Simple, mvc_model_resultset_simple, dao_mvc_model_resultset_ce, dao_mvc_model_resultset_simple_method_entry, 0);

	zend_declare_property_null(dao_mvc_model_resultset_simple_ce, SL("_sourceModel"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_model_resultset_simple_ce, SL("_model"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_model_resultset_simple_ce, SL("_columnMap"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_model_resultset_simple_ce, SL("_rowsModels"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_model_resultset_simple_ce, SL("_rowsObjects"), ZEND_ACC_PROTECTED);

	return SUCCESS;
}

/**
 * Dao\Mvc\Model\Resultset\Simple constructor
 *
 * @param array $columnMap
 * @param Dao\Mvc\ModelInterface $model
 * @param Dao\Db\Result\Pdo $result
 * @param Dao\Cache\BackendInterface $cache
 */
PHP_METHOD(Dao_Mvc_Model_Resultset_Simple, __construct){

	zval *column_map, *model, *result, *cache = NULL, *source_model = NULL, fetch_assoc = {};

	dao_fetch_params(1, 3, 2, &column_map, &model, &result, &cache, &source_model);

	if (!cache) {
		cache = &DAO_GLOBAL(z_null);
	}

	if (!source_model) {
		source_model = model;
	}

	dao_update_property(getThis(), SL("_model"), model);
	dao_update_property(getThis(), SL("_result"), result);
	dao_update_property(getThis(), SL("_cache"), cache);
	dao_update_property(getThis(), SL("_columnMap"), column_map);
	dao_update_property(getThis(), SL("_sourceModel"), source_model);

	dao_update_property_long(getThis(), SL("_type"), DAO_MVC_MODEL_RESULTSET_TYPE_PARTIAL);

	if (Z_TYPE_P(result) == IS_OBJECT) {
		/**
		 * Use only fetch assoc
		 */
		ZVAL_LONG(&fetch_assoc, PDO_FETCH_ASSOC);
		DAO_MM_CALL_METHOD(NULL, result, "setfetchmode", &fetch_assoc);
	}
	RETURN_MM();
}

/**
 * Check whether the internal resource has rows to fetch
 *
 * @return boolean
 */
PHP_METHOD(Dao_Mvc_Model_Resultset_Simple, valid){

	zval key = {}, type = {}, row = {}, rows = {}, dirty_state = {}, hydrate_mode = {}, column_map = {};
	zval source_model = {}, model = {}, active_row = {}, rows_objects = {};
	zend_class_entry *ce;

	DAO_MM_INIT();

	DAO_MM_CALL_SELF(&key, "key");
	DAO_MM_ADD_ENTRY(&key);

	dao_read_property(&type, getThis(), SL("_type"), PH_NOISY|PH_READONLY);
	if (zend_is_true(&type)) {
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

		if (Z_TYPE(rows) != IS_ARRAY || !dao_property_array_isset_fetch(&row, getThis(), SL("_rows"), &key, PH_READONLY)) {
			ZVAL_FALSE(&row);
		}
	}

	if (Z_TYPE(row) != IS_ARRAY) {
		dao_update_property_bool(getThis(), SL("_activeRow"), 0);
		RETURN_MM_FALSE;
	}

	/**
	 * Set records as dirty state PERSISTENT by default
	 */
	ZVAL_LONG(&dirty_state, 0);

	/**
	 * Get current hydration mode
	 */
	dao_read_property(&hydrate_mode, getThis(), SL("_hydrateMode"), PH_NOISY|PH_READONLY);

	/**
	 * Get the resultset column map
	 */
	dao_read_property(&column_map, getThis(), SL("_columnMap"), PH_NOISY|PH_READONLY);

	dao_read_property(&source_model, getThis(), SL("_sourceModel"), PH_NOISY|PH_READONLY);

	if (Z_TYPE(source_model) == IS_OBJECT) {
		ce = Z_OBJCE(source_model);
	} else {
		ce = dao_mvc_model_ce;
	}

	/**
	 * Hydrate based on the current hydration
	 */
	switch (dao_get_intval(&hydrate_mode)) {

		case 0:
			dao_read_property(&rows_objects, getThis(), SL("_rowsModels"), PH_NOISY|PH_READONLY);
			if (!dao_array_isset_fetch(&active_row, &rows_objects, &key, PH_READONLY)) {
				/**
				 * this_ptr->model is the base entity
				 */
				dao_read_property(&model, getThis(), SL("_model"), PH_NOISY|PH_READONLY);

				/**
				 * Performs the standard hydration based on objects
				 */
				DAO_MM_CALL_CE_STATIC(&active_row, ce, "cloneresultmap", &model, &row, &column_map, &dirty_state, &source_model);
				DAO_MM_ADD_ENTRY(&active_row);
				dao_update_property_array(getThis(), SL("_rowsModels"), &key, &active_row);
			}
			break;

		default:
			dao_read_property(&rows_objects, getThis(), SL("_rowsObjects"), PH_NOISY|PH_READONLY);
			if (!dao_array_isset_fetch(&active_row, &rows_objects, &key, PH_READONLY)) {
				/**
				 * Other kinds of hydrations
				 */
				DAO_MM_CALL_CE_STATIC(&active_row, ce, "cloneresultmaphydrate", &row, &column_map, &hydrate_mode, &source_model);
				DAO_MM_ADD_ENTRY(&active_row);
				dao_update_property_array(getThis(), SL("_rowsModels"), &key, &active_row);
			}
			break;
	}

	dao_update_property(getThis(), SL("_activeRow"), &active_row);
	RETURN_MM_TRUE;
}

/**
 * Returns a complete resultset as an array, if the resultset has a big number of rows
 * it could consume more memory than it currently does. Exporting the resultset to an array
 * couldn't be faster with a large number of records
 *
 * @param array $columns
 * @param boolean $mustColumn
 * @return array
 */
PHP_METHOD(Dao_Mvc_Model_Resultset_Simple, toArray){

	zval *columns = NULL, *must_column = NULL;

	dao_fetch_params(1, 0, 2, &columns, &must_column);

	if (!columns) {
		columns = &DAO_GLOBAL(z_null);
	}

	if (!must_column) {
		must_column = &DAO_GLOBAL(z_null);
	}

	array_init(return_value);

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
			DAO_MM_CALL_METHOD(&arr, &current, "toarray", columns, must_column);
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
PHP_METHOD(Dao_Mvc_Model_Resultset_Simple, serialize){

	zval records = {}, count = {}, model = {}, cache = {}, column_map = {}, hydrate_mode = {}, data = {};

	DAO_CALL_METHOD(&records, getThis(), "toarray", &DAO_GLOBAL(z_false));
	dao_fast_count(&count, &records);

	dao_read_property(&model, getThis(), SL("_model"), PH_NOISY|PH_READONLY);
	dao_read_property(&cache, getThis(), SL("_cache"), PH_NOISY|PH_READONLY);
	dao_read_property(&column_map, getThis(), SL("_columnMap"), PH_NOISY|PH_READONLY);
	dao_read_property(&hydrate_mode, getThis(), SL("_hydrateMode"), PH_NOISY|PH_READONLY);

	array_init_size(&data, 5);
	dao_array_update_str(&data, SL("model"), &model, PH_COPY);
	dao_array_update_str(&data, SL("cache"), &cache, PH_COPY);
	dao_array_update_str(&data, SL("rows"), &records, 0);
	dao_array_update_str(&data, SL("columnMap"), &column_map, PH_COPY);
	dao_array_update_str(&data, SL("hydrateMode"), &hydrate_mode, PH_COPY);
	dao_array_update_str(&data, SL("count"), &count, 0);

	/**
	 * Force to re-execute the query
	 */
	dao_update_property_bool(getThis(), SL("_activeRow"), 0);

	/**
	 * Serialize the cache using the serialize function
	 */
	dao_serialize(return_value, &data);
	zval_ptr_dtor(&data);
}

/**
 * Unserializing a resultset only works on the rows present in the saved state
 *
 * @param string $data
 */
PHP_METHOD(Dao_Mvc_Model_Resultset_Simple, unserialize){

	zval *data, resultset = {}, model = {}, rows = {}, count = {}, cache = {}, column_map = {}, hydrate_mode = {};

	dao_fetch_params(0, 1, 0, &data);

	dao_update_property_long(getThis(), SL("_type"), DAO_MVC_MODEL_RESULTSET_TYPE_FULL);

	dao_unserialize(&resultset, data);
	if (Z_TYPE(resultset) != IS_ARRAY) {
		DAO_THROW_EXCEPTION_STR(dao_mvc_model_exception_ce, "Invalid serialization data");
		return;
	}

	dao_array_fetch_str(&model, &resultset, SL("model"), PH_NOISY|PH_READONLY);
	dao_update_property(getThis(), SL("_model"), &model);

	dao_array_fetch_str(&rows, &resultset, SL("rows"), PH_NOISY|PH_READONLY);
	dao_update_property(getThis(), SL("_rows"), &rows);

	dao_array_fetch_str(&count, &resultset, SL("count"), PH_NOISY|PH_READONLY);
	dao_update_property(getThis(), SL("_count"), &count);

	dao_array_fetch_str(&cache, &resultset, SL("cache"), PH_NOISY|PH_READONLY);
	dao_update_property(getThis(), SL("_cache"), &cache);

	dao_array_fetch_str(&column_map, &resultset, SL("columnMap"), PH_NOISY|PH_READONLY);
	dao_update_property(getThis(), SL("_columnMap"), &column_map);

	dao_array_fetch_str(&hydrate_mode, &resultset, SL("hydrateMode"), PH_NOISY|PH_READONLY);
	dao_update_property(getThis(), SL("_hydrateMode"), &hydrate_mode);
	zval_ptr_dtor(&resultset);
}
