
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

#include "mvc/model/metadata/session.h"
#include "mvc/model/metadata.h"
#include "mvc/model/metadatainterface.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/object.h"
#include "kernel/concat.h"
#include "kernel/fcall.h"

/**
 * Dao\Mvc\Model\MetaData\Session
 *
 * Stores model meta-data in session. Data will erased when the session finishes.
 * Meta-data are permanent while the session is active.
 *
 * You can query the meta-data by printing $_SESSION['$PMM$']
 *
 *<code>
 * $metaData = new Dao\Mvc\Model\Metadata\Session(array(
 *    'prefix' => 'my-app-id'
 * ));
 *</code>
 */
zend_class_entry *dao_mvc_model_metadata_session_ce;

PHP_METHOD(Dao_Mvc_Model_MetaData_Session, __construct);
PHP_METHOD(Dao_Mvc_Model_MetaData_Session, read);
PHP_METHOD(Dao_Mvc_Model_MetaData_Session, write);
PHP_METHOD(Dao_Mvc_Model_MetaData_Session, reset);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_metadata_session___construct, 0, 0, 0)
	ZEND_ARG_INFO(0, options)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_mvc_model_metadata_session_method_entry[] = {
	PHP_ME(Dao_Mvc_Model_MetaData_Session, __construct, arginfo_dao_mvc_model_metadata_session___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Mvc_Model_MetaData_Session, read, arginfo_dao_mvc_model_metadatainterface_read, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_MetaData_Session, write, arginfo_dao_mvc_model_metadatainterface_write, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_MetaData_Session, reset, arginfo_dao_mvc_model_metadatainterface_reset, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Mvc\Model\MetaData\Session initializer
 */
DAO_INIT_CLASS(Dao_Mvc_Model_MetaData_Session){

	DAO_REGISTER_CLASS_EX(Dao\\Mvc\\Model\\MetaData, Session, mvc_model_metadata_session, dao_mvc_model_metadata_ce, dao_mvc_model_metadata_session_method_entry, 0);

	zend_declare_property_string(dao_mvc_model_metadata_session_ce, SL("_prefix"), "", ZEND_ACC_PROTECTED);

	zend_class_implements(dao_mvc_model_metadata_session_ce, 1, dao_mvc_model_metadatainterface_ce);

	return SUCCESS;
}

/**
 * Dao\Mvc\Model\MetaData\Session constructor
 *
 * @param array $options
 */
PHP_METHOD(Dao_Mvc_Model_MetaData_Session, __construct)
{
	zval *options = NULL, prefix = {};

	dao_fetch_params(0, 0, 1, &options);

	if (options && Z_TYPE_P(options) == IS_ARRAY) {
		if (dao_array_isset_fetch_str(&prefix, options, SL("prefix"), PH_READONLY)) {
			dao_update_property(getThis(), SL("_prefix"), &prefix);
		}
	}

	dao_update_property_empty_array(getThis(), SL("_metaData"));
}

/**
 * Reads meta-data from $_SESSION
 *
 * @param string $key
 * @return array
 */
PHP_METHOD(Dao_Mvc_Model_MetaData_Session, read){

	zval *key, *session, prefix = {}, prefix_key = {}, r0 = {}, r1 = {}, meta_data = {};

	dao_fetch_params(0, 1, 0, &key);

	session = dao_get_global_str(SL("_SESSION"));
	dao_read_property(&prefix, getThis(), SL("_prefix"), PH_NOISY|PH_READONLY);

	DAO_CONCAT_SV(&prefix_key, "$PMM$", &prefix);

	if (dao_array_isset_fetch(&r0, session, &prefix_key, 0)) {
		if (dao_array_isset(&r0, key)) {
			dao_array_fetch(&r1, session, &prefix_key, PH_NOISY|PH_READONLY);
			dao_array_fetch(&meta_data, &r1, key, PH_NOISY|PH_READONLY);
			zval_ptr_dtor(&prefix_key);
			RETURN_CTOR(&meta_data);
		}
	}
	zval_ptr_dtor(&prefix_key);
}

/**
 * Writes the meta-data to $_SESSION
 *
 * @param string $key
 * @param array $data
 */
PHP_METHOD(Dao_Mvc_Model_MetaData_Session, write){

	zval *key, *data, *_SESSION, prefix = {}, prefix_key = {};

	dao_fetch_params(0, 2, 0, &key, &data);

	_SESSION = dao_get_global_str(SL("_SESSION"));
	dao_read_property(&prefix, getThis(), SL("_prefix"), PH_NOISY|PH_READONLY);

	DAO_CONCAT_SV(&prefix_key, "$PMM$", &prefix);

	dao_array_update_multi_2(_SESSION, &prefix_key, key, data, PH_COPY);
}

PHP_METHOD(Dao_Mvc_Model_MetaData_Session, reset){

	zval *_SESSION, prefix = {}, prefix_key = {};

	_SESSION = dao_get_global_str(SL("_SESSION"));
	dao_read_property(&prefix, getThis(), SL("_prefix"), PH_NOISY|PH_READONLY);

	dao_concat_sv(&prefix_key, SL("$PMM$"), &prefix, 0);
	dao_array_unset(_SESSION, &prefix_key, 0);

	DAO_CALL_PARENT(NULL, dao_mvc_model_metadata_session_ce, getThis(), "reset");
}
