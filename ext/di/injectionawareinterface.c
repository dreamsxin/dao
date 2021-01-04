
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

#include "di/injectionawareinterface.h"

#include "kernel/main.h"

zend_class_entry *dao_di_injectionawareinterface_ce;

static const zend_function_entry dao_di_injectionawareinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Di_InjectionAwareInterface, setDI, arginfo_dao_di_injectionawareinterface_setdi)
	PHP_ABSTRACT_ME(Dao_Di_InjectionAwareInterface, getDI, arginfo_dao_di_injectionawareinterface_getdi)
	PHP_FE_END
};

/**
 * Dao\Di\InjectionAwareInterface initializer
 */
DAO_INIT_CLASS(Dao_Di_InjectionAwareInterface){

	DAO_REGISTER_INTERFACE(Dao\\Di, InjectionAwareInterface, di_injectionawareinterface, dao_di_injectionawareinterface_method_entry);

	return SUCCESS;
}

/**
 * Sets the dependency injector
 *
 * @param Dao\DiInterface $dependencyInjector
 */
DAO_DOC_METHOD(Dao_Di_InjectionAwareInterface, setDI);

/**
 * Returns the internal dependency injector
 *
 * @return Dao\DiInterface
 */
DAO_DOC_METHOD(Dao_Di_InjectionAwareInterface, getDI);
