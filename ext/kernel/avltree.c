
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

#include "kernel/avltree.h"

dao_avltree_node *dao_avltree_first(dao_avltree *tree)
{
	return dao_memory_void_get(&tree->first);
}

dao_avltree_node *dao_avltree_last(dao_avltree *tree)
{
	return dao_memory_void_get(&tree->last);
}

dao_avltree_node *dao_avltree_next(dao_avltree_node *node)
{
	dao_avltree_node* r;

	if (dao_memory_void_get(&node->right))
		return dao_avltree_get_first_avl(dao_memory_void_get(&node->right));

	while ((r = dao_avltree_get_parent_avl(node)) &&
				 (dao_memory_void_get(&r->right) == node) ) {
		node = r;
	}

	return r;
}

dao_avltree_node *dao_avltree_prev(dao_avltree_node *node)
{
	dao_avltree_node* r;

	if (dao_memory_void_get(&node->left))
		return dao_avltree_get_last_avl(dao_memory_void_get(&node->left));

	while ((r = dao_avltree_get_parent_avl(node)) &&
				 (dao_memory_void_get(&r->left) == node)) {
		node = r;
	}
	return r;
}

dao_avltree_node *dao_avltree_lookup(dao_avltree_node *key, dao_avltree_node_compare cmp, dao_avltree *tree)
{
	dao_avltree_node *parent, *unbalanced;
	int is_left;

	return dao_avltree_do_lookup_avl(key, cmp, tree, &parent, &unbalanced, &is_left);
}

/* Insertion never needs more than 2 rotations */
dao_avltree_node *dao_avltree_insert(dao_avltree_node *node, dao_avltree_node_compare cmp, dao_avltree *tree)
{
	dao_avltree_node *key, *parent, *unbalanced;
	int is_left;

	key = dao_avltree_do_lookup_avl(node, cmp, tree, &parent, &unbalanced, &is_left);

	if (key)
		return key;

	dao_avltree_init_node(node);

	if (!parent) {

		dao_memory_void_set(&tree->root, node);
		dao_memory_void_set(&tree->first, node);
		dao_memory_void_set(&tree->last, node);
		tree->height++;
		return NULL;
	}

	if (is_left) {

		if (dao_memory_void_get(&tree->first) == parent)
			dao_memory_void_set(&tree->first, node);

	} else {

		if (dao_memory_void_get(&tree->last) == parent)
			dao_memory_void_set(&tree->last, node);
	}

	dao_avltree_set_parent_avl(parent, node);
	dao_avltree_set_child_avl(node, parent, is_left);

	for (;;) {

		if (dao_memory_void_get(&parent->left) == node)
			dao_avltree_dec_balance(parent);

		else
			dao_avltree_inc_balance(parent);

		if (parent == unbalanced)
			break;

		node = parent;
		parent = dao_avltree_get_parent_avl(parent);
	}

	switch (dao_avltree_get_balance(unbalanced)) {

	case 1:
	case -1:
		tree->height++;
		/* fall through */
	case 0:
		break;
	case 2:
	{
		dao_avltree_node* right = dao_memory_void_get(&unbalanced->right);

		if (dao_avltree_get_balance(right) == 1) {

			dao_avltree_set_balance(0, unbalanced);
			dao_avltree_set_balance(0, right);
		} else {

			switch (dao_avltree_get_balance(dao_memory_void_get(&right->left))) {
			case 1:
				dao_avltree_set_balance(-1, unbalanced);
				dao_avltree_set_balance( 0, right);
				break;
			case 0:
				dao_avltree_set_balance(0, unbalanced);
				dao_avltree_set_balance(0, right);
				break;
			case -1:
				dao_avltree_set_balance(0, unbalanced);
				dao_avltree_set_balance(1, right);
				break;
			}
			dao_avltree_set_balance(0, dao_memory_void_get(&right->left));
			dao_avltree_rotate_right_avl(right, tree);
		}
		dao_avltree_rotate_left_avl(unbalanced, tree);
		break;
	}

	case -2: {
		dao_avltree_node *left = dao_memory_void_get(&unbalanced->left);

		if (dao_avltree_get_balance(left) == -1) {
			dao_avltree_set_balance(0, unbalanced);
			dao_avltree_set_balance(0, left);
		} else {
			switch (dao_avltree_get_balance(dao_memory_void_get(&left->right))) {
			case 1:
				dao_avltree_set_balance( 0, unbalanced);
				dao_avltree_set_balance(-1, left);
				break;
			case 0:
				dao_avltree_set_balance(0, unbalanced);
				dao_avltree_set_balance(0, left);
				break;
			case -1:
				dao_avltree_set_balance(1, unbalanced);
				dao_avltree_set_balance(0, left);
				break;
			}
			dao_avltree_set_balance(0, dao_memory_void_get(&left->right));

			dao_avltree_rotate_left_avl(left, tree);
		}

		dao_avltree_rotate_right_avl(unbalanced, tree);
		break;
	}
	}
	return NULL;
}

