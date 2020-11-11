
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

#include "storage/lmdb.h"
#include "storage/lmdb/cursor.h"
#include "storage/exception.h"
#include "storage/frontendinterface.h"

#include <ext/standard/file.h>
#include <zend_smart_str.h>

#include "kernel/main.h"
#include "kernel/exception.h"
#include "kernel/memory.h"
#include "kernel/variables.h"
#include "kernel/array.h"
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "kernel/file.h"

/**
 * Dao\Storage\Lmdb
 *
 */
zend_class_entry *dao_storage_lmdb_ce;

PHP_METHOD(Dao_Storage_Lmdb, __construct);
PHP_METHOD(Dao_Storage_Lmdb, begin);
PHP_METHOD(Dao_Storage_Lmdb, commit);
PHP_METHOD(Dao_Storage_Lmdb, renew);
PHP_METHOD(Dao_Storage_Lmdb, reset);
PHP_METHOD(Dao_Storage_Lmdb, getAll);
PHP_METHOD(Dao_Storage_Lmdb, get);
PHP_METHOD(Dao_Storage_Lmdb, put);
PHP_METHOD(Dao_Storage_Lmdb, del);
PHP_METHOD(Dao_Storage_Lmdb, cursor);
PHP_METHOD(Dao_Storage_Lmdb, copy);
PHP_METHOD(Dao_Storage_Lmdb, drop);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_lmdb___construct, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 1)
	ZEND_ARG_TYPE_INFO(0, readers, IS_LONG, 1)
	ZEND_ARG_TYPE_INFO(0, mapsize, IS_LONG, 1)
	ZEND_ARG_TYPE_INFO(0, envflags, IS_LONG, 1)
	ZEND_ARG_TYPE_INFO(0, flags, IS_LONG, 1)
	ZEND_ARG_TYPE_INFO(0, frontend, IS_OBJECT, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_lmdb_begin, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, flags, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_lmdb_getall, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, dup, _IS_BOOL, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_lmdb_get, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_lmdb_put, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
	ZEND_ARG_INFO(0, value)
	ZEND_ARG_TYPE_INFO(0, flags, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_lmdb_del, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_lmdb_copy, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, flags, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_lmdb_drop, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, delete, _IS_BOOL, 1)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_storage_lmdb_method_entry[] = {
	PHP_ME(Dao_Storage_Lmdb, __construct, arginfo_dao_storage_lmdb___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Storage_Lmdb, begin, arginfo_dao_storage_lmdb_begin, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Lmdb, commit, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Lmdb, renew, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Lmdb, reset, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Lmdb, getAll, arginfo_dao_storage_lmdb_getall, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Lmdb, get, arginfo_dao_storage_lmdb_get, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Lmdb, put, arginfo_dao_storage_lmdb_put, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Lmdb, del, arginfo_dao_storage_lmdb_del, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Lmdb, cursor, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Lmdb, copy, arginfo_dao_storage_lmdb_copy, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Lmdb, drop, arginfo_dao_storage_lmdb_drop, ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Storage_Lmdb, set, put, arginfo_dao_storage_lmdb_put, ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Storage_Lmdb, delete, del, arginfo_dao_storage_lmdb_del, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

zend_object_handlers dao_storage_lmdb_object_handlers;
zend_object* dao_storage_lmdb_object_create_handler(zend_class_entry *ce)
{
	dao_storage_lmdb_object *intern = ecalloc(1, sizeof(dao_storage_lmdb_object) + zend_object_properties_size(ce));
	intern->std.ce = ce;

	zend_object_std_init(&intern->std, ce);
	object_properties_init(&intern->std, ce);
	intern->std.handlers = &dao_storage_lmdb_object_handlers;

	return &intern->std;
}

void dao_storage_lmdb_object_free_handler(zend_object *object)
{
	dao_storage_lmdb_object *intern;

	intern = dao_storage_lmdb_object_from_obj(object);

	if (intern->txn) {
		mdb_txn_abort(intern->txn);
		intern->txn = NULL;
	}
	if (intern->dbi) {
		mdb_dbi_close(intern->env, intern->dbi);
		intern->dbi = 0;
	}

	if (intern->env) {
		mdb_env_close(intern->env);
		intern->env = NULL;
	}

	zend_object_std_dtor(&intern->std);
}

/**
 * Dao\Storage\Lmdb initializer
 */
DAO_INIT_CLASS(Dao_Storage_Lmdb){

	DAO_REGISTER_CLASS_CREATE_OBJECT(Dao\\Storage, Lmdb, storage_lmdb, dao_storage_lmdb_method_entry, 0);

	zend_declare_property_null(dao_storage_lmdb_ce, SL("_frontend"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_storage_lmdb_ce, SL("_path"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_storage_lmdb_ce, SL("_name"), ZEND_ACC_PROTECTED);
	zend_declare_property_long(dao_storage_lmdb_ce, SL("_flag"), 0, ZEND_ACC_PROTECTED);

	// Environment Flags
	zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("FIXEDMAP"),	MDB_FIXEDMAP);
	zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("NOSUBDIR"),	MDB_NOSUBDIR);
	zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("NOSYNC"),		MDB_NOSYNC);
	zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("RDONLY"),		MDB_RDONLY);
	zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("NOMETASYNC"),	MDB_NOMETASYNC);
	zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("WRITEMAP"),	MDB_WRITEMAP);
	zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("MAPASYNC"),	MDB_MAPASYNC);
	zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("NOTLS"),		MDB_NOTLS);
	zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("NOLOCK"),		MDB_NOLOCK);
	zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("NORDAHEAD"),	MDB_NORDAHEAD);
	zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("NOMEMINIT"),	MDB_NOMEMINIT);
	zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("MDB_PREVSNAPSHOT"),	MDB_PREVSNAPSHOT);

	// Database Flags
	zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("REVERSEKEY"),	MDB_REVERSEKEY);
	zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("DUPSORT"),	MDB_DUPSORT);
	zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("INTEGERKEY"),	MDB_INTEGERKEY);
	zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("DUPFIXED"),	MDB_DUPFIXED);
	zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("INTEGERDUP"),	MDB_INTEGERDUP);
	zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("REVERSEDUP"),	MDB_REVERSEDUP);
	zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("CREATE"),		MDB_CREATE);

	// Write Flags
	zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("NOOVERWRITE"),	MDB_NOOVERWRITE);
	zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("NODUPDATA"),		MDB_NODUPDATA);
	//zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("CURRENT"),		MDB_CURRENT);
	zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("RESERVE"),		MDB_RESERVE);
	zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("APPEND"),			MDB_APPEND);
	zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("APPENDDUP"),		MDB_APPENDDUP);
	//zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("MULTIPLE"),		MDB_MULTIPLE);

	// Copy Flags
	zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("CP_COMPACT"),		MDB_CP_COMPACT);

	// Return Codes
	zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("SUCCESS"),			MDB_SUCCESS);
	zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("KEYEXIST"),			MDB_KEYEXIST);
	zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("NOTFOUND"),			MDB_NOTFOUND);
	zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("PAGE_NOTFOUND"),		MDB_PAGE_NOTFOUND);
	zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("CORRUPTED"),			MDB_CORRUPTED);
	zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("PANIC"),				MDB_PANIC);
	zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("VERSION_MISMATCH"),	MDB_VERSION_MISMATCH);
	zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("INVALID"),			MDB_INVALID);
	zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("MAP_FULL"),			MDB_MAP_FULL);
	zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("DBS_FULL"),			MDB_DBS_FULL);
	zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("READERS_FULL"),		MDB_READERS_FULL);
	zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("TLS_FULL"),			MDB_TLS_FULL);
	zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("TXN_FULL"),			MDB_TXN_FULL);
	zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("CURSOR_FULL"),		MDB_CURSOR_FULL);
	zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("PAGE_FULL"),			MDB_PAGE_FULL);
	zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("MAP_RESIZED"),		MDB_MAP_RESIZED);

	zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("INCOMPATIBLE"),		MDB_INCOMPATIBLE);
	zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("BAD_RSLOT"),			MDB_BAD_RSLOT);
	zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("BAD_TXN"),			MDB_BAD_TXN);
	zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("BAD_VALSIZE"),		MDB_BAD_VALSIZE);
	zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("BAD_DBI"),			MDB_BAD_DBI);
	zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("PROBLEM"),			MDB_PROBLEM);
	zend_declare_class_constant_long(dao_storage_lmdb_ce, SL("LAST_ERRCODE"),		MDB_LAST_ERRCODE);

	return SUCCESS;
}

