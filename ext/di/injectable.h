
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

#ifndef DAO_DI_INJECTABLE_H
#define DAO_DI_INJECTABLE_H

#include "php_dao.h"

extern zend_class_entry *dao_di_injectable_ce;

DAO_INIT_CLASS(Dao_Di_Injectable);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_di_injectable_fireevent, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, eventName, IS_STRING, 0)
	ZEND_ARG_INFO(0, data)
	ZEND_ARG_TYPE_INFO(0, cancelable, _IS_BOOL, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_di_injectable_fireeventcancel, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, eventName, IS_STRING, 0)
	ZEND_ARG_INFO(0, data)
	ZEND_ARG_TYPE_INFO(0, cancelable, _IS_BOOL, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_di_injectable_hasservice, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_di_injectable_setservice, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_di_injectable_getservice, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_di_injectable_getresolveservice, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, args, IS_ARRAY, 1)
	ZEND_ARG_TYPE_INFO(0, noerror, _IS_BOOL, 1)
	ZEND_ARG_TYPE_INFO(0, noshared, _IS_BOOL, 1)
ZEND_END_ARG_INFO()

#endif /* DAO_DI_INJECTABLE_H */
