
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

#ifndef DAO_KERNEL_HASH_H
#define DAO_KERNEL_HASH_H

#include "php_dao.h"
#include <Zend/zend.h>

#define dao_get_current_data_w(ht, pos) zend_hash_get_current_data_ex(ht, pos);

#define dao_is_iterable(var, array_hash, hash_pointer, duplicate, reverse) \
	if (!dao_is_iterable_ex(var, array_hash, hash_pointer, duplicate, reverse)) { \
		zend_error(E_ERROR, "The argument is not iterable()"); \
		return; \
	}

int dao_is_iterable_ex(zval *arr, HashTable **arr_hash, HashPosition *hash_position, int duplicate, int reverse);

static inline void dao_get_current_key(zval **key, HashTable *ht, HashPosition *pos)
{
	zval tmp;
	zend_hash_get_current_key_zval_ex(ht, &tmp, pos);
	ZVAL_COPY_VALUE(*key, &tmp);
}

static inline zval *dao_get_current_key_w(HashTable *ht, HashPosition *pos)
{
	zval *key, tmp;
	zend_hash_get_current_key_zval_ex(ht, &tmp, pos);

	DAO_ALLOC_INIT_ZVAL(key);
	ZVAL_COPY_VALUE(key, &tmp);
	return key;
}

/** int dao_has_numeric_keys(const zval *data); */
int dao_hash_update_or_insert(HashTable *ht, const zval *offset, zval *value);
zval* dao_hash_get(HashTable *ht, const zval *key, int type);
int dao_hash_unset(HashTable *ht, const zval *offset);

#endif /* DAO_KERNEL_HASH_H */
