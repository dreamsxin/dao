
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

#include "cache/backend/redis.h"
#include "cache/backend.h"
#include "cache/backendinterface.h"
#include "cache/exception.h"

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
 * Dao\Cache\Backend\Redis
 *
 * Allows to cache output fragments, PHP data or raw data to a redis backend
 *
 * This adapter uses the special redisd key "_PHCR" to store all the keys internally used by the adapter
 *
 *<code>
 *
 * // Cache data for 2 days
 * $frontCache = new Dao\Cache\Frontend\Data(array(
 *    "lifetime" => 172800
 * ));
 *
 * //Create the Cache setting redisd connection options
 * $cache = new Dao\Cache\Backend\Redis($frontCache, array(
 *		'host' => 'localhost',
 *		'port' => 6379,
 *		'auth' => 'foobared',
 *  	'persistent' => false
 * ));
 *
 * //Cache arbitrary data
 * $cache->save('my-data', array(1, 2, 3, 4, 5));
 *
 * //Get data
 * $data = $cache->get('my-data');
 *
 *</code>
 */
zend_class_entry *dao_cache_backend_redis_ce;

PHP_METHOD(Dao_Cache_Backend_Redis, __construct);
PHP_METHOD(Dao_Cache_Backend_Redis, _connect);
PHP_METHOD(Dao_Cache_Backend_Redis, get);
PHP_METHOD(Dao_Cache_Backend_Redis, save);
PHP_METHOD(Dao_Cache_Backend_Redis, delete);
PHP_METHOD(Dao_Cache_Backend_Redis, queryKeys);
PHP_METHOD(Dao_Cache_Backend_Redis, exists);
PHP_METHOD(Dao_Cache_Backend_Redis, increment);
PHP_METHOD(Dao_Cache_Backend_Redis, decrement);
PHP_METHOD(Dao_Cache_Backend_Redis, flush);
PHP_METHOD(Dao_Cache_Backend_Redis, flushDb);
PHP_METHOD(Dao_Cache_Backend_Redis, getTrackingKey);
PHP_METHOD(Dao_Cache_Backend_Redis, setTrackingKey);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_cache_backend_redis___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, frontend)
	ZEND_ARG_TYPE_INFO(0, options, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_cache_backend_redis_settrackingkey, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_cache_backend_redis_method_entry[] = {
	PHP_ME(Dao_Cache_Backend_Redis, __construct, arginfo_dao_cache_backend_redis___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Cache_Backend_Redis, _connect, arginfo_empty, ZEND_ACC_PROTECTED)
	PHP_ME(Dao_Cache_Backend_Redis, get, arginfo_dao_cache_backendinterface_get, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Backend_Redis, save, arginfo_dao_cache_backendinterface_save, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Backend_Redis, delete, arginfo_dao_cache_backendinterface_delete, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Backend_Redis, queryKeys, arginfo_dao_cache_backendinterface_querykeys, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Backend_Redis, exists, arginfo_dao_cache_backendinterface_exists, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Backend_Redis, increment, arginfo_dao_cache_backendinterface_increment, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Backend_Redis, decrement, arginfo_dao_cache_backendinterface_decrement, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Backend_Redis, flush, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Backend_Redis, flushDb, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Backend_Redis, getTrackingKey, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Backend_Redis, setTrackingKey, arginfo_dao_cache_backend_redis_settrackingkey, ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Cache_Backend_Redis, set, save, arginfo_dao_cache_backendinterface_save, ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Cache_Backend_Redis, del, delete, arginfo_dao_cache_backendinterface_delete, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Cache\Backend\Redis initializer
 */
DAO_INIT_CLASS(Dao_Cache_Backend_Redis)
{
	DAO_REGISTER_CLASS_EX(Dao\\Cache\\Backend, Redis, cache_backend_redis, dao_cache_backend_ce, dao_cache_backend_redis_method_entry, 0);

	zend_declare_property_null(dao_cache_backend_redis_ce, SL("_redis"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_cache_backend_redis_ce, SL("_auth"), ZEND_ACC_PROTECTED);
	zend_declare_property_string(dao_cache_backend_redis_ce, SL("_prefix"), "_PHCR", ZEND_ACC_PROTECTED);

	zend_class_implements(dao_cache_backend_redis_ce, 1, dao_cache_backendinterface_ce);

	return SUCCESS;
}

/**
 * Dao\Cache\Backend\Redis constructor
 *
 * @param Dao\Cache\FrontendInterface $frontend
 * @param array $options
 */
PHP_METHOD(Dao_Cache_Backend_Redis, __construct){

	zval *frontend, *_options = NULL, options = {}, special_key = {}, redis = {};

	dao_fetch_params(1, 1, 1, &frontend, &_options);

	if (!_options || Z_TYPE_P(_options) == IS_NULL) {
		array_init_size(&options, 4);
	} else {
		ZVAL_DUP(&options, _options);
	}
	DAO_MM_ADD_ENTRY(&options);

	if (!dao_array_isset_fetch_str(&special_key, &options, SL("statsKey"), PH_READONLY) || Z_TYPE(special_key) == IS_TRUE) {
		dao_array_update_str_str(&options, SL("statsKey"), SL("_PHCR"), 0);
	}

	if (!dao_array_isset_fetch_str(&redis, &options, SL("redis"), PH_READONLY)) {
		if (!dao_array_isset_str(&options, SL("host"))) {
			dao_array_update_str_str(&options, SL("host"), SL("127.0.0.1"), 0);
		}

		if (!dao_array_isset_str(&options, SL("port"))) {
			dao_array_update_str_long(&options, SL("port"), 6379, 0);
		}

		if (!dao_array_isset_str(&options, SL("persistent"))) {
			dao_array_update_str_bool(&options, SL("persistent"), 0, 0);
		}
	} else {
		zend_class_entry *ce0;
		ce0 = dao_fetch_str_class(SL("Redis"), ZEND_FETCH_CLASS_AUTO);
		if (Z_TYPE(redis) == IS_STRING) {
			zval service = {};
			DAO_MM_CALL_METHOD(&service, getThis(), "getresolveservice", &redis);
			DAO_MM_ADD_ENTRY(&service);
			DAO_MM_VERIFY_CLASS(&service, ce0);
			dao_update_property(getThis(), SL("_redis"), &service);
		} else {
			DAO_MM_VERIFY_CLASS(&redis, ce0);
			dao_update_property(getThis(), SL("_redis"), &redis);
		}
	}

	DAO_MM_CALL_PARENT(NULL, dao_cache_backend_redis_ce, getThis(), "__construct", frontend, &options);
	RETURN_MM();
}

/**
 * Create internal connection to redisd
 */
PHP_METHOD(Dao_Cache_Backend_Redis, _connect)
{
	zval options = {}, redis = {}, host = {}, port = {}, persistent = {}, success = {}, auth = {}, db = {};
	zend_class_entry *ce0;

	DAO_MM_INIT();

	dao_read_property(&options, getThis(), SL("_options"), PH_READONLY);

	if (
		   !dao_array_isset_fetch_str(&host, &options, SL("host"), PH_READONLY)
		|| !dao_array_isset_fetch_str(&port, &options, SL("port"), PH_READONLY)
		|| !dao_array_isset_fetch_str(&persistent, &options, SL("persistent"), PH_READONLY)
	) {
		DAO_MM_THROW_EXCEPTION_STR(dao_cache_exception_ce, "Unexpected inconsistency in options");
		return;
	}

	ce0 = dao_fetch_str_class(SL("Redis"), ZEND_FETCH_CLASS_AUTO);

	object_init_ex(&redis, ce0);
	DAO_MM_ADD_ENTRY(&redis);
	if (dao_has_constructor(&redis)) {
		DAO_MM_CALL_METHOD(NULL, &redis, "__construct");
	}

	if (zend_is_true(&persistent)) {
		DAO_MM_CALL_METHOD(&success, &redis, "pconnect", &host, &port);
	} else {
		DAO_MM_CALL_METHOD(&success, &redis, "connect", &host, &port);
	}
	DAO_MM_ADD_ENTRY(&success);

	if (!zend_is_true(&success)) {
		DAO_MM_THROW_EXCEPTION_STR(dao_cache_exception_ce, "Cannot connect to Redisd server");
		return;
	}

	if (dao_array_isset_fetch_str(&auth, &options, SL("auth"), PH_READONLY) && DAO_IS_NOT_EMPTY(&auth)) {
		DAO_MM_CALL_METHOD(&success, &redis, "auth", &auth);
		DAO_MM_ADD_ENTRY(&success);
		if (!zend_is_true(&success)) {
			DAO_MM_THROW_EXCEPTION_STR(dao_cache_exception_ce, "Redisd server is authentication failed");
			return;
		}
	}

	if (dao_array_isset_fetch_str(&db, &options, SL("db"), PH_READONLY) && Z_TYPE(db) != IS_NULL) {
		DAO_MM_CALL_METHOD(NULL, &redis, "select", &db);
	}

	dao_update_property(getThis(), SL("_redis"), &redis);
	RETURN_MM_CTOR(&redis);
}

/**
 * Returns a cached content
 *
 * @param int|string $keyName
 * @return mixed
 */
PHP_METHOD(Dao_Cache_Backend_Redis, get){

	zval *key_name, redis = {}, frontend = {}, prefix = {}, prefixed_key = {}, cached_content = {};

	dao_fetch_params(1, 1, 0, &key_name);

	dao_read_property(&redis, getThis(), SL("_redis"), PH_READONLY);
	if (Z_TYPE(redis) != IS_OBJECT) {
		DAO_MM_CALL_METHOD(&redis, getThis(), "_connect");
		DAO_MM_ADD_ENTRY(&redis);
	}

	dao_read_property(&frontend, getThis(), SL("_frontend"), PH_READONLY);
	dao_read_property(&prefix, getThis(), SL("_prefix"), PH_READONLY);

	DAO_CONCAT_VV(&prefixed_key, &prefix, key_name);
	DAO_MM_ADD_ENTRY(&prefixed_key);

	DAO_MM_CALL_METHOD(&cached_content, &redis, "get", &prefixed_key);
	DAO_MM_ADD_ENTRY(&cached_content);
	if (DAO_IS_FALSE(&cached_content)) {
		RETURN_MM_NULL();
	}

	if (dao_is_numeric(&cached_content)) {
		RETURN_MM_CTOR(&cached_content);
	}

	DAO_RETURN_CALL_METHOD(&frontend, "afterretrieve", &cached_content);
	RETURN_MM();
}

/**
 * Stores cached content into the Redisd backend and stops the frontend
 *
 * @param int|string $keyName
 * @param string $content
 * @param long $lifetime
 * @param boolean $stopBuffer
 */
PHP_METHOD(Dao_Cache_Backend_Redis, save){

	zval *key_name = NULL, *content = NULL, *lifetime = NULL, *stop_buffer = NULL, key = {}, prefix = {}, prefixed_key = {}, cached_content = {}, prepared_content = {}, success = {};
	zval ttl = {}, is_buffering = {}, frontend = {}, redis = {}, options = {}, special_key = {};

	dao_fetch_params(1, 0, 4, &key_name, &content, &lifetime, &stop_buffer);
	dao_read_property(&prefix, getThis(), SL("_prefix"), PH_READONLY);

	if (!key_name || Z_TYPE_P(key_name) == IS_NULL) {
		dao_read_property(&key, getThis(), SL("_lastKey"), PH_READONLY);
		key_name = &key;
	}

	if (!zend_is_true(key_name)) {
		DAO_MM_THROW_EXCEPTION_STR(dao_cache_exception_ce, "The cache must be started first");
		return;
	}

	DAO_CONCAT_VV(&prefixed_key, &prefix, key_name);
	DAO_MM_ADD_ENTRY(&prefixed_key);

	/**
	 * Check if a connection is created or make a new one
	 */
	dao_read_property(&redis, getThis(), SL("_redis"), PH_READONLY);
	if (Z_TYPE(redis) != IS_OBJECT) {
		DAO_MM_CALL_METHOD(&redis, getThis(), "_connect");
		DAO_MM_ADD_ENTRY(&redis);
	}

	dao_read_property(&frontend, getThis(), SL("_frontend"), PH_READONLY);
	if (!content || Z_TYPE_P(content) == IS_NULL) {
		DAO_MM_CALL_METHOD(&cached_content, &frontend, "getcontent");
	} else {
		ZVAL_COPY(&cached_content, content);
	}
	DAO_MM_ADD_ENTRY(&cached_content);

	/**
	 * Take the lifetime from the frontend or read it from the set in start()
	 */
	if (!lifetime || Z_TYPE_P(lifetime) != IS_LONG) {
		DAO_MM_CALL_METHOD(&ttl, getThis(), "getlifetime");
		DAO_MM_ADD_ENTRY(&ttl);
	} else {
		ZVAL_COPY_VALUE(&ttl, lifetime);
	}

	if (dao_is_numeric(&cached_content)) {
		DAO_MM_CALL_METHOD(&success, &redis, "set", &prefixed_key, &cached_content, &ttl);
		DAO_MM_ADD_ENTRY(&success);
	} else {
		/**
		 * Prepare the content in the frontend
		 */
		DAO_MM_CALL_METHOD(&prepared_content, &frontend, "beforestore", &cached_content);
		DAO_MM_ADD_ENTRY(&prepared_content);
		DAO_MM_CALL_METHOD(&success, &redis, "set", &prefixed_key, &prepared_content, &ttl);
		DAO_MM_ADD_ENTRY(&success);
	}

	if (!zend_is_true(&success)) {
		DAO_MM_THROW_EXCEPTION_STR(dao_cache_exception_ce, "Failed to store data in redisd");
		return;
	}

	if (!zend_is_true(&success)) {
		DAO_MM_THROW_EXCEPTION_STR(dao_cache_exception_ce, "Failed to store data in redisd");
		return;
	}

	dao_read_property(&options, getThis(), SL("_options"), PH_READONLY);

	if (dao_array_isset_fetch_str(&special_key, &options, SL("statsKey"), PH_READONLY) && DAO_IS_NOT_EMPTY_STRING(&special_key)) {
		DAO_MM_CALL_METHOD(NULL, &redis, "sadd", &special_key, &prefixed_key);
	}

	DAO_MM_CALL_METHOD(&is_buffering, &frontend, "isbuffering");
	DAO_MM_ADD_ENTRY(&is_buffering);

	if (!stop_buffer || DAO_IS_TRUE(stop_buffer)) {
		DAO_MM_CALL_METHOD(NULL, &frontend, "stop");
	}

	if (DAO_IS_TRUE(&is_buffering)) {
		zend_print_zval(&cached_content, 0);
	}

	dao_update_property_bool(getThis(), SL("_started"), 0);
	RETURN_MM_TRUE;
}

/**
 * Deletes a value from the cache by its key
 *
 * @param int|string $keyName
 * @return boolean
 */
PHP_METHOD(Dao_Cache_Backend_Redis, delete){

	zval *key_name, redis = {}, prefix = {}, prefixed_key = {}, options = {}, special_key = {}, ret = {};

	dao_fetch_params(1, 1, 0, &key_name);

	dao_read_property(&redis, getThis(), SL("_redis"), PH_READONLY);
	if (Z_TYPE(redis) != IS_OBJECT) {
		DAO_MM_CALL_METHOD(&redis, getThis(), "_connect");
		DAO_MM_ADD_ENTRY(&redis);
	}

	dao_read_property(&prefix, getThis(), SL("_prefix"), PH_READONLY);

	DAO_CONCAT_VV(&prefixed_key, &prefix, key_name);
	DAO_MM_ADD_ENTRY(&prefixed_key);

	dao_read_property(&options, getThis(), SL("_options"), PH_READONLY);

	if (dao_array_isset_fetch_str(&special_key, &options, SL("statsKey"), PH_READONLY) && DAO_IS_NOT_EMPTY_STRING(&special_key)) {
		DAO_MM_CALL_METHOD(NULL, &redis, "srem", &special_key, &prefixed_key);
	}

	/* Delete the key from redisd */
	DAO_MM_CALL_METHOD(&ret, &redis, "delete", &prefixed_key);
	DAO_MM_ADD_ENTRY(&ret);
	if (zend_is_true(&ret)) {
		RETURN_MM_TRUE;
	}

	RETURN_MM_FALSE;
}

/**
 * Query the existing cached keys
 *
 * @param string $prefix
 * @return array
 */
PHP_METHOD(Dao_Cache_Backend_Redis, queryKeys){

	zval *prefix = NULL, redis = {}, options = {}, special_key = {}, keys = {}, *value;

	dao_fetch_params(1, 0, 1, &prefix);

	array_init(return_value);

	dao_read_property(&redis, getThis(), SL("_redis"), PH_READONLY);
	if (Z_TYPE(redis) != IS_OBJECT) {
		DAO_MM_CALL_METHOD(&redis, getThis(), "_connect");
		DAO_MM_ADD_ENTRY(&redis);
	}

	dao_read_property(&options, getThis(), SL("_options"), PH_READONLY);
	if (!dao_array_isset_fetch_str(&special_key, &options, SL("statsKey"), PH_READONLY) || !DAO_IS_NOT_EMPTY_STRING(&special_key)) {
		DAO_MM_THROW_EXCEPTION_STR(dao_cache_exception_ce, "Unexpected inconsistency in options");
		return;
	}

	/* Get the key from redisd */
	DAO_MM_CALL_METHOD(&keys, &redis, "smembers", &special_key);
	DAO_MM_ADD_ENTRY(&keys);
	if (Z_TYPE(keys) == IS_ARRAY) {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL(keys), value) {
			zval real_key = {};
			if (!prefix || !zend_is_true(prefix) || dao_start_with(value, prefix, NULL)) {
				ZVAL_NEW_STR(&real_key, Z_STR_P(value));
				dao_array_append(return_value, &real_key, PH_COPY);
			}
		} ZEND_HASH_FOREACH_END();
	}
	RETURN_MM();
}

/**
 * Checks if cache exists and it hasn't expired
 *
 * @param  string $keyName
 * @param  long $lifetime
 * @return boolean
 */
PHP_METHOD(Dao_Cache_Backend_Redis, exists){

	zval *key_name, value = {}, prefixed_key = {}, prefix = {}, redis = {};

	dao_fetch_params(1, 1, 0, &key_name);

	dao_read_property(&prefix, getThis(), SL("_prefix"), PH_READONLY);
	DAO_CONCAT_VV(&prefixed_key, &prefix, key_name);
	DAO_MM_ADD_ENTRY(&prefixed_key);

	dao_read_property(&redis, getThis(), SL("_redis"), PH_READONLY);
	if (Z_TYPE(redis) != IS_OBJECT) {
		DAO_MM_CALL_METHOD(&redis, getThis(), "_connect");
		DAO_MM_ADD_ENTRY(&redis);
	}

	DAO_MM_CALL_METHOD(&value, &redis, "get", &prefixed_key);
	DAO_MM_ADD_ENTRY(&value);

	RETVAL_BOOL(DAO_IS_NOT_FALSE(&value));
	RETURN_MM();
}

/**
 * Atomic increment of a given key, by number $value
 *
 * @param  string $keyName
 * @param  long $value
 * @return mixed
 */
PHP_METHOD(Dao_Cache_Backend_Redis, increment){

	zval *key_name, *value = NULL, redis = {}, prefixed_key = {}, prefix = {};

	dao_fetch_params(1, 1, 1, &key_name, &value);

	dao_read_property(&prefix, getThis(), SL("_prefix"), PH_READONLY);
	DAO_CONCAT_VV(&prefixed_key, &prefix, key_name);
	DAO_MM_ADD_ENTRY(&prefixed_key);

	if (!value || Z_TYPE_P(value) == IS_NULL) {
		value = &DAO_GLOBAL(z_one);
	} else if (Z_TYPE_P(value) != IS_LONG) {
		DAO_SEPARATE_PARAM(value);
		convert_to_long_ex(value);
	}

	dao_read_property(&redis, getThis(), SL("_redis"), PH_READONLY);
	if (Z_TYPE(redis) != IS_OBJECT) {
		DAO_MM_CALL_METHOD(&redis, getThis(), "_connect");
		DAO_MM_ADD_ENTRY(&redis);
	}

	DAO_MM_CALL_METHOD(return_value, &redis, "incrby", &prefixed_key, value);
	RETURN_MM();
}

/**
 * Atomic decrement of a given key, by number $value
 *
 * @param  string $keyName
 * @param  long $value
 * @return mixed
 */
PHP_METHOD(Dao_Cache_Backend_Redis, decrement){

	zval *key_name, *value = NULL, redis = {}, prefixed_key = {}, prefix = {};

	dao_fetch_params(1, 1, 1, &key_name, &value);

	dao_read_property(&prefix, getThis(), SL("_prefix"), PH_READONLY);
	DAO_CONCAT_VV(&prefixed_key, &prefix, key_name);
	DAO_MM_ADD_ENTRY(&prefixed_key);

	if (!value || Z_TYPE_P(value) == IS_NULL) {
		value = &DAO_GLOBAL(z_one);
	} else if (Z_TYPE_P(value) != IS_LONG) {
		DAO_SEPARATE_PARAM(value);
		convert_to_long_ex(value);
	}

	dao_read_property(&redis, getThis(), SL("_redis"), PH_READONLY);
	if (Z_TYPE(redis) != IS_OBJECT) {
		DAO_MM_CALL_METHOD(&redis, getThis(), "_connect");
		DAO_MM_ADD_ENTRY(&redis);
	}

	DAO_MM_CALL_METHOD(return_value, &redis, "decrby", &prefixed_key, value);
	RETURN_MM();
}

/**
 * Immediately invalidates all existing items.
 *
 * @return boolean
 */
PHP_METHOD(Dao_Cache_Backend_Redis, flush){

	zval redis = {}, options = {}, special_key = {}, keys = {}, *value;

	DAO_MM_INIT();

	dao_read_property(&options, getThis(), SL("_options"), PH_READONLY);

	if (!dao_array_isset_fetch_str(&special_key, &options, SL("statsKey"), PH_READONLY) || !DAO_IS_NOT_EMPTY_STRING(&special_key)) {
		DAO_MM_THROW_EXCEPTION_STR(dao_cache_exception_ce, "Unexpected inconsistency in options");
		return;
	}

	dao_read_property(&redis, getThis(), SL("_redis"), PH_READONLY);
	if (Z_TYPE(redis) != IS_OBJECT) {
		DAO_MM_CALL_METHOD(&redis, getThis(), "_connect");
		DAO_MM_ADD_ENTRY(&redis);
	}

	/* Get the key from redisd */
	DAO_MM_CALL_METHOD(&keys, &redis, "smembers", &special_key);
	DAO_MM_ADD_ENTRY(&keys);
	if (Z_TYPE(keys) == IS_ARRAY) {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL(keys), value) {
			DAO_MM_CALL_METHOD(NULL, &redis, "delete", value);
			DAO_MM_CALL_METHOD(NULL, &redis, "srem", &special_key, value);
		} ZEND_HASH_FOREACH_END();
	}

	RETURN_MM_TRUE;
}

/**
 * Remove all keys from the current database.
 *
 * @return boolean
 */
PHP_METHOD(Dao_Cache_Backend_Redis, flushDb){

	zval redis = {};

	DAO_MM_INIT();

	dao_read_property(&redis, getThis(), SL("_redis"), PH_READONLY);
	if (Z_TYPE(redis) != IS_OBJECT) {
		DAO_MM_CALL_METHOD(&redis, getThis(), "_connect");
		DAO_MM_ADD_ENTRY(&redis);
	}

	DAO_MM_CALL_METHOD(return_value, &redis, "flushdb");
	RETURN_MM();
}

PHP_METHOD(Dao_Cache_Backend_Redis, getTrackingKey)
{
	zval options = {};

	dao_read_property(&options, getThis(), SL("_options"), PH_READONLY);

	if (!dao_array_isset_fetch_str(return_value, &options, SL("statsKey"), PH_COPY)) {
		RETURN_NULL();
	}
}

PHP_METHOD(Dao_Cache_Backend_Redis, setTrackingKey)
{
	zval *key;

	dao_fetch_params(0, 1, 0, &key);

	dao_update_property_array_str(getThis(), SL("_options"), SL("statsKey"), key);

	RETURN_THIS();
}
