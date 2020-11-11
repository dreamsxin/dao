#include <assert.h> /* assert */

#include "storage/btree/bplus.h"
#include "storage/btree/private/pages.h"
#include "storage/btree/private/utils.h"

#include "kernel/main.h"

int _dao_storage_btree_page_create(dao_storage_btree_db_t *t,
                    const enum page_type type,
                    const uint64_t offset,
                    const uint64_t config,
                    _dao_storage_btree_page_t **page)
{
    /* Allocate space for page + keys */
    _dao_storage_btree_page_t *p;

    p = emalloc(sizeof(*p) + sizeof(p->keys[0]) * (t->head.page_size - 1));
    if (p == NULL) return DAO_STORAGE_BTREE_EALLOC;

    p->type = type;
    if (type == kLeaf) {
        p->length = 0;
        p->byte_size = 0;
    } else {
        /* non-leaf pages always have left element */
        p->length = 1;
        p->keys[0].value = NULL;
        p->keys[0].length = 0;
        p->keys[0].offset = 0;
        p->keys[0].config = 0;
        p->keys[0].allocated = 0;
        p->byte_size = DAO_STORAGE_BTREE__KV_SIZE(p->keys[0]);
    }

    /* this two fields will be changed on page_write */
    p->offset = offset;
    p->config = config;

    p->buff_ = NULL;
    p->is_head = 0;

    *page = p;
    return DAO_STORAGE_BTREE_OK;
}

void _dao_storage_btree_page_destroy(dao_storage_btree_db_t *t, _dao_storage_btree_page_t *page)
{
    uint64_t i;
    /* Free all keys */
    for (i = 0; i < page->length; i++) {
        if (page->keys[i].allocated) {
            efree(page->keys[i].value);
            page->keys[i].value = NULL;
        }
    }

    if (page->buff_ != NULL) {
        efree(page->buff_);
        page->buff_ = NULL;
    }

    /* Free page itself */
    efree(page);
}

int _dao_storage_btree_page_clone(dao_storage_btree_db_t *t, _dao_storage_btree_page_t *page, _dao_storage_btree_page_t **clone)
{
    uint64_t i;
    int ret = DAO_STORAGE_BTREE_OK;
    ret = _dao_storage_btree_page_create(t, page->type, page->offset, page->config, clone);
    if (ret != DAO_STORAGE_BTREE_OK) return ret;

    (*clone)->is_head = page->is_head;

    (*clone)->length = 0;
    for (i = 0; i < page->length; i++) {
        ret = _dao_storage_btree_kv_copy(&page->keys[i], &(*clone)->keys[i], 1);
        (*clone)->length++;
        if (ret != DAO_STORAGE_BTREE_OK) break;
    }
    (*clone)->byte_size = page->byte_size;

    /* if failed - efree memory for all allocated keys */
    if (ret != DAO_STORAGE_BTREE_OK) _dao_storage_btree_page_destroy(t, *clone);

    return ret;
}

int _dao_storage_btree_page_read(dao_storage_btree_db_t *t, _dao_storage_btree_page_t *page)
{
    int ret;
    uint64_t size, o;
    uint64_t i;
    _dao_storage_btree_writer_t *w = (_dao_storage_btree_writer_t *) t;

    char *buff = NULL;

    /* Read page size and leaf flag */
    size = page->config >> 1;
    page->type = page->config & 1 ? kLeaf : kPage;

    /* Read page data */
    ret = _dao_storage_btree_writer_read(w, kCompressed, page->offset, &size, (void**) &buff);
    if (ret != DAO_STORAGE_BTREE_OK) return ret;

    /* Parse data */
    i = 0;
    o = 0;
    while (o < size) {
        page->keys[i].length = _dao_ntohll(*(uint64_t *) (buff + o));
        page->keys[i].offset = _dao_ntohll(*(uint64_t *) (buff + o + 8));
        page->keys[i].config = _dao_ntohll(*(uint64_t *) (buff + o + 16));
        page->keys[i].value = buff + o + 24;
        page->keys[i].allocated = 0;

        o += DAO_STORAGE_BTREE__KV_SIZE(page->keys[i]);
        i++;
    }
    page->length = i;
    page->byte_size = size;

    if (page->buff_ != NULL) {
        efree(page->buff_);
    }
    page->buff_ = buff;

    return DAO_STORAGE_BTREE_OK;
}

