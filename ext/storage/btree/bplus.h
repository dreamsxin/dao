#ifndef _DAO_STORAGE_BTREE_BPLUS_H_
#define _DAO_STORAGE_BTREE_BPLUS_H_

#define DAO_STORAGE_BTREE_PADDING 64

#define DAO_STORAGE_BTREE_KEY_FIELDS   \
    uint64_t length;    \
    char *value;

#include <stdint.h> /* uintx_t */
#include "storage/btree/private/errors.h"

typedef struct dao_storage_btree_db_s dao_storage_btree_db_t;

typedef struct dao_storage_btree_key_s dao_storage_btree_key_t;
typedef struct dao_storage_btree_key_s dao_storage_btree_value_t;

typedef int (*dao_storage_btree_compare_cb)(const dao_storage_btree_key_t *a, const dao_storage_btree_key_t *b);
typedef int (*dao_storage_btree_update_cb)(void *arg,
                            const dao_storage_btree_value_t *previous,
                            const dao_storage_btree_value_t *value);
typedef int (*dao_storage_btree_remove_cb)(void *arg,
                            const dao_storage_btree_value_t *value);
typedef void (*dao_storage_btree_range_cb)(void *arg,
                            const dao_storage_btree_key_t *key,
                            const dao_storage_btree_value_t *value);
typedef int (*dao_storage_btree_filter_cb)(void* arg, const dao_storage_btree_key_t *key);

#include "private/tree.h"

/*
 * Open and close database
 */
int dao_storage_btree_open(dao_storage_btree_db_t *tree, const char *filename);
int dao_storage_btree_close(dao_storage_btree_db_t *tree);

/*
 * Get one value by key
 */
int dao_storage_btree_get(dao_storage_btree_db_t *tree, const dao_storage_btree_key_t *key, dao_storage_btree_value_t *value);
int dao_storage_btree_gets(dao_storage_btree_db_t *tree, const char *key, char **value);

/*
 * Get previous value
 */
int dao_storage_btree_get_previous(dao_storage_btree_db_t *tree,
                    const dao_storage_btree_value_t *value,
                    dao_storage_btree_value_t *previous);

/*
 * Set one value by key (without solving conflicts, overwrite)
 */
int dao_storage_btree_set(dao_storage_btree_db_t *tree,
           const dao_storage_btree_key_t *key,
           const dao_storage_btree_value_t *value);
int dao_storage_btree_sets(dao_storage_btree_db_t *tree,
            const char *key,
            const char *value);

/*
 * Update or create value by key (with solving conflicts)
 */
int dao_storage_btree_update(dao_storage_btree_db_t *tree,
              const dao_storage_btree_key_t *key,
              const dao_storage_btree_value_t *value,
              dao_storage_btree_update_cb update_cb,
              void *arg);
int dao_storage_btree_updates(dao_storage_btree_db_t *tree,
               const char *key,
               const char *value,
               dao_storage_btree_update_cb update_cb,
               void *arg);

/*
 * Set multiple values by keys
 */
int dao_storage_btree_bulk_set(dao_storage_btree_db_t *tree,
                const uint64_t count,
                const dao_storage_btree_key_t **keys,
                const dao_storage_btree_value_t **values);
int dao_storage_btree_bulk_sets(dao_storage_btree_db_t *tree,
                 const uint64_t count,
                 const char **keys,
                 const char **values);

/*
 * Update multiple values by keys
 */
int dao_storage_btree_bulk_update(dao_storage_btree_db_t *tree,
                   const uint64_t count,
                   const dao_storage_btree_key_t **keys,
                   const dao_storage_btree_value_t **values,
                   dao_storage_btree_update_cb update_cb,
                   void *arg);
int dao_storage_btree_bulk_updates(dao_storage_btree_db_t *tree,
                    const uint64_t count,
                    const char **keys,
                    const char **values,
                    dao_storage_btree_update_cb update_cb,
                    void *arg);

/*
 * Remove one value by key
 */
int dao_storage_btree_remove(dao_storage_btree_db_t *tree, const dao_storage_btree_key_t *key);
int dao_storage_btree_removes(dao_storage_btree_db_t *tree, const char *key);

/*
 * Remove value by key only if it's equal to specified one
 */
int dao_storage_btree_removev(dao_storage_btree_db_t *tree,
               const dao_storage_btree_key_t *key,
               dao_storage_btree_remove_cb remove_cb,
               void *arg);
int dao_storage_btree_removevs(dao_storage_btree_db_t *tree,
                const char *key,
                dao_storage_btree_remove_cb remove_cb,
                void *arg);

/*
 * Get all values in range
 * Note: value will be automatically efreed after invokation of callback
 */
int dao_storage_btree_get_range(dao_storage_btree_db_t *tree,
                 const dao_storage_btree_key_t *start,
                 const dao_storage_btree_key_t *end,
                 dao_storage_btree_range_cb cb,
                 void *arg);
int dao_storage_btree_get_ranges(dao_storage_btree_db_t *tree,
                  const char *start,
                  const char *end,
                  dao_storage_btree_range_cb cb,
                  void *arg);

/*
 * Get values in range (with custom key-filter)
 * Note: value will be automatically efreed after invokation of callback
 */
int dao_storage_btree_get_filtered_range(dao_storage_btree_db_t *tree,
                          const dao_storage_btree_key_t *start,
                          const dao_storage_btree_key_t *end,
                          dao_storage_btree_filter_cb filter,
                          dao_storage_btree_range_cb cb,
                          void *arg);
int dao_storage_btree_get_filtered_ranges(dao_storage_btree_db_t *tree,
                           const char *start,
                           const char *end,
                           dao_storage_btree_filter_cb filter,
                           dao_storage_btree_range_cb cb,
                           void *arg);

/*
 * Run compaction on database
 */
int dao_storage_btree_compact(dao_storage_btree_db_t *tree);

/*
 * Set compare function to define order of keys in database
 */
void dao_storage_btree_set_compare_cb(dao_storage_btree_db_t *tree, dao_storage_btree_compare_cb cb);

/*
 * Ensure that all data is written to disk
 */
int dao_storage_btree_fsync(dao_storage_btree_db_t *tree);

struct dao_storage_btree_db_s {
    DAO_STORAGE_BTREE_TREE_PRIVATE
};

struct dao_storage_btree_key_s {
    DAO_STORAGE_BTREE_KEY_FIELDS
    DAO_STORAGE_BTREE_KEY_PRIVATE
};

#endif /* _DAO_STORAGE_BTREE_BPLUS_H_ */
