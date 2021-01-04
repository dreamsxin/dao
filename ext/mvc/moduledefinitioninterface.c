
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

#include "mvc/moduledefinitioninterface.h"
#include "kernel/main.h"

zend_class_entry *dao_mvc_moduledefinitioninterface_ce;

static const zend_function_entry dao_mvc_moduledefinitioninterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Mvc_ModuleDefinitionInterface, registerAutoloaders, arginfo_dao_mvc_moduledefinitioninterface_registerautoloaders)
	PHP_ABSTRACT_ME(Dao_Mvc_ModuleDefinitionInterface, registerServices, arginfo_dao_mvc_moduledefinitioninterface_registerservices)
	PHP_FE_END
};


/**
 * Dao\Mvc\ModuleDefinitionInterface initializer
 */
DAO_INIT_CLASS(Dao_Mvc_ModuleDefinitionInterface){

	DAO_REGISTER_INTERFACE(Dao\\Mvc, ModuleDefinitionInterface, mvc_moduledefinitioninterface, dao_mvc_moduledefinitioninterface_method_entry);

	return SUCCESS;
}

/**
 * Registers an autoloader related to the module
 *
 * @param Dao\DiInterface $dependencyInjector
 */
DAO_DOC_METHOD(Dao_Mvc_ModuleDefinitionInterface, registerAutoloaders);

/**
 * Registers services related to the module
 *
 * @param Dao\DiInterface $dependencyInjector
 */
DAO_DOC_METHOD(Dao_Mvc_ModuleDefinitionInterface, registerServices);
