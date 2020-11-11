
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

#include "cache/frontend/output.h"
#include "cache/frontendinterface.h"

#include "kernel/main.h"
#include "kernel/object.h"
#include "kernel/array.h"
#include "kernel/output.h"

/**
 * Dao\Cache\Frontend\Output
 *
 * Allows to cache output fragments captured with ob_* functions
 *
 *<code>
 *
 * //Create an Output frontend. Cache the files for 2 days
 * $frontCache = new Dao\Cache\Frontend\Output(array(
 *   "lifetime" => 172800
 * ));
 *
 * // Create the component that will cache from the "Output" to a "File" backend
 * // Set the cache file directory - it's important to keep the "/" at the end of
 * // the value for the folder
 * $cache = new Dao\Cache\Backend\File($frontCache, array(
 *     "cacheDir" => "../app/cache/"
 * ));
 *
 * // Get/Set the cache file to ../app/cache/my-cache.html
 * $content = $cache->start("my-cache.html");
 *
 * // If $content is null then the content will be generated for the cache
 * if ($content === null) {
 *
 *     //Print date and time
 *     echo date("r");
 *
 *     //Generate a link to the sign-up action
 *     echo Dao\Tag::linkTo(
 *         array(
 *             "user/signup",
 *             "Sign Up",
 *             "class" => "signup-button"
 *         )
 *     );
 *
 *     // Store the output into the cache file
 *     $cache->save();
 *
 * } else {
 *
 *     // Echo the cached output
 *     echo $content;
 * }
 *</code>
 */
zend_class_entry *dao_cache_frontend_output_ce;

PHP_METHOD(Dao_Cache_Frontend_Output, __construct);
PHP_METHOD(Dao_Cache_Frontend_Output, getLifetime);
PHP_METHOD(Dao_Cache_Frontend_Output, isBuffering);
PHP_METHOD(Dao_Cache_Frontend_Output, start);
PHP_METHOD(Dao_Cache_Frontend_Output, getContent);
PHP_METHOD(Dao_Cache_Frontend_Output, stop);
PHP_METHOD(Dao_Cache_Frontend_Output, beforeStore);
PHP_METHOD(Dao_Cache_Frontend_Output, afterRetrieve);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_cache_frontend_output___construct, 0, 0, 0)
	ZEND_ARG_INFO(0, frontendOptions)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_cache_frontend_output_method_entry[] = {
	PHP_ME(Dao_Cache_Frontend_Output, __construct, arginfo_dao_cache_frontend_output___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Cache_Frontend_Output, getLifetime, arginfo_dao_cache_frontendinterface_getlifetime, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Frontend_Output, isBuffering, arginfo_dao_cache_frontendinterface_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Frontend_Output, start, arginfo_dao_cache_frontendinterface_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Frontend_Output, getContent, arginfo_dao_cache_frontendinterface_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Frontend_Output, stop, arginfo_dao_cache_frontendinterface_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Frontend_Output, beforeStore, arginfo_dao_cache_frontendinterface_beforestore, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Frontend_Output, afterRetrieve, arginfo_dao_cache_frontendinterface_afterretrieve, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Cache\Frontend\Output initializer
 */
DAO_INIT_CLASS(Dao_Cache_Frontend_Output){

	DAO_REGISTER_CLASS(Dao\\Cache\\Frontend, Output, cache_frontend_output, dao_cache_frontend_output_method_entry, 0);

	zend_declare_property_bool(dao_cache_frontend_output_ce, SL("_buffering"), 0, ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_cache_frontend_output_ce, SL("_frontendOptions"), ZEND_ACC_PROTECTED);

	zend_class_implements(dao_cache_frontend_output_ce, 1, dao_cache_frontendinterface_ce);

	return SUCCESS;
}

/**
 * Dao\Cache\Frontend\Output constructor
 *
 * @param array $frontendOptions
 */
PHP_METHOD(Dao_Cache_Frontend_Output, __construct){

	zval *frontend_options = NULL;

	dao_fetch_params(0, 0, 1, &frontend_options);

	if (frontend_options) {
		dao_update_property(getThis(), SL("_frontendOptions"), frontend_options);
	}
}

/**
 * Returns cache lifetime
 *
 * @return integer
 */
PHP_METHOD(Dao_Cache_Frontend_Output, getLifetime){

	zval options = {}, lifetime = {};

	dao_read_property(&options, getThis(), SL("_frontendOptions"), PH_NOISY|PH_READONLY);
	if (dao_array_isset_fetch_str(&lifetime, &options, SL("lifetime"), PH_READONLY) && Z_TYPE(lifetime) == IS_LONG) {
		RETURN_CTOR(&lifetime);
	} else {
		RETURN_LONG(1);
	}
}

/**
 * Check whether if frontend is buffering output
 *
 * @return boolean
 */
PHP_METHOD(Dao_Cache_Frontend_Output, isBuffering){


	RETURN_MEMBER(getThis(), "_buffering");
}

/**
 * Starts output frontend
 */
PHP_METHOD(Dao_Cache_Frontend_Output, start){

	dao_update_property_bool(getThis(), SL("_buffering"), 1);
	dao_ob_start();
}

/**
 * Returns output cached content
 *
 * @return string
 */
PHP_METHOD(Dao_Cache_Frontend_Output, getContent){

	zval buffering = {};

	dao_read_property(&buffering, getThis(), SL("_buffering"), PH_NOISY|PH_READONLY);
	if (zend_is_true(&buffering)) {
		dao_ob_get_contents(return_value);
	}
}

/**
 * Stops output frontend
 */
PHP_METHOD(Dao_Cache_Frontend_Output, stop){

	zval buffering = {};

	dao_read_property(&buffering, getThis(), SL("_buffering"), PH_NOISY|PH_READONLY);
	if (zend_is_true(&buffering)) {
		dao_ob_end_clean();
	}

	dao_update_property_bool(getThis(), SL("_buffering"), 0);
}

/**
 * Prepare data to be stored
 *
 * @param mixed $data
 * @return mixed
 */
PHP_METHOD(Dao_Cache_Frontend_Output, beforeStore){

	zval *data;

	dao_fetch_params(0, 1, 0, &data);
	RETURN_ZVAL(data, 1, 0);
}

/**
 * Prepares data to be retrieved to user
 *
 * @param mixed $data
 * @return mixed
 */
PHP_METHOD(Dao_Cache_Frontend_Output, afterRetrieve){

	zval *data;

	dao_fetch_params(0, 1, 0, &data);
	RETURN_ZVAL(data, 1, 0);
}
