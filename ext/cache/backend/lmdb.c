
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

#include "cache/backend/lmdb.h"
#include "cache/backend.h"
#include "cache/backendinterface.h"
#include "cache/exception.h"
#include "storage/lmdb.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/fcall.h"
#include "kernel/object.h"
#include "kernel/exception.h"
#include "kernel/concat.h"
#include "kernel/operators.h"
#include "kernel/hash.h"
#include "kernel/string.h"

/**
 * Dao\Cache\Backend\Lmdb
 *
 * Allows to cache output fragments, PHP data or raw data to a lmdb backend
 *
 * This adapter uses the special lmdbd key "_PHCY" to store all the keys internally used by the adapter
 *
 *<code>
 *
 * // Cache data for 2 days
 * $frontCache = new Dao\Cache\Frontend\Data(array(
 *    "lifetime" => 172800
 * ));
 *
 * // Create the Cache setting lmdb connection options
 * $cache = new Dao\Cache\Backend\Lmdb($frontCache, array(
 * 	'path' => __DIR__.'/lmdb',
 *	'name' => 'dao_test'
 *));
 *
 * // Or
 * $cache = new Dao\Cache\Backend\Lmdb($frontCache, array(
 *		'lmdb' => new Dao\Storage\Lmdb(__DIR__.'/lmdb')
 * ));
 *
 * // Cache arbitrary data
 * $cache->save('my-data', array(1, 2, 3, 4, 5));
 *
 * // Get data
 * $data = $cache->get('my-data');
 *
 *</code>
 */
zend_class_entry *dao_cache_backend_lmdb_ce;

