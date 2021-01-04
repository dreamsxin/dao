
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

#include "kernel/mpool.h"

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

size_t dao_memory_pool_size_hint(size_t itemsize, size_t nitem)
{
	size_t const tagssize = dao_memory_pool_size_aligned(itemsize) * nitem;
	size_t const bitssize = dao_memory_pool_size_bits(tagssize / sizeof(dao_memory_pool_tag));
	return tagssize + bitssize + sizeof(dao_memory_pool);
}

size_t dao_memory_pool_size_stuff(size_t memsize)
{
	size_t const asize = memsize - dao_memory_pool_size_hint(0, 0);
	size_t ntags = asize / sizeof(dao_memory_pool_tag);
	while (dao_memory_pool_size_bits(ntags)	+ ntags * sizeof(dao_memory_pool_tag) > asize) {
		--ntags;
	}
	return memsize - (ntags * sizeof(dao_memory_pool_tag) - sizeof(size_t));
}

dao_memory_pool* dao_memory_pool_attach(void* src)
{
	dao_memory_pool* p = (dao_memory_pool*) src;
	if (BOM == (BOM & p->bom)) return p;
	fprintf(stderr, "%s invalid bom marker\n", __func__);
	return NULL;
}

dao_memory_pool* dao_memory_pool_format(void* src, size_t nsrc)
{
	if (nsrc < dao_memory_pool_size_hint(0, 0) ) {
		fprintf(stderr, "%s need %zu or more memory \n", __func__,
						dao_memory_pool_size_hint(0, 0));
		return NULL;
	}

	size_t const asize = nsrc - dao_memory_pool_size_hint(0, 0);
	size_t ntags = asize / sizeof(dao_memory_pool_tag);
	while (dao_memory_pool_size_bits(ntags)	+ ntags * sizeof(dao_memory_pool_tag) > asize)
		--ntags;

	dao_memory_pool* p = (dao_memory_pool*) src;
	memset(p, 0, sizeof(*p));

	p->bom = BOM;
	p->size = nsrc;
	p->ntags = ntags;
	dao_memory_pool_clear(p);
	return p;
}

void dao_memory_pool_clear(dao_memory_pool* p)
{
	dao_memory_void_set(&p->bits, p + 1);
	uint8_t* bits = dao_memory_void_get(&p->bits);
	memset(bits, 0, dao_memory_pool_size_bits(p->ntags));

	dao_memory_void_set(&p->tags, bits + dao_memory_pool_size_bits(p->ntags));
	dao_memory_void_set(&p->freetag, dao_memory_void_get(&p->tags));

	dao_memory_pool_tag* tag = dao_memory_void_get(&p->freetag);
	tag->size = p->ntags * sizeof(dao_memory_pool_tag);
	dao_memory_pool_tag_link(tag, NULL);
}

size_t dao_memory_pool_memory_size(dao_memory_pool* p)
{
	return p->size;
}

size_t dao_memory_pool_capacity(dao_memory_pool* p)
{
	if (!p->ntags) return 0;
	return p->ntags * sizeof(dao_memory_pool_tag) - sizeof(size_t);
}

size_t dao_memory_pool_balance(dao_memory_pool* p)
{
	return p->balance;
}

double dao_memory_pool_load(dao_memory_pool* p)
{
	if (!p->ntags) return 100.;
	return (double)p->balance / (p->ntags * sizeof(dao_memory_pool_tag));
}

size_t dao_memory_pool_avail(dao_memory_pool* p)
{
		size_t avail = 0;
		dao_memory_pool_tag* tag = dao_memory_void_get(&p->freetag);
		while (tag != NULL) {
			avail += (tag->size - sizeof(size_t));
			tag = dao_memory_pool_tag_next(tag);
		}
		return avail;
}

void dao_memory_pool_dump(dao_memory_pool* p)
{
	size_t const avail = dao_memory_pool_avail(p);
	fprintf(stderr, "> page [%p] size: %zu tags: %zu capacity: %zu avail: %zu used: %zu balance: %zu load: %f\n",
		(void*)p, p->size, p->ntags, dao_memory_pool_capacity(p), avail, dao_memory_pool_capacity(p) - avail,
		p->balance, dao_memory_pool_load(p));
}

