
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

#include "storage/lmdb/cursor.h"
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
 * Dao\Storage\Lmdb\Cursor
 *
 */
zend_class_entry *dao_storage_lmdb_cursor_ce;

PHP_METHOD(Dao_Storage_Lmdb_Cursor, __construct);
PHP_METHOD(Dao_Storage_Lmdb_Cursor, retrieve);
PHP_METHOD(Dao_Storage_Lmdb_Cursor, count);
PHP_METHOD(Dao_Storage_Lmdb_Cursor, get);
PHP_METHOD(Dao_Storage_Lmdb_Cursor, put);
PHP_METHOD(Dao_Storage_Lmdb_Cursor, del);
PHP_METHOD(Dao_Storage_Lmdb_Cursor, current);
PHP_METHOD(Dao_Storage_Lmdb_Cursor, key);
PHP_METHOD(Dao_Storage_Lmdb_Cursor, next);
PHP_METHOD(Dao_Storage_Lmdb_Cursor, prev);
PHP_METHOD(Dao_Storage_Lmdb_Cursor, rewind);
PHP_METHOD(Dao_Storage_Lmdb_Cursor, first);
PHP_METHOD(Dao_Storage_Lmdb_Cursor, last);
PHP_METHOD(Dao_Storage_Lmdb_Cursor, valid);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_lmdb_cursor_retrieve, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_lmdb_cursor_get, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, flags, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_lmdb_cursor_put, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
	ZEND_ARG_INFO(0, value)
	ZEND_ARG_TYPE_INFO(0, flags, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_lmdb_cursor_del, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, flags, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_lmdb_cursor_next, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, flag, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_lmdb_cursor_prev, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, flag, IS_LONG, 1)
ZEND_END_ARG_INFO()

