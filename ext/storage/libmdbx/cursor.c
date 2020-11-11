
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

#include "storage/libmdbx/cursor.h"
#include "storage/exception.h"

#include "kernel/main.h"
#include "kernel/exception.h"
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "kernel/file.h"
#include "kernel/variables.h"

/**
 * Dao\Storage\Libmdbx\Cursor
 *
 */
zend_class_entry *dao_storage_libmdbx_cursor_ce;

PHP_METHOD(Dao_Storage_Libmdbx_Cursor, __construct);
PHP_METHOD(Dao_Storage_Libmdbx_Cursor, retrieve);
PHP_METHOD(Dao_Storage_Libmdbx_Cursor, count);
PHP_METHOD(Dao_Storage_Libmdbx_Cursor, get);
PHP_METHOD(Dao_Storage_Libmdbx_Cursor, put);
PHP_METHOD(Dao_Storage_Libmdbx_Cursor, del);
PHP_METHOD(Dao_Storage_Libmdbx_Cursor, current);
PHP_METHOD(Dao_Storage_Libmdbx_Cursor, key);
PHP_METHOD(Dao_Storage_Libmdbx_Cursor, next);
PHP_METHOD(Dao_Storage_Libmdbx_Cursor, prev);
PHP_METHOD(Dao_Storage_Libmdbx_Cursor, rewind);
PHP_METHOD(Dao_Storage_Libmdbx_Cursor, first);
PHP_METHOD(Dao_Storage_Libmdbx_Cursor, last);
PHP_METHOD(Dao_Storage_Libmdbx_Cursor, valid);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_libmdbx_cursor_retrieve, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_libmdbx_cursor_get, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, flags, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_libmdbx_cursor_put, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
	ZEND_ARG_INFO(0, value)
	ZEND_ARG_TYPE_INFO(0, flags, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_libmdbx_cursor_del, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, flags, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_libmdbx_cursor_next, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, flag, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_libmdbx_cursor_prev, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, flag, IS_LONG, 1)
ZEND_END_ARG_INFO()

