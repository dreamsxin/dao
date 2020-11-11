#include "storage/btree/bplus.h"
#include "storage/btree/private/utils.h"

#include "kernel/main.h"

int dao_storage_btree_open(dao_storage_btree_db_t *tree, const char* filename)
{
    int ret;

    ret = pthread_rwlock_init(&tree->rwlock, NULL) ? DAO_STORAGE_BTREE_ERWLOCK : DAO_STORAGE_BTREE_OK;
    if (ret != DAO_STORAGE_BTREE_OK) return ret;

    ret = _dao_storage_btree_writer_create((_dao_storage_btree_writer_t*) tree, filename);
    if (ret != DAO_STORAGE_BTREE_OK) goto fatal;

    tree->head.page = NULL;

    ret = _dao_storage_btree_init(tree);
    if (ret != DAO_STORAGE_BTREE_OK) goto fatal;

    return DAO_STORAGE_BTREE_OK;

fatal:
    pthread_rwlock_destroy(&tree->rwlock);
    return ret;
}

int dao_storage_btree_close(dao_storage_btree_db_t *tree)
{
    pthread_rwlock_wrlock(&tree->rwlock);
    _dao_storage_btree_destroy(tree);
    pthread_rwlock_unlock(&tree->rwlock);

    pthread_rwlock_destroy(&tree->rwlock);
    return DAO_STORAGE_BTREE_OK;
}

int _dao_storage_btree_init(dao_storage_btree_db_t *tree)
{
    int ret;
    /*
     * Load head.
     * Writer will not compress data chunk smaller than head,
     * that's why we're passing head size as compressed size here
     */
    ret = _dao_storage_btree_writer_find((_dao_storage_btree_writer_t *) tree,
                          kNotCompressed,
                          DAO_STORAGE_BTREE__HEAD_SIZE,
                          &tree->head,
                          _dao_storage_btree_tree_read_head,
                          _dao_storage_btree_tree_write_head);
    if (ret == DAO_STORAGE_BTREE_OK) {
        /* set default compare function */
        dao_storage_btree_set_compare_cb(tree, _dao_storage_btree_default_compare_cb);
    }

    return ret;
}

void _dao_storage_btree_destroy(dao_storage_btree_db_t *tree)
{
    _dao_storage_btree_writer_destroy((_dao_storage_btree_writer_t *) tree);
    if (tree->head.page != NULL) {
        _dao_storage_btree_page_destroy(tree, tree->head.page);
        tree->head.page = NULL;
    }
}

int dao_storage_btree_get(dao_storage_btree_db_t *tree, const dao_storage_btree_key_t* key, dao_storage_btree_value_t *value)
{
    int ret;

    pthread_rwlock_rdlock(&tree->rwlock);

    ret = _dao_storage_btree_page_get(tree, tree->head.page, key, value);

    pthread_rwlock_unlock(&tree->rwlock);

    return ret;
}


int dao_storage_btree_get_previous(dao_storage_btree_db_t *tree,
                    const dao_storage_btree_value_t *value,
                    dao_storage_btree_value_t *previous)
{
    if (value->_prev_offset == 0 && value->_prev_length == 0) {
        return DAO_STORAGE_BTREE_ENOTFOUND;
    }
    return _dao_storage_btree_value_load(tree,
                          value->_prev_offset,
                          value->_prev_length,
                          previous);
}

int dao_storage_btree_update(dao_storage_btree_db_t *tree,
              const dao_storage_btree_key_t *key,
              const dao_storage_btree_value_t *value,
              dao_storage_btree_update_cb update_cb,
              void *arg)
{
    int ret;

    pthread_rwlock_wrlock(&tree->rwlock);

    ret = _dao_storage_btree_page_insert(tree, tree->head.page, key, value, update_cb, arg);
    if (ret == DAO_STORAGE_BTREE_OK) {
        ret = _dao_storage_btree_tree_write_head((_dao_storage_btree_writer_t*) tree, NULL);
    }

    pthread_rwlock_unlock(&tree->rwlock);

    return ret;
}

