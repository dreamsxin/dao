
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

#include "sync/sharedmemory.h"
#include "sync/exception.h"

#include "kernel/main.h"
#include "kernel/object.h"
#include "kernel/array.h"
#include "kernel/string.h"
#include "kernel/fcall.h"
#include "kernel/concat.h"
#include "kernel/operators.h"
#include "kernel/exception.h"
#include "kernel/time.h"

/**
 * Dao\Sync\Sharedmemory
 *
 */
zend_class_entry *dao_sync_sharedmemory_ce;

PHP_METHOD(Dao_Sync_Sharedmemory, __construct);
PHP_METHOD(Dao_Sync_Sharedmemory, first);
PHP_METHOD(Dao_Sync_Sharedmemory, size);
PHP_METHOD(Dao_Sync_Sharedmemory, read);
PHP_METHOD(Dao_Sync_Sharedmemory, write);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_sync_sharedmemory___construct, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, size, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_sync_sharedmemory_read, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, start, IS_LONG, 1)
	ZEND_ARG_TYPE_INFO(0, length, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_sync_sharedmemory_write, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, string, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, start, IS_LONG, 1)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_sync_sharedmemory_method_entry[] = {
	PHP_ME(Dao_Sync_Sharedmemory, __construct, arginfo_dao_sync_sharedmemory___construct, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Sync_Sharedmemory, first, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Sync_Sharedmemory, size, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Sync_Sharedmemory, read, arginfo_dao_sync_sharedmemory_read, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Sync_Sharedmemory, write, arginfo_dao_sync_sharedmemory_write, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

zend_object_handlers dao_sync_sharedmemory_object_handlers;
zend_object* dao_sync_sharedmemory_object_create_handler(zend_class_entry *ce)
{
	dao_sync_sharedmemory_object *intern = ecalloc(1, sizeof(dao_sync_sharedmemory_object) + zend_object_properties_size(ce));
	intern->std.ce = ce;

	zend_object_std_init(&intern->std, ce);
	object_properties_init(&intern->std, ce);
	intern->std.handlers = &dao_sync_sharedmemory_object_handlers;

	intern->MxMemInternal = NULL;
	intern->MxFirst = 0;
	intern->MxSize = 0;
	intern->MxMem = NULL;

	return &intern->std;
}

void dao_sync_sharedmemory_object_free_handler(zend_object *object)
{
	dao_sync_sharedmemory_object *intern = dao_sync_sharedmemory_object_from_obj(object);

	if (intern->MxMemInternal != NULL) dao_namedmem_unmap(intern->MxMemInternal, intern->MxSize);
}

/**
 * Dao\Sync\Sharedmemory initializer
 */
DAO_INIT_CLASS(Dao_Sync_Sharedmemory){

	DAO_REGISTER_CLASS_CREATE_OBJECT(Dao\\Sync, Sharedmemory, sync_sharedmemory, dao_sync_sharedmemory_method_entry, 0);

	return SUCCESS;
}

/**
 * Dao\Sync\Sharedmemory constructor
 *
 * @param string $name
 * @param int $size
 */
PHP_METHOD(Dao_Sync_Sharedmemory, __construct){

	zval *name, *size;
	size_t pos;
	dao_sync_sharedmemory_object *intern;
	int result;

	dao_fetch_params(0, 2, 0, &name, &size);

	if (DAO_IS_EMPTY(name)) {
		DAO_THROW_EXCEPTION_STR(dao_sync_exception_ce, "An invalid name was passed");
		return;
	}

	intern = dao_sync_sharedmemory_object_from_obj(Z_OBJ_P(getThis()));

	result = dao_namedmem_init(&intern->MxMemInternal, &pos, "/Sync_SharedMem", Z_STRVAL_P(name), (size_t)Z_LVAL_P(size));

	if (result < 0) {
		DAO_THROW_EXCEPTION_STR(dao_sync_exception_ce, "Shared memory object could not be created/opened");
		return;
	}

	/* Load the pointers. */
	intern->MxMem = intern->MxMemInternal + pos;
	intern->MxSize = (size_t)Z_LVAL_P(size);

	/* Handle the first time this named memory has been opened. */
	if (result == 0) {
		dao_namedmem_ready(intern->MxMemInternal);
		intern->MxFirst = 1;
	}
}

/**
 * Returns whether or not this shared memory segment is the first time accessed (i.e. not initialized)
 *
 * @return boolean
 */
PHP_METHOD(Dao_Sync_Sharedmemory, first){

	dao_sync_sharedmemory_object *intern;

	intern = dao_sync_sharedmemory_object_from_obj(Z_OBJ_P(getThis()));

	RETURN_BOOL(intern->MxFirst);
}

PHP_METHOD(Dao_Sync_Sharedmemory, size){

	dao_sync_sharedmemory_object *intern;

	intern = dao_sync_sharedmemory_object_from_obj(Z_OBJ_P(getThis()));

	RETURN_LONG(intern->MxSize);
}

/**
 * Copies data from shared memory
 *
 * @param int $start
 * @param int $length
 * @return string
 */
PHP_METHOD(Dao_Sync_Sharedmemory, read){

	zval *_start = NULL, *_length = NULL;
	zend_long start = 0, length;
	dao_sync_sharedmemory_object *intern;

	dao_fetch_params(0, 0, 2, &_start, &_length);

	intern = dao_sync_sharedmemory_object_from_obj(Z_OBJ_P(getThis()));

	if (_start && Z_TYPE_P(_start) == IS_LONG) {
		start = Z_LVAL_P(_start);
	}

	if (_length && Z_TYPE_P(_length) == IS_LONG) {
		length = Z_LVAL_P(_length);
	} else {
		length = intern->MxSize;
	}

	if (start < 0) {
		start += intern->MxSize;
		if (start < 0) {
			start = 0;
		}
	} else if (start > intern->MxSize)  {
		start = intern->MxSize;
	}

	if (length < 0) {
		length += intern->MxSize - start;
		if (length < 0) {
			length = 0;
		}
	}

	if (start + length > intern->MxSize) {
		length = intern->MxSize - start;
	}

	RETURN_STRINGL(intern->MxMem + start, length);
}

/**
 * Copies data to shared memory
 *
 * @param string $str
 * @param int $start
 * @return int
 */
PHP_METHOD(Dao_Sync_Sharedmemory, write){

	zval *str, *_start = NULL;
	zend_long start = 0, length;
	dao_sync_sharedmemory_object *intern;

	dao_fetch_params(0, 1, 1, &str, &_start);

	intern = dao_sync_sharedmemory_object_from_obj(Z_OBJ_P(getThis()));

	length = Z_STRLEN_P(str);

	if (_start && Z_TYPE_P(_start) == IS_LONG) {
		start = Z_LVAL_P(_start);
	}

	if (start < 0) {
		start += intern->MxSize;
		if (start < 0) {
			start = 0;
		}
	} else if (start > intern->MxSize)  {
		start = intern->MxSize;
	}

	if (start + length > intern->MxSize) {
		length = intern->MxSize - start;
	}

	memcpy(intern->MxMem + (size_t)start, Z_STRVAL_P(str), length);

	RETURN_LONG(length);
}
