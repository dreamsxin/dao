
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

#ifndef DAO_STORAGE_LIBMDBX_H
#define DAO_STORAGE_LIBMDBX_H

#include "php_dao.h"

#ifdef DAO_USE_LIBMDBX

#include <mdbx.h>

typedef struct {
	MDBX_env *env;
	MDBX_dbi dbi;
	MDBX_txn *txn;
	int flags;
	zend_object std;
} dao_storage_libmdbx_object;

static inline dao_storage_libmdbx_object *dao_storage_libmdbx_object_from_obj(zend_object *obj) {
	return (dao_storage_libmdbx_object*)((char*)(obj) - XtOffsetOf(dao_storage_libmdbx_object, std));
}

extern zend_class_entry *dao_storage_libmdbx_ce;

DAO_INIT_CLASS(Dao_Storage_Libmdbx);

#endif
#endif /* DAO_STORAGE_LIBMDBX_H */