/**
 * Dao\Storage\Lmdb constructor
 *
 * @param string $path
 * @param string $name
 * @param int $readers
 * @param int $mapsize
 * @param int $envflags
 * @param int $flags
 * @param Dao\Storage\FrontendInterface $frontend
 */
PHP_METHOD(Dao_Storage_Lmdb, __construct)
{
	zval *path, *name = NULL, *readers = NULL, *mapsize = NULL, *_envflags = NULL, *_flags = NULL, *frontend = NULL;
	dao_storage_lmdb_object *intern;
	int envflags = MDB_NOSYNC, flags = MDB_CREATE, rc;

	dao_fetch_params(0, 1, 6, &path, &name, &readers, &mapsize, &_envflags, &_flags, &frontend);

	if (!name) {
		name = &DAO_GLOBAL(z_null);
	}

	if (_envflags && Z_TYPE_P(_envflags) == IS_LONG) {
		envflags = Z_LVAL_P(_envflags);
	}

	if (_flags && Z_TYPE_P(_flags) == IS_LONG) {
		flags = Z_LVAL_P(_flags);
	}
	if (frontend && Z_TYPE_P(frontend) == IS_OBJECT) {
		DAO_VERIFY_INTERFACE_EX(frontend, dao_storage_frontendinterface_ce, dao_storage_exception_ce);
		dao_update_property(getThis(), SL("_frontend"), frontend);
	}

	if (dao_file_exists(path) == FAILURE) {
		php_stream_mkdir(Z_STRVAL_P(path), 0775, PHP_STREAM_MKDIR_RECURSIVE | REPORT_ERRORS, php_stream_context_from_zval(NULL, 0));
	}

	dao_update_property(getThis(), SL("_path"), path);

	intern = dao_storage_lmdb_object_from_obj(Z_OBJ_P(getThis()));
	intern->flags = flags;
	rc = mdb_env_create(&intern->env);
	if (rc != MDB_SUCCESS) {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Failed to create an LMDB environment handle (%s)", mdb_strerror(rc));
		return;
	}
	if (readers && Z_TYPE_P(readers) == IS_LONG) {
		mdb_env_set_maxreaders(intern->env, Z_LVAL_P(readers) + 2);
	} else {
		mdb_env_set_maxreaders(intern->env, 1024);
	}
	if (mapsize && Z_TYPE_P(mapsize) == IS_LONG) {
		mdb_env_set_mapsize(intern->env, Z_LVAL_P(mapsize));
	} else {
		 mdb_env_set_mapsize(intern->env, 256 * 1024 * 1024);
	}
	mdb_env_set_maxdbs(intern->env, 256);

	rc = mdb_env_open(intern->env, Z_STRVAL_P(path), envflags, 0664);
	
	if (rc != MDB_SUCCESS) {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Failed to open an environment handle (%s)", mdb_strerror(rc));
		return;
	}

	rc = mdb_txn_begin(intern->env, NULL, 0, &intern->txn);
	if (rc != MDB_SUCCESS) {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Failed to create a transaction for use with the environment (%s)", mdb_strerror(rc));
		return;
	}

	if (Z_TYPE_P(name) == IS_STRING) {
		dao_update_property(getThis(), SL("_name"), name);
		rc = mdb_dbi_open(intern->txn, Z_STRVAL_P(name), flags, &intern->dbi);
	} else {
		rc = mdb_dbi_open(intern->txn, NULL, flags, &intern->dbi);
	}

	if (rc != MDB_SUCCESS) {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Failed to open a database in the environment (%s)", mdb_strerror(rc));
		return;
	}

	rc = mdb_txn_commit(intern->txn);

	if (rc != MDB_SUCCESS) {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Failed to commit all the operations of a transaction into the database (%s)", mdb_strerror(rc));
		return;
	}
	intern->txn = NULL;
}