int _dao_storage_btree_page_load(dao_storage_btree_db_t *t,
                  const uint64_t offset,
                  const uint64_t config,
                  _dao_storage_btree_page_t **page)
{
    int ret;

    _dao_storage_btree_page_t *new_page;
    ret = _dao_storage_btree_page_create(t, 0, offset, config, &new_page);
    if (ret != DAO_STORAGE_BTREE_OK) return ret;

    ret = _dao_storage_btree_page_read(t, new_page);
    if (ret != DAO_STORAGE_BTREE_OK) {
        _dao_storage_btree_page_destroy(t, new_page);
        return ret;
    }

    /* _dao_storage_btree_page_load should be atomic */
    *page = new_page;

    return DAO_STORAGE_BTREE_OK;
}

int _dao_storage_btree_page_save(dao_storage_btree_db_t *t, _dao_storage_btree_page_t *page)
{
    int ret;
    _dao_storage_btree_writer_t *w = (_dao_storage_btree_writer_t *) t;
    uint64_t i;
    uint64_t o;
    char *buff;

    assert(page->type == kLeaf || page->length != 0);

    /* Allocate space for serialization (header + keys); */
    buff = emalloc(page->byte_size);
    if (buff == NULL) return DAO_STORAGE_BTREE_EALLOC;

    o = 0;
    for (i = 0; i < page->length; i++) {
        assert(o + DAO_STORAGE_BTREE__KV_SIZE(page->keys[i]) <= page->byte_size);

        *(uint64_t *) (buff + o) = _dao_htonll(page->keys[i].length);
        *(uint64_t *) (buff + o + 8) = _dao_htonll(page->keys[i].offset);
        *(uint64_t *) (buff + o + 16) = _dao_htonll(page->keys[i].config);

        memcpy(buff + o + 24, page->keys[i].value, page->keys[i].length);

        o += DAO_STORAGE_BTREE__KV_SIZE(page->keys[i]);
    }
    assert(o == page->byte_size);

    page->config = page->byte_size;
    ret = _dao_storage_btree_writer_write(w,
                           kCompressed,
                           buff,
                           &page->offset,
                           &page->config);
    page->config = (page->config << 1) | (page->type == kLeaf);

    efree(buff);
    return ret;
}


int _dao_storage_btree_page_load_value(dao_storage_btree_db_t *t,
                        _dao_storage_btree_page_t *page,
                        const uint64_t index,
                        dao_storage_btree_value_t *value)
{
    return _dao_storage_btree_value_load(t,
                          page->keys[index].offset,
                          page->keys[index].config,
                          value);
}


