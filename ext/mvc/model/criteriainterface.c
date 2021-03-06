
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

#include "mvc/model/criteriainterface.h"
#include "kernel/main.h"

zend_class_entry *dao_mvc_model_criteriainterface_ce;

static const zend_function_entry dao_mvc_model_criteriainterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Mvc_Model_CriteriaInterface, setModelName, arginfo_dao_mvc_model_criteriainterface_setmodelname)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_CriteriaInterface, getModelName, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_CriteriaInterface, bind, arginfo_dao_mvc_model_criteriainterface_bind)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_CriteriaInterface, bindTypes, arginfo_dao_mvc_model_criteriainterface_bindtypes)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_CriteriaInterface, columns, arginfo_dao_mvc_model_criteriainterface_columns)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_CriteriaInterface, join, arginfo_dao_mvc_model_criteriainterface_join)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_CriteriaInterface, where, arginfo_dao_mvc_model_criteriainterface_where)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_CriteriaInterface, andWhere, arginfo_dao_mvc_model_criteriainterface_andwhere)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_CriteriaInterface, orWhere, arginfo_dao_mvc_model_criteriainterface_orwhere)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_CriteriaInterface, betweenWhere, arginfo_dao_mvc_model_criteriainterface_betweenwhere)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_CriteriaInterface, notBetweenWhere, arginfo_dao_mvc_model_criteriainterface_notbetweenwhere)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_CriteriaInterface, inWhere, arginfo_dao_mvc_model_criteriainterface_inwhere)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_CriteriaInterface, notInWhere, arginfo_dao_mvc_model_criteriainterface_notinwhere)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_CriteriaInterface, getWhere, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_CriteriaInterface, conditions, arginfo_dao_mvc_model_criteriainterface_conditions)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_CriteriaInterface, getColumns, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_CriteriaInterface, getConditions, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_CriteriaInterface, orderBy, arginfo_dao_mvc_model_criteriainterface_orderby)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_CriteriaInterface, getOrder, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_CriteriaInterface, limit, arginfo_dao_mvc_model_criteriainterface_limit)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_CriteriaInterface, getLimit, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_CriteriaInterface, setUniqueRow, arginfo_dao_mvc_model_criteriainterface_setuniquerow)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_CriteriaInterface, getUniqueRow, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_CriteriaInterface, forUpdate, arginfo_dao_mvc_model_criteriainterface_forupdate)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_CriteriaInterface, sharedLock, arginfo_dao_mvc_model_criteriainterface_sharedlock)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_CriteriaInterface, getParams, arginfo_empty)
	ZEND_FENTRY(fromInput, NULL, arginfo_dao_mvc_model_criteriainterface_frominput, ZEND_ACC_STATIC|ZEND_ACC_ABSTRACT|ZEND_ACC_PUBLIC)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_CriteriaInterface, execute, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_CriteriaInterface, insert, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_CriteriaInterface, update, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_CriteriaInterface, delete, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_CriteriaInterface, getPhql, arginfo_empty)
	PHP_FE_END
};

/**
 * Dao\Mvc\Model\CriteriaInterface initializer
 */
DAO_INIT_CLASS(Dao_Mvc_Model_CriteriaInterface){

	DAO_REGISTER_INTERFACE(Dao\\Mvc\\Model, CriteriaInterface, mvc_model_criteriainterface, dao_mvc_model_criteriainterface_method_entry);

	return SUCCESS;
}

/**
 * Set a model on which the query will be executed
 *
 * @param string $modelName
 * @return Dao\Mvc\Model\CriteriaInterface
 */
DAO_DOC_METHOD(Dao_Mvc_Model_CriteriaInterface, setModelName);

/**
 * Returns an internal model name on which the criteria will be applied
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Mvc_Model_CriteriaInterface, getModelName);

/**
 * Adds the bind parameter to the criteria
 *
 * @param array $bindParams
 * @return Dao\Mvc\Model\CriteriaInterface
 */
