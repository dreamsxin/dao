
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

#include "cache/backend/memory.h"
#include "cache/backend.h"
#include "cache/backendinterface.h"
#include "cache/exception.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/concat.h"
#include "kernel/array.h"
#include "kernel/fcall.h"
#include "kernel/exception.h"
#include "kernel/operators.h"

/**
 * Dao\Cache\Backend\Memory
 *
 * Stores content in memory. Data is lost when the request is finished
 *
 *<code>
 *	//Cache data
 *	$frontCache = new Dao\Cache\Frontend\Data();
 *
 *  $cache = new Dao\Cache\Backend\Memory($frontCache);
 *
 *	//Cache arbitrary data
 *	$cache->save('my-data', array(1, 2, 3, 4, 5));
 *
 *	//Get data
 *	$data = $cache->get('my-data');
 *
 *</code>
 */
zend_class_entry *dao_cache_backend_memory_ce;

PHP_METHOD(Dao_Cache_Backend_Memory, get);
PHP_METHOD(Dao_Cache_Backend_Memory, save);
PHP_METHOD(Dao_Cache_Backend_Memory, delete);
PHP_METHOD(Dao_Cache_Backend_Memory, queryKeys);
PHP_METHOD(Dao_Cache_Backend_Memory, exists);
PHP_METHOD(Dao_Cache_Backend_Memory, increment);
PHP_METHOD(Dao_Cache_Backend_Memory, decrement);
PHP_METHOD(Dao_Cache_Backend_Memory, flush);