int _dao_storage_btree_page_save_value(dao_storage_btree_db_t *t,
                        _dao_storage_btree_page_t *page,
                        const uint64_t index,
                        const int cmp,
                        const dao_storage_btree_key_t *key,
                        const dao_storage_btree_value_t *value,
                        dao_storage_btree_update_cb update_cb,
                        void *arg)
{
    int ret;
    _dao_storage_btree_kv_t previous, tmp;

    /* replace item with same key from page */
    if (cmp == 0) {
        /* solve conflicts if callback was provided */
        if (update_cb != NULL) {
            dao_storage_btree_value_t prev_value;

            ret = _dao_storage_btree_page_load_value(t, page, index, &prev_value);
            if (ret != DAO_STORAGE_BTREE_OK) return ret;

            ret = update_cb(arg, &prev_value, value);
            efree(prev_value.value);

            if (!ret) return DAO_STORAGE_BTREE_EUPDATECONFLICT;
        }
        previous.offset = page->keys[index].offset;
        previous.length = page->keys[index].config;
        _dao_storage_btree_page_remove_idx(t, page, index);
    }

    /* store key */
    tmp.value = key->value;
    tmp.length = key->length;

    /* store value */
    ret = _dao_storage_btree_value_save(t,
                         value,
                         cmp == 0 ? &previous : NULL,
                         &tmp.offset,
                         &tmp.config);
    if (ret != DAO_STORAGE_BTREE_OK) return ret;

    /* Shift all keys right */
    _dao_storage_btree_page_shiftr(t, page, index);

    /* Insert key in the middle */
    ret = _dao_storage_btree_kv_copy(&tmp, &page->keys[index], 1);
    if (ret != DAO_STORAGE_BTREE_OK) {
        /* shift keys back */
        _dao_storage_btree_page_shiftl(t, page, index);
        return ret;
    }

    page->byte_size += DAO_STORAGE_BTREE__KV_SIZE(tmp);
    page->length++;

    return DAO_STORAGE_BTREE_OK;
}

int _dao_storage_btree_page_search(dao_storage_btree_db_t *t,
                    _dao_storage_btree_page_t *page,
                    const dao_storage_btree_key_t *key,
                    const enum search_type type,
                    _dao_storage_btree_page_search_res_t *result)
{
    int ret;
    uint64_t i = page->type == kPage;
    int cmp = -1;
    _dao_storage_btree_page_t *child;

    /* assert infinite recursion */
    assert(page->type == kLeaf || page->length > 0);

    while (i < page->length) {
        /* left key is always lower in non-leaf nodes */
        cmp = t->compare_cb((dao_storage_btree_key_t*) &page->keys[i], key);

        if (cmp >= 0) break;
        i++;
    }

    result->cmp = cmp;

    if (page->type == kLeaf) {
        result->index = i;
        result->child = NULL;

        return DAO_STORAGE_BTREE_OK;
    } else {
        assert(i > 0);
        if (cmp != 0) i--;

        if (type == kLoad) {
            ret = _dao_storage_btree_page_load(t,
                                page->keys[i].offset,
                                page->keys[i].config,
                                &child);
            if (ret != DAO_STORAGE_BTREE_OK) return ret;

            result->child = child;
        } else {
            result->child = NULL;
        }

        result->index = i;

        return DAO_STORAGE_BTREE_OK;
    }
}

int _dao_storage_btree_page_get(dao_storage_btree_db_t *t,
                 _dao_storage_btree_page_t *page,
                 const dao_storage_btree_key_t *key,
                 dao_storage_btree_value_t *value)
{
    int ret;
    _dao_storage_btree_page_search_res_t res;
    ret = _dao_storage_btree_page_search(t, page, key, kLoad, &res);
    if (ret != DAO_STORAGE_BTREE_OK) return ret;

    if (res.child == NULL) {
        if (res.cmp != 0) return DAO_STORAGE_BTREE_ENOTFOUND;

        return _dao_storage_btree_page_load_value(t, page, res.index, value);
    } else {
        ret = _dao_storage_btree_page_get(t, res.child, key, value);
        _dao_storage_btree_page_destroy(t, res.child);
        res.child = NULL;
        return ret;
    }
}

