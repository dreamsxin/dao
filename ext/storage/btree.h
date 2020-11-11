
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

#ifndef DAO_STORAGE_BTREE_H
#define DAO_STORAGE_BTREE_H

#include "php_dao.h"
#include "storage/btree/bplus.h"

typedef struct {
	dao_storage_btree_db_t db;
	zend_object std;
} dao_storage_btree_object;

static inline dao_storage_btree_object *dao_storage_btree_object_from_obj(zend_object *obj) {
	return (dao_storage_btree_object*)((char*)(obj) - XtOffsetOf(dao_storage_btree_object, std));
}

extern zend_class_entry *dao_storage_btree_ce;

DAO_INIT_CLASS(Dao_Storage_Btree);

#endif /* DAO_STORAGE_BTREE_H */