DAO_DOC_METHOD(Dao_Mvc_Model_CriteriaInterface, bind);

/**
 * Sets the bind types in the criteria
 * This method replaces all previously set bound parameters
 *
 * @param array $bindTypes
 * @return Dao\Mvc\Model\CriteriaInterface
 */
DAO_DOC_METHOD(Dao_Mvc_Model_CriteriaInterface, bindTypes);

/**
 * Adds the conditions parameter to the criteria
 *
 * @param string $conditions
 * @param array $bindParams
 * @param array $bindTypes
 * @return Dao\Mvc\Model\CriteriaInterface
 */
DAO_DOC_METHOD(Dao_Mvc_Model_CriteriaInterface, where);

/**
 * Adds the conditions parameter to the criteria
 *
 * @param string $conditions
 * @return Dao\Mvc\Model\CriteriaInterface
 */
DAO_DOC_METHOD(Dao_Mvc_Model_CriteriaInterface, conditions);

/**
 * Adds the order-by parameter to the criteria
 *
 * @param string $orderColumns
 * @return Dao\Mvc\Model\CriteriaInterface
 */
DAO_DOC_METHOD(Dao_Mvc_Model_CriteriaInterface, orderBy);

/**
 * Sets the limit parameter to the criteria
 *
 * @param int $limit
 * @param int $offset
 * @return Dao\Mvc\Model\CriteriaInterface
 */
DAO_DOC_METHOD(Dao_Mvc_Model_CriteriaInterface, limit);

/**
 * Sets the "for_update" parameter to the criteria
 *
 * @param boolean $forUpdate
 * @return Dao\Mvc\Model\CriteriaInterface
 */
DAO_DOC_METHOD(Dao_Mvc_Model_CriteriaInterface, forUpdate);

/**
 * Sets the "shared_lock" parameter to the criteria
 *
 * @param boolean $sharedLock
 * @return Dao\Mvc\Model\CriteriaInterface
 */
DAO_DOC_METHOD(Dao_Mvc_Model_CriteriaInterface, sharedLock);

/**
 * Appends a condition to the current conditions using an AND operator
 *
 * @param string $conditions
 * @param array $bindParams
 * @param array $bindTypes
 * @return Dao\Mvc\Model\CriteriaInterface
 */
DAO_DOC_METHOD(Dao_Mvc_Model_CriteriaInterface, andWhere);

/**
 * Appends a condition to the current conditions using an OR operator
 *
 * @param string $conditions
 * @param array $bindParams
 * @param array $bindTypes
 * @return Dao\Mvc\Model\CriteriaInterface
 */
DAO_DOC_METHOD(Dao_Mvc_Model_CriteriaInterface, orWhere);

/**
 * Appends a BETWEEN condition to the current conditions
 *
 *<code>
 *	$criteria->betweenWhere('price', 100.25, 200.50);
 *</code>
 *
 * @param string $expr
 * @param mixed $minimum
 * @param mixed $maximum
 * @return Dao\Mvc\Model\CriteriaInterface
 */
DAO_DOC_METHOD(Dao_Mvc_Model_CriteriaInterface, betweenWhere);

/**
 * Appends a NOT BETWEEN condition to the current conditions
 *
 *<code>
 *	$criteria->notBetweenWhere('price', 100.25, 200.50);
 *</code>
 *
 * @param string $expr
 * @param mixed $minimum
 * @param mixed $maximum
 * @return Dao\Mvc\Model\CriteriaInterface
 */
DAO_DOC_METHOD(Dao_Mvc_Model_CriteriaInterface, notBetweenWhere);

/**
 * Appends an IN condition to the current conditions
 *
 *<code>
 *	$criteria->inWhere('id', [1, 2, 3]);
 *</code>
 *
 * @param string $expr
 * @param array $values
 * @return Dao\Mvc\Model\CriteriaInterface
 */
DAO_DOC_METHOD(Dao_Mvc_Model_CriteriaInterface, inWhere);

