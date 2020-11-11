
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

#include "kernel/rbtree.h"

static void __dao_rbtree_rotate_left(struct dao_rbtree_node *node, struct dao_rbtree *root)
{
	struct dao_rbtree_node *right = node->right;
	struct dao_rbtree_node *parent = dao_rbtree_parent(node);

	if ((node->right = right->left))
		dao_rbtree_set_parent(right->left, node);
	right->left = node;

	dao_rbtree_set_parent(right, parent);

	if (parent)
	{
		if (node == parent->left)
			parent->left = right;
		else
			parent->right = right;
	}
	else
		root->node = right;
	dao_rbtree_set_parent(node, right);
}

static void __dao_rbtree_rotate_right(struct dao_rbtree_node *node, struct dao_rbtree *root)
{
	struct dao_rbtree_node *left = node->left;
	struct dao_rbtree_node *parent = dao_rbtree_parent(node);

	if ((node->left = left->right))
		dao_rbtree_set_parent(left->right, node);
	left->right = node;

	dao_rbtree_set_parent(left, parent);

	if (parent)
	{
		if (node == parent->right)
			parent->right = left;
		else
			parent->left = left;
	}
	else
		root->node = left;
	dao_rbtree_set_parent(node, left);
}

void dao_rbtree_insert_color(struct dao_rbtree_node *node, struct dao_rbtree *root)
{
	struct dao_rbtree_node *parent, *gparent;

	while ((parent = dao_rbtree_parent(node)) && dao_rbtree_is_red(parent))
	{
		gparent = dao_rbtree_parent(parent);

		if (parent == gparent->left)
		{
			{
				register struct dao_rbtree_node *uncle = gparent->right;
				if (uncle && dao_rbtree_is_red(uncle))
				{
					dao_rbtree_set_black(uncle);
					dao_rbtree_set_black(parent);
					dao_rbtree_set_red(gparent);
					node = gparent;
					continue;
				}
			}

			if (parent->right == node)
			{
				register struct dao_rbtree_node *tmp;
				__dao_rbtree_rotate_left(parent, root);
				tmp = parent;
				parent = node;
				node = tmp;
			}

			dao_rbtree_set_black(parent);
			dao_rbtree_set_red(gparent);
			__dao_rbtree_rotate_right(gparent, root);
		} else {
			{
				register struct dao_rbtree_node *uncle = gparent->left;
				if (uncle && dao_rbtree_is_red(uncle))
				{
					dao_rbtree_set_black(uncle);
					dao_rbtree_set_black(parent);
					dao_rbtree_set_red(gparent);
					node = gparent;
					continue;
				}
			}

			if (parent->left == node)
			{
				register struct dao_rbtree_node *tmp;
				__dao_rbtree_rotate_right(parent, root);
				tmp = parent;
				parent = node;
				node = tmp;
			}

			dao_rbtree_set_black(parent);
			dao_rbtree_set_red(gparent);
			__dao_rbtree_rotate_left(gparent, root);
		}
	}

	dao_rbtree_set_black(root->node);
}