/**
 * Create a transaction for use with the environment
 *
 * @param int $flags
 */
PHP_METHOD(Dao_Storage_Lmdb, begin)
{
	zval *flags = NULL;
	dao_storage_lmdb_object *intern;
	int rc;

	dao_fetch_params(0, 0, 1, &flags);

	intern = dao_storage_lmdb_object_from_obj(Z_OBJ_P(getThis()));
	if (flags && Z_TYPE_P(flags) == IS_LONG) {
		rc = mdb_txn_begin(intern->env, NULL, Z_LVAL_P(flags), &intern->txn);
	} else {
		rc = mdb_txn_begin(intern->env, NULL, 0, &intern->txn);
	}
	if (rc != MDB_SUCCESS) {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Failed to create a transaction for use with the environment (%s)", mdb_strerror(rc));
		return;
	}
	RETURN_TRUE;
}

/**
 * Commit all the operations of a transaction into the database
 *
 */
PHP_METHOD(Dao_Storage_Lmdb, commit)
{
	dao_storage_lmdb_object *intern;
	int rc;

	intern = dao_storage_lmdb_object_from_obj(Z_OBJ_P(getThis()));

	rc = mdb_txn_commit(intern->txn);
	if (rc != MDB_SUCCESS) {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Failed to commit all the operations of a transaction into the database (%s)", mdb_strerror(rc));
		return;
	}
	intern->txn = NULL;
	RETURN_TRUE;
}

