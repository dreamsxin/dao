
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

#include "mvc/model/managerinterface.h"
#include "kernel/main.h"

zend_class_entry *dao_mvc_model_managerinterface_ce;

static const zend_function_entry dao_mvc_model_managerinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Mvc_Model_ManagerInterface, initialize, arginfo_dao_mvc_model_managerinterface_initialize)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_ManagerInterface, isInitialized, arginfo_dao_mvc_model_managerinterface_isinitialized)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_ManagerInterface, getLastInitialized, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_ManagerInterface, load, arginfo_dao_mvc_model_managerinterface_load)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_ManagerInterface, addHasOne, arginfo_dao_mvc_model_managerinterface_addhasone)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_ManagerInterface, addBelongsTo, arginfo_dao_mvc_model_managerinterface_addbelongsto)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_ManagerInterface, addHasMany, arginfo_dao_mvc_model_managerinterface_addhasmany)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_ManagerInterface, existsBelongsTo, arginfo_dao_mvc_model_managerinterface_existsbelongsto)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_ManagerInterface, existsHasMany, arginfo_dao_mvc_model_managerinterface_existshasmany)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_ManagerInterface, existsHasOne, arginfo_dao_mvc_model_managerinterface_existshasone)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_ManagerInterface, getBelongsToRecords, arginfo_dao_mvc_model_managerinterface_getbelongstorecords)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_ManagerInterface, getHasManyRecords, arginfo_dao_mvc_model_managerinterface_gethasmanyrecords)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_ManagerInterface, getHasOneRecords, arginfo_dao_mvc_model_managerinterface_gethasonerecords)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_ManagerInterface, getBelongsTo, arginfo_dao_mvc_model_managerinterface_getbelongsto)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_ManagerInterface, getHasMany, arginfo_dao_mvc_model_managerinterface_gethasmany)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_ManagerInterface, getHasOne, arginfo_dao_mvc_model_managerinterface_gethasone)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_ManagerInterface, getHasOneAndHasMany, arginfo_dao_mvc_model_managerinterface_gethasoneandhasmany)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_ManagerInterface, getRelations, arginfo_dao_mvc_model_managerinterface_getrelations)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_ManagerInterface, getRelationsBetween, arginfo_dao_mvc_model_managerinterface_getrelationsbetween)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_ManagerInterface, createQuery, arginfo_dao_mvc_model_managerinterface_createquery)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_ManagerInterface, executeQuery, arginfo_dao_mvc_model_managerinterface_executequery)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_ManagerInterface, createBuilder, arginfo_dao_mvc_model_managerinterface_createbuilder)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_ManagerInterface, addBehavior, arginfo_dao_mvc_model_managerinterface_addbehavior)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_ManagerInterface, notifyEvent, arginfo_dao_mvc_model_managerinterface_notifyevent)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_ManagerInterface, missingMethod, arginfo_dao_mvc_model_managerinterface_missingmethod)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_ManagerInterface, getLastQuery, arginfo_empty)
	PHP_FE_END
};

/**
 * Dao\Mvc\Model\ManagerInterface initializer
 */
DAO_INIT_CLASS(Dao_Mvc_Model_ManagerInterface){

	DAO_REGISTER_INTERFACE(Dao\\Mvc\\Model, ManagerInterface, mvc_model_managerinterface, dao_mvc_model_managerinterface_method_entry);

	return SUCCESS;
}

/**
 * Initializes a model in the model manager
 *
 * @param Dao\Mvc\ModelInterface $model
 */
DAO_DOC_METHOD(Dao_Mvc_Model_ManagerInterface, initialize);

/**
 * Check of a model is already initialized
 *
 * @param string $modelName
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Mvc_Model_ManagerInterface, isInitialized);

/**
 * Get last initialized model
 *
 * @return Dao\Mvc\ModelInterface
 */
DAO_DOC_METHOD(Dao_Mvc_Model_ManagerInterface, getLastInitialized);

/**
 * Loads a model throwing an exception if it doesn't exist
 *
 * @param string $modelName
 * @param boolean $newInstance
 * @return Dao\Mvc\ModelInterface
 */
DAO_DOC_METHOD(Dao_Mvc_Model_ManagerInterface, load);

/**
 * Setup a 1-1 relation between two models
 *
 * @param   Dao\Mvc\ModelInterface $model
 * @param mixed $fields
 * @param string $referencedModel
 * @param mixed $referencedFields
 * @param array $options
 * @return  Dao\Mvc\Model\RelationInterface
 */
DAO_DOC_METHOD(Dao_Mvc_Model_ManagerInterface, addHasOne);

/**
 * Setup a relation reverse 1-1  between two models
 *
 * @param 	Dao\Mvc\ModelInterface $model
 * @param mixed $fields
 * @param string $referencedModel
 * @param mixed $referencedFields
 * @param array $options
 * @return 	Dao\Mvc\Model\RelationInterface
 */
DAO_DOC_METHOD(Dao_Mvc_Model_ManagerInterface, addBelongsTo);

/**
 * Setup a relation 1-n between two models
 *
 * @param 	Dao\Mvc\ModelInterface $model
 * @param mixed $fields
 * @param string $referencedModel
 * @param mixed $referencedFields
 * @param array $options
 * @return 	Dao\Mvc\Model\RelationInterface
 */
DAO_DOC_METHOD(Dao_Mvc_Model_ManagerInterface, addHasMany);