static void __dao_rbtree_remove_color(struct dao_rbtree_node *node, struct dao_rbtree_node *parent, struct dao_rbtree *root)
{
	struct dao_rbtree_node *other;

	while ((!node || dao_rbtree_is_black(node)) && node != root->node)
	{
		if (parent->left == node)
		{
			other = parent->right;
			if (dao_rbtree_is_red(other))
			{
				dao_rbtree_set_black(other);
				dao_rbtree_set_red(parent);
				__dao_rbtree_rotate_left(parent, root);
				other = parent->right;
			}
			if ((!other->left || dao_rbtree_is_black(other->left)) &&
			    (!other->right || dao_rbtree_is_black(other->right)))
			{
				dao_rbtree_set_red(other);
				node = parent;
				parent = dao_rbtree_parent(node);
			}
			else
			{
				if (!other->right || dao_rbtree_is_black(other->right))
				{
					dao_rbtree_set_black(other->left);
					dao_rbtree_set_red(other);
					__dao_rbtree_rotate_right(other, root);
					other = parent->right;
				}
				dao_rbtree_set_color(other, dao_rbtree_color(parent));
				dao_rbtree_set_black(parent);
				dao_rbtree_set_black(other->right);
				__dao_rbtree_rotate_left(parent, root);
				node = root->node;
				break;
			}
		}
		else
		{
			other = parent->left;
			if (dao_rbtree_is_red(other))
			{
				dao_rbtree_set_black(other);
				dao_rbtree_set_red(parent);
				__dao_rbtree_rotate_right(parent, root);
				other = parent->left;
			}
			if ((!other->left || dao_rbtree_is_black(other->left)) &&
			    (!other->right || dao_rbtree_is_black(other->right)))
			{
				dao_rbtree_set_red(other);
				node = parent;
				parent = dao_rbtree_parent(node);
			}
			else
			{
				if (!other->left || dao_rbtree_is_black(other->left))
				{
					dao_rbtree_set_black(other->right);
					dao_rbtree_set_red(other);
					__dao_rbtree_rotate_left(other, root);
					other = parent->left;
				}
				dao_rbtree_set_color(other, dao_rbtree_color(parent));
				dao_rbtree_set_black(parent);
				dao_rbtree_set_black(other->left);
				__dao_rbtree_rotate_right(parent, root);
				node = root->node;
				break;
			}
		}
	}
	if (node)
		dao_rbtree_set_black(node);
}

void dao_rbtree_remove(struct dao_rbtree_node *node, struct dao_rbtree *root)
{
	struct dao_rbtree_node *child, *parent;
	int color;

	if (!node->left)
		child = node->right;
	else if (!node->right)
		child = node->left;
	else
	{
		struct dao_rbtree_node *old = node, *left;

		node = node->right;
		while ((left = node->left) != NULL)
			node = left;

		if (dao_rbtree_parent(old)) {
			if (dao_rbtree_parent(old)->left == old)
				dao_rbtree_parent(old)->left = node;
			else
				dao_rbtree_parent(old)->right = node;
		} else
			root->node = node;

		child = node->right;
		parent = dao_rbtree_parent(node);
		color = dao_rbtree_color(node);

		if (parent == old) {
			parent = node;
		} else {
			if (child)
				dao_rbtree_set_parent(child, parent);
			parent->left = child;

			node->right = old->right;
			dao_rbtree_set_parent(old->right, node);
		}

		node->parent_color = old->parent_color;
		node->left = old->left;
		dao_rbtree_set_parent(old->left, node);

		goto color;
	}

	parent = dao_rbtree_parent(node);
	color = dao_rbtree_color(node);

	if (child)
		dao_rbtree_set_parent(child, parent);
	if (parent)
	{
		if (parent->left == node)
			parent->left = child;
		else
			parent->right = child;
	}
	else
		root->node = child;

 color:
	if (color == DAO_RBTREE_BLACK)
		__dao_rbtree_remove_color(child, parent, root);
}
/*
static void dao_rbtree_augment_path(struct dao_rbtree_node *node, dao_rbtree_augment_f func, void *data)
{
	struct dao_rbtree_node *parent;

up:
	func(node, data);
	parent = dao_rbtree_parent(node);
	if (!parent)
		return;

	if (node == parent->left && parent->right)
		func(parent->right, data);
	else if (parent->left)
		func(parent->left, data);

	node = parent;
	goto up;
}

void dao_rbtree_augment_insert(struct dao_rbtree_node *node, dao_rbtree_augment_f func, void *data)
{
	if (node->left)
		node = node->left;
	else if (node->right)
		node = node->right;

	dao_rbtree_augment_path(node, func, data);
}

struct dao_rbtree_node *dao_rbtree_augment_erase_begin(struct dao_rbtree_node *node)
{
	struct dao_rbtree_node *deepest;

	if (!node->right && !node->left)
		deepest = dao_rbtree_parent(node);
	else if (!node->right)
		deepest = node->left;
	else if (!node->left)
		deepest = node->right;
	else {
		deepest = dao_rbtree_next(node);
		if (deepest->right)
			deepest = deepest->right;
		else if (dao_rbtree_parent(deepest) != node)
			deepest = dao_rbtree_parent(deepest);
	}

	return deepest;
}

void dao_rbtree_augment_erase_end(struct dao_rbtree_node *node, dao_rbtree_augment_f func, void *data)
{
	if (node)
		dao_rbtree_augment_path(node, func, data);
}
*/

