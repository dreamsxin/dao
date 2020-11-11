
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
  |          Kenji Minamoto <kenji.minamoto@gmail.com>                     |
  |          ZhuZongXin <dreamsxin@qq.com>                                 |
  +------------------------------------------------------------------------+
*/

#include "mvc/micro.h"
#include "mvc/micro/collectioninterface.h"
#include "mvc/micro/exception.h"
#include "mvc/micro/lazyloader.h"
#include "mvc/micro/middlewareinterface.h"
#include "mvc/routerinterface.h"
#include "diinterface.h"
#include "di/injectable.h"
#include "di/injectionawareinterface.h"
#include "di/factorydefault.h"
#include "http/responseinterface.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"
#include "kernel/exception.h"
#include "kernel/object.h"
#include "kernel/operators.h"
#include "kernel/file.h"
#include "kernel/array.h"
#include "kernel/concat.h"

#include "interned-strings.h"

/**
 * Dao\Mvc\Micro
 *
 * With Dao you can create "Micro-Framework like" applications. By doing this, you only need to
 * write a minimal amount of code to create a PHP application. Micro applications are suitable
 * to small applications, APIs and prototypes in a practical way.
 *
 *<code>
 *
 * $app = new Dao\Mvc\Micro();
 *
 * $app->get('/say/welcome/{name}', function ($name) {
 *    echo "<h1>Welcome $name!</h1>";
 * });
 *
 * $app->handle();
 *
 *</code>
 */
zend_class_entry *dao_mvc_micro_ce;

PHP_METHOD(Dao_Mvc_Micro, __construct);
PHP_METHOD(Dao_Mvc_Micro, setDI);
PHP_METHOD(Dao_Mvc_Micro, map);
PHP_METHOD(Dao_Mvc_Micro, get);
PHP_METHOD(Dao_Mvc_Micro, post);
PHP_METHOD(Dao_Mvc_Micro, put);
PHP_METHOD(Dao_Mvc_Micro, patch);
PHP_METHOD(Dao_Mvc_Micro, head);
PHP_METHOD(Dao_Mvc_Micro, delete);
PHP_METHOD(Dao_Mvc_Micro, options);
PHP_METHOD(Dao_Mvc_Micro, mount);
PHP_METHOD(Dao_Mvc_Micro, notFound);
PHP_METHOD(Dao_Mvc_Micro, getRouter);
PHP_METHOD(Dao_Mvc_Micro, offsetSet);
PHP_METHOD(Dao_Mvc_Micro, offsetExists);
PHP_METHOD(Dao_Mvc_Micro, offsetGet);
PHP_METHOD(Dao_Mvc_Micro, getSharedService);
PHP_METHOD(Dao_Mvc_Micro, handle);
PHP_METHOD(Dao_Mvc_Micro, stop);
PHP_METHOD(Dao_Mvc_Micro, setActiveHandler);
PHP_METHOD(Dao_Mvc_Micro, getActiveHandler);
PHP_METHOD(Dao_Mvc_Micro, getReturnedValue);
PHP_METHOD(Dao_Mvc_Micro, offsetUnset);
PHP_METHOD(Dao_Mvc_Micro, before);
PHP_METHOD(Dao_Mvc_Micro, after);
PHP_METHOD(Dao_Mvc_Micro, finish);
PHP_METHOD(Dao_Mvc_Micro, getHandlers);
PHP_METHOD(Dao_Mvc_Micro, error);
PHP_METHOD(Dao_Mvc_Micro, _throwException);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_micro___construct, 0, 0, 0)
	ZEND_ARG_INFO(0, dependencyInjector)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_micro_map, 0, 0, 2)
	ZEND_ARG_INFO(0, routePattern)
	ZEND_ARG_INFO(0, handler)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_micro_get, 0, 0, 2)
	ZEND_ARG_INFO(0, routePattern)
	ZEND_ARG_INFO(0, handler)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_micro_post, 0, 0, 2)
	ZEND_ARG_INFO(0, routePattern)
	ZEND_ARG_INFO(0, handler)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_micro_put, 0, 0, 2)
	ZEND_ARG_INFO(0, routePattern)
	ZEND_ARG_INFO(0, handler)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_micro_patch, 0, 0, 2)
	ZEND_ARG_INFO(0, routePattern)
	ZEND_ARG_INFO(0, handler)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_micro_head, 0, 0, 2)
	ZEND_ARG_INFO(0, routePattern)
	ZEND_ARG_INFO(0, handler)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_micro_delete, 0, 0, 2)
	ZEND_ARG_INFO(0, routePattern)
	ZEND_ARG_INFO(0, handler)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_micro_options, 0, 0, 2)
	ZEND_ARG_INFO(0, routePattern)
	ZEND_ARG_INFO(0, handler)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_micro_mount, 0, 0, 1)
	ZEND_ARG_INFO(0, collection)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_micro_notfound, 0, 0, 1)
	ZEND_ARG_INFO(0, handler)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_micro_offsetset, 0, 0, 2)
	ZEND_ARG_INFO(0, serviceName)
	ZEND_ARG_INFO(0, definition)
	ZEND_ARG_INFO(0, shared)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_micro_offsetexists, 0, 0, 1)
	ZEND_ARG_INFO(0, serviceName)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_micro_offsetget, 0, 0, 1)
	ZEND_ARG_INFO(0, serviceName)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_micro_getsharedservice, 0, 0, 1)
	ZEND_ARG_INFO(0, serviceName)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_micro_handle, 0, 0, 0)
	ZEND_ARG_INFO(0, uri)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_micro_setactivehandler, 0, 0, 1)
	ZEND_ARG_INFO(0, activeHandler)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_micro_offsetunset, 0, 0, 1)
	ZEND_ARG_INFO(0, alias)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_micro_before, 0, 0, 1)
	ZEND_ARG_INFO(0, handler)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_micro_after, 0, 0, 1)
	ZEND_ARG_INFO(0, handler)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_micro_finish, 0, 0, 1)
	ZEND_ARG_INFO(0, handler)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_micro_error, 0, 0, 1)
	ZEND_ARG_INFO(0, handler)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_micro__throwexception, 0, 0, 1)
	ZEND_ARG_INFO(0, message)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_mvc_micro_method_entry[] = {
	PHP_ME(Dao_Mvc_Micro, __construct, arginfo_dao_mvc_micro___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Mvc_Micro, setDI, arginfo_dao_di_injectionawareinterface_setdi, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Micro, map, arginfo_dao_mvc_micro_map, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Micro, get, arginfo_dao_mvc_micro_get, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Micro, post, arginfo_dao_mvc_micro_post, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Micro, put, arginfo_dao_mvc_micro_put, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Micro, patch, arginfo_dao_mvc_micro_patch, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Micro, head, arginfo_dao_mvc_micro_head, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Micro, delete, arginfo_dao_mvc_micro_delete, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Micro, options, arginfo_dao_mvc_micro_options, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Micro, mount, arginfo_dao_mvc_micro_mount, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Micro, notFound, arginfo_dao_mvc_micro_notfound, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Micro, getRouter, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Micro, offsetSet, arginfo_dao_mvc_micro_offsetset, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Micro, offsetGet, arginfo_dao_mvc_micro_offsetget, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Micro, getSharedService, arginfo_dao_mvc_micro_getsharedservice, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Micro, handle, arginfo_dao_mvc_micro_handle, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Micro, stop, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Micro, setActiveHandler, arginfo_dao_mvc_micro_setactivehandler, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Micro, getActiveHandler, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Micro, getReturnedValue, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Micro, offsetExists, arginfo_dao_mvc_micro_offsetexists, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Micro, offsetUnset, arginfo_dao_mvc_micro_offsetunset, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Micro, before, arginfo_dao_mvc_micro_before, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Micro, after, arginfo_dao_mvc_micro_after, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Micro, finish, arginfo_dao_mvc_micro_finish, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Micro, getHandlers, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Micro, error, arginfo_dao_mvc_micro_error, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Micro, _throwException, arginfo_dao_mvc_micro__throwexception, ZEND_ACC_PROTECTED)
	PHP_FE_END
};

