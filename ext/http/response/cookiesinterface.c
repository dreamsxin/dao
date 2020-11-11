
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

#include "http/response/cookiesinterface.h"
#include "kernel/main.h"

zend_class_entry *dao_http_response_cookiesinterface_ce;

static const zend_function_entry dao_http_response_cookiesinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Http_Response_CookiesInterface, useEncryption, arginfo_dao_http_response_cookiesinterface_useencryption)
	PHP_ABSTRACT_ME(Dao_Http_Response_CookiesInterface, isUsingEncryption, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Http_Response_CookiesInterface, set, arginfo_dao_http_response_cookiesinterface_set)
	PHP_ABSTRACT_ME(Dao_Http_Response_CookiesInterface, get, arginfo_dao_http_response_cookiesinterface_get)
	PHP_ABSTRACT_ME(Dao_Http_Response_CookiesInterface, has, arginfo_dao_http_response_cookiesinterface_has)
	PHP_ABSTRACT_ME(Dao_Http_Response_CookiesInterface, delete, arginfo_dao_http_response_cookiesinterface_delete)
	PHP_ABSTRACT_ME(Dao_Http_Response_CookiesInterface, send, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Http_Response_CookiesInterface, reset, arginfo_empty)
	PHP_FE_END
};

/**
 * Dao\Http\Response\CookiesInterface initializer
 */
DAO_INIT_CLASS(Dao_Http_Response_CookiesInterface){

	DAO_REGISTER_INTERFACE(Dao\\Http\\Response, CookiesInterface, http_response_cookiesinterface, dao_http_response_cookiesinterface_method_entry);

	return SUCCESS;
}

/**
 * Set if cookies in the bag must be automatically encrypted/decrypted
 *
 * @param boolean $useEncryption
 * @return Dao\Http\Response\CookiesInterface
 */
DAO_DOC_METHOD(Dao_Http_Response_CookiesInterface, useEncryption);

/**
 * Returns if the bag is automatically encrypting/decrypting cookies
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Http_Response_CookiesInterface, isUsingEncryption);

/**
 * Sets a cookie to be sent at the end of the request
 *
 * @param string $name
 * @param mixed $value
 * @param int $expire
 * @param string $path
 * @param boolean $secure
 * @param string $domain
 * @param boolean $httpOnly
 * @return Dao\Http\Response\CookiesInterface
 */
DAO_DOC_METHOD(Dao_Http_Response_CookiesInterface, set);

/**
 * Gets a cookie from the bag
 *
 * @param string $name
 * @return Dao\Http\Cookie
 */
DAO_DOC_METHOD(Dao_Http_Response_CookiesInterface, get);

/**
 * Check if a cookie is defined in the bag or exists in the $_COOKIE superglobal
 *
 * @param string $name
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Http_Response_CookiesInterface, has);

/**
 * Deletes a cookie by its name
 * This method does not removes cookies from the $_COOKIE superglobal
 *
 * @param string $name
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Http_Response_CookiesInterface, delete);

/**
 * Sends the cookies to the client
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Http_Response_CookiesInterface, send);

/**
 * Reset set cookies
 *
 * @return Dao\Http\Response\CookiesInterface
 */
DAO_DOC_METHOD(Dao_Http_Response_CookiesInterface, reset);
