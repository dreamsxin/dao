
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

#include "mvc/router/group.h"
#include "mvc/router/route.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/string.h"
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/concat.h"
#include "kernel/array.h"

#include "interned-strings.h"

/**
 * Dao\Mvc\Router\Group
 *
 * Helper class to create a group of routes with common attributes
 *
 *<code>
 * $router = new Dao\Mvc\Router();
 *
 * //Create a group with a common module and controller
 * $blog = new Dao\Mvc\Router\Group(array(
 * 	'module' => 'blog',
 * 	'controller' => 'index'
 * ));
 *
 * //All the routes start with /blog
 * $blog->setPrefix('/blog');
 *
 * //Add a route to the group
 * $blog->add('/save', array(
 * 	'action' => 'save'
 * ));
 *
 * //Add another route to the group
 * $blog->add('/edit/{id}', array(
 * 	'action' => 'edit'
 * ));
 *
 * //This route maps to a controller different than the default
 * $blog->add('/blog', array(
 * 	'controller' => 'about',
 * 	'action' => 'index'
 * ));
 *
 * //Add the group to the router
 * $router->mount($blog);
 *</code>
 *
 */
zend_class_entry *dao_mvc_router_group_ce;

PHP_METHOD(Dao_Mvc_Router_Group, __construct);
PHP_METHOD(Dao_Mvc_Router_Group, setHostname);
PHP_METHOD(Dao_Mvc_Router_Group, getHostname);
PHP_METHOD(Dao_Mvc_Router_Group, setPrefix);
PHP_METHOD(Dao_Mvc_Router_Group, getPrefix);
PHP_METHOD(Dao_Mvc_Router_Group, beforeMatch);
PHP_METHOD(Dao_Mvc_Router_Group, getBeforeMatch);
PHP_METHOD(Dao_Mvc_Router_Group, setPaths);
PHP_METHOD(Dao_Mvc_Router_Group, getPaths);
PHP_METHOD(Dao_Mvc_Router_Group, getRoutes);
PHP_METHOD(Dao_Mvc_Router_Group, _addRoute);
PHP_METHOD(Dao_Mvc_Router_Group, add);
PHP_METHOD(Dao_Mvc_Router_Group, addGet);
PHP_METHOD(Dao_Mvc_Router_Group, addPost);
PHP_METHOD(Dao_Mvc_Router_Group, addPut);
PHP_METHOD(Dao_Mvc_Router_Group, addPatch);
PHP_METHOD(Dao_Mvc_Router_Group, addDelete);
PHP_METHOD(Dao_Mvc_Router_Group, addOptions);
PHP_METHOD(Dao_Mvc_Router_Group, addHead);
PHP_METHOD(Dao_Mvc_Router_Group, clear);
PHP_METHOD(Dao_Mvc_Router_Group, convert);
PHP_METHOD(Dao_Mvc_Router_Group, getConverters);
PHP_METHOD(Dao_Mvc_Router_Group, setName);
PHP_METHOD(Dao_Mvc_Router_Group, getName);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_router_group___construct, 0, 0, 0)
	ZEND_ARG_INFO(0, paths)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_router_group_sethostname, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, hostname, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_router_group_setprefix, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, prefix, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_router_group_beforematch, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, callback, Closure, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_router_group_setpaths, 0, 0, 1)
	ZEND_ARG_INFO(0, paths)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_router_group_add, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, pattern, IS_STRING, 0)
	ZEND_ARG_INFO(0, paths)
	ZEND_ARG_INFO(0, httpMethods)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_router_group_addget, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, pattern, IS_STRING, 0)
	ZEND_ARG_INFO(0, paths)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_router_group_addpost, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, pattern, IS_STRING, 0)
	ZEND_ARG_INFO(0, paths)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_router_group_addput, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, pattern, IS_STRING, 0)
	ZEND_ARG_INFO(0, paths)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_router_group_addpatch, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, pattern, IS_STRING, 0)
	ZEND_ARG_INFO(0, paths)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_router_group_adddelete, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, pattern, IS_STRING, 0)
	ZEND_ARG_INFO(0, paths)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_router_group_addoptions, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, pattern, IS_STRING, 0)
	ZEND_ARG_INFO(0, paths)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_router_group_addhead, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, pattern, IS_STRING, 0)
	ZEND_ARG_INFO(0, paths)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_router_group_convert, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, converter, Closure, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_router_group_setname, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_mvc_router_group_method_entry[] = {
	PHP_ME(Dao_Mvc_Router_Group, __construct, arginfo_dao_mvc_router_group___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Mvc_Router_Group, setHostname, arginfo_dao_mvc_router_group_sethostname, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Group, getHostname, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Group, setPrefix, arginfo_dao_mvc_router_group_setprefix, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Group, getPrefix, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Group, beforeMatch, arginfo_dao_mvc_router_group_beforematch, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Group, getBeforeMatch, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Group, setPaths, arginfo_dao_mvc_router_group_setpaths, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Group, getPaths, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Group, getRoutes, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Group, _addRoute, arginfo_empty, ZEND_ACC_PROTECTED)
	PHP_ME(Dao_Mvc_Router_Group, add, arginfo_dao_mvc_router_group_add, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Group, addGet, arginfo_dao_mvc_router_group_addget, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Group, addPost, arginfo_dao_mvc_router_group_addpost, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Group, addPut, arginfo_dao_mvc_router_group_addput, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Group, addPatch, arginfo_dao_mvc_router_group_addpatch, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Group, addDelete, arginfo_dao_mvc_router_group_adddelete, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Group, addOptions, arginfo_dao_mvc_router_group_addoptions, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Group, addHead, arginfo_dao_mvc_router_group_addhead, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Group, clear, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Group, convert, arginfo_dao_mvc_router_group_convert, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Group, getConverters, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Group, setName, arginfo_dao_mvc_router_group_setname, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Router_Group, getName, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Mvc\Router\Group initializer
 */
DAO_INIT_CLASS(Dao_Mvc_Router_Group){

	DAO_REGISTER_CLASS(Dao\\Mvc\\Router, Group, mvc_router_group, dao_mvc_router_group_method_entry, 0);

	zend_declare_property_null(dao_mvc_router_group_ce, SL("_prefix"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_router_group_ce, SL("_hostname"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_router_group_ce, SL("_paths"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_router_group_ce, SL("_routes"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_router_group_ce, SL("_beforeMatch"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_router_group_ce, SL("_converters"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_router_group_ce, SL("_name"), ZEND_ACC_PROTECTED);

	return SUCCESS;
}

/**
 * Dao\Mvc\Router\Group constructor
 *
 * @param array $paths
 */
PHP_METHOD(Dao_Mvc_Router_Group, __construct){

	zval *paths = NULL;

	dao_fetch_params(0, 0, 1, &paths);

	if (!paths) {
		paths = &DAO_GLOBAL(z_null);
	}

	if (Z_TYPE_P(paths) == IS_ARRAY || Z_TYPE_P(paths) == IS_STRING) {
		dao_update_property(getThis(), SL("_paths"), paths);
	}

	if (dao_method_exists_ex(getThis(), SL("initialize")) == SUCCESS) {
		DAO_CALL_METHOD(NULL, getThis(), "initialize", paths);
	}
}

/**
 * Set a hostname restriction for all the routes in the group
 *
 * @param string $hostname
 * @return Dao\Mvc\Router\Group
 */
PHP_METHOD(Dao_Mvc_Router_Group, setHostname){

	zval *hostname;

	dao_fetch_params(0, 1, 0, &hostname);

	dao_update_property(getThis(), SL("_hostname"), hostname);
	RETURN_THIS();
}

/**
 * Returns the hostname restriction
 *
 * @return string
 */
PHP_METHOD(Dao_Mvc_Router_Group, getHostname){


	RETURN_MEMBER(getThis(), "_hostname");
}

/**
 * Set a common uri prefix for all the routes in this group
 *
 * @param string $prefix
 * @return Dao\Mvc\Router\Group
 */
PHP_METHOD(Dao_Mvc_Router_Group, setPrefix){

	zval *prefix;

	dao_fetch_params(0, 1, 0, &prefix);

	dao_update_property(getThis(), SL("_prefix"), prefix);
	RETURN_THIS();
}

/**
 * Returns the common prefix for all the routes
 *
 * @return string
 */
PHP_METHOD(Dao_Mvc_Router_Group, getPrefix){


	RETURN_MEMBER(getThis(), "_prefix");
}

/**
 * Set a before-match condition for the whole group
 *
 * @param string $prefix
 * @return Dao\Mvc\Router\Group
 */
PHP_METHOD(Dao_Mvc_Router_Group, beforeMatch){

	zval *before_match;

	dao_fetch_params(0, 1, 0, &before_match);

	dao_update_property(getThis(), SL("_beforeMatch"), before_match);
	RETURN_THIS();
}

/**
 * Returns the before-match condition if any
 *
 * @return string
 */
PHP_METHOD(Dao_Mvc_Router_Group, getBeforeMatch){


	RETURN_MEMBER(getThis(), "_beforeMatch");
}

/**
 * Set common paths for all the routes in the group
 *
 * @param array $paths
 * @return Dao\Mvc\Router\Group
 */
PHP_METHOD(Dao_Mvc_Router_Group, setPaths){

	zval *paths;

	dao_fetch_params(0, 1, 0, &paths);

	dao_update_property(getThis(), SL("_paths"), paths);

}

/**
 * Returns the common paths defined for this group
 *
 * @return array|string
 */
PHP_METHOD(Dao_Mvc_Router_Group, getPaths){


	RETURN_MEMBER(getThis(), "_paths");
}

/**
 * Returns the routes added to the group
 *
 * @return Dao\Mvc\Router\Route[]
 */
PHP_METHOD(Dao_Mvc_Router_Group, getRoutes){


	RETURN_MEMBER(getThis(), "_routes");
}

/**
 * Adds a route applying the common attributes
 *
 * @param string $pattern
 * @param array $paths
 * @param array $httpMethods
 * @return Dao\Mvc\Router\Route
 */
PHP_METHOD(Dao_Mvc_Router_Group, _addRoute){

	zval *pattern, *paths = NULL, *http_methods = NULL, prefix = {}, prefix_pattern = {}, default_paths = {}, merged_paths = {};

	dao_fetch_params(0, 1, 2, &pattern, &paths, &http_methods);
	DAO_ENSURE_IS_STRING(pattern);

	if (!paths) {
		paths = &DAO_GLOBAL(z_null);
	}

	if (!http_methods) {
		http_methods = &DAO_GLOBAL(z_null);
	}

	dao_read_property(&prefix, getThis(), SL("_prefix"), PH_NOISY|PH_READONLY);

	if (Z_TYPE(prefix) != IS_STRING) {
		convert_to_string_ex(&prefix);
	}

	/**
	 * Add the prefix to the pattern
	 */
	{
		const char *s_pattern = Z_STRVAL_P(pattern); /* NUL-terminated */
		const char *s_prefix  = Z_STRVAL(prefix);   /* NUL-terminated */
		int pattern_len       = Z_STRLEN_P(pattern);
		int prefix_len        = Z_STRLEN(prefix);
		if (prefix_len && *s_pattern == '/' && s_prefix[prefix_len-1] == '/') {
			char *new_pattern = safe_emalloc(prefix_len - 1 /* slash */ + 1 /* \0 */, 1, pattern_len);
			memcpy(new_pattern, s_prefix, prefix_len - 1);
			memcpy(new_pattern + prefix_len - 1, s_pattern, pattern_len + 1);
			ZVAL_STRINGL(&prefix_pattern, new_pattern, prefix_len + pattern_len - 1);
		} else {
			DAO_CONCAT_VV(&prefix_pattern, &prefix, pattern);
		}
	}

	dao_read_property(&default_paths, getThis(), SL("_paths"), PH_NOISY|PH_READONLY);

	/**
	 * Check if the paths need to be merged with current paths
	 */
	if (Z_TYPE(default_paths) == IS_ARRAY && Z_TYPE_P(paths) == IS_ARRAY) {
		/**
		 * Merge the paths with the default paths
		 */
		dao_fast_array_merge(&merged_paths, &default_paths, paths);
	} else {
		ZVAL_COPY_VALUE(&merged_paths, paths);
	}

	/**
	 * Every route is internally stored as a Dao\Mvc\Router\Route
	 */
	object_init_ex(return_value, dao_mvc_router_route_ce);
	DAO_CALL_METHOD(NULL, return_value, "__construct", &prefix_pattern, &merged_paths, http_methods);
	DAO_CALL_METHOD(NULL, return_value, "setgroup", getThis());

	dao_update_property_array_append(getThis(), SL("_routes"), return_value);
}

/**
 * Adds a route to the router on any HTTP method
 *
 *<code>
 * $router->add('/about', 'About::index');
 *</code>
 *
 * @param string $pattern
 * @param string/array $paths
 * @param string $httpMethods
 * @return Dao\Mvc\Router\Route
 */
PHP_METHOD(Dao_Mvc_Router_Group, add){

	zval *pattern, *paths = NULL, *http_methods = NULL;

	dao_fetch_params(0, 1, 2, &pattern, &paths, &http_methods);

	if (!paths) {
		paths = &DAO_GLOBAL(z_null);
	}

	if (!http_methods) {
		http_methods = &DAO_GLOBAL(z_null);
	}

	DAO_RETURN_CALL_METHOD(getThis(), "_addroute", pattern, paths, http_methods);
}

static void dao_mvc_router_group_add_helper(INTERNAL_FUNCTION_PARAMETERS, zend_string *method)
{
	zval *pattern, *paths = NULL, http_method = {};

	dao_fetch_params(0, 1, 1, &pattern, &paths);

	if (!paths) {
		paths = &DAO_GLOBAL(z_null);
	}

	ZVAL_STR(&http_method, method);
	DAO_RETURN_CALL_METHOD(getThis(), "_addroute", pattern, paths, &http_method);
}

/**
 * Adds a route to the router that only match if the HTTP method is GET
 *
 * @param string $pattern
 * @param string/array $paths
 * @return Dao\Mvc\Router\Route
 */
PHP_METHOD(Dao_Mvc_Router_Group, addGet){

	dao_mvc_router_group_add_helper(INTERNAL_FUNCTION_PARAM_PASSTHRU, dao_interned_GET);
}

/**
 * Adds a route to the router that only match if the HTTP method is POST
 *
 * @param string $pattern
 * @param string/array $paths
 * @return Dao\Mvc\Router\Route
 */
PHP_METHOD(Dao_Mvc_Router_Group, addPost){

	dao_mvc_router_group_add_helper(INTERNAL_FUNCTION_PARAM_PASSTHRU, dao_interned_POST);
}

/**
 * Adds a route to the router that only match if the HTTP method is PUT
 *
 * @param string $pattern
 * @param string/array $paths
 * @return Dao\Mvc\Router\Route
 */
PHP_METHOD(Dao_Mvc_Router_Group, addPut){

	dao_mvc_router_group_add_helper(INTERNAL_FUNCTION_PARAM_PASSTHRU, dao_interned_PUT);
}

/**
 * Adds a route to the router that only match if the HTTP method is PATCH
 *
 * @param string $pattern
 * @param string/array $paths
 * @return Dao\Mvc\Router\Route
 */
PHP_METHOD(Dao_Mvc_Router_Group, addPatch){

	dao_mvc_router_group_add_helper(INTERNAL_FUNCTION_PARAM_PASSTHRU, dao_interned_PATCH);
}

/**
 * Adds a route to the router that only match if the HTTP method is DELETE
 *
 * @param string $pattern
 * @param string/array $paths
 * @return Dao\Mvc\Router\Route
 */
PHP_METHOD(Dao_Mvc_Router_Group, addDelete){

	dao_mvc_router_group_add_helper(INTERNAL_FUNCTION_PARAM_PASSTHRU, dao_interned_DELETE);
}

/**
 * Add a route to the router that only match if the HTTP method is OPTIONS
 *
 * @param string $pattern
 * @param string/array $paths
 * @return Dao\Mvc\Router\Route
 */
PHP_METHOD(Dao_Mvc_Router_Group, addOptions){

	dao_mvc_router_group_add_helper(INTERNAL_FUNCTION_PARAM_PASSTHRU, dao_interned_OPTIONS);
}

/**
 * Adds a route to the router that only match if the HTTP method is HEAD
 *
 * @param string $pattern
 * @param string/array $paths
 * @return Dao\Mvc\Router\Route
 */
PHP_METHOD(Dao_Mvc_Router_Group, addHead){

	dao_mvc_router_group_add_helper(INTERNAL_FUNCTION_PARAM_PASSTHRU, dao_interned_HEAD);
}

/**
 * Removes all the pre-defined routes
 */
PHP_METHOD(Dao_Mvc_Router_Group, clear){

	zval empty_routes;

	array_init(&empty_routes);
	dao_update_property(getThis(), SL("_routes"), &empty_routes);
}

/**
 * Adds a converter to perform an additional transformation for certain parameter
 *
 * @param string $name
 * @param callable $converter
 * @return Dao\Mvc\Router\Group
 */
PHP_METHOD(Dao_Mvc_Router_Group, convert){

	zval *name, *converter;

	dao_fetch_params(0, 2, 0, &name, &converter);

	dao_update_property_array(getThis(), SL("_converters"), name, converter);
	RETURN_THIS();
}

/**
 * Returns the router converter
 *
 * @return array|null
 */
PHP_METHOD(Dao_Mvc_Router_Group, getConverters) {

	RETURN_MEMBER(getThis(), "_converters");
}

/**
 * Set the name of the group
 *
 * @param string $hostname
 * @return Dao\Mvc\Router\Group
 */
PHP_METHOD(Dao_Mvc_Router_Group, setName){

	zval *name;

	dao_fetch_params(0, 1, 0, &name);

	dao_update_property(getThis(), SL("_name"), name);
	RETURN_THIS();
}

/**
 * Returns the name of this group
 *
 * @return string
 */
PHP_METHOD(Dao_Mvc_Router_Group, getName){


	RETURN_MEMBER(getThis(), "_name");
}