/**
 * Renew a read-only transaction
 *
 */
PHP_METHOD(Dao_Storage_Lmdb, renew)
{
	dao_storage_lmdb_object *intern;

	intern = dao_storage_lmdb_object_from_obj(Z_OBJ_P(getThis()));

	mdb_txn_renew(intern->txn);
}

/**
 * Reset a read-only transaction
 *
 */
PHP_METHOD(Dao_Storage_Lmdb, reset)
{
	dao_storage_lmdb_object *intern;

	intern = dao_storage_lmdb_object_from_obj(Z_OBJ_P(getThis()));

	mdb_txn_reset(intern->txn);
}

/**
 * Get all items from a database
 *
 * @return array
 */
PHP_METHOD(Dao_Storage_Lmdb, getAll)
{
	zval *dup = NULL, frontend = {};
	MDB_val k, v;
	MDB_cursor *cursor;
	dao_storage_lmdb_object *intern;
	int flags = MDB_NEXT, rc;

	dao_fetch_params(0, 0, 1, &dup);

	intern = dao_storage_lmdb_object_from_obj(Z_OBJ_P(getThis()));

	if (dup && zend_is_true(dup)) {
		flags = MDB_NEXT_DUP;
	}

	rc = mdb_cursor_open(intern->txn, intern->dbi, &cursor);
	if (rc != MDB_SUCCESS) {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Failed to create a cursor handle (%s)", mdb_strerror(rc));
		return;
	}

	dao_read_property(&frontend, getThis(), SL("_frontend"), PH_NOISY|PH_READONLY);

	array_init(return_value);
	while ((rc = mdb_cursor_get(cursor, &k, &v, flags)) == 0) {
		zval s = {}, u = {};
		ZVAL_STRINGL(&s, (char *) v.mv_data, (int) v.mv_size);

		if (Z_TYPE(frontend) == IS_OBJECT) {
			DAO_CALL_METHOD_FLAG(rc, &u, &frontend, "afterRetrieve", &s);
		} else {
			dao_unserialize(&u, &s);
		}
		zval_ptr_dtor(&s);

		if (intern->flags & MDB_DUPSORT) {
			zval key;
			ZVAL_STRINGL(&key, (char *) k.mv_data, (int) k.mv_size);
			dao_array_append_multi_2(return_value, &key, &u, 0);
			zval_ptr_dtor(&key);
		} else {
			dao_array_update_str(return_value, (char *) k.mv_data, (int) k.mv_size, &u, 0);
		}
	}
	mdb_cursor_close(cursor);
}

