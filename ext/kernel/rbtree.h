
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

#ifndef	DAO_KERNEL_RBTREE_H
#define	DAO_KERNEL_RBTREE_H

#include "kernel/memory.h"

#define dao_rbtree_container_of(ptr, type, member) ({		\
 	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
 	(type *)( (char *)__mptr - dao_rbtree_offsetof(type,member) );})

#define dao_rbtree_offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#define DAO_RBTREE_RED 0
#define DAO_RBTREE_BLACK 1

struct dao_rbtree_node
{
	unsigned long parent_color;
	struct dao_rbtree_node *right;
	struct dao_rbtree_node *left;
} __attribute__((aligned(sizeof(long))));

struct dao_rbtree
{
	struct dao_rbtree_node *node;
};

#define dao_rbtree_parent(r)   ((struct dao_rbtree_node *)((r)->parent_color & ~3))
#define dao_rbtree_color(r)   ((r)->parent_color & 1)
#define dao_rbtree_is_red(r)   (!dao_rbtree_color(r))
#define dao_rbtree_is_black(r) dao_rbtree_color(r)
#define dao_rbtree_set_red(r)  do { (r)->parent_color &= ~1; } while (0)
#define dao_rbtree_set_black(r)  do { (r)->parent_color |= 1; } while (0)

static inline void dao_rbtree_init(struct dao_rbtree *tree)
{
	tree->node = NULL;
}

static inline void dao_rbtree_set_parent(struct dao_rbtree_node *rb, struct dao_rbtree_node *p)
{
	rb->parent_color = (rb->parent_color & 3) | (unsigned long)p;
}

static inline void dao_rbtree_set_color(struct dao_rbtree_node *rb, int color)
{
	rb->parent_color = (rb->parent_color & ~1) | color;
}

#define DAO_RBTREE_ROOT	(struct dao_rbtree) { NULL, }
#define DAO_RBTREE_EMPTY_ROOT(root)	((root)->node == NULL)
#define DAO_RBTREE_EMPTY_NODE(node)	(dao_rbtree_parent(node) == node)
#define DAO_RBTREE_CLEAR_NODE(node)	(dao_rbtree_set_parent(node, node))

#define	dao_rbtree_entry(ptr, type, member) dao_rbtree_container_of(ptr, type, member)

static inline void dao_rbtree_init_node(struct dao_rbtree_node *rb)
{
	rb->parent_color = 0;
	rb->right = NULL;
	rb->left = NULL;
	dao_rbtree_set_parent(rb, rb);
}

void dao_rbtree_insert_color(struct dao_rbtree_node *, struct dao_rbtree *);
void dao_rbtree_remove(struct dao_rbtree_node *, struct dao_rbtree *);

//typedef void (*dao_rbtree_augment_f)(struct dao_rbtree_node *node, void *data);

//extern void dao_rbtree_augment_insert(struct dao_rbtree_node *node, dao_rbtree_augment_f func, void *data);
//extern struct dao_rbtree_node *dao_rbtree_augment_erase_begin(struct dao_rbtree_node *node);
//extern void dao_rbtree_augment_erase_end(struct dao_rbtree_node *node, dao_rbtree_augment_f func, void *data);

/* Find logical next and previous nodes in a tree */
struct dao_rbtree_node *dao_rbtree_next(const struct dao_rbtree_node *);
struct dao_rbtree_node *dao_rbtree_prev(const struct dao_rbtree_node *);
struct dao_rbtree_node *dao_rbtree_first(const struct dao_rbtree *);
struct dao_rbtree_node *dao_rbtree_last(const struct dao_rbtree *);

/* Fast replacement of a single node without remove/rebalance/add/rebalance */
void dao_rbtree_replace(struct dao_rbtree_node *old, struct dao_rbtree_node *new, struct dao_rbtree *root);

static inline void dao_rbtree_link_node(struct dao_rbtree_node * node, struct dao_rbtree_node * parent, struct dao_rbtree_node ** link)
{
	node->parent_color = (unsigned long )parent;
	node->left = node->right = NULL;

	*link = node;
}

typedef int (*dao_rbtree_node_compare)(struct dao_rbtree_node * , struct dao_rbtree_node *);

struct dao_rbtree_node* dao_rbtree_lookup(struct dao_rbtree_node* key, dao_rbtree_node_compare cmp, struct dao_rbtree* root);
struct dao_rbtree_node *dao_rbtree_insert(struct dao_rbtree_node* node, dao_rbtree_node_compare cmp, struct dao_rbtree* root);

#endif	/* DAO_KERNEL_RBTREE_H */
