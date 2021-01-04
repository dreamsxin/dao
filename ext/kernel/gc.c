
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

#include "kernel/gc.h"

static inline dao_gc_node* dao_gc_list_createnode(zval* value) {
	dao_gc_node* node = malloc(sizeof(dao_gc_node));
	node->value = value;
	node->next = NULL;
	return node;
}

struct dao_gc_list* dao_gc_list_init() {
	struct dao_gc_list* list = malloc(sizeof(struct dao_gc_list));
	list->head = NULL;
	return list;
}

void dao_gc_list_display(struct dao_gc_list* list) {
	dao_gc_node* current = list->head;
	if(list->head == NULL) {
		return;
	}
	while(current->next != NULL){
		zend_print_zval_r(current->value, 0);
		current = current->next;
	}
	zend_print_zval_r(current->value, 0); 
}

void dao_gc_list_add(struct dao_gc_list* list, zval* value){
	dao_gc_node* current = NULL;
	if(list->head == NULL){
		list->head = dao_gc_list_createnode(value);
	} else {
		current = list->head; 
		while (current->next!=NULL){
			current = current->next;
		}
		current->next = dao_gc_list_createnode(value);
	}
}

void dao_gc_list_delete(struct dao_gc_list* list, zval* value){
	dao_gc_node* current = list->head;
	dao_gc_node* previous = current;
	while(current != NULL){
		if(current->value == value){
			previous->next = current->next;
			if(current == list->head) {
				list->head = current->next;
			}
			free(current);
			return;
		}
		previous = current;
		current = current->next;
	}                                 
}                                   

void dao_gc_list_reverse(struct dao_gc_list* list){
	dao_gc_node* reversed = NULL;
	dao_gc_node* current = list->head;
	dao_gc_node* temp = NULL;
	while(current != NULL){
		temp = current;
		current = current->next;
		temp->next = reversed;
		reversed = temp;
	}
	list->head = reversed;
}

void dao_gc_list_destroy(struct dao_gc_list* list){
	dao_gc_node* current = list->head;
	dao_gc_node* next = current;
	while(current != NULL){
		zval_ptr_dtor(current->value);
		next = current->next;
		free(current);
		current = next;
	}
	free(list);
}