int _dao_storage_btree_page_get_range(dao_storage_btree_db_t *t,
                       _dao_storage_btree_page_t *page,
                       const dao_storage_btree_key_t *start,
                       const dao_storage_btree_key_t *end,
                       dao_storage_btree_filter_cb filter,
                       dao_storage_btree_range_cb cb,
                       void *arg)
{
    int ret;
    uint64_t i;
    _dao_storage_btree_page_search_res_t start_res, end_res;

    /* find start and end indexes */
    ret = _dao_storage_btree_page_search(t, page, start, kNotLoad, &start_res);
    if (ret != DAO_STORAGE_BTREE_OK) return ret;
    ret = _dao_storage_btree_page_search(t, page, end, kNotLoad, &end_res);
    if (ret != DAO_STORAGE_BTREE_OK) return ret;

    if (page->type == kLeaf) {
        /* on leaf pages end-key should always be greater or equal than first key */
        if (end_res.cmp > 0 && end_res.index == 0) return DAO_STORAGE_BTREE_OK;

        if (end_res.cmp < 0) end_res.index--;
    }

    /* go through each page item */
    for (i = start_res.index; i <= end_res.index; i++) {
        /* run filter */
        if (!filter(arg, (dao_storage_btree_key_t *) &page->keys[i])) continue;

        if (page->type == kPage) {
            /* load child page and apply range get to it */
            _dao_storage_btree_page_t* child;

            ret = _dao_storage_btree_page_load(t,
                                page->keys[i].offset,
                                page->keys[i].config,
                                &child);
            if (ret != DAO_STORAGE_BTREE_OK) return ret;

            ret = _dao_storage_btree_page_get_range(t, child, start, end, filter, cb, arg);

            /* destroy child regardless of error */
            _dao_storage_btree_page_destroy(t, child);

            if (ret != DAO_STORAGE_BTREE_OK) return ret;
        } else {
            /* load value and pass it to callback */
            dao_storage_btree_value_t value;
            ret = _dao_storage_btree_page_load_value(t, page, i, &value);
            if (ret != DAO_STORAGE_BTREE_OK) return ret;

            cb(arg, (dao_storage_btree_key_t *) &page->keys[i], &value);

            efree(value.value);
        }
    }

    return DAO_STORAGE_BTREE_OK;
}

int _dao_storage_btree_page_insert(dao_storage_btree_db_t *t,
                    _dao_storage_btree_page_t *page,
                    const dao_storage_btree_key_t *key,
                    const dao_storage_btree_value_t *value,
                    dao_storage_btree_update_cb update_cb,
                    void *arg)
{
    int ret;
    _dao_storage_btree_page_search_res_t res;
    ret = _dao_storage_btree_page_search(t, page, key, kLoad, &res);
    if (ret != DAO_STORAGE_BTREE_OK) return ret;

    if (res.child == NULL) {
        /* store value in db file to get offset and config */
        ret = _dao_storage_btree_page_save_value(t,
                                  page,
                                  res.index,
                                  res.cmp,
                                  key,
                                  value,
                                  update_cb,
                                  arg);
        if (ret != DAO_STORAGE_BTREE_OK) return ret;
    } else {
        /* Insert kv in child page */
        ret = _dao_storage_btree_page_insert(t, res.child, key, value, update_cb, arg);

        /* kv was inserted but page is full now */
        if (ret == DAO_STORAGE_BTREE_ESPLITPAGE) {
            ret = _dao_storage_btree_page_split(t, page, res.index, res.child);
        } else if (ret == DAO_STORAGE_BTREE_OK) {
            /* Update offsets in page */
            page->keys[res.index].offset = res.child->offset;
            page->keys[res.index].config = res.child->config;
        }

        _dao_storage_btree_page_destroy(t, res.child);
        res.child = NULL;

        if (ret != DAO_STORAGE_BTREE_OK) {
            return ret;
        }
    }

    if (page->length == t->head.page_size) {
        if (page->is_head) {
            ret = _dao_storage_btree_page_split_head(t, &page);
            if (ret != DAO_STORAGE_BTREE_OK) return ret;
        } else {
            /* Notify caller that it should split page */
            return DAO_STORAGE_BTREE_ESPLITPAGE;
        }
    }

    assert(page->length < t->head.page_size);

    ret = _dao_storage_btree_page_save(t, page);
    if (ret != DAO_STORAGE_BTREE_OK) return ret;

    return DAO_STORAGE_BTREE_OK;
}