PHP_METHOD(Dao_Cache_Backend_Lmdb, __construct);
PHP_METHOD(Dao_Cache_Backend_Lmdb, get);
PHP_METHOD(Dao_Cache_Backend_Lmdb, save);
PHP_METHOD(Dao_Cache_Backend_Lmdb, delete);
PHP_METHOD(Dao_Cache_Backend_Lmdb, queryKeys);
PHP_METHOD(Dao_Cache_Backend_Lmdb, exists);
PHP_METHOD(Dao_Cache_Backend_Lmdb, increment);
PHP_METHOD(Dao_Cache_Backend_Lmdb, decrement);
PHP_METHOD(Dao_Cache_Backend_Lmdb, flush);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_cache_backend_lmdb___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, frontend)
	ZEND_ARG_TYPE_INFO(0, options, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_cache_backend_lmdb_method_entry[] = {
	PHP_ME(Dao_Cache_Backend_Lmdb, __construct, arginfo_dao_cache_backend_lmdb___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Cache_Backend_Lmdb, get, arginfo_dao_cache_backendinterface_get, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Backend_Lmdb, save, arginfo_dao_cache_backendinterface_save, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Backend_Lmdb, delete, arginfo_dao_cache_backendinterface_delete, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Backend_Lmdb, queryKeys, arginfo_dao_cache_backendinterface_querykeys, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Backend_Lmdb, exists, arginfo_dao_cache_backendinterface_exists, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Backend_Lmdb, increment, arginfo_dao_cache_backendinterface_increment, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Backend_Lmdb, decrement, arginfo_dao_cache_backendinterface_decrement, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Backend_Lmdb, flush, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Cache\Backend\Lmdb initializer
 */
DAO_INIT_CLASS(Dao_Cache_Backend_Lmdb)
{
	DAO_REGISTER_CLASS_EX(Dao\\Cache\\Backend, Lmdb, cache_backend_lmdb, dao_cache_backend_ce, dao_cache_backend_lmdb_method_entry, 0);

	zend_declare_property_null(dao_cache_backend_lmdb_ce, SL("_config"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_cache_backend_lmdb_ce, SL("_lmdb"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_cache_backend_lmdb_ce, SL("_lmdb"), ZEND_ACC_PROTECTED);

	zend_class_implements(dao_cache_backend_lmdb_ce, 1, dao_cache_backendinterface_ce);

	return SUCCESS;
}

/**
 * Dao\Cache\Backend\Lmdb constructor
 *
 * @param Dao\Cache\FrontendInterface $frontend
 * @param array $options
 */
PHP_METHOD(Dao_Cache_Backend_Lmdb, __construct){

	zval *frontend, *options = NULL, path = {}, name = {}, lmdb = {};

	dao_fetch_params(0, 1, 1, &frontend, &options);

	if (!options || Z_TYPE_P(options) != IS_ARRAY) {
		DAO_THROW_EXCEPTION_STR(dao_cache_exception_ce, "The options must be array");
		return;
	}

	if (!dao_array_isset_fetch_str(&lmdb, options, SL("lmdb"), PH_COPY)) {
		if (!dao_array_isset_fetch_str(&path, options, SL("path"), PH_READONLY)) {
			DAO_THROW_EXCEPTION_STR(dao_cache_exception_ce, "The parameter 'path' is required");
			return;
		}

		if (!dao_array_isset_fetch_str(&name, options, SL("name"), PH_READONLY)) {
			ZVAL_NULL(&name);
		}

		object_init_ex(&lmdb, dao_storage_lmdb_ce);
		DAO_CALL_METHOD(NULL, &lmdb, "__construct", &path, &name);
	} else {
		DAO_VERIFY_CLASS(&lmdb, dao_storage_lmdb_ce);
	}

	dao_update_property(getThis(), SL("_lmdb"), &lmdb);
	zval_ptr_dtor(&lmdb);

	DAO_CALL_PARENT(NULL, dao_cache_backend_lmdb_ce, getThis(), "__construct", frontend, options);
}

/**
 * Returns a cached content
 *
 * @param int|string $keyName
 * @param int $lifetime
 * @return mixed
 */
PHP_METHOD(Dao_Cache_Backend_Lmdb, get){

	zval *key_name, *lifetime = NULL, lmdb = {}, save_time = {}, frontend = {}, cached_content = {}, val = {}, expired = {};
	long now;

	dao_fetch_params(0, 1, 1, &key_name, &lifetime);

	dao_read_property(&lmdb, getThis(), SL("_lmdb"), PH_READONLY);

	DAO_CALL_METHOD(NULL, &lmdb, "begin");
	DAO_CALL_METHOD(&cached_content, &lmdb, "get", key_name);
	if (Z_TYPE(cached_content) != IS_ARRAY) {
		DAO_CALL_METHOD(NULL, &lmdb, "commit");
		zval_ptr_dtor(&cached_content);
		RETURN_NULL();
	}

	if (!dao_array_isset_fetch_long(&val, &cached_content, 0, PH_READONLY)) {
		zval_ptr_dtor(&cached_content);
		RETURN_NULL();
	}

	now = (long)time(NULL);
	
	if (lifetime && dao_array_isset_fetch_long(&save_time, &cached_content, 2, PH_READONLY)) {
		if ((now - dao_get_intval(&save_time)) > dao_get_intval(lifetime)) {
			DAO_CALL_METHOD(NULL, &lmdb, "commit");
			zval_ptr_dtor(&cached_content);
			RETURN_NULL();
		}
	}

	if (dao_array_isset_fetch_long(&expired, &cached_content, 1, PH_READONLY)) {
		if (dao_get_intval(&expired) < now) {
			DAO_CALL_METHOD(NULL, &lmdb, "delete", key_name);
		}
	}
	DAO_CALL_METHOD(NULL, &lmdb, "commit");
	zval_ptr_dtor(&cached_content);

	if (DAO_IS_NOT_EMPTY(&val)) {
		dao_read_property(&frontend, getThis(), SL("_frontend"), PH_READONLY);
		DAO_RETURN_CALL_METHOD(&frontend, "afterretrieve", &val);
	} else {
		RETURN_CTOR(&val);
	}
}

/**
 * Stores cached content into the Lmdbd backend and stops the frontend
 *
 * @param int|string $keyName
 * @param string $content
 * @param long $lifetime
 * @param boolean $stopBuffer
 */
PHP_METHOD(Dao_Cache_Backend_Lmdb, save){

	zval *key_name = NULL, *content = NULL, *lifetime = NULL, *stop_buffer = NULL, key = {}, expired = {}, val = {}, prepared_val = {}, cached_content = {}, success = {};
	zval ttl = {}, is_buffering = {}, frontend = {}, lmdb = {};
	long now_time;

	dao_fetch_params(0, 0, 4, &key_name, &content, &lifetime, &stop_buffer);

	if (!key_name || Z_TYPE_P(key_name) == IS_NULL) {
		dao_read_property(&key, getThis(), SL("_lastKey"), PH_READONLY);
		key_name = &key;
	}

	if (!zend_is_true(key_name)) {
		DAO_THROW_EXCEPTION_STR(dao_cache_exception_ce, "The cache must be started first");
		return;
	}

	/**
	 * Take the lifetime from the frontend or read it from the set in start()
	 */
	if (!lifetime || Z_TYPE_P(lifetime) != IS_LONG) {
		DAO_CALL_METHOD(&ttl, getThis(), "getlifetime");
	} else {
		ZVAL_COPY_VALUE(&ttl, lifetime);
	}
	now_time = (long)time(NULL);
	ZVAL_LONG(&expired, (now_time + dao_get_intval(&ttl)));
	
	/**
	 * Check if a connection is created or make a new one
	 */
	dao_read_property(&lmdb, getThis(), SL("_lmdb"), PH_READONLY);

	dao_read_property(&frontend, getThis(), SL("_frontend"), PH_READONLY);
	if (!content || Z_TYPE_P(content) == IS_NULL) {
		DAO_CALL_METHOD(&val, &frontend, "getcontent");
	} else {
		ZVAL_COPY(&val, content);
	}

	/**
	 * Prepare the content in the frontend
	 */
	DAO_CALL_METHOD(&prepared_val, &frontend, "beforestore", &val);
	zval_ptr_dtor(&val);

	array_init_size(&cached_content, 3);
	dao_array_append(&cached_content, &prepared_val, PH_READONLY);
	dao_array_append(&cached_content, &expired, PH_READONLY);
	dao_array_append_long(&cached_content, now_time, PH_READONLY);

	DAO_CALL_METHOD(NULL, &lmdb, "begin");
	DAO_CALL_METHOD(&success, &lmdb, "put", key_name, &cached_content);
	DAO_CALL_METHOD(NULL, &lmdb, "commit");

	if (!zend_is_true(&success)) {
		zval_ptr_dtor(&cached_content);
		DAO_THROW_EXCEPTION_STR(dao_cache_exception_ce, "Failed to store data in lmdb");
		return;
	}

	DAO_CALL_METHOD(&is_buffering, &frontend, "isbuffering");

	if (!stop_buffer || DAO_IS_TRUE(stop_buffer)) {
		DAO_CALL_METHOD(NULL, &frontend, "stop");
	}

	if (DAO_IS_TRUE(&is_buffering)) {
		zend_print_zval(&cached_content, 0);
	}
	zval_ptr_dtor(&cached_content);

	dao_update_property_bool(getThis(), SL("_started"), 0);

	RETURN_TRUE;
}

/**
 * Deletes a value from the cache by its key
 *
 * @param int|string $keyName
 * @return boolean
 */
PHP_METHOD(Dao_Cache_Backend_Lmdb, delete){

	zval *key_name, lmdb = {};

	dao_fetch_params(0, 1, 0, &key_name);

	dao_read_property(&lmdb, getThis(), SL("_lmdb"), PH_READONLY);

	DAO_CALL_METHOD(NULL, &lmdb, "begin");
	DAO_CALL_METHOD(return_value, &lmdb, "delete", key_name);
	DAO_CALL_METHOD(NULL, &lmdb, "commit");
}

/**
 * Query the existing cached keys
 *
 * @return array
 */
PHP_METHOD(Dao_Cache_Backend_Lmdb, queryKeys){

	zval *prefix = NULL, lmdb = {}, cursor = {};

	dao_fetch_params(0, 0, 1, &prefix);

	dao_read_property(&lmdb, getThis(), SL("_lmdb"), PH_READONLY);

	DAO_CALL_METHOD(NULL, &lmdb, "begin");
	DAO_CALL_METHOD(&cursor, &lmdb, "cursor");

	array_init(return_value);

	DAO_CALL_METHOD(NULL, &cursor, "rewind");
	while (1) {
		zval r0 = {}, key = {};
		DAO_CALL_METHOD(&r0, &cursor, "valid");
		if (!zend_is_true(&r0)) {
			break;
		}
		DAO_CALL_METHOD(&key, &cursor, "key");

		if (!prefix || !zend_is_true(prefix) || dao_start_with(&key, prefix, NULL)) {
			dao_array_append(return_value, &key, 0);
		}
		DAO_CALL_METHOD(NULL, &cursor, "next");
	}
	zval_ptr_dtor(&cursor);
	DAO_CALL_METHOD(NULL, &lmdb, "commit");
}

/**
 * Checks if cache exists and it hasn't expired
 *
 * @param  string $keyName
 * @param  long $lifetime
 * @return boolean
 */
PHP_METHOD(Dao_Cache_Backend_Lmdb, exists){

	zval *key_name, lmdb = {}, cached_content = {}, expired = {};
	long int now;

	dao_fetch_params(0, 1, 0, &key_name);

	dao_read_property(&lmdb, getThis(), SL("_lmdb"), PH_READONLY);

	DAO_CALL_METHOD(NULL, &lmdb, "begin");
	DAO_CALL_METHOD(&cached_content, &lmdb, "get", key_name);
	if (Z_TYPE(cached_content) != IS_ARRAY) {
		DAO_CALL_METHOD(NULL, &lmdb, "commit");
		RETURN_FALSE;
	}

	RETVAL_TRUE;

	now = (long int)time(NULL);

	if (dao_array_isset_fetch_long(&expired, &cached_content, 1, PH_READONLY)) {
		if (dao_get_intval(&expired) < now) {
			DAO_CALL_METHOD(NULL, &lmdb, "delete", key_name);
			RETVAL_FALSE;
		}
	}

	DAO_CALL_METHOD(NULL, &lmdb, "commit");
}

/**
 * Atomic increment of a given key, by number $value
 *
 * @param string $keyName
 * @param long $value
 * @return mixed
 */
PHP_METHOD(Dao_Cache_Backend_Lmdb, increment){

	zval *key_name, *value = NULL, val = {}, tmp = {};

	dao_fetch_params(0, 1, 1, &key_name, &value);

	if (!value || Z_TYPE_P(value) == IS_NULL) {
		value = &DAO_GLOBAL(z_one);
	} else if (Z_TYPE_P(value) != IS_LONG) {
		DAO_SEPARATE_PARAM(value);
		convert_to_long_ex(value);
	}

	DAO_CALL_METHOD(&val, getThis(), "get", key_name);
	dao_add_function(&tmp, &val, value);
	zval_ptr_dtor(&val);

	DAO_RETURN_CALL_METHOD(getThis(), "put", key_name, &tmp);
	zval_ptr_dtor(&tmp);
}

/**
 * Atomic decrement of a given key, by number $value
 *
 * @param  string $keyName
 * @param  long $value
 * @return mixed
 */
PHP_METHOD(Dao_Cache_Backend_Lmdb, decrement){

	zval *key_name, *value = NULL, val = {}, tmp = {};

	dao_fetch_params(0, 1, 1, &key_name, &value);

	if (!value || Z_TYPE_P(value) == IS_NULL) {
		value = &DAO_GLOBAL(z_one);
	} else if (Z_TYPE_P(value) != IS_LONG) {
		DAO_SEPARATE_PARAM(value);
		convert_to_long_ex(value);
	}

	DAO_CALL_METHOD(&val, getThis(), "get", key_name);
	dao_sub_function(&tmp, &val, value);
	zval_ptr_dtor(&val);

	DAO_RETURN_CALL_METHOD(getThis(), "put", key_name, &tmp);
	zval_ptr_dtor(&tmp);
}

/**
 * Immediately invalidates all existing items.
 *
 * @return boolean
 */
PHP_METHOD(Dao_Cache_Backend_Lmdb, flush){

	zval lmdb = {};

	dao_read_property(&lmdb, getThis(), SL("_lmdb"), PH_READONLY);

	DAO_CALL_METHOD(NULL, &lmdb, "begin");
	DAO_CALL_METHOD(return_value, &lmdb, "drop");
	DAO_CALL_METHOD(NULL, &lmdb, "commit");

	RETURN_TRUE;
}
