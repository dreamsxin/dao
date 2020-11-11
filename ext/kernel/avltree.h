
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

#ifndef DAO_KERNEL_AVLTREE_H
#define DAO_KERNEL_AVLTREE_H

#include "kernel/memory.h"

typedef struct {
    dao_memory_void_value right;
    dao_memory_void_value left;
    dao_memory_void_value parent;
    signed balance:3;		/* balance factor [-2:+2] */
} dao_avltree_node;

typedef int (*dao_avltree_node_compare)(dao_avltree_node * , dao_avltree_node *);

typedef struct {
    dao_memory_void_value root;
    int height;
    dao_memory_void_value first;
    dao_memory_void_value last;
} dao_avltree;

static inline int dao_avltree_is_root_avl(dao_avltree_node *node)
{
	return NULL == dao_memory_void_get(&node->parent);
}

static inline void dao_avltree_init_node(dao_avltree_node *node)
{
	dao_memory_void_set(&node->left, NULL);
	dao_memory_void_set(&node->right, NULL);
	dao_memory_void_set(&node->parent, NULL);
	node->balance = 0;;
}

static inline signed dao_avltree_get_balance(dao_avltree_node *node)
{
	return node->balance;
}

static inline void dao_avltree_set_balance(int balance, dao_avltree_node *node)
{
	node->balance = balance;
}

static inline int dao_avltree_inc_balance(dao_avltree_node *node)
{
	return ++node->balance;
}

static inline int dao_avltree_dec_balance(dao_avltree_node *node)
{
	return --node->balance;
}

static inline dao_avltree_node* dao_avltree_get_parent_avl(dao_avltree_node *node)
{
	return dao_memory_void_get(&node->parent);
}

static inline void dao_avltree_set_parent_avl(dao_avltree_node *parent, dao_avltree_node *node)
{
	dao_memory_void_set(&node->parent, parent);
}

/*
 * Iterators
 */
static inline dao_avltree_node* dao_avltree_get_first_avl(dao_avltree_node *node)
{
	while (dao_memory_void_get(&node->left))
		node = dao_memory_void_get(&node->left);
	return node;
}

static inline dao_avltree_node* dao_avltree_get_last_avl(dao_avltree_node *node)
{
	while (dao_memory_void_get(&node->right))
		node = dao_memory_void_get(&node->right);
	return node;
}

/*
 * The AVL tree is more rigidly balanced than Red-Black trees, leading
 * to slower insertion and removal but faster retrieval.
 */

/* node->balance = height(node->right) - height(node->left); */
static inline void dao_avltree_rotate_left_avl(dao_avltree_node *node, dao_avltree *tree)
{
	dao_avltree_node *p = node;
	dao_avltree_node *q = dao_memory_void_get(&node->right); /* can't be NULL */
	dao_avltree_node *parent = dao_avltree_get_parent_avl(p);

	if (!dao_avltree_is_root_avl(p)) {

		if (dao_memory_void_get(&parent->left) == p)
			dao_memory_void_set(&parent->left, q);
		else
			dao_memory_void_set(&parent->right, q);

	} else {
		dao_memory_void_set(&tree->root, q);
	}

	dao_avltree_set_parent_avl(parent, q);
	dao_avltree_set_parent_avl(q, p);

	dao_memory_void_set(&p->right, dao_memory_void_get(&q->left));

	if (dao_memory_void_get(&p->right))
		dao_avltree_set_parent_avl(p, dao_memory_void_get(&p->right));

	dao_memory_void_set(&q->left, p);
}

static inline void dao_avltree_rotate_right_avl(dao_avltree_node *node, dao_avltree *tree)
{
	dao_avltree_node *p = node;
	dao_avltree_node *q = dao_memory_void_get(&node->left) ; /* can't be NULL */
	dao_avltree_node *parent = dao_avltree_get_parent_avl(p);

	if (!dao_avltree_is_root_avl(p)) {

		if (dao_memory_void_get(&parent->left) == p)
			dao_memory_void_set(&parent->left, q);
		else
			dao_memory_void_set(&parent->right, q);

	} else {
		dao_memory_void_set(&tree->root, q);
	}

	dao_avltree_set_parent_avl(parent, q);
	dao_avltree_set_parent_avl(q, p);

	dao_memory_void_set(&p->left, dao_memory_void_get(&q->right));

	if (dao_memory_void_get(&p->left))
		dao_avltree_set_parent_avl(p, dao_memory_void_get(&p->left));

	dao_memory_void_set(&q->right, p);
}

static inline void dao_avltree_set_child_avl(dao_avltree_node *child, dao_avltree_node *node, int left)
{
	if (left) dao_memory_void_set(&node->left, child);
	else dao_memory_void_set(&node->right, child);
}

/*
 * 'pparent', 'unbalanced' and 'is_left' are only used for
 * insertions. Normally GCC will notice this and get rid of them for
 * lookups.
 */
static inline dao_avltree_node *dao_avltree_do_lookup_avl(dao_avltree_node *key, dao_avltree_node_compare cmp, dao_avltree *tree, dao_avltree_node **pparent, dao_avltree_node **unbalanced, int *is_left)
{
	dao_avltree_node *node = dao_memory_void_get(&tree->root);
	int res = 0;

	*pparent = NULL;
	*unbalanced = node;
	*is_left = 0;

	while (node) {
		if (dao_avltree_get_balance(node) != 0)
			*unbalanced = node;

		res = cmp(node, key);
		if (res == 0)
			return node;
		*pparent = node;
		if ((*is_left = res > 0))
			node = dao_memory_void_get(&node->left);
		else
			node = dao_memory_void_get(&node->right);
	}
	return NULL;
}

dao_avltree_node* dao_avltree_first(dao_avltree* tree);
dao_avltree_node* dao_avltree_last(dao_avltree* tree);
dao_avltree_node* dao_avltree_next(dao_avltree_node* node);
dao_avltree_node* dao_avltree_prev(dao_avltree_node* node);

dao_avltree_node* dao_avltree_lookup(dao_avltree_node* key, dao_avltree_node_compare cmp, dao_avltree* tree);
dao_avltree_node* dao_avltree_insert(dao_avltree_node* node, dao_avltree_node_compare cmp, dao_avltree* tree);
void dao_avltree_remove(dao_avltree_node* node, dao_avltree* tree);
void dao_avltree_replace(dao_avltree_node* old, dao_avltree_node* node, dao_avltree* tree);
void dao_avltree_init(dao_avltree* tree);

#endif /* DAO_KERNEL_AVLTREE_H */