int dao_storage_btree_bulk_update(dao_storage_btree_db_t *tree,
                   const uint64_t count,
                   const dao_storage_btree_key_t **keys,
                   const dao_storage_btree_value_t **values,
                   dao_storage_btree_update_cb update_cb,
                   void *arg)
{
    int ret;
    dao_storage_btree_key_t *keys_iter = (dao_storage_btree_key_t *) *keys;
    dao_storage_btree_value_t* values_iter = (dao_storage_btree_value_t *) *values;
    uint64_t left = count;

    pthread_rwlock_wrlock(&tree->rwlock);

    ret = _dao_storage_btree_page_bulk_insert(tree,
                               tree->head.page,
                               NULL,
                               &left,
                               &keys_iter,
                               &values_iter,
                               update_cb,
                               arg);
    if (ret == DAO_STORAGE_BTREE_OK) {
        ret =  _dao_storage_btree_tree_write_head((_dao_storage_btree_writer_t *) tree, NULL);
    }

    pthread_rwlock_unlock(&tree->rwlock);

    return ret;
}


int dao_storage_btree_set(dao_storage_btree_db_t *tree, const dao_storage_btree_key_t *key, const dao_storage_btree_value_t *value)
{
    return dao_storage_btree_update(tree, key, value, NULL, NULL);
}


int dao_storage_btree_bulk_set(dao_storage_btree_db_t *tree,
                const uint64_t count,
                const dao_storage_btree_key_t **keys,
                const dao_storage_btree_value_t **values)
{
    return dao_storage_btree_bulk_update(tree, count, keys, values, NULL, NULL);
}


int dao_storage_btree_removev(dao_storage_btree_db_t *tree,
               const dao_storage_btree_key_t *key,
               dao_storage_btree_remove_cb remove_cb,
               void *arg)
{
    int ret;

    pthread_rwlock_wrlock(&tree->rwlock);

    ret = _dao_storage_btree_page_remove(tree, tree->head.page, key, remove_cb, arg);
    if (ret == DAO_STORAGE_BTREE_OK) {
        ret = _dao_storage_btree_tree_write_head((_dao_storage_btree_writer_t *) tree, NULL);
    }

    pthread_rwlock_unlock(&tree->rwlock);

    return ret;
}

int dao_storage_btree_remove(dao_storage_btree_db_t *tree, const dao_storage_btree_key_t *key)
{
    return dao_storage_btree_removev(tree, key, NULL, NULL);
}

int dao_storage_btree_compact(dao_storage_btree_db_t *tree)
{
    int ret;
    char *compacted_name;
    dao_storage_btree_db_t compacted;

    /* get name of compacted database (prefixed with .compact) */
    ret = _dao_storage_btree_writer_compact_name((_dao_storage_btree_writer_t *) tree, &compacted_name);
    if (ret != DAO_STORAGE_BTREE_OK) return ret;

    /* open it */
    ret = dao_storage_btree_open(&compacted, compacted_name);
    efree(compacted_name);
    if (ret != DAO_STORAGE_BTREE_OK) return ret;

    /* destroy stub head page */
    _dao_storage_btree_page_destroy(&compacted, compacted.head.page);

    pthread_rwlock_rdlock(&tree->rwlock);

    /* clone source tree's head page */
    ret = _dao_storage_btree_page_clone(&compacted, tree->head.page, &compacted.head.page);

    pthread_rwlock_unlock(&tree->rwlock);

    /* copy all pages starting from head */
    ret = _dao_storage_btree_page_copy(tree, &compacted, compacted.head.page);
    if (ret != DAO_STORAGE_BTREE_OK) return ret;

    ret = _dao_storage_btree_tree_write_head((_dao_storage_btree_writer_t *) &compacted, NULL);
    if (ret != DAO_STORAGE_BTREE_OK) return ret;

    pthread_rwlock_wrlock(&tree->rwlock);

    ret = _dao_storage_btree_writer_compact_finalize((_dao_storage_btree_writer_t *) tree,
                                      (_dao_storage_btree_writer_t *) &compacted);
    pthread_rwlock_unlock(&tree->rwlock);

    return ret;
}

int dao_storage_btree_get_filtered_range(dao_storage_btree_db_t *tree,
                          const dao_storage_btree_key_t *start,
                          const dao_storage_btree_key_t *end,
                          dao_storage_btree_filter_cb filter,
                          dao_storage_btree_range_cb cb,
                          void *arg)
{
    int ret;

    pthread_rwlock_rdlock(&tree->rwlock);

    ret = _dao_storage_btree_page_get_range(tree,
                             tree->head.page,
                             start,
                             end,
                             filter,
                             cb,
                             arg);

    pthread_rwlock_unlock(&tree->rwlock);

    return ret;
}

