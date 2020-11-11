
/*
  +------------------------------------------------------------------------+
  | Dao Framework                                                          |
  +------------------------------------------------------------------------+
  | Copyright (c) 2011-2013 Phalcon Team (http://www.phalconphp.com)       |
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

#include "http/client/header.h"

#include <ext/spl/spl_iterators.h>

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/operators.h"
#include "kernel/object.h"
#include "kernel/array.h"
#include "kernel/concat.h"
#include "kernel/fcall.h"
#include "kernel/exception.h"
#include "kernel/file.h"
#include "kernel/hash.h"
#include "kernel/string.h"

/**
 * Dao\Http\Client\Header
 */
zend_class_entry *dao_http_client_header_ce;

PHP_METHOD(Dao_Http_Client_Header, __construct);
PHP_METHOD(Dao_Http_Client_Header, set);
PHP_METHOD(Dao_Http_Client_Header, setStatusCode);
PHP_METHOD(Dao_Http_Client_Header, setMultiple);
PHP_METHOD(Dao_Http_Client_Header, addMultiple);
PHP_METHOD(Dao_Http_Client_Header, get);
PHP_METHOD(Dao_Http_Client_Header, getStatusCode);
PHP_METHOD(Dao_Http_Client_Header, getAll);
PHP_METHOD(Dao_Http_Client_Header, remove);
PHP_METHOD(Dao_Http_Client_Header, parse);
PHP_METHOD(Dao_Http_Client_Header, build);
PHP_METHOD(Dao_Http_Client_Header, count);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_http_client_header_set, 0, 0, 2)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_http_client_header_setstatuscode, 0, 0, 1)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_http_client_header_setmultiple, 0, 0, 1)
	ZEND_ARG_INFO(0, values)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_http_client_header_addmultiple, 0, 0, 1)
	ZEND_ARG_INFO(0, values)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_http_client_header_get, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_http_client_header_remove, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_http_client_header_parse, 0, 0, 1)
	ZEND_ARG_INFO(0, content)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_http_client_header_build, 0, 0, 0)
	ZEND_ARG_INFO(0, flags)
ZEND_END_ARG_INFO()


