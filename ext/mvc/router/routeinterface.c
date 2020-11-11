
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

#include "mvc/router/routeinterface.h"
#include "kernel/main.h"

zend_class_entry *dao_mvc_router_routeinterface_ce;

static const zend_function_entry dao_mvc_router_routeinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Mvc_Router_RouteInterface, compilePattern, arginfo_dao_mvc_router_routeinterface_compilepattern)
	PHP_ABSTRACT_ME(Dao_Mvc_Router_RouteInterface, via, arginfo_dao_mvc_router_routeinterface_via)
	PHP_ABSTRACT_ME(Dao_Mvc_Router_RouteInterface, reConfigure, arginfo_dao_mvc_router_routeinterface_reconfigure)
	PHP_ABSTRACT_ME(Dao_Mvc_Router_RouteInterface, getName, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Router_RouteInterface, setName, arginfo_dao_mvc_router_routeinterface_setname)
	PHP_ABSTRACT_ME(Dao_Mvc_Router_RouteInterface, setHttpMethods, arginfo_dao_mvc_router_routeinterface_sethttpmethods)
	PHP_ABSTRACT_ME(Dao_Mvc_Router_RouteInterface, getRouteId, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Router_RouteInterface, getPattern, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Router_RouteInterface, getCompiledPattern, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Router_RouteInterface, getPaths, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Router_RouteInterface, getHttpMethods, arginfo_empty)
	ZEND_FENTRY(reset, NULL, arginfo_empty, ZEND_ACC_STATIC|ZEND_ACC_ABSTRACT|ZEND_ACC_PUBLIC)
	PHP_ABSTRACT_ME(Dao_Mvc_Router_RouteInterface, setDefaults, arginfo_dao_mvc_router_routeinterface_setdefaults)
	PHP_ABSTRACT_ME(Dao_Mvc_Router_RouteInterface, getDefaultModule, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Router_RouteInterface, getDefaultNamespace, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Router_RouteInterface, getDefaultController, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Router_RouteInterface, getDefaultAction, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Router_RouteInterface, getDefaultParams, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Router_RouteInterface, setMode, arginfo_dao_mvc_router_routeinterface_setmode)
	PHP_ABSTRACT_ME(Dao_Mvc_Router_RouteInterface, getMode, arginfo_empty)
	PHP_FE_END
};

/**
 * Dao\Mvc\Router\RouteInterface initializer
 */
DAO_INIT_CLASS(Dao_Mvc_Router_RouteInterface){

	DAO_REGISTER_INTERFACE(Dao\\Mvc\\Router, RouteInterface, mvc_router_routeinterface, dao_mvc_router_routeinterface_method_entry);

	return SUCCESS;
}

/**
 * Replaces placeholders from pattern returning a valid PCRE regular expression
 *
 * @param string $pattern
 * @param array $paths
 * @return string
 */
DAO_DOC_METHOD(Dao_Mvc_Router_RouteInterface, compilePattern);

/**
 * Set one or more HTTP methods that constraint the matching of the route
 *
 * @param string|array $httpMethods
 */
DAO_DOC_METHOD(Dao_Mvc_Router_RouteInterface, via);

/**
 * Reconfigure the route adding a new pattern and a set of paths
 *
 * @param string $pattern
 * @param array $paths
 */
DAO_DOC_METHOD(Dao_Mvc_Router_RouteInterface, reConfigure);

/**
 * Returns the route's name
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Mvc_Router_RouteInterface, getName);

/**
 * Sets the route's name
 *
 * @param string $name
 */
DAO_DOC_METHOD(Dao_Mvc_Router_RouteInterface, setName);

/**
 * Sets a set of HTTP methods that constraint the matching of the route
 *
 * @param string|array $httpMethods
 */
DAO_DOC_METHOD(Dao_Mvc_Router_RouteInterface, setHttpMethods);

/**
 * Returns the route's id
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Mvc_Router_RouteInterface, getRouteId);

/**
 * Returns the route's pattern
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Mvc_Router_RouteInterface, getPattern);

/**
 * Returns the route's pattern
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Mvc_Router_RouteInterface, getCompiledPattern);

/**
 * Returns the paths
 *
 * @return array
 */
DAO_DOC_METHOD(Dao_Mvc_Router_RouteInterface, getPaths);

/**
 * Returns the HTTP methods that constraint matching the route
 *
 * @return string|array
 */
DAO_DOC_METHOD(Dao_Mvc_Router_RouteInterface, getHttpMethods);

/**
 * Resets the internal route id generator
 */
DAO_DOC_METHOD(Dao_Mvc_Router_RouteInterface, reset);
