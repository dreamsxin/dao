
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
  |          ZhuZongXin <dreamsxin@qq.com>                                 |
  +------------------------------------------------------------------------+
*/

#ifndef DAO_STORAGE_LEVELDB_H
#define DAO_STORAGE_LEVELDB_H

#include "php_dao.h"
# if DAO_USE_LEVELDB
#include <leveldb/c.h>

typedef struct {
	leveldb_t *db;
	zend_object std;
} dao_storage_leveldb_object;

static inline dao_storage_leveldb_object *dao_storage_leveldb_object_from_obj(zend_object *obj) {
	return (dao_storage_leveldb_object*)((char*)(obj) - XtOffsetOf(dao_storage_leveldb_object, std));
}

extern zend_class_entry *dao_storage_leveldb_ce;

DAO_INIT_CLASS(Dao_Storage_Leveldb);

# endif
#endif /* DAO_STORAGE_LEVELDB_H */
