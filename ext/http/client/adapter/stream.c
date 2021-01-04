
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
  +------------------------------------------------------------------------+
*/

#include "http/client/adapter/stream.h"
#include "http/client/adapter.h"
#include "http/client/adapterinterface.h"
#include "http/client/header.h"
#include "http/client/response.h"
#include "http/client/exception.h"
#include "http/uri.h"
#include "debug.h"

#include "kernel/main.h"
#include "kernel/exception.h"
#include "kernel/memory.h"
#include "kernel/operators.h"
#include "kernel/object.h"
#include "kernel/array.h"
#include "kernel/concat.h"
#include "kernel/fcall.h"
#include "kernel/file.h"
#include "kernel/hash.h"
#include "kernel/string.h"
#include "kernel/debug.h"

/**
 * Dao\Http\Client\Adapter\Stream
 */
zend_class_entry *dao_http_client_adapter_stream_ce;

PHP_METHOD(Dao_Http_Client_Adapter_Stream, __construct);
PHP_METHOD(Dao_Http_Client_Adapter_Stream, buildBody);
PHP_METHOD(Dao_Http_Client_Adapter_Stream, errorHandler);
PHP_METHOD(Dao_Http_Client_Adapter_Stream, sendInternal);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_http_client_adapter_stream_errorhandler, 0, 0, 5)
	ZEND_ARG_INFO(0, errno)
	ZEND_ARG_INFO(0, errstr)
	ZEND_ARG_INFO(0, errfile)
	ZEND_ARG_INFO(0, errline)
	ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_http_client_adapter_stream_method_entry[] = {
	PHP_ME(Dao_Http_Client_Adapter_Stream, __construct, arginfo_dao_http_client_adapterinterface___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Http_Client_Adapter_Stream, buildBody, arginfo_empty, ZEND_ACC_PROTECTED)
	PHP_ME(Dao_Http_Client_Adapter_Stream, errorHandler, arginfo_dao_http_client_adapter_stream_errorhandler, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Client_Adapter_Stream, sendInternal, arginfo_empty, ZEND_ACC_PROTECTED)
	PHP_FE_END
};

/**
 * Dao\Http\Client\Adapter\Stream initializer
 */
DAO_INIT_CLASS(Dao_Http_Client_Adapter_Stream){

	DAO_REGISTER_CLASS_EX(Dao\\Http\\Client\\Adapter, Stream, http_client_adapter_stream, dao_http_client_adapter_ce,  dao_http_client_adapter_stream_method_entry, 0);

	zend_declare_property_null(dao_http_client_adapter_stream_ce, SL("_stream"), ZEND_ACC_PROTECTED);

	zend_class_implements(dao_http_client_adapter_stream_ce, 1, dao_http_client_adapterinterface_ce);

	return SUCCESS;
}

PHP_METHOD(Dao_Http_Client_Adapter_Stream, __construct){

	zval *uri = NULL, *method = NULL, header = {}, stream = {}, http = {}, option = {}, value = {};

	dao_fetch_params(0, 0, 2, &uri, &method);

	if (uri) {
		DAO_CALL_METHOD(NULL, getThis(), "setbaseuri", uri);
	}

	if (method) {
		zval upper_method = {};
		dao_fast_strtoupper(&upper_method, method);
		dao_update_property(getThis(), SL("_method"), &upper_method);
		zval_ptr_dtor(&upper_method);
	}

	object_init_ex(&header, dao_http_client_header_ce);
	DAO_CALL_METHOD(NULL, &header, "__construct");

	DAO_CALL_FUNCTION(&stream, "stream_context_create");

	ZVAL_STRING(&http, "http");
	ZVAL_STRING(&option, "user_agent");
	ZVAL_STRING(&value, "Dao HTTP Client(Stream)");

	DAO_CALL_METHOD(NULL, &header, "set", &option, &value);
	zval_ptr_dtor(&option);
	zval_ptr_dtor(&value);

	dao_update_property(getThis(), SL("_header"), &header);
	zval_ptr_dtor(&header);

	ZVAL_STRING(&option, "follow_location");
	ZVAL_LONG(&value, 1);
	DAO_CALL_FUNCTION(NULL, "stream_context_set_option", &stream, &http, &option, &value);
	zval_ptr_dtor(&option);
	zval_ptr_dtor(&value);

	ZVAL_STRING(&option, "protocol_version");
	ZVAL_DOUBLE(&value, 1.1);
	DAO_CALL_FUNCTION(NULL, "stream_context_set_option", &stream, &http, &option, &value);
	zval_ptr_dtor(&option);
	zval_ptr_dtor(&value);

	ZVAL_STRING(&option, "max_redirects");
	ZVAL_LONG(&value, 20);
	DAO_CALL_FUNCTION(NULL, "stream_context_set_option", &stream, &http, &option, &value);
	zval_ptr_dtor(&option);
	zval_ptr_dtor(&value);

	zval_ptr_dtor(&http);

	dao_update_property(getThis(), SL("_stream"), &stream);
	zval_ptr_dtor(&stream);
}