int dao_storage_btree_get_range(dao_storage_btree_db_t *tree,
                 const dao_storage_btree_key_t *start,
                 const dao_storage_btree_key_t *end,
                 dao_storage_btree_range_cb cb,
                 void *arg)
{
    return dao_storage_btree_get_filtered_range(tree,
                                 start,
                                 end,
                                 _dao_storage_btree_default_filter_cb,
                                 cb,
                                 arg);
}

/* Wrappers to allow string to string set/get/remove */

int dao_storage_btree_gets(dao_storage_btree_db_t *tree, const char *key, char **value)
{
    int ret;
    dao_storage_btree_key_t bkey;
    dao_storage_btree_value_t bvalue;

    DAO_STORAGE_BTREE__STOVAL(key, bkey);

    ret = dao_storage_btree_get(tree, &bkey, &bvalue);
    if (ret != DAO_STORAGE_BTREE_OK) return ret;

    *value = bvalue.value;

    return DAO_STORAGE_BTREE_OK;
}

int dao_storage_btree_updates(dao_storage_btree_db_t *tree,
               const char *key,
               const char *value,
               dao_storage_btree_update_cb update_cb,
               void *arg)
{
    dao_storage_btree_key_t bkey;
    dao_storage_btree_value_t bvalue;

    DAO_STORAGE_BTREE__STOVAL(key, bkey);
    DAO_STORAGE_BTREE__STOVAL(value, bvalue);

    return dao_storage_btree_update(tree, &bkey, &bvalue, update_cb, arg);
}


int dao_storage_btree_sets(dao_storage_btree_db_t *tree, const char *key, const char *value)
{
    return dao_storage_btree_updates(tree, key, value, NULL, NULL);
}

int dao_storage_btree_bulk_updates(dao_storage_btree_db_t *tree,
                    const uint64_t count,
                    const char **keys,
                    const char **values,
                    dao_storage_btree_update_cb update_cb,
                    void *arg)
{
    int ret;
    dao_storage_btree_key_t *bkeys;
    dao_storage_btree_value_t *bvalues;
    uint64_t i;

    /* allocated memory for keys/values */
    bkeys = emalloc(sizeof(*bkeys) * count);
    if (bkeys == NULL) return DAO_STORAGE_BTREE_EALLOC;

    bvalues = emalloc(sizeof(*bvalues) * count);
    if (bvalues == NULL) {
        efree(bkeys);
        return DAO_STORAGE_BTREE_EALLOC;
    }

    /* copy keys/values to allocated memory */
    for (i = 0; i < count; i++) {
        DAO_STORAGE_BTREE__STOVAL(keys[i], bkeys[i]);
        DAO_STORAGE_BTREE__STOVAL(values[i], bvalues[i]);
    }

    ret = dao_storage_btree_bulk_update(tree,
                         count,
                         (const dao_storage_btree_key_t **) &bkeys,
                         (const dao_storage_btree_value_t **) &bvalues,
                         update_cb,
                         arg);

    efree(bkeys);
    efree(bvalues);

    return ret;
}

int dao_storage_btree_bulk_sets(dao_storage_btree_db_t *tree,
                 const uint64_t count,
                 const char **keys,
                 const char **values)
{
    return dao_storage_btree_bulk_updates(tree, count, keys, values, NULL, NULL);
}

int dao_storage_btree_removevs(dao_storage_btree_db_t *tree,
                const char *key,
                dao_storage_btree_remove_cb remove_cb,
                void *arg)
{
    dao_storage_btree_key_t bkey;

    DAO_STORAGE_BTREE__STOVAL(key, bkey);

    return dao_storage_btree_removev(tree, &bkey, remove_cb, arg);
}

int dao_storage_btree_removes(dao_storage_btree_db_t *tree, const char *key)
{
    return dao_storage_btree_removevs(tree, key, NULL, NULL);
}

