
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

#include "mvc/model/metadata/redis.h"
#include "mvc/model/metadata.h"
#include "mvc/model/metadatainterface.h"
#include "mvc/model/exception.h"
#include "cache/backend/redis.h"
#include "cache/frontend/data.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/exception.h"
#include "kernel/array.h"
#include "kernel/object.h"
#include "kernel/concat.h"
#include "kernel/fcall.h"
#include "kernel/hash.h"

/**
 * Dao\Mvc\Model\MetaData\Redis
 *
 * Stores model meta-data in the Redis cache. Data will erased if the web server is restarted
 *
 * By default meta-data is stored for 48 hours (172800 seconds)
 *
 * You can query the meta-data by printing redis_get('$PMM$') or redis_get('$PMM$my-app-id')
 *
 *<code>
 *	$metaData = new Dao\Mvc\Model\Metadata\Redis(array(
 *		'host' => 'localhost',
 *		'port' => 6379,
 *		'auth' => 'foobared',
 *		'persistent' => false
 *		'prefix' => 'my-app-id',
 *		'lifetime' => 86400
 *	));
 *</code>
 */
zend_class_entry *dao_mvc_model_metadata_redis_ce;

PHP_METHOD(Dao_Mvc_Model_MetaData_Redis, __construct);
PHP_METHOD(Dao_Mvc_Model_MetaData_Redis, read);
PHP_METHOD(Dao_Mvc_Model_MetaData_Redis, write);
PHP_METHOD(Dao_Mvc_Model_MetaData_Redis, reset);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_metadata_redis___construct, 0, 0, 0)
	ZEND_ARG_INFO(0, options)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_mvc_model_metadata_redis_method_entry[] = {
	PHP_ME(Dao_Mvc_Model_MetaData_Redis, __construct, arginfo_dao_mvc_model_metadata_redis___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Mvc_Model_MetaData_Redis, read, arginfo_dao_mvc_model_metadatainterface_read, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_MetaData_Redis, write, arginfo_dao_mvc_model_metadatainterface_write, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_MetaData_Redis, reset, arginfo_dao_mvc_model_metadatainterface_reset, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Mvc\Model\MetaData\Redis initializer
 */
DAO_INIT_CLASS(Dao_Mvc_Model_MetaData_Redis){

	DAO_REGISTER_CLASS_EX(Dao\\Mvc\\Model\\MetaData, Redis, mvc_model_metadata_redis, dao_mvc_model_metadata_ce, dao_mvc_model_metadata_redis_method_entry, 0);

	zend_declare_property_long(dao_mvc_model_metadata_redis_ce, SL("_lifetime"), 8600, ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_model_metadata_redis_ce, SL("_redis"), ZEND_ACC_PROTECTED);

	zend_class_implements(dao_mvc_model_metadata_redis_ce, 1, dao_mvc_model_metadatainterface_ce);

	return SUCCESS;
}

/**
 * Dao\Mvc\Model\MetaData\Redis constructor
 *
 * @param array $options
 */
PHP_METHOD(Dao_Mvc_Model_MetaData_Redis, __construct){

	zval *options, host = {}, port = {}, auth = {}, persistent = {}, lifetime = {}, prefix = {}, frontend_data = {}, redis = {}, frontend_option = {}, backend_option = {};


	dao_fetch_params(1, 1, 0, &options);

	if (Z_TYPE_P(options) != IS_ARRAY) {
		DAO_MM_THROW_EXCEPTION_STR(dao_mvc_model_exception_ce, "The options must be an array");
		return;
	}

	array_init(&backend_option);
	DAO_MM_ADD_ENTRY(&backend_option);

	dao_array_update_str_str(&backend_option, SL("statsKey"), SL("$PMM$"), 0);

	if (dao_array_isset_fetch_str(&host, options, SL("host"), PH_READONLY)) {
		dao_array_update_str(&backend_option, SL("host"), &host, PH_COPY);
	}

	if (dao_array_isset_fetch_str(&port, options, SL("port"), PH_READONLY)) {
		dao_array_update_str(&backend_option, SL("port"), &port, PH_COPY);
	}

	if (dao_array_isset_fetch_str(&auth, options, SL("auth"), PH_READONLY)) {
		dao_array_update_str(&backend_option, SL("auth"), &auth, PH_COPY);
	}

	if (dao_array_isset_fetch_str(&persistent, options, SL("persistent"), PH_READONLY)) {
		dao_array_update_str(&backend_option, SL("persistent"), &persistent, PH_COPY);
	}

	if (dao_array_isset_fetch_str(&lifetime, options, SL("lifetime"), PH_READONLY)) {
		dao_update_property(getThis(), SL("_lifetime"), &lifetime);
	} else {
		ZVAL_LONG(&lifetime, 8600);
	}

	if (dao_array_isset_fetch_str(&prefix, options, SL("prefix"), PH_READONLY)) {
		dao_array_update_str(&backend_option, SL("prefix"), &prefix, PH_COPY);
	}

	/* create redis instance */
	array_init_size(&frontend_option, 1);
	DAO_MM_ADD_ENTRY(&frontend_option);

	dao_array_update_str(&frontend_option, SL("lifetime"), &lifetime, PH_COPY);

	object_init_ex(&frontend_data, dao_cache_frontend_data_ce);
	DAO_MM_ADD_ENTRY(&frontend_data);
	DAO_MM_CALL_METHOD(NULL, &frontend_data, "__construct", &frontend_option);

	object_init_ex(&redis, dao_cache_backend_redis_ce);
	DAO_MM_ADD_ENTRY(&redis);
	DAO_MM_CALL_METHOD(NULL, &redis, "__construct", &frontend_data, &backend_option);

	dao_update_property(getThis(), SL("_redis"), &redis);
	dao_update_property_empty_array(getThis(), SL("_metaData"));
	RETURN_MM();
}

/**
 * Reads metadata from Redis
 *
 * @param  string $key
 * @return array
 */
PHP_METHOD(Dao_Mvc_Model_MetaData_Redis, read){

	zval *key, redis = {}, lifetime = {};

	dao_fetch_params(0, 1, 0, &key);

	dao_read_property(&redis, getThis(), SL("_redis"), PH_NOISY|PH_READONLY);

	if (Z_TYPE(redis) == IS_OBJECT) {
		dao_read_property(&lifetime, getThis(), SL("_lifetime"), PH_NOISY|PH_READONLY);
		DAO_RETURN_CALL_METHOD(&redis, "get", key, &lifetime);
		return;
	}

	RETURN_NULL();
}

/**
 *  Writes the metadata to Redis
 *
 * @param string $key
 * @param array $data
 */
PHP_METHOD(Dao_Mvc_Model_MetaData_Redis, write){

	zval *key, *data, redis = {}, lifetime = {};

	dao_fetch_params(0, 2, 0, &key, &data);

	dao_read_property(&redis, getThis(), SL("_redis"), PH_NOISY|PH_READONLY);

	if (Z_TYPE(redis) == IS_OBJECT) {
		dao_read_property(&lifetime, getThis(), SL("_lifetime"), PH_NOISY|PH_READONLY);
		DAO_CALL_METHOD(NULL, &redis, "save", key, data, &lifetime);
	}
}

PHP_METHOD(Dao_Mvc_Model_MetaData_Redis, reset){

	zval redis = {};

	dao_read_property(&redis, getThis(), SL("_redis"), PH_NOISY|PH_READONLY);

	if (Z_TYPE(redis) == IS_OBJECT) {
		DAO_CALL_METHOD(NULL, &redis, "flush");
	}

	DAO_CALL_PARENT(NULL, dao_mvc_model_metadata_redis_ce, getThis(), "reset");
}
