
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

#include "mvc/model/resultsetinterface.h"
#include "kernel/main.h"

zend_class_entry *dao_mvc_model_resultsetinterface_ce;

static const zend_function_entry dao_mvc_model_resultsetinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Mvc_Model_ResultsetInterface, getType, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_ResultsetInterface, getFirst, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_ResultsetInterface, getLast, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_ResultsetInterface, setIsFresh, arginfo_dao_mvc_model_resultsetinterface_setisfresh)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_ResultsetInterface, isFresh, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_ResultsetInterface, getCache, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_ResultsetInterface, toArray, arginfo_empty)
	PHP_FE_END
};

/**
 * Dao\Mvc\Model\ResultsetInterface initializer
 */
DAO_INIT_CLASS(Dao_Mvc_Model_ResultsetInterface){

	DAO_REGISTER_INTERFACE(Dao\\Mvc\\Model, ResultsetInterface, mvc_model_resultsetinterface, dao_mvc_model_resultsetinterface_method_entry);

	return SUCCESS;
}

/**
 * Returns the internal type of data retrieval that the resultset is using
 *
 * @return int
 */
DAO_DOC_METHOD(Dao_Mvc_Model_ResultsetInterface, getType);

/**
 * Get first row in the resultset
 *
 * @return Dao\Mvc\ModelInterface
 */
DAO_DOC_METHOD(Dao_Mvc_Model_ResultsetInterface, getFirst);

/**
 * Get last row in the resultset
 *
 * @return Dao\Mvc\ModelInterface
 */
DAO_DOC_METHOD(Dao_Mvc_Model_ResultsetInterface, getLast);

/**
 * Set if the resultset is fresh or an old one cached
 *
 * @param boolean $isFresh
 */
DAO_DOC_METHOD(Dao_Mvc_Model_ResultsetInterface, setIsFresh);

/**
 * Tell if the resultset if fresh or an old one cached
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Mvc_Model_ResultsetInterface, isFresh);

/**
 * Returns the associated cache for the resultset
 *
 * @return Dao\Cache\BackendInterface
 */
DAO_DOC_METHOD(Dao_Mvc_Model_ResultsetInterface, getCache);

/**
 * Returns a complete resultset as an array, if the resultset has a big number of rows
 * it could consume more memory than currently it does.
 *
 * @return array
 */
DAO_DOC_METHOD(Dao_Mvc_Model_ResultsetInterface, toArray);
