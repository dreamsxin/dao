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
  +------------------------------------------------------------------------+
*/

#include "http/uri.h"

#include <ext/standard/url.h>

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"
#include "kernel/object.h"
#include "kernel/hash.h"
#include "kernel/array.h"
#include "kernel/concat.h"
#include "kernel/operators.h"
#include "kernel/string.h"
#include "kernel/debug.h"

/**
 * Dao\Http\Uri
 *
 *<code>
 *	$uri1 = new Dao\Http\Uri('http://phalconphp.com/foo/bar/baz?var1=a&var2=1');
 *
 *	$uri2 = $uri1->resolve('/last');
 *	echo $uri2->build(); // http://phalconphp.com/last?var1=a&var2=1
 *
 *
 *	$uri3 = $uri1->resolve('last');
 *	echo $uri3->build(); // http://phalconphp.com/foo/bar/baz/last?var1=a&var2=1
 *
 *	$uri4 = new Dao\Http\Uri(array(
 *	    'scheme' => 'https',
 *	    'host' => 'admin.example.com',
 *	    'user' => 'john',
 *	    'pass' => 'doe'
 *	));
 *
 *	$uri5 = $uri1->resolve($uri4);
 *	echo $uri5->build(); // https://john:doe@admin.example.com/foo/bar/baz?var1=a&var2=1
 *</code>
 */
zend_class_entry *dao_http_uri_ce;

