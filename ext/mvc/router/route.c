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

#include "mvc/router/route.h"
#include "mvc/../router.h"
#include "mvc/router/routeinterface.h"
#include "mvc/router/exception.h"
#include "mvc/router/group.h"
#include "debug.h"

#include <Zend/zend_closures.h>

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"
#include "kernel/object.h"
#include "kernel/operators.h"
#include "kernel/string.h"
#include "kernel/concat.h"
#include "kernel/exception.h"
#include "kernel/array.h"
#include "kernel/framework/router.h"
#include "kernel/hash.h"
#include "kernel/debug.h"

#include "interned-strings.h"

/**
 * Dao\Mvc\Router\Route
 *
 * This class represents every route added to the router
 */
zend_class_entry *dao_mvc_router_route_ce;

PHP_METHOD(Dao_Mvc_Router_Route, __construct);
PHP_METHOD(Dao_Mvc_Router_Route, compilePattern);
PHP_METHOD(Dao_Mvc_Router_Route, via);
PHP_METHOD(Dao_Mvc_Router_Route, reConfigure);
PHP_METHOD(Dao_Mvc_Router_Route, getName);
PHP_METHOD(Dao_Mvc_Router_Route, setName);
PHP_METHOD(Dao_Mvc_Router_Route, beforeMatch);
PHP_METHOD(Dao_Mvc_Router_Route, getBeforeMatch);
PHP_METHOD(Dao_Mvc_Router_Route, getRouteId);
PHP_METHOD(Dao_Mvc_Router_Route, getPattern);
PHP_METHOD(Dao_Mvc_Router_Route, getCompiledPattern);
PHP_METHOD(Dao_Mvc_Router_Route, getPaths);
PHP_METHOD(Dao_Mvc_Router_Route, getReversedPaths);
PHP_METHOD(Dao_Mvc_Router_Route, setHttpMethods);
PHP_METHOD(Dao_Mvc_Router_Route, getHttpMethods);
PHP_METHOD(Dao_Mvc_Router_Route, setPrefix);
PHP_METHOD(Dao_Mvc_Router_Route, getPrefix);
PHP_METHOD(Dao_Mvc_Router_Route, setHostname);
PHP_METHOD(Dao_Mvc_Router_Route, getHostname);
PHP_METHOD(Dao_Mvc_Router_Route, setGroup);
PHP_METHOD(Dao_Mvc_Router_Route, getGroup);
PHP_METHOD(Dao_Mvc_Router_Route, convert);
PHP_METHOD(Dao_Mvc_Router_Route, getConverters);
PHP_METHOD(Dao_Mvc_Router_Route, reset);
PHP_METHOD(Dao_Mvc_Router_Route, setDefaults);
PHP_METHOD(Dao_Mvc_Router_Route, getDefaults);
PHP_METHOD(Dao_Mvc_Router_Route, getDefaultNamespace);
PHP_METHOD(Dao_Mvc_Router_Route, getDefaultModule);
PHP_METHOD(Dao_Mvc_Router_Route, getDefaultController);
PHP_METHOD(Dao_Mvc_Router_Route, getDefaultAction);
PHP_METHOD(Dao_Mvc_Router_Route, getDefaultParams);
PHP_METHOD(Dao_Mvc_Router_Route, setUrlGenerator);
PHP_METHOD(Dao_Mvc_Router_Route, getUrlGenerator);
PHP_METHOD(Dao_Mvc_Router_Route, setCaseSensitive);
PHP_METHOD(Dao_Mvc_Router_Route, getCaseSensitive);
PHP_METHOD(Dao_Mvc_Router_Route, setMode);
PHP_METHOD(Dao_Mvc_Router_Route, getMode);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_router_route___construct, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, pattern, IS_STRING, 0)
	ZEND_ARG_INFO(0, paths)
	ZEND_ARG_INFO(0, httpMethods)
	ZEND_ARG_TYPE_INFO(0, regex, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_router_route_setgroup, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, group, Dao\\Mvc\\Router\\Group, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_router_route_beforematch, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, callback, Closure, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_router_route_setprefix, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, prefix, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_router_route_sethostname, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, hostname, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_router_route_convert, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, converter, Closure, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_router_route_seturlgenerator, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, handler, Closure, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_router_route_setcasesensitive, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, caseSensitive, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_mvc_router_route_method_entry[] = {
	PHP_ME(Dao_Mvc_Router_Route, __construct, arginfo_dao_mvc_router_route___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Mvc_Router_Route, compilePattern, arginfo_dao_mvc_router_routeinterface_compilepattern, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Route, via, arginfo_dao_mvc_router_routeinterface_via, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Route, reConfigure, arginfo_dao_mvc_router_routeinterface_reconfigure, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Route, getName, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Route, setName, arginfo_dao_mvc_router_routeinterface_setname, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Route, beforeMatch, arginfo_dao_mvc_router_route_beforematch, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Route, getBeforeMatch, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Route, getRouteId, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Route, getPattern, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Route, getCompiledPattern, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Route, getPaths, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Route, getReversedPaths, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Route, setHttpMethods, arginfo_dao_mvc_router_routeinterface_sethttpmethods, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Route, getHttpMethods, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Route, setPrefix, arginfo_dao_mvc_router_route_setprefix, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Route, getPrefix, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Route, setHostname, arginfo_dao_mvc_router_route_sethostname, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Route, getHostname, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Route, setGroup, arginfo_dao_mvc_router_route_setgroup, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Route, getGroup, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Route, convert, arginfo_dao_mvc_router_route_convert, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Route, getConverters, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Route, reset, arginfo_empty, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Mvc_Router_Route, setDefaults, arginfo_dao_mvc_router_routeinterface_setdefaults, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Route, getDefaults, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Route, getDefaultNamespace, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Route, getDefaultModule, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Route, getDefaultController, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Route, getDefaultAction, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Route, getDefaultParams, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Route, setUrlGenerator, arginfo_dao_mvc_router_route_seturlgenerator, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Route, getUrlGenerator, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Route, setCaseSensitive, arginfo_dao_mvc_router_route_setcasesensitive, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Route, getCaseSensitive, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Route, setMode, arginfo_dao_mvc_router_routeinterface_setmode, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Route, getMode, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Mvc\Router\Route initializer
 */
DAO_INIT_CLASS(Dao_Mvc_Router_Route){

	DAO_REGISTER_CLASS(Dao\\Mvc\\Router, Route, mvc_router_route, dao_mvc_router_route_method_entry, 0);

	zend_declare_property_null(dao_mvc_router_route_ce, SL("_pattern"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_router_route_ce, SL("_compiledPattern"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_router_route_ce, SL("_paths"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_router_route_ce, SL("_methods"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_router_route_ce, SL("_prefix"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_router_route_ce, SL("_hostname"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_router_route_ce, SL("_converters"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_router_route_ce, SL("_id"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_router_route_ce, SL("_name"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_router_route_ce, SL("_beforeMatch"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_router_route_ce, SL("_group"), ZEND_ACC_PROTECTED);
	zend_declare_property_long(dao_mvc_router_route_ce, SL("_uniqueId"), 0, ZEND_ACC_STATIC|ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_router_route_ce, SL("_defaultNamespace"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_router_route_ce, SL("_defaultModule"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_router_route_ce, SL("_defaultController"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_router_route_ce, SL("_defaultAction"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_router_route_ce, SL("_defaultParams"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_router_route_ce, SL("_urlGenerator"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_router_route_ce, SL("_caseSensitive"), ZEND_ACC_PROTECTED);
	zend_declare_property_bool(dao_mvc_router_route_ce, SL("_mode"), DAO_ROUTER_MODE_DEFAULT, ZEND_ACC_PROTECTED);

	zend_class_implements(dao_mvc_router_route_ce, 1, dao_mvc_router_routeinterface_ce);

	return SUCCESS;
}

/**
 * Dao\Mvc\Router\Route constructor
 *
 * @param string $pattern
 * @param array $paths
 * @param array|string $httpMethods
 */
PHP_METHOD(Dao_Mvc_Router_Route, __construct){

	zval *pattern, *paths = NULL, *http_methods = NULL, *regex = NULL, unique_id = {};

	dao_fetch_params(0, 1, 3, &pattern, &paths, &http_methods, &regex);

	if (!paths) {
		paths = &DAO_GLOBAL(z_null);
	}

	if (!http_methods) {
		http_methods = &DAO_GLOBAL(z_null);
	}

	if (!regex) {
		regex = &DAO_GLOBAL(z_null);
	}
#ifdef DAO_TREEROUTER
	dao_update_property(getThis(), SL("_paths"), paths);
#else
	/**
	 * Configure the route (extract parameters, paths, etc)
	 */
	DAO_CALL_METHOD(NULL, getThis(), "reconfigure", pattern, paths, regex);
#endif
	/**
	 * Update the HTTP method constraints
	 */
	dao_update_property(getThis(), SL("_methods"), http_methods);

	/**
	 * Get the unique Id from the static member _uniqueId
	 */
	dao_read_static_property_ce(&unique_id, dao_mvc_router_route_ce, SL("_uniqueId"), PH_READONLY);

	if (Z_TYPE(unique_id) == IS_NULL) {
		ZVAL_LONG(&unique_id, 0);
	}

	dao_update_property(getThis(), SL("_id"), &unique_id);

	/* increment_function() will increment the value of the static property as well */
	increment_function(&unique_id);
	dao_update_static_property_ce(dao_mvc_router_route_ce, SL("_uniqueId"), &unique_id);
}

/**
 * Replaces placeholders from pattern returning a valid PCRE regular expression
 *
 * @param string $pattern
 * @param array $paths
 * @return string
 */
PHP_METHOD(Dao_Mvc_Router_Route, compilePattern){

	zval *pattern, *regex = NULL, compiled_pattern = {}, id_pattern = {}, wildcard = {}, pattern_copy = {}, params_pattern = {};

	dao_fetch_params(0, 1, 1, &pattern, &regex);

	ZVAL_DUP(&compiled_pattern, pattern);

	if (!regex) {
		regex = &DAO_GLOBAL(z_null);
	}
	/**
	 * If a pattern contains ':', maybe there are placeholders to replace
	 */
	if (dao_memnstr_str(pattern, SL(":"))) {

		/**
		 * This is a pattern for valid identifiers
		 */
		ZVAL_STRING(&id_pattern, "/([\\w0-9\\_\\-]+)");

		/**
		 * Replace the module part
		 */
		if (dao_memnstr_str(pattern, SL("/:module"))) {
			ZVAL_STRING(&wildcard, "/:module");

			ZVAL_COPY_VALUE(&pattern_copy, &compiled_pattern);

			if (Z_TYPE_P(regex) == IS_ARRAY && dao_array_isset_fetch_str(&params_pattern, regex, SL("/:module"), PH_READONLY)) {
				DAO_STR_REPLACE(&compiled_pattern, &wildcard, &params_pattern, &pattern_copy);
			} else {
				DAO_STR_REPLACE(&compiled_pattern, &wildcard, &id_pattern, &pattern_copy);
			}
			zval_ptr_dtor(&wildcard);
			zval_ptr_dtor(&pattern_copy);
		}

		/**
		 * Replace the controller placeholder
		 */
		if (dao_memnstr_str(pattern, SL("/:controller"))) {
			ZVAL_STRING(&wildcard, "/:controller");

			ZVAL_COPY_VALUE(&pattern_copy, &compiled_pattern);

			if (Z_TYPE_P(regex) == IS_ARRAY && dao_array_isset_fetch_str(&params_pattern, regex, SL("/:controller"), PH_READONLY)) {
				DAO_STR_REPLACE(&compiled_pattern, &wildcard, &params_pattern, &pattern_copy);
			} else {
				DAO_STR_REPLACE(&compiled_pattern, &wildcard, &id_pattern, &pattern_copy);
			}
			zval_ptr_dtor(&wildcard);
			zval_ptr_dtor(&pattern_copy);
		}

		/**
		 * Replace the namespace placeholder
		 */
		if (dao_memnstr_str(pattern, SL("/:namespace"))) {
			ZVAL_STRING(&wildcard, "/:namespace");

			ZVAL_COPY_VALUE(&pattern_copy, &compiled_pattern);

			if (Z_TYPE_P(regex) == IS_ARRAY && dao_array_isset_fetch_str(&params_pattern, regex, SL("/:namespace"), PH_READONLY)) {
				DAO_STR_REPLACE(&compiled_pattern, &wildcard, &params_pattern, &pattern_copy);
			} else {
				DAO_STR_REPLACE(&compiled_pattern, &wildcard, &id_pattern, &pattern_copy);
			}
			zval_ptr_dtor(&wildcard);
			zval_ptr_dtor(&pattern_copy);
		}

		/**
		 * Replace the action placeholder
		 */
		if (dao_memnstr_str(pattern, SL("/:action"))) {
			ZVAL_STRING(&wildcard, "/:action");

			ZVAL_COPY_VALUE(&pattern_copy, &compiled_pattern);

			if (Z_TYPE_P(regex) == IS_ARRAY && dao_array_isset_fetch_str(&params_pattern, regex, SL("/:action"), PH_READONLY)) {
				DAO_STR_REPLACE(&compiled_pattern, &wildcard, &params_pattern, &pattern_copy);
			} else {
				DAO_STR_REPLACE(&compiled_pattern, &wildcard, &id_pattern, &pattern_copy);
			}
			zval_ptr_dtor(&wildcard);
			zval_ptr_dtor(&pattern_copy);
		}

		/**
		 * Replace the params placeholder
		 */
		if (dao_memnstr_str(pattern, SL("/:params"))) {
			ZVAL_STRING(&wildcard, "/:params");

			ZVAL_COPY_VALUE(&pattern_copy, &compiled_pattern);

			if (Z_TYPE_P(regex) == IS_ARRAY && dao_array_isset_fetch_str(&params_pattern, regex, SL("/:params"), PH_READONLY)) {
				DAO_STR_REPLACE(&compiled_pattern, &wildcard, &params_pattern, &pattern_copy);
			} else {
				zval_ptr_dtor(&id_pattern);
				ZVAL_STRING(&id_pattern, "(/.*)*");
				DAO_STR_REPLACE(&compiled_pattern, &wildcard, &id_pattern, &pattern_copy);
			}
			zval_ptr_dtor(&wildcard);
			zval_ptr_dtor(&pattern_copy);
		}

		/**
		 * Replace the int placeholder
		 */
		if (dao_memnstr_str(pattern, SL("/:int"))) {
			ZVAL_STRING(&wildcard, "/:int");

			ZVAL_COPY_VALUE(&pattern_copy, &compiled_pattern);

			if (Z_TYPE_P(regex) == IS_ARRAY && dao_array_isset_fetch_str(&params_pattern, regex, SL("/:int"), PH_READONLY)) {
				DAO_STR_REPLACE(&compiled_pattern, &wildcard, &params_pattern, &pattern_copy);
			} else {
				zval_ptr_dtor(&id_pattern);
				ZVAL_STRING(&id_pattern, "/([0-9]+)");
				DAO_STR_REPLACE(&compiled_pattern, &wildcard, &id_pattern, &pattern_copy);
			}
			zval_ptr_dtor(&wildcard);
			zval_ptr_dtor(&pattern_copy);
		}

		zval_ptr_dtor(&id_pattern);
	}

	/**
	 * Check if the pattern has parantheses in order to add the regex delimiters
	 */
	if (dao_memnstr_str(&compiled_pattern, SL("("))) {
		DAO_CONCAT_SVS(return_value, "#^", &compiled_pattern, "$#u");
		zval_ptr_dtor(&compiled_pattern);
		return;
	}

	/**
	 * Square brackets are also checked
	 */
	if (dao_memnstr_str(&compiled_pattern, SL("["))) {
		DAO_CONCAT_SVS(return_value, "#^", &compiled_pattern, "$#u");
		zval_ptr_dtor(&compiled_pattern);
		return;
	}

	/**
	 * Square brackets are also checked
	 */
	if (dao_memnstr_str(&compiled_pattern, SL("{"))) {
		DAO_CONCAT_SVS(return_value, "#^", &compiled_pattern, "$#u");
		zval_ptr_dtor(&compiled_pattern);
		return;
	}

	RETURN_CTOR(&compiled_pattern);
}

/**
 * Set one or more HTTP methods that constraint the matching of the route
 *
 *<code>
 * $route->via('GET');
 * $route->via(array('GET', 'POST'));
 *</code>
 *
 * @param string|array $httpMethods
 * @return Dao\Mvc\Router\Route
 */
PHP_METHOD(Dao_Mvc_Router_Route, via){

	zval *http_methods;

	dao_fetch_params(0, 1, 0, &http_methods);

	dao_update_property(getThis(), SL("_methods"), http_methods);
	RETURN_THIS();
}

/**
 * Reconfigure the route adding a new pattern and a set of paths
 *
 * @param string $pattern
 * @param string|array $paths
 */
PHP_METHOD(Dao_Mvc_Router_Route, reConfigure){

	zval *pattern, *paths = NULL, *regex = NULL, module_name = {}, controller_name = {}, action_name = {}, parts = {}, number_parts = {}, route_paths = {};
	zval real_class_name = {}, namespace_name = {}, lower_name = {}, pcre_pattern = {}, compiled_pattern = {}, debug_message = {};

	dao_fetch_params(0, 1, 2, &pattern, &paths, &regex);

	if (!paths) {
		paths = &DAO_GLOBAL(z_null);
	}

	if (!regex) {
		regex = &DAO_GLOBAL(z_null);
	}

	if (Z_TYPE_P(paths) != IS_NULL) {
		if (Z_TYPE_P(paths) == IS_STRING) {
			/**
			 * Explode the short paths using the :: separator
			 */
			dao_fast_explode_str(&parts, SL("::"), paths);
			dao_fast_count(&number_parts, &parts);

			/**
			 * Create the array paths dynamically
			 */

			switch (dao_get_intval(&number_parts)) {

				case 3:
					dao_array_fetch_long(&module_name, &parts, 0, PH_NOISY | PH_READONLY);
					dao_array_fetch_long(&controller_name, &parts, 1, PH_NOISY | PH_READONLY);
					dao_array_fetch_long(&action_name, &parts, 2, PH_NOISY | PH_READONLY);
					break;

				case 2:
					dao_array_fetch_long(&controller_name, &parts, 0, PH_NOISY | PH_READONLY);
					dao_array_fetch_long(&action_name, &parts, 1, PH_NOISY | PH_READONLY);
					break;

				case 1:
					dao_array_fetch_long(&controller_name, &parts, 0, PH_NOISY|PH_READONLY);
					break;

			}

			array_init(&route_paths);

			/**
			 * Process module name
			 */
			if (DAO_IS_NOT_EMPTY_STRING(&module_name)) {
				dao_array_update_str(&route_paths, SL("module"), &module_name, PH_COPY);
			}

			/**
			 * Process controller name
			 */
			if (DAO_IS_NOT_EMPTY_STRING(&controller_name)) {

				/**
				 * Check if we need to obtain the namespace
				 */
				if (dao_memnstr_str(&controller_name, SL("\\"))) {

					/**
					 * Extract the real class name from the namespaced class
					 */
					dao_get_class_ns(&real_class_name, &controller_name, 0);

					/**
					 * Extract the namespace from the namespaced class
					 */
					dao_get_ns_class(&namespace_name, &controller_name, 0);

					/**
					 * Update the namespace
					 */
					if (DAO_IS_NOT_EMPTY_STRING(&namespace_name)) {
						dao_array_update_str(&route_paths, SL("namespace"), &namespace_name, 0);
					}
				} else {
					ZVAL_COPY(&real_class_name, &controller_name);
				}

				/**
				 * Always pass the controller to lowercase
				 */
				dao_uncamelize(&lower_name, &real_class_name);
				zval_ptr_dtor(&real_class_name);

				/**
				 * Update the controller path
				 */
				dao_array_update_str(&route_paths, SL("controller"), &lower_name, 0);
			}

			/**
			 * Process action name
			 */
			if (DAO_IS_NOT_EMPTY_STRING(&action_name)) {
				dao_array_update_str(&route_paths, SL("action"), &action_name, PH_COPY);
			}
			zval_ptr_dtor(&parts);
		} else if (Z_TYPE_P(paths) == IS_ARRAY) {
			if (unlikely(DAO_GLOBAL(debug).enable_debug)) {
				ZVAL_STRING(&debug_message, "Add Route paths: ");
				DAO_DEBUG_LOG(&debug_message);
				zval_ptr_dtor(&debug_message);
				DAO_DEBUG_LOG(paths);
			}

			ZVAL_DUP(&route_paths, paths);
			if (dao_array_isset_fetch_str(&controller_name, &route_paths, SL("controller"), PH_READONLY)) {
				if (Z_TYPE(controller_name) == IS_STRING && !dao_is_numeric_ex(&controller_name)) {
					dao_uncamelize(&lower_name, &controller_name);
					dao_array_update_str(&route_paths, SL("controller"), &lower_name, 0);
				}
			}
		} else {
			DAO_THROW_EXCEPTION_STR(dao_mvc_router_exception_ce, "The route contains invalid paths");
			return;
		}
	} else {
		array_init(&route_paths);
	}

	/**
	 * If the route starts with '#' we assume that it is a regular expression
	 */
	if (!dao_start_with_str(pattern, SL("#"))) {
		/**
		 * Transform the route's pattern to a regular expression
		 */
		DAO_CALL_METHOD(&pcre_pattern, getThis(), "compilepattern", pattern, regex);
		if (dao_memnstr_str(pattern, SL("{"))) {
			/**
			 * The route has named parameters so we need to extract them
			 */
			dao_extract_named_params(&compiled_pattern, &pcre_pattern, &route_paths);
		} else {
			ZVAL_COPY(&compiled_pattern, &pcre_pattern);
		}
		zval_ptr_dtor(&pcre_pattern);
	} else {
		ZVAL_COPY(&compiled_pattern, pattern);
	}

	/**
	 * Update the original pattern
	 */
	dao_update_property(getThis(), SL("_pattern"), pattern);

	/**
	 * Update the compiled pattern
	 */
	dao_update_property(getThis(), SL("_compiledPattern"), &compiled_pattern);
	zval_ptr_dtor(&compiled_pattern);

	/**
	 * Update the route's paths
	 */
	dao_update_property(getThis(), SL("_paths"), &route_paths);
	if (unlikely(DAO_GLOBAL(debug).enable_debug)) {
		ZVAL_STRING(&debug_message, "Update Route paths: ");
		DAO_DEBUG_LOG(&debug_message);
		zval_ptr_dtor(&debug_message);
		DAO_DEBUG_LOG(&route_paths);
	}
	zval_ptr_dtor(&route_paths);
}

/**
 * Returns the route's name
 *
 * @return string
 */
PHP_METHOD(Dao_Mvc_Router_Route, getName){


	RETURN_MEMBER(getThis(), "_name");
}

/**
 * Sets the route's name
 *
 *<code>
 * $router->add('/about', array(
 *     'controller' => 'about'
 * ))->setName('about');
 *</code>
 *
 * @param string $name
 * @return Dao\Mvc\Router\Route
 */
PHP_METHOD(Dao_Mvc_Router_Route, setName){

	zval *name;

	dao_fetch_params(0, 1, 0, &name);

	dao_update_property(getThis(), SL("_name"), name);
	RETURN_THIS();
}

/**
 * Sets a callback that is called if the route is matched.
 * The developer can implement any arbitrary conditions here
 * If the callback returns false the route is treaded as not matched
 *
 * @param callback $callback
 * @return Dao\Mvc\Router\Route
 */
PHP_METHOD(Dao_Mvc_Router_Route, beforeMatch){

	zval *callback;

	dao_fetch_params(0, 1, 0, &callback);

	dao_update_property(getThis(), SL("_beforeMatch"), callback);
	RETURN_THIS();
}

/**
 * Returns the 'before match' callback if any
 *
 * @return mixed
 */
PHP_METHOD(Dao_Mvc_Router_Route, getBeforeMatch){


	RETURN_MEMBER(getThis(), "_beforeMatch");
}

/**
 * Returns the route's id
 *
 * @return string
 */
PHP_METHOD(Dao_Mvc_Router_Route, getRouteId){


	RETURN_MEMBER(getThis(), "_id");
}

/**
 * Returns the route's pattern
 *
 * @return string
 */
PHP_METHOD(Dao_Mvc_Router_Route, getPattern){


	RETURN_MEMBER(getThis(), "_pattern");
}

/**
 * Returns the route's compiled pattern
 *
 * @return string
 */
PHP_METHOD(Dao_Mvc_Router_Route, getCompiledPattern){


	RETURN_MEMBER(getThis(), "_compiledPattern");
}

/**
 * Returns the paths
 *
 * @return array
 */
PHP_METHOD(Dao_Mvc_Router_Route, getPaths){


	RETURN_MEMBER(getThis(), "_paths");
}

/**
 * Returns the paths using positions as keys and names as values
 *
 * @return array
 */
PHP_METHOD(Dao_Mvc_Router_Route, getReversedPaths){

	zval paths = {}, *position;
	zend_string *str_key;
	ulong idx;

	dao_read_property(&paths, getThis(), SL("_paths"), PH_NOISY|PH_READONLY);

	array_init(return_value);

	ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL(paths), idx, str_key, position) {
		zval path = {};
		if (str_key) {
			ZVAL_STR(&path, str_key);
		} else {
			ZVAL_LONG(&path, idx);
		}
		dao_array_update(return_value, position, &path, PH_COPY);
	} ZEND_HASH_FOREACH_END();
}

/**
 * Sets a set of HTTP methods that constraint the matching of the route (alias of via)
 *
 *<code>
 * $route->setHttpMethods('GET');
 * $route->setHttpMethods(array('GET', 'POST'));
 *</code>
 *
 * @param string|array $httpMethods
 * @return Dao\Mvc\Router\Route
 */
PHP_METHOD(Dao_Mvc_Router_Route, setHttpMethods){

	zval *http_methods;

	dao_fetch_params(0, 1, 0, &http_methods);

	dao_update_property(getThis(), SL("_methods"), http_methods);
	RETURN_THIS();
}

/**
 * Returns the HTTP methods that constraint matching the route
 *
 * @return string|array
 */
PHP_METHOD(Dao_Mvc_Router_Route, getHttpMethods){


	RETURN_MEMBER(getThis(), "_methods");
}

/**
 * Set a common uri prefix for the route
 *
 * @param string $prefix
 * @return Dao\Mvc\Router\Route
 */
PHP_METHOD(Dao_Mvc_Router_Route, setPrefix){

	zval *prefix;

	dao_fetch_params(0, 1, 0, &prefix);

	dao_update_property(getThis(), SL("_prefix"), prefix);
	RETURN_THIS();
}

/**
 * Returns the common prefix for the route
 *
 * @return string
 */
PHP_METHOD(Dao_Mvc_Router_Route, getPrefix){


	RETURN_MEMBER(getThis(), "_prefix");
}

/**
 * Sets a hostname restriction to the route
 *
 *<code>
 * $route->setHostname('localhost');
 *</code>
 *
 * @param string|array $httpMethods
 * @return Dao\Mvc\Router\Route
 */
PHP_METHOD(Dao_Mvc_Router_Route, setHostname){

	zval *hostname;

	dao_fetch_params(0, 1, 0, &hostname);

	dao_update_property(getThis(), SL("_hostname"), hostname);
	RETURN_THIS();
}

/**
 * Returns the hostname restriction if any
 *
 * @return string
 */
PHP_METHOD(Dao_Mvc_Router_Route, getHostname){


	RETURN_MEMBER(getThis(), "_hostname");
}

/**
 * Sets the group associated with the route
 *
 * @param Dao\Mvc\Router\Group $group
 * @return Dao\Mvc\RouteInterface
 */
PHP_METHOD(Dao_Mvc_Router_Route, setGroup) {

	zval *group;

	dao_fetch_params(0, 1, 0, &group);
	DAO_VERIFY_CLASS_EX(group, dao_mvc_router_group_ce, dao_mvc_router_exception_ce);

	dao_update_property(getThis(), SL("_group"), group);
	RETURN_THIS();
}

/**
 * Returns the group associated with the route
 *
 * @return Dao\Mvc\Router\Group|null
 */
PHP_METHOD(Dao_Mvc_Router_Route, getGroup) {

	RETURN_MEMBER(getThis(), "_group");
}

/**
 * Adds a converter to perform an additional transformation for certain parameter
 *
 * @param string $name
 * @param callable $converter
 * @return Dao\Mvc\Router\Route
 */
PHP_METHOD(Dao_Mvc_Router_Route, convert){

	zval *name, *converter, callback = {};

	dao_fetch_params(0, 2, 0, &name, &converter);

	DAO_CALL_CE_STATIC(&callback, zend_ce_closure, "bind", converter, getThis());
	dao_update_property_array(getThis(), SL("_converters"), name, &callback);
	zval_ptr_dtor(&callback);
	RETURN_THIS();
}

/**
 * Returns the router converter
 *
 * @return array
 */
PHP_METHOD(Dao_Mvc_Router_Route, getConverters){


	RETURN_MEMBER(getThis(), "_converters");
}

/**
 * Resets the internal route id generator
 */
PHP_METHOD(Dao_Mvc_Router_Route, reset){

	zend_update_static_property_long(dao_mvc_router_route_ce, SL("_uniqueId"), 0);
}

/**
 * Sets an array of default paths.
 *
 * @param array $defaults
 * @return Dao\Mvc\Router\Route
 */
PHP_METHOD(Dao_Mvc_Router_Route, setDefaults){

	zval *defaults, namespace_name = {}, module_name = {}, controller_name = {}, action_name = {}, params = {};

	dao_fetch_params(0, 1, 0, &defaults);

	/* Set the default namespace */
	if (dao_array_isset_fetch_str(&namespace_name, defaults, SL("namespace"), PH_READONLY)) {
		if (Z_TYPE(namespace_name) != IS_STRING) {
			DAO_THROW_EXCEPTION_STR(dao_mvc_router_exception_ce, "Namespace name must be an string");
			return;
		}
		dao_update_property(getThis(), SL("_defaultNamespace"), &namespace_name);
	}

	/* Set the default module */
	if (dao_array_isset_fetch_str(&module_name, defaults, SL("module"), PH_READONLY)) {
		if (Z_TYPE(module_name) != IS_STRING) {
			DAO_THROW_EXCEPTION_STR(dao_mvc_router_exception_ce, "Module name must be an string");
			return;
		}
		dao_update_property(getThis(), SL("_defaultModule"), &module_name);
	}

	/* Set the default controller */
	if (dao_array_isset_fetch_str(&controller_name, defaults, SL("controller"), PH_READONLY)) {
		if (Z_TYPE(controller_name) != IS_STRING) {
			DAO_THROW_EXCEPTION_STR(dao_mvc_router_exception_ce, "Controller name must be an string");
			return;
		}
		dao_update_property(getThis(), SL("_defaultController"), &controller_name);
	}

	/* Set the default action */
	if (dao_array_isset_fetch_str(&action_name, defaults, SL("action"), PH_READONLY)) {
		if (Z_TYPE(action_name) != IS_STRING) {
			DAO_THROW_EXCEPTION_STR(dao_mvc_router_exception_ce, "Action name must be an string");
			return;
		}
		dao_update_property(getThis(), SL("_defaultAction"), &action_name);
	}

	/* Set default parameters */
	if (dao_array_isset_fetch_str(&params, defaults, SL("params"), PH_READONLY)) {
		if (Z_TYPE(params) != IS_ARRAY) {
			DAO_THROW_EXCEPTION_STR(dao_mvc_router_exception_ce, "Params must be an array");
			return;
		}
		dao_update_property(getThis(), SL("_defaultParams"), &params);
	}

	RETURN_THIS();
}

/**
 * Returns an array of default parameters
 *
 * @return array
 */
PHP_METHOD(Dao_Mvc_Router_Route, getDefaults){

	zval namespace_name = {}, module_name = {}, controller_name = {}, action_name = {}, params = {};

	 dao_read_property(&namespace_name, getThis(), SL("_defaultNamespace"), PH_NOISY|PH_READONLY);
	 dao_read_property(&module_name, getThis(), SL("_defaultModule"), PH_NOISY|PH_READONLY);
	 dao_read_property(&controller_name, getThis(), SL("_defaultController"), PH_NOISY|PH_READONLY);
	 dao_read_property(&action_name, getThis(), SL("_defaultAction"), PH_NOISY|PH_READONLY);
	 dao_read_property(&params, getThis(), SL("_defaultParams"), PH_NOISY|PH_READONLY);

	array_init_size(return_value, 5);

	dao_array_update_string(return_value, IS(namespace),  &namespace_name,  PH_COPY);
	dao_array_update_string(return_value, IS(module),     &module_name,     PH_COPY);
	dao_array_update_string(return_value, IS(controller), &controller_name, PH_COPY);
	dao_array_update_string(return_value, IS(action),     &action_name,     PH_COPY);
	dao_array_update_string(return_value, IS(params),     &params,          PH_COPY);
}

/**
 * Returns the name of the default namespace
 *
 * @return string
 */
PHP_METHOD(Dao_Mvc_Router_Route, getDefaultNamespace){

	RETURN_MEMBER(getThis(), "_defaultNamespace");
}

/**
 * Returns the name of the default module
 *
 * @return string
 */
PHP_METHOD(Dao_Mvc_Router_Route, getDefaultModule){

	RETURN_MEMBER(getThis(), "_defaultModule");
}

/**
 * Returns the default controller name
 *
 * @return string
 */
PHP_METHOD(Dao_Mvc_Router_Route, getDefaultController){

	RETURN_MEMBER(getThis(), "_defaultController");
}

/**
 * Returns the default action name
 *
 * @return string
 */
PHP_METHOD(Dao_Mvc_Router_Route, getDefaultAction){

	RETURN_MEMBER(getThis(), "_defaultAction");
}

/**
 * Returns the default params
 *
 * @return mixed
 */
PHP_METHOD(Dao_Mvc_Router_Route, getDefaultParams){

	RETURN_MEMBER(getThis(), "_defaultParams");
}

/**
 * Sets the Url Generator
 * @param callable $generator
 * @return string
 */
PHP_METHOD(Dao_Mvc_Router_Route, setUrlGenerator){

	zval *generator, callback = {};

	dao_fetch_params(0, 1, 0, &generator);

	DAO_CALL_CE_STATIC(&callback, zend_ce_closure, "bind", generator, getThis());
	dao_update_property(getThis(), SL("_urlGenerator"), &callback);

	RETURN_THIS();
}

/**
 * Returns the Url Generator
 *
 * @return callable
 */
PHP_METHOD(Dao_Mvc_Router_Route, getUrlGenerator){

	RETURN_MEMBER(getThis(), "_urlGenerator");
}

/**
 * Sets the case sensitive
 * @param boolean $caseSensitive
 * @return string
 */
PHP_METHOD(Dao_Mvc_Router_Route, setCaseSensitive){

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
PHP_METHOD(Dao_Mvc_Router_Route, getCaseSensitive){

	RETURN_MEMBER(getThis(), "_caseSensitive");
}

/**
 * Sets the mode
 *
 * @param int $mode
 */
PHP_METHOD(Dao_Mvc_Router_Route, setMode){

	zval *mode;

	dao_fetch_params(0, 1, 0, &mode);

	dao_update_property(getThis(), SL("_mode"), mode);
	RETURN_THIS();
}

/**
 * Gets the mode
 *
 * @param int|null $mode
 */
PHP_METHOD(Dao_Mvc_Router_Route, getMode){


	RETURN_MEMBER(getThis(), "_mode");
}
