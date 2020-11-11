#ifndef DAO_STORAGE_BTREE_WRITER_H_
#define DAO_STORAGE_BTREE_WRITER_H_

#include <stdint.h>

#define DAO_STORAGE_BTREE_WRITER_PRIVATE	\
    int fd;                     \
    char *filename;             \
    uint64_t filesize;          \
    char padding[DAO_STORAGE_BTREE_PADDING];

typedef struct _dao_storage_btree_writer_s _dao_storage_btree_writer_t;
typedef int (*_dao_storage_btree_writer_cb)(_dao_storage_btree_writer_t *w, void *data);

enum comp_type {
    kNotCompressed = 0,
    kCompressed = 1
};

int _dao_storage_btree_writer_create(_dao_storage_btree_writer_t *w, const char *filename);
int _dao_storage_btree_writer_destroy(_dao_storage_btree_writer_t *w);

int _dao_storage_btree_writer_fsync(_dao_storage_btree_writer_t *w);

int _dao_storage_btree_writer_compact_name(_dao_storage_btree_writer_t *w, char **compact_name);
int _dao_storage_btree_writer_compact_finalize(_dao_storage_btree_writer_t *s, _dao_storage_btree_writer_t *t);

int _dao_storage_btree_writer_read(_dao_storage_btree_writer_t *w,
                    const enum comp_type comp,
                    const uint64_t offset,
                    uint64_t *size,
                    void **data);
int _dao_storage_btree_writer_write(_dao_storage_btree_writer_t *w,
                     const enum comp_type comp,
                     const void *data,
                     uint64_t *offset,
                     uint64_t *size);

int _dao_storage_btree_writer_find(_dao_storage_btree_writer_t *w,
                    const enum comp_type comp,
                    const uint64_t size,
                    void *data,
                    _dao_storage_btree_writer_cb seek,
                    _dao_storage_btree_writer_cb miss);

struct _dao_storage_btree_writer_s {
    DAO_STORAGE_BTREE_WRITER_PRIVATE
};

#endif /* DAO_STORAGE_BTREE_WRITER_H_ */
