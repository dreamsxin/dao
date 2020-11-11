
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

#ifndef DAO_KERNEL_MEMORY_POOL_H
#define DAO_KERNEL_MEMORY_POOL_H

#include "kernel/memory.h"

#include <stddef.h>
#include <stdint.h>

static const size_t BOM = 0xfafafafafafafafa;

typedef struct _dao_memory_pool {
    size_t bom;
    size_t size;
    size_t balance;
    size_t ntags;
    dao_memory_void_value bits;
    dao_memory_void_value tags;
    dao_memory_void_value freetag;
} dao_memory_pool;

typedef struct _dao_memory_pool_tag {
  size_t size;
  dao_memory_void_value link;
} dao_memory_pool_tag;

static inline void* dao_memory_pool_tag_tomem(dao_memory_pool_tag* tag) {
  return (void*)&tag->link;
}

static inline dao_memory_pool_tag* dao_memory_pool_tag_ofmem(void* mem) {
  return (dao_memory_pool_tag *)((char*)mem - sizeof(size_t));
}

static inline dao_memory_pool_tag* dao_memory_pool_tag_next(dao_memory_pool_tag* tag) {
  return (dao_memory_pool_tag *)dao_memory_void_get(&tag->link);
}

static inline void dao_memory_pool_tag_link(dao_memory_pool_tag* tag, dao_memory_pool_tag* link) {
  dao_memory_void_set(&tag->link, link);
}


static size_t dao_memory_pool_size_aligned(size_t need) {
  size_t const ntags = (1 + (need + sizeof(size_t) - 1) / sizeof(dao_memory_pool_tag));
  return sizeof(dao_memory_pool_tag) * ntags;
}

static inline size_t dao_memory_pool_size_bits(size_t ntags)
{
  return (ntags + 7) / 8;
}

static inline void dao_memory_pool_bits_set(dao_memory_pool_tag* tag, dao_memory_pool* p) {
  uint8_t* const bits = dao_memory_void_get(&p->bits);
  size_t const bit = tag - (dao_memory_pool_tag*)dao_memory_void_get(&p->tags);
  bits[bit / 8] |= 1 << (bit % 8);
}

static inline void dao_memory_pool_bits_drop(dao_memory_pool_tag* tag, dao_memory_pool* p) {
  uint8_t* const bits = dao_memory_void_get(&p->bits);
  size_t const bit = tag - (dao_memory_pool_tag*)dao_memory_void_get(&p->tags);
  bits[bit / 8] &= ~(1 << (bit % 8));
}

static dao_memory_pool_tag* dao_memory_pool_tag_left(dao_memory_pool_tag* tag, dao_memory_pool* p)
{
  dao_memory_pool_tag* tags = dao_memory_void_get(&p->tags);
  uint8_t* bits = dao_memory_void_get(&p->bits);
  size_t bit = tag - tags;
  while (bit --> 0) {
    if (bits[bit / 8]) {
       if (1 << (bit % 8) == (bits[bit / 8] & (1 << (bit % 8))))
         return tags + bit;
    }
  }
  return NULL;
}

static void dao_memory_pool_tag_merge(dao_memory_pool_tag* tag, dao_memory_pool* p)
{
  dao_memory_pool_tag* n;
  while ((n = dao_memory_pool_tag_next(tag))) {

    if ((char*)n > (char*)tag + tag->size)
      break;

    tag->size += n->size;
    dao_memory_pool_tag_link(tag, dao_memory_pool_tag_next(n));
    dao_memory_pool_bits_drop(n, p);
  }

  dao_memory_pool_bits_set(tag, p);
}

size_t dao_memory_pool_size_hint(size_t itemsize, size_t nitem);
size_t dao_memory_pool_size_stuff(size_t memsize);

dao_memory_pool* dao_memory_pool_attach(void* src);
dao_memory_pool* dao_memory_pool_format(void* src, size_t nsrc);
void dao_memory_pool_clear(dao_memory_pool* p);
size_t dao_memory_pool_memory_size(dao_memory_pool* p);
size_t dao_memory_pool_capacity(dao_memory_pool* p);
size_t dao_memory_pool_balance(dao_memory_pool* p);
size_t dao_memory_pool_avail(dao_memory_pool* p);
double dao_memory_pool_load(dao_memory_pool* p);
void* dao_memory_pool_alloc(dao_memory_pool* p, size_t size);
void* dao_memory_pool_realloc(dao_memory_pool* p, void* ptr, size_t newsz);
void* dao_memory_pool_zalloc(dao_memory_pool* p, size_t);
void  dao_memory_pool_free(dao_memory_pool* p, void* ptr);
void* dao_memory_pool_memdup(dao_memory_pool* p, void* ptr, size_t dsz);

#endif /* DAO_KERNEL_MEMORY_POOL_H */
