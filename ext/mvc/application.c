
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

#include "mvc/application.h"
#include "mvc/../application.h"
#include "mvc/application/exception.h"
#include "mvc/dispatcherinterface.h"
#include "mvc/../dispatcherinterface.h"
#include "mvc/routerinterface.h"
#include "mvc/moduledefinitioninterface.h"
#include "mvc/viewinterface.h"
#include "mvc/view/modelinterface.h"
#include "di/injectable.h"
#include "diinterface.h"
#include "di.h"
#include "events/managerinterface.h"
#include "http/responseinterface.h"
#include "http/request.h"

#include <Zend/zend_closures.h>

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/exception.h"
#include "kernel/operators.h"
#include "kernel/fcall.h"
#include "kernel/array.h"
#include "kernel/concat.h"
#include "kernel/file.h"
#include "kernel/require.h"
#include "kernel/string.h"
#include "kernel/debug.h"

#include "interned-strings.h"

/**
 * Dao\Mvc\Application
 *
 * This component encapsulates all the complex operations behind instantiating every component
 * needed and integrating it with the rest to allow the MVC pattern to operate as desired.
 *
 *<code>
 *
 * class Application extends \Dao\Mvc\Application
 * {
 *
 *		/\**
 *		 * Register the services here to make them general or register
 *		 * in the ModuleDefinition to make them module-specific
 *		 *\/
 *		protected function _registerServices()
 *		{
 *
 *		}
 *
 *		/\**
 *		 * This method registers all the modules in the application
 *		 *\/
 *		public function main()
 *		{
 *			$this->registerModules(array(
 *				'frontend' => array(
 *					'className' => 'Multiple\Frontend\Module',
 *					'path' => '../apps/frontend/Module.php'
 *				),
 *				'backend' => array(
 *					'className' => 'Multiple\Backend\Module',
 *					'path' => '../apps/backend/Module.php'
 *				)
 *			));
 *		}
 *	}
 *
 *	$application = new Application();
 *	$application->main();
 *
 *</code>
 */
zend_class_entry *dao_mvc_application_ce;

