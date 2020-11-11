
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

#ifndef DAO_KERNEL_REQUIRE_H
#define DAO_KERNEL_REQUIRE_H

#include "php_dao.h"

zend_array *dao_build_symtable(zval *vars);
static inline void dao_destroy_symtable(zend_array *symbol_table) {
	zend_array_destroy(symbol_table);
}

int _dao_exec(zval* ret, zval *object, zend_op_array *op_array, zend_array *symbol_table);
int dao_exec_file(zval *ret, zval *object, zval *file, zval *vars, zend_array *symbol_table);
int dao_exec_code(zval *ret, zval *object, zval *code, zval * vars);

int dao_require_ret(zval *return_value_ptr, const char *require_path) DAO_ATTR_NONNULL1(2);

DAO_ATTR_NONNULL static inline int dao_require(const char *require_path)
{
	return dao_require_ret(NULL, require_path);
}

DAO_ATTR_NONNULL static inline int dao_require_zval(const zval *require_path)
{
    return dao_require_ret(NULL, Z_TYPE_P(require_path) == IS_STRING ? Z_STRVAL_P(require_path) : "");
}

DAO_ATTR_NONNULL static inline int dao_require_zval_ret(zval *return_value, const zval *require_path)
{
    return dao_require_ret(return_value, Z_TYPE_P(require_path) == IS_STRING ? Z_STRVAL_P(require_path) : "");
}
#endif /* DAO_KERNEL_REQUIRE_H */
