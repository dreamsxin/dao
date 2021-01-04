
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

#include "storage/leveldb/iterator.h"
#include "storage/exception.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "kernel/file.h"
#include "kernel/exception.h"

/**
 * Dao\Storage\Leveldb\Iterator
 *
 */
zend_class_entry *dao_storage_leveldb_iterator_ce;

PHP_METHOD(Dao_Storage_Leveldb_Iterator, __construct);
PHP_METHOD(Dao_Storage_Leveldb_Iterator, current);
PHP_METHOD(Dao_Storage_Leveldb_Iterator, key);
PHP_METHOD(Dao_Storage_Leveldb_Iterator, next);
PHP_METHOD(Dao_Storage_Leveldb_Iterator, prev);
PHP_METHOD(Dao_Storage_Leveldb_Iterator, rewind);
PHP_METHOD(Dao_Storage_Leveldb_Iterator, last);
PHP_METHOD(Dao_Storage_Leveldb_Iterator, seek);
PHP_METHOD(Dao_Storage_Leveldb_Iterator, valid);

static const zend_function_entry dao_storage_leveldb_iterator_method_entry[] = {
	PHP_ME(Dao_Storage_Leveldb_Iterator, __construct, arginfo_empty, ZEND_ACC_PRIVATE|ZEND_ACC_CTOR|ZEND_ACC_FINAL)
	PHP_ME(Dao_Storage_Leveldb_Iterator, current, arginfo_iterator_current, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Leveldb_Iterator, key, arginfo_iterator_key, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Leveldb_Iterator, next, arginfo_iterator_next, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Leveldb_Iterator, prev, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Leveldb_Iterator, rewind, arginfo_iterator_rewind, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Leveldb_Iterator, last, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Leveldb_Iterator, seek, arginfo_seekableiterator_seek, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Leveldb_Iterator, valid, arginfo_iterator_valid, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

zend_object_handlers dao_storage_leveldb_iterator_object_handlers;
zend_object* dao_storage_leveldb_iterator_object_create_handler(zend_class_entry *ce)
{
	dao_storage_leveldb_iterator_object *intern = ecalloc(1, sizeof(dao_storage_leveldb_iterator_object) + zend_object_properties_size(ce));
	intern->std.ce = ce;

	zend_object_std_init(&intern->std, ce);
	object_properties_init(&intern->std, ce);
	intern->std.handlers = &dao_storage_leveldb_iterator_object_handlers;

	return &intern->std;
}

void dao_storage_leveldb_iterator_object_free_handler(zend_object *object)
{
	dao_storage_leveldb_iterator_object *intern = dao_storage_leveldb_iterator_object_from_obj(object);
	if (intern->iterator) {
		leveldb_iter_destroy(intern->iterator);
		intern->iterator = NULL;
	}
}

/**
 * Dao\Storage\Leveldb\Iterator initializer
 */
DAO_INIT_CLASS(Dao_Storage_Leveldb_Iterator){

	DAO_REGISTER_CLASS_CREATE_OBJECT(Dao\\Storage\\Leveldb, Iterator, storage_leveldb_iterator, dao_storage_leveldb_iterator_method_entry, 0);

	zend_class_implements(dao_storage_leveldb_iterator_ce, 2, zend_ce_iterator, spl_ce_SeekableIterator);

	return SUCCESS;
}

/**
 * Dao\Storage\Leveldb\Iterator constructor
 *
 */
PHP_METHOD(Dao_Storage_Leveldb_Iterator, __construct)
{
	/* this constructor shouldn't be called as it's private */
	zend_throw_exception(NULL, "An object of this type cannot be created with the new operator.", 0);
}

/**
 * Return current element
 *
 * @return string
 */
PHP_METHOD(Dao_Storage_Leveldb_Iterator, current)
{
	char *value = NULL;
	size_t value_len;
	dao_storage_leveldb_iterator_object *intern;

	intern = dao_storage_leveldb_iterator_object_from_obj(Z_OBJ_P(getThis()));

	if (!leveldb_iter_valid(intern->iterator) || !(value = (char *)leveldb_iter_value(intern->iterator, &value_len))) {
		RETURN_FALSE;
	}

	RETURN_STRINGL(value, value_len);
}

/**
 * Returns the key of current element
 *
 * @return string
 */
PHP_METHOD(Dao_Storage_Leveldb_Iterator, key)
{
	char *key = NULL;
	size_t key_len;
	dao_storage_leveldb_iterator_object *intern;

	intern = dao_storage_leveldb_iterator_object_from_obj(Z_OBJ_P(getThis()));

	if (!leveldb_iter_valid(intern->iterator) || !(key = (char *)leveldb_iter_key(intern->iterator, &key_len))) {
		RETURN_FALSE;
	}

	RETURN_STRINGL(key, key_len);
}

/**
 * Moves forward to the next element
 *
 * @return boolean
 */
PHP_METHOD(Dao_Storage_Leveldb_Iterator, next)
{
	dao_storage_leveldb_iterator_object *intern;

	intern = dao_storage_leveldb_iterator_object_from_obj(Z_OBJ_P(getThis()));

	if (leveldb_iter_valid(intern->iterator)) {
		leveldb_iter_next(intern->iterator);
		RETURN_TRUE;
	} else {
		RETURN_FALSE;
	}
}

/**
 * Moves backward to the previous element
 *
 * @return boolean
 */
PHP_METHOD(Dao_Storage_Leveldb_Iterator, prev)
{
	dao_storage_leveldb_iterator_object *intern;

	intern = dao_storage_leveldb_iterator_object_from_obj(Z_OBJ_P(getThis()));

	if (leveldb_iter_valid(intern->iterator)) {
		leveldb_iter_prev(intern->iterator);
	}
	RETURN_TRUE;
}

/**
 * Rewinds back to the first element of the Iterator
 *
 * @return boolean
 */
PHP_METHOD(Dao_Storage_Leveldb_Iterator, rewind)
{
	dao_storage_leveldb_iterator_object *intern;

	intern = dao_storage_leveldb_iterator_object_from_obj(Z_OBJ_P(getThis()));

    leveldb_iter_seek_to_first(intern->iterator);
	RETURN_TRUE;
}

/**
 * Moves to the last element of the Iterator
 *
 * @return boolean
 */
PHP_METHOD(Dao_Storage_Leveldb_Iterator, last)
{
	dao_storage_leveldb_iterator_object *intern;

	intern = dao_storage_leveldb_iterator_object_from_obj(Z_OBJ_P(getThis()));

    leveldb_iter_seek_to_last(intern->iterator);
	RETURN_TRUE;
}

/**
 * Seeks to a given key in the Iterator if no such key it will point to nearest key
 *
 * @return boolean
 */
PHP_METHOD(Dao_Storage_Leveldb_Iterator, seek)
{
	zval *key;
	dao_storage_leveldb_iterator_object *intern;

	dao_fetch_params(0, 1, 0, &key);

	intern = dao_storage_leveldb_iterator_object_from_obj(Z_OBJ_P(getThis()));

	leveldb_iter_seek(intern->iterator, Z_STRVAL_P(key), Z_STRLEN_P(key));
	RETURN_TRUE;
}

/**
 * Checks if current position is valid
 *
 * @return boolean
 */
PHP_METHOD(Dao_Storage_Leveldb_Iterator, valid)
{
	dao_storage_leveldb_iterator_object *intern;

	intern = dao_storage_leveldb_iterator_object_from_obj(Z_OBJ_P(getThis()));

	RETURN_BOOL(leveldb_iter_valid(intern->iterator));
}
