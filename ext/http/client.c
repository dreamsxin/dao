
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

#include "http/client.h"
#include "http/client/adapter/curl.h"
#include "http/client/adapter/stream.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"
#include "kernel/object.h"

/**
 * Dao\Http\Client
 *
 * Creates a new request object for the given URI.
 *
 *<code>
 *	$client = Dao\Http\Client::factory();
 *</code>
 *
 */
zend_class_entry *dao_http_client_ce;

PHP_METHOD(Dao_Http_Client, factory);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_http_client_factory, 0, 0, 0)
	ZEND_ARG_INFO(0, uri)
	ZEND_ARG_INFO(0, method)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_http_client_method_entry[] = {
	PHP_ME(Dao_Http_Client, factory, arginfo_dao_http_client_factory, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};

/**
 * Dao\Http\Client initializer
 */
DAO_INIT_CLASS(Dao_Http_Client){

	DAO_REGISTER_CLASS(Dao\\Http, Client, http_client, dao_http_client_method_entry, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	return SUCCESS;
}

PHP_METHOD(Dao_Http_Client, factory)
{
	zval *uri = NULL, *_method = NULL, method = {};

	dao_fetch_params(0, 0, 2, &uri, &method);

	if (!uri) {
		uri = &DAO_GLOBAL(z_null);
	}

	if (!_method) {
		ZVAL_STRING(&method, "GET");
	} else {
		ZVAL_COPY_VALUE(&method, _method);
	}

	if (dao_function_exists_ex(SL("curl_init")) != FAILURE) {
		object_init_ex(return_value, dao_http_client_adapter_curl_ce);
		DAO_CALL_METHOD(NULL, return_value, "__construct", uri, &method);
	} else {
		object_init_ex(return_value, dao_http_client_adapter_stream_ce);
		DAO_CALL_METHOD(NULL, return_value, "__construct", uri, &method);
	}
}
