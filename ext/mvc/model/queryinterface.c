
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

#include "mvc/model/queryinterface.h"
#include "kernel/main.h"

zend_class_entry *dao_mvc_model_queryinterface_ce;

static const zend_function_entry dao_mvc_model_queryinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Mvc_Model_QueryInterface, parse, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_QueryInterface, execute, arginfo_dao_mvc_model_queryinterface_execute)
	PHP_FE_END
};

/**
 * Dao\Mvc\Model\QueryInterface initializer
 */
DAO_INIT_CLASS(Dao_Mvc_Model_QueryInterface){

	DAO_REGISTER_INTERFACE(Dao\\Mvc\\Model, QueryInterface, mvc_model_queryinterface, dao_mvc_model_queryinterface_method_entry);

	return SUCCESS;
}

/**
 * Parses the intermediate code produced by Dao\Mvc\Model\Query\Lang generating another
 * intermediate representation that could be executed by Dao\Mvc\Model\Query
 *
 * @return array
 */
DAO_DOC_METHOD(Dao_Mvc_Model_QueryInterface, parse);

/**
 * Executes a parsed PHQL statement
 *
 * @param array $bindParams
 * @param array $bindTypes
 * @return mixed
 */
DAO_DOC_METHOD(Dao_Mvc_Model_QueryInterface, execute);