/**
 * Dao\Mvc\Micro initializer
 */
DAO_INIT_CLASS(Dao_Mvc_Micro){

	DAO_REGISTER_CLASS_EX(Dao\\Mvc, Micro, mvc_micro, dao_di_injectable_ce, dao_mvc_micro_method_entry, 0);

	zend_declare_property_null(dao_mvc_micro_ce, SL("_handlers"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_micro_ce, SL("_router"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_micro_ce, SL("_stopped"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_micro_ce, SL("_notFoundHandler"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_micro_ce, SL("_activeHandler"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_micro_ce, SL("_beforeHandlers"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_micro_ce, SL("_afterHandlers"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_micro_ce, SL("_finishHandlers"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_micro_ce, SL("_returnedValue"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_micro_ce, SL("_errorHandler"), ZEND_ACC_PROTECTED);

	zend_class_implements(dao_mvc_micro_ce, 1, zend_ce_arrayaccess);

	return SUCCESS;
}

/**
 * Dao\Mvc\Micro constructor
 *
 * @param Dao\DiInterface $dependencyInjector
 */
PHP_METHOD(Dao_Mvc_Micro, __construct){

	zval *dependency_injector = NULL;

	dao_fetch_params(0, 0, 1, &dependency_injector);

	if (dependency_injector && Z_TYPE_P(dependency_injector) == IS_OBJECT) {
		DAO_VERIFY_INTERFACE(dependency_injector, dao_diinterface_ce);
		DAO_CALL_METHOD(NULL, getThis(), "setdi", dependency_injector);
	}
}

/**
 * Sets the DependencyInjector container
 *
 * @param Dao\DiInterface $dependencyInjector
 */
PHP_METHOD(Dao_Mvc_Micro, setDI){

	zval *dependency_injector, service = {}, exists = {};

	dao_fetch_params(0, 1, 0, &dependency_injector);
	DAO_VERIFY_INTERFACE_EX(dependency_injector, dao_diinterface_ce, dao_mvc_micro_exception_ce);

	/**
	 * We automatically set ourselves as application service
	 */
	ZVAL_STR(&service, IS(application));

	DAO_CALL_METHOD(&exists, dependency_injector, "has", &service);
	if (!zend_is_true(&exists)) {
		DAO_CALL_METHOD(NULL, dependency_injector, "set", &service, getThis());
	}

	DAO_CALL_PARENT(NULL, dao_mvc_micro_ce, getThis(), "setdi", dependency_injector);
}

static void dao_mvc_micro_generic_add(INTERNAL_FUNCTION_PARAMETERS, const char *method)
{
	zval *route_pattern, *handler, router = {}, route_id = {};

	dao_fetch_params(1, 2, 0, &route_pattern, &handler);

	/**
	 * We create a router even if there is no one in the DI
	 */
	DAO_MM_CALL_METHOD(&router, getThis(), "getrouter");
	DAO_MM_ADD_ENTRY(&router);

	/**
	 * Routes are added to the router
	 */
	DAO_MM_RETURN_CALL_METHOD(&router, method, route_pattern);

	/**
	 * Using the id produced by the router we store the handler
	 */
	DAO_MM_CALL_METHOD(&route_id, return_value, "getrouteid");
	DAO_MM_ADD_ENTRY(&route_id);

	dao_update_property_array(getThis(), SL("_handlers"), &route_id, handler);

	/**
	 * The route is returned, the developer can add more things on it
	 */
	RETURN_MM();
}

/**
 * Maps a route to a handler without any HTTP method constraint
 *
 * @param string $routePattern
 * @param callable $handler
 * @return Dao\Mvc\Router\RouteInterface
 */
PHP_METHOD(Dao_Mvc_Micro, map){

	dao_mvc_micro_generic_add(INTERNAL_FUNCTION_PARAM_PASSTHRU, "add");
}

/**
 * Maps a route to a handler that only matches if the HTTP method is GET
 *
 * @param string $routePattern
 * @param callable $handler
 * @return Dao\Mvc\Router\RouteInterface
 */
PHP_METHOD(Dao_Mvc_Micro, get){

	dao_mvc_micro_generic_add(INTERNAL_FUNCTION_PARAM_PASSTHRU, "addget");
}

/**
 * Maps a route to a handler that only matches if the HTTP method is POST
 *
 * @param string $routePattern
 * @param callable $handler
 * @return Dao\Mvc\Router\RouteInterface
 */
PHP_METHOD(Dao_Mvc_Micro, post){

	dao_mvc_micro_generic_add(INTERNAL_FUNCTION_PARAM_PASSTHRU, "addpost");
}

/**
 * Maps a route to a handler that only matches if the HTTP method is PUT
 *
 * @param string $routePattern
 * @param callable $handler
 * @return Dao\Mvc\Router\RouteInterface
 */
PHP_METHOD(Dao_Mvc_Micro, put){

	dao_mvc_micro_generic_add(INTERNAL_FUNCTION_PARAM_PASSTHRU, "addput");
}

/**
 * Maps a route to a handler that only matches if the HTTP method is PATCH
 *
 * @param string $routePattern
 * @param callable $handler
 * @return Dao\Mvc\Router\RouteInterface
 */
PHP_METHOD(Dao_Mvc_Micro, patch){

	dao_mvc_micro_generic_add(INTERNAL_FUNCTION_PARAM_PASSTHRU, "addpatch");
}

/**
 * Maps a route to a handler that only matches if the HTTP method is HEAD
 *
 * @param string $routePattern
 * @param callable $handler
 * @return Dao\Mvc\Router\RouteInterface
 */
PHP_METHOD(Dao_Mvc_Micro, head){

	dao_mvc_micro_generic_add(INTERNAL_FUNCTION_PARAM_PASSTHRU, "addhead");
}

/**
 * Maps a route to a handler that only matches if the HTTP method is DELETE
 *
 * @param string $routePattern
 * @param callable $handler
 * @return Dao\Mvc\Router\RouteInterface
 */
PHP_METHOD(Dao_Mvc_Micro, delete){

	dao_mvc_micro_generic_add(INTERNAL_FUNCTION_PARAM_PASSTHRU, "adddelete");
}

/**
 * Maps a route to a handler that only matches if the HTTP method is OPTIONS
 *
 * @param string $routePattern
 * @param callable $handler
 * @return Dao\Mvc\Router\RouteInterface
 */
PHP_METHOD(Dao_Mvc_Micro, options){

	dao_mvc_micro_generic_add(INTERNAL_FUNCTION_PARAM_PASSTHRU, "addoptions");
}

/**
 * Mounts a collection of handlers
 *
 * @param Dao\Mvc\Collection $collection
 * @return Dao\Mvc\Micro
 */
PHP_METHOD(Dao_Mvc_Micro, mount){

	zval *collection, main_handler = {}, handlers = {}, lazy = {}, lazy_handler = {}, prefix = {}, *handler;

	dao_fetch_params(1, 1, 0, &collection);
	DAO_MM_VERIFY_INTERFACE_EX(collection, dao_mvc_micro_collectioninterface_ce, dao_mvc_micro_exception_ce);

	/* Get the main handler */
	DAO_MM_CALL_METHOD(&main_handler, collection, "gethandler");
	DAO_MM_ADD_ENTRY(&main_handler);

	if (DAO_IS_EMPTY(&main_handler)) {
		DAO_MM_THROW_EXCEPTION_STR(dao_mvc_micro_exception_ce, "The collection requires a main handler");
		return;
	}

	DAO_MM_CALL_METHOD(&handlers, collection, "gethandlers");
	DAO_MM_ADD_ENTRY(&handlers);

	if (!dao_fast_count_ev(&handlers)) {
		DAO_MM_THROW_EXCEPTION_STR(dao_mvc_micro_exception_ce, "There are no handlers to mount");
		return;
	}

	if (Z_TYPE(handlers) == IS_ARRAY) {
		/* Check if handler is lazy */
		DAO_MM_CALL_METHOD(&lazy, collection, "islazy");

		if (zend_is_true(&lazy)) {
			object_init_ex(&lazy_handler, dao_mvc_micro_lazyloader_ce);
			DAO_MM_CALL_METHOD(NULL, &lazy_handler, "__construct", &main_handler);
			DAO_MM_ADD_ENTRY(&lazy_handler);
		} else {
			ZVAL_COPY_VALUE(&lazy_handler, &main_handler);
		}

		/* Get the main prefix for the collection */
		DAO_MM_CALL_METHOD(&prefix, collection, "getprefix");
		DAO_MM_ADD_ENTRY(&prefix);

		ZEND_HASH_FOREACH_VAL(Z_ARRVAL(handlers), handler) {
			zval methods = {}, pattern = {}, sub_handler = {}, name = {}, real_handler = {}, prefixed_pattern = {}, route = {};
			if (Z_TYPE_P(handler) != IS_ARRAY) {
				DAO_MM_THROW_EXCEPTION_STR(dao_mvc_micro_exception_ce, "One of the registered handlers is invalid");
				return;
			}

			if (
				    !dao_array_isset_fetch_long(&methods, handler, 0, PH_READONLY)
				 || !dao_array_isset_fetch_long(&pattern, handler, 1, PH_READONLY)
				 || !dao_array_isset_fetch_long(&sub_handler, handler, 2, PH_READONLY)
				 || !dao_array_isset_fetch_long(&name, handler, 3, PH_READONLY)
			) {
				DAO_MM_THROW_EXCEPTION_STR(dao_mvc_micro_exception_ce, "One of the registered handlers is invalid");
				return;
			}

			/* Create a real handler */
			array_init_size(&real_handler, 2);
			dao_array_append(&real_handler, &lazy_handler, PH_COPY);
			dao_array_append(&real_handler, &sub_handler, PH_COPY);
			DAO_MM_ADD_ENTRY(&real_handler);

			if (DAO_IS_NOT_EMPTY(&prefix)) {
				if (DAO_IS_STRING(&pattern, "/")) {
					ZVAL_COPY_VALUE(&prefixed_pattern, &prefix);
				} else {
					DAO_CONCAT_VV(&prefixed_pattern, &prefix, &pattern);
					DAO_MM_ADD_ENTRY(&prefixed_pattern);
				}
			} else {
				ZVAL_COPY_VALUE(&prefixed_pattern, &pattern);
			}

			/* Map the route manually */
			DAO_MM_CALL_METHOD(&route, getThis(), "map", &prefixed_pattern, &real_handler);
			DAO_MM_ADD_ENTRY(&route);
			if (Z_TYPE(methods) != IS_NULL) {
				DAO_MM_CALL_METHOD(NULL, &route, "via", &methods);
			}

			if (Z_TYPE(name) != IS_NULL) {
				DAO_MM_CALL_METHOD(NULL, &route, "setname", &name);
			}
		} ZEND_HASH_FOREACH_END();

	}

	RETURN_MM_THIS();
}

/**
 * Sets a handler that will be called when the router doesn't match any of the defined routes
 *
 * @param callable $handler
 * @return Dao\Mvc\Micro
 */
PHP_METHOD(Dao_Mvc_Micro, notFound){

	zval *handler;

	dao_fetch_params(0, 1, 0, &handler);

	dao_update_property(getThis(), SL("_notFoundHandler"), handler);
	RETURN_THIS();
}

/**
 * Returns the internal router used by the application
 *
 * @return Dao\Mvc\RouterInterface
 */
PHP_METHOD(Dao_Mvc_Micro, getRouter){

	zval router = {}, service_name = {};

	DAO_MM_INIT();

	dao_read_property(&router, getThis(), SL("_router"), PH_READONLY);
	if (Z_TYPE(router) != IS_OBJECT) {
		ZVAL_STR(&service_name, IS(router));

		DAO_MM_CALL_METHOD(&router, getThis(), "getsharedservice", &service_name);
		DAO_MM_ADD_ENTRY(&router);
		DAO_MM_VERIFY_INTERFACE(&router, dao_mvc_routerinterface_ce);

		/**
		 * Clear the set routes if any
		 */
		DAO_MM_CALL_METHOD(NULL, &router, "clear");

		/**
		 * Automatically remove extra slashes
		 */
		DAO_MM_CALL_METHOD(NULL, &router, "removeextraslashes", &DAO_GLOBAL(z_true));

		/**
		 * Update the internal router
		 */
		dao_update_property(getThis(), SL("_router"), &router);
	}

	RETURN_MM_CTOR(&router);
}

/**
 * Sets a service from the DI
 *
 * @param string $serviceName
 * @param mixed $definition
 * @param boolean $shared
 * @return Dao\Di\ServiceInterface
 */
PHP_METHOD(Dao_Mvc_Micro, offsetSet){

	zval *service_name, *definition, *shared = NULL, dependency_injector = {};

	dao_fetch_params(1, 2, 1, &service_name, &definition, &shared);

	if (!shared) {
		shared = &DAO_GLOBAL(z_false);
	}

	DAO_MM_CALL_METHOD(&dependency_injector, getThis(), "getdi");
	DAO_MM_ADD_ENTRY(&dependency_injector);
	if (!zend_is_true(&dependency_injector)) {
		DAO_MM_THROW_EXCEPTION_STR(dao_mvc_micro_exception_ce, "A dependency injection container is required to access related dispatching services");
		return;
	}
	DAO_MM_RETURN_CALL_METHOD(&dependency_injector, "set", service_name, definition, shared);
	RETURN_MM();
}

/**
 * Checks if a service is registered in the DI
 *
 * @param string $serviceName
 * @return boolean
 */
PHP_METHOD(Dao_Mvc_Micro, offsetExists){

	zval *service_name;

	dao_fetch_params(0, 1, 0, &service_name);

	DAO_RETURN_CALL_PARENT(dao_mvc_micro_ce, getThis(), "hasservice", service_name);
}

/**
 * Obtains a service from the DI
 *
 * @param string $serviceName
 * @return object
 */
PHP_METHOD(Dao_Mvc_Micro, offsetGet){

	zval *service_name, dependency_injector = {};

	dao_fetch_params(1, 1, 0, &service_name);

	DAO_MM_CALL_METHOD(&dependency_injector, getThis(), "getdi");
	DAO_MM_ADD_ENTRY(&dependency_injector);
	if (Z_TYPE(dependency_injector) != IS_OBJECT) {
		DAO_MM_THROW_EXCEPTION_STR(dao_mvc_micro_exception_ce, "A dependency injection container is required to access related dispatching services");
		return;
	}

	DAO_MM_RETURN_CALL_METHOD(&dependency_injector, "get", service_name);
	RETURN_MM();
}

/**
 * Obtains a shared service from the DI
 *
 * @param string $serviceName
 * @return mixed
 */
PHP_METHOD(Dao_Mvc_Micro, getSharedService){

	zval *service_name, dependency_injector = {};

	dao_fetch_params(1, 1, 0, &service_name);

	DAO_MM_CALL_METHOD(&dependency_injector, getThis(), "getdi");
	DAO_MM_ADD_ENTRY(&dependency_injector);
	if (Z_TYPE(dependency_injector) != IS_OBJECT) {
		DAO_MM_THROW_EXCEPTION_STR(dao_mvc_micro_exception_ce, "A dependency injection container is required to access related dispatching services");
		return;
	}

	DAO_MM_RETURN_CALL_METHOD(&dependency_injector, "getshared", service_name);
	RETURN_MM();
}

/**
 * Handle the whole request
 *
 * @param string $uri
 * @return mixed
 */
PHP_METHOD(Dao_Mvc_Micro, handle){

	zval *uri = NULL, dependency_injector = {}, error_message = {}, event_name = {}, status = {}, service = {}, router = {}, matched_route = {};
	zval handlers = {}, route_id = {}, handler = {}, before_handlers = {}, *before, stopped = {}, params = {};
	zval after_handlers = {}, *after, not_found_handler = {}, finish_handlers = {}, *finish, returned_response_sent = {};

	dao_fetch_params(1, 0, 1, &uri);

	if (!uri) {
		uri = &DAO_GLOBAL(z_null);
	}

	DAO_MM_CALL_METHOD(&dependency_injector, getThis(), "getdi");
	DAO_MM_ADD_ENTRY(&dependency_injector);
	if (Z_TYPE(dependency_injector) != IS_OBJECT) {
		DAO_MM_THROW_EXCEPTION_STR(dao_mvc_micro_exception_ce, "A dependency injection container is required to access related dispatching services");
		return;
	}

	/**
	 * Calling beforeHandle routing
	 */
	DAO_MM_ZVAL_STRING(&event_name, "micro:beforeHandleRoute");
	DAO_MM_CALL_SELF(&status, "fireeventcancel", &event_name);

	if (DAO_IS_FALSE(&status)) {
		RETURN_MM_FALSE;
	}
	zval_ptr_dtor(&status);

	/**
	 * Handling routing information
	 */
	ZVAL_STR(&service, IS(router));

	DAO_MM_CALL_METHOD(&router, &dependency_injector, "getshared", &service);
	DAO_MM_ADD_ENTRY(&router);
	DAO_MM_VERIFY_INTERFACE(&router, dao_mvc_routerinterface_ce);

	/**
	 * Handle the URI as normal
	 */
	DAO_MM_CALL_METHOD(NULL, &router, "handle", uri);

	/**
	 * Check if one route was matched
	 */
	DAO_MM_CALL_METHOD(&matched_route, &router, "getmatchedroute");
	DAO_MM_ADD_ENTRY(&matched_route);
	if (Z_TYPE(matched_route) == IS_OBJECT) {

		dao_read_property(&handlers, getThis(), SL("_handlers"), PH_NOISY|PH_READONLY);

		DAO_MM_CALL_METHOD(&route_id, &matched_route, "getrouteid");
		DAO_MM_ADD_ENTRY(&route_id);
		if (!dao_array_isset_fetch(&handler, &handlers, &route_id, PH_READONLY)) {
			DAO_MM_ZVAL_STRING(&error_message, "Matched route doesn't have an associate handler");

			DAO_MM_RETURN_CALL_SELF("_throwexception", &error_message);
			RETURN_MM();
		}

		/**
		 * Updating active handler
		 */
		dao_update_property(getThis(), SL("_activeHandler"), &handler);

		/**
		 * Calling beforeExecuteRoute event
		 */
		DAO_MM_ZVAL_STRING(&event_name, "micro:beforeExecuteRoute");
		DAO_MM_CALL_SELF(&status, "fireeventcancel", &event_name);

		if (DAO_IS_FALSE(&status)) {
			RETURN_MM_FALSE;
		} else {
			dao_read_property(&handler, getThis(), SL("_activeHandler"), PH_READONLY);
		}
		zval_ptr_dtor(&status);

		dao_read_property(&before_handlers, getThis(), SL("_beforeHandlers"), PH_READONLY);
		if (Z_TYPE(before_handlers) == IS_ARRAY) {
			dao_update_property(getThis(), SL("_stopped"), &DAO_GLOBAL(z_false));

			/**
			 * Calls the before handlers
			 */
			ZEND_HASH_FOREACH_VAL(Z_ARRVAL(before_handlers), before) {
				int is_middleware;
				if (Z_TYPE_P(before) == IS_OBJECT) {
					is_middleware = instanceof_function_ex(Z_OBJCE_P(before), dao_mvc_micro_middlewareinterface_ce, 1);

					if (is_middleware) {

						/**
						 * Call the middleware
						 */
						DAO_MM_CALL_METHOD(NULL, before, "call", getThis());

						/**
						 * Reload the status
						 */
						dao_read_property(&stopped, getThis(), SL("_stopped"), PH_NOISY|PH_READONLY);

						/**
						 * break the execution if the middleware was stopped
						 */
						if (zend_is_true(&stopped)) {
							break;
						}
						continue;
					}
				}
				if (!dao_is_callable(before)) {
					DAO_MM_ZVAL_STRING(&error_message, "The before handler is not callable");

					DAO_MM_RETURN_CALL_SELF("_throwexception", &error_message);
					RETURN_MM();
				}

				/**
				 * Call the before handler, if it returns false exit
				 */
				DAO_MM_CALL_USER_FUNC(&status, before);
				if (DAO_IS_FALSE(&status)) {
					RETURN_MM_FALSE;
				}

				/**
				 * Reload the 'stopped' status
				 */
				dao_read_property(&stopped, getThis(), SL("_stopped"), PH_NOISY|PH_READONLY);
				if (zend_is_true(&stopped)) {
					RETURN_MM_NCTOR(&status);
				}
			} ZEND_HASH_FOREACH_END();

		}

		/**
		 * Calling the Handler in the PHP userland
		 */
		DAO_MM_CALL_METHOD(&params, &router, "getparams");
		DAO_MM_ADD_ENTRY(&params);

		DAO_MM_CALL_USER_FUNC_ARRAY(return_value, &handler, &params);

		/**
		 * Update the returned value
		 */
		dao_update_property(getThis(), SL("_returnedValue"), return_value);

		/**
		 * Calling afterExecuteRoute event
		 */
		DAO_MM_ZVAL_STRING(&event_name, "micro:afterExecuteRoute");
		DAO_MM_CALL_SELF(NULL, "fireevent", &event_name);

		dao_read_property(&after_handlers, getThis(), SL("_afterHandlers"), PH_NOISY|PH_READONLY);
		if (Z_TYPE(after_handlers) == IS_ARRAY) {
			dao_update_property_bool(getThis(), SL("_stopped"), 0);

			/**
			 * Calls the after handlers
			 */
			ZEND_HASH_FOREACH_VAL(Z_ARRVAL(after_handlers), after) {
				int is_middleware;
				if (Z_TYPE_P(after) == IS_OBJECT) {
					is_middleware = instanceof_function_ex(Z_OBJCE_P(after), dao_mvc_micro_middlewareinterface_ce, 1);
					if (is_middleware) {

						/**
						 * Call the middleware
						 */
						DAO_MM_CALL_METHOD(NULL, after, "call", getThis());

						/**
						 * Reload the status
						 */
						dao_read_property(&stopped, getThis(), SL("_stopped"), PH_NOISY|PH_READONLY);

						/**
						 * break the execution if the middleware was stopped
						 */
						if (zend_is_true(&stopped)) {
							break;
						}
						continue;
					}
				}
				if (!dao_is_callable(after)) {
					ZVAL_STRING(&error_message, "One of the 'after' handlers is not callable");

					DAO_RETURN_CALL_SELF("_throwexception", &error_message);
					return;
				}

				DAO_MM_CALL_USER_FUNC(NULL, after);
			} ZEND_HASH_FOREACH_END();

		}
	} else {
		/**
		 * Calling beforeNotFound event
		 */
		DAO_MM_ZVAL_STRING(&event_name, "micro:beforeNotFound");
		DAO_MM_CALL_SELF(&status, "fireeventcancel", &event_name);

		if (DAO_IS_FALSE(&status)) {
			RETURN_MM_FALSE;
		}
		zval_ptr_dtor(&status);

		/**
		 * Check if a notfoundhandler is defined and it's callable
		 */
		dao_read_property(&not_found_handler, getThis(), SL("_notFoundHandler"), PH_NOISY|PH_READONLY);
		if (!dao_is_callable(&not_found_handler)) {
			DAO_MM_ZVAL_STRING(&error_message, "The Not-Found handler is not callable or is not defined");

			DAO_MM_RETURN_CALL_SELF("_throwexception", &error_message);
			RETURN_MM();
		}

		/**
		 * Call the Not-Found handler
		 */
		DAO_MM_CALL_USER_FUNC(return_value, &not_found_handler);

		/**
		 * Update the returned value
		 */
		dao_update_property(getThis(), SL("_returnedValue"), return_value);

		RETURN_MM();
	}

	/**
	 * Calling afterHandleRoute event
	 */
	DAO_MM_ZVAL_STRING(&event_name, "micro:afterHandleRoute");
	DAO_MM_CALL_SELF(NULL, "fireevent", &event_name);

	dao_read_property(&finish_handlers, getThis(), SL("_finishHandlers"), PH_NOISY|PH_READONLY);
	if (Z_TYPE(finish_handlers) == IS_ARRAY) {
		dao_update_property(getThis(), SL("_stopped"), &DAO_GLOBAL(z_false));
		/**
		 * Calls the finish handlers
		 */
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL(finish_handlers), finish) {
			/**
			 * Try to execute middleware as plugins
			 */
			if (Z_TYPE_P(finish) == IS_OBJECT) {
				int is_middleware = instanceof_function_ex(Z_OBJCE_P(finish), dao_mvc_micro_middlewareinterface_ce, 1);
				if (is_middleware) {

					/**
					 * Call the middleware
					 */
					DAO_MM_CALL_METHOD(NULL, finish, "call", getThis());

					/**
					 * Reload the status
					 */
					dao_read_property(&stopped, getThis(), SL("_stopped"), PH_NOISY|PH_READONLY);

					/**
					 * break the execution if the middleware was stopped
					 */
					if (zend_is_true(&stopped)) {
						break;
					}
					continue;
				}
			}
			if (!dao_is_callable(finish)) {
				DAO_MM_ZVAL_STRING(&error_message, "One of finish handlers is not callable");

				DAO_MM_RETURN_CALL_SELF("_throwexception", &error_message);
				return;
			}

			if (Z_TYPE(params) == IS_NULL) {
				array_init_size(&params, 1);
				dao_array_append(&params, getThis(), PH_COPY);
				DAO_MM_ADD_ENTRY(&params);
			}

			/**
			 * Call the 'finish' middleware
			 */
			DAO_MM_CALL_USER_FUNC_ARRAY(NULL, finish, &params);

			/**
			 * Reload the status
			 */
			dao_read_property(&stopped, getThis(), SL("_stopped"), PH_NOISY|PH_READONLY);

			/**
			 * break the execution if the middleware was stopped
			 */
			if (zend_is_true(&stopped)) {
				break;
			}
		} ZEND_HASH_FOREACH_END();

	}

	/**
	 * Check if the returned object is already a response
	 */
	if (Z_TYPE_P(return_value) == IS_OBJECT) {
		int returned_response = instanceof_function_ex(Z_OBJCE_P(return_value), dao_http_responseinterface_ce, 1);

		if (returned_response) {
			DAO_MM_CALL_METHOD(&returned_response_sent, return_value, "issent");

			if (DAO_IS_FALSE(&returned_response_sent)) {
				/**
				 * Automatically send the responses
				 */
				DAO_MM_CALL_METHOD(NULL, return_value, "send");
			}
		}
	}
	RETURN_MM();
}

/**
 * Stops the middleware execution avoiding than other middlewares be executed
 */
PHP_METHOD(Dao_Mvc_Micro, stop){


	dao_update_property(getThis(), SL("_stopped"), &DAO_GLOBAL(z_true));

}

/**
 * Sets externally the handler that must be called by the matched route
 *
 * @param callable $activeHandler
 */
PHP_METHOD(Dao_Mvc_Micro, setActiveHandler){

	zval *active_handler;

	dao_fetch_params(0, 1, 0, &active_handler);

	dao_update_property(getThis(), SL("_activeHandler"), active_handler);

}

/**
 * Return the handler that will be called for the matched route
 *
 * @return callable
 */
PHP_METHOD(Dao_Mvc_Micro, getActiveHandler){


	RETURN_MEMBER(getThis(), "_activeHandler");
}

/**
 * Returns the value returned by the executed handler
 *
 * @return mixed
 */
PHP_METHOD(Dao_Mvc_Micro, getReturnedValue){


	RETURN_MEMBER(getThis(), "_returnedValue");
}

/**
 * Removes a service from the internal services container using the array syntax
 *
 * @param string $alias
 * @todo Not implemented
 */
PHP_METHOD(Dao_Mvc_Micro, offsetUnset){

	zval *alias;

	dao_fetch_params(0, 1, 0, &alias);

	RETURN_ZVAL(alias, 1, 0);
}

/**
 * Appends a before middleware to be called before execute the route
 *
 * @param callable $handler
 * @return Dao\Mvc\Micro
 */
PHP_METHOD(Dao_Mvc_Micro, before){

	zval *handler;

	dao_fetch_params(0, 1, 0, &handler);

	dao_update_property_array_append(getThis(), SL("_beforeHandlers"), handler);
	RETURN_THIS();
}

/**
 * Appends an 'after' middleware to be called after execute the route
 *
 * @param callable $handler
 * @return Dao\Mvc\Micro
 */
PHP_METHOD(Dao_Mvc_Micro, after){

	zval *handler;

	dao_fetch_params(0, 1, 0, &handler);

	dao_update_property_array_append(getThis(), SL("_afterHandlers"), handler);
	RETURN_THIS();
}

/**
 * Appends a 'finish' middleware to be called when the request is finished
 *
 * @param callable $handler
 * @return Dao\Mvc\Micro
 */
PHP_METHOD(Dao_Mvc_Micro, finish){

	zval *handler;

	dao_fetch_params(0, 1, 0, &handler);

	dao_update_property_array_append(getThis(), SL("_finishHandlers"), handler);
	RETURN_THIS();
}

/**
 * Returns the internal handlers attached to the application
 *
 * @return array
 */
PHP_METHOD(Dao_Mvc_Micro, getHandlers){


	RETURN_MEMBER(getThis(), "_handlers");
}

/**
 * Sets a handler that will be called when an exception is thrown handling the route
 *
 * @param callable handler
 * @return Dao\Mvc\Micro
 */
PHP_METHOD(Dao_Mvc_Micro, error){

	zval *handler;

	dao_fetch_params(0, 1, 0, &handler);

	dao_update_property_array_append(getThis(), SL("_errorHandler"), handler);
	RETURN_THIS();
}

/**
 * Sets a handler that will be called when an exception is thrown handling the route
 *
 * @param string message
 * @return Dao\Mvc\Micro
 */
PHP_METHOD(Dao_Mvc_Micro, _throwException){

	zval *message, object = {}, handler = {}, arguments = {};

	dao_fetch_params(1, 1, 0, &message);

	dao_read_property(&handler, getThis(), SL("_errorHandler"), PH_NOISY|PH_READONLY);

	if (!dao_is_callable(&handler)) {
		DAO_MM_THROW_EXCEPTION_ZVAL(dao_mvc_micro_exception_ce, message);
		return;
	}

	object_init_ex(&object, dao_mvc_micro_exception_ce);
	DAO_MM_ADD_ENTRY(&object);
	DAO_MM_CALL_METHOD(NULL, &object, "__construct", message);

	array_init_size(&arguments, 1);
	dao_array_append(&arguments, &object, PH_COPY);
	DAO_MM_ADD_ENTRY(&arguments);

	DAO_MM_CALL_USER_FUNC_ARRAY(return_value, &handler, &arguments);

	if (Z_TYPE_P(return_value) != IS_OBJECT || !instanceof_function_ex(Z_OBJCE_P(return_value), dao_http_responseinterface_ce, 1)) {
		DAO_MM_THROW_EXCEPTION_ZVAL(dao_mvc_micro_exception_ce, message);
		return;
	} else {
		DAO_MM_CALL_METHOD(NULL, return_value, "send");
	}
	RETURN_MM();
}
