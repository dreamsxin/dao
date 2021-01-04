
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

#include "http/responseinterface.h"
#include "kernel/main.h"

zend_class_entry *dao_http_responseinterface_ce;

static const zend_function_entry dao_http_responseinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Http_ResponseInterface, setStatusCode, arginfo_dao_http_responseinterface_setstatuscode)
	PHP_ABSTRACT_ME(Dao_Http_ResponseInterface, getHeaders, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Http_ResponseInterface, setHeader, arginfo_dao_http_responseinterface_setheader)
	PHP_ABSTRACT_ME(Dao_Http_ResponseInterface, setRawHeader, arginfo_dao_http_responseinterface_setrawheader)
	PHP_ABSTRACT_ME(Dao_Http_ResponseInterface, resetHeaders, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Http_ResponseInterface, setExpires, arginfo_dao_http_responseinterface_setexpires)
	PHP_ABSTRACT_ME(Dao_Http_ResponseInterface, setNotModified, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Http_ResponseInterface, setContentType, arginfo_dao_http_responseinterface_setcontenttype)
	PHP_ABSTRACT_ME(Dao_Http_ResponseInterface, redirect, arginfo_dao_http_responseinterface_redirect)
	PHP_ABSTRACT_ME(Dao_Http_ResponseInterface, setContent, arginfo_dao_http_responseinterface_setcontent)
	PHP_ABSTRACT_ME(Dao_Http_ResponseInterface, setJsonContent, arginfo_dao_http_responseinterface_setjsoncontent)
	PHP_ABSTRACT_ME(Dao_Http_ResponseInterface, appendContent, arginfo_dao_http_responseinterface_appendcontent)
	PHP_ABSTRACT_ME(Dao_Http_ResponseInterface, getContent, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Http_ResponseInterface, sendHeaders, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Http_ResponseInterface, sendCookies, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Http_ResponseInterface, send, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Http_ResponseInterface, setFileToSend, arginfo_dao_http_responseinterface_setfiletosend)
	PHP_FE_END
};

/**
 * Dao\Http\ResponseInterface initializer
 */
DAO_INIT_CLASS(Dao_Http_ResponseInterface){

	DAO_REGISTER_INTERFACE(Dao\\Http, ResponseInterface, http_responseinterface, dao_http_responseinterface_method_entry);

	return SUCCESS;
}

/**
 * Sets the HTTP response code
 *
 * @param int $code
 * @param string $message
 * @return Dao\Http\ResponseInterface
 */
DAO_DOC_METHOD(Dao_Http_ResponseInterface, setStatusCode);

/**
 * Returns headers set by the user
 *
 * @return Dao\Http\Response\Headers
 */
DAO_DOC_METHOD(Dao_Http_ResponseInterface, getHeaders);

/**
 * Overwrites a header in the response
 *
 * @param string $name
 * @param string $value
 * @return Dao\Http\ResponseInterface
 */
DAO_DOC_METHOD(Dao_Http_ResponseInterface, setHeader);

/**
 * Send a raw header to the response
 *
 * @param string $header
 * @return Dao\Http\ResponseInterface
 */
DAO_DOC_METHOD(Dao_Http_ResponseInterface, setRawHeader);

/**
 * Resets all the stablished headers
 *
 * @return Dao\Http\ResponseInterface
 */
DAO_DOC_METHOD(Dao_Http_ResponseInterface, resetHeaders);

/**
 * Sets output expire time header
 *
 * @param DateTime $datetime
 * @return Dao\Http\ResponseInterface
 */
DAO_DOC_METHOD(Dao_Http_ResponseInterface, setExpires);

/**
 * Sends a Not-Modified response
 *
 * @return Dao\Http\ResponseInterface
 */
DAO_DOC_METHOD(Dao_Http_ResponseInterface, setNotModified);

/**
 * Sets the response content-type mime, optionally the charset
 *
 * @param string $contentType
 * @param string $charset
 * @return Dao\Http\ResponseInterface
 */
DAO_DOC_METHOD(Dao_Http_ResponseInterface, setContentType);

/**
 * Redirect by HTTP to another action or URL
 *
 * @param string $location
 * @param boolean $externalRedirect
 * @param int $statusCode
 * @return Dao\Http\ResponseInterface
 */
DAO_DOC_METHOD(Dao_Http_ResponseInterface, redirect);

/**
 * Sets HTTP response body
 *
 * @param string $content
 * @return Dao\Http\ResponseInterface
 */
DAO_DOC_METHOD(Dao_Http_ResponseInterface, setContent);

/**
 * Sets HTTP response body. The parameter is automatically converted to JSON
 *
 *<code>
 *	$response->setJsonContent(array("status" => "OK"));
 *</code>
 *
 * @param string $content
 * @return Dao\Http\ResponseInterface
 */
DAO_DOC_METHOD(Dao_Http_ResponseInterface, setJsonContent);

/**
 * Appends a string to the HTTP response body
 *
 * @param string $content
 * @return Dao\Http\ResponseInterface
 */
DAO_DOC_METHOD(Dao_Http_ResponseInterface, appendContent);

/**
 * Gets the HTTP response body
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Http_ResponseInterface, getContent);

/**
 * Sends headers to the client
 *
 * @return Dao\Http\ResponseInterface
 */
DAO_DOC_METHOD(Dao_Http_ResponseInterface, sendHeaders);

/**
 * Sends cookies to the client
 *
 * @return Dao\Http\ResponseInterface
 */
DAO_DOC_METHOD(Dao_Http_ResponseInterface, sendCookies);

/**
 * Prints out HTTP response to the client
 *
 * @return Dao\Http\ResponseInterface
 */
DAO_DOC_METHOD(Dao_Http_ResponseInterface, send);

/**
 * Sets an attached file to be sent at the end of the request
 *
 * @param string $filePath
 * @param string $attachmentName
 */
DAO_DOC_METHOD(Dao_Http_ResponseInterface, setFileToSend);