/**
 * Checks whether a model has a belongsTo relation with another model
 *
 * @param 	string $modelName
 * @param 	string $modelRelation
 * @return 	boolean
 */
DAO_DOC_METHOD(Dao_Mvc_Model_ManagerInterface, existsBelongsTo);

/**
 * Checks whether a model has a hasMany relation with another model
 *
 * @param 	string $modelName
 * @param 	string $modelRelation
 * @return 	boolean
 */
DAO_DOC_METHOD(Dao_Mvc_Model_ManagerInterface, existsHasMany);

/**
 * Checks whether a model has a hasOne relation with another model
 *
 * @param 	string $modelName
 * @param 	string $modelRelation
 * @return 	boolean
 */
DAO_DOC_METHOD(Dao_Mvc_Model_ManagerInterface, existsHasOne);

/**
 * Gets belongsTo related records from a model
 *
 * @param string $method
 * @param string $modelName
 * @param string $modelRelation
 * @param Dao\Mvc\Model $record
 * @param array $parameters
 * @return Dao\Mvc\Model\ResultsetInterface
 */
DAO_DOC_METHOD(Dao_Mvc_Model_ManagerInterface, getBelongsToRecords);

/**
 * Gets hasMany related records from a model
 *
 * @param string $method
 * @param string $modelName
 * @param string $modelRelation
 * @param Dao\Mvc\Model $record
 * @param array $parameters
 * @return Dao\Mvc\Model\ResultsetInterface
 */
DAO_DOC_METHOD(Dao_Mvc_Model_ManagerInterface, getHasManyRecords);

/**
 * Gets belongsTo related records from a model
 *
 * @param string $method
 * @param string $modelName
 * @param string $modelRelation
 * @param Dao\Mvc\Model $record
 * @param array $parameters
 * @return Dao\Mvc\Model\ResultsetInterface
 */
DAO_DOC_METHOD(Dao_Mvc_Model_ManagerInterface, getHasOneRecords);

/**
 * Gets belongsTo relations defined on a model
 *
 * @param  Dao\Mvc\ModelInterface $model
 * @return array
 */
DAO_DOC_METHOD(Dao_Mvc_Model_ManagerInterface, getBelongsTo);

/**
 * Gets hasMany relations defined on a model
 *
 * @param  Dao\Mvc\ModelInterface $model
 * @return array
 */
DAO_DOC_METHOD(Dao_Mvc_Model_ManagerInterface, getHasMany);

/**
 * Gets hasOne relations defined on a model
 *
 * @param  Dao\Mvc\ModelInterface $model
 * @return array
 */
DAO_DOC_METHOD(Dao_Mvc_Model_ManagerInterface, getHasOne);

/**
 * Gets hasOne relations defined on a model
 *
 * @param  Dao\Mvc\ModelInterface $model
 * @return array
 */
DAO_DOC_METHOD(Dao_Mvc_Model_ManagerInterface, getHasOneAndHasMany);

/**
 * Query all the relationships defined on a model
 *
 * @param string $modelName
 * @return Dao\Mvc\Model\RelationInterface[]
 */
DAO_DOC_METHOD(Dao_Mvc_Model_ManagerInterface, getRelations);

/**
 * Query the relations between two models
 *
 * @param string $first
 * @param string $second
 * @return array
 */
DAO_DOC_METHOD(Dao_Mvc_Model_ManagerInterface, getRelationsBetween);

/**
 * Creates a Dao\Mvc\Model\Query without execute it
 *
 * @param string $phql
 * @return Dao\Mvc\Model\QueryInterface
 */
DAO_DOC_METHOD(Dao_Mvc_Model_ManagerInterface, createQuery);

/**
 * Creates a Dao\Mvc\Model\Query and execute it
 *
 * @param string $phql
 * @param array $placeholders
 * @return Dao\Mvc\Model\QueryInterface
 */
DAO_DOC_METHOD(Dao_Mvc_Model_ManagerInterface, executeQuery);

/**
 * Creates a Dao\Mvc\Model\Query\Builder\Select
 *
 * @param string $params
 * @return Dao\Mvc\Model\Query\BuilderInterface
 */
DAO_DOC_METHOD(Dao_Mvc_Model_ManagerInterface, createBuilder);

/**
 * Binds a behavior to a model
 *
 * @param Dao\Mvc\ModelInterface $model
 * @param Dao\Mvc\Model\BehaviorInterface $behavior
 */
DAO_DOC_METHOD(Dao_Mvc_Model_ManagerInterface, addBehavior);

/**
 * Receives events generated in the models and dispatches them to a events-manager if available
 * Notify the behaviors that are listening in the model
 *
 * @param string $eventName
 * @param Dao\Mvc\ModelInterface $model
 */
DAO_DOC_METHOD(Dao_Mvc_Model_ManagerInterface, notifyEvent);

/**
 * Dispatch a event to the listeners and behaviors
 * This method expects that the endpoint listeners/behaviors returns true
 * meaning that a least one is implemented
 *
 * @param Dao\Mvc\ModelInterface $model
 * @param string $eventName
 * @param array $data
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Mvc_Model_ManagerInterface, missingMethod);

/**
 * Returns the last query created or executed in the
 *
 * @return Dao\Mvc\Model\QueryInterface
 */
DAO_DOC_METHOD(Dao_Mvc_Model_ManagerInterface, getLastQuery);