PHP_METHOD(Dao_Mvc_Application, __construct);
PHP_METHOD(Dao_Mvc_Application, useImplicitView);
PHP_METHOD(Dao_Mvc_Application, autoSendHeader);
PHP_METHOD(Dao_Mvc_Application, handle);
PHP_METHOD(Dao_Mvc_Application, request);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_application___construct, 0, 0, 0)
	ZEND_ARG_OBJ_INFO(0, dependencyInjector, Dao\\DiInterface, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_application_useimplicitview, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, implicitView, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_application_autosendheader, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, autoSendHeader, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_application_request, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, uri, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_ARRAY, 1)
	ZEND_ARG_OBJ_INFO(0, dependencyInjector, Dao\\DiInterface, 1)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_mvc_application_method_entry[] = {
	PHP_ME(Dao_Mvc_Application, __construct, arginfo_dao_mvc_application___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Mvc_Application, useImplicitView, arginfo_dao_mvc_application_useimplicitview, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Application, autoSendHeader, arginfo_dao_mvc_application_autosendheader, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Application, handle, arginfo_dao_application_handle, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Application, request, arginfo_dao_mvc_application_request, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Mvc\Application initializer
 */
DAO_INIT_CLASS(Dao_Mvc_Application){

	DAO_REGISTER_CLASS_EX(Dao\\Mvc, Application, mvc_application, dao_application_ce, dao_mvc_application_method_entry, 0);

	zend_declare_property_bool(dao_mvc_application_ce, SL("_implicitView"), 1, ZEND_ACC_PROTECTED);
	zend_declare_property_bool(dao_mvc_application_ce, SL("_autoSendHeader"), 1, ZEND_ACC_PROTECTED);

	return SUCCESS;
}

/**
 * Dao\Mvc\Application
 *
 * @param Dao\Di $dependencyInjector
 */
PHP_METHOD(Dao_Mvc_Application, __construct){

	zval *dependency_injector = NULL;

	dao_fetch_params(0, 0, 1, &dependency_injector);

	if (dependency_injector && Z_TYPE_P(dependency_injector) != IS_NULL) {
		DAO_CALL_METHOD(NULL, getThis(), "setdi", dependency_injector);
	}
}

/**
 * By default. The view is implicitly buffering all the output
 * You can full disable the view component using this method
 *
 * @param boolean $implicitView
 * @return Dao\Mvc\Application
 */
PHP_METHOD(Dao_Mvc_Application, useImplicitView){

	zval *implicit_view;

	dao_fetch_params(0, 1, 0, &implicit_view);

	dao_update_property(getThis(), SL("_implicitView"), implicit_view);
	RETURN_THIS();
}

/**
 * Enable or disable sending cookies by each request
 *
 * @param boolean $autoSendHeader
 * @return Dao\Mvc\Application
 */
PHP_METHOD(Dao_Mvc_Application, autoSendHeader){

	zval *auto_send;

	dao_fetch_params(0, 1, 0, &auto_send);

	dao_update_property(getThis(), SL("_autoSendHeader"), auto_send);
	RETURN_THIS();
}

/**
 * Handles a MVC request
 *
 * @param string $uri
 * @return Dao\Http\ResponseInterface
 */
PHP_METHOD(Dao_Mvc_Application, handle){

	zval *uri = NULL, dependency_injector = {}, event_name = {}, status = {}, service = {}, router = {}, module_name = {};
	zval modules = {}, module = {}, module_namespace = {}, module_class = {}, class_name = {}, path = {}, module_object = {}, module_params = {};
	zval implicit_view = {}, view = {}, namespace_name = {}, controller_name = {}, action_name = {}, params = {}, exact = {};
	zval dispatcher = {}, controller = {}, possible_response = {}, returned_response = {}, response = {}, content = {}, auto_send = {};
	int f_implicit_view;

	dao_fetch_params(1, 0, 1, &uri);

	if (!uri) {
		uri = &DAO_GLOBAL(z_null);
	}

	/* Call boot event, this allows the developer to perform initialization actions */
	DAO_MM_ZVAL_STRING(&event_name, "application:boot");
	DAO_MM_CALL_METHOD(&status, getThis(), "fireeventcancel", &event_name);
	if (DAO_IS_FALSE(&status)) {
		RETURN_MM_FALSE;
	}
	zval_ptr_dtor(&status);

	DAO_MM_CALL_METHOD(&dependency_injector, getThis(), "getdi");
	DAO_MM_ADD_ENTRY(&dependency_injector);
	if (Z_TYPE(dependency_injector) != IS_OBJECT) {
		DAO_MM_THROW_EXCEPTION_STR(dao_mvc_application_exception_ce, "A dependency injection object is required to access internal services");
		return;
	}

	ZVAL_STR(&service, IS(app));
	DAO_MM_CALL_METHOD(NULL, &dependency_injector, "setshared", &service, getThis());

	ZVAL_STR(&service, IS(router));
	DAO_MM_CALL_METHOD(&router, &dependency_injector, "getshared", &service);
	DAO_MM_ADD_ENTRY(&router);
	DAO_MM_VERIFY_INTERFACE(&router, dao_mvc_routerinterface_ce);

	DAO_MM_ZVAL_STRING(&event_name, "application:beforeHandleRouter");
	DAO_MM_CALL_METHOD(&status, getThis(), "fireeventcancel", &event_name, &router);
	if (DAO_IS_FALSE(&status)) {
		RETURN_MM_FALSE;
	}
	zval_ptr_dtor(&status);

	/* Handle the URI pattern (if any) */
	DAO_MM_CALL_METHOD(NULL, &router, "handle", uri);

	DAO_MM_ZVAL_STRING(&event_name, "application:afterHandleRouter");
	DAO_MM_CALL_METHOD(&status, getThis(), "fireeventcancel", &event_name, &router);
	if (DAO_IS_FALSE(&status)) {
		RETURN_MM_FALSE;
	}
	zval_ptr_dtor(&status);

	/* Load module config */
	DAO_MM_CALL_METHOD(&module_name, &router, "getmodulename");
	DAO_MM_ADD_ENTRY(&module_name);

	/* If the router doesn't return a valid module we use the default module */
	if (!zend_is_true(&module_name)) {
		 dao_read_property(&module_name, getThis(), SL("_defaultModule"), PH_READONLY);
	}

	/**
	 * Process the module definition
	 */
	if (zend_is_true(&module_name)) {
		DAO_MM_ZVAL_STRING(&event_name, "application:beforeStartModule");
		DAO_MM_CALL_METHOD(&status, getThis(), "fireeventcancel", &event_name, &module_name);

		if (DAO_IS_FALSE(&status)) {
			RETURN_MM_FALSE;
		}
		zval_ptr_dtor(&status);

		/**
		 * Check if the module passed by the router is registered in the modules container
		 */
		dao_read_property(&modules, getThis(), SL("_modules"), PH_NOISY|PH_READONLY);
		if (!dao_array_isset_fetch(&module, &modules, &module_name, PH_READONLY)) {
			convert_to_string(&module_name);
			zend_throw_exception_ex(dao_mvc_application_exception_ce, 0, "Module %s is not registered in the application container", Z_STRVAL(module_name));
			RETURN_MM();
		}

		/**
		 * A module definition must be an array or an object
		 */
		if (Z_TYPE(module) != IS_ARRAY && Z_TYPE(module) != IS_OBJECT) {
			DAO_MM_THROW_EXCEPTION_STR(dao_mvc_application_exception_ce, "Invalid module definition");
			return;
		}

		/* An array module definition contains a path to a module definition class */
		if (unlikely(Z_TYPE(module) == IS_ARRAY)) {
			/* Class name used to load the module definition */
			if (!dao_array_isset_fetch_str(&module_namespace, &module, SL("namespaceName"), PH_READONLY)) {
				DAO_MM_ZVAL_EMPTY_STRING(&module_namespace);
			}

			/* Class name used to load the module definition */
			if (!dao_array_isset_fetch_str(&module_class, &module, SL("className"), PH_READONLY)) {
				DAO_MM_ZVAL_STRING(&module_class, "Module");
			}

			if (Z_TYPE(module_class) == IS_STRING && zend_is_true(&module_namespace) && !dao_memnstr_str(&module_class, SL("\\"))) {
				if (dao_end_with_str(&module_namespace, SL("\\"))) {
					DAO_CONCAT_VV(&class_name, &module_namespace, &module_class);
				} else {
					DAO_CONCAT_VSV(&class_name, &module_namespace, "\\", &module_class);
				}
				DAO_MM_ADD_ENTRY(&class_name);
			} else {
				ZVAL_COPY_VALUE(&class_name, &module_class);
			}

			/* If the developer has specified a path, try to include the file */
			if (dao_array_isset_fetch_str(&path, &module, SL("path"), PH_READONLY)) {
				convert_to_string_ex(&path);
				if (Z_TYPE(class_name) != IS_STRING || dao_class_exists(&class_name, 0) == NULL) {
					if (dao_file_exists(&path) == SUCCESS) {
						RETURN_MM_ON_FAILURE(dao_require(Z_STRVAL(path)));
					} else {
						zend_throw_exception_ex(dao_mvc_application_exception_ce, 0, "Module definition path '%s' does not exist", Z_STRVAL(path));
						RETURN_MM();
					}
				}
			}

			DAO_MM_CALL_METHOD(&module_object, &dependency_injector, "get", &class_name);
			DAO_MM_ADD_ENTRY(&module_object);
			if (instanceof_function(Z_OBJCE(module_object), zend_ce_closure)) {
				/* A module definition object, can be a Closure instance */
				array_init_size(&module_params, 1);
				dao_array_append(&module_params, &dependency_injector, PH_COPY);
				DAO_MM_ADD_ENTRY(&module_params);
				DAO_MM_CALL_USER_FUNC_ARRAY(NULL, &module_object, &module_params);
			} else if (instanceof_function(Z_OBJCE(module_object), dao_mvc_moduledefinitioninterface_ce)) {
				/**
				 * 'registerAutoloaders' and 'registerServices' are automatically called
				 */
				DAO_MM_CALL_METHOD(NULL, &module_object, "registerautoloaders", &dependency_injector);
				DAO_MM_CALL_METHOD(NULL, &module_object, "registerservices", &dependency_injector);
			} else {
				DAO_MM_THROW_EXCEPTION_STR(dao_mvc_application_exception_ce, "Invalid module definition");
				return;
			}
		} else if (Z_TYPE(module) == IS_OBJECT) {
			if (instanceof_function(Z_OBJCE(module), zend_ce_closure)) {
				/* A module definition object, can be a Closure instance */
				array_init_size(&module_params, 1);
				dao_array_append(&module_params, &dependency_injector, PH_COPY);
				DAO_MM_ADD_ENTRY(&module_params);
				DAO_MM_CALL_USER_FUNC_ARRAY(&status, &module, &module_params);
			} else if (instanceof_function(Z_OBJCE(module), dao_mvc_moduledefinitioninterface_ce)) {
				DAO_MM_CALL_METHOD(NULL, &module, "registerautoloaders", &dependency_injector);
				DAO_MM_CALL_METHOD(NULL, &module, "registerservices", &dependency_injector);
			}
		} else {
			DAO_MM_THROW_EXCEPTION_STR(dao_mvc_application_exception_ce, "Invalid module definition");
			return;
		}

		/* Calling afterStartModule event */
		DAO_MM_ZVAL_STRING(&event_name, "application:afterStartModule");
		DAO_MM_CALL_METHOD(&status, getThis(), "fireeventcancel", &event_name, &module_name);

		if (DAO_IS_FALSE(&status)) {
			RETURN_MM_FALSE;
		}
		zval_ptr_dtor(&status);
	}

	/**
	 * Check whether use implicit views or not
	 */
	DAO_MM_ZVAL_STRING(&event_name, "application:beforeCheckUseImplicitView");
	DAO_MM_CALL_METHOD(NULL, getThis(), "fireevent", &event_name);

	dao_read_property(&implicit_view, getThis(), SL("_implicitView"), PH_NOISY|PH_READONLY);

	/*
	 * The safe way is to use a flag because it *might* be possible to alter the value
	 * of _implicitView later which might result in crashes because 'view'
	 * is initialized only when _implicitView evaluates to false
	 */
	f_implicit_view = DAO_IS_TRUE(&implicit_view);

	ZVAL_STR(&service, IS(view));

	DAO_MM_CALL_METHOD(&view, &dependency_injector, "getshared", &service, &DAO_GLOBAL(z_null), &DAO_GLOBAL(z_true));
	if (Z_TYPE(view) > IS_NULL) {
		DAO_MM_ADD_ENTRY(&view);
		DAO_MM_VERIFY_INTERFACE(&view, dao_mvc_viewinterface_ce);
	}

	if (f_implicit_view && Z_TYPE(view) == IS_OBJECT) {

		/**
		 * Start the view component (start output buffering)
		 */
		DAO_MM_CALL_METHOD(NULL, &view, "start");
	}

	DAO_MM_ZVAL_STRING(&event_name, "application:afterCheckUseImplicitView");
	DAO_MM_CALL_METHOD(NULL, getThis(), "fireevent", &event_name);

	/* We get the parameters from the router and assign them to the dispatcher */
	DAO_MM_CALL_METHOD(&module_name, &router, "getmodulename");
	DAO_MM_ADD_ENTRY(&module_name);
	DAO_MM_CALL_METHOD(&namespace_name, &router, "getnamespacename");
	DAO_MM_ADD_ENTRY(&namespace_name);
	DAO_MM_CALL_METHOD(&controller_name, &router, "getcontrollername");
	DAO_MM_ADD_ENTRY(&controller_name);
	DAO_MM_CALL_METHOD(&action_name, &router, "getactionname");
	DAO_MM_ADD_ENTRY(&action_name);
	DAO_MM_CALL_METHOD(&params, &router, "getparams");
	DAO_MM_ADD_ENTRY(&params);
	DAO_MM_CALL_METHOD(&exact, &router, "isexactcontrollername");
	DAO_MM_ADD_ENTRY(&exact);\

	if (!zend_is_true(&namespace_name) && zend_is_true(&module_namespace)) {
		ZVAL_COPY_VALUE(&namespace_name, &module_namespace);
	}

	ZVAL_STR(&service, IS(dispatcher));

	DAO_MM_CALL_METHOD(&dispatcher, &dependency_injector, "getshared", &service);
	DAO_MM_ADD_ENTRY(&dispatcher);
	DAO_MM_VERIFY_INTERFACE(&dispatcher, dao_mvc_dispatcherinterface_ce);

	/* Assign the values passed from the router */
	DAO_MM_CALL_METHOD(NULL, &dispatcher, "setmodulename", &module_name);
	DAO_MM_CALL_METHOD(NULL, &dispatcher, "setnamespacename", &namespace_name);
	DAO_MM_CALL_METHOD(NULL, &dispatcher, "setcontrollername", &controller_name, &exact);
	DAO_MM_CALL_METHOD(NULL, &dispatcher, "setactionname", &action_name);
	DAO_MM_CALL_METHOD(NULL, &dispatcher, "setparams", &params);

	/* Calling beforeHandleRequest */
	DAO_MM_ZVAL_STRING(&event_name, "application:beforeHandleRequest");
	DAO_MM_CALL_METHOD(&status, getThis(), "fireeventcancel", &event_name, &dispatcher);

	if (DAO_IS_FALSE(&status)) {
		RETURN_MM_FALSE;
	}
	zval_ptr_dtor(&status);

	/* The dispatcher must return an object */
	DAO_MM_CALL_METHOD(&controller, &dispatcher, "dispatch");
	DAO_MM_ADD_ENTRY(&controller);

	/* Calling afterHandleRequest */
	DAO_MM_ZVAL_STRING(&event_name, "application:afterHandleRequest");
	DAO_MM_CALL_METHOD(&status, getThis(), "fireeventcancel", &event_name, &controller);

	if (DAO_IS_FALSE(&status)) {
		RETURN_MM_FALSE;
	}
	zval_ptr_dtor(&status);

	/* Get the latest value returned by an action */
	DAO_MM_CALL_METHOD(&possible_response, &dispatcher, "getreturnedvalue");
	DAO_MM_ADD_ENTRY(&possible_response);

	ZVAL_TRUE(&returned_response);

	/* Check if the returned object is already a response */
	if (Z_TYPE(possible_response) == IS_OBJECT && instanceof_function_ex(Z_OBJCE(possible_response), dao_http_responseinterface_ce, 1)) {
		ZVAL_COPY_VALUE(&response, &possible_response);
	} else {
		ZVAL_STR(&service, IS(response));

		DAO_MM_CALL_METHOD(&response, &dependency_injector, "getshared", &service);
		DAO_MM_ADD_ENTRY(&response);
		DAO_MM_VERIFY_INTERFACE(&response, dao_http_responseinterface_ce);

		if (DAO_IS_FALSE(&possible_response)) {
			RETURN_MM_CTOR(&response);
		} else if (Z_TYPE(possible_response) == IS_STRING) {
			DAO_MM_CALL_METHOD(NULL, &response, "setcontent", &possible_response);
			RETURN_MM_CTOR(&response);
		} else if (!f_implicit_view && Z_TYPE(possible_response) == IS_ARRAY) {
			DAO_MM_CALL_METHOD(NULL, &response, "setjsoncontent", &possible_response);
			RETURN_MM_CTOR(&response);
		}
		if (Z_TYPE(view) == IS_OBJECT) {
			ZVAL_FALSE(&returned_response);
		}
	}

	if (DAO_IS_FALSE(&returned_response)) {
		/**
		 * This allows to make a custom view render
		 */
		DAO_MM_ZVAL_STRING(&event_name, "application:beforeRenderView");
		DAO_MM_CALL_METHOD(&status, getThis(), "fireeventcancel", &event_name, &view);

		if (Z_TYPE(controller) == IS_OBJECT && unlikely(dao_method_exists_ex(&controller, SL("beforerenderview")) == SUCCESS)) {
			if (likely(DAO_IS_NOT_FALSE(&status))) {
				zval_ptr_dtor(&status);
				DAO_MM_CALL_METHOD(&status, &controller, "beforerenderview", &view);
			} else {
				DAO_MM_CALL_METHOD(NULL, &controller, "beforerenderview", &view);
			}
		}

		/* Check if the view process has been treated by the developer */
		if (DAO_IS_NOT_FALSE(&status)) {
			zval_ptr_dtor(&status);
			DAO_MM_CALL_METHOD(&namespace_name, &dispatcher, "getnamespacename");
			DAO_MM_ADD_ENTRY(&namespace_name);
			DAO_MM_CALL_METHOD(&controller_name, &dispatcher, "getcontrollername");
			DAO_MM_ADD_ENTRY(&controller_name);
			DAO_MM_CALL_METHOD(&action_name, &dispatcher, "getactionname");
			DAO_MM_ADD_ENTRY(&action_name);
			DAO_MM_CALL_METHOD(&params, &dispatcher, "getparams");
			DAO_MM_ADD_ENTRY(&params);

			/* Automatic render based on the latest controller executed */
			if (Z_TYPE(possible_response) == IS_OBJECT && instanceof_function_ex(Z_OBJCE(possible_response), dao_mvc_view_modelinterface_ce, 1)) {
				DAO_MM_CALL_METHOD(NULL, &view, "render", &controller_name, &action_name, &params, &namespace_name, &possible_response);
			} else {
				if (Z_TYPE(possible_response) == IS_ARRAY) {
					DAO_MM_CALL_METHOD(NULL, &view, "setvars", &possible_response, &DAO_GLOBAL(z_true));
				}
				/* Automatic render based on the latest controller executed */
				DAO_MM_CALL_METHOD(NULL, &view, "render", &controller_name, &action_name, &params, &namespace_name);
			}
		}
	} 

	if (f_implicit_view && Z_TYPE(view) == IS_OBJECT) {
		DAO_MM_CALL_METHOD(NULL, &view, "finish");
	}

	if (DAO_IS_FALSE(&returned_response)) {
		DAO_MM_ZVAL_STRING(&event_name, "application:afterRenderView");
		DAO_MM_CALL_METHOD(NULL, getThis(), "fireevent", &event_name, &view);

		if (Z_TYPE(controller) == IS_OBJECT && unlikely(dao_method_exists_ex(&controller, SL("afterrenderview")) == SUCCESS)) {
			DAO_MM_CALL_METHOD(NULL, &controller, "afterrenderview", &view);
		}
		/* The content returned by the view is passed to the response service */
		DAO_MM_CALL_METHOD(&content, &view, "getcontent");
		DAO_MM_ADD_ENTRY(&content);
		DAO_MM_CALL_METHOD(NULL, &response, "setcontent", &content);
	}

	/* Calling beforeSendResponse */
	DAO_MM_ZVAL_STRING(&event_name, "application:beforeSendResponse");
	DAO_MM_CALL_METHOD(NULL, getThis(), "fireevent", &event_name, &response);

	if (Z_TYPE(controller) == IS_OBJECT && unlikely(dao_method_exists_ex(&controller, SL("beforesendresponse")) == SUCCESS)) {
		DAO_MM_CALL_METHOD(NULL, &controller, "beforesendresponse", &response);
	}

	dao_read_property(&auto_send, getThis(), SL("_autoSendHeader"), PH_NOISY|PH_READONLY);

	if (likely(zend_is_true(&auto_send))) {
		/* Headers & Cookies are automatically sent */
		DAO_MM_CALL_METHOD(NULL, &response, "sendheaders");
		DAO_MM_CALL_METHOD(NULL, &response, "sendcookies");
	}

	DAO_MM_ZVAL_STRING(&event_name, "application:afterSendResponse");
	DAO_MM_CALL_METHOD(NULL, getThis(), "fireevent", &event_name, &response);

	if (Z_TYPE(controller) == IS_OBJECT && unlikely(dao_method_exists_ex(&controller, SL("aftersendresponse")) == SUCCESS)) {
		DAO_MM_CALL_METHOD(NULL, &controller, "aftersendresponse", &response);
	}

	/* Return the response */
	RETURN_MM_CTOR(&response);
}

/**
 * Does a HMVC request in the application
 *
 * @param array $location
 * @param array $data
 * @return mixed
 */
PHP_METHOD(Dao_Mvc_Application, request){

	zval *uri, *data = NULL, *_dependency_injector = NULL, dependency_injector_new = {}, service = {}, definition = {};
	zval app = {}, response = {}, requset = {};

	dao_fetch_params(1, 1, 2, &uri, &data, &_dependency_injector);

	if (!data) {
		data = &DAO_GLOBAL(z_null);
	}


	if (!_dependency_injector || Z_TYPE_P(_dependency_injector) == IS_NULL) {
		zval dependency_injector = {}, services = {}, *value;
		zend_string *key;
		ulong idx;
		DAO_MM_CALL_METHOD(&dependency_injector, getThis(), "getdi");
		DAO_MM_ADD_ENTRY(&dependency_injector);
		DAO_MM_CALL_METHOD(&services, &dependency_injector, "getservices");
		DAO_MM_ADD_ENTRY(&services);

		object_init_ex(&dependency_injector_new, dao_di_ce);
		DAO_MM_ADD_ENTRY(&dependency_injector_new);
		
		ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL(services), idx, key, value) {
			zval tmp = {};
			if (key) {
				ZVAL_STR(&tmp, key);
			} else {
				ZVAL_LONG(&tmp, idx);
			}
			DAO_MM_CALL_METHOD(NULL, &dependency_injector_new, "setservice", &tmp, value);
		} ZEND_HASH_FOREACH_END();
	} else {
		ZVAL_COPY_VALUE(&dependency_injector_new, _dependency_injector);
	}

	/**
	 * Request
	 */
	ZVAL_STR(&service, IS(request));
	object_init_ex(&requset, dao_http_request_ce);
	DAO_MM_ADD_ENTRY(&requset);
	DAO_MM_CALL_METHOD(NULL, &requset, "__construct", data);
	DAO_MM_CALL_METHOD(NULL, &dependency_injector_new, "set", &service, &requset);

	/**
	 * Mvc Router
	 */
	ZVAL_STR(&service, IS(router));
	DAO_MM_CALL_METHOD(&definition, &dependency_injector_new, "getraw", &service);
	DAO_MM_ADD_ENTRY(&definition);
	DAO_MM_CALL_METHOD(NULL, &dependency_injector_new, "set", &service, &definition, &DAO_GLOBAL(z_true));

	/**
	 * Mvc Dispatcher
	 */
	ZVAL_STR(&service, IS(dispatcher));
	DAO_CALL_METHOD(&definition, &dependency_injector_new, "getraw", &service);
	DAO_MM_ADD_ENTRY(&definition);
	DAO_CALL_METHOD(NULL, &dependency_injector_new, "set", &service, &definition, &DAO_GLOBAL(z_true));

	/**
	 * Mvc View
	 */
	ZVAL_STR(&service, IS(view));
	DAO_CALL_METHOD(&definition, &dependency_injector_new, "getraw", &service);
	DAO_MM_ADD_ENTRY(&definition);
	DAO_CALL_METHOD(NULL, &dependency_injector_new, "set", &service, &definition, &DAO_GLOBAL(z_true));

	ZVAL_COPY_VALUE(&app, getThis());

	DAO_MM_CALL_METHOD(NULL, &app, "setdi", &dependency_injector_new);
	DAO_MM_CALL_METHOD(&response, &app, "handle", uri);
	DAO_MM_ADD_ENTRY(&response);
	if (Z_TYPE(response) == IS_OBJECT) {
		DAO_MM_RETURN_CALL_METHOD(&response, "getcontent");
	}

	RETURN_MM();
}