PHP_METHOD(Dao_Http_Client_Adapter_Stream, buildBody){

	zval stream = {}, header = {}, data = {}, type = {}, files = {}, username = {}, password = {}, authtype = {}, digest = {}, method = {}, entity_body = {};
	zval key = {}, key_value = {}, realm = {}, qop = {}, ha2_txt = {}, ha2 = {}, nonce = {};
	zval nc = {}, cnonce = {}, qoc = {}, digest_value = {}, path = {}, md5_entity_body = {};
	zval http = {}, option = {}, body = {}, headers = {}, uniqid = {}, boundary = {}, *value, *file;
	zend_string *str_key;
	ulong idx;

	DAO_MM_INIT();

	dao_read_property(&stream, getThis(), SL("_stream"), PH_NOISY|PH_READONLY);
	dao_read_property(&header, getThis(), SL("_header"), PH_NOISY|PH_READONLY);
	dao_read_property(&data, getThis(), SL("_data"), PH_NOISY|PH_READONLY);
	dao_read_property(&type, getThis(), SL("_type"), PH_NOISY|PH_READONLY);
	dao_read_property(&files, getThis(), SL("_files"), PH_NOISY|PH_READONLY);
	dao_read_property(&username, getThis(), SL("_username"), PH_NOISY|PH_READONLY);
	dao_read_property(&password, getThis(), SL("_password"), PH_NOISY|PH_READONLY);
	dao_read_property(&authtype, getThis(), SL("_authtype"), PH_NOISY|PH_READONLY);
	dao_read_property(&digest, getThis(), SL("_digest"), PH_NOISY|PH_READONLY);
	dao_read_property(&method, getThis(), SL("_method"), PH_NOISY|PH_READONLY);
	dao_read_property(&entity_body, getThis(), SL("_entity_body"), PH_NOISY|PH_READONLY);

	if (DAO_IS_NOT_EMPTY(&username)) {
		if (DAO_IS_STRING(&authtype, "basic")) {
			DAO_MM_ZVAL_STRING(&key, "Authorization");
			DAO_CONCAT_SVSV(&key_value, "Basic ", &username, ":", &password);
			DAO_MM_ADD_ENTRY(&key_value);

			DAO_MM_CALL_METHOD(NULL, &header, "set", &key, &key_value);
		} else if (DAO_IS_STRING(&authtype, "digest") && DAO_IS_NOT_EMPTY(&digest)) {
			zval ha1_txt = {}, ha1 = {};
			if (!dao_array_isset_fetch_str(&realm, &digest, SL("realm"), PH_READONLY)) {
				ZVAL_NULL(&realm);
			}

			DAO_CONCAT_VSVSV(&ha1_txt, &username, ":", &realm, ":", &password);
			DAO_MM_ADD_ENTRY(&ha1_txt);
			DAO_MM_CALL_FUNCTION(&ha1, "md5", &ha1_txt);
			DAO_MM_ADD_ENTRY(&ha1);

			if (!dao_array_isset_fetch_str(&qop, &digest, SL("qop"), PH_READONLY)) {
				ZVAL_NULL(&qop);
			}

			if (DAO_IS_EMPTY(&qop) || dao_memnstr_str(&qop, SL("auth"))) {
				DAO_MM_CALL_SELF(&path, "getpath");
				DAO_MM_ADD_ENTRY(&path);

				DAO_CONCAT_VSV(&ha2_txt, &method, ":", &path);
				DAO_MM_ADD_ENTRY(&ha2_txt);

				DAO_MM_CALL_FUNCTION(&ha2, "md5", &ha2_txt);
				DAO_MM_ADD_ENTRY(&ha2);

			} else if (dao_memnstr_str(&qop, SL("auth-int"))) {
				DAO_MM_CALL_SELF(&path, "getpath");
				DAO_MM_ADD_ENTRY(&path);
				DAO_MM_CALL_FUNCTION(&md5_entity_body, "md5", &entity_body);
				DAO_MM_ADD_ENTRY(&md5_entity_body);

				DAO_CONCAT_VSVSV(&ha2_txt, &method, ":", &path, ":", &md5_entity_body);
				DAO_MM_ADD_ENTRY(&ha2_txt);

				DAO_MM_CALL_FUNCTION(&ha2, "md5", &ha2_txt);
				DAO_MM_ADD_ENTRY(&ha2);
			}

			DAO_MM_ZVAL_STRING(&key, "Authorization");

			if (!dao_array_isset_fetch_str(&nonce, &digest, SL("nonce"), PH_READONLY)) {
				ZVAL_NULL(&nonce);
			}

			if (DAO_IS_EMPTY(&qop)) {
				DAO_CONCAT_VSVSV(&key_value, &ha1, ":", &nonce, ":", &ha2);
				DAO_MM_ADD_ENTRY(&key_value);
				DAO_CALL_FUNCTION(&digest_value, "md5", &key_value);
				DAO_MM_ADD_ENTRY(&digest_value);

				DAO_CONCAT_SV(&key_value, "Digest ", &digest_value);
				DAO_MM_ADD_ENTRY(&key_value);

				DAO_MM_CALL_METHOD(NULL, &header, "set", &key, &key_value);
			} else {
				if (!dao_array_isset_fetch_str(&nc, &digest, SL("nc"), PH_READONLY)) {
					ZVAL_NULL(&nc);
				}

				if (!dao_array_isset_fetch_str(&cnonce, &digest, SL("cnonce"), PH_READONLY)) {
					ZVAL_NULL(&cnonce);
				}

				if (!dao_array_isset_fetch_str(&qoc, &digest, SL("qoc"), PH_READONLY)) {
					ZVAL_NULL(&qoc);
				}

				DAO_CONCAT_VSVSVS(&key_value, &ha1, ":", &nonce, ":", &nc, ":");
				DAO_MM_ADD_ENTRY(&key_value);
				DAO_SCONCAT_VSVSV(&key_value, &cnonce, ":", &qoc, ":", &ha2);
				DAO_MM_ADD_ENTRY(&key_value);

				DAO_MM_CALL_FUNCTION(&digest_value, "md5", &key_value);
				DAO_MM_ADD_ENTRY(&digest_value);
				DAO_CONCAT_SV(&key_value, "Digest ", &digest_value);
				DAO_MM_ADD_ENTRY(&key_value);

				DAO_MM_CALL_METHOD(NULL, &header, "set", &key, &key_value);
			}
		}
	}

	DAO_MM_ZVAL_STRING(&http, "http");

	if (Z_TYPE(data) == IS_STRING && DAO_IS_NOT_EMPTY(&data)) {
		DAO_MM_ZVAL_STRING(&key, "Content-Type");

		if (DAO_IS_EMPTY(&type)) {
			DAO_MM_CALL_METHOD(&key_value, &header, "get", &key);
			DAO_MM_ADD_ENTRY(&key_value);
			if (DAO_IS_EMPTY(&key_value)) {
				DAO_MM_ZVAL_STRING(&key_value, "application/x-www-form-urlencoded");
				DAO_MM_CALL_METHOD(NULL, &header, "set", &key, &key_value);
			}
		} else {
			DAO_MM_ZVAL_COPY(&key_value, &type);
			DAO_MM_CALL_METHOD(NULL, &header, "set", &key, &key_value);
		}

		DAO_MM_ZVAL_STRING(&key, "Content-Length");

		ZVAL_LONG(&key_value, Z_STRLEN(data));

		DAO_MM_CALL_METHOD(NULL, &header, "set", &key, &key_value);

		DAO_MM_ZVAL_STRING(&option, "content");
		DAO_MM_CALL_FUNCTION(NULL, "stream_context_set_option", &stream, &http, &option, &data);

		if (unlikely(DAO_GLOBAL(debug).enable_debug)) {
			zval debug_message = {};
			ZVAL_STRING(&debug_message, "HTTP REQUEST BODY:");
			DAO_DEBUG_LOG(&debug_message);
			zval_ptr_dtor(&debug_message);
			DAO_DEBUG_LOG(&data);
		}
	} else {

		DAO_MM_CALL_FUNCTION(&uniqid, "uniqid");
		DAO_MM_ADD_ENTRY(&uniqid);
		DAO_CONCAT_SV(&boundary, "--------------", &uniqid);
		DAO_MM_ADD_ENTRY(&boundary);
		if (Z_TYPE(data) == IS_ARRAY) {
			ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL(data), idx, str_key, value) {
				zval key = {};
				if (str_key) {
					ZVAL_STR(&key, str_key);
				} else {
					ZVAL_LONG(&key, idx);
				}

				DAO_SCONCAT_SVS(&body, "--", &boundary, "\r\n");
				DAO_MM_ADD_ENTRY(&body);
				DAO_SCONCAT_SVSVS(&body, "Content-Disposition: form-data; name=\"", &key, "\"\r\n\r\n", value, "\r\n");
				DAO_MM_ADD_ENTRY(&body);
			} ZEND_HASH_FOREACH_END();
		}

		if (Z_TYPE(files) == IS_ARRAY) {
			ZEND_HASH_FOREACH_VAL(Z_ARRVAL(files), file) {
				zval path_parts = {}, filename = {}, basename = {}, filedata = {};
				if (DAO_IS_NOT_EMPTY(file)) {
					DAO_MM_CALL_FUNCTION(&path_parts, "pathinfo", file);
					DAO_MM_ADD_ENTRY(&path_parts);

					if (dao_array_isset_fetch_str(&filename, &path_parts, SL("filename"), PH_READONLY)
						&& dao_array_isset_fetch_str(&basename, &path_parts, SL("basename"), PH_READONLY)) {
						DAO_MM_CALL_FUNCTION(&filedata, "file_get_contents", file);
						DAO_MM_ADD_ENTRY(&filedata);

						DAO_SCONCAT_SVS(&body, "--", &boundary, "\r\n");
						DAO_MM_ADD_ENTRY(&body);
						DAO_SCONCAT_SVSVS(&body, "Content-Disposition: form-data; name=\"", &filename, "\"; filename=\"", &basename, "\"\r\n");
						DAO_MM_ADD_ENTRY(&body);
						DAO_SCONCAT_SVS(&body, "Content-Type: application/octet-stream\r\n\r\n", &filedata, "\r\n");
						DAO_MM_ADD_ENTRY(&body);
					}
				}
			} ZEND_HASH_FOREACH_END();
		}

		if (!DAO_IS_EMPTY(&body)) {
			DAO_SCONCAT_SVS(&body, "--", &boundary, "--\r\n");
			DAO_MM_ADD_ENTRY(&body);

			DAO_MM_ZVAL_STRING(&key, "Content-Type");
			DAO_CONCAT_SV(&key_value, "multipart/form-data; boundary=", &boundary);
			DAO_MM_ADD_ENTRY(&key_value);

			DAO_MM_CALL_METHOD(NULL, &header, "set", &key, &key_value);

			DAO_MM_ZVAL_STRING(&key, "Content-Length");
			ZVAL_LONG(&key_value, Z_STRLEN(body));

			DAO_MM_CALL_METHOD(NULL, &header, "set", &key, &key_value);

			DAO_MM_ZVAL_STRING(&option, "content");
			DAO_MM_CALL_FUNCTION(NULL, "stream_context_set_option", &stream, &http, &option, &body);
		}
		if (unlikely(DAO_GLOBAL(debug).enable_debug)) {
			zval debug_message = {};
			ZVAL_STRING(&debug_message, "HTTP REQUEST BODY:");
			DAO_DEBUG_LOG(&debug_message);
			zval_ptr_dtor(&debug_message);
			DAO_DEBUG_LOG(&body);
		}
	}
	ZVAL_LONG(&option, DAO_HTTP_CLIENT_HEADER_BUILD_FIELDS);

	DAO_MM_CALL_METHOD(&headers, &header, "build", &option);
	DAO_MM_ADD_ENTRY(&headers);

	if (unlikely(DAO_GLOBAL(debug).enable_debug)) {
		zval debug_message = {};
		ZVAL_STRING(&debug_message, "HTTP REQUEST HEADERS:");
		DAO_DEBUG_LOG(&debug_message);
		zval_ptr_dtor(&debug_message);
		DAO_DEBUG_LOG(&headers);
	}

	DAO_MM_ZVAL_STRING(&option, "header");
	DAO_MM_CALL_FUNCTION(NULL, "stream_context_set_option", &stream, &http, &option, &headers);
	RETURN_MM();
}

