
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

#include "mvc/model/metadatainterface.h"
#include "kernel/main.h"

zend_class_entry *dao_mvc_model_metadatainterface_ce;

static const zend_function_entry dao_mvc_model_metadatainterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Mvc_Model_MetaDataInterface, setStrategy, arginfo_dao_mvc_model_metadatainterface_setstrategy)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_MetaDataInterface, getStrategy, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_MetaDataInterface, readMetaData, arginfo_dao_mvc_model_metadatainterface_readmetadata)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_MetaDataInterface, readMetaDataIndex, arginfo_dao_mvc_model_metadatainterface_readmetadataindex)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_MetaDataInterface, writeMetaDataIndex, arginfo_dao_mvc_model_metadatainterface_writemetadataindex)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_MetaDataInterface, readColumnMap, arginfo_dao_mvc_model_metadatainterface_readcolumnmap)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_MetaDataInterface, readColumnMapIndex, arginfo_dao_mvc_model_metadatainterface_readcolumnmapindex)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_MetaDataInterface, getAttributes, arginfo_dao_mvc_model_metadatainterface_getattributes)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_MetaDataInterface, getPrimaryKeyAttributes, arginfo_dao_mvc_model_metadatainterface_getprimarykeyattributes)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_MetaDataInterface, getNonPrimaryKeyAttributes, arginfo_dao_mvc_model_metadatainterface_getnonprimarykeyattributes)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_MetaDataInterface, getNotNullAttributes, arginfo_dao_mvc_model_metadatainterface_getnotnullattributes)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_MetaDataInterface, getDataTypes, arginfo_dao_mvc_model_metadatainterface_getdatatypes)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_MetaDataInterface, getDataType, arginfo_dao_mvc_model_metadatainterface_getdatatype)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_MetaDataInterface, getDataSizes, arginfo_dao_mvc_model_metadatainterface_getdatasizes)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_MetaDataInterface, getDataSize, arginfo_dao_mvc_model_metadatainterface_getdatasize)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_MetaDataInterface, getDataBytes, arginfo_dao_mvc_model_metadatainterface_getdatabytes)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_MetaDataInterface, getDataByte, arginfo_dao_mvc_model_metadatainterface_getdatabyte)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_MetaDataInterface, getDataScales, arginfo_dao_mvc_model_metadatainterface_getdatascales)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_MetaDataInterface, getDataScale, arginfo_dao_mvc_model_metadatainterface_getdatascale)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_MetaDataInterface, getDataTypesNumeric, arginfo_dao_mvc_model_metadatainterface_getdatatypesnumeric)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_MetaDataInterface, getIdentityField, arginfo_dao_mvc_model_metadatainterface_getidentityfield)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_MetaDataInterface, getBindTypes, arginfo_dao_mvc_model_metadatainterface_getbindtypes)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_MetaDataInterface, getAutomaticCreateAttributes, arginfo_dao_mvc_model_metadatainterface_getautomaticcreateattributes)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_MetaDataInterface, getAutomaticUpdateAttributes, arginfo_dao_mvc_model_metadatainterface_getautomaticupdateattributes)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_MetaDataInterface, setAutomaticCreateAttributes, arginfo_dao_mvc_model_metadatainterface_setautomaticcreateattributes)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_MetaDataInterface, setAutomaticUpdateAttributes, arginfo_dao_mvc_model_metadatainterface_setautomaticupdateattributes)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_MetaDataInterface, getColumnMap, arginfo_dao_mvc_model_metadatainterface_getcolumnmap)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_MetaDataInterface, getReverseColumnMap, arginfo_dao_mvc_model_metadatainterface_getreversecolumnmap)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_MetaDataInterface, hasAttribute, arginfo_dao_mvc_model_metadatainterface_hasattribute)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_MetaDataInterface, isEmpty, arginfo_dao_mvc_model_metadatainterface_isempty)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_MetaDataInterface, reset, arginfo_dao_mvc_model_metadatainterface_reset)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_MetaDataInterface, read, arginfo_dao_mvc_model_metadatainterface_read)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_MetaDataInterface, write, arginfo_dao_mvc_model_metadatainterface_write)
	PHP_FE_END
};

