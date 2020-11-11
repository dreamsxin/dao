
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

#include "mvc/view/engine.h"
#include "mvc/view/engineinterface.h"
#include "mvc/view/exception.h"
#include "di/injectable.h"

#include <Zend/zend_closures.h>

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/array.h"
#include "kernel/concat.h"
#include "kernel/operators.h"
#include "kernel/exception.h"

#include "interned-strings.h"

/**
 * Dao\Mvc\View\Engine
 *
 * All the template engine adapters must inherit this class. This provides
 * basic interfacing between the engine and the Dao\Mvc\View component.
 */
zend_class_entry *dao_mvc_view_engine_ce;

PHP_METHOD(Dao_Mvc_View_Engine, __construct);
PHP_METHOD(Dao_Mvc_View_Engine, getContent);
PHP_METHOD(Dao_Mvc_View_Engine, startSection);
PHP_METHOD(Dao_Mvc_View_Engine, stopSection);
PHP_METHOD(Dao_Mvc_View_Engine, section);
PHP_METHOD(Dao_Mvc_View_Engine, partial);
PHP_METHOD(Dao_Mvc_View_Engine, getView);
PHP_METHOD(Dao_Mvc_View_Engine, addMethod);
PHP_METHOD(Dao_Mvc_View_Engine, __call);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_view_engine___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, view)
	ZEND_ARG_INFO(0, dependencyInjector)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_mvc_view_engine_method_entry[] = {
	PHP_ME(Dao_Mvc_View_Engine, __construct, arginfo_dao_mvc_view_engine___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Mvc_View_Engine, getContent, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View_Engine, startSection, arginfo_dao_mvc_view_engineinterface_startsection, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View_Engine, stopSection, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View_Engine, section, arginfo_dao_mvc_view_engineinterface_section, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View_Engine, partial, arginfo_dao_mvc_view_engineinterface_partial, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View_Engine, getView, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View_Engine, addMethod, arginfo_dao_mvc_view_engineinterface_addmethod, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View_Engine, __call, arginfo_dao_mvc_view_engineinterface___call, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Mvc\View\Engine initializer
 */
DAO_INIT_CLASS(Dao_Mvc_View_Engine){

	DAO_REGISTER_CLASS_EX(Dao\\Mvc\\View, Engine, mvc_view_engine, dao_di_injectable_ce, dao_mvc_view_engine_method_entry, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	zend_declare_property_null(dao_mvc_view_engine_ce, SL("_view"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_view_engine_ce, SL("_layout"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_view_engine_ce, SL("_params"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_view_engine_ce, SL("_methods"), ZEND_ACC_PROTECTED);

	zend_class_implements(dao_mvc_view_engine_ce, 1, dao_mvc_view_engineinterface_ce);

	return SUCCESS;
}

/**
 * Dao\Mvc\View\Engine constructor
 *
 * @param Dao\Mvc\ViewInterface $view
 * @param Dao\DiInterface $dependencyInjector
 */
PHP_METHOD(Dao_Mvc_View_Engine, __construct){

	zval *view, *dependency_injector = NULL;

	dao_fetch_params(0, 1, 1, &view, &dependency_injector);

	if (dependency_injector && Z_TYPE_P(dependency_injector) != IS_NULL) {
		DAO_CALL_METHOD(NULL, getThis(), "setdi", dependency_injector);
	}

	dao_update_property(getThis(), SL("_view"), view);
}

/**
 * Returns cached ouput on another view stage
 *
 * @return array
 */
PHP_METHOD(Dao_Mvc_View_Engine, getContent){

	zval view = {};
	dao_read_property(&view, getThis(), SL("_view"), PH_NOISY|PH_READONLY);
	DAO_RETURN_CALL_METHOD(&view, "getcontent");
}

/**
 * Start a new section block
 *
 * @param string $name
 */
PHP_METHOD(Dao_Mvc_View_Engine, startSection){

	zval *name, view = {};

	dao_fetch_params(0, 1, 0, &name);

	dao_read_property(&view, getThis(), SL("_view"), PH_NOISY|PH_READONLY);
	DAO_CALL_METHOD(NULL, &view, "startsection", name);
}

/**
 * Stop the current section block
 *
 * @return string
 */
PHP_METHOD(Dao_Mvc_View_Engine, stopSection){

	zval view = {};

	dao_read_property(&view, getThis(), SL("_view"), PH_NOISY|PH_READONLY);
	DAO_CALL_METHOD(NULL, &view, "stopsection");
}

/**
 * Returns the content for a section block
 *
 * @param string $name
 * @param string $default
 * @return string|null
 */
PHP_METHOD(Dao_Mvc_View_Engine, section){

	zval *name, *default_value = NULL, view = {};

	dao_fetch_params(0, 1, 1, &name, &default_value);

	if (!default_value) {
		default_value = &DAO_GLOBAL(z_null);
	}

	dao_read_property(&view, getThis(), SL("_view"), PH_NOISY|PH_READONLY);

	DAO_CALL_METHOD(return_value, &view, "section", name, default_value);
}

/**
 * Renders a partial inside another view
 *
 * @param string $partialPath
 * @param array $params
 * @return string
 */
PHP_METHOD(Dao_Mvc_View_Engine, partial){

	zval *partial_path, *params = NULL, view = {};

	dao_fetch_params(0, 1, 1, &partial_path, &params);

	if (!params) {
		params = &DAO_GLOBAL(z_null);
	}

	dao_read_property(&view, getThis(), SL("_view"), PH_NOISY|PH_READONLY);
	DAO_RETURN_CALL_METHOD(&view, "partial", partial_path, params);
}

/**
 * Returns the view component related to the adapter
 *
 * @return Dao\Mvc\ViewInterface
 */
PHP_METHOD(Dao_Mvc_View_Engine, getView){


	RETURN_MEMBER(getThis(), "_view");
}

/**
 * Adds a user-defined method
 *
 * @param string $name
 * @param closure $methodCallable
 * @return Dao\Mvc\View\Engine
 */
PHP_METHOD(Dao_Mvc_View_Engine, addMethod){

	zval *name, *method_callable, class_name = {}, method = {};

	dao_fetch_params(1, 2, 0, &name, &method_callable);
	DAO_ENSURE_IS_STRING(name);

	if (Z_TYPE_P(method_callable) != IS_OBJECT || !instanceof_function(Z_OBJCE_P(method_callable), zend_ce_closure)) {
		DAO_MM_THROW_EXCEPTION_STR(dao_mvc_view_exception_ce, "Method must be an closure object");
		return;
	}

	dao_get_class(&class_name, getThis(), 0);
	DAO_MM_ADD_ENTRY(&class_name);
	DAO_MM_CALL_CE_STATIC(&method, zend_ce_closure, "bind", method_callable, getThis(), &class_name);
	DAO_MM_ADD_ENTRY(&method);
	dao_update_property_array(getThis(), SL("_methods"), name, &method);
	RETURN_MM_THIS();
}

/**
 * Handles method calls when a method is not implemented
 *
 * @param string $method
 * @param array $arguments
 * @return mixed
 */
PHP_METHOD(Dao_Mvc_View_Engine, __call){

	zval *method, *args = NULL, method_name = {}, arguments = {}, methods = {}, func = {}, exception_message = {}, service_name = {}, service = {}, callback = {};

	dao_fetch_params(1, 1, 1, &method, &args);
	DAO_MM_ZVAL_DUP(&method_name, method);

	if (!args || Z_TYPE_P(args) == IS_NULL) {
		array_init(&arguments);
		DAO_MM_ADD_ENTRY(&arguments);
	} else {
		ZVAL_COPY_VALUE(&arguments, args);
	}

	dao_read_property(&methods, getThis(), SL("_methods"), PH_NOISY|PH_READONLY);
	if (dao_array_isset_fetch(&func, &methods, &method_name, PH_READONLY)) {
			DAO_MM_CALL_USER_FUNC_ARRAY(return_value, &func, &arguments);
			RETURN_MM();
	}

	if (dao_compare_strict_string(&method_name, SL("get"))
		|| dao_compare_strict_string(&method_name, SL("getPost"))
		|| dao_compare_strict_string(&method_name, SL("getPut"))
		|| dao_compare_strict_string(&method_name, SL("getQuery"))
		|| dao_compare_strict_string(&method_name, SL("getServer"))) {
		ZVAL_STR(&service_name, IS(request));
	} else if (dao_compare_strict_string(&method_name, SL("getSession"))) {
		DAO_MM_ZVAL_STRING(&method_name, "get");
		ZVAL_STR(&service_name, IS(session));
	} else if (dao_compare_strict_string(&method_name, SL("getParam"))) {
		ZVAL_STR(&service_name, IS(dispatcher));
	}

	DAO_MM_CALL_METHOD(&service, getThis(), "getresolveservice", &service_name);
	DAO_MM_ADD_ENTRY(&service);
	if (Z_TYPE(service) != IS_OBJECT) {
		DAO_CONCAT_SVS(&exception_message, "The injected service '", &service_name, "' is not valid");
		DAO_MM_ADD_ENTRY(&exception_message);
		DAO_MM_THROW_EXCEPTION_ZVAL(dao_mvc_view_exception_ce, &exception_message);
		return;
	}

	if (dao_method_exists(&service, &method_name) == FAILURE) {
		DAO_CONCAT_SVS(&exception_message, "The method \"", &method_name, "\" doesn't exist on view");
		DAO_MM_ADD_ENTRY(&exception_message);
		DAO_MM_THROW_EXCEPTION_ZVAL(dao_mvc_view_exception_ce, &exception_message);
		return;
	}

	array_init(&callback);
	DAO_MM_ADD_ENTRY(&callback);
	dao_array_append(&callback, &service, PH_COPY);
	dao_array_append(&callback, &method_name, PH_COPY);

	DAO_MM_CALL_USER_FUNC_ARRAY(return_value, &callback, &arguments);
	RETURN_MM();
}
