
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
  |          ZhuZongXin <dreamsxin@qq.com>                                 |
  +------------------------------------------------------------------------+
*/

#ifndef DAO_VALIDATIONINTERFACE_H
#define DAO_VALIDATIONINTERFACE_H

#include "php_dao.h"

extern zend_class_entry *dao_validationinterface_ce;

DAO_INIT_CLASS(Dao_ValidationInterface);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_validationinterface_validate, 0, 0, 1)
	ZEND_ARG_INFO(0, data)
	ZEND_ARG_TYPE_INFO(0, entity, IS_OBJECT, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_validationinterface_add, 0, 0, 2)
	ZEND_ARG_INFO(0, attribute)
	ZEND_ARG_INFO(0, validator)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_dao_validationinterface_getlabel, 0, 1, IS_STRING, 0)
	ZEND_ARG_INFO(0, field)
ZEND_END_ARG_INFO()
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_dao_validationinterface_getlabel, 0, 1, IS_STRING, NULL, 0)
	ZEND_ARG_INFO(0, field)
ZEND_END_ARG_INFO()
#endif

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_validationinterface_getvalue, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, attribute, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, entity, IS_OBJECT, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_validationinterface_appendmessage, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, message, Dao\\Validation\\MessageInterface, 0)
ZEND_END_ARG_INFO()

#endif /* DAO_VALIDATIONINTERFACE_H */
