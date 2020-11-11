
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

#ifndef DAO_ESCAPERINTERFACE_H
#define DAO_ESCAPERINTERFACE_H

#include "php_dao.h"

extern zend_class_entry *dao_escaperinterface_ce;

DAO_INIT_CLASS(Dao_EscaperInterface);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_escaperinterface_setencoding, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, encoding, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_escaperinterface_sethtmlquotetype, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, quoteType, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_escaperinterface_escapehtml, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, text, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_escaperinterface_escapehtmlattr, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, attr, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_escaperinterface_escapecss, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, css, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_escaperinterface_escapejs, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, js, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_escaperinterface_escapeurl, 0, 0, 1)
	ZEND_ARG_INFO(0, url)
ZEND_END_ARG_INFO()

#endif /* DAO_ESCAPERINTERFACE_H */
