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

#include "mvc/model/metadata.h"
#include "mvc/model/metadatainterface.h"
#include "mvc/model/metadata/strategy/introspection.h"
#include "mvc/model/exception.h"
#include "mvc/modelinterface.h"
#include "diinterface.h"
#include "di/injectable.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"
#include "kernel/object.h"
#include "kernel/hash.h"
#include "kernel/array.h"
#include "kernel/concat.h"
#include "kernel/exception.h"
#include "kernel/string.h"
#include "kernel/file.h"
#include "kernel/operators.h"

/**
 * Dao\Mvc\Model\MetaData
 *
 * <p>Because Dao\Mvc\Model requires meta-data like field names, data types, primary keys, etc.
 * this component collect them and store for further querying by Dao\Mvc\Model.
 * Dao\Mvc\Model\MetaData can also use adapters to store temporarily or permanently the meta-data.</p>
 *
 * <p>A standard Dao\Mvc\Model\MetaData can be used to query model attributes:</p>
 *
 * <code>
 *	$metaData = new Dao\Mvc\Model\MetaData\Memory();
 *	$attributes = $metaData->getAttributes(new Robots());
 *	print_r($attributes);
 * </code>
 *
 */
zend_class_entry *dao_mvc_model_metadata_ce;

PHP_METHOD(Dao_Mvc_Model_MetaData, _initialize);
PHP_METHOD(Dao_Mvc_Model_MetaData, setStrategy);
PHP_METHOD(Dao_Mvc_Model_MetaData, getStrategy);
PHP_METHOD(Dao_Mvc_Model_MetaData, getCacheKey);
PHP_METHOD(Dao_Mvc_Model_MetaData, readMetaData);
PHP_METHOD(Dao_Mvc_Model_MetaData, readMetaDataIndex);
PHP_METHOD(Dao_Mvc_Model_MetaData, writeMetaDataIndex);
PHP_METHOD(Dao_Mvc_Model_MetaData, readColumnMap);
PHP_METHOD(Dao_Mvc_Model_MetaData, readColumnMapIndex);
PHP_METHOD(Dao_Mvc_Model_MetaData, getAttributes);
PHP_METHOD(Dao_Mvc_Model_MetaData, getPrimaryKeyAttributes);
PHP_METHOD(Dao_Mvc_Model_MetaData, getNonPrimaryKeyAttributes);
PHP_METHOD(Dao_Mvc_Model_MetaData, getNotNullAttributes);
PHP_METHOD(Dao_Mvc_Model_MetaData, isNotNull);
PHP_METHOD(Dao_Mvc_Model_MetaData, getDataTypes);
PHP_METHOD(Dao_Mvc_Model_MetaData, getDataType);
PHP_METHOD(Dao_Mvc_Model_MetaData, getDataSizes);
PHP_METHOD(Dao_Mvc_Model_MetaData, getDataSize);
PHP_METHOD(Dao_Mvc_Model_MetaData, getDataBytes);
PHP_METHOD(Dao_Mvc_Model_MetaData, getDataByte);
PHP_METHOD(Dao_Mvc_Model_MetaData, getDataScales);
PHP_METHOD(Dao_Mvc_Model_MetaData, getDataScale);
PHP_METHOD(Dao_Mvc_Model_MetaData, getDataTypesNumeric);
PHP_METHOD(Dao_Mvc_Model_MetaData, isNumeric);
PHP_METHOD(Dao_Mvc_Model_MetaData, getIdentityField);
PHP_METHOD(Dao_Mvc_Model_MetaData, getBindTypes);
PHP_METHOD(Dao_Mvc_Model_MetaData, getDefaultValues);
PHP_METHOD(Dao_Mvc_Model_MetaData, getAutomaticCreateAttributes);
PHP_METHOD(Dao_Mvc_Model_MetaData, getAutomaticUpdateAttributes);
PHP_METHOD(Dao_Mvc_Model_MetaData, setAutomaticCreateAttributes);
PHP_METHOD(Dao_Mvc_Model_MetaData, setAutomaticUpdateAttributes);
PHP_METHOD(Dao_Mvc_Model_MetaData, getColumnMap);
PHP_METHOD(Dao_Mvc_Model_MetaData, getReverseColumnMap);
PHP_METHOD(Dao_Mvc_Model_MetaData, hasAttribute);
PHP_METHOD(Dao_Mvc_Model_MetaData, getAttribute);
PHP_METHOD(Dao_Mvc_Model_MetaData, hasRealAttribute);
PHP_METHOD(Dao_Mvc_Model_MetaData, getRealAttribute);
PHP_METHOD(Dao_Mvc_Model_MetaData, isEmpty);
PHP_METHOD(Dao_Mvc_Model_MetaData, reset);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_metadata_getcachekey, 0, 0, 3)
	ZEND_ARG_INFO(0, model)
	ZEND_ARG_INFO(0, table)
	ZEND_ARG_INFO(0, schema)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_mvc_model_metadata_method_entry[] = {
	PHP_ME(Dao_Mvc_Model_MetaData, _initialize, arginfo_empty, ZEND_ACC_PROTECTED)
	PHP_ME(Dao_Mvc_Model_MetaData, setStrategy, arginfo_dao_mvc_model_metadatainterface_setstrategy, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_MetaData, getStrategy, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_MetaData, getCacheKey, arginfo_dao_mvc_model_metadata_getcachekey, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_MetaData, readMetaData, arginfo_dao_mvc_model_metadatainterface_readmetadata, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_MetaData, readMetaDataIndex, arginfo_dao_mvc_model_metadatainterface_readmetadataindex, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_MetaData, writeMetaDataIndex, arginfo_dao_mvc_model_metadatainterface_writemetadataindex, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_MetaData, readColumnMap, arginfo_dao_mvc_model_metadatainterface_readcolumnmap, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_MetaData, readColumnMapIndex, arginfo_dao_mvc_model_metadatainterface_readcolumnmapindex, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_MetaData, getAttributes, arginfo_dao_mvc_model_metadatainterface_getattributes, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_MetaData, getPrimaryKeyAttributes, arginfo_dao_mvc_model_metadatainterface_getprimarykeyattributes, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_MetaData, getNonPrimaryKeyAttributes, arginfo_dao_mvc_model_metadatainterface_getnonprimarykeyattributes, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_MetaData, getNotNullAttributes, arginfo_dao_mvc_model_metadatainterface_getnotnullattributes, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_MetaData, isNotNull, arginfo_dao_mvc_model_metadatainterface_isnotnull, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_MetaData, getDataTypes, arginfo_dao_mvc_model_metadatainterface_getdatatypes, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_MetaData, getDataType, arginfo_dao_mvc_model_metadatainterface_getdatatype, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_MetaData, getDataSizes, arginfo_dao_mvc_model_metadatainterface_getdatasizes, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_MetaData, getDataSize, arginfo_dao_mvc_model_metadatainterface_getdatasize, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_MetaData, getDataBytes, arginfo_dao_mvc_model_metadatainterface_getdatabytes, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_MetaData, getDataByte, arginfo_dao_mvc_model_metadatainterface_getdatabyte, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_MetaData, getDataScales, arginfo_dao_mvc_model_metadatainterface_getdatascales, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_MetaData, getDataScale, arginfo_dao_mvc_model_metadatainterface_getdatascale, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_MetaData, getDataTypesNumeric, arginfo_dao_mvc_model_metadatainterface_getdatatypesnumeric, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_MetaData, isNumeric, arginfo_dao_mvc_model_metadatainterface_isnumeric, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_MetaData, getIdentityField, arginfo_dao_mvc_model_metadatainterface_getidentityfield, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_MetaData, getBindTypes, arginfo_dao_mvc_model_metadatainterface_getbindtypes, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_MetaData, getDefaultValues, arginfo_dao_mvc_model_metadatainterface_getdefaultvalues, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_MetaData, getAutomaticCreateAttributes, arginfo_dao_mvc_model_metadatainterface_getautomaticcreateattributes, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_MetaData, getAutomaticUpdateAttributes, arginfo_dao_mvc_model_metadatainterface_getautomaticupdateattributes, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_MetaData, setAutomaticCreateAttributes, arginfo_dao_mvc_model_metadatainterface_setautomaticcreateattributes, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_MetaData, setAutomaticUpdateAttributes, arginfo_dao_mvc_model_metadatainterface_setautomaticupdateattributes, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_MetaData, getColumnMap, arginfo_dao_mvc_model_metadatainterface_getcolumnmap, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_MetaData, getReverseColumnMap, arginfo_dao_mvc_model_metadatainterface_getreversecolumnmap, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_MetaData, hasAttribute, arginfo_dao_mvc_model_metadatainterface_hasattribute, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_MetaData, getAttribute, arginfo_dao_mvc_model_metadatainterface_getattribute, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_MetaData, hasRealAttribute, arginfo_dao_mvc_model_metadatainterface_hasrealattribute, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_MetaData, getRealAttribute, arginfo_dao_mvc_model_metadatainterface_getrealattribute, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_MetaData, isEmpty, arginfo_dao_mvc_model_metadatainterface_isempty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_MetaData, reset, arginfo_dao_mvc_model_metadatainterface_reset, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Mvc\Model\MetaData initializer
 */
DAO_INIT_CLASS(Dao_Mvc_Model_MetaData){

	DAO_REGISTER_CLASS_EX(Dao\\Mvc\\Model, MetaData, mvc_model_metadata, dao_di_injectable_ce, dao_mvc_model_metadata_method_entry, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	zend_declare_property_null(dao_mvc_model_metadata_ce, SL("_strategy"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_model_metadata_ce, SL("_metaData"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_model_metadata_ce, SL("_columnMap"), ZEND_ACC_PROTECTED);

	zend_declare_class_constant_long(dao_mvc_model_metadata_ce, SL("MODELS_ATTRIBUTES"),               DAO_MVC_MODEL_METADATA_MODELS_ATTRIBUTES              );
	zend_declare_class_constant_long(dao_mvc_model_metadata_ce, SL("MODELS_PRIMARY_KEY"),              DAO_MVC_MODEL_METADATA_MODELS_PRIMARY_KEY             );
	zend_declare_class_constant_long(dao_mvc_model_metadata_ce, SL("MODELS_NON_PRIMARY_KEY"),          DAO_MVC_MODEL_METADATA_MODELS_NON_PRIMARY_KEY         );
	zend_declare_class_constant_long(dao_mvc_model_metadata_ce, SL("MODELS_NOT_NULL"),                 DAO_MVC_MODEL_METADATA_MODELS_NOT_NULL                );
	zend_declare_class_constant_long(dao_mvc_model_metadata_ce, SL("MODELS_DATA_TYPES"),               DAO_MVC_MODEL_METADATA_MODELS_DATA_TYPES              );
	zend_declare_class_constant_long(dao_mvc_model_metadata_ce, SL("MODELS_DATA_TYPES_NUMERIC"),       DAO_MVC_MODEL_METADATA_MODELS_DATA_TYPES_NUMERIC      );
	zend_declare_class_constant_long(dao_mvc_model_metadata_ce, SL("MODELS_DATE_AT"),                  DAO_MVC_MODEL_METADATA_MODELS_DATE_AT                 );
	zend_declare_class_constant_long(dao_mvc_model_metadata_ce, SL("MODELS_DATE_IN"),                  DAO_MVC_MODEL_METADATA_MODELS_DATE_IN                 );
	zend_declare_class_constant_long(dao_mvc_model_metadata_ce, SL("MODELS_IDENTITY_COLUMN"),          DAO_MVC_MODEL_METADATA_MODELS_IDENTITY_COLUMN         );
	zend_declare_class_constant_long(dao_mvc_model_metadata_ce, SL("MODELS_DATA_TYPES_BIND"),          DAO_MVC_MODEL_METADATA_MODELS_DATA_TYPES_BIND         );
	zend_declare_class_constant_long(dao_mvc_model_metadata_ce, SL("MODELS_AUTOMATIC_DEFAULT_INSERT"), DAO_MVC_MODEL_METADATA_MODELS_AUTOMATIC_DEFAULT_INSERT);
	zend_declare_class_constant_long(dao_mvc_model_metadata_ce, SL("MODELS_AUTOMATIC_DEFAULT_UPDATE"), DAO_MVC_MODEL_METADATA_MODELS_AUTOMATIC_DEFAULT_UPDATE);
	zend_declare_class_constant_long(dao_mvc_model_metadata_ce, SL("MODELS_DATA_DEFAULT_VALUE"),       DAO_MVC_MODEL_METADATA_MODELS_DATA_DEFAULT_VALUE);
	zend_declare_class_constant_long(dao_mvc_model_metadata_ce, SL("MODELS_DATA_SZIE"),                DAO_MVC_MODEL_METADATA_MODELS_DATA_SZIE);
	zend_declare_class_constant_long(dao_mvc_model_metadata_ce, SL("MODELS_DATA_SCALE"),               DAO_MVC_MODEL_METADATA_MODELS_DATA_SCALE);
	zend_declare_class_constant_long(dao_mvc_model_metadata_ce, SL("MODELS_DATA_BYTE"),                DAO_MVC_MODEL_METADATA_MODELS_DATA_BYTE);

	zend_declare_class_constant_long(dao_mvc_model_metadata_ce, SL("MODELS_COLUMN_MAP"),         DAO_MVC_MODEL_METADATA_MODELS_COLUMN_MAP);
	zend_declare_class_constant_long(dao_mvc_model_metadata_ce, SL("MODELS_REVERSE_COLUMN_MAP"), DAO_MVC_MODEL_METADATA_MODELS_REVERSE_COLUMN_MAP);

	zend_class_implements(dao_mvc_model_metadata_ce, 1, dao_mvc_model_metadatainterface_ce);

	return SUCCESS;
}

/**
 * Initialize the metadata for certain table
 *
 * @param Dao\Mvc\ModelInterface $model
 * @param string $key
 * @param string $table
 * @param string $schema
 */
PHP_METHOD(Dao_Mvc_Model_MetaData, _initialize){

	zval *model, *key, *table, *schema, dependency_injector = {}, class_name = {}, meta_data = {}, prefix_key = {}, model_metadata = {}, strategy = {}, exception_message = {}, column_map = {}, model_column_map = {};

	dao_fetch_params(1, 4, 0, &model, &key, &table, &schema);
	ZVAL_NULL(&strategy);

	if (DAO_IS_EMPTY(key)) {
		DAO_MM_THROW_EXCEPTION_STR(dao_mvc_model_exception_ce, "The key is not valid");
		return;
	}

	DAO_MM_CALL_METHOD(&dependency_injector, getThis(), "getdi");
	DAO_MM_ADD_ENTRY(&dependency_injector);
	dao_get_class(&class_name, model, 0);
	DAO_MM_ADD_ENTRY(&class_name);

	dao_read_property(&meta_data, getThis(), SL("_metaData"), PH_READONLY);
	if (Z_TYPE(meta_data) != IS_ARRAY) {
		array_init(&meta_data);
		DAO_MM_ADD_ENTRY(&meta_data);
	}

	if (!dao_array_isset(&meta_data, key)) {
		/**
		 * Get the meta-data extraction strategy
		 */
		DAO_MM_CALL_METHOD(&strategy, getThis(), "getstrategy");
		DAO_MM_ADD_ENTRY(&strategy);
		DAO_CONCAT_SV(&prefix_key, "meta-", key);
		DAO_MM_ADD_ENTRY(&prefix_key);

		/**
		 * The meta-data is read from the cache adapter always
		 */
		DAO_MM_CALL_METHOD(&model_metadata, getThis(), "read", &prefix_key);
		DAO_MM_ADD_ENTRY(&model_metadata);

		if (Z_TYPE(model_metadata) != IS_NULL) {
			dao_update_property_array(getThis(), SL("_metaData"), key, &model_metadata);
		} else {
			/**
			 * Check if there is a method 'metaData' in the model to retrieve meta-data from it
			 */
			if (dao_method_exists_ex(model, SL("metadata")) == SUCCESS) {
				DAO_MM_CALL_METHOD(&model_metadata, model, "metadata");
				DAO_MM_ADD_ENTRY(&model_metadata);

				if (Z_TYPE(model_metadata) != IS_ARRAY) {
					DAO_CONCAT_SV(&exception_message, "Invalid meta-data for model ", &class_name);
					DAO_MM_ADD_ENTRY(&exception_message);
					DAO_MM_THROW_EXCEPTION_ZVAL(dao_mvc_model_exception_ce, &exception_message);
					return;
				}
			} else {
				/**
				 * Get the meta-data
				 */
				DAO_MM_CALL_METHOD(&model_metadata, &strategy, "getmetadata", model, &dependency_injector);
				DAO_MM_ADD_ENTRY(&model_metadata);
			}

			/**
			 * Store the meta-data locally
			 */
			dao_update_property_array(getThis(), SL("_metaData"), key, &model_metadata);

			/**
			 * Store the meta-data in the adapter
			 */
			DAO_MM_CALL_METHOD(NULL, getThis(), "write", &prefix_key, &model_metadata);
		}

		/**
		 * Check for a column map, store in _columnMap in order and reversed order
		 */
		dao_read_property(&column_map, getThis(), SL("_columnMap"), PH_READONLY);

		if (Z_TYPE(column_map) != IS_ARRAY) {
			array_init(&column_map);
			
			dao_update_property(getThis(), SL("_columnMap"), &column_map);
			zval_ptr_dtor(&column_map);
		} else if (dao_array_isset_fetch(&model_column_map, &column_map, key, PH_READONLY) && Z_TYPE(model_column_map) != IS_NULL) {
			RETURN_MM();
		}

		DAO_CONCAT_SV(&prefix_key, "map-", key);
		DAO_MM_ADD_ENTRY(&prefix_key);

		/**
		 * Check if the meta-data is already in the adapter
		 */
		DAO_MM_CALL_METHOD(&model_column_map, getThis(), "read", &prefix_key);
		DAO_MM_ADD_ENTRY(&model_column_map);
		if (Z_TYPE(model_column_map) != IS_NULL) {
			dao_array_update(&column_map, key, &model_column_map, PH_COPY);
			RETURN_MM();
		}

		/**
		 * Get the meta-data
		 */
		DAO_MM_CALL_METHOD(&model_column_map, &strategy, "getcolumnmaps", model, &dependency_injector);
		DAO_MM_ADD_ENTRY(&model_column_map);

		/**
		 * Update the column map locally
		 */
		dao_update_property_array(getThis(), SL("_columnMap"), key, &model_column_map);

		/**
		 * Write the data to the adapter
		 */
		DAO_MM_CALL_METHOD(NULL, getThis(), "write", &prefix_key, &model_column_map);
	}
	RETURN_MM();
}

/**
 * Set the meta-data extraction strategy
 *
 * @param Dao\Mvc\Model\MetaData\Strategy\Introspection $strategy
 */
PHP_METHOD(Dao_Mvc_Model_MetaData, setStrategy){

	zval *strategy;

	dao_fetch_params(0, 1, 0, &strategy);

	if (Z_TYPE_P(strategy) != IS_OBJECT) {
		DAO_THROW_EXCEPTION_STR(dao_mvc_model_exception_ce, "The meta-data extraction strategy is not valid");
		return;
	}
	dao_update_property(getThis(), SL("_strategy"), strategy);

}

/**
 * Return the strategy to obtain the meta-data
 *
 * @return Dao\Mvc\Model\MetaData\Strategy\Introspection
 */
PHP_METHOD(Dao_Mvc_Model_MetaData, getStrategy){

	dao_read_property(return_value, getThis(), SL("_strategy"), PH_COPY);
	if (Z_TYPE_P(return_value) == IS_NULL) {
		object_init_ex(return_value, dao_mvc_model_metadata_strategy_introspection_ce);
		dao_update_property(getThis(), SL("_strategy"), return_value);
	}
}

/**
 * Return the meta data cache key
 *
 * @param Dao\Mvc\Model model
 * @param string table
 * @param string schema
 * @return string
 */
PHP_METHOD(Dao_Mvc_Model_MetaData, getCacheKey){

	zval *model, *table, *schema, key = {}, event_name = {};

	dao_fetch_params(1, 3, 0, &model, &table, &schema);
	DAO_MM_VERIFY_INTERFACE_EX(model, dao_mvc_modelinterface_ce, dao_mvc_model_exception_ce);

	DAO_MM_ZVAL_STRING(&event_name, "beforeGetCacheKey");
	DAO_MM_CALL_METHOD(return_value, getThis(), "fireevent", &event_name, model);

	if (Z_TYPE_P(return_value) == IS_STRING) {
		RETURN_MM();
	}

	if (dao_method_exists_ex(model, SL("getmetadatacachekey")) == SUCCESS) {
		DAO_MM_CALL_METHOD(&key, model, "getmetadatacachekey");
		DAO_MM_ADD_ENTRY(&key);
	}

	if (Z_TYPE(key) != IS_STRING || DAO_IS_EMPTY(&key)) {
		zval class_name = {};
		dao_get_class(&class_name, model, 1);
		DAO_MM_ADD_ENTRY(&class_name);

		/**
		 * Unique key for meta-data is created using class-name-schema-table
		 */
		DAO_CONCAT_VSVV(&key, &class_name, "-", schema, table);
		DAO_MM_ADD_ENTRY(&key);
	}

	DAO_MM_ZVAL_STRING(&event_name, "afterGetCacheKey");
	DAO_MM_CALL_METHOD(return_value, getThis(), "fireevent", &event_name, &key);

	if (Z_TYPE_P(return_value) == IS_STRING) {
		RETURN_MM();
	}
	RETURN_MM_CTOR(&key);
}

/**
 * Reads the complete meta-data for certain model
 *
 *<code>
 *	print_r($metaData->readMetaData(new Robots()));
 *</code>
 *
 * @param Dao\Mvc\ModelInterface $model
 * @return array
 */
PHP_METHOD(Dao_Mvc_Model_MetaData, readMetaData){

	zval *model, table = {}, schema = {}, key = {}, meta_data = {};

	dao_fetch_params(1, 1, 0, &model);
	DAO_MM_VERIFY_INTERFACE_EX(model, dao_mvc_modelinterface_ce, dao_mvc_model_exception_ce);

	DAO_MM_CALL_METHOD(&table, model, "getsource");
	DAO_MM_ADD_ENTRY(&table);
	DAO_MM_CALL_METHOD(&schema, model, "getschema");
	DAO_MM_ADD_ENTRY(&schema);

	DAO_MM_CALL_METHOD(&key, getThis(), "getcachekey", model, &table, &schema);
	DAO_MM_ADD_ENTRY(&key);

	dao_read_property(&meta_data, getThis(), SL("_metaData"), PH_NOISY|PH_READONLY);
	if (!dao_array_isset(&meta_data, &key)) {
		DAO_MM_CALL_METHOD(NULL, getThis(), "_initialize", model, &key, &table, &schema);
		dao_read_property(&meta_data, getThis(), SL("_metaData"), PH_NOISY|PH_READONLY);
	}

	if (!dao_array_isset_fetch(return_value, &meta_data, &key, PH_COPY)) {
		RETURN_MM_NULL();
	}
	RETURN_MM();
}

/**
 * Reads meta-data for certain model
 *
 *<code>
 *	print_r($metaData->readMetaDataIndex(new Robots(), 0);
 *</code>
 *
 * @param Dao\Mvc\ModelInterface $model
 * @param int $index
 * @return array
 */
PHP_METHOD(Dao_Mvc_Model_MetaData, readMetaDataIndex){

	zval *model, *index, table = {}, schema = {}, key = {}, meta_data = {}, meta_data_index = {};

	dao_fetch_params(1, 2, 0, &model, &index);
	DAO_MM_VERIFY_INTERFACE_EX(model, dao_mvc_modelinterface_ce, dao_mvc_model_exception_ce);
	DAO_ENSURE_IS_LONG(index);

	DAO_MM_CALL_METHOD(&table, model, "getsource");
	DAO_MM_ADD_ENTRY(&table);
	DAO_MM_CALL_METHOD(&schema, model, "getschema");
	DAO_MM_ADD_ENTRY(&schema);

	DAO_MM_CALL_METHOD(&key, getThis(), "getcachekey", model, &table, &schema);
	DAO_MM_ADD_ENTRY(&key);

	dao_read_property(&meta_data, getThis(), SL("_metaData"), PH_NOISY|PH_READONLY);

	if (!dao_array_isset(&meta_data, &key)) {
		DAO_MM_CALL_METHOD(NULL, getThis(), "_initialize", model, &key, &table, &schema);
		dao_read_property(&meta_data, getThis(), SL("_metaData"), PH_NOISY|PH_READONLY);
	}

	if (dao_array_isset_fetch(&meta_data_index, &meta_data, &key, PH_NOISY|PH_READONLY)) {
		dao_array_fetch(return_value, &meta_data_index, index, PH_NOISY|PH_COPY);
	}

	RETURN_MM();
}

/**
 * Writes meta-data for certain model using a MODEL_* constant
 *
 *<code>
 *	print_r($metaData->writeColumnMapIndex(new Robots(), MetaData::MODELS_REVERSE_COLUMN_MAP, array('leName' => 'name')));
 *</code>
 *
 * @param Dao\Mvc\ModelInterface $model
 * @param int $index
 * @param mixed $data
 */
PHP_METHOD(Dao_Mvc_Model_MetaData, writeMetaDataIndex){

	zval *model, *index, *data, *replace, table = {}, schema = {}, key = {}, meta_data = {}, arr = {}, value = {}, *v;
	zend_string *str_key;
	ulong idx;

	dao_fetch_params(1, 4, 0, &model, &index, &data, &replace);
	DAO_MM_VERIFY_INTERFACE_EX(model, dao_mvc_modelinterface_ce, dao_mvc_model_exception_ce);

	if (Z_TYPE_P(index) != IS_LONG) {
		DAO_MM_THROW_EXCEPTION_STR(dao_mvc_model_exception_ce, "Index must be a valid integer constant");
		return;
	}

	if (Z_TYPE_P(data) != IS_ARRAY && Z_TYPE_P(data) != IS_STRING && !DAO_IS_BOOL(data)) {
		DAO_MM_THROW_EXCEPTION_STR(dao_mvc_model_exception_ce, "Invalid data for index");
		return;
	}

	DAO_MM_CALL_METHOD(&table, model, "getsource");
	DAO_MM_ADD_ENTRY(&table);
	DAO_MM_CALL_METHOD(&schema, model, "getschema");
	DAO_MM_ADD_ENTRY(&schema);

	DAO_MM_CALL_METHOD(&key, getThis(), "getcachekey", model, &table, &schema);
	DAO_MM_ADD_ENTRY(&key);

	dao_read_property(&meta_data, getThis(), SL("_metaData"), PH_NOISY|PH_READONLY);
	if (!dao_array_isset(&meta_data, &key)) {
		DAO_MM_CALL_METHOD(NULL, getThis(), "_initialize", model, &key, &table, &schema);
		dao_read_property(&meta_data, getThis(), SL("_metaData"), PH_NOISY|PH_READONLY);
	} else if (!zend_is_true(replace)) {
		dao_array_fetch(&arr, &meta_data, &key, PH_NOISY|PH_READONLY);
		dao_array_fetch(&value, &arr, index, PH_NOISY|PH_READONLY);

		DAO_SEPARATE_PARAM(data);

		ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL(value), idx, str_key, v) {
			zval tmp = {};
			if (str_key) {
				ZVAL_STR(&tmp, str_key);
			} else {
				ZVAL_LONG(&tmp, idx);
			}

			if (!dao_array_isset(data, &tmp)) {
				dao_array_update(data, &tmp, v, PH_COPY);
			}
		} ZEND_HASH_FOREACH_END();
	}

	dao_array_update_multi_2(&meta_data, &key, index, data, PH_COPY);
	RETURN_MM();
}

/**
 * Reads the ordered/reversed column map for certain model
 *
 *<code>
 *	print_r($metaData->readColumnMap(new Robots()));
 *</code>
 *
 * @param Dao\Mvc\ModelInterface $model
 * @return array
 */
PHP_METHOD(Dao_Mvc_Model_MetaData, readColumnMap){

	zval *model, table = {}, schema = {}, key = {}, column_map = {}, data = {};

	dao_fetch_params(1, 1, 0, &model);
	DAO_MM_VERIFY_INTERFACE_EX(model, dao_mvc_modelinterface_ce, dao_mvc_model_exception_ce);

	DAO_MM_CALL_METHOD(&table, model, "getsource");
	DAO_MM_ADD_ENTRY(&table);
	DAO_MM_CALL_METHOD(&schema, model, "getschema");
	DAO_MM_ADD_ENTRY(&schema);

	DAO_MM_CALL_METHOD(&key, getThis(), "getcachekey", model, &table, &schema);
	DAO_MM_ADD_ENTRY(&key);

	dao_read_property(&column_map, getThis(), SL("_columnMap"), PH_NOISY|PH_READONLY);
	if (!dao_array_isset(&column_map, &key)) {
		DAO_MM_CALL_METHOD(NULL, getThis(), "_initialize", model, &key, &table, &schema);
		dao_read_property(&column_map, getThis(), SL("_columnMap"), PH_NOISY|PH_READONLY);
	}

	dao_array_fetch(&data, &column_map, &key, PH_NOISY|PH_READONLY);

	RETURN_MM_CTOR(&data);
}

/**
 * Reads column-map information for certain model using a MODEL_* constant
 *
 *<code>
 *	print_r($metaData->readColumnMapIndex(new Robots(), MetaData::MODELS_REVERSE_COLUMN_MAP));
 *</code>
 *
 * @param Dao\Mvc\ModelInterface $model
 * @param int $index
 */
PHP_METHOD(Dao_Mvc_Model_MetaData, readColumnMapIndex){

	zval *model, *index, table = {}, schema = {}, key = {}, column_map = {}, column_map_model = {}, attributes = {};

	dao_fetch_params(1, 2, 0, &model, &index);
	DAO_MM_VERIFY_INTERFACE_EX(model, dao_mvc_modelinterface_ce, dao_mvc_model_exception_ce);
	DAO_ENSURE_IS_LONG(index);

	DAO_MM_CALL_METHOD(&table, model, "getsource");
	DAO_MM_ADD_ENTRY(&table);
	DAO_MM_CALL_METHOD(&schema, model, "getschema");
	DAO_MM_ADD_ENTRY(&schema);

	DAO_MM_CALL_METHOD(&key, getThis(), "getcachekey", model, &table, &schema);
	DAO_MM_ADD_ENTRY(&key);

	dao_read_property(&column_map, getThis(), SL("_columnMap"), PH_NOISY|PH_READONLY);
	if (!dao_array_isset(&column_map, &key)) {
		DAO_MM_CALL_SELF(NULL, "_initialize", model, &key, &table, &schema);

		dao_read_property(&column_map, getThis(), SL("_columnMap"), PH_NOISY|PH_READONLY);
	}

	dao_array_fetch(&column_map_model, &column_map, &key, PH_NOISY|PH_READONLY);
	dao_array_fetch(&attributes, &column_map_model, index, PH_NOISY|PH_READONLY);

	RETURN_MM_CTOR(&attributes);
}

/**
 * Returns table attributes names (fields)
 *
 *<code>
 *	print_r($metaData->getAttributes(new Robots()));
 *</code>
 *
 * @param Dao\Mvc\ModelInterface $model
 * @return array
 */
PHP_METHOD(Dao_Mvc_Model_MetaData, getAttributes){

	zval *model, what = {};

	dao_fetch_params(0, 1, 0, &model);

	ZVAL_LONG(&what, DAO_MVC_MODEL_METADATA_MODELS_ATTRIBUTES);

	DAO_RETURN_CALL_METHOD(getThis(), "readmetadataindex", model, &what);

	if (Z_TYPE_P(return_value) != IS_ARRAY) {
		DAO_THROW_EXCEPTION_STR(dao_mvc_model_exception_ce, "The meta-data is invalid or is corrupted");
		return;
	}
}

/**
 * Returns an array of fields which are part of the primary key
 *
 *<code>
 *	print_r($metaData->getPrimaryKeyAttributes(new Robots()));
 *</code>
 *
 * @param Dao\Mvc\ModelInterface $model
 * @return array
 */
PHP_METHOD(Dao_Mvc_Model_MetaData, getPrimaryKeyAttributes){

	zval *model, index = {};

	dao_fetch_params(0, 1, 0, &model);

	ZVAL_LONG(&index, 1);

	DAO_CALL_METHOD(return_value, getThis(), "readmetadataindex", model, &index);
	if (Z_TYPE_P(return_value) != IS_ARRAY) {
		DAO_THROW_EXCEPTION_STR(dao_mvc_model_exception_ce, "The meta-data is invalid or is corrupt");
		return;
	}
}

/**
 * Returns an arrau of fields which are not part of the primary key
 *
 *<code>
 *	print_r($metaData->getNonPrimaryKeyAttributes(new Robots()));
 *</code>
 *
 * @param Dao\Mvc\ModelInterface $model
 * @return 	array
 */
PHP_METHOD(Dao_Mvc_Model_MetaData, getNonPrimaryKeyAttributes){

	zval *model, index = {};

	dao_fetch_params(0, 1, 0, &model);

	ZVAL_LONG(&index, 2);

	DAO_CALL_METHOD(return_value, getThis(), "readmetadataindex", model, &index);
	if (Z_TYPE_P(return_value) != IS_ARRAY) {
		DAO_THROW_EXCEPTION_STR(dao_mvc_model_exception_ce, "The meta-data is invalid or is corrupt");
		return;
	}
}

/**
 * Returns an array of not null attributes
 *
 *<code>
 *	print_r($metaData->getNotNullAttributes(new Robots()));
 *</code>
 *
 * @param Dao\Mvc\ModelInterface $model
 * @return array
 */
PHP_METHOD(Dao_Mvc_Model_MetaData, getNotNullAttributes){

	zval *model, index = {};

	dao_fetch_params(0, 1, 0, &model);

	ZVAL_LONG(&index, 3);

	DAO_CALL_METHOD(return_value, getThis(), "readmetadataindex", model, &index);
	if (Z_TYPE_P(return_value) != IS_ARRAY) {
		DAO_THROW_EXCEPTION_STR(dao_mvc_model_exception_ce, "The meta-data is invalid or is corrupt");
		return;
	}
}

/**
 * Checks if the attribute is not null
 *
 *<code>
 *	var_dump($metaData->isNotNull(new Robots(), 'type');
 *</code>
 *
 * @param Dao\Mvc\ModelInterface $model
 * @param string $attribute
 * @return boolean
 */
PHP_METHOD(Dao_Mvc_Model_MetaData, isNotNull){

	zval *model, *attribute, data = {};

	dao_fetch_params(0, 2, 0, &model, &attribute);

	DAO_CALL_METHOD(&data, getThis(), "getnotnullattributes", model);

	if (dao_fast_in_array(attribute, &data)) {
		RETVAL_TRUE;
	} else {
		RETVAL_FALSE;
	}
	zval_ptr_dtor(&data);
}

/**
 * Returns attributes and their data types
 *
 *<code>
 *	print_r($metaData->getDataTypes(new Robots()));
 *</code>
 *
 * @param Dao\Mvc\ModelInterface $model
 * @return array
 */
PHP_METHOD(Dao_Mvc_Model_MetaData, getDataTypes){

	zval *model, index = {};

	dao_fetch_params(0, 1, 0, &model);

	ZVAL_LONG(&index, DAO_MVC_MODEL_METADATA_MODELS_DATA_TYPES);

	DAO_CALL_METHOD(return_value, getThis(), "readmetadataindex", model, &index);

	if (Z_TYPE_P(return_value) != IS_ARRAY) {
		DAO_THROW_EXCEPTION_STR(dao_mvc_model_exception_ce, "The meta-data is invalid or is corrupt");
		return;
	}
}

/**
 * Returns attribute data type
 *
 *<code>
 *	print_r($metaData->getDataType(new Robots(), 'type'));
 *</code>
 *
 * @param Dao\Mvc\ModelInterface $model
 * @param string $attribute
 * @return int
 */
PHP_METHOD(Dao_Mvc_Model_MetaData, getDataType){

	zval *model, *attribute, data = {};

	dao_fetch_params(0, 2, 0, &model, &attribute);

	DAO_CALL_METHOD(&data, getThis(), "getdatatypes", model);

	if (!dao_array_isset_fetch(return_value, &data, attribute, PH_NOISY|PH_COPY)) {
		RETVAL_NULL();
	}
	zval_ptr_dtor(&data);
}

/**
 * Returns attributes and their data sizes
 *
 *<code>
 *	print_r($metaData->getDataSizes(new Robots()));
 *</code>
 *
 * @param Dao\Mvc\ModelInterface $model
 * @return array
 */
PHP_METHOD(Dao_Mvc_Model_MetaData, getDataSizes){

	zval *model, index = {};

	dao_fetch_params(0, 1, 0, &model);

	ZVAL_LONG(&index, DAO_MVC_MODEL_METADATA_MODELS_DATA_SZIE);

	DAO_CALL_METHOD(return_value, getThis(), "readmetadataindex", model, &index);
	if (Z_TYPE_P(return_value) != IS_ARRAY) {
		DAO_THROW_EXCEPTION_STR(dao_mvc_model_exception_ce, "The meta-data is invalid or is corrupt");
		return;
	}
}

/**
 * Returns attribute data size
 *
 *<code>
 *	print_r($metaData->getDataSize(new Robots(), 'type'));
 *</code>
 *
 * @param Dao\Mvc\ModelInterface $model
 * @param string $attribute
 * @return int
 */
PHP_METHOD(Dao_Mvc_Model_MetaData, getDataSize){

	zval *model, *attribute, data = {};

	dao_fetch_params(0, 2, 0, &model, &attribute);

	DAO_CALL_METHOD(&data, getThis(), "getdatasizes", model);

	if (!dao_array_isset_fetch(return_value, &data, attribute, PH_NOISY|PH_COPY)) {
		RETVAL_NULL();
	}
	zval_ptr_dtor(&data);
}

/**
 * Returns attributes and their data bytes
 *
 *<code>
 *	print_r($metaData->getDataBytes(new Robots()));
 *</code>
 *
 * @param Dao\Mvc\ModelInterface $model
 * @return int
 */
PHP_METHOD(Dao_Mvc_Model_MetaData, getDataBytes){

	zval *model, index = {};

	dao_fetch_params(0, 1, 0, &model);

	ZVAL_LONG(&index, DAO_MVC_MODEL_METADATA_MODELS_DATA_BYTE);

	DAO_CALL_METHOD(return_value, getThis(), "readmetadataindex", model, &index);
	if (Z_TYPE_P(return_value) != IS_ARRAY) {
		DAO_THROW_EXCEPTION_STR(dao_mvc_model_exception_ce, "The meta-data is invalid or is corrupt");
		return;
	}
}

/**
 * Returns attribute data byte
 *
 *<code>
 *	print_r($metaData->getDataByte(new Robots(), 'type'));
 *</code>
 *
 * @param Dao\Mvc\ModelInterface $model
 * @param string $attribute
 * @return int
 */
PHP_METHOD(Dao_Mvc_Model_MetaData, getDataByte){

	zval *model, *attribute, data = {};

	dao_fetch_params(0, 2, 0, &model, &attribute);

	DAO_CALL_METHOD(&data, getThis(), "getdatabytes", model);

	if (!dao_array_isset_fetch(return_value, &data, attribute, PH_NOISY|PH_COPY)) {
		RETVAL_NULL();
	}
	zval_ptr_dtor(&data);
}

/**
 * Returns attributes and their data scales
 *
 *<code>
 *	print_r($metaData->getDataScales(new Robots()));
 *</code>
 *
 * @param Dao\Mvc\ModelInterface $model
 * @return array
 */
PHP_METHOD(Dao_Mvc_Model_MetaData, getDataScales){

	zval *model, index = {};

	dao_fetch_params(0, 1, 0, &model);

	ZVAL_LONG(&index, DAO_MVC_MODEL_METADATA_MODELS_DATA_SCALE);

	DAO_CALL_METHOD(return_value, getThis(), "readmetadataindex", model, &index);
	if (Z_TYPE_P(return_value) != IS_ARRAY) {
		DAO_THROW_EXCEPTION_STR(dao_mvc_model_exception_ce, "The meta-data is invalid or is corrupt");
		return;
	}
}

/**
 * Returns attribute data scale
 *
 *<code>
 *	print_r($metaData->getDataScale(new Robots(), 'type'));
 *</code>
 *
 * @param Dao\Mvc\ModelInterface $model
 * @param string $attribute
 * @return int
 */
PHP_METHOD(Dao_Mvc_Model_MetaData, getDataScale){

	zval *model, *attribute, data = {};

	dao_fetch_params(0, 2, 0, &model, &attribute);

	DAO_CALL_METHOD(&data, getThis(), "getdatascales", model);

	if (!dao_array_isset_fetch(return_value, &data, attribute, PH_NOISY|PH_COPY)) {
		RETVAL_NULL();
	}
	zval_ptr_dtor(&data);
}

/**
 * Returns attributes which types are numerical
 *
 *<code>
 *	print_r($metaData->getDataTypesNumeric(new Robots()));
 *</code>
 *
 * @param  Dao\Mvc\ModelInterface $model
 * @return array
 */
PHP_METHOD(Dao_Mvc_Model_MetaData, getDataTypesNumeric){

	zval *model, index = {};

	dao_fetch_params(0, 1, 0, &model);

	ZVAL_LONG(&index, DAO_MVC_MODEL_METADATA_MODELS_DATA_TYPES_NUMERIC);

	DAO_CALL_METHOD(return_value, getThis(), "readmetadataindex", model, &index);
	if (Z_TYPE_P(return_value) != IS_ARRAY) {
		DAO_THROW_EXCEPTION_STR(dao_mvc_model_exception_ce, "The meta-data is invalid or is corrupt");
		return;
	}
}

/**
 * Checks if the attribute is numerical
 *
 *<code>
 *	var_dump($metaData->isNumeric(new Robots(), 'id'));
 *</code>
 *
 * @param Dao\Mvc\ModelInterface $model
 * @param string $attribute
 * @return int
 */
PHP_METHOD(Dao_Mvc_Model_MetaData, isNumeric){

	zval *model, *attribute, data = {};

	dao_fetch_params(0, 1, 0, &model, &attribute);

	DAO_CALL_METHOD(&data, getThis(), "getdatatypesnumeric", model);

	if (dao_array_isset(&data, attribute)) {
		RETVAL_TRUE;
	} else {
		RETVAL_FALSE;
	}
	zval_ptr_dtor(&data);
}

/**
 * Returns the name of identity field (if one is present)
 *
 *<code>
 *	print_r($metaData->getIdentityField(new Robots()));
 *</code>
 *
 * @param  Dao\Mvc\ModelInterface $model
 * @return string
 */
PHP_METHOD(Dao_Mvc_Model_MetaData, getIdentityField){

	zval *model, index = {};

	dao_fetch_params(0, 1, 0, &model);

	ZVAL_LONG(&index, DAO_MVC_MODEL_METADATA_MODELS_IDENTITY_COLUMN);

	DAO_CALL_METHOD(return_value, getThis(), "readmetadataindex", model, &index);
}

/**
 * Returns attributes and their bind data types
 *
 *<code>
 *	print_r($metaData->getBindTypes(new Robots()));
 *</code>
 *
 * @param Dao\Mvc\ModelInterface $model
 * @return array
 */
PHP_METHOD(Dao_Mvc_Model_MetaData, getBindTypes){

	zval *model, index = {};

	dao_fetch_params(0, 1, 0, &model);

	ZVAL_LONG(&index, DAO_MVC_MODEL_METADATA_MODELS_DATA_TYPES_BIND);

	DAO_CALL_METHOD(return_value, getThis(), "readmetadataindex", model, &index);
	if (Z_TYPE_P(return_value) != IS_ARRAY) {
		DAO_THROW_EXCEPTION_STR(dao_mvc_model_exception_ce, "The meta-data is invalid or is corrupt");
		return;
	}
}

/**
 * Returns attributes and their default values
 *
 *<code>
 *	print_r($metaData->getDefaultValues(new Robots()));
 *</code>
 *
 * @param Dao\Mvc\ModelInterface $model
 * @return array
 */
PHP_METHOD(Dao_Mvc_Model_MetaData, getDefaultValues){

	zval *model, index = {};

	dao_fetch_params(0, 1, 0, &model);

	ZVAL_LONG(&index, 12);

	DAO_CALL_METHOD(return_value, getThis(), "readmetadataindex", model, &index);
	if (Z_TYPE_P(return_value) != IS_ARRAY) {
		DAO_THROW_EXCEPTION_STR(dao_mvc_model_exception_ce, "The meta-data is invalid or is corrupt");
		return;
	}
}

/**
 * Returns attributes that must be ignored from the INSERT SQL generation
 *
 *<code>
 *	print_r($metaData->getAutomaticCreateAttributes(new Robots()));
 *</code>
 *
 * @param Dao\Mvc\ModelInterface $model
 * @return array
 */
PHP_METHOD(Dao_Mvc_Model_MetaData, getAutomaticCreateAttributes){

	zval *model, index = {};

	dao_fetch_params(0, 1, 0, &model);

	ZVAL_LONG(&index, 10);

	DAO_CALL_METHOD(return_value, getThis(), "readmetadataindex", model, &index);
	if (Z_TYPE_P(return_value) != IS_ARRAY) {
		DAO_THROW_EXCEPTION_STR(dao_mvc_model_exception_ce, "The meta-data is invalid or is corrupt");
		return;
	}
}

/**
 * Returns attributes that must be ignored from the UPDATE SQL generation
 *
 *<code>
 *	print_r($metaData->getAutomaticUpdateAttributes(new Robots()));
 *</code>
 *
 * @param Dao\Mvc\ModelInterface $model
 * @return array
 */
PHP_METHOD(Dao_Mvc_Model_MetaData, getAutomaticUpdateAttributes){

	zval *model, index = {};

	dao_fetch_params(0, 1, 0, &model);

	ZVAL_LONG(&index, 11);

	DAO_CALL_METHOD(return_value, getThis(), "readmetadataindex", model, &index);
	if (Z_TYPE_P(return_value) != IS_ARRAY) {
		DAO_THROW_EXCEPTION_STR(dao_mvc_model_exception_ce, "The meta-data is invalid or is corrupt");
		return;
	}
}

/**
 * Set the attributes that must be ignored from the INSERT SQL generation
 *
 *<code>
 *	$metaData->setAutomaticCreateAttributes(new Robots(), array('created_at' => true));
 *</code>
 *
 * @param  Dao\Mvc\ModelInterface $model
 * @param  array $attributes
 */
PHP_METHOD(Dao_Mvc_Model_MetaData, setAutomaticCreateAttributes){

	zval *model, *attributes, *replace, index = {};

	dao_fetch_params(0, 3, 0, &model, &attributes, &replace);

	ZVAL_LONG(&index, 10);
	DAO_CALL_METHOD(NULL, getThis(), "writemetadataindex", model, &index, attributes, replace);
}

/**
 * Set the attributes that must be ignored from the UPDATE SQL generation
 *
 *<code>
 *	$metaData->setAutomaticUpdateAttributes(new Robots(), array('modified_at' => true));
 *</code>
 *
 * @param  Dao\Mvc\ModelInterface $model
 * @param  array $attributes
 */
PHP_METHOD(Dao_Mvc_Model_MetaData, setAutomaticUpdateAttributes){

	zval *model, *attributes, *replace, index = {};

	dao_fetch_params(0, 3, 0, &model, &attributes, &replace);

	ZVAL_LONG(&index, 11);
	DAO_CALL_METHOD(NULL, getThis(), "writemetadataindex", model, &index, attributes, replace);
}

/**
 * Returns the column map if any
 *
 *<code>
 *	print_r($metaData->getColumnMap(new Robots()));
 *</code>
 *
 * @param Dao\Mvc\ModelInterface $model
 * @return array
 */
PHP_METHOD(Dao_Mvc_Model_MetaData, getColumnMap){

	zval *model, index = {};

	dao_fetch_params(0, 1, 0, &model);

	ZVAL_LONG(&index, DAO_MVC_MODEL_METADATA_MODELS_COLUMN_MAP);

	DAO_CALL_METHOD(return_value, getThis(), "readcolumnmapindex", model, &index);
	if (Z_TYPE_P(return_value) != IS_NULL) {
		if (Z_TYPE_P(return_value) != IS_ARRAY) {
			DAO_THROW_EXCEPTION_STR(dao_mvc_model_exception_ce, "The meta-data is invalid or is corrupt");
			return;
		}
	}
}

/**
 * Returns the reverse column map if any
 *
 *<code>
 *	print_r($metaData->getReverseColumnMap(new Robots()));
 *</code>
 *
 * @param Dao\Mvc\ModelInterface $model
 * @return array
 */
PHP_METHOD(Dao_Mvc_Model_MetaData, getReverseColumnMap){

	zval *model, index = {};

	dao_fetch_params(0, 1, 0, &model);

	ZVAL_LONG(&index, DAO_MVC_MODEL_METADATA_MODELS_REVERSE_COLUMN_MAP);

	DAO_CALL_METHOD(return_value, getThis(), "readcolumnmapindex", model, &index);
	if (Z_TYPE_P(return_value) != IS_NULL) {
		if (Z_TYPE_P(return_value) != IS_ARRAY) {
			DAO_THROW_EXCEPTION_STR(dao_mvc_model_exception_ce, "The meta-data is invalid or is corrupt");
			return;
		}
	}
}

/**
 * Check if a model has certain attribute
 *
 *<code>
 *	var_dump($metaData->hasAttribute(new Robots(), 'name'));
 *</code>
 *
 * @param Dao\Mvc\ModelInterface $model
 * @param string $attribute
 * @return boolean
 */
PHP_METHOD(Dao_Mvc_Model_MetaData, hasAttribute){

	zval *model, *attribute, reverse_column_map = {}, column_map = {}, meta_data = {}, data_types = {};

	dao_fetch_params(1, 2, 0, &model, &attribute);

	if (Z_TYPE_P(attribute) != IS_STRING) {
		DAO_MM_THROW_EXCEPTION_STR(dao_mvc_model_exception_ce, "Attribute must be a string");
		return;
	}

	DAO_MM_CALL_METHOD(&reverse_column_map, getThis(), "getreversecolumnmap", model);
	DAO_MM_ADD_ENTRY(&reverse_column_map);
	if (Z_TYPE(reverse_column_map) == IS_ARRAY && DAO_IS_NOT_EMPTY(&reverse_column_map)) {
		if (dao_array_isset(&reverse_column_map, attribute)) {
			RETURN_MM_TRUE;
		}
		RETURN_MM_FALSE;
	}

	DAO_MM_CALL_METHOD(&column_map, getThis(), "getcolumnmap", model);
	DAO_MM_ADD_ENTRY(&column_map);
	if (Z_TYPE(column_map) == IS_ARRAY && DAO_IS_NOT_EMPTY(&column_map)) {
		if (dao_fast_in_array(attribute, &column_map)) {
			RETURN_MM_TRUE;
		}
		RETURN_MM_FALSE;
	}

	DAO_MM_CALL_METHOD(&meta_data, getThis(), "readmetadata", model);

	dao_array_fetch_long(&data_types, &meta_data, DAO_MVC_MODEL_METADATA_MODELS_DATA_TYPES, PH_NOISY|PH_READONLY);
	if (dao_array_isset(&data_types, attribute)) {
		RETURN_MM_TRUE;
	}
	RETURN_MM_FALSE;
}

/**
 * Gets a model certain attribute
 *
 *<code>
 *	var_dump($metaData->getAttribute(new Robots(), 'name'));
 *</code>
 *
 * @param Dao\Mvc\ModelInterface $model
 * @param string $column
 * @return string
 */
PHP_METHOD(Dao_Mvc_Model_MetaData, getAttribute){

	zval *model, *column, column_map = {}, meta_data = {}, data_types = {};

	dao_fetch_params(1, 2, 0, &model, &column);

	if (Z_TYPE_P(column) != IS_STRING) {
		DAO_MM_THROW_EXCEPTION_STR(dao_mvc_model_exception_ce, "Column must be a string");
		return;
	}

	DAO_MM_CALL_METHOD(&column_map, getThis(), "getcolumnmap", model);
	DAO_MM_ADD_ENTRY(&column_map);
	if (Z_TYPE(column_map) == IS_ARRAY && DAO_IS_NOT_EMPTY(&column_map)) {
		if (!dao_array_isset_fetch(return_value, &column_map, column, PH_COPY)) {
			RETURN_MM_FALSE;
		}
		RETURN_MM();
	}

	DAO_MM_CALL_METHOD(&meta_data, getThis(), "readmetadata", model);
	DAO_MM_ADD_ENTRY(&meta_data);
	dao_array_fetch_long(&data_types, &meta_data, DAO_MVC_MODEL_METADATA_MODELS_DATA_TYPES, PH_NOISY|PH_READONLY);
	if (dao_array_isset(&data_types, column)) {
		RETURN_MM_CTOR(column);
	}
	RETURN_MM_FALSE;
}

/**
 * Check if a model has real attribute name
 *
 *<code>
 *	var_dump($metaData->hasRealAttribute(new Robots(), 'name'));
 *</code>
 *
 * @param Dao\Mvc\ModelInterface $model
 * @param string $column
 * @return boolean
 */
PHP_METHOD(Dao_Mvc_Model_MetaData, hasRealAttribute){

	zval *model, *attribute, column_map = {}, reverse_column_map = {}, meta_data = {}, data_types = {};

	dao_fetch_params(1, 2, 0, &model, &attribute);

	if (Z_TYPE_P(attribute) != IS_STRING) {
		DAO_MM_THROW_EXCEPTION_STR(dao_mvc_model_exception_ce, "Attribute must be a string");
		return;
	}

	DAO_MM_CALL_METHOD(&column_map, getThis(), "getcolumnmap", model);
	DAO_MM_ADD_ENTRY(&column_map);
	if (Z_TYPE(column_map) == IS_ARRAY && DAO_IS_NOT_EMPTY(&column_map)) {
		if (dao_array_isset(&column_map, attribute)) {
			RETURN_MM_TRUE;
		}
		RETURN_MM_FALSE;
	}

	DAO_MM_CALL_METHOD(&reverse_column_map, getThis(), "getreversecolumnmap", model);
	DAO_MM_ADD_ENTRY(&reverse_column_map);
	if (Z_TYPE(reverse_column_map) == IS_ARRAY && DAO_IS_NOT_EMPTY(&reverse_column_map)) {
		if (dao_fast_in_array(attribute, &reverse_column_map)) {
			RETURN_MM_TRUE;
		}
		RETURN_MM_FALSE;
	}

	DAO_MM_CALL_METHOD(&meta_data, getThis(), "readmetadata", model);
	DAO_MM_ADD_ENTRY(&meta_data);
	dao_array_fetch_long(&data_types, &meta_data, DAO_MVC_MODEL_METADATA_MODELS_DATA_TYPES, PH_NOISY|PH_READONLY);
	if (dao_array_isset(&data_types, attribute)) {
		RETURN_MM_TRUE;
	}
	RETURN_MM_FALSE;
}

/**
 * Gets a real attribute name
 *
 *<code>
 *	var_dump($metaData->getRealAttribute(new Robots(), 'name'));
 *</code>
 *
 * @param Dao\Mvc\ModelInterface $model
 * @param string $column
 * @return string
 */
PHP_METHOD(Dao_Mvc_Model_MetaData, getRealAttribute){

	zval *model, *column, reverse_column_map = {}, meta_data = {}, data_types = {};

	dao_fetch_params(1, 2, 0, &model, &column);

	if (Z_TYPE_P(column) != IS_STRING) {
		DAO_MM_THROW_EXCEPTION_STR(dao_mvc_model_exception_ce, "Column must be a string");
		return;
	}

	DAO_MM_CALL_METHOD(&reverse_column_map, getThis(), "getreversecolumnmap", model);
	DAO_MM_ADD_ENTRY(&reverse_column_map);
	if (Z_TYPE(reverse_column_map) == IS_ARRAY && DAO_IS_NOT_EMPTY(&reverse_column_map)) {
		if (!dao_array_isset_fetch(return_value, &reverse_column_map, column, PH_COPY)) {
			RETVAL_FALSE;
		}
		RETURN_MM();
	}

	DAO_MM_CALL_METHOD(&meta_data, getThis(), "readmetadata", model);
	DAO_MM_ADD_ENTRY(&meta_data);

	dao_array_fetch_long(&data_types, &meta_data, DAO_MVC_MODEL_METADATA_MODELS_DATA_TYPES, PH_NOISY|PH_READONLY);
	if (dao_array_isset(&data_types, column)) {
		RETURN_MM_CTOR(column);
	}
	RETURN_MM_FALSE;
}

/**
 * Checks if the internal meta-data container is empty
 *
 *<code>
 *	var_dump($metaData->isEmpty());
 *</code>
 *
 * @return boolean
 */
PHP_METHOD(Dao_Mvc_Model_MetaData, isEmpty){

	zval meta_data = {};

	dao_read_property(&meta_data, getThis(), SL("_metaData"), PH_NOISY|PH_READONLY);
	if (dao_fast_count_ev(&meta_data)) {
		RETURN_FALSE;
	}

	RETURN_TRUE;
}

/**
 * Resets internal meta-data in order to regenerate it
 *
 *<code>
 *	$metaData->reset();
 *</code>
 */
PHP_METHOD(Dao_Mvc_Model_MetaData, reset){

	dao_update_property_empty_array(getThis(), SL("_metaData"));
	dao_update_property_empty_array(getThis(), SL("_columnMap"));
}