int _dao_storage_btree_page_bulk_insert(dao_storage_btree_db_t *t,
                         _dao_storage_btree_page_t *page,
                         const dao_storage_btree_key_t *limit,
                         uint64_t *count,
                         dao_storage_btree_key_t **keys,
                         dao_storage_btree_value_t **values,
                         dao_storage_btree_update_cb update_cb,
                         void *arg)
{
    int ret;
    _dao_storage_btree_page_search_res_t res;

    while (*count > 0 &&
            (limit == NULL || t->compare_cb(limit, *keys) > 0)) {

        ret = _dao_storage_btree_page_search(t, page, *keys, kLoad, &res);
        if (ret != DAO_STORAGE_BTREE_OK) return ret;

        if (res.child == NULL) {
            /* store value in db file to get offset and config */
            ret = _dao_storage_btree_page_save_value(t,
                                      page,
                                      res.index,
                                      res.cmp,
                                      *keys,
                                      *values,
                                      update_cb,
                                      arg);
            /*
             * ignore update conflicts, to handle situations where
             * only one kv failed in a bulk
             */
            if (ret != DAO_STORAGE_BTREE_OK && ret != DAO_STORAGE_BTREE_EUPDATECONFLICT) return ret;

            *keys = *keys + 1;
            *values = *values + 1;
            *count = *count - 1;
        } else {
            /* we're in regular page */
            dao_storage_btree_key_t* new_limit = NULL;

            if (res.index + 1 < page->length) {
                new_limit = (dao_storage_btree_key_t*) &page->keys[res.index + 1];
            }

            ret = _dao_storage_btree_page_bulk_insert(t,
                                       res.child,
                                       new_limit,
                                       count,
                                       keys,
                                       values,
                                       update_cb,
                                       arg);
            if (ret == DAO_STORAGE_BTREE_ESPLITPAGE) {
                ret = _dao_storage_btree_page_split(t, page, res.index, res.child);
            } else if (ret == DAO_STORAGE_BTREE_OK) {
                /* Update offsets in page */
                page->keys[res.index].offset = res.child->offset;
                page->keys[res.index].config = res.child->config;
            }

            _dao_storage_btree_page_destroy(t, res.child);
            res.child = NULL;

            if (ret != DAO_STORAGE_BTREE_OK) return ret;
        }

        if (page->length == t->head.page_size) {
            if (page->is_head) {
                ret = _dao_storage_btree_page_split_head(t, &page);
                if (ret != DAO_STORAGE_BTREE_OK) return ret;
            } else {
                /* Notify caller that it should split page */
                return DAO_STORAGE_BTREE_ESPLITPAGE;
            }
        }

        assert(page->length < t->head.page_size);
    }

    return _dao_storage_btree_page_save(t, page);
}