PHP_METHOD(Dao_Http_Client_Adapter_Stream, errorHandler)
{
	zval *no, *message, *file, *line, *data;

	dao_fetch_params(0, 5, 0, &no, &message, &file, &line, &data);

	DAO_THROW_EXCEPTION_ZVAL(dao_http_client_exception_ce, message);
}

PHP_METHOD(Dao_Http_Client_Adapter_Stream, sendInternal)
{
	zval stream = {}, header = {}, method = {}, useragent = {}, timeout = {}, uri = {}, url = {}, http = {}, option = {}, handler = {};
	zval fp = {}, meta = {}, wrapper_data = {}, bodystr = {}, response = {};

	DAO_MM_INIT();

	dao_read_property(&stream, getThis(), SL("_stream"), PH_NOISY|PH_READONLY);
	dao_read_property(&header, getThis(), SL("_header"), PH_NOISY|PH_READONLY);
	dao_read_property(&method, getThis(), SL("_method"), PH_NOISY|PH_READONLY);
	dao_read_property(&useragent, getThis(), SL("_useragent"), PH_NOISY|PH_READONLY);
	dao_read_property(&timeout, getThis(), SL("_timeout"), PH_NOISY|PH_READONLY);

	DAO_MM_CALL_METHOD(&uri, getThis(), "geturi");
	DAO_MM_ADD_ENTRY(&uri);
	DAO_MM_CALL_METHOD(&url, &uri, "build");
	DAO_MM_ADD_ENTRY(&url);

	if (unlikely(DAO_GLOBAL(debug).enable_debug)) {
		zval debug_message = {};
		DAO_CONCAT_SV(&debug_message, "HTTP REQUEST URL: ", &url);
		DAO_DEBUG_LOG(&debug_message);
		zval_ptr_dtor(&debug_message);
	}

	DAO_MM_ZVAL_STRING(&http, "http");
	DAO_MM_ZVAL_STRING(&option, "method");

	DAO_MM_CALL_FUNCTION(NULL, "stream_context_set_option", &stream, &http, &option, &method);

	if (DAO_IS_NOT_EMPTY(&useragent)) {
		DAO_MM_ZVAL_STRING(&option, "User-Agent");
		DAO_MM_CALL_METHOD(NULL, &header, "set", &option, &useragent);
		DAO_MM_CALL_FUNCTION(NULL, "stream_context_set_option", &stream, &http, &option, &useragent);
	}

	DAO_MM_ZVAL_STRING(&option, "timeout");

	DAO_MM_CALL_FUNCTION(NULL, "stream_context_set_option", &stream, &http, &option, &timeout);

	DAO_MM_CALL_SELF(NULL, "buildBody");

	array_init_size(&handler, 2);
	dao_array_append(&handler, getThis(), PH_COPY);
	add_next_index_stringl(&handler, SL("errorHandler"));
	DAO_MM_ADD_ENTRY(&handler);

	DAO_MM_CALL_FUNCTION(NULL, "set_error_handler", &handler);

	DAO_MM_ZVAL_STRING(&option, "r");
	DAO_MM_CALL_FUNCTION(&fp, "fopen", &url, &option, &DAO_GLOBAL(z_false), &stream);

	DAO_MM_CALL_FUNCTION(NULL, "restore_error_handler");

	DAO_MM_CALL_FUNCTION(&meta, "stream_get_meta_data", &fp);
	DAO_MM_ADD_ENTRY(&meta);
	DAO_MM_CALL_FUNCTION(&bodystr, "stream_get_contents", &fp);
	DAO_MM_ADD_ENTRY(&bodystr);
	DAO_MM_CALL_FUNCTION(NULL, "fclose", &fp);

	object_init_ex(&response, dao_http_client_response_ce);
	DAO_MM_ADD_ENTRY(&response);
	DAO_MM_CALL_METHOD(NULL, &response, "__construct");

	if (dao_array_isset_fetch_str(&wrapper_data, &meta, SL("wrapper_data"), PH_READONLY)) {
		DAO_MM_CALL_METHOD(NULL, &response, "setHeader", &wrapper_data);
	}

	DAO_MM_CALL_METHOD(NULL, &response, "setbody", &bodystr);

	RETURN_MM_CTOR(&response);
}