// arginfo_iterator_rewind
ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_libmdbx_cursor_first, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, dup, _IS_BOOL, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_libmdbx_cursor_last, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, dup, _IS_BOOL, 1)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_storage_libmdbx_cursor_method_entry[] = {
	PHP_ME(Dao_Storage_Libmdbx_Cursor, __construct, arginfo_empty, ZEND_ACC_PRIVATE|ZEND_ACC_CTOR|ZEND_ACC_FINAL)
	PHP_ME(Dao_Storage_Libmdbx_Cursor, retrieve, arginfo_dao_storage_libmdbx_cursor_retrieve, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Libmdbx_Cursor, get, arginfo_dao_storage_libmdbx_cursor_get, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Libmdbx_Cursor, put, arginfo_dao_storage_libmdbx_cursor_put, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Libmdbx_Cursor, del, arginfo_dao_storage_libmdbx_cursor_del, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Libmdbx_Cursor, count, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Libmdbx_Cursor, current, arginfo_iterator_current, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Libmdbx_Cursor, key, arginfo_iterator_key, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Libmdbx_Cursor, next, arginfo_dao_storage_libmdbx_cursor_next, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Libmdbx_Cursor, prev, arginfo_dao_storage_libmdbx_cursor_first, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Libmdbx_Cursor, rewind, arginfo_dao_storage_libmdbx_cursor_first, ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Storage_Libmdbx_Cursor, first, rewind, arginfo_dao_storage_libmdbx_cursor_first, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Libmdbx_Cursor, last, arginfo_dao_storage_libmdbx_cursor_last, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Libmdbx_Cursor, valid, arginfo_iterator_valid, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

zend_object_handlers dao_storage_libmdbx_cursor_object_handlers;
zend_object* dao_storage_libmdbx_cursor_object_create_handler(zend_class_entry *ce)
{
	dao_storage_libmdbx_cursor_object *intern = ecalloc(1, sizeof(dao_storage_libmdbx_cursor_object) + zend_object_properties_size(ce));
	intern->std.ce = ce;

	zend_object_std_init(&intern->std, ce);
	object_properties_init(&intern->std, ce);
	intern->std.handlers = &dao_storage_libmdbx_cursor_object_handlers;

	return &intern->std;
}

void dao_storage_libmdbx_cursor_object_free_handler(zend_object *object)
{
	dao_storage_libmdbx_cursor_object *intern = dao_storage_libmdbx_cursor_object_from_obj(object);
	if (intern->cursor) {
		mdbx_cursor_close(intern->cursor);
	}

	zend_object_std_dtor(&intern->std);
}

/**
 * Dao\Storage\Libmdbx\Cursor initializer
 */
DAO_INIT_CLASS(Dao_Storage_Libmdbx_Cursor){

	DAO_REGISTER_CLASS_CREATE_OBJECT(Dao\\Storage\\Libmdbx, Cursor, storage_libmdbx_cursor, dao_storage_libmdbx_cursor_method_entry, 0);

	zend_declare_property_null(dao_storage_libmdbx_cursor_ce, SL("_frontend"), ZEND_ACC_PROTECTED);

	zend_declare_class_constant_long(dao_storage_libmdbx_cursor_ce, SL("GET_BOTH"),		MDBX_GET_BOTH);
	zend_declare_class_constant_long(dao_storage_libmdbx_cursor_ce, SL("GET_BOTH_RANGE"),	MDBX_GET_BOTH_RANGE);
	zend_declare_class_constant_long(dao_storage_libmdbx_cursor_ce, SL("GET_CURRENT"),		MDBX_GET_CURRENT);
	zend_declare_class_constant_long(dao_storage_libmdbx_cursor_ce, SL("GET_MULTIPLE"),	MDBX_GET_MULTIPLE);

	zend_declare_class_constant_long(dao_storage_libmdbx_cursor_ce, SL("CURRENT"),			MDBX_CURRENT);
	zend_declare_class_constant_long(dao_storage_libmdbx_cursor_ce, SL("NODUPDATA"),		MDBX_NODUPDATA);
	zend_declare_class_constant_long(dao_storage_libmdbx_cursor_ce, SL("NOOVERWRITE"),		MDBX_NOOVERWRITE);
	zend_declare_class_constant_long(dao_storage_libmdbx_cursor_ce, SL("RESERVE"),			MDBX_RESERVE);
	zend_declare_class_constant_long(dao_storage_libmdbx_cursor_ce, SL("APPEND"),			MDBX_APPEND);
	zend_declare_class_constant_long(dao_storage_libmdbx_cursor_ce, SL("APPENDDUP"),		MDBX_APPENDDUP);
	zend_declare_class_constant_long(dao_storage_libmdbx_cursor_ce, SL("MULTIPLE"),		MDBX_MULTIPLE);

	zend_declare_class_constant_long(dao_storage_libmdbx_cursor_ce, SL("NEXT"),			MDBX_NEXT);
	zend_declare_class_constant_long(dao_storage_libmdbx_cursor_ce, SL("NEXT_DUP"),		MDBX_NEXT_DUP);
	zend_declare_class_constant_long(dao_storage_libmdbx_cursor_ce, SL("NEXT_MULTIPLE"),	MDBX_NEXT_MULTIPLE);
	zend_declare_class_constant_long(dao_storage_libmdbx_cursor_ce, SL("NEXT_NODUP"),		MDBX_NEXT_NODUP);
	zend_declare_class_constant_long(dao_storage_libmdbx_cursor_ce, SL("PREV"),			MDBX_PREV);
	zend_declare_class_constant_long(dao_storage_libmdbx_cursor_ce, SL("PREV_DUP"),		MDBX_PREV_DUP);
	zend_declare_class_constant_long(dao_storage_libmdbx_cursor_ce, SL("PREV_MULTIPLE"),	MDBX_PREV_MULTIPLE);
	zend_declare_class_constant_long(dao_storage_libmdbx_cursor_ce, SL("PREV_NODUP"),		MDBX_PREV_NODUP);

	zend_class_implements(dao_storage_libmdbx_cursor_ce, 1, zend_ce_iterator);

	return SUCCESS;
}

/**
 * Dao\Storage\Lmdb\Cursor constructor
 *
 */
PHP_METHOD(Dao_Storage_Libmdbx_Cursor, __construct)
{
	/* this constructor shouldn't be called as it's private */
	zend_throw_exception(NULL, "An object of this type cannot be created with the new operator.", 0);
}

/**
 * Retrieve by cursor
 *
 * @param string $key
 * @return string
 */
PHP_METHOD(Dao_Storage_Libmdbx_Cursor, retrieve)
{
	zval *key;
	MDBX_val k;
	MDBX_val v;
	dao_storage_libmdbx_cursor_object *intern;
	int rc;

	dao_fetch_params(0, 1, 0, &key);

	intern = dao_storage_libmdbx_cursor_object_from_obj(Z_OBJ_P(getThis()));
	k.iov_len = Z_STRLEN_P(key);
	k.iov_base = Z_STRVAL_P(key);

	rc = mdbx_cursor_get(intern->cursor, &k, &v, MDBX_SET);

	if (rc == MDBX_SUCCESS) {
		RETVAL_TRUE;
	} else if (rc == MDBX_NOTFOUND) {
		RETVAL_FALSE;
	} else {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Failed to retrieve by cursor (%s)", mdbx_strerror(rc));
		return;
	}
}

/**
 * Return key/data at current cursor position
 *
 * @param string $key
 * @param int $flags
 * @return mixed
 */
PHP_METHOD(Dao_Storage_Libmdbx_Cursor, get)
{
	zval *key, *_flags = NULL, frontend = {}, value = {};
	MDBX_val k;
	MDBX_val v;
	dao_storage_libmdbx_cursor_object *intern;
	int flags = MDBX_GET_CURRENT, rc;

	dao_fetch_params(0, 1, 1, &key, &_flags);

	if (_flags && Z_TYPE_P(_flags) == IS_LONG) {
		switch (Z_LVAL_P(_flags)) {
		case MDBX_GET_BOTH:
		case MDBX_GET_BOTH_RANGE:
		case MDBX_GET_CURRENT:
		case MDBX_GET_MULTIPLE:
			flags = Z_LVAL_P(_flags);
			break;
		default:
			break;
		}
	}

	intern = dao_storage_libmdbx_cursor_object_from_obj(Z_OBJ_P(getThis()));
	k.iov_len = Z_STRLEN_P(key);
	k.iov_base = Z_STRVAL_P(key);

	rc = mdbx_cursor_get(intern->cursor, &k, &v, flags);
	if (rc != MDBX_SUCCESS) {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Failed to store by cursor (%s)", mdbx_strerror(rc));
		return;
	}

	dao_read_property(&frontend, getThis(), SL("_frontend"), PH_NOISY|PH_READONLY);
	
	ZVAL_STRINGL(&value, (char *) v.iov_base, (int) v.iov_len);

	if (Z_TYPE(frontend) == IS_OBJECT) {
		DAO_CALL_METHOD_FLAG(rc, return_value, &frontend, "afterretrieve", &value);
	} else {
		dao_unserialize(return_value, &value);
	}

	zval_ptr_dtor(&value);
}

/**
 * Store by cursor
 *
 * @param string $key
 * @param mixed $value
 * @param int $flags
 * @return mixed
 */
PHP_METHOD(Dao_Storage_Libmdbx_Cursor, put)
{
	zval *key, *value, *_flags = NULL, frontend = {}, s = {};
	MDBX_val k;
	MDBX_val v;
	dao_storage_libmdbx_cursor_object *intern;
	int flags = 0, rc;

	dao_fetch_params(0, 2, 1, &key, &value, &_flags);

	if (_flags && Z_TYPE_P(_flags) == IS_LONG) {
		flags = Z_LVAL_P(_flags);
	}

	intern = dao_storage_libmdbx_cursor_object_from_obj(Z_OBJ_P(getThis()));

	if (intern->flags & MDBX_DUPSORT) {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Failed to store by cursor (%s)", mdbx_strerror(MDBX_INCOMPATIBLE));
		return;
	}

	dao_read_property(&frontend, getThis(), SL("_frontend"), PH_NOISY|PH_READONLY);
	if (Z_TYPE(frontend) == IS_OBJECT) {
		DAO_CALL_METHOD_FLAG(rc, &s, &frontend, "beforestore", value);
	} else {
		dao_serialize(&s, value);
	}
	k.iov_len = Z_STRLEN_P(key);
	k.iov_base = Z_STRVAL_P(key);
	v.iov_len = Z_STRLEN(s);
	v.iov_base = Z_STRVAL(s);

	rc = mdbx_cursor_put(intern->cursor, &k, &v, flags);
	zval_ptr_dtor(&s);
	if (rc != MDBX_SUCCESS) {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Failed to store by cursor (%s)", mdbx_strerror(rc));
		return;
	}

	RETURN_TRUE;
}

/**
 * Delete current key/data pair
 *
 * @param int $flags
 * @return boolean
 */
PHP_METHOD(Dao_Storage_Libmdbx_Cursor, del)
{
	zval *_flags = NULL;
	dao_storage_libmdbx_cursor_object *intern;
	int flags = 0, rc;

	dao_fetch_params(0, 0, 1, &_flags);

	if (_flags && Z_TYPE_P(_flags) == IS_LONG) {
		flags = Z_LVAL_P(_flags);
	}

	intern = dao_storage_libmdbx_cursor_object_from_obj(Z_OBJ_P(getThis()));

	rc = mdbx_cursor_del(intern->cursor, flags);

	if (rc != MDBX_SUCCESS) {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Failed to delete current key/data pair (%s)", mdbx_strerror(rc));
		return;
	}

	RETURN_TRUE;
}

/**
 * Return count of duplicates for current key
 *
 * @return int
 */
PHP_METHOD(Dao_Storage_Libmdbx_Cursor, count)
{
	dao_storage_libmdbx_cursor_object *intern;
	size_t out = 0;
	int rc;

	intern = dao_storage_libmdbx_cursor_object_from_obj(Z_OBJ_P(getThis()));

	rc = mdbx_cursor_count(intern->cursor, &out);
	if (rc == MDBX_SUCCESS) {
		RETURN_LONG((zend_long)out);
	} else if (rc == MDBX_NOTFOUND) {
		RETURN_LONG(0);
	} else {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Failed to count of duplicates for current key (%s)", mdbx_strerror(rc));
		return;
	}
}

/**
 * Gets current value
 *
 * @return string
 */
PHP_METHOD(Dao_Storage_Libmdbx_Cursor, current)
{
	dao_storage_libmdbx_cursor_object *intern;

	intern = dao_storage_libmdbx_cursor_object_from_obj(Z_OBJ_P(getThis()));

	if (!intern->start) {
		intern->rc = mdbx_cursor_get(intern->cursor, &intern->k, &intern->v, MDBX_NEXT);
		intern->start = 1;
	}

	if (intern->rc == MDBX_SUCCESS) {
		zval frontend = {}, s = {};
		ZVAL_STRINGL(&s, (char *) intern->v.iov_base, (int) intern->v.iov_len);

		dao_read_property(&frontend, getThis(), SL("_frontend"), PH_NOISY|PH_READONLY);
		if (Z_TYPE(frontend) == IS_OBJECT) {
			DAO_CALL_METHOD_FLAG(intern->rc, return_value, &frontend, "afterretrieve", &s);
		} else {
			dao_unserialize(return_value, &s);
		}
		zval_ptr_dtor(&s);
	} else if (intern->rc == MDBX_NOTFOUND) {
		RETVAL_FALSE;
	} else {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Failed to get item from a database (%s)", mdbx_strerror(intern->rc));
		return;
	}
}

/**
 * Gets current key
 *
 * @return string
 */
PHP_METHOD(Dao_Storage_Libmdbx_Cursor, key)
{
	dao_storage_libmdbx_cursor_object *intern;

	intern = dao_storage_libmdbx_cursor_object_from_obj(Z_OBJ_P(getThis()));

	if (!intern->start) {
		intern->rc = mdbx_cursor_get(intern->cursor, &intern->k, &intern->v, MDBX_NEXT);
		intern->start = 1;
	}

	if (intern->rc == MDBX_SUCCESS) {
		ZVAL_STRINGL(return_value, (char *) intern->k.iov_base, (int) intern->k.iov_len);
	} else if (intern->rc == MDBX_NOTFOUND) {
		RETVAL_FALSE;
	} else {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Failed to get key from a database (%s)", mdbx_strerror(intern->rc));
		return;
	}
}

/**
 * Moves cursor to next row
 *
 * @param int $flag
 * @return boolean
 */
PHP_METHOD(Dao_Storage_Libmdbx_Cursor, next)
{
	zval *_flag = NULL;
	dao_storage_libmdbx_cursor_object *intern;
	int flag = MDBX_NEXT;

	dao_fetch_params(0, 0, 1, &_flag);
	intern = dao_storage_libmdbx_cursor_object_from_obj(Z_OBJ_P(getThis()));
	if (_flag && Z_TYPE_P(_flag) == IS_LONG) {
		switch (Z_LVAL_P(_flag)) {
		case MDBX_NEXT_DUP:
			flag = MDBX_NEXT_DUP;
			break;
		case MDBX_NEXT_MULTIPLE:
			flag = MDBX_NEXT_MULTIPLE;
			break;
		case MDBX_NEXT_NODUP:
			flag = MDBX_NEXT_NODUP;
			break;
		default:
			flag = MDBX_NEXT;
			break;
		}
	}
    intern->rc = mdbx_cursor_get(intern->cursor, &intern->k, &intern->v, flag);
	intern->start = 1;
	if (intern->rc == MDBX_SUCCESS) {
		RETURN_TRUE;
	} else if (intern->rc == MDBX_NOTFOUND) {
		RETVAL_FALSE;
	} else {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Failed to move next (%s)", mdbx_strerror(intern->rc));
		return;
	}
}

/**
 * Moves cursor to prev row
 *
 * @param int $flag
 * @return boolean
 */
PHP_METHOD(Dao_Storage_Libmdbx_Cursor, prev)
{
	zval *_flag = NULL;
	dao_storage_libmdbx_cursor_object *intern;
	int flag = MDBX_PREV;

	dao_fetch_params(0, 0, 1, &_flag);
	intern = dao_storage_libmdbx_cursor_object_from_obj(Z_OBJ_P(getThis()));
	if (_flag && Z_TYPE_P(_flag) == IS_LONG) {
		switch (Z_LVAL_P(_flag)) {
		case MDBX_PREV_DUP:
			flag = MDBX_PREV_DUP;
			break;
		case MDBX_PREV_MULTIPLE:
			flag = MDBX_PREV_MULTIPLE;
			break;
		case MDBX_PREV_NODUP:
			flag = MDBX_PREV_NODUP;
			break;
		default:
			flag = MDBX_PREV;
			break;
		}
	}
    intern->rc = mdbx_cursor_get(intern->cursor, &intern->k, &intern->v, flag);
	intern->start = 1;
	if (intern->rc == MDBX_SUCCESS) {
		RETURN_TRUE;
	} else if (intern->rc == MDBX_NOTFOUND) {
		RETVAL_FALSE;
	} else {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Failed to move prev (%s)", mdbx_strerror(intern->rc));
		return;
	}
}

/**
 * Rewinds cursor to it's beginning
 *
 * @param boolean $dup
 * @return boolean
 */
PHP_METHOD(Dao_Storage_Libmdbx_Cursor, rewind)
{
	zval *dup = NULL;
	dao_storage_libmdbx_cursor_object *intern;

	dao_fetch_params(0, 0, 1, &dup);
	intern = dao_storage_libmdbx_cursor_object_from_obj(Z_OBJ_P(getThis()));
	if (dup && zend_is_true(dup)) {
		intern->rc = mdbx_cursor_get(intern->cursor, &intern->k, &intern->v, MDBX_FIRST_DUP);
	} else {
    	intern->rc = mdbx_cursor_get(intern->cursor, &intern->k, &intern->v, MDBX_FIRST);
	}
	intern->start = 1;
	if (intern->rc == MDBX_SUCCESS) {
		RETURN_TRUE;
	} else if (intern->rc == MDBX_NOTFOUND) {
		RETVAL_FALSE;
	} else {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Failed to rewind (%s)", mdbx_strerror(intern->rc));
		return;
	}
}

/**
 * Rewinds cursor to it's last
 *
 * @param boolean $dup
 * @return boolean
 */
PHP_METHOD(Dao_Storage_Libmdbx_Cursor, last)
{
	zval *dup = NULL;
	dao_storage_libmdbx_cursor_object *intern;

	dao_fetch_params(0, 0, 1, &dup);
	intern = dao_storage_libmdbx_cursor_object_from_obj(Z_OBJ_P(getThis()));
	if (dup && zend_is_true(dup)) {
		intern->rc = mdbx_cursor_get(intern->cursor, &intern->k, &intern->v, MDBX_LAST_DUP);
	} else {
    	intern->rc = mdbx_cursor_get(intern->cursor, &intern->k, &intern->v, MDBX_LAST);
	}
	intern->start = 1;
	if (intern->rc == MDBX_SUCCESS) {
		RETURN_TRUE;
	} else if (intern->rc == MDBX_NOTFOUND) {
		RETVAL_FALSE;
	} else {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Failed to move last (%s)", mdbx_strerror(intern->rc));
		return;
	}
}

/**
 * Checks if current position is valid
 *
 * @return boolean
 */
PHP_METHOD(Dao_Storage_Libmdbx_Cursor, valid)
{
	dao_storage_libmdbx_cursor_object *intern;

	intern = dao_storage_libmdbx_cursor_object_from_obj(Z_OBJ_P(getThis()));

	if (!intern->start) {
		intern->rc = mdbx_cursor_get(intern->cursor, &intern->k, &intern->v, MDBX_NEXT);
		intern->start = 1;
	}
	if (intern->rc == MDBX_SUCCESS) {
		RETVAL_TRUE;
	} else if (intern->rc == MDBX_NOTFOUND) {
		RETVAL_FALSE;
	} else {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Failed to valid (%s)", mdbx_strerror(intern->rc));
		return;
	}
}
