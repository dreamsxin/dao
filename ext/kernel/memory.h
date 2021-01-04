
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

#ifndef DAO_KERNEL_MEMORY_H
#define DAO_KERNEL_MEMORY_H

#include "php_dao.h"
#include "kernel/main.h"

#include <stdint.h>

typedef struct _dao_memory_void_value {
    ptrdiff_t offset;
} dao_memory_void_value;

inline static void dao_memory_void_set(dao_memory_void_value* ptr, void* addr) {
	if (addr == ptr) {
		ptr->offset = INTPTR_MIN;
	} else if (addr) {
		ptr->offset = (char const*)addr - (char const*)ptr;
	} else {
		ptr->offset = 0;
	}
}

inline static void* dao_memory_void_get(dao_memory_void_value const* ptr) {
	if (INTPTR_MIN == ptr->offset) {
		return (void*)ptr;
	} else if (ptr->offset) {
		return (char*)ptr + ptr->offset;
	}
	return NULL;
}

#define dao_memory_container_of(nodepointer, type, member) \
  ((type *)((char *)(nodepointer) - offsetof(type, member)))

void dao_initialize_memory(zend_dao_globals *dao_globals_ptr);
void dao_deinitialize_memory();

int dao_set_symbol(zend_array *symbol_table, zval *key_name, zval *value);
int dao_set_symbol_str(zend_array *symbol_table, char *key_name, unsigned int key_length, zval *value);
int dao_del_symbol(zend_array *symbol_table, zval *key_name);
int dao_del_symbol_str(zend_array *symbol_table, char *key_name, unsigned int key_length);

#define DAO_PTR_DTOR(z) zval_ptr_dtor(z);
#define DAO_DTOR(z) zval_dtor(z);

#define DAO_INIT_VAR(z) ZVAL_UNDEF(z);

#define DAO_INIT_NVAR(z) \
	do { \
		if (Z_TYPE_P(z) > IS_NULL) { \
			DAO_PTR_DTOR(z); \
		} \
		DAO_INIT_VAR(z); \
	} while (0)

#define DAO_ZVAL_DUP(d, v) ZVAL_DUP(d, v);
#define DAO_MM_ZVAL_DUP(d, v) \
	do { \
		ZVAL_DUP(d, v); \
		dao_array_append(&dao_memory_entry, d, 0); \
	} while (0)

#define DAO_ZVAL_COPY(d, v) ZVAL_COPY(d, v);
#define DAO_MM_ZVAL_COPY(d, v) \
	do { \
		ZVAL_COPY(d, v); \
		dao_array_append(&dao_memory_entry, d, 0); \
	} while (0)

#define DAO_SEPARATE(z) \
	do { \
		zval *_zv = (z); \
		if (Z_TYPE_P(_zv) == IS_ARRAY) { \
			ZVAL_ARR(_zv, zend_array_dup(Z_ARR_P(_zv))); \
		} else if (Z_ISREF_P(_zv)) { \
			ZVAL_DUP(_zv, Z_REFVAL_P(_zv)); \
		} else if (Z_REFCOUNTED_P(_zv)) { \
			Z_ADDREF_P(_zv); \
		} \
	} while (0)

#define DAO_MM_SEPARATE(z) \
	do { \
		DAO_SEPARATE(z); \
		if (Z_REFCOUNTED_P(z)) { \
			dao_array_append(&dao_memory_entry, z, 0); \
		} \
	} while (0)

#define DAO_SEPARATE_PARAM(z) \
	do { \
		ZVAL_DEREF(z); \
		SEPARATE_ZVAL_IF_NOT_REF(z); \
	} while (0)

#define DAO_COPY_TO_STACK(a, b) \
	{ \
    	memcpy(a, b, sizeof(zval)); \
	}

#define DAO_MM_GROW()       dao_gc_list* gc_list = dao_gc_list_init();
#define DAO_MM_RESTORE()    dao_gc_list_destroy(gc_list);

/* Backwards compatibility for GC API change in PHP 7.3 */
#if PHP_VERSION_ID < 70300
#  define GC_ADDREF(p)            ++GC_REFCOUNT(p)
#  define GC_DELREF(p)            --GC_REFCOUNT(p)
#  define GC_SET_REFCOUNT(p, rc)  GC_REFCOUNT(p) = rc
#endif

#endif /* DAO_KERNEL_MEMORY_H */
