
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

#include "mvc/model/relationinterface.h"
#include "kernel/main.h"

zend_class_entry *dao_mvc_model_relationinterface_ce;

static const zend_function_entry dao_mvc_model_relationinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Mvc_Model_RelationInterface, setIntermediateRelation, arginfo_dao_mvc_model_relationinterface_setintermediaterelation)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_RelationInterface, getType, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_RelationInterface, getReferencedModel, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_RelationInterface, getFields, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_RelationInterface, getReferencedFields, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_RelationInterface, getOptions, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_RelationInterface, isForeignKey, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_RelationInterface, getForeignKey, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_RelationInterface, isThrough, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_RelationInterface, getIntermediateFields, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_RelationInterface, getIntermediateModel, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_RelationInterface, getIntermediateReferencedFields, arginfo_empty)
	PHP_FE_END
};

/**
 * Dao\Mvc\Model\RelationInterface initializer
 */
DAO_INIT_CLASS(Dao_Mvc_Model_RelationInterface){

	DAO_REGISTER_INTERFACE(Dao\\Mvc\\Model, RelationInterface, mvc_model_relationinterface, dao_mvc_model_relationinterface_method_entry);

	return SUCCESS;
}

/**
 * Sets the intermediate model dat for has-*-through relations
 *
 * @param string|array $intermediateFields
 * @param string $intermediateModel
 * @param string $intermediateReferencedFields
 */
DAO_DOC_METHOD(Dao_Mvc_Model_RelationInterface, setIntermediateRelation);

/**
 * Returns the relations type
 *
 * @return int
 */
DAO_DOC_METHOD(Dao_Mvc_Model_RelationInterface, getType);

/**
 * Returns the referenced model
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Mvc_Model_RelationInterface, getReferencedModel);

/**
 * Returns the fields
 *
 * @return string|array
 */
DAO_DOC_METHOD(Dao_Mvc_Model_RelationInterface, getFields);

/**
 * Returns the referenced fields
 *
 * @return string|array
 */
DAO_DOC_METHOD(Dao_Mvc_Model_RelationInterface, getReferencedFields);

/**
 * Returns the options
 *
 * @return string|array
 */
DAO_DOC_METHOD(Dao_Mvc_Model_RelationInterface, getOptions);

/**
 * Check whether the relation act as a foreign key
 *
 * @return string|array
 */
DAO_DOC_METHOD(Dao_Mvc_Model_RelationInterface, isForeignKey);

/**
 * Returns the foreign key configuration
 *
 * @return string|array
 */
DAO_DOC_METHOD(Dao_Mvc_Model_RelationInterface, getForeignKey);

/**
 * Check whether the relation is a 'many-to-many' relation or not
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Mvc_Model_RelationInterface, isThrough);

/**
 * Gets the intermediate fields for has-*-through relations
 *
 * @return string|array
 */
DAO_DOC_METHOD(Dao_Mvc_Model_RelationInterface, getIntermediateFields);

/**
 * Gets the intermediate model for has-*-through relations
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Mvc_Model_RelationInterface, getIntermediateModel);

/**
 * Gets the intermediate referenced fields for has-*-through relations
 *
 * @return string|array
 */
DAO_DOC_METHOD(Dao_Mvc_Model_RelationInterface, getIntermediateReferencedFields);