PHP_METHOD(Dao_Http_Uri, __construct);
PHP_METHOD(Dao_Http_Uri, __toString);
PHP_METHOD(Dao_Http_Uri, __unset);
PHP_METHOD(Dao_Http_Uri, __set);
PHP_METHOD(Dao_Http_Uri, __get);
PHP_METHOD(Dao_Http_Uri, __isset);
PHP_METHOD(Dao_Http_Uri, getParts);
PHP_METHOD(Dao_Http_Uri, getPath);
PHP_METHOD(Dao_Http_Uri, build);
PHP_METHOD(Dao_Http_Uri, resolve);
PHP_METHOD(Dao_Http_Uri, extend);
PHP_METHOD(Dao_Http_Uri, extendQuery);
PHP_METHOD(Dao_Http_Uri, extendPath);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_http_uri___construct, 0, 0, 0)
	ZEND_ARG_INFO(0, uri)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_http_uri___unset, 0, 0, 1)
	ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_http_uri___set, 0, 0, 2)
	ZEND_ARG_INFO(0, key)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_http_uri___get, 0, 0, 1)
	ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_http_uri___isset, 0, 0, 1)
	ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_http_uri_build, 0, 0, 0)
	ZEND_ARG_INFO(0, fix)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_http_uri_resolve, 0, 0, 1)
	ZEND_ARG_INFO(0, uri)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_http_uri_extend, 0, 0, 1)
	ZEND_ARG_INFO(0, uri)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_http_uri_extendquery, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, param, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_http_uri_extendpath, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, param, IS_STRING, 0)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_http_uri_method_entry[] = {
	PHP_ME(Dao_Http_Uri, __construct, arginfo_dao_http_uri___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Http_Uri, __toString, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Uri, __unset, arginfo_dao_http_uri___unset, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Uri, __set, arginfo_dao_http_uri___set, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Uri, __get, arginfo_dao_http_uri___get, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Uri, __isset, arginfo_dao_http_uri___isset, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Uri, getParts, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Uri, getPath, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Uri, build, arginfo_dao_http_uri_build, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Uri, resolve, arginfo_dao_http_uri_resolve, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Uri, extend, arginfo_dao_http_uri_extend, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Uri, extendQuery, arginfo_dao_http_uri_extendquery, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Uri, extendPath, arginfo_dao_http_uri_extendpath, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Http\Uri initializer
 */
DAO_INIT_CLASS(Dao_Http_Uri){

	DAO_REGISTER_CLASS(Dao\\Http, Uri, http_uri, dao_http_uri_method_entry, 0);

	zend_declare_property_null(dao_http_uri_ce, SL("_parts"), ZEND_ACC_PROTECTED);

	return SUCCESS;
}

/**
 * Dao\Http\Uri constructor
 *
 * @param mixed $uri
 */
PHP_METHOD(Dao_Http_Uri, __construct)
{
	zval *uri = NULL, parts = {}, query = {}, params = {};

	dao_fetch_params(1, 0, 1, &uri);

	if (!uri || DAO_IS_EMPTY(uri)) {
		dao_update_property_empty_array(getThis(), SL("_parts"));
	} else if (Z_TYPE_P(uri) == IS_STRING) {
		DAO_MM_CALL_FUNCTION(&parts, "parse_url", uri);
		DAO_MM_ADD_ENTRY(&parts);
		if (dao_array_isset_fetch_str(&query, &parts, SL("query"), PH_READONLY)) {
			ZVAL_MAKE_REF(&params);
			DAO_MM_CALL_FUNCTION(NULL, "parse_str", &query, &params);
			ZVAL_UNREF(&params);
			dao_array_update_str(&parts, SL("query"), &params, 0);
		}

		dao_update_property(getThis(), SL("_parts"), &parts);
	} else if (Z_TYPE_P(uri) == IS_ARRAY) {
		dao_update_property(getThis(), SL("_parts"), uri);
	} else if (Z_TYPE_P(uri) == IS_OBJECT && Z_OBJCE_P(uri) == dao_http_uri_ce) {
		dao_read_property(&parts, uri, SL("_parts"), PH_SEPARATE);
		DAO_MM_ADD_ENTRY(&parts);
		dao_update_property(getThis(), SL("_parts"), &parts);
	} else {
		dao_update_property_empty_array(getThis(), SL("_parts"));
	}
	RETURN_MM();
}

/**
 * Magic __toString method returns uri
 *
 * @return string
 */
PHP_METHOD(Dao_Http_Uri, __toString){

	DAO_RETURN_CALL_SELF("build");
}

/**
 * Magic __unset method
 *
 * @param string $key
 */
PHP_METHOD(Dao_Http_Uri, __unset){

	zval *key;

	dao_fetch_params(0, 1, 0, &key);

	dao_unset_property_array(getThis(), SL("_parts"), key);
}

/**
 * Magic __set method
 *
 * @param string $key
 */
PHP_METHOD(Dao_Http_Uri, __set){

	zval *key, *value;

	dao_fetch_params(0, 2, 0, &key, &value);

	dao_update_property_array(getThis(), SL("_parts"), key, value);
}

/**
 * Magic __get method
 *
 * @param string $key
 * @return string
 */
PHP_METHOD(Dao_Http_Uri, __get){

	zval *key, parts = {};

	dao_fetch_params(0, 1, 0, &key);

	dao_read_property(&parts, getThis(), SL("_parts"), PH_NOISY|PH_READONLY);

	if (!dao_array_isset_fetch(return_value, &parts, key,PH_COPY)) {
		 RETURN_NULL();
	}
}

/**
 * Magic __isset method
 *
 * @param string $key
 * @return boolean
 */
PHP_METHOD(Dao_Http_Uri, __isset){

	zval *key, parts = {};

	dao_fetch_params(0, 1, 0, &key);

	dao_read_property(&parts, getThis(), SL("_parts"), PH_NOISY|PH_READONLY);

	RETURN_BOOL(dao_array_isset(&parts, key));
}

/**
 * Returns parts
 *
 * @return array
 */
PHP_METHOD(Dao_Http_Uri, getParts){

	RETURN_MEMBER(getThis(), "_parts");
}

/**
 * Retrieve the URI path
 *
 * @return string
 */
PHP_METHOD(Dao_Http_Uri, getPath)
{
	zval parts = {}, value = {};

	dao_read_property(&parts, getThis(), SL("_parts"), PH_NOISY|PH_READONLY);

	if (!dao_array_isset_fetch_str(&value, &parts, SL("path"), PH_READONLY)) {
		 RETURN_NULL();
	}

	RETURN_CTOR(&value);
}

/**
 * Returns uri
 *
 * @param boolean $fix
 * @return string
 */
PHP_METHOD(Dao_Http_Uri, build)
{
	zval *fix = NULL, parts = {}, uri = {}, scheme = {}, host = {}, user = {}, pass = {}, port = {}, path = {}, query = {}, fragment = {}, tmp = {};

	dao_fetch_params(1, 0, 1, &fix);

	dao_read_property(&parts, getThis(), SL("_parts"), PH_NOISY|PH_READONLY);

	if (dao_array_isset_fetch_str(&scheme, &parts, SL("scheme"), PH_READONLY) && DAO_IS_NOT_EMPTY(&scheme)) {
		if (dao_array_isset_fetch_str(&host, &parts, SL("host"), PH_READONLY) && DAO_IS_NOT_EMPTY(&host)) {
			if (dao_array_isset_fetch_str(&user, &parts, SL("user"), PH_READONLY) && DAO_IS_NOT_EMPTY(&user)) {
				if (dao_array_isset_fetch_str(&pass, &parts, SL("pass"), PH_READONLY) && DAO_IS_NOT_EMPTY(&pass)) {
					DAO_CONCAT_VSVSVSV(&uri, &scheme, "://", &user, ":", &pass, "@", &host);
					DAO_MM_ADD_ENTRY(&uri);
				} else {
					DAO_CONCAT_VSVSV(&uri, &scheme, "://", &user, "@", &host);
					DAO_MM_ADD_ENTRY(&uri);
				}
			} else {
				DAO_CONCAT_VSV(&uri, &scheme, "://", &host);
				DAO_MM_ADD_ENTRY(&uri);
			}
		} else {
			DAO_CONCAT_VS(&uri, &scheme, ":");
			DAO_MM_ADD_ENTRY(&uri);
		}
	}

	if (dao_array_isset_fetch_str(&port, &parts, SL("port"), PH_READONLY) && DAO_IS_NOT_EMPTY(&port)) {
		DAO_SCONCAT_SV(&uri, ":", &port);
		DAO_MM_ADD_ENTRY(&uri);
	}

	if (dao_array_isset_fetch_str(&path, &parts, SL("path"), PH_READONLY) && DAO_IS_NOT_EMPTY(&path)) {
		zval tmp = {};
		if (!dao_start_with_str(&path, SL("/"))) {
			DAO_CONCAT_SV(&tmp, "/", &path);
			DAO_MM_ADD_ENTRY(&tmp);
		} else {
			DAO_MM_ZVAL_COPY(&tmp, &path);
		}
		if (fix && zend_is_true(fix)) {
			zval regex = {}, replace = {}, last = {};
			DAO_MM_ZVAL_STRING(&regex, "#/[^/]+/../#");
			DAO_MM_ZVAL_STRING(&replace, "/");

			while (!DAO_IS_EQUAL(&last, &tmp)) {
				DAO_MM_ZVAL_COPY(&last, &tmp);
				dao_fast_preg_replace(&tmp, &regex, &replace, &last);
				DAO_MM_ADD_ENTRY(&tmp);
			}
			ZVAL_NULL(&last);
			DAO_MM_ZVAL_STRING(&regex, "#([./]/)+#");
			while (!DAO_IS_EQUAL(&last, &tmp)) {
				DAO_MM_ZVAL_COPY(&last, &tmp);
				dao_fast_preg_replace(&tmp, &regex, &replace, &last);
				DAO_MM_ADD_ENTRY(&tmp);
			}
		}
		DAO_SCONCAT(&uri, &tmp);
		DAO_MM_ADD_ENTRY(&uri);
	}

	if (dao_array_isset_fetch_str(&query, &parts, SL("query"), PH_READONLY) && DAO_IS_NOT_EMPTY(&query)) {
		if (dao_http_build_query(&tmp, &query, "&") == SUCCESS) {
			DAO_SCONCAT_SV(&uri, "?", &tmp);
			zval_ptr_dtor(&tmp);
			DAO_MM_ADD_ENTRY(&uri);
		}
	}

	if (dao_array_isset_fetch_str(&fragment, &parts, SL("fragment"), PH_READONLY) && DAO_IS_NOT_EMPTY(&fragment)) {
		DAO_SCONCAT_SV(&uri, "#", &fragment);
		DAO_MM_ADD_ENTRY(&uri);
	}

	RETURN_MM_CTOR(&uri);
}

PHP_METHOD(Dao_Http_Uri, resolve)
{
	zval *uri, self = {};

	dao_fetch_params(1, 1, 0, &uri);

	object_init_ex(&self, dao_http_uri_ce);
	DAO_MM_ADD_ENTRY(&self);
	DAO_MM_CALL_METHOD(NULL, &self, "__construct", getThis());
	DAO_MM_CALL_METHOD(NULL, &self, "extend", uri);

	RETURN_MM_CTOR(&self);
}

PHP_METHOD(Dao_Http_Uri, extend)
{
	zval *uri, parts = {}, parts2 = {}, path = {}, path2 = {};

	dao_fetch_params(1, 1, 0, &uri);

	dao_read_property(&parts, getThis(), SL("_parts"), PH_NOISY|PH_READONLY);

	if (Z_TYPE_P(uri) != IS_OBJECT || Z_OBJCE_P(uri) != dao_http_uri_ce) {
		zval self = {};
		object_init_ex(&self, dao_http_uri_ce);
		DAO_MM_ADD_ENTRY(&self);
		DAO_MM_CALL_METHOD(NULL, &self, "__construct", uri);
		DAO_MM_CALL_METHOD(&parts2, &self, "getParts");
	} else {
		DAO_MM_CALL_METHOD(&parts2, uri, "getParts");
	}
	zval_ptr_dtor(&parts2);

	if (dao_array_isset_fetch_str(&path, &parts, SL("path"), PH_READONLY)) {
		if (dao_array_isset_fetch_str(&path2, &parts2, SL("path"), PH_READONLY)) {
			if (!dao_start_with_str(&path2, SL("/"))) {
				zval tmp = {};
				if (!dao_end_with_str(&path, SL("/"))) {
					DAO_CONCAT_VSV(&tmp, &path, "/", &path2);
				} else {
					DAO_CONCAT_VV(&tmp, &path, &path2);
				}
				dao_array_update_str(&parts, SL("path"), &tmp, 0);
				dao_array_unset_str(&parts2, SL("path"), 0);
			}
		}
	}

	dao_array_merge_recursive_n(&parts, &parts2);

	RETURN_MM_THIS();
}

PHP_METHOD(Dao_Http_Uri, extendQuery){

	zval *params, parts = {}, query = {};

	dao_fetch_params(0, 1, 0, &params);

	dao_read_property(&parts, getThis(), SL("_parts"), PH_NOISY|PH_READONLY);

	if (dao_array_isset_fetch_str(&query, &parts, SL("query"), PH_READONLY)) {
		dao_array_merge_recursive_n(&query, params);
	} else {
		dao_array_update_str(&parts, SL("query"), params, PH_COPY);
	}

	RETURN_THIS();
}

PHP_METHOD(Dao_Http_Uri, extendPath){

	zval *uri_path, parts = {}, path = {};

	dao_fetch_params(0, 1, 0, &uri_path);

	dao_read_property(&parts, getThis(), SL("_parts"), PH_NOISY|PH_READONLY);

	if (!dao_start_with_str(uri_path, SL("/"))) {
		if (dao_array_isset_fetch_str(&path, &parts, SL("path"), PH_READONLY)) {
			zval tmp = {};
			if (!dao_end_with_str(&path, SL("/"))) {
				DAO_CONCAT_VSV(&tmp, &path, "/", uri_path);
			} else {
				DAO_CONCAT_VV(&tmp, &path, uri_path);
			}
			dao_array_update_str(&parts, SL("path"), &tmp, 0);
		}
	} else {
		dao_array_update_str(&parts, SL("path"), uri_path, PH_COPY);
	}

	RETURN_THIS();
}
