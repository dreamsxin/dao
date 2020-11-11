
/*
  +------------------------------------------------------------------------+
  | Dao Framework                                                          |
  +------------------------------------------------------------------------+
  | Copyright (c) 2011-2014 Phalcon Team (http://www.phalconphp.com)       |
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
  +------------------------------------------------------------------------+
*/

#ifndef DAO_CACHE_BACKEND_MONGO_H
#define DAO_CACHE_BACKEND_MONGO_H

#include "php_dao.h"

#ifdef DAO_USE_MONGOC
#include <bson.h>
#include <bcon.h>
#include <mongoc.h>

typedef struct {
	mongoc_client_t *client;
	mongoc_collection_t  *collection;
	zend_object std;
} dao_cache_backend_mongo_object;

static inline dao_cache_backend_mongo_object *dao_cache_backend_mongo_object_from_obj(zend_object *obj) {
	return (dao_cache_backend_mongo_object*)((char*)(obj) - XtOffsetOf(dao_cache_backend_mongo_object, std));
}

#endif

extern zend_class_entry *dao_cache_backend_mongo_ce;

DAO_INIT_CLASS(Dao_Cache_Backend_Mongo);

#endif /* DAO_CACHE_BACKEND_MONGO_H */
