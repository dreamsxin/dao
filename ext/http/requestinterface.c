
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

#include "http/requestinterface.h"
#include "kernel/main.h"

zend_class_entry *dao_http_requestinterface_ce;

static const zend_function_entry dao_http_requestinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Http_RequestInterface, get, arginfo_dao_http_requestinterface_get)
	PHP_ABSTRACT_ME(Dao_Http_RequestInterface, getPost, arginfo_dao_http_requestinterface_getpost)
	PHP_ABSTRACT_ME(Dao_Http_RequestInterface, getPut, arginfo_dao_http_requestinterface_getput)
	PHP_ABSTRACT_ME(Dao_Http_RequestInterface, getQuery, arginfo_dao_http_requestinterface_getquery)
	PHP_ABSTRACT_ME(Dao_Http_RequestInterface, getServer, arginfo_dao_http_requestinterface_getserver)
	PHP_ABSTRACT_ME(Dao_Http_RequestInterface, getParam, arginfo_dao_http_requestinterface_getparam)
	PHP_ABSTRACT_ME(Dao_Http_RequestInterface, has, arginfo_dao_http_requestinterface_has)
	PHP_ABSTRACT_ME(Dao_Http_RequestInterface, hasPost, arginfo_dao_http_requestinterface_haspost)
	PHP_ABSTRACT_ME(Dao_Http_RequestInterface, hasPut, arginfo_dao_http_requestinterface_hasput)
	PHP_ABSTRACT_ME(Dao_Http_RequestInterface, hasQuery, arginfo_dao_http_requestinterface_hasquery)
	PHP_ABSTRACT_ME(Dao_Http_RequestInterface, hasServer, arginfo_dao_http_requestinterface_hasserver)
	PHP_ABSTRACT_ME(Dao_Http_RequestInterface, getHeader, arginfo_dao_http_requestinterface_getheader)
	PHP_ABSTRACT_ME(Dao_Http_RequestInterface, getScheme, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Http_RequestInterface, isAjax, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Http_RequestInterface, isSoapRequested, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Http_RequestInterface, isSecureRequest, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Http_RequestInterface, getRawBody, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Http_RequestInterface, getServerAddress, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Http_RequestInterface, getServerName, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Http_RequestInterface, getHttpHost, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Http_RequestInterface, getClientAddress, arginfo_dao_http_requestinterface_getclientaddress)
	PHP_ABSTRACT_ME(Dao_Http_RequestInterface, getMethod, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Http_RequestInterface, getUserAgent, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Http_RequestInterface, isMethod, arginfo_dao_http_requestinterface_ismethod)
	PHP_ABSTRACT_ME(Dao_Http_RequestInterface, isPost, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Http_RequestInterface, isGet, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Http_RequestInterface, isPut, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Http_RequestInterface, isHead, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Http_RequestInterface, isDelete, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Http_RequestInterface, isOptions, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Http_RequestInterface, hasFiles, arginfo_dao_http_requestinterface_hasfiles)
	PHP_ABSTRACT_ME(Dao_Http_RequestInterface, getUploadedFiles, arginfo_dao_http_requestinterface_getuploadedfiles)
	PHP_ABSTRACT_ME(Dao_Http_RequestInterface, getHTTPReferer, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Http_RequestInterface, getAcceptableContent, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Http_RequestInterface, getBestAccept, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Http_RequestInterface, getClientCharsets, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Http_RequestInterface, getBestCharset, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Http_RequestInterface, getLanguages, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Http_RequestInterface, getBestLanguage, arginfo_empty)
	PHP_FE_END
};

/**
 * Dao\Http\RequestInterface initializer
 */
DAO_INIT_CLASS(Dao_Http_RequestInterface){

	DAO_REGISTER_INTERFACE(Dao\\Http, RequestInterface, http_requestinterface, dao_http_requestinterface_method_entry);

	return SUCCESS;
}

/**
 * Gets a variable from the $_REQUEST superglobal applying filters if needed
 *
 * @param string $name
 * @param string|array $filters
 * @param mixed $defaultValue
 * @return mixed
 */
DAO_DOC_METHOD(Dao_Http_RequestInterface, get);

/**
 * Gets a variable from the $_POST superglobal applying filters if needed
 *
 * @param string $name
 * @param string|array $filters
 * @param mixed $defaultValue
 * @return mixed
 */
DAO_DOC_METHOD(Dao_Http_RequestInterface, getPost);

/**
 * Gets variable from $_GET superglobal applying filters if needed
 *
 * @param string $name
 * @param string|array $filters
 * @param mixed $defaultValue
 * @return mixed
 */
DAO_DOC_METHOD(Dao_Http_RequestInterface, getQuery);

/**
 * Gets variable from $_SERVER superglobal
 *
 * @param string $name
 * @return mixed
 */
DAO_DOC_METHOD(Dao_Http_RequestInterface, getServer);

