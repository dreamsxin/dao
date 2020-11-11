
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

#include "kernel/list.h"

dao_list_node* dao_list_lookup(dao_list_node const* key, dao_list_node_compare cmp, dao_list const* list)
{
	dao_list_node* node = dao_memory_void_get(&list->first);
	while (node && cmp(node, key))
		node = dao_memory_void_get(&node->next);
	return node;
}

dao_list_node* dao_list_front(dao_list const* list)
{
	return dao_memory_void_get(&list->first);
}

dao_list_node* dao_list_back(dao_list const* list)
{
	return dao_memory_void_get(&list->last);
}

dao_list_node* dao_list_first(dao_list_node const* node)
{
	while (node && dao_memory_void_get(&node->prev))
		node = dao_memory_void_get(&node->prev);
	return (dao_list_node*)node;
}

dao_list_node* dao_list_last(dao_list_node const* node)
{
	while (node && dao_memory_void_get(&node->next))
		node = dao_memory_void_get(&node->next);
	return (dao_list_node*)node;
}

dao_list_node* dao_list_next(dao_list_node const* node)
{
	return dao_memory_void_get(&node->next);
}

dao_list_node* dao_list_prev(dao_list_node const* node)
{
	return dao_memory_void_get(&node->prev);
}

void dao_list_insert_befor(dao_list_node* where, dao_list_node* node, dao_list* list)
{
	dao_list_node_init(node);

	dao_list_node* prev = dao_list_prev(where);
	if (prev) {
		dao_memory_void_set(&prev->next, node);
		dao_memory_void_set(&node->prev, prev);
	}
	dao_memory_void_set(&where->prev, node);
	dao_memory_void_set(&node->next, where);

	if (where == dao_memory_void_get(&list->first))
		dao_memory_void_set(&list->first, node);
}

void dao_list_insert_after(dao_list_node* where, dao_list_node* node, dao_list* list)
{
	dao_list_node_init(node);

	dao_list_node* next = dao_list_next(where);

	if (next) {
		dao_memory_void_set(&next->prev, node);
		dao_memory_void_set(&node->next, next);
	}

	dao_memory_void_set(&where->next, node);
	dao_memory_void_set(&node->prev, where);

	if (where == dao_memory_void_get(&list->last))
		dao_memory_void_set(&list->last, node);
}

void dao_list_push_back(dao_list_node* node, dao_list* list)
{
	if (dao_memory_void_get(&list->last))
		dao_list_insert_after(dao_memory_void_get(&list->last), node, list);
	else
		dao_list_push_front(node, list);
}

void dao_list_push_front(dao_list_node* node, dao_list* list)
{
	if (dao_memory_void_get(&list->first)) {
		dao_list_insert_befor(dao_memory_void_get(&list->first), node, list);
	} else {
		dao_list_node_init(node);
		dao_memory_void_set(&list->first, node);
		dao_memory_void_set(&list->last, node);
	}
}

void dao_list_remove(dao_list_node* node, dao_list* list)
{
	dao_list_node* prev = dao_list_prev(node);
	dao_list_node* next = dao_list_next(node);

	if (prev) dao_memory_void_set(&prev->next, next);
	if (next) dao_memory_void_set(&next->prev, prev);

	if (dao_memory_void_get(&list->first) == node)
		dao_memory_void_set(&list->first, next);
	if (dao_memory_void_get(&list->last) == node)
		dao_memory_void_set(&list->last, prev);

	dao_list_node_init(node);
}

void dao_list_replace(dao_list_node* old, dao_list_node* node, dao_list* list)
{
	dao_list_node_init(node);

	dao_list_node* prev = dao_list_prev(old);
	dao_list_node* next = dao_list_next(old);

	if (prev) {
		dao_memory_void_set(&prev->next, node);
		dao_memory_void_set(&node->prev, prev);
	}

	if (next) {
		dao_memory_void_set(&next->prev, node);
		dao_memory_void_set(&node->next, next);
	}

	if (dao_memory_void_get(&list->first) == old)
		dao_memory_void_set(&list->first, node);
	if (dao_memory_void_get(&list->last) == old)
		dao_memory_void_set(&list->last, node);

	dao_list_node_init(old);
}