/**
 * Get items from a database
 *
 * @param string $key
 * @return string
 */
PHP_METHOD(Dao_Storage_Lmdb, get)
{
	zval *key, s = {};
	MDB_val k, v;
	dao_storage_lmdb_object *intern;
	int rc;

	dao_fetch_params(0, 1, 0, &key);

	k.mv_size = Z_STRLEN_P(key);
	k.mv_data = Z_STRVAL_P(key);

	intern = dao_storage_lmdb_object_from_obj(Z_OBJ_P(getThis()));

	rc = mdb_get(intern->txn, intern->dbi, &k, &v);
	if (rc == MDB_SUCCESS) {
		zval frontend = {};
		ZVAL_STRINGL(&s, (char *) v.mv_data, (int) v.mv_size);
		dao_read_property(&frontend, getThis(), SL("_frontend"), PH_NOISY|PH_READONLY);
		if (Z_TYPE(frontend) == IS_OBJECT) {
			DAO_CALL_METHOD_FLAG(rc, return_value, &frontend, "afterretrieve", &s);
		} else {
			dao_unserialize(return_value, &s);
		}
		zval_ptr_dtor(&s);
	} else if (rc == MDB_NOTFOUND) {
		RETVAL_FALSE;
	} else {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Failed to get item from a database (%s)", mdb_strerror(rc));
		return;
	}
}

/**
 * Store items into a database
 *
 * @param string $key
 * @param mixed $value
 * @param int $flags
 * @return mixed
 */
PHP_METHOD(Dao_Storage_Lmdb, put)
{
	zval *key, *value, *_flags = NULL, frontend = {}, s = {};
	MDB_val k, v;
	dao_storage_lmdb_object *intern;
	int flags = 0, rc;

	dao_fetch_params(0, 2, 1, &key, &value, &_flags);

	if (_flags && Z_TYPE_P(_flags) == IS_LONG) {
		flags = Z_LVAL_P(_flags);
	}

	dao_read_property(&frontend, getThis(), SL("_frontend"), PH_NOISY|PH_READONLY);
	if (Z_TYPE(frontend) == IS_OBJECT) {
		DAO_CALL_METHOD_FLAG(rc, &s, &frontend, "beforestore", value);
		if (rc == FAILURE) {
			return;
		}
	} else {
		dao_serialize(&s, value);
	}

	k.mv_size = Z_STRLEN_P(key);
	k.mv_data = Z_STRVAL_P(key);
	v.mv_size = Z_STRLEN(s);
	v.mv_data = Z_STRVAL(s);

	intern = dao_storage_lmdb_object_from_obj(Z_OBJ_P(getThis()));

	rc = mdb_put(intern->txn, intern->dbi, &k, &v, flags);
	zval_ptr_dtor(&s);
	if (rc != MDB_SUCCESS) {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Failed to store items into a database (%s)", mdb_strerror(rc));
		return;
	}

	RETURN_TRUE;
}

/**
 * Delete items from a database
 *
 * @param string $keys
 * @return boolean
 */