int dao_storage_btree_get_filtered_ranges(dao_storage_btree_db_t *tree,
                           const char *start,
                           const char *end,
                           dao_storage_btree_filter_cb filter,
                           dao_storage_btree_range_cb cb,
                           void *arg)
{
    dao_storage_btree_key_t bstart;
    dao_storage_btree_key_t bend;

    DAO_STORAGE_BTREE__STOVAL(start, bstart);
    DAO_STORAGE_BTREE__STOVAL(end, bend);

    return dao_storage_btree_get_filtered_range(tree, &bstart, &bend, filter, cb, arg);
}

int dao_storage_btree_get_ranges(dao_storage_btree_db_t *tree,
                  const char *start,
                  const char *end,
                  dao_storage_btree_range_cb cb,
                  void *arg)
{
    return dao_storage_btree_get_filtered_ranges(tree,
                                  start,
                                  end,
                                  _dao_storage_btree_default_filter_cb,
                                  cb,
                                  arg);
}

/* various functions */

void dao_storage_btree_set_compare_cb(dao_storage_btree_db_t *tree, dao_storage_btree_compare_cb cb)
{
    tree->compare_cb = cb;
}


int dao_storage_btree_fsync(dao_storage_btree_db_t *tree)
{
    int ret;

    pthread_rwlock_wrlock(&tree->rwlock);
    ret = _dao_storage_btree_writer_fsync((_dao_storage_btree_writer_t *) tree);
    pthread_rwlock_unlock(&tree->rwlock);

    return ret;
}

/* internal utils */

int _dao_storage_btree_tree_read_head(_dao_storage_btree_writer_t *w, void *data)
{
    int ret;
    dao_storage_btree_db_t *t = (dao_storage_btree_db_t *) w;
    _dao_storage_btree_tree_head_t* head = (_dao_storage_btree_tree_head_t *) data;

    t->head.offset = _dao_ntohll(head->offset);
    t->head.config = _dao_ntohll(head->config);
    t->head.page_size = _dao_ntohll(head->page_size);
    t->head.hash = _dao_ntohll(head->hash);

    /* we've copied all data - efree it */
    efree(data);

    /* Check hash first */
    if (_dao_storage_btree_compute_hashl(t->head.offset) != t->head.hash) return 1;

    ret = _dao_storage_btree_page_load(t, t->head.offset, t->head.config, &t->head.page);
    if (ret != DAO_STORAGE_BTREE_OK) return ret;

    t->head.page->is_head = 1;

    return ret;
}

int _dao_storage_btree_tree_write_head(_dao_storage_btree_writer_t *w, void *data)
{
    int ret;
    dao_storage_btree_db_t* t = (dao_storage_btree_db_t*) w;
    _dao_storage_btree_tree_head_t nhead;
    uint64_t offset;
    uint64_t size;

    if (t->head.page == NULL) {
        /* TODO: page size should be configurable */
        t->head.page_size = 64;

        /* Create empty leaf page */
        ret = _dao_storage_btree_page_create(t, kLeaf, 0, 1, &t->head.page);
        if (ret != DAO_STORAGE_BTREE_OK) return ret;

        t->head.page->is_head = 1;
    }

    /* Update head's position */
    t->head.offset = t->head.page->offset;
    t->head.config = t->head.page->config;

    t->head.hash = _dao_storage_btree_compute_hashl(t->head.offset);

    /* Create temporary head with fields in network byte order */
    nhead.offset = _dao_htonll(t->head.offset);
    nhead.config = _dao_htonll(t->head.config);
    nhead.page_size = _dao_htonll(t->head.page_size);
    nhead.hash = _dao_htonll(t->head.hash);

    size = DAO_STORAGE_BTREE__HEAD_SIZE;
    ret = _dao_storage_btree_writer_write(w,
                           kNotCompressed,
                           &nhead,
                           &offset,
                           &size);

    return ret;
}

int _dao_storage_btree_default_compare_cb(const dao_storage_btree_key_t *a, const dao_storage_btree_key_t *b)
{
    uint32_t i, len = a->length < b->length ? a->length : b->length;

    for (i = 0; i < len; i++) {
        if (a->value[i] != b->value[i])
            return (uint8_t) a->value[i] > (uint8_t) b->value[i] ? 1 : -1;
    }

    return a->length - b->length;
}


int _dao_storage_btree_default_filter_cb(void *arg, const dao_storage_btree_key_t *key)
{
    /* default filter accepts all keys */
    return 1;
}
