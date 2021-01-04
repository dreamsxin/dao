
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
  |          Vladimir Kolesnikov <vladimir@extrememember.com>              |
  +------------------------------------------------------------------------+
*/

#include "cache/frontend/data.h"
#include "cache/frontend/igbinary.h"
#include "cache/frontendinterface.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"

/**
 * Dao\Cache\Frontend\Igbinary
 *
 * Allows to cache native PHP data in a serialized form using igbinary extension
 *
 *<code>
 *
 *	// Cache the files for 2 days using Igbinary frontend
 *	$frontCache = new Dao\Cache\Frontend\Igbinary(array(
 *		"lifetime" => 172800
 *	));
 *
 *	// Create the component that will cache "Igbinary" to a "File" backend
 *	// Set the cache file directory - important to keep the "/" at the end of
 *	// of the value for the folder
 *	$cache = new Dao\Cache\Backend\File($frontCache, array(
 *		"cacheDir" => "../app/cache/"
 *	));
 *
 *	// Try to get cached records
 *	$cacheKey  = 'robots_order_id.cache';
 *	$robots    = $cache->get($cacheKey);
 *	if ($robots === null) {
 *
 *		// $robots is null due to cache expiration or data do not exist
 *		// Make the database call and populate the variable
 *		$robots = Robots::find(array("order" => "id"));
 *
 *		// Store it in the cache
 *		$cache->save($cacheKey, $robots);
 *	}
 *
 *	// Use $robots :)
 *	foreach ($robots as $robot) {
 *		echo $robot->name, "\n";
 *	}
 *</code>
 */
zend_class_entry *dao_cache_frontend_igbinary_ce;

PHP_METHOD(Dao_Cache_Frontend_Igbinary, beforeStore);
PHP_METHOD(Dao_Cache_Frontend_Igbinary, afterRetrieve);

static const zend_function_entry dao_cache_frontend_igbinary_method_entry[] = {
	PHP_ME(Dao_Cache_Frontend_Igbinary, beforeStore, arginfo_dao_cache_frontendinterface_beforestore, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Frontend_Igbinary, afterRetrieve, arginfo_dao_cache_frontendinterface_afterretrieve, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Cache\Frontend\Igbinary initializer
 */
DAO_INIT_CLASS(Dao_Cache_Frontend_Igbinary){

	DAO_REGISTER_CLASS_EX(Dao\\Cache\\Frontend, Igbinary, cache_frontend_igbinary, dao_cache_frontend_data_ce, dao_cache_frontend_igbinary_method_entry, 0);

	zend_class_implements(dao_cache_frontend_igbinary_ce, 1, dao_cache_frontendinterface_ce);

	return SUCCESS;
}

/**
 * Serializes data before storing them
 *
 * @param mixed $data
 * @return string
 */
PHP_METHOD(Dao_Cache_Frontend_Igbinary, beforeStore)
{
	zval *data;

	dao_fetch_params(0, 1, 0, &data);
	
	DAO_RETURN_CALL_FUNCTION("igbinary_serialize", data);
}

/**
 * Unserializes data after retrieval
 *
 * @param mixed $data
 * @return mixed
 */
PHP_METHOD(Dao_Cache_Frontend_Igbinary, afterRetrieve)
{
	zval *data;

	dao_fetch_params(0, 1, 0, &data);
	
	DAO_RETURN_CALL_FUNCTION("igbinary_unserialize", data);
}
