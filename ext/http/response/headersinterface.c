
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

#include "http/response/headersinterface.h"
#include "kernel/main.h"

zend_class_entry *dao_http_response_headersinterface_ce;

static const zend_function_entry dao_http_response_headersinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Http_Response_HeadersInterface, set, arginfo_dao_http_response_headersinterface_set)
	PHP_ABSTRACT_ME(Dao_Http_Response_HeadersInterface, get, arginfo_dao_http_response_headersinterface_get)
	PHP_ABSTRACT_ME(Dao_Http_Response_HeadersInterface, setRaw, arginfo_dao_http_response_headersinterface_setraw)
	PHP_ABSTRACT_ME(Dao_Http_Response_HeadersInterface, send, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Http_Response_HeadersInterface, reset, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Http_Response_HeadersInterface, toArray, arginfo_empty)
	PHP_FE_END
};

/**
 * Dao\Http\Response\HeadersInterface initializer
 */
DAO_INIT_CLASS(Dao_Http_Response_HeadersInterface){

	DAO_REGISTER_INTERFACE(Dao\\Http\\Response, HeadersInterface, http_response_headersinterface, dao_http_response_headersinterface_method_entry);

	return SUCCESS;
}

/**
 * Sets a header to be sent at the end of the request
 *
 * @param string $name
 * @param string $value
 */
DAO_DOC_METHOD(Dao_Http_Response_HeadersInterface, set);

/**
 * Gets a header value from the internal bag
 *
 * @param string $name
 * @return string
 */
DAO_DOC_METHOD(Dao_Http_Response_HeadersInterface, get);

/**
 * Sets a raw header to be sent at the end of the request
 *
 * @param string $header
 */
DAO_DOC_METHOD(Dao_Http_Response_HeadersInterface, setRaw);

/**
 * Sends the headers to the client
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Http_Response_HeadersInterface, send);

/**
 * Reset set headers
 *
 */
DAO_DOC_METHOD(Dao_Http_Response_HeadersInterface, reset);

/**
 * Returns the current headers as an array
 *
 * @return array
 */
DAO_DOC_METHOD(Dao_Http_Response_HeadersInterface, toArray);
