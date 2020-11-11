
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
  |          Rack Lin <racklin@gmail.com>                                  |
  |          ZhuZongXin <dreamsxin@qq.com>                                 |
  +------------------------------------------------------------------------+
*/

#include "dispatcher.h"
#include "dispatcherinterface.h"
#include "diinterface.h"
#include "di/injectable.h"
#include "events/eventsawareinterface.h"
#include "exception.h"
#include "continueexception.h"
#include "filterinterface.h"
#include "user/logic.h"
#include "debug.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/array.h"
#include "kernel/concat.h"
#include "kernel/operators.h"
#include "kernel/string.h"
#include "kernel/exception.h"
#include "kernel/debug.h"

#include "interned-strings.h"

/**
 * Dao\Dispatcher
 *
 * This is the base class for Dao\Mvc\Dispatcher and Dao\Cli\Dispatcher.
 * This class can't be instantiated directly, you can use it to create your own dispatchers
 */
zend_class_entry *dao_dispatcher_ce;

PHP_METHOD(Dao_Dispatcher, __construct);
PHP_METHOD(Dao_Dispatcher, setActionSuffix);
PHP_METHOD(Dao_Dispatcher, getActionSuffix);
PHP_METHOD(Dao_Dispatcher, setHandlerSuffix);
PHP_METHOD(Dao_Dispatcher, getHandlerSuffix);
PHP_METHOD(Dao_Dispatcher, setDefaultModule);
PHP_METHOD(Dao_Dispatcher, getDefaultModule);
PHP_METHOD(Dao_Dispatcher, setDefaultNamespace);
PHP_METHOD(Dao_Dispatcher, getDefaultNamespace);
PHP_METHOD(Dao_Dispatcher, setDefaultHandler);
PHP_METHOD(Dao_Dispatcher, getDefaultHandler);
PHP_METHOD(Dao_Dispatcher, setDefaultAction);
PHP_METHOD(Dao_Dispatcher, getDefaultAction);
PHP_METHOD(Dao_Dispatcher, setModuleName);
PHP_METHOD(Dao_Dispatcher, getModuleName);
PHP_METHOD(Dao_Dispatcher, setNamespaceName);
PHP_METHOD(Dao_Dispatcher, getNamespaceName);
PHP_METHOD(Dao_Dispatcher, setHandlerName);
PHP_METHOD(Dao_Dispatcher, getHandlerName);
PHP_METHOD(Dao_Dispatcher, setActionName);
PHP_METHOD(Dao_Dispatcher, getActionName);
PHP_METHOD(Dao_Dispatcher, setLogicBinding);
PHP_METHOD(Dao_Dispatcher, isLogicBinding);
PHP_METHOD(Dao_Dispatcher, setParams);
PHP_METHOD(Dao_Dispatcher, getParams);
PHP_METHOD(Dao_Dispatcher, hasParam);
PHP_METHOD(Dao_Dispatcher, setParam);
PHP_METHOD(Dao_Dispatcher, getParam);
PHP_METHOD(Dao_Dispatcher, getActiveHandler);
PHP_METHOD(Dao_Dispatcher, getActiveMethod);
PHP_METHOD(Dao_Dispatcher, isFinished);
PHP_METHOD(Dao_Dispatcher, setFinished);
PHP_METHOD(Dao_Dispatcher, setReturnedValue);
PHP_METHOD(Dao_Dispatcher, getReturnedValue);
PHP_METHOD(Dao_Dispatcher, dispatch);
PHP_METHOD(Dao_Dispatcher, forward);
PHP_METHOD(Dao_Dispatcher, wasForwarded);
PHP_METHOD(Dao_Dispatcher, getHandlerClass);
PHP_METHOD(Dao_Dispatcher, camelizeNamespace);
PHP_METHOD(Dao_Dispatcher, camelizeController);
PHP_METHOD(Dao_Dispatcher, setErrorHandler);
PHP_METHOD(Dao_Dispatcher, getErrorHandler);
PHP_METHOD(Dao_Dispatcher, getLastException);
PHP_METHOD(Dao_Dispatcher, getLastHandler);
PHP_METHOD(Dao_Dispatcher, getPreviousNamespaceName);
PHP_METHOD(Dao_Dispatcher, getPreviousActionName);
PHP_METHOD(Dao_Dispatcher, getPreviousParams);
PHP_METHOD(Dao_Dispatcher, getPreviousParam);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_dispatcher_setreturnedvalue, 0, 0, 1)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_dispatcher_setfinished, 0, 0, 1)
	ZEND_ARG_INFO(0, finished)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_dispatcher_method_entry[] = {
	PHP_ME(Dao_Dispatcher, __construct, arginfo_empty, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Dispatcher, setActionSuffix, arginfo_dao_dispatcherinterface_setactionsuffix, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Dispatcher, getActionSuffix, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Dispatcher, setHandlerSuffix, arginfo_dao_dispatcherinterface_sethandlersuffix, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Dispatcher, getHandlerSuffix, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Dispatcher, setDefaultModule, arginfo_dao_dispatcherinterface_setdefaultmodule, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Dispatcher, getDefaultModule, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Dispatcher, setDefaultNamespace, arginfo_dao_dispatcherinterface_setdefaultnamespace, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Dispatcher, getDefaultNamespace, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Dispatcher, setDefaultHandler, arginfo_dao_dispatcherinterface_setdefaulthandler, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Dispatcher, getDefaultHandler, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Dispatcher, setDefaultAction, arginfo_dao_dispatcherinterface_setdefaultaction, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Dispatcher, getDefaultAction, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Dispatcher, setModuleName, arginfo_dao_dispatcherinterface_setmodulename, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Dispatcher, getModuleName, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Dispatcher, setNamespaceName, arginfo_dao_dispatcherinterface_setnamespacename, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Dispatcher, getNamespaceName, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Dispatcher, setHandlerName, arginfo_dao_dispatcherinterface_sethandlername, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Dispatcher, getHandlerName, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Dispatcher, setActionName, arginfo_dao_dispatcherinterface_setactionname, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Dispatcher, getActionName, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Dispatcher, setLogicBinding, arginfo_dao_dispatcherinterface_setlogicbinding, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Dispatcher, isLogicBinding, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Dispatcher, setParams, arginfo_dao_dispatcherinterface_setparams, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Dispatcher, getParams, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Dispatcher, hasParam, arginfo_dao_dispatcherinterface_hasparam, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Dispatcher, setParam, arginfo_dao_dispatcherinterface_setparam, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Dispatcher, getParam, arginfo_dao_dispatcherinterface_getparam, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Dispatcher, getActiveHandler, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Dispatcher, getActiveMethod, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Dispatcher, isFinished, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Dispatcher, setFinished, arginfo_dao_dispatcher_setfinished, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Dispatcher, setReturnedValue, arginfo_dao_dispatcher_setreturnedvalue, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Dispatcher, getReturnedValue, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Dispatcher, dispatch, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Dispatcher, forward, arginfo_dao_dispatcherinterface_forward, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Dispatcher, wasForwarded, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Dispatcher, getHandlerClass, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Dispatcher, camelizeNamespace, arginfo_dao_dispatcherinterface_camelizenamespace, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Dispatcher, camelizeController, arginfo_dao_dispatcherinterface_camelizecontroller, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Dispatcher, setErrorHandler, arginfo_dao_dispatcherinterface_seterrorhandler, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Dispatcher, getErrorHandler, arginfo_dao_dispatcherinterface_geterrorhandler, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Dispatcher, getLastException, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Dispatcher, getLastHandler, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Dispatcher, getPreviousNamespaceName, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Dispatcher, getPreviousActionName, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Dispatcher, getPreviousParams, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Dispatcher, getPreviousParam, arginfo_dao_dispatcherinterface_getparam, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Dispatcher initializer
 */
DAO_INIT_CLASS(Dao_Dispatcher){

	DAO_REGISTER_CLASS_EX(Dao, Dispatcher, dispatcher, dao_di_injectable_ce, dao_dispatcher_method_entry, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	zend_declare_property_null(dao_dispatcher_ce, SL("_activeHandler"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_dispatcher_ce, SL("_finished"), ZEND_ACC_PROTECTED);
	zend_declare_property_bool(dao_dispatcher_ce, SL("_forwarded"), 0, ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_dispatcher_ce, SL("_moduleName"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_dispatcher_ce, SL("_namespaceName"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_dispatcher_ce, SL("_handlerName"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_dispatcher_ce, SL("_actionName"), ZEND_ACC_PROTECTED);
	zend_declare_property_bool(dao_dispatcher_ce, SL("_logicBinding"), 0, ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_dispatcher_ce, SL("_params"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_dispatcher_ce, SL("_returnedValue"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_dispatcher_ce, SL("_lastHandler"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_dispatcher_ce, SL("_defaultModule"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_dispatcher_ce, SL("_defaultNamespace"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_dispatcher_ce, SL("_defaultHandler"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_dispatcher_ce, SL("_defaultAction"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_dispatcher_ce, SL("_handlerSuffix"), ZEND_ACC_PROTECTED);
	zend_declare_property_string(dao_dispatcher_ce, SL("_actionSuffix"), "Action", ZEND_ACC_PROTECTED);
	zend_declare_property_bool(dao_dispatcher_ce, SL("_isExactHandler"), 0, ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_dispatcher_ce, SL("_previousNamespaceName"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_dispatcher_ce, SL("_previousHandlerName"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_dispatcher_ce, SL("_previousActionName"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_dispatcher_ce, SL("_previousParams"), ZEND_ACC_PROTECTED);
	zend_declare_property_bool(dao_dispatcher_ce, SL("_camelizeNamespace"), 1, ZEND_ACC_PROTECTED);
	zend_declare_property_bool(dao_dispatcher_ce, SL("_camelizeController"), 1, ZEND_ACC_PROTECTED);
	zend_declare_property_bool(dao_dispatcher_ce, SL("_errorHandlers"), 1, ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_dispatcher_ce, SL("_lastException"), ZEND_ACC_PROTECTED);

	zend_declare_class_constant_long(dao_dispatcher_ce, SL("EXCEPTION_NO_DI"), DAO_EXCEPTION_NO_DI);
	zend_declare_class_constant_long(dao_dispatcher_ce, SL("EXCEPTION_CYCLIC_ROUTING"), DAO_EXCEPTION_CYCLIC_ROUTING);
	zend_declare_class_constant_long(dao_dispatcher_ce, SL("EXCEPTION_HANDLER_NOT_FOUND"), DAO_EXCEPTION_HANDLER_NOT_FOUND);
	zend_declare_class_constant_long(dao_dispatcher_ce, SL("EXCEPTION_INVALID_HANDLER"), DAO_EXCEPTION_INVALID_HANDLER);
	zend_declare_class_constant_long(dao_dispatcher_ce, SL("EXCEPTION_INVALID_PARAMS"), DAO_EXCEPTION_INVALID_PARAMS);
	zend_declare_class_constant_long(dao_dispatcher_ce, SL("EXCEPTION_ACTION_NOT_FOUND"), DAO_EXCEPTION_ACTION_NOT_FOUND);

	zend_class_implements(dao_dispatcher_ce, 1, dao_dispatcherinterface_ce);

	return SUCCESS;
}

/**
 * Dao\Dispatcher constructor
 */
PHP_METHOD(Dao_Dispatcher, __construct){

	dao_update_property_empty_array(getThis(), SL("_params"));
}

/**
 * Sets the default action suffix
 *
 * @param string $actionSuffix
 */
PHP_METHOD(Dao_Dispatcher, setActionSuffix){

	zval *action_suffix;

	dao_fetch_params(0, 1, 0, &action_suffix);

	dao_update_property(getThis(), SL("_actionSuffix"), action_suffix);

}

/**
 * Gets the default action suffix
 *
 * @return string
 */
PHP_METHOD(Dao_Dispatcher, getActionSuffix){

	RETURN_MEMBER(getThis(), "_actionSuffix");
}

/**
 * Sets the default handler suffix
 *
 * @param string $handlerSuffix
 */
PHP_METHOD(Dao_Dispatcher, setHandlerSuffix){

	zval *handler_suffix;

	dao_fetch_params(0, 1, 0, &handler_suffix);

	dao_update_property(getThis(), SL("_handlerSuffix"), handler_suffix);

}

/**
 * Gets the default handler suffix
 *
 * @return string
 */
PHP_METHOD(Dao_Dispatcher, getHandlerSuffix){

	RETURN_MEMBER(getThis(), "_handlerSuffix");
}

/**
 * Sets the default module
 *
 * @param string $module
 */
PHP_METHOD(Dao_Dispatcher, setDefaultModule){

	zval *module;

	dao_fetch_params(0, 1, 0, &module);

	dao_update_property(getThis(), SL("_defaultModule"), module);

}

/**
 * Returns the default module
 *
 * @return string
 */
PHP_METHOD(Dao_Dispatcher, getDefaultModule){


	RETURN_MEMBER(getThis(), "_defaultModule");
}

/**
 * Sets the default namespace
 *
 * @param string $namespace
 */
PHP_METHOD(Dao_Dispatcher, setDefaultNamespace){

	zval *namespace;

	dao_fetch_params(0, 1, 0, &namespace);

	dao_update_property(getThis(), SL("_defaultNamespace"), namespace);

}

/**
 * Returns the default namespace
 *
 * @return string
 */
PHP_METHOD(Dao_Dispatcher, getDefaultNamespace){


	RETURN_MEMBER(getThis(), "_defaultNamespace");
}

/**
 * Sets the default handler
 *
 * @param string $handler
 */
PHP_METHOD(Dao_Dispatcher, setDefaultHandler){

	zval *handler;

	dao_fetch_params(0, 1, 0, &handler);

	dao_update_property(getThis(), SL("_defaultHandler"), handler);

}

/**
 * Returns the default handler
 *
 * @return string
 */
PHP_METHOD(Dao_Dispatcher, getDefaultHandler){


	RETURN_MEMBER(getThis(), "_defaultHandler");
}

/**
 * Sets the default action name
 *
 * @param string $actionName
 */
PHP_METHOD(Dao_Dispatcher, setDefaultAction){

	zval *action_name;

	dao_fetch_params(0, 1, 0, &action_name);

	dao_update_property(getThis(), SL("_defaultAction"), action_name);

}

/**
 * Returns the default action
 *
 * @return string
 */
PHP_METHOD(Dao_Dispatcher, getDefaultAction){


	RETURN_MEMBER(getThis(), "_defaultAction");
}

/**
 * Sets the module where the controller is (only informative)
 *
 * @param string $moduleName
 */
PHP_METHOD(Dao_Dispatcher, setModuleName){

	zval *module_name;

	dao_fetch_params(0, 1, 0, &module_name);

	dao_update_property(getThis(), SL("_moduleName"), module_name);

}

/**
 * Gets the module where the controller class is
 *
 * @return string
 */
PHP_METHOD(Dao_Dispatcher, getModuleName){


	RETURN_MEMBER(getThis(), "_moduleName");
}

/**
 * Sets the namespace where the controller class is
 *
 * @param string $namespaceName
 */
PHP_METHOD(Dao_Dispatcher, setNamespaceName){

	zval *namespace_name;

	dao_fetch_params(0, 1, 0, &namespace_name);

	dao_update_property(getThis(), SL("_namespaceName"), namespace_name);

}

/**
 * Gets a namespace to be prepended to the current handler name
 *
 * @return string
 */
PHP_METHOD(Dao_Dispatcher, getNamespaceName){


	RETURN_MEMBER(getThis(), "_namespaceName");
}

/**
 * Sets the action name to be dispatched
 *
 * @param string $handlerName
 */
PHP_METHOD(Dao_Dispatcher, setHandlerName){

	zval *handler_name;

	dao_fetch_params(0, 1, 0, &handler_name);

	dao_update_property(getThis(), SL("_handlerName"), handler_name);

}

/**
 * Gets the lastest dispatched handler name
 *
 * @return string
 */
PHP_METHOD(Dao_Dispatcher, getHandlerName){


	RETURN_MEMBER(getThis(), "_handlerName");
}

/**
 * Sets the action name to be dispatched
 *
 * @param string $actionName
 */
PHP_METHOD(Dao_Dispatcher, setActionName){

	zval *action_name;

	dao_fetch_params(0, 1, 0, &action_name);

	dao_update_property(getThis(), SL("_actionName"), action_name);

}

/**
 * Gets the lastest dispatched action name
 *
 * @return string
 */
PHP_METHOD(Dao_Dispatcher, getActionName){


	RETURN_MEMBER(getThis(), "_actionName");
}

/**
 * Enable/Disable logic binding during dispatch
 *
 * @param boolean $value
 */
PHP_METHOD(Dao_Dispatcher, setLogicBinding){

	zval *value;

	dao_fetch_params(0, 1, 0, &value);

	if (DAO_IS_TRUE(value)) {
		dao_update_property(getThis(), SL("_logicBinding"), &DAO_GLOBAL(z_true));
	} else {
		dao_update_property(getThis(), SL("_logicBinding"), &DAO_GLOBAL(z_false));
	}
}

/**
 * Check if logic binding
 *
 * @return boolean
 */
PHP_METHOD(Dao_Dispatcher, isLogicBinding){


	RETURN_MEMBER(getThis(), "_logicBinding");
}

/**
 * Sets action params to be dispatched
 *
 * @param array $params
 */
PHP_METHOD(Dao_Dispatcher, setParams){

	zval *params;

	dao_fetch_params(0, 1, 0, &params);
	DAO_SEPARATE_PARAM(params);

	dao_update_property(getThis(), SL("_params"), params);
}

/**
 * Gets action params
 *
 * @return array
 */
PHP_METHOD(Dao_Dispatcher, getParams){


	RETURN_MEMBER(getThis(), "_params");
}

/**
 * Check if a param exists
 *
 * @param mixed param
 * @return boolean
 */
PHP_METHOD(Dao_Dispatcher, hasParam){

	zval *param, params = {};

	dao_fetch_params(0, 1, 0, &param);

	dao_read_property(&params, getThis(), SL("_params"), PH_NOISY|PH_READONLY);
	if (dao_array_isset(&params, param)) {
		RETURN_TRUE;
	}

	RETURN_FALSE;
}

/**
 * Set a param by its name or numeric index
 *
 * @param mixed $param
 * @param mixed $value
 */
PHP_METHOD(Dao_Dispatcher, setParam){

	zval *param, *value;

	dao_fetch_params(0, 2, 0, &param, &value);

	dao_update_property_array(getThis(), SL("_params"), param, value);

}

/**
 * Gets a param by its name or numeric index
 *
 * @param mixed $param
 * @param string|array $filters
 * @param mixed $defaultValue
 * @return mixed
 */
PHP_METHOD(Dao_Dispatcher, getParam){

	zval *param, *filters = NULL, *default_value = NULL, params = {}, param_value = {}, dependency_injector = {}, exception_code = {}, exception_message = {};
	zval service = {}, filter = {};

	dao_fetch_params(1, 1, 2, &param, &filters, &default_value);

	dao_read_property(&params, getThis(), SL("_params"), PH_NOISY|PH_READONLY);
	if (dao_array_isset_fetch(&param_value, &params, param, PH_READONLY)) {
		if (filters && Z_TYPE_P(filters) != IS_NULL) {
			DAO_MM_CALL_METHOD(&dependency_injector, getThis(), "getdi");
			DAO_MM_ADD_ENTRY(&dependency_injector);
			if (Z_TYPE(dependency_injector) != IS_OBJECT) {
				ZVAL_LONG(&exception_code, DAO_EXCEPTION_NO_DI);
				DAO_MM_ZVAL_STRING(&exception_message, "A dependency injection object is required to access the 'filter' service");
				DAO_MM_CALL_METHOD(NULL, getThis(), "_throwdispatchexception", &exception_message, &exception_code);
				RETURN_MM();
			}

			ZVAL_STR(&service, IS(filter));

			DAO_MM_CALL_METHOD(&filter, &dependency_injector, "getshared", &service);
			DAO_MM_ADD_ENTRY(&filter);
			DAO_MM_VERIFY_INTERFACE(&filter, dao_filterinterface_ce);
			DAO_MM_RETURN_CALL_METHOD(&filter, "sanitize", &param_value, filters);
			RETURN_MM();
		} else {
			RETURN_MM_CTOR(&param_value);
		}
	}

	if (default_value) {
		RETURN_MM_CTOR(default_value);
	}

	RETURN_MM_NULL();
}

/**
 * Returns the current handler to be/executed in the dispatcher
 *
 * @return Dao\Mvc\Controller
 */
PHP_METHOD(Dao_Dispatcher, getActiveHandler){

	RETURN_MEMBER(getThis(), "_activeHandler");
}

/**
 * Returns the current method to be/executed in the dispatcher
 *
 * @return string
 */
PHP_METHOD(Dao_Dispatcher, getActiveMethod){

	zval suffix = {}, action_name = {};

	dao_read_property(&suffix, getThis(), SL("_actionSuffix"), PH_NOISY|PH_READONLY);
	dao_read_property(&action_name, getThis(), SL("_actionName"), PH_NOISY|PH_READONLY);

	DAO_CONCAT_VV(return_value, &action_name, &suffix);
}

/**
 * Checks if the dispatch loop is finished or has more pendent controllers/tasks to disptach
 *
 * @return boolean
 */
PHP_METHOD(Dao_Dispatcher, isFinished){


	RETURN_MEMBER(getThis(), "_finished");
}

/**
 * Sets the finished
 *
 * @param boolean $finished
 */
PHP_METHOD(Dao_Dispatcher, setFinished){

	zval *finished;

	dao_fetch_params(0, 1, 0, &finished);

	if (DAO_IS_TRUE(finished)) {
		dao_update_property(getThis(), SL("_finished"), &DAO_GLOBAL(z_true));
	} else {
		dao_update_property(getThis(), SL("_finished"), &DAO_GLOBAL(z_false));
	}
}

/**
 * Sets the latest returned value by an action manually
 *
 * @param mixed $value
 */
PHP_METHOD(Dao_Dispatcher, setReturnedValue){

	zval *value;

	dao_fetch_params(0, 1, 0, &value);

	dao_update_property(getThis(), SL("_returnedValue"), value);

}

/**
 * Returns value returned by the lastest dispatched action
 *
 * @return mixed
 */
PHP_METHOD(Dao_Dispatcher, getReturnedValue){


	RETURN_MEMBER(getThis(), "_returnedValue");
}

/**
 * Dispatches a handle action taking into account the routing parameters
 *
 * @return object
 */
PHP_METHOD(Dao_Dispatcher, dispatch){

	zval dependency_injector = {}, events_manager = {}, event_name = {}, exception_code = {}, exception_message = {}, status = {}, handler = {};
	zval handler_suffix = {}, action_suffix = {}, debug_message = {};
	int number_dispatches = 0, max_dispatches = 256;

	if (unlikely(DAO_GLOBAL(debug).enable_debug)) {
		ZVAL_STRING(&debug_message, "Dispatch: ");
		DAO_DEBUG_LOG(&debug_message);
		zval_ptr_dtor(&debug_message);
	}

	DAO_MM_INIT();

	DAO_MM_CALL_METHOD(&dependency_injector, getThis(), "getdi");
	DAO_MM_ADD_ENTRY(&dependency_injector);
	if (Z_TYPE(dependency_injector) != IS_OBJECT) {
		ZVAL_LONG(&exception_code, DAO_EXCEPTION_NO_DI);
		DAO_MM_ZVAL_STRING(&exception_message, "A dependency injection container is required to access related dispatching services");
		DAO_MM_CALL_METHOD(NULL, getThis(), "_throwdispatchexception", &exception_message, &exception_code);
		RETURN_MM();
	}

	DAO_MM_CALL_METHOD(&events_manager, getThis(), "geteventsmanager");
	DAO_MM_ADD_ENTRY(&events_manager);

	/**
	 * Calling beforeDispatchLoop
	 */
	DAO_MM_ZVAL_STRING(&event_name, "dispatch:beforeDispatchLoop");
	DAO_MM_CALL_METHOD(&status, getThis(), "fireeventcancel", &event_name);

	if (DAO_IS_FALSE(&status)) {
		RETURN_MM_FALSE;
	}
	zval_ptr_dtor(&status);

	dao_read_property(&handler_suffix, getThis(), SL("_handlerSuffix"), PH_NOISY|PH_READONLY);
	dao_read_property(&action_suffix, getThis(), SL("_actionSuffix"), PH_NOISY|PH_READONLY);

	/**
	 * Do at least one dispatch
	 */
	dao_update_property(getThis(), SL("_finished"), &DAO_GLOBAL(z_false));

	do {
		zval finished = {}, namespace_name = {}, handler_name = {}, action_name = {}, camelize = {}, camelized_class = {};
		zval handler_class = {}, has_service = {}, was_fresh = {}, action_method = {}, action_params = {}, params = {}, tmp_params = {}, *param, logic_binding = {}, reflection_method = {};
		zval reflection_parameters = {}, *reflection_parameter, call_object = {}, value = {}, exception = {};
		zend_class_entry *reflection_method_ce;
		zend_string *param_key;
		ulong param_idx;
		long int count_action_params = 0;

		if (unlikely(DAO_GLOBAL(debug).enable_debug)) {
			zval times = {};
			ZVAL_LONG(&times, number_dispatches);
			DAO_CONCAT_SV(&debug_message, "--number_dispatches: ", &times);
			DAO_DEBUG_LOG(&debug_message);
			zval_ptr_dtor(&debug_message);
		}

		/**
		 * Loop until finished is false
		 */
		dao_read_property(&finished, getThis(), SL("_finished"), PH_READONLY);
		if (zend_is_true(&finished)) {
			break;
		}

		++number_dispatches;

		/**
		 * Throw an exception after 256 consecutive forwards
		 */
		if (number_dispatches == max_dispatches) {
			ZVAL_LONG(&exception_code, DAO_EXCEPTION_CYCLIC_ROUTING);
			DAO_MM_ZVAL_STRING(&exception_message, "Dispatcher has detected a cyclic routing causing stability problems");
			DAO_MM_CALL_METHOD(NULL, getThis(), "_throwdispatchexception", &exception_message, &exception_code);
			break;
		}

		dao_update_property(getThis(), SL("_finished"), &DAO_GLOBAL(z_true));

		/**
		 * If the current namespace is null we used the set in this_ptr::_defaultNamespace
		 */
		dao_read_property(&namespace_name, getThis(), SL("_namespaceName"), PH_READONLY);
		if (!zend_is_true(&namespace_name)) {
			dao_read_property(&namespace_name, getThis(), SL("_defaultNamespace"), PH_READONLY);
			dao_update_property(getThis(), SL("_namespaceName"), &namespace_name);
		}

		/**
		 * If the handler is null we use the set in this_ptr::_defaultHandler
		 */
		dao_read_property(&handler_name, getThis(), SL("_handlerName"), PH_READONLY);
		if (!zend_is_true(&handler_name)) {
			dao_read_property(&handler_name, getThis(), SL("_defaultHandler"), PH_READONLY);
			dao_update_property(getThis(), SL("_handlerName"), &handler_name);
		}

		/**
		 * If the action is null we use the set in this_ptr::_defaultAction
		 */
		dao_read_property(&action_name, getThis(), SL("_actionName"), PH_READONLY);
		if (!zend_is_true(&action_name)) {
			dao_read_property(&action_name, getThis(), SL("_defaultAction"), PH_READONLY);
			dao_update_property(getThis(), SL("_actionName"), &action_name);
		}

		if (unlikely(DAO_GLOBAL(debug).enable_debug)) {
			DAO_CONCAT_SV(&debug_message, "--namespace: ", &namespace_name);
			DAO_DEBUG_LOG(&debug_message);
			zval_ptr_dtor(&debug_message);
			DAO_CONCAT_SV(&debug_message, "--handler name: ", &handler_name);
			DAO_DEBUG_LOG(&debug_message);
			zval_ptr_dtor(&debug_message);
			DAO_CONCAT_SV(&debug_message, "--action name: ", &action_name);
			DAO_DEBUG_LOG(&debug_message);
			zval_ptr_dtor(&debug_message);
		}

		/**
		 * Calling beforeDispatch
		 */
		if (Z_TYPE(events_manager) == IS_OBJECT) {
			DAO_MM_ZVAL_STRING(&event_name, "dispatch:beforeDispatch");
			DAO_MM_CALL_METHOD(&status, getThis(), "fireeventcancel", &event_name);
			if (DAO_IS_FALSE(&status)) {
				continue;
			}
			zval_ptr_dtor(&status);

			/**
			 * Check if the user made a forward in the listener
			 */
			dao_read_property(&finished, getThis(), SL("_finished"), PH_READONLY);
			if (DAO_IS_FALSE(&finished)) {
				continue;
			}
		}

		/**
		 * We don't camelize the classes if they are in namespaces
		 */
		if (!dao_memnstr_str(&handler_name, SL("\\"))) {
			dao_read_property(&camelize, getThis(), SL("_camelizeController"), PH_READONLY);
			if (!zend_is_true(&camelize)) {
				DAO_MM_ZVAL_COPY(&camelized_class, &handler_name);
			} else {
				dao_camelize(&camelized_class, &handler_name);
				DAO_MM_ADD_ENTRY(&camelized_class);
			}
		} else if (dao_start_with_str(&handler_name, SL("\\"))) {
			DAO_MM_ZVAL_STRINGL(&camelized_class, Z_STRVAL(handler_name)+1, Z_STRLEN(handler_name)-1);
		} else {
			DAO_MM_ZVAL_COPY(&camelized_class, &handler_name);
		}

		/**
		 * Create the complete controller class name prepending the namespace
		 */
		if (zend_is_true(&namespace_name)) {
			zval camelized_namespace = {};
			dao_read_property(&camelize, getThis(), SL("_camelizeNamespace"), PH_READONLY);
			if (!zend_is_true(&camelize)) {
				ZVAL_COPY(&camelized_namespace, &namespace_name);
			} else {
				dao_camelize(&camelized_namespace, &namespace_name);
			}
			if (dao_end_with_str(&camelized_namespace, SL("\\"))) {
				DAO_CONCAT_VVV(&handler_class, &camelized_namespace, &camelized_class, &handler_suffix);
			} else {
				DAO_CONCAT_VSVV(&handler_class, &camelized_namespace, "\\", &camelized_class, &handler_suffix);
			}
			zval_ptr_dtor(&camelized_namespace);
		} else {
			DAO_CONCAT_VV(&handler_class, &camelized_class, &handler_suffix);
		}
		DAO_MM_ADD_ENTRY(&handler_class);

		/**
		 * Handlers are retrieved as shared instances from the Service Container
		 */
		DAO_MM_CALL_METHOD(&has_service, &dependency_injector, "has", &handler_class);
		if (!zend_is_true(&has_service)) {
			/**
			 * DI doesn't have a service with that name, try to load it using an autoloader
			 */
			assert(Z_TYPE(handler_class) == IS_STRING);
			ZVAL_BOOL(&has_service, (dao_class_exists(&handler_class, 1) != NULL) ? 1 : 0);

		}

		/**
		 * If the service cannot be loaded we throw an exception
		 */
		if (!zend_is_true(&has_service)) {
			ZVAL_LONG(&exception_code, DAO_EXCEPTION_HANDLER_NOT_FOUND);
			DAO_CONCAT_VS(&exception_message, &handler_class, " handler class cannot be loaded");
			DAO_MM_ADD_ENTRY(&exception_message);
			DAO_MM_CALL_METHOD(&status, getThis(), "_throwdispatchexception", &exception_message, &exception_code);
			if (DAO_IS_FALSE(&status)) {
				/**
				 * Check if the user made a forward in the listener
				 */
				dao_read_property(&finished, getThis(), SL("_finished"), PH_READONLY);
				if (DAO_IS_FALSE(&finished)) {
					continue;
				}
			}

			break;
		}

		/**
		 * Handlers must be only objects
		 */
		DAO_MM_CALL_METHOD(&handler, &dependency_injector, "getshared", &handler_class);
		DAO_MM_ADD_ENTRY(&handler);
		if (Z_TYPE(handler) != IS_OBJECT) {
			ZVAL_LONG(&exception_code, DAO_EXCEPTION_INVALID_HANDLER);
			DAO_MM_ZVAL_STRING(&exception_message, "Invalid handler returned from the services container");
			DAO_MM_CALL_METHOD(&status, getThis(), "_throwdispatchexception", &exception_message, &exception_code);
			if (DAO_IS_FALSE(&status)) {
				dao_read_property(&finished, getThis(), SL("_finished"), PH_READONLY);
				if (DAO_IS_FALSE(&finished)) {
					continue;
				}
			}
			zval_ptr_dtor(&status);

			break;
		}

		/**
		 * If the object was recently created in the DI we initialize it
		 */
		DAO_MM_CALL_METHOD(&was_fresh, &dependency_injector, "wasfreshinstance");

		/**
		 * Update the active handler making it available for events
		 */
		dao_update_property(getThis(), SL("_activeHandler"), &handler);

		/**
		 * Check if the method exists in the handler
		 */
		DAO_CONCAT_VV(&action_method, &action_name, &action_suffix);
		DAO_MM_ADD_ENTRY(&action_method);
		if (dao_method_exists(&handler, &action_method) == FAILURE) {
			/**
			 * Call beforeNotFoundAction
			 */
			if (Z_TYPE(events_manager) == IS_OBJECT) {
				DAO_MM_ZVAL_STRING(&event_name, "dispatch:beforeNotFoundAction");
				DAO_MM_CALL_METHOD(&status, getThis(), "fireeventcancel", &event_name);

				if (DAO_IS_FALSE(&status)) {
					continue;
				}
				zval_ptr_dtor(&status);

				dao_read_property(&finished, getThis(), SL("_finished"), PH_READONLY);
				if (DAO_IS_FALSE(&finished)) {
					continue;
				}
			}

			ZVAL_LONG(&exception_code, DAO_EXCEPTION_ACTION_NOT_FOUND);
			DAO_CONCAT_SVSVS(&exception_message, "Action '", &action_name, "' was not found on handler '", &handler_name, "'");
			DAO_MM_ADD_ENTRY(&exception_message);

			/**
			 * Try to throw an exception when an action isn't defined on the object
			 */
			DAO_MM_CALL_METHOD(&status, getThis(), "_throwdispatchexception", &exception_message, &exception_code);
			if (DAO_IS_FALSE(&status)) {
				dao_read_property(&finished, getThis(), SL("_finished"), PH_READONLY);
				if (DAO_IS_FALSE(&finished)) {
					continue;
				}
			}
			zval_ptr_dtor(&status);
			break;
		}

		/**
		 * Calling beforeExecuteRoute
		 */
		if (Z_TYPE(events_manager) == IS_OBJECT) {
			DAO_MM_ZVAL_STRING(&event_name, "dispatch:beforeExecuteRoute");
			DAO_MM_CALL_METHOD(&status, getThis(), "fireeventcancel", &event_name);
			if (DAO_IS_FALSE(&status)) {
				continue;
			}
			zval_ptr_dtor(&status);

			/**
			 * Check if the user made a forward in the listener
			 */
			dao_read_property(&finished, getThis(), SL("_finished"), PH_READONLY);
			if (DAO_IS_FALSE(&finished)) {
				continue;
			}
		}

		/**
		 * Calling beforeExecuteRoute as callback and event
		 */
		if (dao_method_exists_ex(&handler, SL("beforeexecuteroute")) == SUCCESS) {
			DAO_MM_CALL_METHOD(&status, &handler, "beforeexecuteroute", getThis());
			if (DAO_IS_FALSE(&status)) {
				continue;
			}

			/**
			 * Check if the user made a forward in the listener
			 */
			dao_read_property(&finished, getThis(), SL("_finished"), PH_READONLY);
			if (DAO_IS_FALSE(&finished)) {
				continue;
			}
		}

		/**
		 * Call the 'initialize' method just once per request
		 */
		if (DAO_IS_TRUE(&was_fresh)) {
			/**
			 * Calling afterInitialize
			 */
			if (unlikely(Z_TYPE(events_manager) == IS_OBJECT)) {
				DAO_MM_ZVAL_STRING(&event_name, "dispatch:beforeInitialize");
				DAO_MM_CALL_METHOD(&status, getThis(), "fireeventcancel", &event_name);
				if (DAO_IS_FALSE(&status)) {
					continue;
				}
				zval_ptr_dtor(&status);

				/**
				 * Check if the user made a forward in the listener
				 */
				dao_read_property(&finished, getThis(), SL("_finished"), PH_READONLY);
				if (DAO_IS_FALSE(&finished)) {
					continue;
				}
			}

			if (dao_method_exists_ex(&handler, SL("initialize")) == SUCCESS) {
				int flag = 0;
				DAO_CALL_METHOD_FLAG(flag, NULL, &handler, "initialize");
				if (flag && EG(exception)) {
					zval e = {};
					ZVAL_OBJ(&e, EG(exception));
					
					if (likely(!instanceof_function_ex(Z_OBJCE(e), dao_continueexception_ce, 0))) {
						RETURN_MM();
					}

#if PHP_VERSION_ID >= 80000
					ZVAL_OBJ(&exception, zend_objects_clone_obj( EG(exception)));
#else
					ZVAL_OBJ(&exception, zend_objects_clone_obj(&e));
#endif
					DAO_MM_ADD_ENTRY(&exception);
					dao_update_property(getThis(), SL("_lastException"), &exception);

					/* Clear the exception  */
					zend_clear_exception();

					/**
					 * Calling afterInitialize
					 */
					if (unlikely(Z_TYPE(events_manager) == IS_OBJECT)) {
						DAO_MM_ZVAL_STRING(&event_name, "dispatch:afterInitialize");
						DAO_MM_CALL_METHOD(&status, getThis(), "fireeventcancel", &event_name);
						if (DAO_IS_FALSE(&status)) {
							continue;
						}
						zval_ptr_dtor(&status);

						/**
						 * Check if the user made a forward in the listener
						 */
						dao_read_property(&finished, getThis(), SL("_finished"), PH_READONLY);
						if (DAO_IS_FALSE(&finished)) {
							continue;
						}
					}

					dao_update_property(getThis(), SL("_lastHandler"), &handler);
					goto afterexecuteroute;
				}
			}

			/**
			 * Calling afterInitialize
			 */
			if (unlikely(Z_TYPE(events_manager) == IS_OBJECT)) {
				DAO_MM_ZVAL_STRING(&event_name, "dispatch:afterInitialize");
				DAO_MM_CALL_METHOD(&status, getThis(), "fireeventcancel", &event_name);
				if (DAO_IS_FALSE(&status)) {
					continue;
				}
				zval_ptr_dtor(&status);

				/**
				 * Check if the user made a forward in the listener
				 */
				dao_read_property(&finished, getThis(), SL("_finished"), PH_READONLY);
				if (DAO_IS_FALSE(&finished)) {
					continue;
				}
			}
		}

		/**
		 * Check if the params is an array
		 */
		dao_read_property(&action_params, getThis(), SL("_params"), PH_READONLY);
		if (Z_TYPE(action_params) != IS_ARRAY) {
			ZVAL_LONG(&exception_code, DAO_EXCEPTION_INVALID_PARAMS);
			DAO_MM_ZVAL_STRING(&exception_message, "Action parameters must be an Array");

			/**
			 * An invalid parameter variable was passed throw an exception
			 */
			DAO_MM_CALL_METHOD(&status, getThis(), "_throwdispatchexception", &exception_message, &exception_code);
			if (DAO_IS_FALSE(&status)) {
				dao_read_property(&finished, getThis(), SL("_finished"), PH_READONLY);
				if (DAO_IS_FALSE(&finished)) {
					continue;
				}
			}
			break;
		}

		/**
		 * Check if logic binding
		 */
		dao_read_property(&logic_binding, getThis(), SL("_logicBinding"), PH_READONLY);
		if (zend_is_true(&logic_binding)) {
			DAO_MM_ZVAL_DUP(&tmp_params, &action_params);

			count_action_params = dao_fast_count_int(&action_params);

			array_init(&params);
			DAO_MM_ADD_ENTRY(&params);

			reflection_method_ce = dao_fetch_str_class(SL("ReflectionMethod"), ZEND_FETCH_CLASS_AUTO);
			object_init_ex(&reflection_method, reflection_method_ce);
			DAO_MM_ADD_ENTRY(&reflection_method);
			DAO_MM_CALL_METHOD(NULL, &reflection_method, "__construct", &handler, &action_method);
			DAO_MM_CALL_METHOD(&reflection_parameters, &reflection_method, "getparameters");
			DAO_MM_ADD_ENTRY(&reflection_parameters);

			ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL(reflection_parameters), param_idx, param_key, reflection_parameter) {
				zval key = {}, reflection_class = {}, logic_classname = {}, logic = {}, var_name = {}, var_value = {};
				zend_class_entry *logic_ce;
				if (param_key) {
					ZVAL_STR(&key, param_key);
				} else {
					ZVAL_LONG(&key, param_idx);
				}

#if PHP_VERSION_ID >= 80000
				DAO_MM_CALL_METHOD(&reflection_class, reflection_parameter, "gettype");
#else
				DAO_MM_CALL_METHOD(&reflection_class, reflection_parameter, "getclass");
#endif
				DAO_MM_ADD_ENTRY(&reflection_class);
				if (Z_TYPE(reflection_class) == IS_OBJECT) {
					DAO_MM_CALL_METHOD(&logic_classname, &reflection_class, "getname");
					DAO_MM_ADD_ENTRY(&logic_classname);
					if (Z_TYPE(logic_classname) == IS_STRING) {
						logic_ce = dao_fetch_class(&logic_classname, ZEND_FETCH_CLASS_AUTO);
						if (instanceof_function_ex(logic_ce, dao_user_logic_ce, 0)) {
							DAO_MM_CALL_CE_STATIC(&logic, logic_ce, "call", &action_name, &action_params);

							dao_array_update(&params, &key, &logic, 0);

							if (dao_method_exists_ex(&logic, SL("start")) == SUCCESS) {
								DAO_MM_CALL_METHOD(NULL, &logic, "start");
							}
						}
					}
				} else {
					DAO_MM_CALL_METHOD(&var_name, reflection_parameter, "getname");
					if (dao_array_isset_fetch(&var_value, &action_params, &var_name, 0)) {
						dao_array_update(&params, &var_name, &var_value, PH_COPY);
						dao_array_unset(&tmp_params, &var_name, 0);
					} else if (count_action_params >= 0 && dao_array_isset_fetch(&var_value, &action_params, &key, 0)) {
						dao_array_update(&params, &key, &var_value, PH_COPY);
						dao_array_unset(&tmp_params, &key, 0);
					} else if (count_action_params) {
						dao_array_get_current(&var_value, &action_params);
						dao_array_update(&params, &key, &var_value, 0);

						dao_array_get_key(&var_name, &action_params);
						dao_array_unset(&tmp_params, &var_name, 0);
					}
					zval_ptr_dtor(&var_name);
				}
				if (count_action_params) {
					zend_hash_move_forward(Z_ARRVAL_P(&action_params));
					count_action_params -= 1;
				}
			} ZEND_HASH_FOREACH_END();

			ZEND_HASH_FOREACH_VAL(Z_ARRVAL(tmp_params), param) {
				dao_array_append(&params, param, PH_COPY);
			} ZEND_HASH_FOREACH_END();
		} else {
			DAO_MM_ZVAL_COPY(&params, &action_params);
		}

		/**
		 * Create a call handler
		 */
		array_init_size(&call_object, 2);
		dao_array_append(&call_object, &handler, PH_COPY);
		dao_array_append(&call_object, &action_method, PH_COPY);
		DAO_MM_ADD_ENTRY(&call_object);

		/* Call the method allowing exceptions */
		dao_call_user_func_array_noex(&value, &call_object, &params);
		DAO_MM_ADD_ENTRY(&value);

		/* Check if an exception has ocurred */
		if (EG(exception)) {
#if PHP_VERSION_ID >= 80000
			ZVAL_OBJ(&exception, zend_objects_clone_obj(EG(exception)));
#else
			zval e = {};
			ZVAL_OBJ(&e, EG(exception));
			ZVAL_OBJ(&exception, zend_objects_clone_obj(&e));
#endif
			DAO_MM_ADD_ENTRY(&exception);
			dao_update_property(getThis(), SL("_lastException"), &exception);

			/* Clear the exception  */
			zend_clear_exception();

			/* Try to handle the exception */
			DAO_MM_CALL_METHOD(&status, getThis(), "_handleexception", &exception);

			if (likely(!instanceof_function_ex(Z_OBJCE(exception), dao_continueexception_ce, 0))) {
				if (DAO_IS_FALSE(&status)) {
					dao_read_property(&finished, getThis(), SL("_finished"), PH_READONLY);
					if (DAO_IS_FALSE(&finished)) {
						continue;
					}
				} else {
					/* Exception was not handled, rethrow it */
					dao_throw_exception(&exception);
					goto end;
				}
			}
		} else {
			/* Update the latest value produced by the latest handler */
			dao_update_property(getThis(), SL("_returnedValue"), &value);
		}

		dao_update_property(getThis(), SL("_lastHandler"), &handler);

		if (zend_is_true(&logic_binding)) {
			ZEND_HASH_FOREACH_VAL(Z_ARRVAL(params), param) {
				if (Z_TYPE_P(param) == IS_OBJECT && instanceof_function_ex(Z_OBJCE_P(param), dao_user_logic_ce, 0)) {
					if (dao_method_exists_ex(param, SL("finish")) == SUCCESS) {
						DAO_MM_CALL_METHOD(NULL, param, "finish");
					}
				}
			} ZEND_HASH_FOREACH_END();
		}

afterexecuteroute:

		if (Z_TYPE(events_manager) == IS_OBJECT) {
			/**
			 * Call afterExecuteRoute
			 */
			DAO_MM_ZVAL_STRING(&event_name, "dispatch:afterExecuteRoute");
			DAO_MM_CALL_METHOD(NULL, getThis(), "fireevent", &event_name);

			dao_read_property(&finished, getThis(), SL("_finished"), PH_READONLY);
			if (DAO_IS_FALSE(&finished)) {
				continue;
			}

			/**
			 * Call afterDispatch
			 */
			DAO_MM_ZVAL_STRING(&event_name, "dispatch:afterDispatch");
			DAO_MM_CALL_METHOD(NULL, getThis(), "fireevent", &event_name);
		}

		/**
		 * Calling afterExecuteRoute as callback and event
		 */
		if (dao_method_exists_ex(&handler, SL("afterexecuteroute")) == SUCCESS) {
			DAO_MM_CALL_METHOD(&status, &handler, "afterexecuteroute", getThis(), &value);
			if (DAO_IS_FALSE(&status)) {
				continue;
			}
			zval_ptr_dtor(&status);

			dao_read_property(&finished, getThis(), SL("_finished"), PH_READONLY);
			if (DAO_IS_FALSE(&finished)) {
				continue;
			}
		}
	} while (number_dispatches <= max_dispatches);

	RETVAL_ZVAL(&handler, 1, 0);

end:

	/**
	 * Call afterDispatchLoop
	 */
	DAO_MM_ZVAL_STRING(&event_name, "dispatch:afterDispatchLoop");
	DAO_MM_CALL_METHOD(NULL, getThis(), "fireevent", &event_name);
	RETURN_MM();
}

/**
 * Forwards the execution flow to another controller/action
 * Dispatchers are unique per module. Forwarding between modules is not allowed
 *
 *<code>
 *  $this->dispatcher->forward(array('namespace' => 'my', 'controller' => 'posts', 'action' => 'index'));
 *  $this->dispatcher->forward('my\\posts::index');
 *</code>
 *
 * @param string|array $forward
 * @return bool
 */
PHP_METHOD(Dao_Dispatcher, forward){

	zval *forward, event_name = {}, forward_parts = {}, parts = {}, number_parts = {}, controller_part = {}, real_namespace_name = {}, real_controller_name = {}, action_part = {}, exception_code = {}, exception_message = {};
	zval namespace_name = {}, controller_name = {}, task_name = {}, action_name = {}, params = {}, previous_namespace_name = {}, previous_controller_name = {}, previous_action_name = {}, previous_params = {};
	int num = 0;

	dao_fetch_params(1, 1, 0, &forward);

	DAO_MM_ZVAL_STRING(&event_name, "dispatch:beforeForward");
	DAO_MM_CALL_METHOD(NULL, getThis(), "fireevent", &event_name, forward);

	if (Z_TYPE_P(forward) == IS_STRING) {
		array_init(&forward_parts);
		DAO_MM_ADD_ENTRY(&forward_parts);
		dao_fast_explode_str(&parts, SL("::"), forward);
		dao_fast_count(&number_parts, &parts);

		num = dao_get_intval(&number_parts);

		if (num > 0 && num <= 3) {
			switch (num) {

				case 3:
					dao_array_fetch_long(&controller_part, &parts, 1, PH_NOISY|PH_READONLY);
					dao_array_fetch_long(&action_part, &parts, 2, PH_NOISY|PH_READONLY);
					break;

				case 2:
					dao_array_fetch_long(&controller_part, &parts, 0, PH_NOISY|PH_READONLY);
					dao_array_fetch_long(&action_part, &parts, 1, PH_NOISY|PH_READONLY);
					break;

				case 1:
					dao_array_fetch_long(&controller_part, &parts, 0, PH_NOISY|PH_READONLY);
					break;

			}

			if (dao_memnstr_str(&controller_part, SL("\\"))) {
				dao_get_class_ns(&real_controller_name, &controller_part, 0);

				dao_array_update_str(&forward_parts, SL("controller"), &real_controller_name, PH_COPY);
				dao_get_ns_class(&real_namespace_name, &controller_part, 0);

				if (zend_is_true(&namespace_name)) {
					dao_array_update_str(&forward_parts, SL("namespace"), &real_namespace_name, 0);
				}
			} else {
				ZVAL_COPY(&real_controller_name, &controller_part);
			}

			dao_uncamelize(&controller_part, &real_controller_name);
			zval_ptr_dtor(&real_controller_name);
			dao_array_update_str(&forward_parts, SL("controller"), &controller_part, 0);

			if (Z_TYPE(action_part) > IS_NULL) {
				dao_array_update_str(&forward_parts, SL("action"), &action_part, PH_COPY);
			}
		}
		zval_ptr_dtor(&parts);
	} else {
		DAO_MM_ZVAL_COPY(&forward_parts, forward);
	}

	if (Z_TYPE(forward_parts) != IS_ARRAY) {
		ZVAL_LONG(&exception_code, DAO_EXCEPTION_INVALID_PARAMS);
		DAO_MM_ZVAL_STRING(&exception_message, "Forward parameter must be an Array");
		DAO_MM_CALL_METHOD(NULL, getThis(), "_throwdispatchexception", &exception_message, &exception_code);
		RETURN_MM();
	}

	dao_read_property(&previous_namespace_name, getThis(), SL("_namespaceName"), PH_READONLY);
	dao_update_property(getThis(), SL("_previousNamespaceName"), &previous_namespace_name);

	dao_read_property(&previous_controller_name, getThis(), SL("_handlerName"), PH_READONLY);
	dao_update_property(getThis(), SL("_previousHandlerName"), &previous_controller_name);

	dao_read_property(&previous_action_name, getThis(), SL("_actionName"), PH_READONLY);
	dao_update_property(getThis(), SL("_previousActionName"), &previous_action_name);

	dao_read_property(&previous_params, getThis(), SL("_params"), PH_READONLY);
	dao_update_property(getThis(), SL("_previousParams"), &previous_params);

	/**
	 * Check if we need to forward to another namespace
	 */
	if (dao_array_isset_fetch_str(&namespace_name, &forward_parts, SL("namespace"), PH_READONLY)) {
		dao_update_property(getThis(), SL("_namespaceName"), &namespace_name);
	}

	/**
	 * Check if we need to forward to another controller
	 */
	if (dao_array_isset_fetch_str(&controller_name, &forward_parts, SL("controller"), PH_READONLY)) {
		dao_update_property(getThis(), SL("_handlerName"), &controller_name);
	} else if (dao_array_isset_fetch_str(&task_name, &forward_parts, SL("task"), PH_READONLY)) {
		dao_update_property(getThis(), SL("_handlerName"), &task_name);
	}

	/**
	 * Check if we need to forward to another action
	 */
	if (dao_array_isset_fetch_str(&action_name, &forward_parts, SL("action"), PH_READONLY)) {
		dao_update_property(getThis(), SL("_actionName"), &action_name);
	}

	/**
	 * Check if we need to forward changing the current parameters
	 */
	if (dao_array_isset_fetch_str(&params, &forward_parts, SL("params"), PH_READONLY)) {
		dao_update_property(getThis(), SL("_params"), &params);
	}

	dao_update_property(getThis(), SL("_finished"), &DAO_GLOBAL(z_false));
	dao_update_property(getThis(), SL("_forwarded"), &DAO_GLOBAL(z_true));

	DAO_MM_ZVAL_STRING(&event_name, "dispatch:afterForward");
	DAO_MM_CALL_METHOD(NULL, getThis(), "fireevent", &event_name, forward);
	RETURN_MM();
}

/**
 * Check if the current executed action was forwarded by another one
 *
 * @return boolean
 */
PHP_METHOD(Dao_Dispatcher, wasForwarded){


	RETURN_MEMBER(getThis(), "_forwarded");
}

/**
 * Possible class name that will be located to dispatch the request
 *
 * @return string
 */
PHP_METHOD(Dao_Dispatcher, getHandlerClass){

	zval handler_suffix = {}, namespace_name = {}, handler_name = {}, camelized_class = {}, camelize = {};

	/**
	 * The handler suffix
	 */
	dao_read_property(&handler_suffix, getThis(), SL("_handlerSuffix"), PH_NOISY|PH_READONLY);

	/**
	 * If the current namespace is null we used the set in this_ptr::_defaultNamespace
	 */
	dao_read_property(&namespace_name, getThis(), SL("_namespaceName"), PH_NOISY|PH_READONLY);
	if (!zend_is_true(&namespace_name)) {
		dao_read_property(&namespace_name, getThis(), SL("_defaultNamespace"), PH_NOISY|PH_READONLY);
		dao_update_property(getThis(), SL("_namespaceName"), &namespace_name);
	}

	/**
	 * If the handler is null we use the set in this_ptr::_defaultHandler
	 */
	dao_read_property(&handler_name, getThis(), SL("_handlerName"), PH_NOISY|PH_READONLY);
	if (!zend_is_true(&handler_name)) {
		dao_read_property(&handler_name, getThis(), SL("_defaultHandler"), PH_NOISY|PH_READONLY);
		dao_update_property(getThis(), SL("_handlerName"), &handler_name);
	}

	/**
	 * We don't camelize the classes if they are in namespaces
	 */
	if (!dao_memnstr_str(&handler_name, SL("\\"))) {
		dao_camelize(&camelized_class, &handler_name);
	} else if (dao_start_with_str(&handler_name, SL("\\"))) {
		ZVAL_STRINGL(&camelized_class, Z_STRVAL(handler_name)+1, Z_STRLEN(handler_name)-1);
	} else {
		ZVAL_COPY(&camelized_class, &handler_name);
	}

	/**
	 * Create the complete controller class name prepending the namespace
	 */
	if (zend_is_true(&namespace_name)) {
		zval camelized_namespace = {};
		dao_read_property(&camelize, getThis(), SL("_camelizeNamespace"), PH_NOISY|PH_READONLY);
		if (!zend_is_true(&camelize)) {
			ZVAL_COPY(&camelized_namespace, &namespace_name);
		} else {
			dao_camelize(&camelized_namespace, &namespace_name);
		}
		if (dao_end_with_str(&camelized_namespace, SL("\\"))) {
			DAO_CONCAT_VVV(return_value, &camelized_namespace, &camelized_class, &handler_suffix);
		} else {
			DAO_CONCAT_VSVV(return_value, &camelized_namespace, "\\", &camelized_class, &handler_suffix);
		}
		zval_ptr_dtor(&camelized_namespace);
	} else {
		DAO_CONCAT_VV(return_value, &camelized_class, &handler_suffix);
	}
	zval_ptr_dtor(&camelized_class);

	dao_update_property(getThis(), SL("_isExactHandler"), &DAO_GLOBAL(z_false));
}

/**
 * Enables/Disables automatically camelize namespace
 *
 *<code>
 *  $this->dispatcher->camelizeNamespace(FALSE);
 *</code>
 *
 * @param bool $camelize
 */
PHP_METHOD(Dao_Dispatcher, camelizeNamespace){

	zval *camelize;

	dao_fetch_params(0, 1, 0, &camelize);

	if (DAO_IS_TRUE(camelize)) {
		dao_update_property(getThis(), SL("_camelizeNamespace"), &DAO_GLOBAL(z_true));
	} else {
		dao_update_property(getThis(), SL("_camelizeNamespace"), &DAO_GLOBAL(z_false));
	}
}

/**
 * Enables/Disables automatically camelize controller
 *
 *<code>
 *  $this->dispatcher->camelizeController(FALSE);
 *</code>
 *
 * @param bool $camelize
 */
PHP_METHOD(Dao_Dispatcher, camelizeController){

	zval *camelize;

	dao_fetch_params(0, 1, 0, &camelize);

	if (DAO_IS_TRUE(camelize)) {
		dao_update_property(getThis(), SL("_camelizeController"), &DAO_GLOBAL(z_true));
	} else {
		dao_update_property(getThis(), SL("_camelizeController"), &DAO_GLOBAL(z_false));
	}
}

/**
 * Set error handler
 *
 * @param mixed $error_handler
 * @param int $exception_code
 * @return Dao\DispatcherInterface
 */
PHP_METHOD(Dao_Dispatcher, setErrorHandler){

	zval *error_handler, *exception_code = NULL;

	dao_fetch_params(0, 1, 1, &error_handler, &exception_code);

	if (!exception_code) {
		exception_code = &DAO_GLOBAL(z_zero);
	}

	dao_update_property_array(getThis(), SL("_errorHandlers"), exception_code, error_handler);

	RETURN_THIS();
}

/**
 * Get error handler
 *
 * @param int $exception_code
 * @return mixed
 */
PHP_METHOD(Dao_Dispatcher, getErrorHandler){

	zval *exception_code = NULL, error_handlers = {}, error_handler = {};

	dao_fetch_params(0, 0, 1, &exception_code);

	if (!exception_code) {
		exception_code = &DAO_GLOBAL(z_zero);
	}

	dao_read_property(&error_handlers, getThis(), SL("_errorHandlers"), PH_NOISY|PH_READONLY);

	if (Z_TYPE(error_handlers) == IS_ARRAY) {
		if (dao_array_isset_fetch(&error_handler, &error_handlers, exception_code, PH_READONLY)) {
			RETURN_CTOR(&error_handler);
		}
	}

	RETURN_NULL();
}

/**
 * Returns the last exception
 *
 * @return \Exception
 */
PHP_METHOD(Dao_Dispatcher, getLastException){


	RETURN_MEMBER(getThis(), "_lastException");
}

/**
 * Returns the last handler
 *
 * @return Object
 */
PHP_METHOD(Dao_Dispatcher, getLastHandler){


	RETURN_MEMBER(getThis(), "_lastHandler");
}

/**
 * Returns the previons namespace
 *
 * @return string
 */
PHP_METHOD(Dao_Dispatcher, getPreviousNamespaceName){


	RETURN_MEMBER(getThis(), "_previousNamespaceName");
}

/**
 * Returns the previons action
 *
 * @return string
 */
PHP_METHOD(Dao_Dispatcher, getPreviousActionName){


	RETURN_MEMBER(getThis(), "_previousActionName");
}

/**
 * Returns the previons action params
 *
 * @return array
 */
PHP_METHOD(Dao_Dispatcher, getPreviousParams){


	RETURN_MEMBER(getThis(), "_previousParams");
}

/**
 * Gets a previons param by its name or numeric index
 *
 * @param mixed $param
 * @param string|array $filters
 * @param mixed $defaultValue
 * @return mixed
 */
PHP_METHOD(Dao_Dispatcher, getPreviousParam){

	zval *param, *filters = NULL, *default_value = NULL, params = {}, param_value = {}, exception_code = {}, exception_message = {};
	zval service = {}, filter = {};

	dao_fetch_params(1, 1, 2, &param, &filters, &default_value);

	dao_read_property(&params, getThis(), SL("_previousParams"), PH_NOISY|PH_READONLY);
	if (dao_array_isset_fetch(&param_value, &params, param, PH_READONLY)) {
		if (filters && Z_TYPE_P(filters) != IS_NULL) {
			zval dependency_injector = {};
			DAO_MM_CALL_METHOD(&dependency_injector, getThis(), "getdi");
			DAO_MM_ADD_ENTRY(&dependency_injector);
			if (Z_TYPE(dependency_injector) != IS_OBJECT) {
				ZVAL_LONG(&exception_code, DAO_EXCEPTION_NO_DI);
				DAO_MM_ZVAL_STRING(&exception_message, "A dependency injection object is required to access the 'filter' service");
				DAO_MM_CALL_METHOD(NULL, getThis(), "_throwdispatchexception", &exception_message, &exception_code);
				RETURN_MM();
			}

			ZVAL_STR(&service, IS(filter));

			DAO_MM_CALL_METHOD(&filter, &dependency_injector, "getshared", &service);
			DAO_MM_ADD_ENTRY(&filter);
			DAO_MM_VERIFY_INTERFACE(&filter, dao_filterinterface_ce);
			DAO_MM_CALL_METHOD(return_value, &filter, "sanitize", &param_value, filters);
			RETURN_MM();
		} else {
			RETURN_MM_CTOR(&param_value);
		}
	}

	if (default_value) {
		RETURN_MM_CTOR(default_value);
	}

	RETURN_MM_NULL();
}
