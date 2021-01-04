
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

#include "cache/backend.h"
#include "cache/backendinterface.h"
#include "cache/frontendinterface.h"
#include "cache/exception.h"
#include "di/injectable.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/object.h"
#include "kernel/string.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "kernel/exception.h"

/**
 * Dao\Cache\Backend
 *
 * This class implements common functionality for backend adapters. A backend cache adapter may extend this class
 */
zend_class_entry *dao_cache_backend_ce;

PHP_METHOD(Dao_Cache_Backend, __construct);
PHP_METHOD(Dao_Cache_Backend, start);
PHP_METHOD(Dao_Cache_Backend, stop);
PHP_METHOD(Dao_Cache_Backend, getFrontend);
PHP_METHOD(Dao_Cache_Backend, getOptions);
PHP_METHOD(Dao_Cache_Backend, isFresh);
PHP_METHOD(Dao_Cache_Backend, isStarted);
PHP_METHOD(Dao_Cache_Backend, getLifetime);
PHP_METHOD(Dao_Cache_Backend, setPrefix);
PHP_METHOD(Dao_Cache_Backend, getPrefix);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_cache_backend___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, frontend)
	ZEND_ARG_TYPE_INFO(0, options, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_cache_backend_setprefix, 0, 0, 1)
	ZEND_ARG_INFO(0, prefix)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_cache_backend_method_entry[] = {
	PHP_ME(Dao_Cache_Backend, __construct, arginfo_dao_cache_backend___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Cache_Backend, start, arginfo_dao_cache_backendinterface_start, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Backend, stop, arginfo_dao_cache_backendinterface_stop, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Backend, getFrontend, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Backend, getOptions, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Backend, isFresh, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Backend, isStarted, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Backend, getLifetime, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Backend, setPrefix, arginfo_dao_cache_backend_setprefix, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Backend, getPrefix, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Cache\Backend initializer
 */
DAO_INIT_CLASS(Dao_Cache_Backend){

	DAO_REGISTER_CLASS_EX(Dao\\Cache, Backend, cache_backend, dao_di_injectable_ce, dao_cache_backend_method_entry, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	zend_declare_property_null(dao_cache_backend_ce, SL("_frontend"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_cache_backend_ce, SL("_options"), ZEND_ACC_PROTECTED);
	zend_declare_property_string(dao_cache_backend_ce, SL("_prefix"), "", ZEND_ACC_PROTECTED);
	zend_declare_property_string(dao_cache_backend_ce, SL("_lastKey"), "", ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_cache_backend_ce, SL("_lastLifetime"), ZEND_ACC_PROTECTED);
	zend_declare_property_bool(dao_cache_backend_ce, SL("_fresh"), 0, ZEND_ACC_PROTECTED);
	zend_declare_property_bool(dao_cache_backend_ce, SL("_started"), 0, ZEND_ACC_PROTECTED);

	zend_class_implements(dao_cache_backend_ce, 1, dao_cache_backendinterface_ce);

	return SUCCESS;
}

/**
 * Dao\Cache\Backend constructor
 *
 * @param Dao\Cache\FrontendInterface $frontend
 * @param array $options
 */
PHP_METHOD(Dao_Cache_Backend, __construct){

	zval *frontend, *options = NULL, prefix = {};

	dao_fetch_params(0, 1, 1, &frontend, &options);
	DAO_VERIFY_INTERFACE_EX(frontend, dao_cache_frontendinterface_ce, dao_cache_exception_ce);

	dao_update_property(getThis(), SL("_frontend"), frontend);

	if (options && Z_TYPE_P(options) == IS_ARRAY) {
		/**
		 * A common option is the prefix
		 */
		if (dao_array_isset_fetch_str(&prefix, options, SL("prefix"), PH_READONLY)) {
			if (dao_fast_strpos_str(NULL, &prefix, SL("#"))) {
				DAO_THROW_EXCEPTION_STR(dao_cache_exception_ce, "The prefix is currupted, can't contain `#`");
				return;
			}
			dao_update_property(getThis(), SL("_prefix"), &prefix);
		}

		dao_update_property(getThis(), SL("_options"), options);
	}
}

/**
 * Starts a cache. The $keyname allows to identify the created fragment
 *
 * @param int|string $keyName
 * @param long $lifetime
 * @return mixed
 */
PHP_METHOD(Dao_Cache_Backend, start){

	zval *key_name, *lifetime = NULL, *nobuffer = NULL, *fresh = NULL, frontend = {};

	dao_fetch_params(0, 1, 2, &key_name, &lifetime, &nobuffer);

	dao_update_property(getThis(), SL("_lastKey"), key_name);

	if (lifetime && Z_TYPE_P(lifetime) == IS_LONG) {
		dao_update_property(getThis(), SL("_lastLifetime"), lifetime);
	}
	DAO_CALL_METHOD(return_value, getThis(), "get", key_name);

	if (nobuffer && Z_TYPE_P(nobuffer) == IS_TRUE) {
		return;
	}

	if (Z_TYPE_P(return_value) == IS_NULL) {
		fresh = &DAO_GLOBAL(z_true);

		dao_read_property(&frontend, getThis(), SL("_frontend"), PH_NOISY|PH_READONLY);
		DAO_CALL_METHOD(NULL, &frontend, "start");
	} else {
		fresh = &DAO_GLOBAL(z_false);
	}

	dao_update_property(getThis(), SL("_fresh"), fresh);
	dao_update_property(getThis(), SL("_started"), &DAO_GLOBAL(z_true));
}

/**
 * Stops the frontend without store any cached content
 *
 * @param boolean $stopBuffer
 */
PHP_METHOD(Dao_Cache_Backend, stop){

	zval *stop_buffer = NULL, frontend = {};

	dao_fetch_params(0, 0, 1, &stop_buffer);

	if (!stop_buffer || DAO_IS_TRUE(stop_buffer)) {
		dao_read_property(&frontend, getThis(), SL("_frontend"), PH_NOISY|PH_READONLY);
		DAO_CALL_METHOD(NULL, &frontend, "stop");
	}

	dao_update_property_bool(getThis(), SL("_started"), 0);
}

/**
 * Returns front-end instance adapter related to the back-end
 *
 * @return mixed
 */
PHP_METHOD(Dao_Cache_Backend, getFrontend){


	RETURN_MEMBER(getThis(), "_frontend");
}

/**
 * Returns the backend options
 *
 * @return array
 */
PHP_METHOD(Dao_Cache_Backend, getOptions){


	RETURN_MEMBER(getThis(), "_options");
}

/**
 * Checks whether the last cache is fresh or cached
 *
 * @return boolean
 */
PHP_METHOD(Dao_Cache_Backend, isFresh){


	RETURN_MEMBER(getThis(), "_fresh");
}

/**
 * Checks whether the cache has starting buffering or not
 *
 * @return boolean
 */
PHP_METHOD(Dao_Cache_Backend, isStarted){


	RETURN_MEMBER(getThis(), "_started");
}

/**
 * Gets the last lifetime set
 *
 * @return int
 */
PHP_METHOD(Dao_Cache_Backend, getLifetime){

	zval lifetime = {}, frontend = {};

	dao_read_property(&lifetime, getThis(), SL("_lastLifetime"), PH_NOISY|PH_COPY);
	if (Z_TYPE(lifetime) != IS_LONG) {
		dao_read_property(&frontend, getThis(), SL("_frontend"), PH_NOISY|PH_READONLY);
		DAO_CALL_METHOD(&lifetime, &frontend, "getlifetime");
	}
	RETURN_ZVAL(&lifetime, 0, 0);
}

/**
 * Sets prefix
 *
 * @param string $prefix
 * @return Dao\Cache\Backend
 */
PHP_METHOD(Dao_Cache_Backend, setPrefix){

	zval *prefix;

	dao_fetch_params(0, 1, 0, &prefix);

	dao_update_property(getThis(), SL("_prefix"), prefix);

	RETURN_THIS();
}

/**
 * Gets prefix
 *
 * @return string
 */
PHP_METHOD(Dao_Cache_Backend, getPrefix){

	RETURN_MEMBER(getThis(), "_prefix");
}
