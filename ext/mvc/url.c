
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

#include "mvc/url.h"
#include "mvc/urlinterface.h"
#include "mvc/url/exception.h"
#include "mvc/routerinterface.h"
#include "diinterface.h"
#include "di/injectable.h"

#include <Zend/zend_closures.h>

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/exception.h"
#include "kernel/object.h"
#include "kernel/array.h"
#include "kernel/fcall.h"
#include "kernel/framework/url.h"
#include "kernel/concat.h"
#include "kernel/string.h"
#include "kernel/framework/router.h"

#include "interned-strings.h"

/**
 * Dao\Mvc\Url
 *
 * This components aids in the generation of: URIs, URLs and Paths
 *
 *<code>
 *
 * //Generate a URL appending the URI to the base URI
 * echo $url->get('products/edit/1');
 *
 * //Generate a URL for a predefined route
 * echo $url->get(array('for' => 'blog-post', 'title' => 'some-cool-stuff', 'year' => '2012'));
 *
 *</code>
 */
zend_class_entry *dao_mvc_url_ce;

PHP_METHOD(Dao_Mvc_Url, setBaseUri);
PHP_METHOD(Dao_Mvc_Url, setStaticBaseUri);
PHP_METHOD(Dao_Mvc_Url, getBaseUri);
PHP_METHOD(Dao_Mvc_Url, getStaticBaseUri);
PHP_METHOD(Dao_Mvc_Url, setBasePath);
PHP_METHOD(Dao_Mvc_Url, getBasePath);
PHP_METHOD(Dao_Mvc_Url, get);
PHP_METHOD(Dao_Mvc_Url, getStatic);
PHP_METHOD(Dao_Mvc_Url, path);
PHP_METHOD(Dao_Mvc_Url, isLocal);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_url_setstaticbaseuri, 0, 0, 1)
	ZEND_ARG_INFO(0, staticBaseUri)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_url_getstatic, 0, 0, 0)
	ZEND_ARG_INFO(0, uri)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_url_islocal, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, uri, IS_STRING, 0)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_mvc_url_method_entry[] = {
	PHP_ME(Dao_Mvc_Url, setBaseUri, arginfo_dao_mvc_urlinterface_setbaseuri, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Url, setStaticBaseUri, arginfo_dao_mvc_url_setstaticbaseuri, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Url, getBaseUri, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Url, getStaticBaseUri, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Url, setBasePath, arginfo_dao_mvc_urlinterface_setbasepath, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Url, getBasePath, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Url, get, arginfo_dao_mvc_urlinterface_get, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Url, getStatic, arginfo_dao_mvc_url_getstatic, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Url, path, arginfo_dao_mvc_urlinterface_path, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Url, isLocal, arginfo_dao_mvc_url_islocal, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Mvc\Url initializer
 */
DAO_INIT_CLASS(Dao_Mvc_Url){

	DAO_REGISTER_CLASS_EX(Dao\\Mvc, Url, mvc_url, dao_di_injectable_ce, dao_mvc_url_method_entry, 0);

	zend_declare_property_null(dao_mvc_url_ce, SL("_baseUri"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_url_ce, SL("_staticBaseUri"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_url_ce, SL("_basePath"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_url_ce, SL("_router"), ZEND_ACC_PROTECTED);

	zend_class_implements(dao_mvc_url_ce, 1, dao_mvc_urlinterface_ce);

	return SUCCESS;
}

/**
 * Sets a prefix for all the URIs to be generated
 *
 *<code>
 *	$url->setBaseUri('/invo/');
 *	$url->setBaseUri('/invo/index.php/');
 *</code>
 *
 * @param string $baseUri
 * @return Dao\Mvc\Url
 */
PHP_METHOD(Dao_Mvc_Url, setBaseUri){

	zval *base_uri, trimmed = {}, static_base_uri = {};

	dao_fetch_params(0, 1, 0, &base_uri);
	
	if (!dao_end_with_str(base_uri, SL("/"))) {
		DAO_CONCAT_VS(&trimmed, base_uri, "/");
	} else {
		ZVAL_COPY(&trimmed, base_uri);
	}

	dao_update_property(getThis(), SL("_baseUri"), &trimmed);

	dao_read_property(&static_base_uri, getThis(), SL("_staticBaseUri"), PH_NOISY|PH_READONLY);
	if (Z_TYPE(static_base_uri) == IS_NULL) {
		dao_update_property(getThis(), SL("_staticBaseUri"), &trimmed);
	}
	zval_ptr_dtor(&trimmed);

	RETURN_THIS();
}

/**
 * Sets a prefix for all static URLs generated
 *
 *<code>
 *	$url->setStaticBaseUri('/invo/');
 *</code>
 *
 * @param string $staticBaseUri
 * @return Dao\Mvc\Url
 */
PHP_METHOD(Dao_Mvc_Url, setStaticBaseUri){

	zval *static_base_uri, trimmed = {};

	dao_fetch_params(0, 1, 0, &static_base_uri);
	
	if (!dao_end_with_str(static_base_uri, SL("/"))) {
		DAO_CONCAT_VS(&trimmed, static_base_uri, "/");
	} else {
		ZVAL_COPY(&trimmed, static_base_uri);
	}

	dao_update_property(getThis(), SL("_staticBaseUri"), &trimmed);
	zval_ptr_dtor(&trimmed);
	RETURN_THIS();
}

/**
 * Returns the prefix for all the generated urls. By default /
 *
 * @return string
 */
PHP_METHOD(Dao_Mvc_Url, getBaseUri){

	zval base_uri = {}, slash = {}, *_SERVER, php_self = {}, uri = {};

	dao_read_property(&base_uri, getThis(), SL("_baseUri"), PH_NOISY|PH_COPY);
	if (Z_TYPE(base_uri) == IS_NULL) {
		ZVAL_STRING(&slash, "/");
		_SERVER = dao_get_global_str(SL("_SERVER"));
		if (dao_array_isset_fetch_str(&php_self, _SERVER, SL("PHP_SELF"), PH_READONLY)) {
			dao_get_uri(&uri, &php_self);
		}

		if (!zend_is_true(&uri)) {
			ZVAL_COPY(&base_uri, &slash);
		} else {
			DAO_CONCAT_VVV(&base_uri, &slash, &uri, &slash);
			zval_ptr_dtor(&uri);
		}
		zval_ptr_dtor(&slash);

		dao_update_property(getThis(), SL("_baseUri"), &base_uri);
	}

	RETURN_ZVAL(&base_uri, 0, 0);
}

/**
 * Returns the prefix for all the generated static urls. By default /
 *
 * @return string
 */
PHP_METHOD(Dao_Mvc_Url, getStaticBaseUri){

	zval static_base_uri = {};

	dao_read_property(&static_base_uri, getThis(), SL("_staticBaseUri"), PH_NOISY|PH_READONLY);
	if (Z_TYPE(static_base_uri) != IS_NULL) {
		RETURN_CTOR(&static_base_uri);
	}

	DAO_RETURN_CALL_METHOD(getThis(), "getbaseuri");
}

/**
 * Sets a base path for all the generated paths
 *
 *<code>
 *	$url->setBasePath('/var/www/htdocs/');
 *</code>
 *
 * @param string $basePath
 * @return Dao\Mvc\Url
 */
PHP_METHOD(Dao_Mvc_Url, setBasePath){

	zval *base_path;

	dao_fetch_params(0, 1, 0, &base_path);

	dao_update_property(getThis(), SL("_basePath"), base_path);
	RETURN_THIS();
}

/**
 * Returns the base path
 *
 * @return string
 */
PHP_METHOD(Dao_Mvc_Url, getBasePath){


	RETURN_MEMBER(getThis(), "_basePath");
}

/**
 * Generates a URL
 *
 *<code>
 *
 * //Generate a URL appending the URI to the base URI
 * echo $url->get('products/edit/1');
 *
 * //Generate a URL for a predefined route
 * echo $url->get(array('for' => 'blog-post', 'title' => 'some-cool-stuff', 'year' => '2012'));
 * echo $url->get(array('for' => 'blog-post', 'hostname' => true, 'title' => 'some-cool-stuff', 'year' => '2012'));
 *
 *</code>
 *
 * @param string|array $uri
 * @param array|object args Optional arguments to be appended to the query string
 * @param bool|null $local
 * @return string
 */
PHP_METHOD(Dao_Mvc_Url, get){

	zval *uri = NULL, *args = NULL, *_local = NULL, local = {}, base_uri = {}, dependency_injector = {};
	zval service = {}, route_name = {}, exception_message = {}, realuri = {};
	zval paths = {}, matched = {}, regexp = {}, generator = {}, arguments = {};

	dao_fetch_params(1, 0, 3, &uri, &args, &local);

	if (!uri) {
		uri = &DAO_GLOBAL(z_null);
	}

	if (!args) {
		args = &DAO_GLOBAL(z_null);
	}

	if (_local) {
		ZVAL_COPY_VALUE(&local, _local);
	} else {
		ZVAL_TRUE(&local);
	}

	DAO_MM_CALL_METHOD(&base_uri, getThis(), "getbaseuri");
	DAO_MM_ADD_ENTRY(&base_uri);

	if (Z_TYPE_P(uri) == IS_STRING) {
		if (strstr(Z_STRVAL_P(uri), ":")) {
			ZVAL_STRING(&regexp, "/^[^:\\/?#]++:/");
			dao_preg_match(&matched, &regexp, uri, NULL, 0, 0);
			zval_ptr_dtor(&regexp);
			if (zend_is_true(&matched)) {
				ZVAL_FALSE(&local);
			}
		}

		if (zend_is_true(&local)) {
			if (!dao_start_with_str(uri, SL("/"))) {
				DAO_CONCAT_VV(&realuri, &base_uri, uri);
				DAO_MM_ADD_ENTRY(&realuri);
			} else {
				zval trimmed = {};
				dao_fast_trim_str(&trimmed, uri, "/", DAO_TRIM_LEFT);
				DAO_CONCAT_VV(&realuri, &base_uri, &trimmed);
				zval_ptr_dtor(&trimmed);
				DAO_MM_ADD_ENTRY(&realuri);
			}
		} else {
			DAO_MM_ZVAL_COPY(&realuri, uri);
		}
	} else if (Z_TYPE_P(uri) == IS_ARRAY) {
		zval router = {}, route = {};

		if (!dao_array_isset_fetch_str(&route_name, uri, SL("for"), PH_READONLY)) {
			DAO_MM_THROW_EXCEPTION_STR(dao_mvc_url_exception_ce, "It's necessary to define the route name with the parameter \"for\"");
			return;
		}

		dao_read_property(&router, getThis(), SL("_router"), PH_COPY);

		/**
		 * Check if the router has not previously set
		 */
		if (Z_TYPE(router) != IS_OBJECT) {
			DAO_MM_CALL_METHOD(&dependency_injector, getThis(), "getdi");
			DAO_MM_ADD_ENTRY(&dependency_injector);
			if (!zend_is_true(&dependency_injector)) {
				DAO_MM_THROW_EXCEPTION_STR(dao_mvc_url_exception_ce, "A dependency injector container is required to obtain the \"url\" service");
				return;
			}

			ZVAL_STR(&service, IS(router));

			DAO_MM_CALL_METHOD(&router, &dependency_injector, "getshared", &service);
			DAO_MM_ADD_ENTRY(&router);
			DAO_MM_VERIFY_INTERFACE(&router, dao_mvc_routerinterface_ce);
			dao_update_property(getThis(), SL("_router"), &router);
		}

		/**
		 * Every route is uniquely identified by a name
		 */
		DAO_MM_CALL_METHOD(&route, &router, "getroutebyname", &route_name);
		DAO_MM_ADD_ENTRY(&route);
		if (Z_TYPE(route) != IS_OBJECT) {
			DAO_CONCAT_SVS(&exception_message, "Cannot obtain a route using the name \"", &route_name, "\"");
			DAO_MM_ADD_ENTRY(&exception_message);
			DAO_MM_THROW_EXCEPTION_ZVAL(dao_mvc_url_exception_ce, &exception_message);
			return;
		}

		/**
		 * Return the reversed paths
		 */
		DAO_MM_CALL_METHOD(&paths, &route, "getreversedpaths");
		DAO_MM_ADD_ENTRY(&paths);

		/**
		 * Return the Url Generator
		 */
		DAO_MM_CALL_METHOD(&generator, &route, "geturlgenerator");
		DAO_MM_ADD_ENTRY(&generator);

		if (dao_is_callable(&generator) || (Z_TYPE(generator) == IS_OBJECT && instanceof_function(Z_OBJCE(generator), zend_ce_closure))) {
			array_init_size(&arguments, 3);
			dao_array_append(&arguments, &base_uri, PH_COPY);
			dao_array_append(&arguments, &paths, PH_COPY);
			dao_array_append(&arguments, uri, PH_COPY);
			DAO_MM_ADD_ENTRY(&arguments);
			DAO_MM_CALL_USER_FUNC_ARRAY(&realuri, &generator, &arguments);
			DAO_MM_ADD_ENTRY(&realuri);
		} else {
			zval pattern = {}, processed_uri = {}, has_hostname = {};
			DAO_MM_CALL_METHOD(&pattern, &route, "getpattern");
			DAO_MM_ADD_ENTRY(&pattern);

			/**
			 * Replace the patterns by its variables
			 */
			dao_replace_paths(&processed_uri, &pattern, &paths, uri);
			DAO_MM_ADD_ENTRY(&processed_uri);

			if (dao_array_isset_fetch_str(&has_hostname, uri, SL("hostname"), PH_READONLY) && zend_is_true(&has_hostname)) {
				zval hostname = {};
				DAO_MM_CALL_METHOD(&hostname, &route, "gethostname");
				DAO_MM_ADD_ENTRY(&hostname);
				DAO_CONCAT_VVV(&realuri, &hostname, &base_uri, &processed_uri);
				DAO_MM_ADD_ENTRY(&realuri);
			} else {
				DAO_CONCAT_VV(&realuri, &base_uri, &processed_uri);
				DAO_MM_ADD_ENTRY(&realuri);
			}
		}
	}

	if (zend_is_true(args)) {
		zval query_string = {};
		dao_http_build_query(&query_string, args, "&");
		DAO_MM_ADD_ENTRY(&query_string);
		if (Z_TYPE(query_string) == IS_STRING && Z_STRLEN_P(&query_string)) {
			if (dao_memnstr_str(&realuri, "?", 1)) {
				DAO_SCONCAT_SV(&realuri, "&", &query_string);
				DAO_MM_ADD_ENTRY(&realuri);
			}
			else {
				DAO_SCONCAT_SV(&realuri, "?", &query_string);
				DAO_MM_ADD_ENTRY(&realuri);
			}
		}
	}
	RETURN_MM_CTOR(&realuri);
}

/**
 * Generates a URL for a static resource
 *
 * @param string|array $uri
 * @param array $args
 * @return string
 */
PHP_METHOD(Dao_Mvc_Url, getStatic){

	zval *uri = NULL, *args = NULL, static_base_uri = {}, matched = {}, pattern = {};

	dao_fetch_params(0, 0, 2, &uri, &args);

	if (!uri) {
		uri = &DAO_GLOBAL(z_null);
	} else {
		DAO_ENSURE_IS_STRING(uri);

		if (strstr(Z_STRVAL_P(uri), "://")) {
			ZVAL_STRING(&pattern, "/^[^:\\/?#]++:/");
			dao_preg_match(&matched, &pattern, uri, NULL, 0, 0);
			zval_ptr_dtor(&pattern);
			if (zend_is_true(&matched)) {
				RETURN_CTOR(uri);
			}
		}
	}

	dao_read_property(&static_base_uri, getThis(), SL("_staticBaseUri"), PH_NOISY|PH_READONLY);
	if (Z_TYPE(static_base_uri) != IS_NULL) {
		if (!dao_start_with_str(uri, SL("/"))) {
			DAO_CONCAT_VV(return_value, &static_base_uri, uri);
		} else {
			zval trimmed = {};
			dao_fast_trim_str(&trimmed, uri, "/", DAO_TRIM_LEFT);
			DAO_CONCAT_VV(return_value, &static_base_uri, &trimmed);
			zval_ptr_dtor(&trimmed);
		}
	} else {
		zval base_uri = {};
		DAO_CALL_METHOD(&base_uri, getThis(), "getbaseuri");
		if (!dao_start_with_str(uri, SL("/"))) {
			DAO_CONCAT_VV(return_value, &base_uri, uri);
		} else {
			zval trimmed = {};
			dao_fast_trim_str(&trimmed, uri, "/", DAO_TRIM_LEFT);
			DAO_CONCAT_VV(return_value, &base_uri, &trimmed);
			zval_ptr_dtor(&trimmed);
		}
		zval_ptr_dtor(&base_uri);
	}

	if (args && Z_TYPE_P(args) == IS_ARRAY) {
		zval query_string = {};
		dao_http_build_query(&query_string, args, "&");
		if (Z_TYPE(query_string) == IS_STRING && Z_STRLEN(query_string)) {
			if (dao_memnstr_str(return_value, "?", 1)) {
				DAO_SCONCAT_SV(return_value, "&", &query_string);
			}
			else {
				DAO_SCONCAT_SV(return_value, "?", &query_string);
			}
		}
		zval_ptr_dtor(&query_string);
	}
}

/**
 * Generates a local path
 *
 * @param string $path
 * @return string
 */
PHP_METHOD(Dao_Mvc_Url, path){

	zval *path = NULL, base_path = {};

	dao_fetch_params(0, 0, 1, &path);

	if (!path) {
		path = &DAO_GLOBAL(z_null);
	}

	dao_read_property(&base_path, getThis(), SL("_basePath"), PH_NOISY|PH_READONLY);
	DAO_CONCAT_VV(return_value, &base_path, path);
}

/**
 * Check whether the URI is local
 *
 * @param string $uri
 * @return boolean
 */
PHP_METHOD(Dao_Mvc_Url, isLocal){

	zval *uri, regexp = {}, matched = {};

	dao_fetch_params(0, 1, 0, &uri);

	ZVAL_TRUE(return_value);
	if (strstr(Z_STRVAL_P(uri), ":")) {
		ZVAL_STRING(&regexp, "/^[^:\\/?#]++:/");
		dao_preg_match(&matched, &regexp, uri, NULL, 0, 0);
		zval_ptr_dtor(&regexp);
		if (zend_is_true(&matched)) {
			ZVAL_FALSE(return_value);
		}
	}
}
