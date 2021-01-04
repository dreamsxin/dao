
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

#include "mvc/model/metadata/files.h"
#include "mvc/model/metadata.h"
#include "mvc/model/metadatainterface.h"
#include "mvc/model/exception.h"

#include <Zend/zend_smart_str.h>
#include <ext/standard/php_var.h>
#include <ext/spl/spl_directory.h>

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/file.h"
#include "kernel/concat.h"
#include "kernel/require.h"
#include "kernel/operators.h"
#include "kernel/exception.h"

/**
 * Dao\Mvc\Model\MetaData\Files
 *
 * Stores model meta-data in PHP files.
 *
 *<code>
 * $metaData = new \Dao\Mvc\Model\Metadata\Files(array(
 *    'metaDataDir' => 'app/cache/metadata/'
 * ));
 *</code>
 */
zend_class_entry *dao_mvc_model_metadata_files_ce;

PHP_METHOD(Dao_Mvc_Model_MetaData_Files, __construct);
PHP_METHOD(Dao_Mvc_Model_MetaData_Files, read);
PHP_METHOD(Dao_Mvc_Model_MetaData_Files, write);
PHP_METHOD(Dao_Mvc_Model_MetaData_Files, reset);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_metadata_files___construct, 0, 0, 0)
	ZEND_ARG_INFO(0, options)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_mvc_model_metadata_files_method_entry[] = {
	PHP_ME(Dao_Mvc_Model_MetaData_Files, __construct, arginfo_dao_mvc_model_metadata_files___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Mvc_Model_MetaData_Files, read, arginfo_dao_mvc_model_metadatainterface_read, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_MetaData_Files, write, arginfo_dao_mvc_model_metadatainterface_write, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_MetaData_Files, reset, arginfo_dao_mvc_model_metadatainterface_reset, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Mvc\Model\MetaData\Files initializer
 */
DAO_INIT_CLASS(Dao_Mvc_Model_MetaData_Files){

	DAO_REGISTER_CLASS_EX(Dao\\Mvc\\Model\\MetaData, Files, mvc_model_metadata_files, dao_mvc_model_metadata_ce, dao_mvc_model_metadata_files_method_entry, 0);

	zend_declare_property_string(dao_mvc_model_metadata_files_ce, SL("_metaDataDir"), "./", ZEND_ACC_PROTECTED);

	zend_class_implements(dao_mvc_model_metadata_files_ce, 1, dao_mvc_model_metadatainterface_ce);

	return SUCCESS;
}

/**
 * Dao\Mvc\Model\MetaData\Files constructor
 *
 * @param array $options
 */
PHP_METHOD(Dao_Mvc_Model_MetaData_Files, __construct){

	zval *options = NULL, meta_data_dir = {};

	dao_fetch_params(0, 0, 1, &options);

	if (options && Z_TYPE_P(options) == IS_ARRAY) {
		if (dao_array_isset_fetch_str(&meta_data_dir, options, SL("metaDataDir"), PH_READONLY)) {
			dao_update_property(getThis(), SL("_metaDataDir"), &meta_data_dir);
		}
	}

	dao_update_property_empty_array(getThis(), SL("_metaData"));
}

/**
 * Reads meta-data from files
 *
 * @param string $key
 * @return array
 */
PHP_METHOD(Dao_Mvc_Model_MetaData_Files, read){

	zval *key, meta_data_dir = {}, virtual_key = {}, path = {};

	dao_fetch_params(1, 1, 0, &key);
	DAO_ENSURE_IS_STRING(key);

	dao_read_property(&meta_data_dir, getThis(), SL("_metaDataDir"), PH_NOISY|PH_READONLY);

	dao_prepare_virtual_path_ex(&virtual_key, Z_STRVAL_P(key), Z_STRLEN_P(key), '_');
	DAO_MM_ADD_ENTRY(&virtual_key);
	DAO_CONCAT_VVS(&path, &meta_data_dir, &virtual_key, ".php");
	DAO_MM_ADD_ENTRY(&path);

	if (dao_file_exists(&path) == SUCCESS) {
		RETURN_MM_ON_FAILURE(dao_require_ret(return_value, Z_STRVAL(path)));
	}
	RETURN_MM();
}

/**
 * Writes the meta-data to files
 *
 * @param string $key
 * @param array $data
 */
PHP_METHOD(Dao_Mvc_Model_MetaData_Files, write){

	zval *key, *data, meta_data_dir = {}, virtual_key = {}, path = {}, php_export = {}, status = {};
	smart_str exp = { 0 };

	dao_fetch_params(1, 2, 0, &key, &data);

	dao_read_property(&meta_data_dir, getThis(), SL("_metaDataDir"), PH_NOISY|PH_READONLY);

	dao_prepare_virtual_path_ex(&virtual_key, Z_STRVAL_P(key), Z_STRLEN_P(key), '_');
	DAO_MM_ADD_ENTRY(&virtual_key);

	DAO_CONCAT_VVS(&path, &meta_data_dir, &virtual_key, ".php");
	DAO_MM_ADD_ENTRY(&path);

	smart_str_appends(&exp, "<?php return ");
	php_var_export_ex(data, 0, &exp);
	smart_str_appendc(&exp, ';');
	smart_str_0(&exp);

	ZVAL_STR(&php_export, exp.s);
	DAO_MM_ADD_ENTRY(&php_export);

	dao_file_put_contents(&status, &path, &php_export);

	if (DAO_IS_FALSE(&status)) {
		DAO_MM_THROW_EXCEPTION_STR(dao_mvc_model_exception_ce, "Meta-Data directory cannot be written");
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Dao_Mvc_Model_MetaData_Files, reset)
{
	zval metadata_dir = {}, pattern = {}, iterator = {};
	zend_object_iterator *it;

	DAO_MM_INIT();
	
	DAO_MM_CALL_PARENT(NULL, dao_mvc_model_metadata_files_ce, getThis(), "reset");

	dao_read_property(&metadata_dir, getThis(), SL("_metaDataDir"), PH_NOISY|PH_READONLY);

	DAO_CONCAT_VS(&pattern, &metadata_dir, "meta-*.php");
	DAO_MM_ADD_ENTRY(&pattern);
	object_init_ex(&iterator, spl_ce_GlobIterator);
	DAO_MM_ADD_ENTRY(&iterator);
	DAO_MM_CALL_METHOD(NULL, &iterator, "__construct", &pattern);

	it = spl_ce_GlobIterator->get_iterator(spl_ce_GlobIterator, &iterator, 0);
	it->funcs->rewind(it);
	while (SUCCESS == it->funcs->valid(it) && !EG(exception)) {
		zval itkey = {}, dummy = {};
		it->funcs->get_current_key(it, &itkey);
		dao_unlink(&dummy, &itkey);
		it->funcs->move_forward(it);
	}
	zend_iterator_dtor(it);
	//it->funcs->dtor(it);
	//efree(it);

	RETURN_MM();
}
