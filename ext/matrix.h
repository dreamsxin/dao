
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

#ifndef DAO_MATRIX_H
#define DAO_MATRIX_H

#include "php_dao.h"

#define DAO_MATRIX_TYPE_DOUBLE		0
#define DAO_MATRIX_TYPE_LONG		1

typedef struct ZMatrix{
    int m; //i, rows
    int n; //j, columns
    zval *matrix;
} ZMatrix;

typedef struct DMatrix{
    int m; //i, rows
    int n; //j, columns
    double *matrix;
} DMatrix;

typedef struct LMatrix{
    int m; //i, rows
    int n; //j, columns
    long *matrix;
} LMatrix;

void dao_lmatrix_to_pzval(LMatrix, zval*);
void dao_dmatrix_to_pzval(LMatrix, zval*);

int dao_matrix_init_matrix(ZMatrix*, zval*);
void dao_matrix_free(ZMatrix);
void dao_matrix_free_matrix(ZMatrix);

int dao_matrix_init_matrix_double(DMatrix*, zval*);
void dao_matrix_free_double(DMatrix);

int dao_matrix_init_matrix_long(LMatrix*, zval*);
void dao_matrix_free_long(LMatrix);

void dao_matrix_elementwise_function(zval*, zval*, zval*, void (*f)(zval*, zval*, zval*));
void dao_matrix_scalar_matrix_function(zval*, double, zval*, double (*f)(double, double));
void dao_matrix_scalar_matrix_function_long(zval*, long, zval*, long (*f)(long, long));

void dao_array_add_sum_long(zval*, zval*, zval*);
void dao_array_add_sum_double(zval*, zval*, zval*);
void dao_array_add_mul_zvals_to_long(zval*, zval*, zval*);
void dao_array_add_mul_zvals_to_double(zval*, zval*, zval*);

double dao_matrix_mul_double(double, double);
long dao_matrix_mul_long(long, long);

extern zend_class_entry *dao_matrix_ce;

DAO_INIT_CLASS(Dao_Matrix);

#endif /* DAO_MATRIX_H */
