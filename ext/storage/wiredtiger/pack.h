
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

#include "storage/wiredtiger/cursor.h"

#include <unistd.h>

typedef struct _dao_storage_wiredtiger_pack_item {
	char *data;
	size_t size;
	size_t asize;
} dao_storage_wiredtiger_pack_item;

void dao_storage_wiredtiger_pack_item_free(dao_storage_wiredtiger_pack_item *item);
int dao_storage_wiredtiger_pack_key(dao_storage_wiredtiger_cursor_object *intern, dao_storage_wiredtiger_pack_item *item, zval *key);
int dao_storage_wiredtiger_pack_value(dao_storage_wiredtiger_cursor_object *intern, dao_storage_wiredtiger_pack_item *item, zval *value);
int dao_storage_wiredtiger_unpack_key(dao_storage_wiredtiger_cursor_object *intern, zval *return_value, WT_ITEM *item);
int dao_storage_wiredtiger_unpack_value(dao_storage_wiredtiger_cursor_object *intern, zval *return_value, WT_ITEM *item);
