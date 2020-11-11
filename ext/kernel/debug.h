
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

#ifndef DAO_KERNEL_DEBUG_H
#define DAO_KERNEL_DEBUG_H

#include "php_dao.h"

#include <stdio.h>

#include <ext/standard/php_var.h>

#define PHV(v) dao_vdump(v)
#define PHPR(v) dao_print_r(v)

#define RC_DUMP(zv)                                                                                                                \
	do {                                                                                                                           \
		char *_n = (strrchr((#zv), '&') ? strrchr((#zv), '&') + 1 : (#zv));                                                        \
		char *_f = (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__);                                               \
		zval *_z = (zv);                                                                                                           \
		if (Z_REFCOUNTED_P(_z)) {                                                                                                  \
			fprintf(stderr, "[DUMP]: %s:%d %s (%p) refcount=%d, type=%d\n", _f, __LINE__, _n, _z, Z_REFCOUNT_P(_z), Z_TYPE_P(_z)); \
		} else {                                                                                                                   \
			fprintf(stderr, "[DUMP]: %s:%d %s (%p) is not reference-counted, type=%d\n", _f, __LINE__, _n, _z, Z_TYPE_P(_z));      \
		}                                                                                                                          \
	} while (0)

#define DAO_DEBUG_ZVAL(v) DAO_DEBUG_SIMPLE(); php_debug_zval_dump(v, 1)
#define DAO_DEBUG_SIMPLE() zend_printf("\nFile:%s, Line:%d\n", __FILE__, __LINE__)
#define DAO_DEBUG_SIMPLE_ZVAL(v) zend_printf("\nVar:%s, Refcount:%d, File:%s, Line:%d\n", #v, Z_REFCOUNT_P(v), __FILE__, __LINE__)

#define DAO_DEBUG_PRINT_ADDR(var) printf("Addr:  0x%p  \'"#var"\'\n",&(var))
#define DAO_DEBUG_PRINT_SIZE(var) printf("Size of \'"#var"\': %dByte\n",(int)sizeof(var))
#define DAO_DEBUG_PRINT_VALUE_F(var, format) printf("Value: "#format"  \'"#var"\'\n",var)
#define DAO_DEBUG_PRINT_VALUE(var) DAO_DEBUG_PRINT_VALUE_F(var, 0x%p)

typedef struct _dao_debug_entry {
	struct _dao_debug_entry *prev;
	struct _dao_debug_entry *next;
	char *class_name;
	char *method_name;
	int lineno;
} dao_debug_entry;

int dao_start_debug();
int dao_stop_debug();

int dao_print_r(zval *userval);
int dao_debug_print_r(zval *message);
int dao_vdump(zval *uservar);
int dao_debug_assign(char *name, zval *value);
int dao_vpdump(const zval **uservar);
int dao_dump_ce(zend_class_entry *ce);
int dao_class_debug(zval *val);

int dao_debug_backtrace_internal();
int dao_debug_str(char *what, char *message);
int dao_debug_long(char *what, uint vlong);
int dao_debug_screen(char *message);

int dao_step_over(char *message);
int dao_step_into(char *message);
int dao_step_out(char *message);

int dao_step_over_zval(zval *message);
int dao_step_into_zval(zval *message);
int dao_step_out_zval(zval *message);

int dao_step_into_entry(char *class_name, char *method_name, int lineno);
int dao_step_out_entry();

int dao_debug_method_call(zval *obj, char *method_name);
int dao_debug_vdump(char *preffix, zval *value);
int dao_debug_param(zval *param);

int dao_error_space();
int dao_debug_space();

extern FILE *dao_log;
extern int dao_debug_trace;
extern dao_debug_entry *start;
extern dao_debug_entry *active;

#endif /* DAO_KERNEL_DEBUG_H */
