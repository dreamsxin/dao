
/*
  +------------------------------------------------------------------------+
  | Dao Framework                                                          |
  +------------------------------------------------------------------------+
  | Copyright (c) 2011-2015 Phalcon Team (http://www.phalconphp.com)       |
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


#ifndef DAO_KERNEL_MATH_H
#define DAO_KERNEL_MATH_H

#include "php_dao.h"

double dao_floor(zval *op1);
double dao_ceil(zval *op1);
void dao_round(zval *return_value, zval *op1, zval *op2, zval *op3);
void dao_pow(zval *return_value, zval *op1, zval *op2);
long dao_mt_rand(long min, long max);

#define dao_pow_function(result, op1, op2) pow_function(result, op1, op2)

#if PHP_VERSION_ID >= 80000
# define dao_math_basetozval(str, base, ret) _php_math_basetozval(Z_STR_P(str), base, ret)
#else
# define dao_math_basetozval(str, base, ret)  _php_math_basetozval(str, base, ret)
#endif

#endif
