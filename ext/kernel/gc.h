
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

#ifndef DAO_KERNEL_GC_H
#define DAO_KERNEL_GC_H

#include "kernel/memory.h"

typedef struct dao_gc_node {
	zval* value;
	struct dao_gc_node* next;
} dao_gc_node;

struct dao_gc_list {
	dao_gc_node* head; 
};

struct dao_gc_list* dao_gc_list_init();
void dao_gc_list_add(struct dao_gc_list* list, zval* value);
void dao_gc_list_delete(struct dao_gc_list* list, zval* value);
void dao_gc_list_display(struct dao_gc_list* list);
void dao_gc_list_reverse(struct dao_gc_list* list);
void dao_gc_list_destroy(struct dao_gc_list* list);

#endif /* DAO_KERNEL_GC_H */
