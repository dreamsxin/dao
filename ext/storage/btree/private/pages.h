#ifndef DAO_STORAGE_BTREE_PAGES_H_
#define DAO_STORAGE_BTREE_PAGES_H_

#include "storage/btree/private/tree.h"
#include "storage/btree/private/values.h"

typedef struct _dao_storage_btree_page_s _dao_storage_btree_page_t;
typedef struct _dao_storage_btree_page_search_res_s _dao_storage_btree_page_search_res_t;

enum page_type {
    kPage = 0,
    kLeaf = 1
};

enum search_type {
    kNotLoad = 0,
    kLoad = 1
};

int _dao_storage_btree_page_create(dao_storage_btree_db_t *t,
                    const enum page_type type,
                    const uint64_t offset,
                    const uint64_t config,
                    _dao_storage_btree_page_t **page);
void _dao_storage_btree_page_destroy(dao_storage_btree_db_t *t, _dao_storage_btree_page_t *page);
int _dao_storage_btree_page_clone(dao_storage_btree_db_t *t, _dao_storage_btree_page_t *page, _dao_storage_btree_page_t **clone);

int _dao_storage_btree_page_read(dao_storage_btree_db_t *t, _dao_storage_btree_page_t *page);
int _dao_storage_btree_page_load(dao_storage_btree_db_t *t,
                  const uint64_t offset,
                  const uint64_t config,
                  _dao_storage_btree_page_t **page);
int _dao_storage_btree_page_save(dao_storage_btree_db_t *t, _dao_storage_btree_page_t *page);

int _dao_storage_btree_page_load_value(dao_storage_btree_db_t *t,
                        _dao_storage_btree_page_t *page,
                        const uint64_t index,
                        dao_storage_btree_value_t *value);
int _dao_storage_btree_page_save_value(dao_storage_btree_db_t *t,
                        _dao_storage_btree_page_t *page,
                        const uint64_t index,
                        const int cmp,
                        const dao_storage_btree_key_t *key,
                        const dao_storage_btree_value_t *value,
                        dao_storage_btree_update_cb cb,
                        void *arg);

int _dao_storage_btree_page_search(dao_storage_btree_db_t *t,
                    _dao_storage_btree_page_t *page,
                    const dao_storage_btree_key_t *key,
                    const enum search_type type,
                    _dao_storage_btree_page_search_res_t *result);
int _dao_storage_btree_page_get(dao_storage_btree_db_t *t,
                 _dao_storage_btree_page_t *page,
                 const dao_storage_btree_key_t *key,
                 dao_storage_btree_value_t *value);
int _dao_storage_btree_page_get_range(dao_storage_btree_db_t *t,
                       _dao_storage_btree_page_t *page,
                       const dao_storage_btree_key_t *start,
                       const dao_storage_btree_key_t *end,
                       dao_storage_btree_filter_cb filter,
                       dao_storage_btree_range_cb cb,
                       void *arg);
int _dao_storage_btree_page_insert(dao_storage_btree_db_t *t,
                    _dao_storage_btree_page_t *page,
                    const dao_storage_btree_key_t *key,
                    const dao_storage_btree_value_t *value,
                    dao_storage_btree_update_cb update_cb,
                    void *arg);
int _dao_storage_btree_page_bulk_insert(dao_storage_btree_db_t *t,
                         _dao_storage_btree_page_t *page,
                         const dao_storage_btree_key_t *limit,
                         uint64_t *count,
                         dao_storage_btree_key_t **keys,
                         dao_storage_btree_value_t **values,
                         dao_storage_btree_update_cb update_cb,
                         void *arg);
int _dao_storage_btree_page_remove(dao_storage_btree_db_t *t,
                    _dao_storage_btree_page_t *page,
                    const dao_storage_btree_key_t *key,
                    dao_storage_btree_remove_cb remove_cb,
                    void *arg);
int _dao_storage_btree_page_copy(dao_storage_btree_db_t *source, dao_storage_btree_db_t *target, _dao_storage_btree_page_t *page);

int _dao_storage_btree_page_remove_idx(dao_storage_btree_db_t *t, _dao_storage_btree_page_t *page, const uint64_t index);
int _dao_storage_btree_page_split(dao_storage_btree_db_t *t,
                   _dao_storage_btree_page_t *parent,
                   const uint64_t index,
                   _dao_storage_btree_page_t *child);
int _dao_storage_btree_page_split_head(dao_storage_btree_db_t *t, _dao_storage_btree_page_t **page);

void _dao_storage_btree_page_shiftr(dao_storage_btree_db_t *t, _dao_storage_btree_page_t *page, const uint64_t index);
void _dao_storage_btree_page_shiftl(dao_storage_btree_db_t *t, _dao_storage_btree_page_t *page, const uint64_t index);

struct _dao_storage_btree_page_s {
    enum page_type type;

    uint64_t length;
    uint64_t byte_size;

    uint64_t offset;
    uint64_t config;

    void *buff_;
    int is_head;

    _dao_storage_btree_kv_t keys[1];
};

struct _dao_storage_btree_page_search_res_s {
    _dao_storage_btree_page_t *child;

    uint64_t index;
    int cmp;
};

#endif /* DAO_STORAGE_BTREE_PAGES_H_ */
