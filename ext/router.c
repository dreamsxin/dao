
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
  |          ZhuZongXin <dreamsxin@qq.com>                                 |
  +------------------------------------------------------------------------+
*/

#include "router.h"
#include "routerinterface.h"
#include "router/exception.h"
#include "diinterface.h"
#include "di/injectable.h"

#include <main/SAPI.h>

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/exception.h"
#include "kernel/array.h"
#include "kernel/string.h"
#include "kernel/object.h"
#include "kernel/fcall.h"

/**
 * Dao\Router
 *
 * Base class for Dao\Router and Dao\Mvc\Router
 *
 */
zend_class_entry *dao_router_ce;

PHP_METHOD(Dao_Router, setDefaultModule);
PHP_METHOD(Dao_Router, getDefaultModule);
PHP_METHOD(Dao_Router, setDefaultNamespace);
PHP_METHOD(Dao_Router, getDefaultNamespace);
PHP_METHOD(Dao_Router, setDefaultHandler);
PHP_METHOD(Dao_Router, getDefaultHandler);
PHP_METHOD(Dao_Router, setDefaultAction);
PHP_METHOD(Dao_Router, getDefaultAction);
PHP_METHOD(Dao_Router, setDefaultParams);
PHP_METHOD(Dao_Router, getDefaultParams);
PHP_METHOD(Dao_Router, setCaseSensitive);
PHP_METHOD(Dao_Router, getCaseSensitive);
PHP_METHOD(Dao_Router, setMode);
PHP_METHOD(Dao_Router, getMode);
PHP_METHOD(Dao_Router, setModuleName);
PHP_METHOD(Dao_Router, getModuleName);
PHP_METHOD(Dao_Router, setNamespaceName);
PHP_METHOD(Dao_Router, getNamespaceName);
PHP_METHOD(Dao_Router, setHandlerName);
PHP_METHOD(Dao_Router, getHandlerName);
PHP_METHOD(Dao_Router, setActionName);
PHP_METHOD(Dao_Router, getActionName);
PHP_METHOD(Dao_Router, setParams);
PHP_METHOD(Dao_Router, getParams);