/* Deletion might require up to log(n) rotations */
void dao_avltree_remove(dao_avltree_node *node, dao_avltree *tree)
{
	dao_avltree_node *parent = dao_avltree_get_parent_avl(node);
	dao_avltree_node *left = dao_memory_void_get(&node->left);
	dao_avltree_node *right = dao_memory_void_get(&node->right);
	dao_avltree_node *next;
	int is_left = 0;

	if (node == dao_memory_void_get(&tree->first))
		dao_memory_void_set(&tree->first, dao_avltree_next(node));
	if (node == dao_memory_void_get(&tree->last))
		dao_memory_void_set(&tree->last, dao_avltree_prev(node));

	if (!left)
		next = right;
	else if (!right)
		next = left;
	else
		next = dao_avltree_get_first_avl(right);

	if (parent) {
		is_left = dao_memory_void_get(&parent->left) == node;
		dao_avltree_set_child_avl(next, parent, is_left);
	} else
		dao_memory_void_set(&tree->root, next);

	if (left && right) {
		dao_avltree_set_balance(dao_avltree_get_balance(node), next);

		dao_memory_void_set(&next->left, left);
		dao_avltree_set_parent_avl(next, left);

		if (next != right) {
			parent = dao_avltree_get_parent_avl(next);
			dao_avltree_set_parent_avl(dao_avltree_get_parent_avl(node), next);

			node = dao_memory_void_get(&next->right);
			dao_memory_void_set(&parent->left, node);
			is_left = 1;

			dao_memory_void_set(&next->right, right);
			dao_avltree_set_parent_avl(next, right);
		} else {
			dao_avltree_set_parent_avl(parent, next);
			parent = next;
			node = dao_memory_void_get(&parent->right);
			is_left = 0;
		}
		assert(parent != NULL);
	} else
		node = next;

	if (node)
		dao_avltree_set_parent_avl(parent, node);

	/*
	 * At this point, 'parent' can only be null, if 'node' is the
	 * tree's root and has at most one child.
	 *
	 * case 1: the subtree is now balanced but its height has
	 * decreased.
	 *
	 * case 2: the subtree is mostly balanced and its height is
	 * unchanged.
	 *
	 * case 3: the subtree is unbalanced and its height may have
	 * been changed during the rebalancing process, see below.
	 *
	 * case 3.1: after a left rotation, the subtree becomes mostly
	 * balanced and its height is unchanged.
	 *
	 * case 3.2: after a left rotation, the subtree becomes
	 * balanced but its height has decreased.
	 *
	 * case 3.3: after a left and a right rotation, the subtree
	 * becomes balanced or mostly balanced but its height has
	 * decreased for all cases.
	 */
	while (parent) {
		int balance;
		node	 = parent;
		parent = dao_avltree_get_parent_avl(parent);

		if (is_left) {
			is_left = parent && dao_memory_void_get(&parent->left) == node;

			balance = dao_avltree_inc_balance(node);
			if (balance == 0)		/* case 1 */
				continue;
			if (balance == 1)		/* case 2 */
				return;
			right = dao_memory_void_get(&node->right);		/* case 3 */
			switch (dao_avltree_get_balance(right)) {
			case 0:				/* case 3.1 */
				dao_avltree_set_balance( 1, node);
				dao_avltree_set_balance(-1, right);
				dao_avltree_rotate_left_avl(node, tree);
				return;
			case 1:				/* case 3.2 */
				dao_avltree_set_balance(0, node);
				dao_avltree_set_balance(0, right);
				break;
			case -1:			/* case 3.3 */
				switch (dao_avltree_get_balance(dao_memory_void_get(&right->left))) {
				case 1:
					dao_avltree_set_balance(-1, node);
					dao_avltree_set_balance( 0, right);
					break;
				case 0:
					dao_avltree_set_balance(0, node);
					dao_avltree_set_balance(0, right);
					break;
				case -1:
					dao_avltree_set_balance(0, node);
					dao_avltree_set_balance(1, right);
					break;
				}
				dao_avltree_set_balance(0, dao_memory_void_get(&right->left));

				dao_avltree_rotate_right_avl(right, tree);
			}
			dao_avltree_rotate_left_avl(node, tree);
		} else {
			is_left = parent && dao_memory_void_get(&parent->left) == node;

			balance = dao_avltree_dec_balance(node);
			if (balance == 0)
				continue;
			if (balance == -1)
				return;
			left = dao_memory_void_get(&node->left);
			switch (dao_avltree_get_balance(left)) {
			case 0:
				dao_avltree_set_balance(-1, node);
				dao_avltree_set_balance(1, left);
				dao_avltree_rotate_right_avl(node, tree);
				return;
			case -1:
				dao_avltree_set_balance(0, node);
				dao_avltree_set_balance(0, left);
				break;
			case 1:
				switch (dao_avltree_get_balance(dao_memory_void_get(&left->right))) {
				case 1:
					dao_avltree_set_balance(0, node);
					dao_avltree_set_balance(-1, left);
					break;
				case 0:
					dao_avltree_set_balance(0, node);
					dao_avltree_set_balance(0, left);
					break;
				case -1:
					dao_avltree_set_balance(1, node);
					dao_avltree_set_balance(0, left);
					break;
				}
				dao_avltree_set_balance(0, dao_memory_void_get(&left->right));

				dao_avltree_rotate_left_avl(left, tree);
			}
			dao_avltree_rotate_right_avl(node, tree);
		}
	}
	tree->height--;
}

