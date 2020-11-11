
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

#include "mvc/model/behaviorinterface.h"
#include "kernel/main.h"

zend_class_entry *dao_mvc_model_behaviorinterface_ce;

static const zend_function_entry dao_mvc_model_behaviorinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Mvc_Model_BehaviorInterface, notify, arginfo_dao_mvc_model_behaviorinterface_notify)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_BehaviorInterface, missingMethod, arginfo_dao_mvc_model_behaviorinterface_missingmethod)
	PHP_FE_END
};

/**
 * Dao\Mvc\Model\BehaviorInterface initializer
 */
DAO_INIT_CLASS(Dao_Mvc_Model_BehaviorInterface){

	DAO_REGISTER_INTERFACE(Dao\\Mvc\\Model, BehaviorInterface, mvc_model_behaviorinterface, dao_mvc_model_behaviorinterface_method_entry);

	return SUCCESS;
}

/**
 * This method receives the notifications from the EventsManager
 *
 * @param string $type
 * @param Dao\Mvc\ModelInterface $model
 */
DAO_DOC_METHOD(Dao_Mvc_Model_BehaviorInterface, notify);

/**
 * Calls a method when it's missing in the model
 *
 * @param Dao\Mvc\ModelInterface $model
 * @param string $method
 * @param array $arguments
 */
DAO_DOC_METHOD(Dao_Mvc_Model_BehaviorInterface, missingMethod);