int _dao_storage_btree_page_remove(dao_storage_btree_db_t *t,
                    _dao_storage_btree_page_t *page,
                    const dao_storage_btree_key_t *key,
                    dao_storage_btree_remove_cb remove_cb,
                    void *arg)
{
    int ret;
    _dao_storage_btree_page_search_res_t res;
    ret = _dao_storage_btree_page_search(t, page, key, kLoad, &res);
    if (ret != DAO_STORAGE_BTREE_OK) return ret;

    if (res.child == NULL) {
        if (res.cmp != 0) return DAO_STORAGE_BTREE_ENOTFOUND;

        /* remove only if remove_cb returns DAO_STORAGE_BTREE_OK */
        if (remove_cb != NULL) {
            dao_storage_btree_value_t prev_val;

            ret = _dao_storage_btree_page_load_value(t, page, res.index, &prev_val);
            if (ret != DAO_STORAGE_BTREE_OK) return ret;

            ret = remove_cb(arg, &prev_val);
            efree(prev_val.value);

            if (!ret) return DAO_STORAGE_BTREE_EREMOVECONFLICT;
        }
        _dao_storage_btree_page_remove_idx(t, page, res.index);

        if (page->length == 0 && !page->is_head) return DAO_STORAGE_BTREE_EEMPTYPAGE;
    } else {
        /* Insert kv in child page */
        ret = _dao_storage_btree_page_remove(t, res.child, key, remove_cb, arg);

        if (ret != DAO_STORAGE_BTREE_OK && ret != DAO_STORAGE_BTREE_EEMPTYPAGE) {
            return ret;
        }

        /* kv was inserted but page is full now */
        if (ret == DAO_STORAGE_BTREE_EEMPTYPAGE) {
            _dao_storage_btree_page_remove_idx(t, page, res.index);

            /* we don't need child now */
            _dao_storage_btree_page_destroy(t, res.child);
            res.child = NULL;

            /* only one item left - lift kv from last child to current page */
            if (page->length == 1) {
                page->offset = page->keys[0].offset;
                page->config = page->keys[0].config;

                /* remove child to efree memory */
                _dao_storage_btree_page_remove_idx(t, page, 0);

                /* and load child as current page */
                ret = _dao_storage_btree_page_read(t, page);
                if (ret != DAO_STORAGE_BTREE_OK) return ret;
            }
        } else {
            /* Update offsets in page */
            page->keys[res.index].offset = res.child->offset;
            page->keys[res.index].config = res.child->config;

            /* we don't need child now */
            _dao_storage_btree_page_destroy(t, res.child);
            res.child = NULL;
        }
    }

    return _dao_storage_btree_page_save(t, page);
}

int _dao_storage_btree_page_copy(dao_storage_btree_db_t *source, dao_storage_btree_db_t *target, _dao_storage_btree_page_t *page)
{
    uint64_t i;
    int ret;
    for (i = 0; i < page->length; i++) {
        if (page->type == kPage) {
            /* copy child page */
            _dao_storage_btree_page_t *child;
            ret = _dao_storage_btree_page_load(source,
                                page->keys[i].offset,
                                page->keys[i].config,
                                &child);
            if (ret != DAO_STORAGE_BTREE_OK) return ret;

            ret = _dao_storage_btree_page_copy(source, target, child);
            if (ret != DAO_STORAGE_BTREE_OK) return ret;

            /* update child position */
            page->keys[i].offset = child->offset;
            page->keys[i].config = child->config;

            _dao_storage_btree_page_destroy(source, child);
        } else {
            /* copy value */
            dao_storage_btree_value_t value;

            ret = _dao_storage_btree_page_load_value(source, page, i, &value);
            if (ret != DAO_STORAGE_BTREE_OK) return ret;

            page->keys[i].config = value.length;
            ret = _dao_storage_btree_value_save(target,
                                 &value,
                                 NULL,
                                 &page->keys[i].offset,
                                 &page->keys[i].config);

            /* value is not needed anymore */
            efree(value.value);
            if (ret != DAO_STORAGE_BTREE_OK) return ret;
        }
    }

    return _dao_storage_btree_page_save(target, page);
}


int _dao_storage_btree_page_remove_idx(dao_storage_btree_db_t *t, _dao_storage_btree_page_t *page, const uint64_t index)
{
    assert(index < page->length);

    /* Free memory allocated for kv and reduce byte_size of page */
    page->byte_size -= DAO_STORAGE_BTREE__KV_SIZE(page->keys[index]);
    if (page->keys[index].allocated) {
        efree(page->keys[index].value);
        page->keys[index].value = NULL;
    }

    /* Shift all keys left */
    _dao_storage_btree_page_shiftl(t, page, index);

    page->length--;

    return DAO_STORAGE_BTREE_OK;
}