void dao_list_swap(dao_list_node* node1, dao_list_node* node2, dao_list* list)
{
	dao_list_node* p1 = dao_list_prev(node1);
	dao_list_node* n1 = dao_list_next(node1);

	dao_list_node* p2 = dao_list_prev(node2);
	dao_list_node* n2 = dao_list_next(node2);

	if (n1 == node2) {
		if (p1) dao_memory_void_set(&p1->next, node2);
		dao_memory_void_set(&node2->prev, p1);
		dao_memory_void_set(&node2->next, node1);
		dao_memory_void_set(&node1->prev, node2);
		dao_memory_void_set(&node1->next, n2);
		if (n2) dao_memory_void_set(&n2->prev, node1);
	} else if (p1 == node2) {
		if (p2) dao_memory_void_set(&p2->next, node1);
		dao_memory_void_set(&node1->prev, p2);
		dao_memory_void_set(&node1->next, node2);
		dao_memory_void_set(&node2->prev, node1);
		dao_memory_void_set(&node2->next, n1);
		if (n1) dao_memory_void_set(&n1->prev, node2);
	} else {
		if (p1) dao_memory_void_set(&p1->next, node2);
		dao_memory_void_set(&node2->prev, p1);
		dao_memory_void_set(&node2->next, n1);
		if (n1) dao_memory_void_set(&n1->prev, node2);

		if (p2) dao_memory_void_set(&p2->next, node1);
		dao_memory_void_set(&node1->prev, p2);
		dao_memory_void_set(&node1->next, n2);
		if (n2) dao_memory_void_set(&n2->prev, node1);
	}

	if (dao_memory_void_get(&list->first) == node1)
		dao_memory_void_set(&list->first, node2);
	else if (dao_memory_void_get(&list->first) == node2)
		dao_memory_void_set(&list->first, node1);

	if (dao_memory_void_get(&list->last) == node1)
		dao_memory_void_set(&list->last, node2);
	else if (dao_memory_void_get(&list->last) == node2)
		dao_memory_void_set(&list->last, node1);
}

void dao_list_sort(dao_list* list, dao_list_node_compare cmp)
{
	dao_list_node* first = dao_memory_void_get(&list->first);
	dao_list_node *p, *q, *e, *tail;
	int insize, nmerges, psize, qsize, i;

	insize = 1;

	while (1) {

		if (!first)
			break;

		p = first;

		first = NULL;
		tail = NULL;

		nmerges = 0;	/* count number of merges we do in this pass */

		while (p) {

			nmerges++;	/* there exists a merge to be done */
			/* step `insize' places along from p */
			q = p;
			psize = 0;
			for (i = 0; i < insize; i++) {
				psize++;
				q = dao_memory_void_get(&q->next);
				if (!q) break;
			}

			/* if q hasn't fallen off end, we have two lists to merge */
			qsize = insize;

			/* now we have two lists; merge them */
			while (psize > 0 || (qsize > 0 && q)) {

				/* decide whether next element of merge comes from p or q */
				if (psize == 0) {
					/* p is empty; e must come from q. */
					e = q;
					q = dao_memory_void_get(&q->next);
					qsize--;
				} else if (qsize == 0 || !q) {
					/* q is empty; e must come from p. */
					e = p;
					p = dao_memory_void_get(&p->next);
					psize--;
				} else if (cmp(p,q) <= 0) {
					/* First element of p is lower (or same);
										* e must come from p. */
					e = p;
					p = dao_memory_void_get(&p->next);
					psize--;
				} else {
					/* First element of q is lower; e must come from q. */
					e = q;
					q = dao_memory_void_get(&q->next);
					qsize--;
				}


				/* add the next element to the merged list */
				if (tail) {
					dao_memory_void_set(&tail->next, e);
				} else {
					first = e;
				}

				/* Maintain reverse pointers in a doubly linked list. */
				dao_memory_void_set(&e->prev, tail);

				tail = e;
			}

			/* now p has stepped `insize' places along, and q has too */
			p = q;
		} // while (p)

		dao_memory_void_set(&tail->next, NULL);

		/* If we have done only one merge, we're finished. */
		if (nmerges <= 1)	 /* allow for nmerges==0, the empty list case */
			break;

		/* Otherwise repeat, merging lists twice the size */
		insize *= 2;

	} // while (1)

	dao_memory_void_set(&list->first, first);
	dao_memory_void_set(&list->last, dao_list_last(first));
}

int dao_list_init(dao_list* list)
{
	dao_memory_void_set(&list->first, NULL);
	dao_memory_void_set(&list->last, NULL);
	return 0;
}
