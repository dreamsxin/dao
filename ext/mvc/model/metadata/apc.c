
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

#include "mvc/model/metadata/apc.h"
#include "mvc/model/metadata.h"
#include "mvc/model/metadatainterface.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/object.h"
#include "kernel/concat.h"
#include "kernel/fcall.h"
#include "kernel/hash.h"

/**
 * Dao\Mvc\Model\MetaData\Apc
 *
 * Stores model meta-data in the APC cache. Data will erased if the web server is restarted
 *
 * By default meta-data is stored for 48 hours (172800 seconds)
 *
 * You can query the meta-data by printing apc_fetch('$PMM$') or apc_fetch('$PMM$my-app-id')
 *
 *<code>
 *	$metaData = new Dao\Mvc\Model\Metadata\Apc(array(
 *		'prefix' => 'my-app-id',
 *		'lifetime' => 86400
 *	));
 *</code>
 */
zend_class_entry *dao_mvc_model_metadata_apc_ce;

PHP_METHOD(Dao_Mvc_Model_MetaData_Apc, __construct);
PHP_METHOD(Dao_Mvc_Model_MetaData_Apc, read);
PHP_METHOD(Dao_Mvc_Model_MetaData_Apc, write);
PHP_METHOD(Dao_Mvc_Model_MetaData_Apc, reset);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_metadata_apc___construct, 0, 0, 0)
	ZEND_ARG_INFO(0, options)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_mvc_model_metadata_apc_method_entry[] = {
	PHP_ME(Dao_Mvc_Model_MetaData_Apc, __construct, arginfo_dao_mvc_model_metadata_apc___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Mvc_Model_MetaData_Apc, read, arginfo_dao_mvc_model_metadatainterface_read, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_MetaData_Apc, write, arginfo_dao_mvc_model_metadatainterface_write, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_MetaData_Apc, reset, arginfo_dao_mvc_model_metadatainterface_reset, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Mvc\Model\MetaData\Apc initializer
 */
DAO_INIT_CLASS(Dao_Mvc_Model_MetaData_Apc){

	DAO_REGISTER_CLASS_EX(Dao\\Mvc\\Model\\MetaData, Apc, mvc_model_metadata_apc, dao_mvc_model_metadata_ce, dao_mvc_model_metadata_apc_method_entry, 0);

	zend_declare_property_string(dao_mvc_model_metadata_apc_ce, SL("_prefix"), "", ZEND_ACC_PROTECTED);
	zend_declare_property_long(dao_mvc_model_metadata_apc_ce, SL("_ttl"), 172800, ZEND_ACC_PROTECTED);

	zend_class_implements(dao_mvc_model_metadata_apc_ce, 1, dao_mvc_model_metadatainterface_ce);

	return SUCCESS;
}

/**
 * Dao\Mvc\Model\MetaData\Apc constructor
 *
 * @param array $options
 */
PHP_METHOD(Dao_Mvc_Model_MetaData_Apc, __construct){

	zval *options = NULL, prefix = {}, lifetime = {};

	dao_fetch_params(0, 0, 1, &options);

	if (options && Z_TYPE_P(options) == IS_ARRAY) {
		if (dao_array_isset_fetch_str(&prefix, options, SL("prefix"), PH_READONLY)) {
			dao_update_property(getThis(), SL("_prefix"), &prefix);
		}

		if (dao_array_isset_fetch_str(&lifetime, options, SL("lifetime"), PH_READONLY)) {
			dao_update_property(getThis(), SL("_ttl"), &lifetime);
		}
	}

	dao_update_property_empty_array(getThis(), SL("_metaData"));
}

/**
 * Reads meta-data from APC
 *
 * @param  string $key
 * @return array
 */
PHP_METHOD(Dao_Mvc_Model_MetaData_Apc, read){

	zval *key, prefix = {}, apc_key = {};

	dao_fetch_params(0, 1, 0, &key);

	dao_read_property(&prefix, getThis(), SL("_prefix"), PH_NOISY|PH_READONLY);

	DAO_CONCAT_SVV(&apc_key, "$PMM$", &prefix, key);

	DAO_CALL_FUNCTION(return_value, "apc_fetch", &apc_key);
	zval_ptr_dtor(&apc_key);
}

/**
 * Writes the meta-data to APC
 *
 * @param string $key
 * @param array $data
 */
PHP_METHOD(Dao_Mvc_Model_MetaData_Apc, write){

	zval *key, *data, prefix = {}, apc_key = {}, ttl = {};

	dao_fetch_params(0, 2, 0, &key, &data);

	dao_read_property(&prefix, getThis(), SL("_prefix"), PH_NOISY|PH_READONLY);

	DAO_CONCAT_SVV(&apc_key, "$PMM$", &prefix, key);

	dao_read_property(&ttl, getThis(), SL("_ttl"), PH_NOISY|PH_READONLY);
	DAO_CALL_FUNCTION(NULL, "apc_store", &apc_key, data, &ttl);
	zval_ptr_dtor(&apc_key);
}

PHP_METHOD(Dao_Mvc_Model_MetaData_Apc, reset){

	zval meta = {}, prefix = {};
	zend_string *str_key;
	ulong idx;

	dao_read_property(&meta, getThis(), SL("_metaData"), PH_NOISY|PH_READONLY);

	if (Z_TYPE(meta) == IS_ARRAY) {
		dao_read_property(&prefix, getThis(), SL("_prefix"), PH_NOISY|PH_READONLY);

		ZEND_HASH_FOREACH_KEY(Z_ARRVAL(meta), idx, str_key) {
			zval key = {}, real_key = {};
			if (str_key) {
				ZVAL_STR(&key, str_key);
			} else {
				ZVAL_LONG(&key, idx);
			}

			dao_concat_svsv(&real_key, SL("$PMM$"), &prefix, SL("meta-"), &key, 0);
			DAO_CALL_FUNCTION(NULL, "apc_delete", &real_key);
			zval_ptr_dtor(&real_key);
		} ZEND_HASH_FOREACH_END();
	}

	DAO_CALL_PARENT(NULL, dao_mvc_model_metadata_apc_ce, getThis(), "reset");
}