/**
 * Appends a NOT IN condition to the current conditions
 *
 *<code>
 *	$criteria->notInWhere('id', [1, 2, 3]);
 *</code>
 *
 * @param string $expr
 * @param array $values
 * @return Dao\Mvc\Model\CriteriaInterface
 */
DAO_DOC_METHOD(Dao_Mvc_Model_CriteriaInterface, notInWhere);

/**
 * Returns the conditions parameter in the criteria
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Mvc_Model_CriteriaInterface, getWhere);

/**
 * Returns the columns to be queried
 *
 * @return string|array
 */
DAO_DOC_METHOD(Dao_Mvc_Model_CriteriaInterface, getColumns);

/**
 * Returns the conditions parameter in the criteria
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Mvc_Model_CriteriaInterface, getConditions);

/**
 * Returns the limit parameter in the criteria
 *
 * @return int
 */
DAO_DOC_METHOD(Dao_Mvc_Model_CriteriaInterface, getLimit);

/**
 * Returns the order parameter in the criteria
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Mvc_Model_CriteriaInterface, getOrder);

/**
 * Returns all the parameters defined in the criteria
 *
 * @return array
 */
DAO_DOC_METHOD(Dao_Mvc_Model_CriteriaInterface, getParams);

/**
 * Builds a Dao\Mvc\Model\Criteria based on an input array like $_POST
 *
 * @param Dao\DiInterface $dependencyInjector
 * @param string $modelName
 * @param array $data
 * @return Dao\Mvc\Model\CriteriaInterface
 */
DAO_DOC_METHOD(Dao_Mvc_Model_CriteriaInterface, fromInput);

/**
 * Executes a find using the parameters built with the criteria
 *
 * @return Dao\Mvc\Model\ResultsetInterface
 */
DAO_DOC_METHOD(Dao_Mvc_Model_CriteriaInterface, execute);

/**
 * Sets the columns to be queried
 *
 *<code>
 *	$criteria->columns(array('id', 'name'));
 *</code>
 *
 * @param string|array $columns
 * @return Dao\Mvc\Model\CriteriaInterface
 */
DAO_DOC_METHOD(Dao_Mvc_Model_CriteriaInterface, columns);

/**
 * Adds a join to the query
 *
 *<code>
 *	$criteria->join('Robots');
 *	$criteria->join('Robots', 'r.id = RobotsParts.robots_id');
 *	$criteria->join('Robots', 'r.id = RobotsParts.robots_id', 'r');
 *	$criteria->join('Robots', 'r.id = RobotsParts.robots_id', 'r', 'LEFT');
 *</code>
 *
 * @param string $model
 * @param string $conditions
 * @param string $alias
 * @param string $type
 * @return Dao\Mvc\Model\CriteriaInterface
 */
DAO_DOC_METHOD(Dao_Mvc_Model_CriteriaInterface, join)

/**
 * Set columns for an insert
 *
 *<code>
 *	$criteria->insert(array('name', 'type'), array(array('phalcon', 1), array('zephir', 2)));
 *</code>
 *
 * @param array $columns
 * @param array $values
 * @return Dao\Mvc\Model\CriteriaInterface
 */
DAO_DOC_METHOD(Dao_Mvc_Model_CriteriaInterface, insert);

/**
 * Set columns for an update
 *
 *<code>
 *	$criteria->update(array('name' => 'phalcon'));
 *</code>
 *
 * @param array $columns
 * @return Dao\Mvc\Model\CriteriaInterface
 */
DAO_DOC_METHOD(Dao_Mvc_Model_CriteriaInterface, update);

/**
 * Set the table for a delete.
 *
 * @return Dao\Mvc\Model\CriteriaInterface
 */
DAO_DOC_METHOD(Dao_Mvc_Model_CriteriaInterface, delete);

/**
 * Returns a PHQL statement built with the criteria
 *
 * @return String
 */
DAO_DOC_METHOD(Dao_Mvc_Model_CriteriaInterface, getPhql);
