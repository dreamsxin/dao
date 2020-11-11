
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

#include "mvc/dispatcherinterface.h"
#include "mvc/../dispatcherinterface.h"
#include "kernel/main.h"

zend_class_entry *dao_mvc_dispatcherinterface_ce;

static const zend_function_entry dao_mvc_dispatcherinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Mvc_DispatcherInterface, setControllerSuffix, arginfo_dao_mvc_dispatcherinterface_setcontrollersuffix)
	PHP_ABSTRACT_ME(Dao_Mvc_DispatcherInterface, setDefaultController, arginfo_dao_mvc_dispatcherinterface_setdefaultcontroller)
	PHP_ABSTRACT_ME(Dao_Mvc_DispatcherInterface, setControllerName, arginfo_dao_mvc_dispatcherinterface_setcontrollername)
	PHP_ABSTRACT_ME(Dao_Mvc_DispatcherInterface, getControllerName, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_DispatcherInterface, getLastController, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_DispatcherInterface, getActiveController, arginfo_empty)
	PHP_FE_END
};

/**
 * Dao\Mvc\DispatcherInterface initializer
 */
DAO_INIT_CLASS(Dao_Mvc_DispatcherInterface){

	DAO_REGISTER_INTERFACE_EX(Dao\\Mvc, DispatcherInterface, mvc_dispatcherinterface, dao_dispatcherinterface_ce, dao_mvc_dispatcherinterface_method_entry);

	return SUCCESS;
}

/**
 * Sets the default controller suffix
 *
 * @param string $controllerSuffix
 */
DAO_DOC_METHOD(Dao_Mvc_DispatcherInterface, setControllerSuffix);

/**
 * Sets the default controller name
 *
 * @param string $controllerName
 */
DAO_DOC_METHOD(Dao_Mvc_DispatcherInterface, setDefaultController);

/**
 * Sets the controller name to be dispatched
 *
 * @param string $controllerName
 * @param bool $isExact If true, the name should not be mangled in any way
 */
DAO_DOC_METHOD(Dao_Mvc_DispatcherInterface, setControllerName);

/**
 * Gets last dispatched controller name
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Mvc_DispatcherInterface, getControllerName);

/**
 * Returns the lastest dispatched controller
 *
 * @return Dao\Mvc\ControllerInterface
 */
DAO_DOC_METHOD(Dao_Mvc_DispatcherInterface, getLastController);

/**
 * Returns the active controller in the dispatcher
 *
 * @return Dao\Mvc\ControllerInterface
 */
DAO_DOC_METHOD(Dao_Mvc_DispatcherInterface, getActiveController);
