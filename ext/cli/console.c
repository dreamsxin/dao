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

#include "cli/console.h"
#include "cli/../application.h"
#include "cli/console/exception.h"
#include "cli/../routerinterface.h"
#include "diinterface.h"
#include "dispatcherinterface.h"
#include "di/injectable.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/exception.h"
#include "kernel/fcall.h"
#include "kernel/array.h"
#include "kernel/operators.h"
#include "kernel/concat.h"
#include "kernel/file.h"
#include "kernel/require.h"

#include "interned-strings.h"

/**
 * Dao\Cli\Console
 *
 * This component allows to create CLI applications using Dao
 */
zend_class_entry *dao_cli_console_ce;

PHP_METHOD(Dao_Cli_Console, __construct);
PHP_METHOD(Dao_Cli_Console, addModules);
PHP_METHOD(Dao_Cli_Console, getModules);
PHP_METHOD(Dao_Cli_Console, handle);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_cli_console___construct, 0, 0, 0)
	ZEND_ARG_INFO(0, dependencyInjector)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_cli_console_addmodules, 0, 0, 1)
	ZEND_ARG_INFO(0, modules)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_cli_console_method_entry[] = {
	PHP_ME(Dao_Cli_Console, __construct, arginfo_dao_cli_console___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Cli_Console, addModules, arginfo_dao_cli_console_addmodules, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cli_Console, handle, arginfo_dao_application_handle, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Cli\Console initializer
 */
DAO_INIT_CLASS(Dao_Cli_Console){

	DAO_REGISTER_CLASS_EX(Dao\\Cli, Console, cli_console, dao_application_ce, dao_cli_console_method_entry, 0);

	zend_declare_property_null(dao_cli_console_ce, SL("_modules"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_cli_console_ce, SL("_moduleObject"), ZEND_ACC_PROTECTED);

	return SUCCESS;
}

/**
 * Dao\Cli\Console constructor
 */
PHP_METHOD(Dao_Cli_Console, __construct){

	zval *dependency_injector = NULL;

	dao_fetch_params(0, 0, 1, &dependency_injector);

	if (dependency_injector && Z_TYPE_P(dependency_injector) != IS_NULL) {
		DAO_CALL_METHOD(NULL, getThis(), "setdi", dependency_injector);
	}
}

/**
 * Merge modules with the existing ones
 *
 *<code>
 *	$application->addModules(array(
 *		'admin' => array(
 *			'className' => 'Multiple\Admin\Module',
 *			'path' => '../apps/admin/Module.php'
 *		)
 *	));
 *</code>
 *
 * @param array $modules
 */
PHP_METHOD(Dao_Cli_Console, addModules){

	zval *modules;

	dao_fetch_params(0, 1, 0, &modules);

	DAO_CALL_METHOD(return_value, getThis(), "registermodules", modules, &DAO_GLOBAL(z_true));
}

/**
 * Handle the command-line arguments.
 *
 *
 * <code>
 * 	$arguments = array(
 * 		'task' => 'taskname',
 * 		'action' => 'action',
 * 		'params' => array('parameter1', 'parameter2')
 * 	);
 * 	$console->handle($arguments);
 * </code>
 *
 * @param array $arguments
 * @return mixed
 */
PHP_METHOD(Dao_Cli_Console, handle){

	zval *_arguments = NULL, arguments = {}, dependency_injector = {}, event_name = {}, service = {}, router = {}, module_name = {};
	zval status = {}, modules = {}, exception_msg = {}, module = {}, path = {}, class_name = {}, module_object = {};
	zval namespace_name = {}, task_name = {}, action_name = {}, params = {}, dispatcher = {};

	dao_fetch_params(1, 0, 1, &_arguments);

	if (!_arguments) {
		ZVAL_NULL(&arguments);
	} else {
		DAO_MM_ZVAL_COPY(&arguments, _arguments);
	}

	ZVAL_STR(&service, IS(router));

	DAO_MM_CALL_METHOD(&dependency_injector, getThis(), "getdi");
	DAO_MM_ADD_ENTRY(&dependency_injector);
	if (Z_TYPE(dependency_injector) != IS_OBJECT) {
		DAO_MM_THROW_EXCEPTION_FORMAT(dao_cli_console_exception_ce, "A dependency injection container is required to access the '%s' service", Z_STRVAL(service));
		return;
	}

	DAO_MM_CALL_METHOD(&router, &dependency_injector, "getshared", &service);
	DAO_MM_ADD_ENTRY(&router);
	DAO_MM_VERIFY_INTERFACE(&router, dao_routerinterface_ce);

	DAO_MM_CALL_METHOD(NULL, &router, "handle", &arguments);
	DAO_MM_CALL_METHOD(&module_name, &router, "getmodulename");
	DAO_MM_ADD_ENTRY(&module_name);
	if (zend_is_true(&module_name)) {
		DAO_MM_ZVAL_STRING(&event_name, "console:beforeStartModule");
		DAO_MM_CALL_METHOD(&status, getThis(), "fireeventcancel", &event_name, &module_name);
		if (DAO_IS_FALSE(&status)) {
			RETURN_MM_FALSE;
		}
		zval_ptr_dtor(&status);

		dao_read_property(&modules, getThis(), SL("_modules"), PH_NOISY|PH_READONLY);
		if (!dao_array_isset_fetch(&module, &modules, &module_name, PH_READONLY)) {
			DAO_CONCAT_SVS(&exception_msg, "Module '", &module_name, "' isn't registered in the console container");
			DAO_MM_ADD_ENTRY(&exception_msg);
			DAO_MM_THROW_EXCEPTION_ZVAL(dao_cli_console_exception_ce, &exception_msg);
			return;
		}

		if (Z_TYPE(module) != IS_ARRAY) {
			DAO_MM_THROW_EXCEPTION_STR(dao_cli_console_exception_ce, "Invalid module definition path");
			return;
		}

		if (dao_array_isset_fetch_str(&path, &module, SL("path"), PH_READONLY)) {
			convert_to_string_ex(&path);

			if (dao_file_exists(&path) == SUCCESS) {
				RETURN_MM_ON_FAILURE(dao_require(Z_STRVAL(path)));
			} else {
				DAO_MM_THROW_EXCEPTION_FORMAT(dao_cli_console_exception_ce, "Modules definition path '%s' does not exist", Z_STRVAL(path));
				return;
			}
		}

		if (!dao_array_isset_fetch_str(&class_name, &module, SL("className"), PH_COPY)) {
			DAO_MM_ZVAL_STRING(&class_name, "Module");
		}

		DAO_MM_CALL_METHOD(&module_object, &dependency_injector, "getshared", &class_name);
		DAO_MM_ADD_ENTRY(&module_object);
		DAO_MM_CALL_METHOD(NULL, &module_object, "registerautoloaders");
		DAO_MM_CALL_METHOD(NULL, &module_object, "registerservices", &dependency_injector);
		dao_update_property(getThis(), SL("_moduleObject"), &module_object);

		DAO_MM_ZVAL_STRING(&event_name, "console:afterStartModule");
		DAO_MM_CALL_METHOD(&status, getThis(), "fireeventcancel", &event_name, &module_name);
		if (DAO_IS_FALSE(&status)) {
			RETURN_MM_FALSE;
		}
		zval_ptr_dtor(&status);
	}

	DAO_MM_CALL_METHOD(&namespace_name, &router, "getnamespacename");
	DAO_MM_ADD_ENTRY(&namespace_name);
	DAO_MM_CALL_METHOD(&task_name, &router, "gethandlername");
	DAO_MM_ADD_ENTRY(&task_name);
	DAO_MM_CALL_METHOD(&action_name, &router, "getactionname");
	DAO_MM_ADD_ENTRY(&action_name);
	DAO_MM_CALL_METHOD(&params, &router, "getparams");
	DAO_MM_ADD_ENTRY(&params);

	ZVAL_STR(&service, IS(dispatcher));

	DAO_MM_CALL_METHOD(&dispatcher, &dependency_injector, "getshared", &service);
	DAO_MM_ADD_ENTRY(&dispatcher);
	DAO_MM_VERIFY_INTERFACE(&dispatcher, dao_dispatcherinterface_ce);

	DAO_MM_CALL_METHOD(NULL, &dispatcher, "setnamespacename", &namespace_name);
	DAO_MM_CALL_METHOD(NULL, &dispatcher, "sethandlername", &task_name);
	DAO_MM_CALL_METHOD(NULL, &dispatcher, "setactionname", &action_name);
	DAO_MM_CALL_METHOD(NULL, &dispatcher, "setparams", &params);

	DAO_MM_ZVAL_STRING(&event_name, "console:beforeHandleTask");
	DAO_MM_CALL_METHOD(&status, getThis(), "fireeventcancel", &event_name, &dispatcher);

	if (DAO_IS_FALSE(&status)) {
		RETURN_MM_FALSE;
	}
	zval_ptr_dtor(&status);

	DAO_MM_CALL_METHOD(&status, &dispatcher, "dispatch");

	DAO_MM_ZVAL_STRING(&event_name, "console:afterHandleTask");
	DAO_MM_CALL_METHOD(NULL, getThis(), "fireevent", &event_name, &status);

	RETURN_MM_NCTOR(&status);
}