/**
 * Dao\Mvc\Model\MetaDataInterface initializer
 */
DAO_INIT_CLASS(Dao_Mvc_Model_MetaDataInterface){

	DAO_REGISTER_INTERFACE(Dao\\Mvc\\Model, MetaDataInterface, mvc_model_metadatainterface, dao_mvc_model_metadatainterface_method_entry);

	return SUCCESS;
}

/**
 * Set the meta-data extraction strategy
 *
 * @param Dao\Mvc\Model\MetaData\Strategy\Introspection $strategy
 */
DAO_DOC_METHOD(Dao_Mvc_Model_MetaDataInterface, setStrategy);

/**
 * Return the strategy to obtain the meta-data
 *
 * @return Dao\Mvc\Model\MetaData\Strategy\Introspection
 */
DAO_DOC_METHOD(Dao_Mvc_Model_MetaDataInterface, getStrategy);

/**
 * Reads meta-data for certain model
 *
 * @param Dao\Mvc\ModelInterface $model
 * @return array
 */
DAO_DOC_METHOD(Dao_Mvc_Model_MetaDataInterface, readMetaData);

/**
 * Reads meta-data for certain model using a MODEL_* constant
 *
 * @param Dao\Mvc\ModelInterface $model
 * @param int $index
 * @return mixed
 */
DAO_DOC_METHOD(Dao_Mvc_Model_MetaDataInterface, readMetaDataIndex);

/**
 * Writes meta-data for certain model using a MODEL_* constant
 *
 * @param Dao\Mvc\Model $model
 * @param int $index
 * @param mixed $data
 */
DAO_DOC_METHOD(Dao_Mvc_Model_MetaDataInterface, writeMetaDataIndex);

/**
 * Reads the ordered/reversed column map for certain model
 *
 * @param Dao\Mvc\ModelInterface $model
 * @return array
 */
DAO_DOC_METHOD(Dao_Mvc_Model_MetaDataInterface, readColumnMap);

/**
 * Reads column-map information for certain model using a MODEL_* constant
 *
 * @param Dao\Mvc\ModelInterface $model
 * @param int $index
 */
DAO_DOC_METHOD(Dao_Mvc_Model_MetaDataInterface, readColumnMapIndex);

/**
 * Returns table attributes names (fields)
 *
 * @param Dao\Mvc\ModelInterface $model
 * @return 	array
 */
DAO_DOC_METHOD(Dao_Mvc_Model_MetaDataInterface, getAttributes);

/**
 * Returns an array of fields which are part of the primary key
 *
 * @param Dao\Mvc\ModelInterface $model
 * @return array
 */
DAO_DOC_METHOD(Dao_Mvc_Model_MetaDataInterface, getPrimaryKeyAttributes);

/**
 * Returns an arrau of fields which are not part of the primary key
 *
 * @param Dao\Mvc\ModelInterface $model
 * @return 	array
 */
DAO_DOC_METHOD(Dao_Mvc_Model_MetaDataInterface, getNonPrimaryKeyAttributes);

/**
 * Returns an array of not null attributes
 *
 * @param  Dao\Mvc\ModelInterface $model
 * @return array
 */
DAO_DOC_METHOD(Dao_Mvc_Model_MetaDataInterface, getNotNullAttributes);

/**
 * Returns attributes and their data types
 *
 * @param Dao\Mvc\ModelInterface $model
 * @return array
 */
DAO_DOC_METHOD(Dao_Mvc_Model_MetaDataInterface, getDataTypes);

/**
 * Returns attributes and their data type
 *
 * @param Dao\Mvc\ModelInterface $model
 * @return int
 */
DAO_DOC_METHOD(Dao_Mvc_Model_MetaDataInterface, getDataType);

/**
 * Returns attributes and their data sizes
 *
 * @param Dao\Mvc\ModelInterface $model
 * @return array
 */
DAO_DOC_METHOD(Dao_Mvc_Model_MetaDataInterface, getDataSizes);

