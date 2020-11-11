#ifndef DAO_STORAGE_BTREE_TREE_H_
#define DAO_STORAGE_BTREE_TREE_H_

#include "storage/btree/private/writer.h"
#include "storage/btree/private/pages.h"

#include <pthread.h>

#define DAO_STORAGE_BTREE__HEAD_SIZE  sizeof(uint64_t) * 4

#define DAO_STORAGE_BTREE_TREE_PRIVATE         \
    DAO_STORAGE_BTREE_WRITER_PRIVATE           \
    pthread_rwlock_t rwlock;    \
    _dao_storage_btree_tree_head_t head;       \
    dao_storage_btree_compare_cb compare_cb;

typedef struct _dao_storage_btree_tree_head_s _dao_storage_btree_tree_head_t;

int _dao_storage_btree_init(dao_storage_btree_db_t *tree);
void _dao_storage_btree_destroy(dao_storage_btree_db_t *tree);

int _dao_storage_btree_tree_read_head(_dao_storage_btree_writer_t *w, void *data);
int _dao_storage_btree_tree_write_head(_dao_storage_btree_writer_t *w, void *data);

int _dao_storage_btree_default_compare_cb(const dao_storage_btree_key_t *a, const dao_storage_btree_key_t *b);
int _dao_storage_btree_default_filter_cb(void *arg, const dao_storage_btree_key_t *key);


struct _dao_storage_btree_tree_head_s {
    uint64_t offset;
    uint64_t config;
    uint64_t page_size;
    uint64_t hash;

    _dao_storage_btree_page_t *page;
};

#endif /* DAO_STORAGE_BTREE_TREE_H_ */