// arginfo_iterator_rewind
ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_lmdb_cursor_first, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, dup, _IS_BOOL, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_lmdb_cursor_last, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, dup, _IS_BOOL, 1)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_storage_lmdb_cursor_method_entry[] = {
	PHP_ME(Dao_Storage_Lmdb_Cursor, __construct, arginfo_empty, ZEND_ACC_PRIVATE|ZEND_ACC_CTOR|ZEND_ACC_FINAL)
	PHP_ME(Dao_Storage_Lmdb_Cursor, retrieve, arginfo_dao_storage_lmdb_cursor_retrieve, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Lmdb_Cursor, get, arginfo_dao_storage_lmdb_cursor_get, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Lmdb_Cursor, put, arginfo_dao_storage_lmdb_cursor_put, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Lmdb_Cursor, del, arginfo_dao_storage_lmdb_cursor_del, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Lmdb_Cursor, count, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Lmdb_Cursor, current, arginfo_iterator_current, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Lmdb_Cursor, key, arginfo_iterator_key, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Lmdb_Cursor, next, arginfo_dao_storage_lmdb_cursor_next, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Lmdb_Cursor, prev, arginfo_dao_storage_lmdb_cursor_prev, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Lmdb_Cursor, rewind, arginfo_dao_storage_lmdb_cursor_first, ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Storage_Lmdb_Cursor, first, rewind, arginfo_dao_storage_lmdb_cursor_first, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Lmdb_Cursor, last, arginfo_dao_storage_lmdb_cursor_last, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Lmdb_Cursor, valid, arginfo_iterator_valid, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

zend_object_handlers dao_storage_lmdb_cursor_object_handlers;
zend_object* dao_storage_lmdb_cursor_object_create_handler(zend_class_entry *ce)
{
	dao_storage_lmdb_cursor_object *intern = ecalloc(1, sizeof(dao_storage_lmdb_cursor_object) + zend_object_properties_size(ce));
	intern->std.ce = ce;

	zend_object_std_init(&intern->std, ce);
	object_properties_init(&intern->std, ce);
	intern->std.handlers = &dao_storage_lmdb_cursor_object_handlers;

	return &intern->std;
}

void dao_storage_lmdb_cursor_object_free_handler(zend_object *object)
{
	dao_storage_lmdb_cursor_object *intern = dao_storage_lmdb_cursor_object_from_obj(object);
	if (intern->cursor) {
		mdb_cursor_close(intern->cursor);
	}

	zend_object_std_dtor(&intern->std);
}

/**
 * Dao\Storage\Lmdb\Cursor initializer
 */
DAO_INIT_CLASS(Dao_Storage_Lmdb_Cursor){

	DAO_REGISTER_CLASS_CREATE_OBJECT(Dao\\Storage\\Lmdb, Cursor, storage_lmdb_cursor, dao_storage_lmdb_cursor_method_entry, 0);

	zend_declare_property_null(dao_storage_lmdb_cursor_ce, SL("_frontend"), ZEND_ACC_PROTECTED);

	zend_declare_class_constant_long(dao_storage_lmdb_cursor_ce, SL("GET_BOTH"),		MDB_GET_BOTH);
	zend_declare_class_constant_long(dao_storage_lmdb_cursor_ce, SL("GET_BOTH_RANGE"),	MDB_GET_BOTH_RANGE);
	zend_declare_class_constant_long(dao_storage_lmdb_cursor_ce, SL("GET_CURRENT"),		MDB_GET_CURRENT);
	zend_declare_class_constant_long(dao_storage_lmdb_cursor_ce, SL("GET_MULTIPLE"),	MDB_GET_MULTIPLE);

	zend_declare_class_constant_long(dao_storage_lmdb_cursor_ce, SL("CURRENT"),			MDB_CURRENT);
	zend_declare_class_constant_long(dao_storage_lmdb_cursor_ce, SL("NODUPDATA"),		MDB_NODUPDATA);
	zend_declare_class_constant_long(dao_storage_lmdb_cursor_ce, SL("NOOVERWRITE"),		MDB_NOOVERWRITE);
	zend_declare_class_constant_long(dao_storage_lmdb_cursor_ce, SL("RESERVE"),			MDB_RESERVE);
	zend_declare_class_constant_long(dao_storage_lmdb_cursor_ce, SL("APPEND"),			MDB_APPEND);
	zend_declare_class_constant_long(dao_storage_lmdb_cursor_ce, SL("APPENDDUP"),		MDB_APPENDDUP);
	zend_declare_class_constant_long(dao_storage_lmdb_cursor_ce, SL("MULTIPLE"),		MDB_MULTIPLE);

	zend_declare_class_constant_long(dao_storage_lmdb_cursor_ce, SL("NEXT"),			MDB_NEXT);
	zend_declare_class_constant_long(dao_storage_lmdb_cursor_ce, SL("NEXT_DUP"),		MDB_NEXT_DUP);
	zend_declare_class_constant_long(dao_storage_lmdb_cursor_ce, SL("NEXT_MULTIPLE"),	MDB_NEXT_MULTIPLE);
	zend_declare_class_constant_long(dao_storage_lmdb_cursor_ce, SL("NEXT_NODUP"),		MDB_NEXT_NODUP);
	zend_declare_class_constant_long(dao_storage_lmdb_cursor_ce, SL("PREV"),			MDB_PREV);
	zend_declare_class_constant_long(dao_storage_lmdb_cursor_ce, SL("PREV_DUP"),		MDB_PREV_DUP);
	zend_declare_class_constant_long(dao_storage_lmdb_cursor_ce, SL("PREV_MULTIPLE"),	MDB_PREV_MULTIPLE);
	zend_declare_class_constant_long(dao_storage_lmdb_cursor_ce, SL("PREV_NODUP"),		MDB_PREV_NODUP);

	zend_class_implements(dao_storage_lmdb_cursor_ce, 1, zend_ce_iterator);

	return SUCCESS;
}

/**
 * Dao\Storage\Lmdb\Cursor constructor
 *
 */
PHP_METHOD(Dao_Storage_Lmdb_Cursor, __construct)
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
PHP_METHOD(Dao_Storage_Lmdb_Cursor, retrieve)
{
	zval *key;
	MDB_val k;
	MDB_val v;
	dao_storage_lmdb_cursor_object *intern;
	int rc;

	dao_fetch_params(0, 1, 0, &key);

	intern = dao_storage_lmdb_cursor_object_from_obj(Z_OBJ_P(getThis()));
	k.mv_size = Z_STRLEN_P(key);
	k.mv_data = Z_STRVAL_P(key);

	rc = mdb_cursor_get(intern->cursor, &k, &v, MDB_SET);

	if (rc == MDB_SUCCESS) {
		RETVAL_TRUE;
	} else if (rc == MDB_NOTFOUND) {
		RETVAL_FALSE;
	} else {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Failed to retrieve by cursor (%s)", mdb_strerror(rc));
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
PHP_METHOD(Dao_Storage_Lmdb_Cursor, get)
{
	zval *key, *_flags = NULL, frontend = {}, value = {};
	MDB_val k;
	MDB_val v;
	dao_storage_lmdb_cursor_object *intern;
	int flags = MDB_GET_CURRENT, rc;

	dao_fetch_params(0, 1, 1, &key, &_flags);

	if (_flags && Z_TYPE_P(_flags) == IS_LONG) {
		switch (Z_LVAL_P(_flags)) {
		case MDB_GET_BOTH:
		case MDB_GET_BOTH_RANGE:
		case MDB_GET_CURRENT:
		case MDB_GET_MULTIPLE:
			flags = Z_LVAL_P(_flags);
			break;
		default:
			break;
		}
	}

	intern = dao_storage_lmdb_cursor_object_from_obj(Z_OBJ_P(getThis()));
	k.mv_size = Z_STRLEN_P(key);
	k.mv_data = Z_STRVAL_P(key);

	rc = mdb_cursor_get(intern->cursor, &k, &v, flags);
	if (rc != MDB_SUCCESS) {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Failed to store by cursor (%s)", mdb_strerror(rc));
		return;
	}

	dao_read_property(&frontend, getThis(), SL("_frontend"), PH_NOISY|PH_READONLY);
	
	ZVAL_STRINGL(&value, (char *) v.mv_data, (int) v.mv_size);

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
PHP_METHOD(Dao_Storage_Lmdb_Cursor, put)
{
	zval *key, *value, *_flags = NULL, frontend = {}, s = {};
	MDB_val k;
	MDB_val v;
	dao_storage_lmdb_cursor_object *intern;
	int flags = 0, rc;

	dao_fetch_params(0, 2, 1, &key, &value, &_flags);

	if (_flags && Z_TYPE_P(_flags) == IS_LONG) {
		flags = Z_LVAL_P(_flags);
	}

	intern = dao_storage_lmdb_cursor_object_from_obj(Z_OBJ_P(getThis()));

	if (intern->flags & MDB_DUPSORT) {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Failed to store by cursor (%s)", mdb_strerror(MDB_INCOMPATIBLE));
		return;
	}

	dao_read_property(&frontend, getThis(), SL("_frontend"), PH_NOISY|PH_READONLY);
	if (Z_TYPE(frontend) == IS_OBJECT) {
		DAO_CALL_METHOD_FLAG(rc, &s, &frontend, "beforestore", value);
	} else {
		dao_serialize(&s, value);
	}
	k.mv_size = Z_STRLEN_P(key);
	k.mv_data = Z_STRVAL_P(key);
	v.mv_size = Z_STRLEN(s);
	v.mv_data = Z_STRVAL(s);

	rc = mdb_cursor_put(intern->cursor, &k, &v, flags);
	zval_ptr_dtor(&s);
	if (rc != MDB_SUCCESS) {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Failed to store by cursor (%s)", mdb_strerror(rc));
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
PHP_METHOD(Dao_Storage_Lmdb_Cursor, del)
{
	zval *_flags = NULL;
	dao_storage_lmdb_cursor_object *intern;
	int flags = 0, rc;

	dao_fetch_params(0, 0, 1, &_flags);

	if (_flags && Z_TYPE_P(_flags) == IS_LONG) {
		flags = Z_LVAL_P(_flags);
	}

	intern = dao_storage_lmdb_cursor_object_from_obj(Z_OBJ_P(getThis()));

	rc = mdb_cursor_del(intern->cursor, flags);

	if (rc != MDB_SUCCESS) {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Failed to delete current key/data pair (%s)", mdb_strerror(rc));
		return;
	}

	RETURN_TRUE;
}

/**
 * Return count of duplicates for current key
 *
 * @return int
 */
PHP_METHOD(Dao_Storage_Lmdb_Cursor, count)
{
	dao_storage_lmdb_cursor_object *intern;
	mdb_size_t out = 0;
	int rc;

	intern = dao_storage_lmdb_cursor_object_from_obj(Z_OBJ_P(getThis()));

	rc = mdb_cursor_count(intern->cursor, &out);
	if (rc == MDB_SUCCESS) {
		RETURN_LONG((zend_long)out);
	} else if (rc == MDB_NOTFOUND) {
		RETURN_LONG(0);
	} else {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Failed to count of duplicates for current key (%s)", mdb_strerror(rc));
		return;
	}
}

/**
 * Gets current value
 *
 * @return string
 */
PHP_METHOD(Dao_Storage_Lmdb_Cursor, current)
{
	dao_storage_lmdb_cursor_object *intern;

	intern = dao_storage_lmdb_cursor_object_from_obj(Z_OBJ_P(getThis()));

	if (!intern->start) {
		int flag = MDB_NEXT;
		intern->rc = mdb_cursor_get(intern->cursor, &intern->k, &intern->v, flag);
		intern->start = 1;
	}

	if (intern->rc == MDB_SUCCESS) {
		zval frontend = {}, s = {};
		ZVAL_STRINGL(&s, (char *) intern->v.mv_data, (int) intern->v.mv_size);

		dao_read_property(&frontend, getThis(), SL("_frontend"), PH_NOISY|PH_READONLY);
		if (Z_TYPE(frontend) == IS_OBJECT) {
			DAO_CALL_METHOD_FLAG(intern->rc, return_value, &frontend, "afterretrieve", &s);
		} else {
			dao_unserialize(return_value, &s);
		}
		zval_ptr_dtor(&s);
	} else if (intern->rc == MDB_NOTFOUND) {
		RETVAL_FALSE;
	} else {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Failed to get item from a database (%s)", mdb_strerror(intern->rc));
		return;
	}
}

/**
 * Gets current key
 *
 * @return string
 */
PHP_METHOD(Dao_Storage_Lmdb_Cursor, key)
{
	dao_storage_lmdb_cursor_object *intern;

	intern = dao_storage_lmdb_cursor_object_from_obj(Z_OBJ_P(getThis()));

	if (!intern->start) {
		int flag = MDB_NEXT;
		intern->rc = mdb_cursor_get(intern->cursor, &intern->k, &intern->v, flag);
		intern->start = 1;
	}
	if (intern->rc == MDB_SUCCESS) {
		ZVAL_STRINGL(return_value, (char *) intern->k.mv_data, (int) intern->k.mv_size);
	} else if (intern->rc == MDB_NOTFOUND) {
		RETVAL_FALSE;
	} else {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Failed to get key from a database (%s)", mdb_strerror(intern->rc));
		return;
	}
}

/**
 * Moves cursor to next row
 *
 * @param int $flag
 * @return boolean
 */
PHP_METHOD(Dao_Storage_Lmdb_Cursor, next)
{
	zval *_flag = NULL;
	dao_storage_lmdb_cursor_object *intern;
	int flag = MDB_NEXT;

	dao_fetch_params(0, 0, 1, &_flag);

	intern = dao_storage_lmdb_cursor_object_from_obj(Z_OBJ_P(getThis()));
	if (_flag && Z_TYPE_P(_flag) == IS_LONG) {
		switch (Z_LVAL_P(_flag)) {
		case MDB_NEXT_DUP:
			flag = MDB_NEXT_DUP;
			break;
		case MDB_NEXT_MULTIPLE:
			flag = MDB_NEXT_MULTIPLE;
			break;
		case MDB_NEXT_NODUP:
			flag = MDB_NEXT_NODUP;
			break;
		default:
			flag = MDB_NEXT;
			break;
		}
	}
	intern->rc = mdb_cursor_get(intern->cursor, &intern->k, &intern->v, flag);
	intern->start = 1;
	if (intern->rc == MDB_SUCCESS) {
		RETURN_TRUE;
	} else if (intern->rc == MDB_NOTFOUND) {
		RETVAL_FALSE;
	} else {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Failed to move next (%s)", mdb_strerror(intern->rc));
		return;
	}
}

/**
 * Moves cursor to prev row
 *
 * @param int $flag
 * @return boolean
 */
PHP_METHOD(Dao_Storage_Lmdb_Cursor, prev)
{
	zval *_flag = NULL;
	dao_storage_lmdb_cursor_object *intern;
	int flag = MDB_PREV;

	dao_fetch_params(0, 0, 1, &_flag);

	intern = dao_storage_lmdb_cursor_object_from_obj(Z_OBJ_P(getThis()));
	if (_flag && Z_TYPE_P(_flag) == IS_LONG) {
		switch (Z_LVAL_P(_flag)) {
		case MDB_PREV_DUP:
			flag = MDB_PREV_DUP;
			break;
		case MDB_PREV_MULTIPLE:
			flag = MDB_PREV_MULTIPLE;
			break;
		case MDB_PREV_NODUP:
			flag = MDB_PREV_NODUP;
			break;
		default:
			flag = MDB_PREV;
			break;
		}
	}
	intern->rc = mdb_cursor_get(intern->cursor, &intern->k, &intern->v, flag);
	intern->start = 1;
	if (intern->rc == MDB_SUCCESS) {
		RETURN_TRUE;
	} else if (intern->rc == MDB_NOTFOUND) {
		RETVAL_FALSE;
	} else {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Failed to move prev (%s)", mdb_strerror(intern->rc));
		return;
	}
}

/**
 * Rewinds cursor to it's beginning
 *
 * @param boolean $dup
 * @return boolean
 */
PHP_METHOD(Dao_Storage_Lmdb_Cursor, rewind)
{
	zval *dup = NULL;
	dao_storage_lmdb_cursor_object *intern;

	dao_fetch_params(0, 0, 1, &dup);

	intern = dao_storage_lmdb_cursor_object_from_obj(Z_OBJ_P(getThis()));
	if (dup && zend_is_true(dup)) {
		intern->rc = mdb_cursor_get(intern->cursor, &intern->k, &intern->v, MDB_FIRST_DUP);
	} else {
		intern->rc = mdb_cursor_get(intern->cursor, &intern->k, &intern->v, MDB_FIRST);
	}
	intern->start = 1;
	if (intern->rc == MDB_SUCCESS) {
		RETURN_TRUE;
	} else if (intern->rc == MDB_NOTFOUND) {
		RETVAL_FALSE;
	} else {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Failed to rewind (%s)", mdb_strerror(intern->rc));
		return;
	}
}

/**
 * Rewinds cursor to it's last
 *
 * @param boolean $dup
 * @return boolean
 */
PHP_METHOD(Dao_Storage_Lmdb_Cursor, last)
{
	zval *dup = NULL;
	dao_storage_lmdb_cursor_object *intern;

	dao_fetch_params(0, 0, 1, &dup);

	intern = dao_storage_lmdb_cursor_object_from_obj(Z_OBJ_P(getThis()));
	if (dup && zend_is_true(dup)) {
		intern->rc = mdb_cursor_get(intern->cursor, &intern->k, &intern->v, MDB_LAST_DUP);
	} else {
		intern->rc = mdb_cursor_get(intern->cursor, &intern->k, &intern->v, MDB_LAST);
	}
	intern->start = 1;
	if (intern->rc == MDB_SUCCESS) {
		RETURN_TRUE;
	} else if (intern->rc == MDB_NOTFOUND) {
		RETVAL_FALSE;
	} else {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Failed to move last (%s)", mdb_strerror(intern->rc));
		return;
	}
}

/**
 * Checks if current position is valid
 *
 * @return boolean
 */
PHP_METHOD(Dao_Storage_Lmdb_Cursor, valid)
{
	dao_storage_lmdb_cursor_object *intern;

	intern = dao_storage_lmdb_cursor_object_from_obj(Z_OBJ_P(getThis()));

	if (!intern->start) {
		int flag = MDB_NEXT;
		intern->rc = mdb_cursor_get(intern->cursor, &intern->k, &intern->v, flag);
		intern->start = 1;
	}
	if (intern->rc == MDB_SUCCESS) {
		RETVAL_TRUE;
	} else if (intern->rc == MDB_NOTFOUND) {
		RETVAL_FALSE;
	} else {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Failed to valid (%s)", mdb_strerror(intern->rc));
		return;
	}
}
