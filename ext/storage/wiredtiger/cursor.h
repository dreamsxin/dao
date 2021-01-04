
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

#ifndef DAO_STORAGE_WIREDTIGER_CURSOR_H
#define DAO_STORAGE_WIREDTIGER_CURSOR_H

#include "php_dao.h"
# if DAO_USE_WIREDTIGER

#include <wiredtiger.h>

typedef struct {
    WT_ITEM key;
    WT_ITEM value;
} dao_storage_wiredtiger_cursor_current;

typedef struct {
    zend_object *db;
    WT_CURSOR *cursor;
    dao_storage_wiredtiger_cursor_current current;
	zend_bool started_iterating;
	zend_object std;
} dao_storage_wiredtiger_cursor_object;

static inline dao_storage_wiredtiger_cursor_object *dao_storage_wiredtiger_cursor_object_from_obj(zend_object *obj) {
	return (dao_storage_wiredtiger_cursor_object*)((char*)(obj) - XtOffsetOf(dao_storage_wiredtiger_cursor_object, std));
}

extern zend_class_entry *dao_storage_wiredtiger_cursor_ce;

DAO_INIT_CLASS(Dao_Storage_Wiredtiger_Cursor);

# endif
#endif /* DAO_STORAGE_WIREDTIGER_CURSOR_H */
