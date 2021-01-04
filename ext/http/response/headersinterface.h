
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

#ifndef DAO_HTTP_RESPONSE_HEADERSINTERFACE_H
#define DAO_HTTP_RESPONSE_HEADERSINTERFACE_H

#include "php_dao.h"

extern zend_class_entry *dao_http_response_headersinterface_ce;

DAO_INIT_CLASS(Dao_Http_Response_HeadersInterface);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_http_response_headersinterface_set, 0, 0, 2)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_http_response_headersinterface_get, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_http_response_headersinterface_setraw, 0, 0, 1)
	ZEND_ARG_INFO(0, header)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_http_response_headersinterface_remove, 0, 0, 1)
	ZEND_ARG_INFO(0, header_index)
ZEND_END_ARG_INFO()

#endif /* DAO_HTTP_RESPONSE_HEADERSINTERFACE_H */
