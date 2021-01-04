
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

#include "mvc/dispatcher.h"
#include "mvc/dispatcherinterface.h"
#include "mvc/../dispatcher.h"
#include "mvc/../dispatcherinterface.h"
#include "mvc/dispatcher/exception.h"
#include "http/responseinterface.h"
#include "mvc/../debug.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/concat.h"
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/exception.h"
#include "kernel/operators.h"
#include "kernel/array.h"

#include "interned-strings.h"

/**
 * Dao\Mvc\Dispatcher
 *
 * Dispatching is the process of taking the request object, extracting the module name,
 * controller name, action name, and optional parameters contained in it, and then
 * instantiating a controller and calling an action of that controller.
 *
 *<code>
 *
 *	$di = new Dao\Di();
 *
 *	$dispatcher = new Dao\Mvc\Dispatcher();
 *
 *  $dispatcher->setDI($di);
 *
 *	$dispatcher->setControllerName('posts');
 *	$dispatcher->setActionName('index');
 *	$dispatcher->setParams(array());
 *
 *	$controller = $dispatcher->dispatch();
 *
 *</code>
 */
zend_class_entry *dao_mvc_dispatcher_ce;

PHP_METHOD(Dao_Mvc_Dispatcher, setControllerSuffix);
PHP_METHOD(Dao_Mvc_Dispatcher, getControllerSuffix);
PHP_METHOD(Dao_Mvc_Dispatcher, setDefaultController);
PHP_METHOD(Dao_Mvc_Dispatcher, setControllerName);
PHP_METHOD(Dao_Mvc_Dispatcher, getControllerName);
PHP_METHOD(Dao_Mvc_Dispatcher, _throwDispatchException);
PHP_METHOD(Dao_Mvc_Dispatcher, _handleException);
PHP_METHOD(Dao_Mvc_Dispatcher, getControllerClass);
PHP_METHOD(Dao_Mvc_Dispatcher, getLastController);
PHP_METHOD(Dao_Mvc_Dispatcher, getActiveController);
PHP_METHOD(Dao_Mvc_Dispatcher, getPreviousControllerName);
PHP_METHOD(Dao_Mvc_Dispatcher, getPreviousActionName);

