
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

#ifndef DAO_VALIDATION_MESSAGE_H
#define DAO_VALIDATION_MESSAGE_H

#include "php_dao.h"

extern zend_class_entry *dao_validation_message_ce;

DAO_INIT_CLASS(Dao_Validation_Message);

DAO_STATIC void dao_validation_message_construct_helper(zval *result, zval *message, zval *field, const char *type, zval *code) DAO_ATTR_NONNULL;

#endif /* DAO_VALIDATION_MESSAGE_H */
