#ifndef DAO_STORAGE_BTREE_VALUES_H_
#define DAO_STORAGE_BTREE_VALUES_H_

#include "storage/btree/private/tree.h"
#include <stdint.h>

#define DAO_STORAGE_BTREE__KV_HEADER_SIZE 24
#define DAO_STORAGE_BTREE__KV_SIZE(kv)  DAO_STORAGE_BTREE__KV_HEADER_SIZE + kv.length
#define DAO_STORAGE_BTREE__STOVAL(str, key)		\
    key.value = (char *) str;           \
    key.length = strlen(str) + 1;

#define DAO_STORAGE_BTREE_KEY_PRIVATE                  \
    uint64_t _prev_offset;              \
    uint64_t _prev_length;

typedef struct _dao_storage_btree_kv_s _dao_storage_btree_kv_t;


int _dao_storage_btree_value_load(dao_storage_btree_db_t *t,
                   const uint64_t offset,
                   const uint64_t length,
                   dao_storage_btree_value_t *value);
int _dao_storage_btree_value_save(dao_storage_btree_db_t *t,
                   const dao_storage_btree_value_t *value,
                   const _dao_storage_btree_kv_t *previous,
                   uint64_t *offset,
                   uint64_t *length);

int _dao_storage_btree_kv_copy(const _dao_storage_btree_kv_t *source, _dao_storage_btree_kv_t *target, int alloc);

struct _dao_storage_btree_kv_s {
    DAO_STORAGE_BTREE_KEY_FIELDS

    uint64_t offset;
    uint64_t config;

    uint8_t allocated;
};

#endif /* DAO_STORAGE_BTREE_VALUES_H_ */