/**
 * Checks whether $_SERVER superglobal has certain index
 *
 * @param string $name
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Http_RequestInterface, has);

/**
 * Checks whether $_POST superglobal has certain index
 *
 * @param string $name
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Http_RequestInterface, hasPost);

/**
 * Checks whether $_SERVER superglobal has certain index
 *
 * @param string $name
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Http_RequestInterface, hasQuery);

/**
 * Checks whether $_SERVER superglobal has certain index
 *
 * @param string $name
 * @return mixed
 */
DAO_DOC_METHOD(Dao_Http_RequestInterface, hasServer);

/**
 * Gets HTTP header from request data
 *
 * @param string $header
 * @return string
 */
DAO_DOC_METHOD(Dao_Http_RequestInterface, getHeader);

/**
 * Gets HTTP schema (http/https)
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Http_RequestInterface, getScheme);

/**
 * Checks whether request has been made using ajax. Checks if $_SERVER['HTTP_X_REQUESTED_WITH']=='XMLHttpRequest'
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Http_RequestInterface, isAjax);

/**
 * Checks whether request has been made using SOAP
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Http_RequestInterface, isSoapRequested);

/**
 * Checks whether request has been made using any secure layer
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Http_RequestInterface, isSecureRequest);

/**
 * Gets HTTP raws request body
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Http_RequestInterface, getRawBody);

/**
 * Gets active server address IP
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Http_RequestInterface, getServerAddress);

/**
 * Gets active server name
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Http_RequestInterface, getServerName);

/**
 * Gets information about schema, host and port used by the request
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Http_RequestInterface, getHttpHost);

/**
 * Gets most possibly client IPv4 Address. This methods search in $_SERVER['REMOTE_ADDR'] and optionally in $_SERVER['HTTP_X_FORWARDED_FOR']
 *
 * @param boolean $trustForwardedHeader
 * @return string
 */
DAO_DOC_METHOD(Dao_Http_RequestInterface, getClientAddress);

/**
 * Gets HTTP method which request has been made
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Http_RequestInterface, getMethod);

/**
 * Gets HTTP user agent used to made the request
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Http_RequestInterface, getUserAgent);

/**
 * Check if HTTP method match any of the passed methods
 *
 * @param string|array $methods
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Http_RequestInterface, isMethod);

/**
 * Checks whether HTTP method is POST. if $_SERVER['REQUEST_METHOD']=='POST'
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Http_RequestInterface, isPost);

/**
 *
 * Checks whether HTTP method is GET. if $_SERVER['REQUEST_METHOD']=='GET'
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Http_RequestInterface, isGet);

/**
 * Checks whether HTTP method is PUT. if $_SERVER['REQUEST_METHOD']=='PUT'
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Http_RequestInterface, isPut);

/**
 * Checks whether HTTP method is HEAD. if $_SERVER['REQUEST_METHOD']=='HEAD'
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Http_RequestInterface, isHead);

/**
 * Checks whether HTTP method is DELETE. if $_SERVER['REQUEST_METHOD']=='DELETE'
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Http_RequestInterface, isDelete);

/**
 * Checks whether HTTP method is OPTIONS. if $_SERVER['REQUEST_METHOD']=='OPTIONS'
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Http_RequestInterface, isOptions);

/**
 * Checks whether request include attached files
 *
 * @param boolean $notErrored
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Http_RequestInterface, hasFiles);

/**
 * Gets attached files as Dao\Http\Request\FileInterface compatible instances
 *
 * @param boolean $notErrored
 * @return Dao\Http\Request\FileInterface[]
 */
DAO_DOC_METHOD(Dao_Http_RequestInterface, getUploadedFiles);

/**
 * Gets web page that refers active request. ie: http://www.google.com
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Http_RequestInterface, getHTTPReferer);

/**
 * Gets array with mime/types and their quality accepted by the browser/client from $_SERVER['HTTP_ACCEPT']
 *
 * @return array
 */
DAO_DOC_METHOD(Dao_Http_RequestInterface, getAcceptableContent);

/**
 * Gets best mime/type accepted by the browser/client from $_SERVER['HTTP_ACCEPT']
 *
 * @return array
 */
DAO_DOC_METHOD(Dao_Http_RequestInterface, getBestAccept);

/**
 * Gets charsets array and their quality accepted by the browser/client from $_SERVER['HTTP_ACCEPT_CHARSET']
 *
 * @return array
 */
DAO_DOC_METHOD(Dao_Http_RequestInterface, getClientCharsets);

/**
 * Gets best charset accepted by the browser/client from $_SERVER['HTTP_ACCEPT_CHARSET']
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Http_RequestInterface, getBestCharset);

/**
 * Gets languages array and their quality accepted by the browser/client from $_SERVER['HTTP_ACCEPT_LANGUAGE']
 *
 * @return array
 */
DAO_DOC_METHOD(Dao_Http_RequestInterface, getLanguages);

/**
 * Gets best language accepted by the browser/client from $_SERVER['HTTP_ACCEPT_LANGUAGE']
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Http_RequestInterface, getBestLanguage);