void dao_avltree_replace(dao_avltree_node *old, dao_avltree_node *n, dao_avltree *tree)
{
	dao_avltree_node *parent = dao_avltree_get_parent_avl(old);

	if (parent) {
		dao_avltree_set_child_avl(n, parent, dao_memory_void_get(&parent->left) == old);
	} else {
		dao_memory_void_set(&tree->root, n);
	}

	if (dao_memory_void_get(&old->left))
		dao_avltree_set_parent_avl(n, dao_memory_void_get(&old->left));

	if (dao_memory_void_get(&old->right))
		dao_avltree_set_parent_avl(n, dao_memory_void_get(&old->right));

	if (dao_memory_void_get(&old->left))
		dao_avltree_set_parent_avl(n, dao_memory_void_get(&old->left));
	if (dao_memory_void_get(&old->right))
		dao_avltree_set_parent_avl(n, dao_memory_void_get(&old->right));

	if (dao_memory_void_get(&tree->first) == old)
		dao_memory_void_set(&tree->first, n);
	if (dao_memory_void_get(&tree->last) == old)
		dao_memory_void_set(&tree->last, n);

	n->balance = old->balance;
	dao_memory_void_set(&n->parent, dao_memory_void_get(&old->parent));
	dao_memory_void_set(&n->left, dao_memory_void_get(&old->left));
	dao_memory_void_set(&n->right, dao_memory_void_get(&old->right));
}

void dao_avltree_init(dao_avltree *tree)
{
	dao_memory_void_set(&tree->root, NULL);
	tree->height = -1;
	dao_memory_void_set(&tree->first, NULL);
	dao_memory_void_set(&tree->last, NULL);
}
