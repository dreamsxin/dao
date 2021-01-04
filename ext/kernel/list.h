
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

#ifndef DAO_KERNEL_LIST_H
#define DAO_KERNEL_LIST_H

#include "kernel/memory.h"

typedef struct _dao_list_node {
    dao_memory_void_value next;
    dao_memory_void_value prev;
} dao_list_node;

typedef int (*dao_list_node_compare)(dao_list_node const* l, dao_list_node const* r);

typedef struct {
  dao_memory_void_value first;
  dao_memory_void_value last;
} dao_list;

static inline void dao_list_node_init(dao_list_node* node)
{
  dao_memory_void_set(&node->next, NULL);
  dao_memory_void_set(&node->prev, NULL);
}

dao_list_node* dao_list_lookup(dao_list_node const*, dao_list_node_compare cmp, dao_list const*);

dao_list_node* dao_list_front(dao_list const*);
dao_list_node* dao_list_back(dao_list const*);
dao_list_node* dao_list_first(dao_list_node const* node);
dao_list_node* dao_list_last(dao_list_node const* node);
dao_list_node* dao_list_next(dao_list_node const* node);
dao_list_node* dao_list_prev(dao_list_node const* node);

void dao_list_insert_befor(dao_list_node* where, dao_list_node* node, dao_list*);
void dao_list_insert_after(dao_list_node* where, dao_list_node* node, dao_list*);
void dao_list_push_back(dao_list_node* node, dao_list*);
void dao_list_push_front(dao_list_node* node, dao_list*);

void dao_list_remove(dao_list_node* node, dao_list*);
void dao_list_replace(dao_list_node* old, dao_list_node* node, dao_list*);

void dao_list_swap(dao_list_node* node1, dao_list_node* node2, dao_list*);
void dao_list_sort(dao_list*, dao_list_node_compare cmp);
int dao_list_init(dao_list*);

#endif /* DAO_KERNEL_LIST_H */