PHP_METHOD(Dao_Storage_Lmdb, del)
{
	zval *key;
	MDB_val k;
	dao_storage_lmdb_object *intern;
	int rc;

	dao_fetch_params(0, 1, 0, &key);

	k.mv_size = Z_STRLEN_P(key);
	k.mv_data = Z_STRVAL_P(key);

	intern = dao_storage_lmdb_object_from_obj(Z_OBJ_P(getThis()));

	rc = mdb_del(intern->txn, intern->dbi, &k, NULL);
	if (rc == MDB_SUCCESS) {
		RETVAL_TRUE;
	} else if (rc == MDB_NOTFOUND) {
		RETVAL_FALSE;
	} else {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Failed to delete items from a database (%s)", mdb_strerror(rc));
		return;
	}
}

/**
 * Create a cursor handle
 *
 * @return Dao\Storage\Lmdb\Cursor
 */
PHP_METHOD(Dao_Storage_Lmdb, cursor)
{
	zval frontend = {};
	MDB_cursor *cursor;
	dao_storage_lmdb_object *intern;
	dao_storage_lmdb_cursor_object *cursor_intern;
	int rc;

	intern = dao_storage_lmdb_object_from_obj(Z_OBJ_P(getThis()));

	rc = mdb_cursor_open(intern->txn, intern->dbi, &cursor);
	if (rc != MDB_SUCCESS) {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Failed to create a cursor handle (%s)", mdb_strerror(rc));
		return;
	}

	object_init_ex(return_value, dao_storage_lmdb_cursor_ce);
	cursor_intern = dao_storage_lmdb_cursor_object_from_obj(Z_OBJ_P(return_value));
	cursor_intern->cursor = cursor;
	cursor_intern->flags = intern->flags;
	
	dao_read_property(&frontend, getThis(), SL("_frontend"), PH_NOISY|PH_READONLY);
	if (Z_TYPE(frontend) == IS_OBJECT) {
		dao_update_property(return_value, SL("_frontend"), &frontend);
	}
}

/**
 * Copy an LMDB environment to the specified path
 *
 * @param string $path
 * @param int $flags
 * @return boolean
 */
PHP_METHOD(Dao_Storage_Lmdb, copy)
{
	zval *path, *_flags = NULL;
	dao_storage_lmdb_object *intern;
	int flags = 0, rc;

	dao_fetch_params(0, 1, 1, &path, &_flags);

	if (dao_file_exists(path) == FAILURE) {
		php_stream_mkdir(Z_STRVAL_P(path), 0775, PHP_STREAM_MKDIR_RECURSIVE | REPORT_ERRORS, php_stream_context_from_zval(NULL, 0));
	}

	if (_flags && Z_TYPE_P(_flags) == IS_LONG) {
		flags = Z_LVAL_P(_flags);
	}

	intern = dao_storage_lmdb_object_from_obj(Z_OBJ_P(getThis()));

	rc = mdb_env_copy2(intern->env, Z_STRVAL_P(path), flags);
	if (rc != MDB_SUCCESS) {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Failed to copy an LMDB environment (%s)", mdb_strerror(rc));
		return;
	}
	RETURN_TRUE;
}

/**
 * Empty or delete+close a database
 *
 * @return boolean
 */
PHP_METHOD(Dao_Storage_Lmdb, drop)
{
	zval *del = NULL;
	dao_storage_lmdb_object *intern;
	int rc;

	dao_fetch_params(0, 0, 1, &del);

	if (!del) {
		del = &DAO_GLOBAL(z_false);
	}

	intern = dao_storage_lmdb_object_from_obj(Z_OBJ_P(getThis()));

	rc = mdb_drop(intern->txn, intern->dbi, zend_is_true(del) ? 1 : 0);
	if (rc != MDB_SUCCESS) {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Failed to drop a database (%s)", mdb_strerror(rc));
		return;
	}

	RETURN_TRUE;
}