static const zend_function_entry dao_mvc_dispatcher_method_entry[] = {
	PHP_ME(Dao_Mvc_Dispatcher, setControllerSuffix, arginfo_dao_mvc_dispatcherinterface_setcontrollersuffix, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Dispatcher, getControllerSuffix, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Dispatcher, setDefaultController, arginfo_dao_mvc_dispatcherinterface_setdefaultcontroller, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Dispatcher, setControllerName, arginfo_dao_mvc_dispatcherinterface_setcontrollername, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Dispatcher, getControllerName, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Dispatcher, _throwDispatchException, arginfo_empty, ZEND_ACC_PROTECTED)
	PHP_ME(Dao_Mvc_Dispatcher, _handleException, arginfo_empty, ZEND_ACC_PROTECTED)
	PHP_ME(Dao_Mvc_Dispatcher, getControllerClass, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Dispatcher, getLastController, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Dispatcher, getActiveController, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Dispatcher, getPreviousControllerName, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Dispatcher, getPreviousActionName, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Mvc\Dispatcher initializer
 */
DAO_INIT_CLASS(Dao_Mvc_Dispatcher){

	DAO_REGISTER_CLASS_EX(Dao\\Mvc, Dispatcher, mvc_dispatcher, dao_dispatcher_ce, dao_mvc_dispatcher_method_entry, 0);

	zend_declare_property_string(dao_mvc_dispatcher_ce, SL("_handlerSuffix"), "Controller", ZEND_ACC_PROTECTED);
	zend_declare_property_string(dao_mvc_dispatcher_ce, SL("_defaultHandler"), "index", ZEND_ACC_PROTECTED);
	zend_declare_property_string(dao_mvc_dispatcher_ce, SL("_defaultAction"), "index", ZEND_ACC_PROTECTED);

	zend_class_implements(dao_mvc_dispatcher_ce, 2, dao_dispatcherinterface_ce, dao_mvc_dispatcherinterface_ce);

	return SUCCESS;
}

/**
 * Sets the default controller suffix
 *
 * @param string $controllerSuffix
 */
PHP_METHOD(Dao_Mvc_Dispatcher, setControllerSuffix){

	zval *controller_suffix;

	dao_fetch_params(0, 1, 0, &controller_suffix);

	dao_update_property(getThis(), SL("_handlerSuffix"), controller_suffix);

}

/**
 * Gets the default controller suffix
 *
 * @return string
 */
PHP_METHOD(Dao_Mvc_Dispatcher, getControllerSuffix){

	RETURN_MEMBER(getThis(), "_handlerSuffix");
}

/**
 * Sets the default controller name
 *
 * @param string $controllerName
 */
PHP_METHOD(Dao_Mvc_Dispatcher, setDefaultController){

	zval *controller_name;

	dao_fetch_params(0, 1, 0, &controller_name);

	dao_update_property(getThis(), SL("_defaultHandler"), controller_name);

}

/**
 * Sets the controller name to be dispatched
 *
 * @param string $controllerName
 */
PHP_METHOD(Dao_Mvc_Dispatcher, setControllerName){

	zval *controller_name, *is_exact = NULL, name = {};

	dao_fetch_params(0, 1, 1, &controller_name, &is_exact);

	if (is_exact && zend_is_true(is_exact)) {
		DAO_CONCAT_SV(&name, "\\", controller_name);
		dao_update_property(getThis(), SL("_handlerName"), &name);
		zval_ptr_dtor(&name);
		dao_update_property(getThis(), SL("_isExactHandler"), &DAO_GLOBAL(z_true));
	} else {
		dao_update_property(getThis(), SL("_handlerName"), controller_name);
		dao_update_property(getThis(), SL("_isExactHandler"), &DAO_GLOBAL(z_false));
	}
}

/**
 * Gets last dispatched controller name
 *
 * @return string
 */
PHP_METHOD(Dao_Mvc_Dispatcher, getControllerName){

	zval is_exact = {}, handler_name = {};

	dao_read_property(&is_exact, getThis(), SL("_isExactHandler"), PH_NOISY|PH_READONLY);

	if (!zend_is_true(&is_exact)) {
		RETURN_MEMBER(getThis(), "_handlerName");
	}

	dao_read_property(&handler_name, getThis(), SL("_handlerName"), PH_READONLY);
	if (likely(Z_TYPE(handler_name) == IS_STRING) && Z_STRLEN(handler_name) > 1) {
		if (Z_STRVAL(handler_name)[0] == '\\') {
			char *c = Z_STRVAL(handler_name);
			int len = Z_STRLEN(handler_name);
			memmove(c, c+1, len); /* This will include the trailing zero */
			RETVAL_STRINGL(c, len - 1);
		}
	}
}

/**
 * Throws an internal exception
 *
 * @param string $message
 * @param int $exceptionCode
 */
PHP_METHOD(Dao_Mvc_Dispatcher, _throwDispatchException){

	zval *message, *code = NULL, error_handlers = {}, error_handler = {}, previous_namespace_name = {}, previous_controller_name = {};
	zval previous_action_name = {}, previous_params = {}, namespace_name = {}, controller_name = {}, action_name = {}, params = {};
	zval exception = {}, event_name = {}, status = {}, debug_message = {};

	dao_fetch_params(1, 1, 1, &message, &code);

	if (unlikely(DAO_GLOBAL(debug).enable_debug)) {
		DAO_CONCAT_SV(&debug_message, "--exception: ", message);
		DAO_DEBUG_LOG(&debug_message);
		zval_ptr_dtor(&debug_message);
	}

	if (!code) {
		code = &DAO_GLOBAL(z_zero);
	} else {
		DAO_SEPARATE_PARAM(code);
	}

	dao_read_property(&error_handlers, getThis(), SL("_errorHandlers"), PH_NOISY|PH_READONLY);

	if (Z_TYPE(error_handlers) == IS_ARRAY) {
		if (dao_array_isset_fetch(&error_handler, &error_handlers, code, PH_READONLY)) {
			DAO_CALL_SELF(NULL, "forward", &error_handler);
			dao_read_property(&previous_namespace_name, getThis(), SL("_previousNamespaceName"), PH_NOISY|PH_READONLY);
			dao_read_property(&previous_controller_name, getThis(), SL("_previousHandlerName"), PH_NOISY|PH_READONLY);
			dao_read_property(&previous_action_name, getThis(), SL("_previousActionName"), PH_NOISY|PH_READONLY);
			dao_read_property(&previous_params, getThis(), SL("_previousParams"), PH_NOISY|PH_READONLY);

			dao_read_property(&namespace_name, getThis(), SL("_namespaceName"), PH_NOISY|PH_READONLY);
			dao_read_property(&controller_name, getThis(), SL("_handlerName"), PH_NOISY|PH_READONLY);
			dao_read_property(&action_name, getThis(), SL("_actionName"), PH_NOISY|PH_READONLY);
			dao_read_property(&params, getThis(), SL("_params"), PH_NOISY|PH_READONLY);

			if (
				!DAO_IS_EQUAL(&previous_namespace_name, &namespace_name) ||
				!DAO_IS_EQUAL(&previous_controller_name, &controller_name) ||
				!DAO_IS_EQUAL(&previous_action_name, &action_name) ||
				!DAO_IS_EQUAL(&previous_params, &params)
			) {
				RETURN_MM_FALSE;
			}

		}
	}

	/**
	 * Create the real exception
	 */
	object_init_ex(&exception, dao_mvc_dispatcher_exception_ce);
	DAO_MM_ADD_ENTRY(&exception);
	DAO_MM_CALL_METHOD(NULL, &exception, "__construct", message, code);

	DAO_MM_ZVAL_STRING(&event_name, "dispatch:beforeException");
	DAO_MM_CALL_METHOD(&status, getThis(), "fireeventcancel", &event_name, &exception);

	if (DAO_IS_FALSE(&status)) {
		RETURN_MM_FALSE;
	}
	zval_ptr_dtor(&status);

	/**
	 * Throw the exception if it wasn't handled
	 */
	dao_throw_exception(&exception);
	RETURN_MM();
}

/**
 * Handles a user exception
 *
 * @param \Exception $exception
 *
 * @warning If any additional logic is to be implemented here, please check
 * dao_dispatcher_fire_event() first
 */
PHP_METHOD(Dao_Mvc_Dispatcher, _handleException)
{
	zval *exception, event_name = {};

	dao_fetch_params(1, 1, 0, &exception);

	DAO_MM_ZVAL_STRING(&event_name, "dispatch:beforeException");
	DAO_MM_CALL_METHOD(return_value, getThis(), "fireevent", &event_name, exception);

	RETURN_MM();
}

/**
 * Possible controller class name that will be located to dispatch the request
 *
 * @return string
 */
PHP_METHOD(Dao_Mvc_Dispatcher, getControllerClass)
{
	DAO_RETURN_CALL_METHOD(getThis(), "gethandlerclass");
}

/**
 * Returns the lastest dispatched controller
 *
 * @return Dao\Mvc\ControllerInterface
 */
PHP_METHOD(Dao_Mvc_Dispatcher, getLastController){


	RETURN_MEMBER(getThis(), "_lastHandler");
}

/**
 * Returns the active controller in the dispatcher
 *
 * @return Dao\Mvc\ControllerInterface
 */
PHP_METHOD(Dao_Mvc_Dispatcher, getActiveController){


	RETURN_MEMBER(getThis(), "_activeHandler");
}

/**
 * Returns the previous controller in the dispatcher
 *
 * @return string
 */
PHP_METHOD(Dao_Mvc_Dispatcher, getPreviousControllerName){


	RETURN_MEMBER(getThis(), "_previousHandlerName");
}

/**
 * Returns the previous action in the dispatcher
 *
 * @return string
 */
PHP_METHOD(Dao_Mvc_Dispatcher, getPreviousActionName){


	RETURN_MEMBER(getThis(), "_previousActionName");
}
