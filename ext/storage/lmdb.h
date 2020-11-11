
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

#ifndef DAO_STORAGE_LMDB_H
#define DAO_STORAGE_LMDB_H

#include "php_dao.h"
#include "storage/lmdb/lmdb.h"

typedef struct {
	MDB_env *env;
	MDB_dbi dbi;
	MDB_txn *txn;
	int flags;
	zend_object std;
} dao_storage_lmdb_object;

static inline dao_storage_lmdb_object *dao_storage_lmdb_object_from_obj(zend_object *obj) {
	return (dao_storage_lmdb_object*)((char*)(obj) - XtOffsetOf(dao_storage_lmdb_object, std));
}

extern zend_class_entry *dao_storage_lmdb_ce;

DAO_INIT_CLASS(Dao_Storage_Lmdb);

#endif /* DAO_STORAGE_LMDB_H */