static const zend_function_entry dao_http_client_header_method_entry[] = {
	PHP_ME(Dao_Http_Client_Header, __construct, arginfo_empty, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Http_Client_Header, set, arginfo_dao_http_client_header_set, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Client_Header, setStatusCode, arginfo_dao_http_client_header_setstatuscode, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Client_Header, setMultiple, arginfo_dao_http_client_header_setmultiple, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Client_Header, addMultiple, arginfo_dao_http_client_header_addmultiple, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Client_Header, get, arginfo_dao_http_client_header_get, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Client_Header, getStatusCode, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Client_Header, getAll, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Client_Header, remove, arginfo_dao_http_client_header_remove, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Client_Header, parse, arginfo_dao_http_client_header_parse, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Client_Header, build, arginfo_dao_http_client_header_build, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Client_Header, count, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Http\Client\Header initializer
 */
DAO_INIT_CLASS(Dao_Http_Client_Header){

	DAO_REGISTER_CLASS(Dao\\Http\\Client, Header, http_client_header, dao_http_client_header_method_entry, 0);

	zend_declare_property_null(dao_http_client_header_ce, SL("_messages"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);
	zend_declare_property_null(dao_http_client_header_ce, SL("_fields"), ZEND_ACC_PROTECTED);
	zend_declare_property_long(dao_http_client_header_ce, SL("_version"), 1, ZEND_ACC_PROTECTED);
	zend_declare_property_long(dao_http_client_header_ce, SL("_status_code"), 0, ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_http_client_header_ce, SL("_status_message"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_http_client_header_ce, SL("_status"), ZEND_ACC_PROTECTED);

	zend_declare_class_constant_long(dao_http_client_header_ce, SL("BUILD_STATUS"), DAO_HTTP_CLIENT_HEADER_BUILD_STATUS);
	zend_declare_class_constant_long(dao_http_client_header_ce, SL("BUILD_FIELDS"), DAO_HTTP_CLIENT_HEADER_BUILD_FIELDS);

	zend_class_implements(dao_http_client_header_ce, 1, spl_ce_Countable);

	return SUCCESS;
}

/**
 * Dao\Http\Client\Header constructor
 */
PHP_METHOD(Dao_Http_Client_Header, __construct){

	zval messages = {};

	array_init(&messages);

	dao_array_update_long_string(&messages, 100, SL("Continue"), 0);
	dao_array_update_long_string(&messages, 101, SL("Switching Protocols"), 0);

	dao_array_update_long_string(&messages, 200, SL("OK"), 0);
	dao_array_update_long_string(&messages, 201, SL("Created"), 0);
	dao_array_update_long_string(&messages, 202, SL("Accepted"), 0);
	dao_array_update_long_string(&messages, 203, SL("Non-Authoritative Information"), 0);
	dao_array_update_long_string(&messages, 204, SL("No Content"), 0);
	dao_array_update_long_string(&messages, 205, SL("Reset Content"), 0);
	dao_array_update_long_string(&messages, 206, SL("Partial Content"), 0);

	dao_array_update_long_string(&messages, 300, SL("Multiple Choices"), 0);
	dao_array_update_long_string(&messages, 301, SL("Moved Permanently"), 0);
	dao_array_update_long_string(&messages, 302, SL("Found"), 0);
	dao_array_update_long_string(&messages, 303, SL("See Other"), 0);
	dao_array_update_long_string(&messages, 304, SL("Not Modified"), 0);
	dao_array_update_long_string(&messages, 305, SL("Use Proxy"), 0);
	dao_array_update_long_string(&messages, 306, SL("(Unused)"), 0);
	dao_array_update_long_string(&messages, 307, SL("Temporary Redirect"), 0);

	dao_array_update_long_string(&messages, 400, SL("Bad Request"), 0);
	dao_array_update_long_string(&messages, 401, SL("Unauthorized"), 0);
	dao_array_update_long_string(&messages, 402, SL("Payment Required"), 0);
	dao_array_update_long_string(&messages, 403, SL("Forbidden"), 0);
	dao_array_update_long_string(&messages, 404, SL("Not Found"), 0);
	dao_array_update_long_string(&messages, 405, SL("Method Not Allowed"), 0);
	dao_array_update_long_string(&messages, 406, SL("Not Acceptable"), 0);
	dao_array_update_long_string(&messages, 407, SL("Proxy Authentication Required"), 0);
	dao_array_update_long_string(&messages, 408, SL("Request Timeout"), 0);
	dao_array_update_long_string(&messages, 409, SL("Conflict"), 0);
	dao_array_update_long_string(&messages, 410, SL("Gone"), 0);
	dao_array_update_long_string(&messages, 411, SL("Length Required"), 0);
	dao_array_update_long_string(&messages, 412, SL("Precondition Failed"), 0);
	dao_array_update_long_string(&messages, 413, SL("Request Entity Too Large"), 0);
	dao_array_update_long_string(&messages, 414, SL("Request-URI Too Long"), 0);
	dao_array_update_long_string(&messages, 415, SL("Unsupported Media Type"), 0);
	dao_array_update_long_string(&messages, 416, SL("Requested Range Not Satisfiable"), 0);
	dao_array_update_long_string(&messages, 417, SL("Expectation Failed"), 0);

	dao_array_update_long_string(&messages, 500, SL("Internal Server Error"), 0);
	dao_array_update_long_string(&messages, 501, SL("Not Implemented"), 0);
	dao_array_update_long_string(&messages, 502, SL("Bad Gateway"), 0);
	dao_array_update_long_string(&messages, 503, SL("Service Unavailable"), 0);
	dao_array_update_long_string(&messages, 504, SL("Gateway Timeout"), 0);
	dao_array_update_long_string(&messages, 505, SL("HTTP Version Not Supported"), 0);
	dao_array_update_long_string(&messages, 506, SL("Bandwidth Limit Exceeded"), 0);

	dao_update_static_property_ce(dao_http_client_header_ce, SL("_messages"), &messages);
	dao_update_property_empty_array(getThis(), SL("_fields"));
	zval_ptr_dtor(&messages);
}

PHP_METHOD(Dao_Http_Client_Header, set){

	zval *name, *value;

	dao_fetch_params(0, 2, 0, &name, &value);

	dao_update_property_array(getThis(), SL("_fields"), name, value);

}

PHP_METHOD(Dao_Http_Client_Header, setStatusCode){

	zval *value;

	dao_fetch_params(0, 1, 0, &value);

	dao_update_property(getThis(), SL("_status_code"), value);
}

PHP_METHOD(Dao_Http_Client_Header, setMultiple){

	zval *values;

	dao_fetch_params(0, 1, 0, &values);

	dao_update_property(getThis(), SL("_fields"), values);
}

PHP_METHOD(Dao_Http_Client_Header, addMultiple){

	zval *values, fields = {};

	dao_fetch_params(0, 1, 0, &values);

	dao_read_property(&fields, getThis(), SL("_fields"), PH_NOISY|PH_READONLY);

	dao_array_merge_recursive_n(&fields, values);

	dao_update_property(getThis(), SL("_fields"), &fields);
}

PHP_METHOD(Dao_Http_Client_Header, get){

	zval *name;

	dao_fetch_params(0, 1, 0, &name);

	dao_read_property_array(return_value, getThis(), SL("_fields"), name, PH_COPY);
}

PHP_METHOD(Dao_Http_Client_Header, getStatusCode){

	RETURN_MEMBER(getThis(), "_status_code");
}

PHP_METHOD(Dao_Http_Client_Header, getAll){

	RETURN_MEMBER(getThis(), "_fields");
}

PHP_METHOD(Dao_Http_Client_Header, remove){

	zval *name;

	dao_fetch_params(0, 1, 0, &name);

	dao_unset_property_array(getThis(), SL("_fields"), name);
}

PHP_METHOD(Dao_Http_Client_Header, parse){

	zval *content, content_parts = {}, *header;

	dao_fetch_params(0, 1, 0, &content);

	if (DAO_IS_EMPTY(content)) {
		RETURN_FALSE;
	}

	if (Z_TYPE_P(content) == IS_STRING) {
		dao_fast_explode_str(&content_parts, SL("\r\n"), content);
	} else if (Z_TYPE_P(content) == IS_ARRAY) {
		ZVAL_COPY(&content_parts, content);
	} else {
		RETURN_FALSE;
	}

	ZEND_HASH_FOREACH_VAL(Z_ARRVAL(content_parts), header) {
		zval header_parts = {}, val1 = {}, val2 = {}, trimmed = {};

		if (Z_TYPE_P(header) == IS_STRING) {
			if (dao_memnstr_str(header , SL(":"))) {
				dao_fast_explode_str(&header_parts, SL(":"), header);
			} else {
				if (dao_start_with_str(header , SL("HTTP/"))) {
					dao_fast_explode_str(&header_parts, SL(" "), header);
					if (Z_TYPE(header_parts) == IS_ARRAY && dao_array_isset_fetch_long(&val1, &header_parts, 1, PH_READONLY)
						&& dao_array_isset_fetch_long(&val2, &header_parts, 2, PH_READONLY)) {
						dao_update_property(getThis(), SL("_status_code"), &val1);
						dao_update_property(getThis(), SL("_status_message"), &val2);
					}
					zval_ptr_dtor(&header_parts);
				}
				continue;
			}
		} else {
			ZVAL_COPY(&header_parts, header);
		}

		if (Z_TYPE(header_parts) == IS_ARRAY && dao_array_isset_fetch_long(&val1, &header_parts, 0, PH_READONLY)
			&& dao_array_isset_fetch_long(&val2, &header_parts, 1, PH_READONLY)) {
				ZVAL_STR(&trimmed, dao_trim(&val2, NULL, DAO_TRIM_BOTH));
				DAO_CALL_METHOD(NULL, getThis(), "set", &val1, &trimmed);
				zval_ptr_dtor(&trimmed);
		}
		zval_ptr_dtor(&header_parts);
	} ZEND_HASH_FOREACH_END();
	zval_ptr_dtor(&content_parts);
}

PHP_METHOD(Dao_Http_Client_Header, build)
{
	zval *flags = NULL, messages = {}, status_code = {}, lines = {}, message = {}, version = {}, fields = {}, *value, join_filed = {};
	zend_string *str_key;
	ulong idx;
	int f = 0;

	dao_fetch_params(0, 0, 1, &flags);

	if (flags) {
		f = dao_get_intval(flags);
	}

	dao_read_static_property_ce(&messages, dao_http_client_header_ce, SL("_messages"), PH_READONLY);
	dao_read_property(&status_code, getThis(), SL("_status_code"), PH_NOISY|PH_READONLY);

	array_init(&lines);

	if ((f & DAO_HTTP_CLIENT_HEADER_BUILD_STATUS) && dao_array_isset_fetch(&message, &messages, &status_code, PH_READONLY)) {
		zval line = {};
		dao_read_property(&version, getThis(), SL("_version "), PH_NOISY|PH_READONLY);

		DAO_CONCAT_SVS(&line, "HTTP/", &version, " ");
		DAO_SCONCAT_VSV(&line, &status_code, " ", &message);

		dao_merge_append(&lines, &line);
		zval_ptr_dtor(&line);
	}

	dao_read_property(&fields, getThis(), SL("_fields"), PH_NOISY|PH_READONLY);

	ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL(fields), idx, str_key, value) {
		zval filed = {}, tmp = {};
		if (str_key) {
			ZVAL_STR(&filed, str_key);
		} else {
			ZVAL_LONG(&filed, idx);
		}

		DAO_CONCAT_VSV(&tmp, &filed, ": ", value);

		dao_merge_append(&lines, &tmp);
		zval_ptr_dtor(&tmp);
	} ZEND_HASH_FOREACH_END();

	if (f & DAO_HTTP_CLIENT_HEADER_BUILD_FIELDS) {
		dao_fast_join_str(&join_filed, SL("\r\n"), &lines);
		zval_ptr_dtor(&lines);

		RETURN_ZVAL(&join_filed, 0, 0);
	}

	RETURN_ZVAL(&lines, 0, 0);
}

PHP_METHOD(Dao_Http_Client_Header, count){

	zval fields = {};

	dao_read_property(&fields, getThis(), SL("_fields"), PH_NOISY|PH_READONLY);
	dao_fast_count(return_value, &fields);
}