/*
 * This function returns the first node (in sort order) of the tree.
 */
struct dao_rbtree_node *dao_rbtree_first(const struct dao_rbtree *root)
{
	struct dao_rbtree_node	*n;

	n = root->node;
	if (!n)
		return NULL;
	while (n->left)
		n = n->left;
	return n;
}

struct dao_rbtree_node *dao_rbtree_last(const struct dao_rbtree *root)
{
	struct dao_rbtree_node	*n;

	n = root->node;
	if (!n)
		return NULL;
	while (n->right)
		n = n->right;
	return n;
}

struct dao_rbtree_node *dao_rbtree_next(const struct dao_rbtree_node *node)
{
	struct dao_rbtree_node *parent;

	if (dao_rbtree_parent(node) == node)
		return NULL;

	/* If we have a right-hand child, go down and then left as far
	   as we can. */
	if (node->right) {
		node = node->right;
		while (node->left)
			node=node->left;
		return (struct dao_rbtree_node *)node;
	}

	/* No right-hand children.  Everything down and left is
	   smaller than us, so any 'next' node must be in the general
	   direction of our parent. Go up the tree; any time the
	   ancestor is a right-hand child of its parent, keep going
	   up. First time it's a left-hand child of its parent, said
	   parent is our 'next' node. */
	while ((parent = dao_rbtree_parent(node)) && node == parent->right)
		node = parent;

	return parent;
}

struct dao_rbtree_node *dao_rbtree_prev(const struct dao_rbtree_node *node)
{
	struct dao_rbtree_node *parent;

	if (dao_rbtree_parent(node) == node)
		return NULL;

	/* If we have a left-hand child, go down and then right as far
	   as we can. */
	if (node->left) {
		node = node->left;
		while (node->right)
			node=node->right;
		return (struct dao_rbtree_node *)node;
	}

	/* No left-hand children. Go up till we find an ancestor which
	   is a right-hand child of its parent */
	while ((parent = dao_rbtree_parent(node)) && node == parent->left)
		node = parent;

	return parent;
}

void dao_rbtree_replace(struct dao_rbtree_node *old, struct dao_rbtree_node *new, struct dao_rbtree *root)
{
	struct dao_rbtree_node *parent = dao_rbtree_parent(old);

	/* Set the surrounding nodes to point to the replacement */
	if (parent) {
		if (old == parent->left)
			parent->left = new;
		else
			parent->right = new;
	} else {
		root->node = new;
	}
	if (old->left)
		dao_rbtree_set_parent(old->left, new);
	if (old->right)
		dao_rbtree_set_parent(old->right, new);

	/* Copy the pointers/colour from the victim to the replacement */
	*new = *old;
}

struct dao_rbtree_node* dao_rbtree_lookup(struct dao_rbtree_node* key, dao_rbtree_node_compare cmp, struct dao_rbtree* root) {
  	struct dao_rbtree_node *node = root->node;

  	while (node) {
		int result = cmp(key, node);

		if (result < 0)
  			node = node->left;
		else if (result > 0)
  			node = node->right;
		else
  			return node;
	}
	return NULL;
}

struct dao_rbtree_node *dao_rbtree_insert(struct dao_rbtree_node* node, dao_rbtree_node_compare cmp, struct dao_rbtree* root) {
  	struct dao_rbtree_node **new = &(root->node), *parent = NULL;

  	while (*new) {
  		int result = cmp(node, *new);
		parent = *new;
  		if (result < 0)
  			new = &((*new)->left);
  		else if (result > 0)
  			new = &((*new)->right);
  		else
  			return *new;
  	}

  	dao_rbtree_link_node(node, parent, new);
  	dao_rbtree_insert_color(node, root);
	return *new;
}