static const zend_function_entry dao_router_method_entry[] = {
	PHP_ME(Dao_Router, setDefaultModule, arginfo_dao_routerinterface_setdefaultmodule, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Router, getDefaultModule, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Router, setDefaultNamespace, arginfo_dao_routerinterface_setdefaultnamespace, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Router, getDefaultNamespace, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Router, setDefaultHandler, arginfo_dao_routerinterface_setdefaulthandler, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Router, getDefaultHandler, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Router, setDefaultAction, arginfo_dao_routerinterface_setdefaultaction, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Router, getDefaultAction, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Router, setDefaultParams, arginfo_dao_routerinterface_setdefaultparams, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Router, getDefaultParams, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Router, setCaseSensitive, arginfo_dao_routerinterface_setcasesensitive, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Router, getCaseSensitive, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Router, setMode, arginfo_dao_routerinterface_setmode, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Router, getMode, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Router, setModuleName, arginfo_dao_routerinterface_setmodulename, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Router, getModuleName, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Router, setNamespaceName, arginfo_dao_routerinterface_setnamespacename, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Router, getNamespaceName, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Router, setHandlerName, arginfo_dao_routerinterface_sethandlername, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Router, getHandlerName, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Router, setActionName, arginfo_dao_routerinterface_setactionname, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Router, getActionName, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Router, setParams, arginfo_dao_routerinterface_setparams, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Router, getParams, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Router initializer
 */
DAO_INIT_CLASS(Dao_Router){

	DAO_REGISTER_CLASS_EX(Dao, Router, router, dao_di_injectable_ce, dao_router_method_entry, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	zend_declare_property_null(dao_router_ce, SL("_module"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_router_ce, SL("_namespace"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_router_ce, SL("_handler"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_router_ce, SL("_action"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_router_ce, SL("_params"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_router_ce, SL("_defaultModule"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_router_ce, SL("_defaultNamespace"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_router_ce, SL("_defaultHandler"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_router_ce, SL("_defaultAction"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_router_ce, SL("_defaultParams"), ZEND_ACC_PROTECTED);
	zend_declare_property_bool(dao_router_ce, SL("_caseSensitive"), 0, ZEND_ACC_PROTECTED);
	zend_declare_property_long(dao_router_ce, SL("_mode"), DAO_ROUTER_MODE_DEFAULT, ZEND_ACC_PROTECTED);

	zend_declare_class_constant_long(dao_router_ce, SL("MODE_DEFAULT"),   DAO_ROUTER_MODE_DEFAULT);
	zend_declare_class_constant_long(dao_router_ce, SL("MODE_NONE"),   DAO_ROUTER_MODE_NONE);
	zend_declare_class_constant_long(dao_router_ce, SL("MODE_REST"),   DAO_ROUTER_MODE_REST);

	zend_class_implements(dao_router_ce, 1, dao_routerinterface_ce);

	return SUCCESS;
}

/**
 * Sets the name of the default module
 *
 * @param string $moduleName
 */
PHP_METHOD(Dao_Router, setDefaultModule){

	zval *module_name;

	dao_fetch_params(0, 1, 0, &module_name);

	dao_update_property(getThis(), SL("_defaultModule"), module_name);
	RETURN_THIS();
}

/**
 * Gets the name of the default module
 *
 * @return string
 */
PHP_METHOD(Dao_Router, getDefaultModule){


	RETURN_MEMBER(getThis(), "_defaultModule");
}

/**
 * Sets the name of the default namespace
 *
 * @param string $namespaceName
 */
PHP_METHOD(Dao_Router, setDefaultNamespace){

	zval *namespace_name;

	dao_fetch_params(0, 1, 0, &namespace_name);

	dao_update_property(getThis(), SL("_defaultNamespace"), namespace_name);
	RETURN_THIS();
}

/**
 * Gets the name of the default namespace
 *
 * @return string
 */
PHP_METHOD(Dao_Router, getDefaultNamespace){


	RETURN_MEMBER(getThis(), "_defaultNamespace");
}

/**
 * Sets the default handle name
 *
 * @param string $handleName
 */
PHP_METHOD(Dao_Router, setDefaultHandler){

	zval *handle_name;

	dao_fetch_params(0, 1, 0, &handle_name);

	dao_update_property(getThis(), SL("_defaultHandler"), handle_name);
	RETURN_THIS();
}

/**
 * Gets the default handle name
 *
 * @return string
 */
PHP_METHOD(Dao_Router, getDefaultHandler){


	RETURN_MEMBER(getThis(), "_defaultHandler");
}

/**
 * Sets the default action name
 *
 * @param string $actionName
 */
PHP_METHOD(Dao_Router, setDefaultAction){

	zval *action_name;

	dao_fetch_params(0, 1, 0, &action_name);

	dao_update_property(getThis(), SL("_defaultAction"), action_name);
	RETURN_THIS();
}

/**
 * Gets the default action name
 *
 * @return string
 */
PHP_METHOD(Dao_Router, getDefaultAction){


	RETURN_MEMBER(getThis(), "_defaultAction");
}

/**
 * Sets the default extra params
 *
 * @param string $actionName
 */
PHP_METHOD(Dao_Router, setDefaultParams){

	zval *action_name;

	dao_fetch_params(0, 1, 0, &action_name);

	dao_update_property(getThis(), SL("_defaultParams"), action_name);
	RETURN_THIS();
}

/**
 * Gets the default extra params
 *
 * @return string
 */
PHP_METHOD(Dao_Router, getDefaultParams){


	RETURN_MEMBER(getThis(), "_defaultParams");
}

/**
 * Sets the case sensitive
 * @param boolean $caseSensitive
 * @return string
 */
PHP_METHOD(Dao_Router, setCaseSensitive){

	zval *case_sensitive;

	dao_fetch_params(0, 1, 0, &case_sensitive);

	dao_update_property_bool(getThis(), SL("_caseSensitive"), zend_is_true(case_sensitive));

	RETURN_THIS();
}

/**
 * Returns the case sensitive
 *
 * @return boolean
 */
PHP_METHOD(Dao_Router, getCaseSensitive){

	RETURN_MEMBER(getThis(), "_caseSensitive");
}

/**
 * Sets the mode
 *
 * @param int $mode
 */
PHP_METHOD(Dao_Router, setMode){

	zval *mode;

	dao_fetch_params(0, 1, 0, &mode);

	dao_update_property(getThis(), SL("_mode"), mode);
	RETURN_THIS();
}

/**
 * Gets the mode
 *
 * @param int $mode
 */
PHP_METHOD(Dao_Router, getMode){


	RETURN_MEMBER(getThis(), "_mode");
}

/**
 * Sets proccesed module name
 *
 * @param string $moduleName
 */
PHP_METHOD(Dao_Router, setModuleName){

	zval *module_name;

	dao_fetch_params(0, 1, 0, &module_name);

	dao_update_property(getThis(), SL("_module"), module_name);
	RETURN_THIS();
}

/**
 * Returns proccesed module name
 *
 * @return string
 */
PHP_METHOD(Dao_Router, getModuleName){


	RETURN_MEMBER(getThis(), "_module");
}

/**
 * Sets proccesed namespace name
 *
 * @param string $namespaceName
 */
PHP_METHOD(Dao_Router, setNamespaceName){

	zval *namespace_name;

	dao_fetch_params(0, 1, 0, &namespace_name);

	dao_update_property(getThis(), SL("_namespace"), namespace_name);
	RETURN_THIS();
}

/**
 * Returns proccesed namespace name
 *
 * @return string
 */
PHP_METHOD(Dao_Router, getNamespaceName){


	RETURN_MEMBER(getThis(), "_namespace");
}

/**
 * Sets proccesed handle name
 *
 * @param string $handleName
 */
PHP_METHOD(Dao_Router, setHandlerName){

	zval *handler_name;

	dao_fetch_params(0, 1, 0, &handler_name);

	dao_update_property(getThis(), SL("_handler"), handler_name);
	RETURN_THIS();
}

/**
 * Returns proccesed handle name
 *
 * @return string
 */
PHP_METHOD(Dao_Router, getHandlerName){


	RETURN_MEMBER(getThis(), "_handler");
}

/**
 * Sets proccesed action name
 *
 * @param string $actionName
 */
PHP_METHOD(Dao_Router, setActionName){

	zval *action_name;

	dao_fetch_params(0, 1, 0, &action_name);

	dao_update_property(getThis(), SL("_action"), action_name);
	RETURN_THIS();
}

/**
 * Returns proccesed action name
 *
 * @return string
 */
PHP_METHOD(Dao_Router, getActionName){


	RETURN_MEMBER(getThis(), "_action");
}

/**
 * Sets proccesed extra params
 *
 * @param array $params
 */
PHP_METHOD(Dao_Router, setParams){

	zval *params;

	dao_fetch_params(0, 1, 0, &params);

	dao_update_property(getThis(), SL("_params"), params);
	RETURN_THIS();
}

/**
 * Returns proccesed extra params
 *
 * @return array
 */
PHP_METHOD(Dao_Router, getParams){


	RETURN_MEMBER(getThis(), "_params");
}
