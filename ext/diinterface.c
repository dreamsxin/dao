
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

#include "diinterface.h"

#include <Zend/zend_interfaces.h>

#include "kernel/main.h"

zend_class_entry *dao_diinterface_ce;

static const zend_function_entry dao_diinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_DiInterface, set, arginfo_dao_diinterface_set)
	PHP_ABSTRACT_ME(Dao_DiInterface, remove, arginfo_dao_diinterface_remove)
	PHP_ABSTRACT_ME(Dao_DiInterface, get, arginfo_dao_diinterface_get)
	PHP_ABSTRACT_ME(Dao_DiInterface, getShared, arginfo_dao_diinterface_getshared)
	PHP_ABSTRACT_ME(Dao_DiInterface, setService, arginfo_dao_diinterface_setservice)
	PHP_ABSTRACT_ME(Dao_DiInterface, getService, arginfo_dao_diinterface_getservice)
	PHP_ABSTRACT_ME(Dao_DiInterface, has, arginfo_dao_diinterface_has)
	PHP_ABSTRACT_ME(Dao_DiInterface, wasFreshInstance, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_DiInterface, getServices, arginfo_empty)
	ZEND_FENTRY(setDefault, NULL, arginfo_dao_diinterface_setdefault, ZEND_ACC_STATIC|ZEND_ACC_ABSTRACT|ZEND_ACC_PUBLIC)
	ZEND_FENTRY(getDefault, NULL, arginfo_empty, ZEND_ACC_STATIC|ZEND_ACC_ABSTRACT|ZEND_ACC_PUBLIC)
	ZEND_FENTRY(reset, NULL, arginfo_empty, ZEND_ACC_STATIC|ZEND_ACC_ABSTRACT|ZEND_ACC_PUBLIC)
	PHP_FE_END
};


/**
 * Dao\DiInterface initializer
 */
DAO_INIT_CLASS(Dao_DiInterface){

	DAO_REGISTER_INTERFACE_EX(Dao, DiInterface, diinterface, zend_ce_arrayaccess, dao_diinterface_method_entry);

	return SUCCESS;
}

/**
 * Registers a service in the service container
 *
 * @param string $name
 * @param mixed $definition
 * @param boolean $shared
 * @return Dao\Di\ServiceInterface
 */
DAO_DOC_METHOD(Dao_DiInterface, set);

/**
 * Removes a service from the service container
 *
 * @param string $name
 */
DAO_DOC_METHOD(Dao_DiInterface, remove);

/**
 * Resolves the service based on its configuration
 *
 * @param string $name
 * @param array $parameters
 * @param boolean $noError
 * @return object
 */
DAO_DOC_METHOD(Dao_DiInterface, get);

/**
 * Resolves a shared service based on their configuration
 *
 * @param string $name
 * @param array $parameters
 * @param boolean $noError
 * @return object
 */
DAO_DOC_METHOD(Dao_DiInterface, getShared);

/**
 * Sets a service using a raw Dao\Di\Service definition
 *
 * @param string $name
 * @param Dao\Di\ServiceInterface $rawDefinition
 * @return Dao\Di\ServiceInterface
 */
DAO_DOC_METHOD(Dao_DiInterface, setService);

/**
 * Returns the corresponding Dao\Di\Service instance for a service
 *
 * @param string $name
 * @return Dao\Di\ServiceInterface
 */
DAO_DOC_METHOD(Dao_DiInterface, getService);

/**
 * Check whether the DI contains a service by a name
 *
 * @param string $name
 * @return boolean
 */
DAO_DOC_METHOD(Dao_DiInterface, has);

/**
 * Check whether the last service obtained via getShared produced a fresh instance or an existing one
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_DiInterface, wasFreshInstance);

/**
 * Return the services registered in the DI
 *
 * @return array
 */
DAO_DOC_METHOD(Dao_DiInterface, getServices);

/**
 * Set the default dependency injection container to be obtained into static methods
 *
 * @param Dao_DiInterface $dependencyInjector
 */
DAO_DOC_METHOD(Dao_DiInterface, setDefault);

/**
 * Return the last DI created
 *
 * @return Dao_DiInterface
 */
DAO_DOC_METHOD(Dao_DiInterface, getDefault);

/**
 * Resets the internal default DI
 */
DAO_DOC_METHOD(Dao_DiInterface, reset);
