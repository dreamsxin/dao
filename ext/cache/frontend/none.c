
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

#include "cache/frontend/none.h"
#include "cache/frontend/data.h"
#include "cache/frontendinterface.h"

#include "kernel/main.h"
#include "kernel/memory.h"

/**
 * Dao\Cache\Frontend\None
 *
 * Discards any kind of frontend data input. This frontend does not have expiration time or any other options
 *
 *<code>
 *
 *	//Create a None Cache
 *	$frontCache = new Dao\Cache\Frontend\None();
 *
 *	// Create the component that will cache "Data" to a "Memcached" backend
 *	// Memcached connection settings
 *	$cache = new Dao\Cache\Backend\Memcache($frontCache, array(
 *		"host" => "localhost",
 *		"port" => "11211"
 *	));
 *
 *	// This Frontend always return the data as it's returned by the backend
 *	$cacheKey = 'robots_order_id.cache';
 *	$robots    = $cache->get($cacheKey);
 *	if ($robots === null) {
 *
 *		// This cache doesn't perform any expiration checking, so the data is always expired
 *		// Make the database call and populate the variable
 *		$robots = Robots::find(array("order" => "id"));
 *
 *		$cache->save($cacheKey, $robots);
 *	}
 *
 *	// Use $robots :)
 *	foreach ($robots as $robot) {
 *		echo $robot->name, "\n";
 *	}
 *</code>
 */
zend_class_entry *dao_cache_frontend_none_ce;

PHP_METHOD(Dao_Cache_Frontend_None, getLifetime);
PHP_METHOD(Dao_Cache_Frontend_None, beforeStore);
PHP_METHOD(Dao_Cache_Frontend_None, afterRetrieve);

static const zend_function_entry dao_cache_frontend_none_method_entry[] = {
	PHP_ME(Dao_Cache_Frontend_None, getLifetime, arginfo_dao_cache_frontendinterface_getlifetime, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Frontend_None, beforeStore, arginfo_dao_cache_frontendinterface_beforestore, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Frontend_None, afterRetrieve, arginfo_dao_cache_frontendinterface_afterretrieve, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Cache\Frontend\None initializer
 */
DAO_INIT_CLASS(Dao_Cache_Frontend_None){

	DAO_REGISTER_CLASS_EX(Dao\\Cache\\Frontend, None, cache_frontend_none, dao_cache_frontend_data_ce, dao_cache_frontend_none_method_entry, 0);

	zend_class_implements(dao_cache_frontend_none_ce, 1, dao_cache_frontendinterface_ce);

	return SUCCESS;
}

/**
 * Returns cache lifetime, always one second expiring content
 *
 * @return int
 */
PHP_METHOD(Dao_Cache_Frontend_None, getLifetime){


	RETURN_LONG(1);
}

/**
 * Prepare data to be stored
 *
 * @param mixed $data
 */
PHP_METHOD(Dao_Cache_Frontend_None, beforeStore){

	zval *data;

	dao_fetch_params(0, 1, 0, &data);
	RETURN_ZVAL(data, 1, 0);
}

/**
 * Prepares data to be retrieved to user
 *
 * @param mixed $data
 */
PHP_METHOD(Dao_Cache_Frontend_None, afterRetrieve){

	zval *data;

	dao_fetch_params(0, 1, 0, &data);
	RETURN_ZVAL(data, 1, 0);
}