int _dao_storage_btree_page_split(dao_storage_btree_db_t *t,
                   _dao_storage_btree_page_t *parent,
                   const uint64_t index,
                   _dao_storage_btree_page_t *child)
{
    int ret;
    uint64_t i, middle;
    _dao_storage_btree_page_t *left = NULL, *right = NULL;
    _dao_storage_btree_kv_t middle_key;

    _dao_storage_btree_page_create(t, child->type, 0, 0, &left);
    _dao_storage_btree_page_create(t, child->type, 0, 0, &right);

    middle = t->head.page_size >> 1;
    ret = _dao_storage_btree_kv_copy(&child->keys[middle], &middle_key, 1);
    if (ret != DAO_STORAGE_BTREE_OK) goto fatal;

    /* non-leaf nodes has byte_size > 0 nullify it first */
    left->byte_size = 0;
    left->length = 0;
    for (i = 0; i < middle; i++) {
        ret = _dao_storage_btree_kv_copy(&child->keys[i], &left->keys[left->length++], 1);
        if (ret != DAO_STORAGE_BTREE_OK) goto fatal;
        left->byte_size += DAO_STORAGE_BTREE__KV_SIZE(child->keys[i]);
    }

    right->byte_size = 0;
    right->length = 0;
    for (; i < t->head.page_size; i++) {
        ret = _dao_storage_btree_kv_copy(&child->keys[i], &right->keys[right->length++], 1);
        if (ret != DAO_STORAGE_BTREE_OK) goto fatal;
        right->byte_size += DAO_STORAGE_BTREE__KV_SIZE(child->keys[i]);
    }

    /* save left and right parts to get offsets */
    ret = _dao_storage_btree_page_save(t, left);
    if (ret != DAO_STORAGE_BTREE_OK) goto fatal;

    ret = _dao_storage_btree_page_save(t, right);
    if (ret != DAO_STORAGE_BTREE_OK) goto fatal;

    /* store offsets with middle key */
    middle_key.offset = right->offset;
    middle_key.config = right->config;

    /* insert middle key into parent page */
    _dao_storage_btree_page_shiftr(t, parent, index + 1);
    _dao_storage_btree_kv_copy(&middle_key, &parent->keys[index + 1], 0);

    parent->byte_size += DAO_STORAGE_BTREE__KV_SIZE(middle_key);
    parent->length++;

    /* change left element */
    parent->keys[index].offset = left->offset;
    parent->keys[index].config = left->config;

    ret = DAO_STORAGE_BTREE_OK;
fatal:
    /* cleanup */
    _dao_storage_btree_page_destroy(t, left);
    _dao_storage_btree_page_destroy(t, right);
    return ret;
}

int _dao_storage_btree_page_split_head(dao_storage_btree_db_t *t, _dao_storage_btree_page_t **page)
{
    int ret;
    _dao_storage_btree_page_t *new_head = NULL;
    _dao_storage_btree_page_create(t, 0, 0, 0, &new_head);
    new_head->is_head = 1;

    ret = _dao_storage_btree_page_split(t, new_head, 0, *page);
    if (ret != DAO_STORAGE_BTREE_OK) {
        _dao_storage_btree_page_destroy(t, new_head);
        return ret;
    }

    t->head.page = new_head;
    _dao_storage_btree_page_destroy(t, *page);
    *page = new_head;

    return DAO_STORAGE_BTREE_OK;
}

void _dao_storage_btree_page_shiftr(dao_storage_btree_db_t *t, _dao_storage_btree_page_t *p, const uint64_t index)
{
    uint64_t i;
    if (p->length != 0) {
        for (i = p->length - 1; i >= index; i--) {
            _dao_storage_btree_kv_copy(&p->keys[i], &p->keys[i + 1], 0);

            if (i == 0) break;
        }
    }
}

void _dao_storage_btree_page_shiftl(dao_storage_btree_db_t *t, _dao_storage_btree_page_t *p, const uint64_t index)
{
    uint64_t i;
    for (i = index + 1; i < p->length; i++) {
        _dao_storage_btree_kv_copy(&p->keys[i], &p->keys[i - 1], 0);
    }
}
