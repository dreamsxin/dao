
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

#ifndef DAO_FILTERINTERFACE_H
#define DAO_FILTERINTERFACE_H

#include "php_dao.h"

extern zend_class_entry *dao_filterinterface_ce;

DAO_INIT_CLASS(Dao_FilterInterface);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_filterinterface_add, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_INFO(0, handler)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_filterinterface_sanitize, 0, 0, 2)
	ZEND_ARG_INFO(0, value)
	ZEND_ARG_INFO(0, filters)
	ZEND_ARG_TYPE_INFO(0, recursive, _IS_BOOL, 1)
	ZEND_ARG_TYPE_INFO(0, options, IS_ARRAY, 1)
	ZEND_ARG_TYPE_INFO(0, recursiveLevel, IS_LONG, 1)
ZEND_END_ARG_INFO()

#endif /* DAO_FILTERINTERFACE_H */
