
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

#include "annotations/adapter/cache.h"
#include "annotations/adapter.h"
#include "annotations/adapterinterface.h"
#include "annotations/exception.h"
#include "cache/backendinterface.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/concat.h"
#include "kernel/fcall.h"
#include "kernel/string.h"
#include "kernel/array.h"
#include "kernel/object.h"
#include "kernel/exception.h"

/**
 * Dao\Annotations\Adapter\Cache
 *
 * Stores the parsed annotations in cache. This adapter is suitable for production
 *
 *<code>
 * $annotations = new \Dao\Annotations\Adapter\Cache();
 *</code>
 */
zend_class_entry *dao_annotations_adapter_cache_ce;

PHP_METHOD(Dao_Annotations_Adapter_Cache, __construct);
PHP_METHOD(Dao_Annotations_Adapter_Cache, read);
PHP_METHOD(Dao_Annotations_Adapter_Cache, write);


ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_annotations_adapter_cache___construct, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, options, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_annotations_adapter_cache_read, 0, 0, 1)
	ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_annotations_adapter_cache_write, 0, 0, 2)
	ZEND_ARG_INFO(0, key)
	ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_annotations_adapter_cache_method_entry[] = {
	PHP_ME(Dao_Annotations_Adapter_Cache, __construct, arginfo_dao_annotations_adapter_cache___construct, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Annotations_Adapter_Cache, read, arginfo_dao_annotations_adapter_cache_read, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Annotations_Adapter_Cache, write, arginfo_dao_annotations_adapter_cache_write, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Annotations\Adapter\Cache initializer
 */
DAO_INIT_CLASS(Dao_Annotations_Adapter_Cache){

	DAO_REGISTER_CLASS_EX(Dao\\Annotations\\Adapter, Cache, annotations_adapter_cache, dao_annotations_adapter_ce, dao_annotations_adapter_cache_method_entry, 0);

	zend_declare_property_long(dao_annotations_adapter_cache_ce, SL("_lifetime"), 8600, ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_annotations_adapter_cache_ce, SL("_cache"), ZEND_ACC_PROTECTED);

	zend_class_implements(dao_annotations_adapter_cache_ce, 1, dao_annotations_adapterinterface_ce);

	return SUCCESS;
}

/**
 * Constructor for Dao\Session\Adapter\Cache
 *
 * @param array $options
 */
PHP_METHOD(Dao_Annotations_Adapter_Cache, __construct){

	zval *options, service = {}, cache = {}, lifetime = {};

	dao_fetch_params(0, 1, 0, &options);

	if (!dao_array_isset_fetch_str(&service, options, SL("service"), PH_READONLY)) {
		DAO_THROW_EXCEPTION_STR(dao_annotations_exception_ce, "No service given in options");
		return;
	}

	if (Z_TYPE(service) != IS_OBJECT) {
		DAO_CALL_METHOD(&cache, getThis(), "getresolveservice", &service);
	} else {
		ZVAL_COPY_VALUE(&cache, &service);
	}
	DAO_VERIFY_INTERFACE(&cache, dao_cache_backendinterface_ce);

	dao_update_property(getThis(), SL("_cache"), &cache);

	if (dao_array_isset_fetch_str(&lifetime, options, SL("lifetime"), PH_READONLY)) {
		dao_update_property(getThis(), SL("_lifetime"), &lifetime);
	}

	dao_update_property(getThis(), SL("_cache"), &cache);
}

/**
 * Reads parsed annotations from cache
 *
 * @param string $key
 * @return Dao\Annotations\Reflection
 */
PHP_METHOD(Dao_Annotations_Adapter_Cache, read)
{
	zval *key, lifetime = {}, cache = {};

	dao_fetch_params(0, 1, 0, &key);

	dao_read_property(&lifetime, getThis(), SL("_lifetime"), PH_NOISY|PH_READONLY);
	dao_read_property(&cache, getThis(), SL("_cache"), PH_NOISY|PH_READONLY);

	DAO_RETURN_CALL_METHOD(&cache, "get", key, &lifetime);
}

/**
 * Writes parsed annotations to cache
 *
 * @param string $key
 * @param Dao\Annotations\Reflection $data
 */
PHP_METHOD(Dao_Annotations_Adapter_Cache, write){

	zval *key, *data, lifetime = {}, cache = {};

	dao_fetch_params(0, 2, 0, &key, &data);

	dao_read_property(&lifetime, getThis(), SL("_lifetime"), PH_NOISY|PH_READONLY);
	dao_read_property(&cache, getThis(), SL("_cache"), PH_NOISY|PH_READONLY);

	DAO_CALL_METHOD(NULL, &cache, "save", key, data, &lifetime);
}