/**
 * Returns attributes and their data size
 *
 * @param Dao\Mvc\ModelInterface $model
 * @return int
 */
DAO_DOC_METHOD(Dao_Mvc_Model_MetaDataInterface, getDataSize);

/**
 * Returns attributes and their data bytes
 *
 * @param Dao\Mvc\ModelInterface $model
 * @return int
 */
DAO_DOC_METHOD(Dao_Mvc_Model_MetaDataInterface, getDataBytes);

/**
 * Returns attributes and their data scales
 *
 * @param Dao\Mvc\ModelInterface $model
 * @return array
 */
DAO_DOC_METHOD(Dao_Mvc_Model_MetaDataInterface, getDataScales);

/**
 * Returns attributes and their data scale
 *
 * @param Dao\Mvc\ModelInterface $model
 * @return int
 */
DAO_DOC_METHOD(Dao_Mvc_Model_MetaDataInterface, getDataScale);

/**
 * Returns attributes which types are numerical
 *
 * @param  Dao\Mvc\ModelInterface $model
 * @return array
 */
DAO_DOC_METHOD(Dao_Mvc_Model_MetaDataInterface, getDataTypesNumeric);

/**
 * Returns the name of identity field (if one is present)
 *
 * @param  Dao\Mvc\ModelInterface $model
 * @return string
 */
DAO_DOC_METHOD(Dao_Mvc_Model_MetaDataInterface, getIdentityField);

/**
 * Returns attributes and their bind data types
 *
 * @param Dao\Mvc\ModelInterface $model
 * @return array
 */
DAO_DOC_METHOD(Dao_Mvc_Model_MetaDataInterface, getBindTypes);

/**
 * Returns attributes that must be ignored from the INSERT SQL generation
 *
 * @param Dao\Mvc\ModelInterface $model
 * @return array
 */
DAO_DOC_METHOD(Dao_Mvc_Model_MetaDataInterface, getAutomaticCreateAttributes);

/**
 * Returns attributes that must be ignored from the UPDATE SQL generation
 *
 * @param Dao\Mvc\ModelInterface $model
 * @return array
 */
DAO_DOC_METHOD(Dao_Mvc_Model_MetaDataInterface, getAutomaticUpdateAttributes);

/**
 * Set the attributes that must be ignored from the INSERT SQL generation
 *
 * @param  Dao\Mvc\ModelInterface $model
 * @param  array $attributes
 */
DAO_DOC_METHOD(Dao_Mvc_Model_MetaDataInterface, setAutomaticCreateAttributes);

/**
 * Set the attributes that must be ignored from the UPDATE SQL generation
 *
 * @param  Dao\Mvc\ModelInterface $model
 * @param  array $attributes
 */
DAO_DOC_METHOD(Dao_Mvc_Model_MetaDataInterface, setAutomaticUpdateAttributes);

/**
 * Returns the column map if any
 *
 * @param Dao\Mvc\ModelInterface $model
 * @return array
 */
DAO_DOC_METHOD(Dao_Mvc_Model_MetaDataInterface, getColumnMap);

/**
 * Returns the reverse column map if any
 *
 * @param Dao\Mvc\ModelInterface $model
 * @return array
 */
DAO_DOC_METHOD(Dao_Mvc_Model_MetaDataInterface, getReverseColumnMap);

/**
 * Check if a model has certain attribute
 *
 * @param Dao\Mvc\ModelInterface $model
 * @param string $attribute
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Mvc_Model_MetaDataInterface, hasAttribute);

/**
 * Checks if the internal meta-data container is empty
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Mvc_Model_MetaDataInterface, isEmpty);

/**
 * Resets internal meta-data in order to regenerate it
 */
DAO_DOC_METHOD(Dao_Mvc_Model_MetaDataInterface, reset);

/**
 * Reads meta-data from the adapter
 *
 * @param string $key
 * @return array
 */
DAO_DOC_METHOD(Dao_Mvc_Model_MetaDataInterface, read);

/**
 * Writes meta-data to the adapter
 *
 * @param string $key
 * @param array $data
 */
DAO_DOC_METHOD(Dao_Mvc_Model_MetaDataInterface, write);
