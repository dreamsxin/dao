
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

#ifndef DAO_KERNEL_EXCEPTION_H
#define DAO_KERNEL_EXCEPTION_H

#include "php_dao.h"

#define DAO_THROW_EXCEPTION_DEBUG_STR(class_entry, message, file, line) \
  do { \
    dao_throw_exception_string_debug(class_entry, message, strlen(message), file, line); \
  } while (0)

#define DAO_MM_THROW_EXCEPTION_DEBUG_STR(class_entry, message, file, line) \
  do { \
    dao_throw_exception_string_debug(class_entry, message, strlen(message), file, line); \
	zval_ptr_dtor(&dao_memory_entry); \
  } while (0)

#define DAO_THROW_EXCEPTION_DEBUG_ZVAL(class_entry, message, file, line) \
  do { \
    dao_throw_exception_zval(class_entry, message, file, line); \
  } while (0)

#define DAO_MM_THROW_EXCEPTION_DEBUG_ZVAL(class_entry, message, file, line) \
  do { \
    dao_throw_exception_zval(class_entry, message, file, line); \
	zval_ptr_dtor(&dao_memory_entry); \
  } while (0)

#define DAO_THROW_EXCEPTION_STR(class_entry, message)  dao_throw_exception_string(class_entry, message)
#define DAO_MM_THROW_EXCEPTION_STR(class_entry, message) \
  do { \
	dao_throw_exception_string(class_entry, message);\
	zval_ptr_dtor(&dao_memory_entry); \
  } while (0)

#define DAO_THROW_EXCEPTION_ZVAL(class_entry, message) dao_throw_exception_zval(class_entry, message)
#define DAO_MM_THROW_EXCEPTION_ZVAL(class_entry, message) \
  do { \
	dao_throw_exception_zval(class_entry, message);\
	zval_ptr_dtor(&dao_memory_entry); \
  } while (0)

#define DAO_THROW_EXCEPTION_FORMAT(class_entry, format, ...) dao_throw_exception_format(class_entry, format, __VA_ARGS__);
#define DAO_MM_THROW_EXCEPTION_FORMAT(class_entry, format, ...) \
  do { \
	dao_throw_exception_format(class_entry, format, __VA_ARGS__);\
	zval_ptr_dtor(&dao_memory_entry); \
  } while (0)


/** Throw Exceptions */
void dao_throw_exception(zval *object) DAO_ATTR_NONNULL;
void dao_throw_exception_debug(zval *object, const char *file, uint32_t line);
void dao_throw_exception_string(zend_class_entry *ce, const char *message) DAO_ATTR_NONNULL;
void dao_throw_exception_string_debug(zend_class_entry *ce, const char *message, uint32_t message_len, const char *file, uint32_t line);
void dao_throw_exception_zval(zend_class_entry *ce, zval *message) DAO_ATTR_NONNULL;
void dao_throw_exception_zval_debug(zend_class_entry *ce, zval *message, const char *file, uint32_t line);
void dao_throw_exception_format(zend_class_entry *ce, const char *format, ...);

#endif /* DAO_KERNEL_EXCEPTION_H */