void* dao_memory_pool_alloc(dao_memory_pool* p, size_t size)
{
	size_t const aligned = dao_memory_pool_size_aligned(size);

	dao_memory_pool_tag* prev = NULL;
	dao_memory_pool_tag* tag = dao_memory_void_get(&p->freetag);
	while (tag && tag->size < aligned) {
		prev = tag;
		tag = dao_memory_pool_tag_next(tag);
	}

	if (!tag) {
		fprintf(stderr, "%s no memory\n", __func__);
		return NULL;
	}

	if (tag->size > aligned) {
		dao_memory_pool_tag* n = (dao_memory_pool_tag*)((char*)tag + aligned);
		n->size = tag->size - aligned;
		tag->size = aligned;
		dao_memory_pool_tag_link(n, dao_memory_pool_tag_next(tag));
		dao_memory_pool_tag_link(tag, n);
		dao_memory_pool_tag_merge(n, p);
	}

	if (prev) {
		dao_memory_pool_tag_link(prev, dao_memory_pool_tag_next(tag));
	} else {
		dao_memory_void_set(&p->freetag, dao_memory_pool_tag_next(tag));
	}

	dao_memory_pool_bits_drop(tag, p);
	p->balance += aligned;
	return dao_memory_pool_tag_tomem(tag);
}

void* dao_memory_pool_realloc(dao_memory_pool* p, void* ptr, size_t newsz)
{
	if (!ptr) return dao_memory_pool_alloc(p, newsz);

	size_t const aligned = dao_memory_pool_size_aligned(newsz);
	dao_memory_pool_tag* tag = dao_memory_pool_tag_ofmem(ptr);

	if (aligned == tag->size) {
		return ptr;
	}

	if (tag->size < aligned) {
		void* np = dao_memory_pool_alloc(p, newsz);
		if (!np) return NULL;
		memcpy(np, ptr, tag->size - sizeof(size_t));
		dao_memory_pool_free(p, ptr);
		return np;
	}

	dao_memory_pool_tag* n = (dao_memory_pool_tag*)((char*)tag + aligned);
	n->size = tag->size - aligned;
	tag->size = aligned;

	if (p->balance < n->size) {
		fprintf(stderr, "%s pool balance error\n", __func__);
		return NULL;
	}

	p->balance -= n->size;

	dao_memory_pool_tag* tmp = dao_memory_void_get(&p->freetag);

	if (!tmp || tmp > n) {
		dao_memory_void_set(&p->freetag, n);
		dao_memory_pool_tag_link(n, tmp);
		dao_memory_pool_tag_merge(n, p);
		return ptr;
	}

	tmp = dao_memory_pool_tag_left(tag, p);

	if (!tmp) {
		fprintf(stderr, "%s bits error \n", __func__);
		return NULL;
	}

	dao_memory_pool_tag_link(n, dao_memory_pool_tag_next(tmp));
	dao_memory_pool_tag_link(tmp, n);

	dao_memory_pool_tag_merge(n, p);
	dao_memory_pool_tag_merge(tmp, p);
	return ptr;
}

void* dao_memory_pool_zalloc(dao_memory_pool* p, size_t nbytes)
{
	void* r = dao_memory_pool_alloc(p, nbytes);
	if (r) memset(r, 0, nbytes);
	return r;
}

void dao_memory_pool_free(dao_memory_pool* p, void* ptr)
{
	if (!ptr) {
		return;
	}

	dao_memory_pool_tag* tag = dao_memory_pool_tag_ofmem(ptr);

	if (p->balance < tag->size) {
		fprintf(stderr, "%s pool balance error\n", __func__);
		return;
	}

	p->balance -= tag->size;

	dao_memory_pool_tag* tmp = dao_memory_void_get(&p->freetag);

	if (!tmp || tmp > tag) {
		dao_memory_pool_tag_link(tag, tmp);
		dao_memory_pool_tag_merge(tag, p);
		dao_memory_void_set(&p->freetag, tag);
		return;
	}

	tmp = dao_memory_pool_tag_left(tag, p);

	if (!tmp) {
		fprintf(stderr, "%s bits error \n", __func__);
		return;
	}

	dao_memory_pool_tag_link(tag, dao_memory_pool_tag_next(tmp));
	dao_memory_pool_tag_link(tmp, tag);

	dao_memory_pool_tag_merge(tag, p);
	dao_memory_pool_tag_merge(tmp, p);
}

void* dao_memory_pool_memdup(dao_memory_pool* p, void* d, size_t dsz)
{
	void* r = dao_memory_pool_alloc(p, dsz);
	if (r) {
		memcpy(r, d, dsz);
	}
	return r;
}
