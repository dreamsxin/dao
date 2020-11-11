
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

#include "cache/backend/wiredtiger.h"
#include "cache/backend.h"
#include "cache/backendinterface.h"
#include "cache/exception.h"
#include "storage/wiredtiger.h"

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
 * Dao\Cache\Backend\Wiredtiger
 *
 * Allows to cache output fragments, PHP data or raw data to a wiredtiger backend
 *
 * This adapter uses the special wiredtigerd key "_PHCY" to store all the keys internally used by the adapter
 *
 *<code>
 *
 * // Cache data for 2 days
 * $frontCache = new Dao\Cache\Frontend\Data(array(
 *    "lifetime" => 172800
 * ));
 *
 * //Create the Cache setting wiredtigerd connection options
 * $cache = new Dao\Cache\Backend\Wiredtiger($frontCache, array(
 * 	'home' => __DIR__.'/wiredtiger'
 *	'table' => 'dao_test'
 *));
 *
 * //Cache arbitrary data
 * $cache->save('my-data', array(1, 2, 3, 4, 5));
 *
 * //Get data
 * $data = $cache->get('my-data');
 *
 *</code>
 */
zend_class_entry *dao_cache_backend_wiredtiger_ce;

PHP_METHOD(Dao_Cache_Backend_Wiredtiger, __construct);
PHP_METHOD(Dao_Cache_Backend_Wiredtiger, get);
PHP_METHOD(Dao_Cache_Backend_Wiredtiger, save);
PHP_METHOD(Dao_Cache_Backend_Wiredtiger, delete);
PHP_METHOD(Dao_Cache_Backend_Wiredtiger, queryKeys);
PHP_METHOD(Dao_Cache_Backend_Wiredtiger, exists);
PHP_METHOD(Dao_Cache_Backend_Wiredtiger, increment);
PHP_METHOD(Dao_Cache_Backend_Wiredtiger, decrement);
PHP_METHOD(Dao_Cache_Backend_Wiredtiger, flush);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_cache_backend_wiredtiger___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, frontend)
	ZEND_ARG_TYPE_INFO(0, options, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_cache_backend_wiredtiger_method_entry[] = {
	PHP_ME(Dao_Cache_Backend_Wiredtiger, __construct, arginfo_dao_cache_backend_wiredtiger___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Cache_Backend_Wiredtiger, get, arginfo_dao_cache_backendinterface_get, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Backend_Wiredtiger, save, arginfo_dao_cache_backendinterface_save, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Backend_Wiredtiger, delete, arginfo_dao_cache_backendinterface_delete, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Backend_Wiredtiger, queryKeys, arginfo_dao_cache_backendinterface_querykeys, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Backend_Wiredtiger, exists, arginfo_dao_cache_backendinterface_exists, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Backend_Wiredtiger, increment, arginfo_dao_cache_backendinterface_increment, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Backend_Wiredtiger, decrement, arginfo_dao_cache_backendinterface_decrement, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Backend_Wiredtiger, flush, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Cache\Backend\Wiredtiger initializer
 */
DAO_INIT_CLASS(Dao_Cache_Backend_Wiredtiger)
{
	DAO_REGISTER_CLASS_EX(Dao\\Cache\\Backend, Wiredtiger, cache_backend_wiredtiger, dao_cache_backend_ce, dao_cache_backend_wiredtiger_method_entry, 0);

	zend_declare_property_null(dao_cache_backend_wiredtiger_ce, SL("_table"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_cache_backend_wiredtiger_ce, SL("_config"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_cache_backend_wiredtiger_ce, SL("_wiredtiger"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_cache_backend_wiredtiger_ce, SL("_cursor"), ZEND_ACC_PROTECTED);

	zend_class_implements(dao_cache_backend_wiredtiger_ce, 1, dao_cache_backendinterface_ce);

	return SUCCESS;
}

/**
 * Dao\Cache\Backend\Wiredtiger constructor
 *
 * @param Dao\Cache\FrontendInterface $frontend
 * @param array $options
 */
PHP_METHOD(Dao_Cache_Backend_Wiredtiger, __construct){

	zval *frontend, *options = NULL, home = {}, table = {}, prefixed_table = {}, config = {}, wiredtiger = {}, cursor = {};

	dao_fetch_params(0, 1, 1, &frontend, &options);

	if (!options || Z_TYPE_P(options) != IS_ARRAY) {
		DAO_THROW_EXCEPTION_STR(dao_cache_exception_ce, "The options must be array");
		return;
	}

	if (!dao_array_isset_fetch_str(&home, options, SL("home"), PH_READONLY)) {
		DAO_THROW_EXCEPTION_STR(dao_cache_exception_ce, "The parameter 'home' is required");
		return;
	}

	if (!dao_array_isset_fetch_str(&table, options, SL("table"), PH_READONLY)) {
		DAO_THROW_EXCEPTION_STR(dao_cache_exception_ce, "The parameter 'table' is required");
		return;
	}

	DAO_CONCAT_SV(&prefixed_table, "table:", &table);
	ZVAL_STRING(&config, "key_format=S,value_format=Si");

	dao_update_property(getThis(), SL("_table"), &prefixed_table);
	dao_update_property(getThis(), SL("_config"), &config);

	object_init_ex(&wiredtiger, dao_storage_wiredtiger_ce);
	DAO_CALL_METHOD(NULL, &wiredtiger, "__construct", &home);
	DAO_CALL_METHOD(NULL, &wiredtiger, "create", &prefixed_table, &config);
	DAO_CALL_METHOD(&cursor, &wiredtiger, "open", &prefixed_table);

	dao_update_property(getThis(), SL("_wiredtiger"), &wiredtiger);
	dao_update_property(getThis(), SL("_cursor"), &cursor);
	zval_ptr_dtor(&wiredtiger);
	zval_ptr_dtor(&cursor);

	DAO_CALL_PARENT(NULL, dao_cache_backend_wiredtiger_ce, getThis(), "__construct", frontend, options);
	zval_ptr_dtor(&prefixed_table);
	zval_ptr_dtor(&config);
}

/**
 * Returns a cached content
 *
 * @param int|string $keyName
 * @return mixed
 */
PHP_METHOD(Dao_Cache_Backend_Wiredtiger, get){

	zval *key_name, cursor = {}, frontend = {}, cached_content = {}, val = {}, expired = {};
	long int now;

	dao_fetch_params(0, 1, 0, &key_name);

	dao_read_property(&cursor, getThis(), SL("_cursor"), PH_READONLY);

	DAO_CALL_METHOD(&cached_content, &cursor, "get", key_name);
	if (Z_TYPE(cached_content) != IS_ARRAY) {
		RETURN_NULL();
	}

	if (!dao_array_isset_fetch_long(&val, &cached_content, 0, PH_COPY)) {
		ZVAL_NULL(&val);
	}

	now = (long int)time(NULL);

	if (dao_array_isset_fetch_long(&expired, &cached_content, 1, PH_READONLY)) {
		if (dao_get_intval(&expired) < now) {
			DAO_CALL_METHOD(NULL, &cursor, "delete", key_name);
		}
	}
	zval_ptr_dtor(&cached_content);

	if (DAO_IS_NOT_EMPTY(&val)) {
		dao_read_property(&frontend, getThis(), SL("_frontend"), PH_READONLY);
		DAO_RETURN_CALL_METHOD(&frontend, "afterretrieve", &val);
		zval_ptr_dtor(&val);
	} else {
		RETURN_ZVAL(&val, 0, 0);
	}
}

/**
 * Stores cached content into the Wiredtigerd backend and stops the frontend
 *
 * @param int|string $keyName
 * @param string $content
 * @param long $lifetime
 * @param boolean $stopBuffer
 */
PHP_METHOD(Dao_Cache_Backend_Wiredtiger, save){

	zval *key_name = NULL, *content = NULL, *lifetime = NULL, *stop_buffer = NULL, key = {}, expired = {}, val = {}, prepared_val = {}, cached_content = {}, success = {};
	zval ttl = {}, is_buffering = {}, frontend = {}, cursor = {};

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
	ZVAL_LONG(&expired, (time(NULL) + dao_get_intval(&ttl)));

	/**
	 * Check if a connection is created or make a new one
	 */
	dao_read_property(&cursor, getThis(), SL("_cursor"), PH_READONLY);

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

	array_init_size(&cached_content, 2);
	dao_array_append(&cached_content, &prepared_val, 0);
	dao_array_append(&cached_content, &expired, 0);

	DAO_CALL_METHOD(&success, &cursor, "set", key_name, &cached_content);

	if (!zend_is_true(&success)) {
		zval_ptr_dtor(&cached_content);
		DAO_THROW_EXCEPTION_STR(dao_cache_exception_ce, "Failed to store data in wiredtiger");
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
PHP_METHOD(Dao_Cache_Backend_Wiredtiger, delete){

	zval *key_name, cursor = {};

	dao_fetch_params(0, 1, 0, &key_name);

	dao_read_property(&cursor, getThis(), SL("_cursor"), PH_READONLY);

	DAO_CALL_METHOD(return_value, &cursor, "delete", key_name);
}

/**
 * Query the existing cached keys
 *
 * @return array
 */
PHP_METHOD(Dao_Cache_Backend_Wiredtiger, queryKeys){

	zval *prefix = NULL, cursor = {};

	dao_fetch_params(0, 0, 1, &prefix);

	dao_read_property(&cursor, getThis(), SL("_cursor"), PH_READONLY);

	array_init(return_value);

	/* Get the key from wiredtigerd */
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
}

/**
 * Checks if cache exists and it hasn't expired
 *
 * @param  string $keyName
 * @param  long $lifetime
 * @return boolean
 */
PHP_METHOD(Dao_Cache_Backend_Wiredtiger, exists){

	zval *key_name, cursor = {}, cached_content = {}, expired = {};
	long int now;

	dao_fetch_params(0, 1, 0, &key_name);

	dao_read_property(&cursor, getThis(), SL("_cursor"), PH_READONLY);

	DAO_CALL_METHOD(&cached_content, &cursor, "get", key_name);
	if (Z_TYPE(cached_content) != IS_ARRAY) {
		RETURN_FALSE;
	}

	now = (long int)time(NULL);

	if (dao_array_isset_fetch_long(&expired, &cached_content, 1, PH_READONLY)) {
		if (dao_get_intval(&expired) < now) {
			DAO_CALL_METHOD(NULL, &cursor, "delete", key_name);
			RETURN_FALSE;
		}
	}

	RETURN_TRUE;
}

/**
 * Atomic increment of a given key, by number $value
 *
 * @param string $keyName
 * @param long $value
 * @return mixed
 */
PHP_METHOD(Dao_Cache_Backend_Wiredtiger, increment){

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

	DAO_RETURN_CALL_METHOD(getThis(), "save", key_name, &tmp);
}

/**
 * Atomic decrement of a given key, by number $value
 *
 * @param  string $keyName
 * @param  long $value
 * @return mixed
 */
PHP_METHOD(Dao_Cache_Backend_Wiredtiger, decrement){

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

	DAO_RETURN_CALL_METHOD(getThis(), "save", key_name, &tmp);
}

/**
 * Immediately invalidates all existing items.
 *
 * @return boolean
 */
PHP_METHOD(Dao_Cache_Backend_Wiredtiger, flush){

	zval table = {}, config = {}, wiredtiger = {}, cursor = {};

	dao_read_property(&table, getThis(), SL("_table"), PH_READONLY);
	dao_read_property(&config, getThis(), SL("_config"), PH_READONLY);

	dao_read_property(&wiredtiger, getThis(), SL("_wiredtiger"), PH_READONLY);
	dao_read_property(&cursor, getThis(), SL("_cursor"), PH_READONLY);

	DAO_CALL_METHOD(NULL, &cursor, "close");
	DAO_CALL_METHOD(return_value, &wiredtiger, "drop", &table);

	DAO_CALL_METHOD(NULL, &wiredtiger, "create", &table, &config);
	DAO_CALL_METHOD(&cursor, &wiredtiger, "open", &table);

	dao_update_property(getThis(), SL("_cursor"), &cursor);

	RETURN_TRUE;
}
