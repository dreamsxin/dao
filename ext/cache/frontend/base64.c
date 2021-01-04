
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

#include "cache/frontend/data.h"
#include "cache/frontend/base64.h"
#include "cache/frontendinterface.h"

#include "kernel/main.h"
#include "kernel/string.h"

/**
 * Dao\Cache\Frontend\Base64
 *
 * Allows to cache data converting/deconverting them to base64.
 *
 * This adapters uses the base64_encode/base64_decode PHP's functions
 *
 *<code>
 *
 * // Cache the files for 2 days using a Base64 frontend
 * $frontCache = new Dao\Cache\Frontend\Base64(array(
 *    "lifetime" => 172800
 * ));
 *
 * //Create a MongoDB cache
 * $cache = new Dao\Cache\Backend\Mongo($frontCache, array(
 *		'server' => "mongodb://localhost",
 *      'db' => 'caches',
 *		'collection' => 'images'
 * ));
 *
 * // Try to get cached image
 * $cacheKey = 'some-image.jpg.cache';
 * $image    = $cache->get($cacheKey);
 * if ($image === null) {
 *
 *     // Store the image in the cache
 *     $cache->save($cacheKey, file_get_contents('tmp-dir/some-image.jpg'));
 * }
 *
 * header('Content-Type: image/jpeg');
 * echo $image;
 *</code>
 */
zend_class_entry *dao_cache_frontend_base64_ce;

PHP_METHOD(Dao_Cache_Frontend_Base64, beforeStore);
PHP_METHOD(Dao_Cache_Frontend_Base64, afterRetrieve);

static const zend_function_entry dao_cache_frontend_base64_method_entry[] = {
	PHP_ME(Dao_Cache_Frontend_Base64, beforeStore, arginfo_dao_cache_frontendinterface_beforestore, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Frontend_Base64, afterRetrieve, arginfo_dao_cache_frontendinterface_afterretrieve, ZEND_ACC_PUBLIC)
	PHP_FE_END
};


/**
 * Dao\Cache\Frontend\Base64 initializer
 */
DAO_INIT_CLASS(Dao_Cache_Frontend_Base64){

	DAO_REGISTER_CLASS_EX(Dao\\Cache\\Frontend, Base64, cache_frontend_base64, dao_cache_frontend_data_ce, dao_cache_frontend_base64_method_entry, 0);

	zend_class_implements(dao_cache_frontend_base64_ce, 1, dao_cache_frontendinterface_ce);

	return SUCCESS;
}

/**
 * Serializes data before storing them
 *
 * @param mixed $data
 * @return string
 */
PHP_METHOD(Dao_Cache_Frontend_Base64, beforeStore){

	zval *data;

	dao_fetch_params(0, 1, 0, &data);
	dao_base64_encode(return_value, data);
}

/**
 * Unserializes data after retrieval
 *
 * @param mixed $data
 * @return mixed
 */
PHP_METHOD(Dao_Cache_Frontend_Base64, afterRetrieve){

	zval *data;

	dao_fetch_params(0, 1, 0, &data);
	dao_base64_decode(return_value, data);
}
