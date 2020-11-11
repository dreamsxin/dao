
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
  +------------------------------------------------------------------------+
*/

#include "cli/dispatcher.h"
#include "cli/dispatcher/exception.h"
#include "cli/../dispatcher.h"
#include "dispatcherinterface.h"

#include "kernel/main.h"
#include "kernel/array.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "kernel/exception.h"

/**
 * Dao\Cli\Dispatcher
 *
 * Dispatching is the process of taking the command-line arguments, extracting the module name,
 * task name, action name, and optional parameters contained in it, and then
 * instantiating a task and calling an action on it.
 *
 *<code>
 *
 *	$di = new Dao\Di();
 *
 *	$dispatcher = new Dao\Cli\Dispatcher();
 *
 *  $dispatcher->setDI($di);
 *
 *	$dispatcher->setTaskName('posts');
 *	$dispatcher->setActionName('index');
 *	$dispatcher->setParams(array());
 *
 *	$handle = $dispatcher->dispatch();
 *
 *</code>
 */
zend_class_entry *dao_cli_dispatcher_ce;

PHP_METHOD(Dao_Cli_Dispatcher, setTaskSuffix);
PHP_METHOD(Dao_Cli_Dispatcher, getTaskSuffix);
PHP_METHOD(Dao_Cli_Dispatcher, setDefaultTask);
PHP_METHOD(Dao_Cli_Dispatcher, setTaskName);
PHP_METHOD(Dao_Cli_Dispatcher, getTaskName);
PHP_METHOD(Dao_Cli_Dispatcher, _throwDispatchException);
PHP_METHOD(Dao_Cli_Dispatcher, _handleException);
PHP_METHOD(Dao_Cli_Dispatcher, getTaskClass);
PHP_METHOD(Dao_Cli_Dispatcher, getLastTask);
PHP_METHOD(Dao_Cli_Dispatcher, getActiveTask);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_cli_dispatcher_settasksuffix, 0, 0, 1)
	ZEND_ARG_INFO(0, taskSuffix)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_cli_dispatcher_setdefaulttask, 0, 0, 1)
	ZEND_ARG_INFO(0, taskName)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_cli_dispatcher_settaskname, 0, 0, 1)
	ZEND_ARG_INFO(0, taskName)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_cli_dispatcher_method_entry[] = {
	PHP_ME(Dao_Cli_Dispatcher, setTaskSuffix, arginfo_dao_cli_dispatcher_settasksuffix, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cli_Dispatcher, getTaskSuffix, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cli_Dispatcher, setDefaultTask, arginfo_dao_cli_dispatcher_setdefaulttask, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cli_Dispatcher, setTaskName, arginfo_dao_cli_dispatcher_settaskname, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cli_Dispatcher, getTaskName, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cli_Dispatcher, _throwDispatchException, arginfo_empty, ZEND_ACC_PROTECTED)
	PHP_ME(Dao_Cli_Dispatcher, _handleException, arginfo_empty, ZEND_ACC_PROTECTED)
	PHP_ME(Dao_Cli_Dispatcher, getTaskClass, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cli_Dispatcher, getLastTask, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cli_Dispatcher, getActiveTask, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Cli\Dispatcher initializer
 */
DAO_INIT_CLASS(Dao_Cli_Dispatcher){

	DAO_REGISTER_CLASS_EX(Dao\\Cli, Dispatcher, cli_dispatcher, dao_dispatcher_ce, dao_cli_dispatcher_method_entry, 0);

	zend_declare_property_string(dao_cli_dispatcher_ce, SL("_handlerSuffix"), "Task", ZEND_ACC_PROTECTED);
	zend_declare_property_string(dao_cli_dispatcher_ce, SL("_defaultHandler"), "main", ZEND_ACC_PROTECTED);
	zend_declare_property_string(dao_cli_dispatcher_ce, SL("_defaultAction"), "main", ZEND_ACC_PROTECTED);

	zend_class_implements(dao_cli_dispatcher_ce, 1, dao_dispatcherinterface_ce);

	return SUCCESS;
}

/**
 * Sets the default task suffix
 *
 * @param string $taskSuffix
 */
PHP_METHOD(Dao_Cli_Dispatcher, setTaskSuffix){

	zval *task_suffix;

	dao_fetch_params(0, 1, 0, &task_suffix);

	dao_update_property(getThis(), SL("_handlerSuffix"), task_suffix);

}

/**
 * Gets the default task suffix
 *
 * @return string
 */
PHP_METHOD(Dao_Cli_Dispatcher, getTaskSuffix){


	RETURN_MEMBER(getThis(), "_handlerSuffix");
}

/**
 * Sets the default task name
 *
 * @param string $taskName
 */
PHP_METHOD(Dao_Cli_Dispatcher, setDefaultTask){

	zval *task_name;

	dao_fetch_params(0, 1, 0, &task_name);

	dao_update_property(getThis(), SL("_defaultHandler"), task_name);

}

/**
 * Sets the task name to be dispatched
 *
 * @param string $taskName
 */
PHP_METHOD(Dao_Cli_Dispatcher, setTaskName){

	zval *task_name;

	dao_fetch_params(0, 1, 0, &task_name);

	dao_update_property(getThis(), SL("_handlerName"), task_name);

}

/**
 * Gets last dispatched task name
 *
 * @return string
 */
PHP_METHOD(Dao_Cli_Dispatcher, getTaskName){


	RETURN_MEMBER(getThis(), "_handlerName");
}

/**
 * Throws an internal exception
 *
 * @param string $message
 * @param int $exceptionCode
 */
PHP_METHOD(Dao_Cli_Dispatcher, _throwDispatchException){

	zval *message, *exception_code = NULL, exception = {}, event_name = {}, status = {};

	dao_fetch_params(1, 1, 1, &message, &exception_code);

	if (!exception_code) {
		exception_code = &DAO_GLOBAL(z_zero);
	}

	object_init_ex(&exception, dao_cli_dispatcher_exception_ce);
	DAO_MM_ADD_ENTRY(&exception);
	DAO_MM_CALL_METHOD(NULL, &exception, "__construct", message, exception_code);

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
 */
PHP_METHOD(Dao_Cli_Dispatcher, _handleException){

	zval *exception, event_name = {}, status = {};

	dao_fetch_params(0, 1, 0, &exception);

	ZVAL_STRING(&event_name, "dispatch:beforeException");
	DAO_CALL_METHOD(&status, getThis(), "fireeventcancel", &event_name, exception);
	zval_ptr_dtor(&event_name);
	if (DAO_IS_FALSE(&status)) {
		RETURN_FALSE;
	}
	zval_ptr_dtor(&status);
}

/**
 * Possible task class name that will be located to dispatch the request
 *
 * @return string
 */
PHP_METHOD(Dao_Cli_Dispatcher, getTaskClass){


	DAO_RETURN_CALL_METHOD(getThis(), "gethandlerclass");
}

/**
 * Returns the lastest dispatched controller
 *
 * @return Dao\Cli\Task
 */
PHP_METHOD(Dao_Cli_Dispatcher, getLastTask){


	RETURN_MEMBER(getThis(), "_lastHandler");
}

/**
 * Returns the active task in the dispatcher
 *
 * @return Dao\Cli\Task
 */
PHP_METHOD(Dao_Cli_Dispatcher, getActiveTask){


	RETURN_MEMBER(getThis(), "_activeHandler");
}
