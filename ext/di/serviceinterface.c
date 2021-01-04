
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

#include "di/serviceinterface.h"
#include "kernel/main.h"

zend_class_entry *dao_di_serviceinterface_ce;

static const zend_function_entry dao_di_serviceinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Di_ServiceInterface, getName, arginfo_dao_di_serviceinterface_getname)
	PHP_ABSTRACT_ME(Dao_Di_ServiceInterface, setShared, arginfo_dao_di_serviceinterface_setshared)
	PHP_ABSTRACT_ME(Dao_Di_ServiceInterface, isShared, arginfo_dao_di_serviceinterface_isshared)
	PHP_ABSTRACT_ME(Dao_Di_ServiceInterface, setDefinition, arginfo_dao_di_serviceinterface_setdefinition)
	PHP_ABSTRACT_ME(Dao_Di_ServiceInterface, getDefinition, arginfo_dao_di_serviceinterface_getdefinition)
	PHP_ABSTRACT_ME(Dao_Di_ServiceInterface, isResolved, arginfo_dao_di_serviceinterface_isresolved)
	PHP_ABSTRACT_ME(Dao_Di_ServiceInterface, resolve, arginfo_dao_di_serviceinterface_resolve)
	PHP_FE_END
};

/**
 * Dao\Di\ServiceInterface initializer
 */
DAO_INIT_CLASS(Dao_Di_ServiceInterface){

	DAO_REGISTER_INTERFACE(Dao\\Di, ServiceInterface, di_serviceinterface, dao_di_serviceinterface_method_entry);

	return SUCCESS;
}

/**
 * Returns the name of the service
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Di_ServiceInterface, getName);

/**
 * Sets whether the service is shared or not
 *
 * @param boolean $shared
 */
DAO_DOC_METHOD(Dao_Di_ServiceInterface, setShared);

/**
 * Check whether the service is shared or not
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Di_ServiceInterface, isShared);

/**
 * Set the service definition
 *
 * @param mixed $definition
 */
DAO_DOC_METHOD(Dao_Di_ServiceInterface, setDefinition);

/**
 * Returns the service definition
 *
 * @return mixed
 */
DAO_DOC_METHOD(Dao_Di_ServiceInterface, getDefinition);

/**
 * Resolves the service
 *
 * @param array $parameters
 * @param Dao\DiInterface $dependencyInjector
 * @return object
 */
DAO_DOC_METHOD(Dao_Di_ServiceInterface, resolve);

/**
 * Checks if the service was resolved
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Di_ServiceInterface, isResolved);