static const zend_function_entry dao_cache_backend_memory_method_entry[] = {
	PHP_ME(Dao_Cache_Backend_Memory, get, arginfo_dao_cache_backendinterface_get, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Backend_Memory, save, arginfo_dao_cache_backendinterface_save, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Backend_Memory, delete, arginfo_dao_cache_backendinterface_delete, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Backend_Memory, queryKeys, arginfo_dao_cache_backendinterface_querykeys, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Backend_Memory, exists, arginfo_dao_cache_backendinterface_exists, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Backend_Memory, increment, arginfo_dao_cache_backendinterface_increment, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Backend_Memory, decrement, arginfo_dao_cache_backendinterface_decrement, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Backend_Memory, flush, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Cache\Backend\Memory initializer
 */
DAO_INIT_CLASS(Dao_Cache_Backend_Memory){

	DAO_REGISTER_CLASS_EX(Dao\\Cache\\Backend, Memory, cache_backend_memory, dao_cache_backend_ce, dao_cache_backend_memory_method_entry, 0);

	zend_declare_property_null(dao_cache_backend_memory_ce, SL("_data"), ZEND_ACC_PROTECTED);

	zend_class_implements(dao_cache_backend_memory_ce, 1, dao_cache_backendinterface_ce);

	return SUCCESS;
}

/**
 * Returns a cached content
 *
 * @param string $keyName
 * @return mixed
 */
PHP_METHOD(Dao_Cache_Backend_Memory, get){

	zval *key_name, data = {}, cached_content = {}, frontend = {}, prefixed_key = {}, prefix = {};

	dao_fetch_params(0, 1, 0, &key_name);

	dao_read_property(&prefix, getThis(), SL("_prefix"), PH_READONLY);
	DAO_CONCAT_VV(&prefixed_key, &prefix, key_name);

	dao_read_property(&data, getThis(), SL("_data"), PH_READONLY);
	if (dao_array_isset_fetch(&cached_content, &data, &prefixed_key, PH_READONLY)) {
		if (Z_TYPE(cached_content) != IS_NULL) {
			if (dao_is_numeric(&cached_content)) {
				RETVAL_ZVAL(&cached_content, 1 , 0);
			} else {
				dao_read_property(&frontend, getThis(), SL("_frontend"), PH_READONLY);
				DAO_RETURN_CALL_METHOD(&frontend, "afterretrieve", &cached_content);
			}
		}
	}
	zval_ptr_dtor(&prefixed_key);
}

/**
 * Stores cached content into the backend and stops the frontend
 *
 * @param string $keyName
 * @param string $content
 * @param long $lifetime
 * @param boolean $stopBuffer
 */
PHP_METHOD(Dao_Cache_Backend_Memory, save){

	zval *key_name = NULL, *content = NULL, *lifetime = NULL, *stop_buffer = NULL, key = {}, cached_content = {}, prepared_content = {}, is_buffering = {}, prefix = {}, prefixed_key = {}, frontend = {};

	dao_fetch_params(0, 0, 4, &key_name, &content, &lifetime, &stop_buffer);

	dao_read_property(&prefix, getThis(), SL("_prefix"), PH_READONLY);

	if (!key_name || Z_TYPE_P(key_name) == IS_NULL) {
		dao_read_property(&key, getThis(), SL("_lastKey"), PH_READONLY);
		key_name = &key;
	}

	if (!zend_is_true(key_name)) {
		DAO_THROW_EXCEPTION_STR(dao_cache_exception_ce, "The cache must be started first");
		return;
	}

	DAO_CONCAT_VV(&prefixed_key, &prefix, key_name);

	dao_read_property(&frontend, getThis(), SL("_frontend"), PH_READONLY);
	if (!content || Z_TYPE_P(content) == IS_NULL) {
		DAO_CALL_METHOD(&cached_content, &frontend, "getcontent");
	} else {
		ZVAL_COPY(&cached_content, content);
	}

	if (dao_is_numeric(&cached_content))	{
		dao_update_property_array(getThis(), SL("_data"), &prefixed_key, &cached_content);
	} else {
		DAO_CALL_METHOD(&prepared_content, &frontend, "beforestore", &cached_content);
		dao_update_property_array(getThis(), SL("_data"), &prefixed_key, &prepared_content);
		zval_ptr_dtor(&prepared_content);
	}
	zval_ptr_dtor(&prefixed_key);

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
 * @param string $keyName
 * @return boolean
 */
PHP_METHOD(Dao_Cache_Backend_Memory, delete){

	zval *key_name, prefix = {}, key = {}, data = {};

	dao_fetch_params(0, 1, 0, &key_name);

	dao_read_property(&prefix, getThis(), SL("_prefix"), PH_READONLY);

	DAO_CONCAT_VV(&key, &prefix, key_name);

	dao_read_property(&data, getThis(), SL("_data"), PH_READONLY);
	if (dao_array_isset(&data, &key)) {
		dao_unset_property_array(getThis(), SL("_data"), &key);
		RETVAL_TRUE;
	} else {
		RETVAL_FALSE;
	}
	zval_ptr_dtor(&key);
}

/**
 * Query the existing cached keys
 *
 * @param string $prefix
 * @return array
 */
PHP_METHOD(Dao_Cache_Backend_Memory, queryKeys){

	zval *_prefix = NULL, prefix = {}, data = {};
	zend_string *str_key;
	ulong idx;

	dao_fetch_params(0, 0, 1, &prefix);

	if (!_prefix || Z_TYPE_P(_prefix) != IS_NULL) {
		dao_read_property(&prefix, getThis(), SL("_prefix"), PH_NOISY|PH_READONLY);
	} else {
		ZVAL_COPY_VALUE(&prefix, _prefix);
	}

	dao_read_property(&data, getThis(), SL("_data"), PH_READONLY);

	if (likely(Z_TYPE(data) == IS_ARRAY)) {
		if (DAO_IS_EMPTY(&prefix)) {
			dao_array_keys(return_value, &data);
		} else {
			array_init(return_value);
			if (likely(Z_TYPE(data) == IS_ARRAY)) {
				ZEND_HASH_FOREACH_KEY(Z_ARRVAL(data), idx, str_key) {
					zval key = {};
					if (str_key) {
							ZVAL_STR(&key, str_key);
					} else {
							ZVAL_LONG(&key, idx);
					}
					if (str_key && ZSTR_LEN(str_key) > (uint)(Z_STRLEN(prefix)) && !memcmp(Z_STRVAL(prefix), ZSTR_VAL(str_key), ZSTR_LEN(str_key)-1)) {
						Z_TRY_ADDREF(key);
						zend_hash_next_index_insert(Z_ARRVAL_P(return_value), &key);
					} else {
						char buf[8 * sizeof(ulong) + 2];
						int buflength = 8 * sizeof(ulong) + 2;
						int size;
						size = snprintf(buf, buflength, "%ld", (long) idx);
						if (size >= Z_STRLEN(prefix) && !memcmp(Z_STRVAL(prefix), buf, size)) {
							Z_TRY_ADDREF(key);
							zend_hash_next_index_insert(Z_ARRVAL_P(return_value), &key);
						}
					}
				} ZEND_HASH_FOREACH_END();
			}
		}
	} else {
		array_init(return_value);
	}
}

/**
 * Checks if cache exists and it hasn't expired
 *
 * @param  string $keyName
 * @param  long $lifetime
 * @return boolean
 */
PHP_METHOD(Dao_Cache_Backend_Memory, exists){

	zval *key_name, prefix = {}, prefixed_key = {}, data = {};

	dao_fetch_params(0, 1, 0, &key_name);

	dao_read_property(&prefix, getThis(), SL("_prefix"), PH_READONLY);
	DAO_CONCAT_VV(&prefixed_key, &prefix, key_name);

	dao_read_property(&data, getThis(), SL("_data"), PH_READONLY);
	if (dao_array_isset(&data, &prefixed_key)) {
		RETVAL_TRUE;
	} else {
		RETVAL_FALSE;
	}
	zval_ptr_dtor(&prefixed_key);
}

/**
 * Increment of given $keyName by $value
 *
 * @param  string $keyName
 * @param  long $lifetime
 * @return mixed
 */
PHP_METHOD(Dao_Cache_Backend_Memory, increment){

	zval *key_name, *value = NULL, prefix = {}, prefixed_key = {}, data = {}, cached_content = {};

	dao_fetch_params(0, 1, 1, &key_name, &value);

	if (!value || Z_TYPE_P(value) == IS_NULL) {
		value = &DAO_GLOBAL(z_one);
	}

	dao_read_property(&prefix, getThis(), SL("_prefix"), PH_READONLY);
	DAO_CONCAT_VV(&prefixed_key, &prefix, key_name);

	dao_read_property(&data, getThis(), SL("_data"), PH_READONLY);
	if (!dao_array_isset_fetch(&cached_content, &data, &prefixed_key, PH_READONLY)) {
		RETVAL_FALSE;
	} else {
		add_function(return_value, &cached_content, value);
		dao_update_property_array(getThis(), SL("_data"), &prefixed_key, return_value);
	}

	zval_ptr_dtor(&prefixed_key);
}

/**
 * Decrement of $keyName by given $value
 *
 * @param  string $keyName
 * @param  long $value
 * @return long
 */
PHP_METHOD(Dao_Cache_Backend_Memory, decrement){

	zval *key_name, *value = NULL, prefix = {}, prefixed_key = {}, data = {}, cached_content = {};

	dao_fetch_params(0, 1, 1, &key_name, &value);

	if (!value || Z_TYPE_P(value) == IS_NULL) {
		value = &DAO_GLOBAL(z_one);
	}

	dao_read_property(&prefix, getThis(), SL("_prefix"), PH_READONLY);
	DAO_CONCAT_VV(&prefixed_key, &prefix, key_name);

	dao_read_property(&data, getThis(), SL("_data"), PH_READONLY);
	if (!dao_array_isset_fetch(&cached_content, &data, &prefixed_key, PH_READONLY)) {
		RETVAL_FALSE;
	} else {
		dao_sub_function(return_value, &cached_content, value);
		dao_update_property_array(getThis(), SL("_data"), &prefixed_key, return_value);
	}
	zval_ptr_dtor(&prefixed_key);
}

/**
 * Immediately invalidates all existing items.
 *
 * @return boolean
 */
PHP_METHOD(Dao_Cache_Backend_Memory, flush){

	dao_update_property_null(getThis(), SL("_data"));

	RETURN_TRUE;
}
