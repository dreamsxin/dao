
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
  |          Xinchen Hui <laruence@php.net>                                |
  +------------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "zend_smart_str.h"

#include "cache/yac.h"
#include "cache/yac/storage.h"
#include "cache/yac/serializer.h"
#include "cache/exception.h"
#include "di/injectable.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"

/**
 * Dao\Cache\Yac
 *
 * It can be used to replace APC or local memcached.
 */
zend_class_entry *dao_cache_yac_ce;

PHP_METHOD(Dao_Cache_Yac, __construct);
PHP_METHOD(Dao_Cache_Yac, add);
PHP_METHOD(Dao_Cache_Yac, set);
PHP_METHOD(Dao_Cache_Yac, get);
PHP_METHOD(Dao_Cache_Yac, delete);
PHP_METHOD(Dao_Cache_Yac, flush);
PHP_METHOD(Dao_Cache_Yac, dump);
PHP_METHOD(Dao_Cache_Yac, __set);
PHP_METHOD(Dao_Cache_Yac, __get);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_cache_yac___construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, prefix, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_cache_yac_set, 0, 0, 1)
	ZEND_ARG_INFO(0, keys)
	ZEND_ARG_INFO(0, value)
	ZEND_ARG_TYPE_INFO(0, lifetime, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_cache_yac_get, 0, 0, 1)
	ZEND_ARG_INFO(0, keys)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_cache_yac_delete, 0, 0, 1)
	ZEND_ARG_INFO(0, keys)
	ZEND_ARG_TYPE_INFO(0, lifetime, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_cache_yac_dump, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, limit, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_cache_yac___set, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_cache_yac___get, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_cache_yac_method_entry[] = {
	PHP_ME(Dao_Cache_Yac, __construct, arginfo_dao_cache_yac___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Cache_Yac, set, arginfo_dao_cache_yac_set, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Yac, get, arginfo_dao_cache_yac_get, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Yac, delete, arginfo_dao_cache_yac_delete, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Yac, flush, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Yac, dump, arginfo_dao_cache_yac_dump, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Yac, __set, arginfo_dao_cache_yac___set, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Yac, __get, arginfo_dao_cache_yac___get, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Cache\Yac initializer
 */
DAO_INIT_CLASS(Dao_Cache_Yac){

	DAO_REGISTER_CLASS(Dao\\Cache, Yac, cache_yac, dao_cache_yac_method_entry, 0);

	zend_declare_property_string(dao_cache_yac_ce, SL("_prefix"), "phshm_", ZEND_ACC_PROTECTED);

	return SUCCESS;
}

static int dao_cache_yac_add_impl(zend_string *prefix, zend_string *key, zval *value, int ttl, int add) /* {{{ */ {
	int ret = 0, flag = Z_TYPE_P(value);
	char *msg;
	time_t tv;
	zend_string *prefix_key;

	if ((ZSTR_LEN(key) + prefix->len) > DAO_CACHE_YAC_STORAGE_MAX_KEY_LEN) {
		php_error_docref(NULL, E_WARNING, "Key%s can not be longer than %d bytes",
				prefix->len? "(include prefix)" : "", DAO_CACHE_YAC_STORAGE_MAX_KEY_LEN);
		return ret;
	}

	if (prefix->len) {
		prefix_key = strpprintf(DAO_CACHE_YAC_STORAGE_MAX_KEY_LEN, "%s%s", ZSTR_VAL(prefix), ZSTR_VAL(key));
		key = prefix_key;
	}

	tv = time(NULL);
	switch (Z_TYPE_P(value)) {
		case IS_NULL:
		case IS_TRUE:
		case IS_FALSE:
			ret = dao_cache_yac_storage_update(ZSTR_VAL(key), ZSTR_LEN(key), (char *)&flag, sizeof(int), flag, ttl, add, tv);
			break;
		case IS_LONG:
			ret = dao_cache_yac_storage_update(ZSTR_VAL(key), ZSTR_LEN(key), (char *)&Z_LVAL_P(value), sizeof(long), flag, ttl, add, tv);
			break;
		case IS_DOUBLE:
			ret = dao_cache_yac_storage_update(ZSTR_VAL(key), ZSTR_LEN(key), (char *)&Z_DVAL_P(value), sizeof(double), flag, ttl, add, tv);
			break;
		case IS_STRING:
#if PHP_VERSION_ID >= 70200
		case IS_CONSTANT_AST:
#else
		case IS_CONSTANT:
#endif
			{
				if (Z_STRLEN_P(value) > DAO_CACHE_YAC_STORAGE_MAX_ENTRY_LEN) {
					php_error_docref(NULL, E_WARNING, "Value is too long(%lu bytes) to be stored", (unsigned long)Z_STRLEN_P(value));
					if (prefix->len) {
						zend_string_release(prefix_key);
					}
					return ret;
				} else {
					ret = dao_cache_yac_storage_update(ZSTR_VAL(key), ZSTR_LEN(key), Z_STRVAL_P(value), Z_STRLEN_P(value), flag, ttl, add, tv);
				}
			}
			break;
		case IS_ARRAY:
		case IS_OBJECT:
			{
				smart_str buf = {0};

				if (dao_cache_yac_serializer_php_pack(value, &buf, &msg))
				{
					if (buf.s->len > DAO_CACHE_YAC_STORAGE_MAX_ENTRY_LEN) {
						php_error_docref(NULL, E_WARNING, "Value is too big to be stored");
						if (prefix->len) {
							zend_string_release(prefix_key);
						}
						return ret;
					} else {
						ret = dao_cache_yac_storage_update(ZSTR_VAL(key), ZSTR_LEN(key), ZSTR_VAL(buf.s), ZSTR_LEN(buf.s), flag, ttl, add, tv);
					}
					smart_str_free(&buf);
				} else {
					php_error_docref(NULL, E_WARNING, "Serialization failed");
					smart_str_free(&buf);
				}
			}
			break;
		case IS_RESOURCE:
			php_error_docref(NULL, E_WARNING, "Type 'IS_RESOURCE' cannot be stored");
			break;
		default:
			php_error_docref(NULL, E_WARNING, "Unsupported valued type to be stored '%d'", flag);
			break;
	}

	if (prefix->len) {
		zend_string_release(prefix_key);
	}

	return ret;
}

static int dao_cache_yac_add_multi_impl(zend_string *prefix, zval *kvs, int ttl, int add) /* {{{ */ {
	HashTable *ht = Z_ARRVAL_P(kvs);
	zend_string *key;
	zend_ulong idx;
	zval *value;

	ZEND_HASH_FOREACH_KEY_VAL(ht, idx, key, value) {
		uint32_t should_free = 0;
		if (!key) {
			key = strpprintf(0, "%lu", idx);
			should_free = 1;
		}
		if (dao_cache_yac_add_impl(prefix, key, value, ttl, add)) {
			if (should_free) {
				zend_string_release(key);
			}
			continue;
		} else {
			if (should_free) {
				zend_string_release(key);
			}
			return 0;
		}
	} ZEND_HASH_FOREACH_END();

	return 1;
}

static zval * dao_cache_yac_get_impl(zend_string *prefix, zend_string *key, zval *rv) /* {{{ */ {
	uint32_t flag, size = 0;
	char *data, *msg;
	time_t tv;
	zend_string *prefix_key;

	if ((ZSTR_LEN(key) + prefix->len) > DAO_CACHE_YAC_STORAGE_MAX_KEY_LEN) {
		php_error_docref(NULL, E_WARNING, "Key%s can not be longer than %d bytes",
				prefix->len? "(include prefix)" : "", DAO_CACHE_YAC_STORAGE_MAX_KEY_LEN);
		return NULL;
	}

	if (prefix->len) {
		prefix_key = strpprintf(DAO_CACHE_YAC_STORAGE_MAX_KEY_LEN, "%s%s", ZSTR_VAL(prefix), ZSTR_VAL(key));
		key = prefix_key;
	}

	tv = time(NULL);
	if (dao_cache_yac_storage_find(ZSTR_VAL(key), ZSTR_LEN(key), &data, &size, &flag, tv)) {
		switch ((flag & DAO_CACHE_YAC_ENTRY_TYPE_MASK)) {
			case IS_NULL:
				if (size == sizeof(int)) {
					ZVAL_NULL(rv);
				}
				efree(data);
				break;
			case IS_TRUE:
				if (size == sizeof(int)) {
					ZVAL_TRUE(rv);
				}
				efree(data);
				break;
			case IS_FALSE:
				if (size == sizeof(int)) {
					ZVAL_FALSE(rv);
				}
				efree(data);
				break;
			case IS_LONG:
				if (size == sizeof(long)) {
					ZVAL_LONG(rv, *(long*)data);
				}
				efree(data);
				break;
			case IS_DOUBLE:
				if (size == sizeof(double)) {
					ZVAL_DOUBLE(rv, *(double*)data);
				}
				efree(data);
				break;
			case IS_STRING:
#if PHP_VERSION_ID >= 70200
			case IS_CONSTANT_AST:
#else
			case IS_CONSTANT:
#endif
				{
					ZVAL_STRINGL(rv, data, size);
					efree(data);
				}
				break;
			case IS_ARRAY:
			case IS_OBJECT:
				{
					rv = dao_cache_yac_serializer_php_unpack(data, size, &msg, rv);
					if (!rv) {
						php_error_docref(NULL, E_WARNING, "Unserialization failed");
					}
					efree(data);
				}
				break;
			default:
				php_error_docref(NULL, E_WARNING, "Unexpected valued type '%d'", flag);
				rv = NULL;
				break;
		}
	} else {
		rv = NULL;
	}

	if (prefix->len) {
		zend_string_release(prefix_key);
	}

	return rv;
}

static zval * dao_cache_yac_get_multi_impl(zend_string *prefix, zval *keys, zval *rv) {
	zval *value;
	HashTable *ht = Z_ARRVAL_P(keys);

	array_init(rv);

	ZEND_HASH_FOREACH_VAL(ht, value) {
		zval *v, tmp_rv;

		ZVAL_UNDEF(&tmp_rv);

		switch (Z_TYPE_P(value)) {
			case IS_STRING:
				if ((v = dao_cache_yac_get_impl(prefix, Z_STR_P(value), &tmp_rv)) && !Z_ISUNDEF(tmp_rv)) {
					zend_symtable_update(Z_ARRVAL_P(rv), Z_STR_P(value), v);
				} else {
					ZVAL_FALSE(&tmp_rv);
					zend_symtable_update(Z_ARRVAL_P(rv), Z_STR_P(value), &tmp_rv);
				}
				continue;
			default:
				{
					zend_string *key = zval_get_string(value);
					if ((v = dao_cache_yac_get_impl(prefix, key, &tmp_rv)) && !Z_ISUNDEF(tmp_rv)) {
						zend_symtable_update(Z_ARRVAL_P(rv), key, v);
					} else {
						ZVAL_FALSE(&tmp_rv);
						zend_symtable_update(Z_ARRVAL_P(rv), key, &tmp_rv);
					}
					zend_string_release(key);
				}
				continue;
		}
	} ZEND_HASH_FOREACH_END();

	return rv;
}

void dao_cache_yac_delete_impl(char *prefix, uint32_t prefix_len, char *key, uint32_t len, int ttl) {
	char buf[DAO_CACHE_YAC_STORAGE_MAX_KEY_LEN];
	time_t tv = 0;

	if ((len + prefix_len) > DAO_CACHE_YAC_STORAGE_MAX_KEY_LEN) {
		php_error_docref(NULL, E_WARNING, "Key%s can not be longer than %d bytes",
				prefix_len? "(include prefix)" : "", DAO_CACHE_YAC_STORAGE_MAX_KEY_LEN);
		return;
	}

	if (prefix_len) {
		len = snprintf(buf, sizeof(buf), "%s%s", prefix, key);
		key = (char *)buf;
	}

	if (ttl) {
		tv = (ulong)time(NULL);
	}

	dao_cache_yac_storage_delete(key, len, ttl, tv);
}

static void dao_cache_yac_delete_multi_impl(char *prefix, uint32_t prefix_len, zval *keys, int ttl) /* {{{ */ {
	HashTable *ht = Z_ARRVAL_P(keys);
	zval *value;

	ZEND_HASH_FOREACH_VAL(ht, value) {
		switch (Z_TYPE_P(value)) {
			case IS_STRING:
				dao_cache_yac_delete_impl(prefix, prefix_len, Z_STRVAL_P(value), Z_STRLEN_P(value), ttl);
				continue;
			default:
				{
					zval copy;
					zend_make_printable_zval(value, &copy);
					dao_cache_yac_delete_impl(prefix, prefix_len, Z_STRVAL(copy), Z_STRLEN(copy), ttl);
					zval_dtor(&copy);
				}
				continue;
		}
	} ZEND_HASH_FOREACH_END();
}

/**
 * Dao\Cache\Yac constructor
 *
 * @param string $prefix
 */
PHP_METHOD(Dao_Cache_Yac, __construct){

	zval *prefix = NULL;

	dao_fetch_params(0, 0, 1, &prefix);

	if (prefix && Z_TYPE_P(prefix) != IS_NULL) {
		if (Z_TYPE_P(prefix) != IS_STRING) {
			php_error_docref(NULL, E_WARNING, "prefix parameter must be an string");
			return;
		}

		dao_update_property(getThis(), SL("_prefix"), prefix);
	}
}

/**
 * Stores cached content
 *
 * @param string|array $keys
 * @param mixed $value
 * @param long $lifetime
 * @return Dao\Cache\Yac
 */
PHP_METHOD(Dao_Cache_Yac, set){

	zval *keys, *value = NULL, *lifetime = NULL, prefix = {};
	uint32_t ret;

	if (!DAO_GLOBAL(cache).enable_yac) {
		RETURN_FALSE;
	}

	dao_fetch_params(0, 1, 2, &keys, &value, &lifetime);

	if (!value) {
		value = &DAO_GLOBAL(z_null);
	}

	if (!lifetime) {
		lifetime = &DAO_GLOBAL(z_zero);
	}

	dao_read_property(&prefix, getThis(), SL("_prefix"), PH_NOISY|PH_READONLY);

	if (Z_TYPE_P(keys) == IS_ARRAY) {
		ret = dao_cache_yac_add_multi_impl(Z_STR(prefix), keys, Z_LVAL_P(lifetime), 0);
	} else if (Z_TYPE_P(keys) == IS_STRING) {
		ret = dao_cache_yac_add_impl(Z_STR(prefix), Z_STR_P(keys), value, Z_LVAL_P(lifetime), 0);
	} else {
		zval copy;
		zend_make_printable_zval(keys, &copy);
		ret = dao_cache_yac_add_impl(Z_STR(prefix), Z_STR(copy), value, Z_LVAL_P(lifetime), 0);
		zval_dtor(&copy);
	}

	RETURN_BOOL(ret);
}

/**
 * Returns a cached content
 *
 * @param string|array $keys
 * @return mixed
 */
PHP_METHOD(Dao_Cache_Yac, get)
{
	zval *keys, prefix = {}, *ret = NULL;

	if (!DAO_GLOBAL(cache).enable_yac) {
		RETURN_FALSE;
	}

	dao_fetch_params(0, 1, 0, &keys);

	dao_read_property(&prefix, getThis(), SL("_prefix"), PH_NOISY|PH_READONLY);

	if (Z_TYPE_P(keys) == IS_ARRAY) {
		ret = dao_cache_yac_get_multi_impl(Z_STR(prefix), keys, return_value);
	} else if (Z_TYPE_P(keys) == IS_STRING) {
		ret = dao_cache_yac_get_impl(Z_STR(prefix), Z_STR_P(keys), return_value);
	} else {
		zval copy;
		zend_make_printable_zval(keys, &copy);
		ret = dao_cache_yac_get_impl(Z_STR(prefix), Z_STR(copy), return_value);
		zval_dtor(&copy);
	}

	if (ret == NULL) {
		RETURN_FALSE;
	}
}

/**
 * Returns a cached content
 *
 * @param string|array $keys
 * @param long $lifetime
 * @return boolean
 */
PHP_METHOD(Dao_Cache_Yac, delete)
{
	zval *keys, *lifetime = NULL, prefix = {};

	if (!DAO_GLOBAL(cache).enable_yac) {
		RETURN_FALSE;
	}

	dao_fetch_params(0, 1, 1, &keys, &lifetime);

	if (!lifetime) {
		lifetime = &DAO_GLOBAL(z_zero);
	}

	dao_read_property(&prefix, getThis(), SL("_prefix"), PH_NOISY|PH_READONLY);

	if (Z_TYPE_P(keys) == IS_ARRAY) {
		dao_cache_yac_delete_multi_impl(Z_STRVAL(prefix), Z_STRLEN(prefix), keys, Z_LVAL_P(lifetime));
	} else if (Z_TYPE_P(keys) == IS_STRING) {
		dao_cache_yac_delete_impl(Z_STRVAL(prefix), Z_STRLEN(prefix), Z_STRVAL_P(keys), Z_STRLEN_P(keys), Z_LVAL_P(lifetime));
	} else {
		zval copy;
		zend_make_printable_zval(keys, &copy);
		dao_cache_yac_delete_impl(Z_STRVAL(prefix), Z_STRLEN(prefix), Z_STRVAL(copy), Z_STRLEN(copy), Z_LVAL_P(lifetime));
		zval_dtor(&copy);
	}

	RETURN_TRUE;
}

PHP_METHOD(Dao_Cache_Yac, flush) {

	if (!DAO_GLOBAL(cache).enable_yac) {
		RETURN_FALSE;
	}

	dao_cache_yac_storage_flush();

	RETURN_TRUE;
}

PHP_METHOD(Dao_Cache_Yac, dump)
{
	zval *_limit = NULL;
	dao_cache_yac_item_list *list, *l;
	long limit = 100;

	if (!DAO_GLOBAL(cache).enable_yac) {
		RETURN_FALSE;
	}

	dao_fetch_params(0, 0, 1, &_limit);

	if (_limit) {
		if (Z_TYPE_P(_limit) != IS_LONG) {
			php_error_docref(NULL, E_WARNING, "limit parameter must be an integer");
			return;
		}
		limit = Z_LVAL_P(_limit);
	}

	array_init(return_value);
	list = l = dao_cache_yac_storage_dump(limit);
	for (; l; l = l->next) {
		zval item;
		array_init(&item);
		add_assoc_long(&item, "index", l->index);
		add_assoc_long(&item, "hash", l->h);
		add_assoc_long(&item, "crc", l->crc);
		add_assoc_long(&item, "ttl", l->ttl);
		add_assoc_long(&item, "k_len", l->k_len);
		add_assoc_long(&item, "v_len", l->v_len);
		add_assoc_long(&item, "size", l->size);
		add_assoc_string(&item, "key", (char*)l->key);
		add_next_index_zval(return_value, &item);
	}

	dao_cache_yac_storage_free_list(list);
	return;
}

/**
 * Stores cached content
 *
 * @param string $key
 * @param mixed $value
 * @param long $lifetime
 */
PHP_METHOD(Dao_Cache_Yac, __set)
{
	zval *key, *value, prefix = {};

	if (!DAO_GLOBAL(cache).enable_yac) {
		RETURN_FALSE;
	}

	dao_fetch_params(0, 2, 0, &key, &value);

	dao_read_property(&prefix, getThis(), SL("_prefix"), PH_NOISY|PH_READONLY);

	dao_cache_yac_add_impl(Z_STR(prefix), Z_STR_P(key), value, 0, 0);
}

/**
 * Returns a cached content
 *
 * @param string $key
 * @return mixed
 */
PHP_METHOD(Dao_Cache_Yac, __get)
{
	zval *key, prefix = {}, *ret = NULL;

	if (!DAO_GLOBAL(cache).enable_yac) {
		RETURN_FALSE;
	}

	dao_fetch_params(0, 1, 0, &key);

	dao_read_property(&prefix, getThis(), SL("_prefix"), PH_NOISY|PH_READONLY);

	if (Z_TYPE_P(key) == IS_STRING) {
		ret = dao_cache_yac_get_impl(Z_STR(prefix), Z_STR_P(key), return_value);
	} else {
		zval copy;
		zend_make_printable_zval(key, &copy);
		ret = dao_cache_yac_get_impl(Z_STR(prefix), Z_STR(copy), return_value);
		zval_dtor(&copy);
	}

	if (ret == NULL) {
		RETURN_FALSE;
	}
}
