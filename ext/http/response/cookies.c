
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

#include "http/response/cookies.h"
#include "http/response/cookiesinterface.h"
#include "http/response/exception.h"
#include "http/cookie/exception.h"
#include "http/cookie.h"
#include "http/responseinterface.h"
#include "diinterface.h"
#include "di/injectable.h"

#include <main/SAPI.h>

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/exception.h"
#include "kernel/array.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"

#include "interned-strings.h"

/**
 * Dao\Http\Response\Cookies
 *
 * This class is a bag to manage the cookies
 * A cookies bag is automatically registered as part of the 'response' service in the DI
 */
zend_class_entry *dao_http_response_cookies_ce;

PHP_METHOD(Dao_Http_Response_Cookies, __construct);
PHP_METHOD(Dao_Http_Response_Cookies, useEncryption);
PHP_METHOD(Dao_Http_Response_Cookies, isUsingEncryption);
PHP_METHOD(Dao_Http_Response_Cookies, set);
PHP_METHOD(Dao_Http_Response_Cookies, get);
PHP_METHOD(Dao_Http_Response_Cookies, has);
PHP_METHOD(Dao_Http_Response_Cookies, delete);
PHP_METHOD(Dao_Http_Response_Cookies, send);
PHP_METHOD(Dao_Http_Response_Cookies, reset);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_http_response_cookies___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, value)
	ZEND_ARG_INFO(0, expire)
	ZEND_ARG_INFO(0, path)
	ZEND_ARG_INFO(0, secure)
	ZEND_ARG_INFO(0, domain)
	ZEND_ARG_INFO(0, httpOnly)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_http_response_cookies_method_entry[] = {
	PHP_ME(Dao_Http_Response_Cookies, __construct, arginfo_dao_http_response_cookies___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Http_Response_Cookies, useEncryption, arginfo_dao_http_response_cookiesinterface_useencryption, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Response_Cookies, isUsingEncryption, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Response_Cookies, set, arginfo_dao_http_response_cookiesinterface_set, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Response_Cookies, get, arginfo_dao_http_response_cookiesinterface_get, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Response_Cookies, has, arginfo_dao_http_response_cookiesinterface_has, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Response_Cookies, delete, arginfo_dao_http_response_cookiesinterface_delete, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Response_Cookies, send, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Response_Cookies, reset, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Http\Response\Cookies initializer
 */
DAO_INIT_CLASS(Dao_Http_Response_Cookies){

	DAO_REGISTER_CLASS_EX(Dao\\Http\\Response, Cookies, http_response_cookies, dao_di_injectable_ce, dao_http_response_cookies_method_entry, 0);

	zend_declare_property_bool(dao_http_response_cookies_ce, SL("_registered"), 0, ZEND_ACC_PROTECTED);
	zend_declare_property_bool(dao_http_response_cookies_ce, SL("_useEncryption"), 0, ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_http_response_cookies_ce, SL("_cookies"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_http_response_cookies_ce, SL("_expire"), ZEND_ACC_PROTECTED);
	zend_declare_property_string(dao_http_response_cookies_ce, SL("_path"), "/", ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_http_response_cookies_ce, SL("_secure"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_http_response_cookies_ce, SL("_domain"), ZEND_ACC_PROTECTED);
	zend_declare_property_bool(dao_http_response_cookies_ce, SL("_httpOnly"), 1, ZEND_ACC_PROTECTED);

	zend_class_implements(dao_http_response_cookies_ce, 1, dao_http_response_cookiesinterface_ce);

	return SUCCESS;
}

/**
 * Dao\Http\Response\Cookie constructor
 *
 * @param int $expire
 * @param string $path
 * @param boolean $secure
 * @param string $domain
 * @param boolean $httpOnly
 */
PHP_METHOD(Dao_Http_Response_Cookies, __construct){

	zval *expire = NULL, *path = NULL, *secure = NULL, *domain = NULL, *http_only = NULL;

	dao_fetch_params(0, 0, 5, &expire, &path, &secure, &domain, &http_only);

	if (expire && Z_TYPE_P(expire) != IS_NULL) {
		dao_update_property(getThis(), SL("_expire"), expire);
	}

	if (path && Z_TYPE_P(path) != IS_NULL) {
		dao_update_property(getThis(), SL("_path"), path);
	}

	if (secure && Z_TYPE_P(secure) != IS_NULL) {
		dao_update_property(getThis(), SL("_secure"), secure);
	}

	if (domain && Z_TYPE_P(domain) != IS_NULL) {
		dao_update_property(getThis(), SL("_domain"), domain);
	}

	if (http_only && Z_TYPE_P(http_only) != IS_NULL) {
		dao_update_property(getThis(), SL("_httpOnly"), http_only);
	}

	dao_update_property_empty_array(getThis(), SL("_cookies"));
}

/**
 * Set if cookies in the bag must be automatically encrypted/decrypted
 *
 * @param boolean $useEncryption
 * @return Dao\Http\Response\Cookies
 */
PHP_METHOD(Dao_Http_Response_Cookies, useEncryption){

	zval *use_encryption;

	dao_fetch_params(0, 1, 0, &use_encryption);

	dao_update_property(getThis(), SL("_useEncryption"), use_encryption);
	RETURN_THIS();
}

/**
 * Returns if the bag is automatically encrypting/decrypting cookies
 *
 * @return boolean
 */
PHP_METHOD(Dao_Http_Response_Cookies, isUsingEncryption){


	RETURN_MEMBER(getThis(), "_useEncryption");
}

/**
 * Sets a cookie to be sent at the end of the request
 * This method overrides any cookie set before with the same name
 *
 * @param string $name
 * @param mixed $value
 * @param int $expire
 * @param string $path
 * @param boolean $secure
 * @param string $domain
 * @param boolean $httpOnly
 * @return Dao\Http\Response\Cookies
 */
PHP_METHOD(Dao_Http_Response_Cookies, set){

	zval *name, *value = NULL, *_expire = NULL, *_path = NULL, *_secure = NULL, *_domain = NULL, *_http_only = NULL;
	zval expire = {}, path = {}, secure = {}, domain = {}, http_only = {}, cookies = {}, encryption = {};
	zval dependency_injector = {}, cookie = {}, registered = {}, service = {}, response = {};

	dao_fetch_params(1, 1, 6, &name, &value, &_expire, &_path, &_secure, &_domain, &_http_only);

	if (Z_TYPE_P(name) != IS_STRING) {
		DAO_MM_THROW_EXCEPTION_STR(dao_http_cookie_exception_ce, "The cookie name must be string");
		return;
	}

	if (!value) {
		value = &DAO_GLOBAL(z_null);
	}

	if (!_expire) {
		dao_read_property(&expire, getThis(), SL("_expire"), PH_NOISY|PH_READONLY);
	} else {
		ZVAL_COPY_VALUE(&expire, _expire);
	}

	if (!_path) {
		dao_read_property(&path, getThis(), SL("_path"), PH_NOISY|PH_READONLY);
	} else {
		ZVAL_COPY_VALUE(&path, _path);
	}

	if (!_secure) {
		dao_read_property(&secure, getThis(), SL("_secure"), PH_NOISY|PH_READONLY);
	} else {
		ZVAL_COPY_VALUE(&secure, _secure);
	}

	if (!_domain) {
		dao_read_property(&domain, getThis(), SL("_domain"), PH_NOISY|PH_READONLY);
	} else {
		ZVAL_COPY_VALUE(&domain, _domain);
	}

	if (!_http_only) {
		dao_read_property(&http_only, getThis(), SL("_httpOnly"), PH_NOISY|PH_READONLY);
	} else {
		ZVAL_COPY_VALUE(&http_only, _http_only);
	}

	dao_read_property(&cookies, getThis(), SL("_cookies"), PH_NOISY|PH_READONLY);
	dao_read_property(&encryption, getThis(), SL("_useEncryption"), PH_NOISY|PH_READONLY);
	DAO_MM_CALL_METHOD(&dependency_injector, getThis(), "getdi");
	DAO_MM_ADD_ENTRY(&dependency_injector);

	/**
	 * Check if the cookie needs to be updated or
	 */
	if (!dao_array_isset_fetch(&cookie, &cookies, name, PH_READONLY)) {
		object_init_ex(&cookie, dao_http_cookie_ce);
		DAO_MM_ADD_ENTRY(&cookie);

		DAO_MM_CALL_METHOD(NULL, &cookie, "__construct", name, value, &expire, &path, &secure, &domain, &http_only);

		/**
		 * Pass the DI to created cookies
		 */
		DAO_MM_CALL_METHOD(NULL, &cookie, "setdi", &dependency_injector);

		/**
		 * Enable encryption in the cookie
		 */
		if (zend_is_true(&encryption)) {
			DAO_MM_CALL_METHOD(NULL, &cookie, "useencryption", &encryption);
		}

		dao_update_property_array(getThis(), SL("_cookies"), name, &cookie);
	} else {
		/**
		 * Override any settings in the cookie
		 */
		DAO_MM_CALL_METHOD(NULL, &cookie, "setvalue", value);
		DAO_MM_CALL_METHOD(NULL, &cookie, "setexpiration", &expire);
		DAO_MM_CALL_METHOD(NULL, &cookie, "setpath", &path);
		DAO_MM_CALL_METHOD(NULL, &cookie, "setsecure", &secure);
		DAO_MM_CALL_METHOD(NULL, &cookie, "setdomain", &domain);
		DAO_MM_CALL_METHOD(NULL, &cookie, "sethttponly", &http_only);
	}

	/**
	 * Register the cookies bag in the response
	 */
	dao_read_property(&registered, getThis(), SL("_registered"), PH_NOISY|PH_READONLY);
	if (DAO_IS_FALSE(&registered)) {
		if (Z_TYPE(dependency_injector) != IS_OBJECT) {
			DAO_MM_THROW_EXCEPTION_STR(dao_http_cookie_exception_ce, "A dependency injection object is required to access the 'response' service");
			return;
		}

		ZVAL_STR(&service, IS(response));

		DAO_MM_CALL_METHOD(&response, &dependency_injector, "getshared", &service);
		DAO_MM_ADD_ENTRY(&response);
		DAO_MM_VERIFY_INTERFACE(&response, dao_http_responseinterface_ce);

		/**
		 * Pass the cookies bag to the response so it can send the headers at the of the
		 * request
		 */
		DAO_MM_CALL_METHOD(NULL, &response, "setcookies", getThis());
	}

	RETURN_MM_THIS();
}

/**
 * Gets a cookie from the bag
 *
 * @param string $name
 * @return Dao\Http\Cookie
 */
PHP_METHOD(Dao_Http_Response_Cookies, get){

	zval *name, cookies = {}, dependency_injector = {}, encryption = {};

	dao_fetch_params(1, 1, 0, &name);

	if (Z_TYPE_P(name) != IS_STRING) {
		DAO_MM_THROW_EXCEPTION_STR(dao_http_cookie_exception_ce, "The cookie name must be string");
		return;
	}

	dao_read_property(&cookies, getThis(), SL("_cookies"), PH_NOISY|PH_READONLY);
	if (!dao_array_isset_fetch(return_value, &cookies, name, PH_COPY)) {
		/**
		 * Create the cookie if the it does not exist
		 */
		object_init_ex(return_value, dao_http_cookie_ce);
		DAO_MM_CALL_METHOD(NULL, return_value, "__construct", name);

		DAO_MM_CALL_METHOD(&dependency_injector, getThis(), "getdi");
		DAO_MM_ADD_ENTRY(&dependency_injector);
		if (Z_TYPE(dependency_injector) == IS_OBJECT) {
			DAO_MM_CALL_METHOD(NULL, return_value, "setdi", &dependency_injector);

			dao_read_property(&encryption, getThis(), SL("_useEncryption"), PH_NOISY|PH_READONLY);
			if (zend_is_true(&encryption)) {
				DAO_MM_CALL_METHOD(NULL, return_value, "useencryption", &encryption);
			}
		}

		dao_update_property_array(getThis(), SL("_cookies"), name, return_value);
	}
	RETURN_MM();
}

/**
 * Check if a cookie is defined in the bag or exists in the $_COOKIE superglobal
 *
 * @param string $name
 * @return boolean
 */
PHP_METHOD(Dao_Http_Response_Cookies, has){

	zval *name, cookies = {}, *_COOKIE;

	dao_fetch_params(0, 1, 0, &name);

	dao_read_property(&cookies, getThis(), SL("_cookies"), PH_NOISY|PH_READONLY);

	/* Check the internal bag */
	if (dao_array_isset(&cookies, name)) {
		RETURN_TRUE;
	}

	/* Check the superglobal */
	_COOKIE = dao_get_global_str(SL("_COOKIE"));
	RETURN_BOOL(dao_array_isset(_COOKIE, name));
}

/**
 * Deletes a cookie by its name
 * This method does not removes cookies from the $_COOKIE superglobal
 *
 * @param string $name
 * @return boolean
 */
PHP_METHOD(Dao_Http_Response_Cookies, delete){

	zval *name, cookies = {}, cookie = {}, *_COOKIE, dependency_injector = {};

	dao_fetch_params(1, 1, 0, &name);

	dao_read_property(&cookies, getThis(), SL("_cookies"), PH_NOISY|PH_READONLY);

	/**
	 * Check the internal bag
	 */
	if (dao_array_isset_fetch(&cookie, &cookies, name, PH_READONLY)) {
		DAO_MM_CALL_METHOD(NULL, &cookie, "delete");
		RETURN_MM_TRUE;
	}

	_COOKIE = dao_get_global_str(SL("_COOKIE"));
	if (dao_array_isset(_COOKIE, name)) {
		DAO_MM_CALL_METHOD(&dependency_injector, getThis(), "getdi");
		DAO_MM_ADD_ENTRY(&dependency_injector);

		object_init_ex(&cookie, dao_http_cookie_ce);
		DAO_MM_ADD_ENTRY(&cookie);

		DAO_MM_CALL_METHOD(NULL, &cookie, "__construct", name);
		DAO_MM_CALL_METHOD(NULL, &cookie, "setdi", &dependency_injector);
		DAO_MM_CALL_METHOD(NULL, &cookie, "restore");
		DAO_MM_CALL_METHOD(NULL, &cookie, "delete");

		RETURN_MM_TRUE;
	}

	RETURN_MM_FALSE;
}

/**
 * Sends the cookies to the client
 * Cookies aren't sent if headers are sent in the current request
 *
 * @return boolean
 */
PHP_METHOD(Dao_Http_Response_Cookies, send){

	zval cookies = {}, *cookie;

	if (!SG(headers_sent)) {
		dao_read_property(&cookies, getThis(), SL("_cookies"), PH_NOISY|PH_READONLY);

		ZEND_HASH_FOREACH_VAL(Z_ARRVAL(cookies), cookie) {
			DAO_CALL_METHOD(NULL, cookie, "send");
		} ZEND_HASH_FOREACH_END();

		RETURN_TRUE;
	}

	RETURN_FALSE;
}

/**
 * Reset set cookies
 *
 * @return Dao\Http\Response\Cookies
 */
PHP_METHOD(Dao_Http_Response_Cookies, reset){

	dao_update_property_empty_array(getThis(), SL("_cookies"));
	RETURN_THIS();
}
