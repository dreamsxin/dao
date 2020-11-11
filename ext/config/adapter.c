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
  |          Vladimir Kolesnikov <vladimir@free-sevastopol.com>            |
  +------------------------------------------------------------------------+
*/

#include "pconfig.h"
#include "config/adapter.h"
#include "config/adapter/php.h"
#include "config/adapterinterface.h"

#include "kernel/main.h"
#include "kernel/object.h"
#include "kernel/string.h"
#include "kernel/fcall.h"
#include "kernel/array.h"
#include "kernel/operators.h"

/**
 * Dao\Config\Adapter
 *
 * Base class for Dao\Config adapters
 */
zend_class_entry *dao_config_adapter_ce;

PHP_METHOD(Dao_Config_Adapter, __construct);
PHP_METHOD(Dao_Config_Adapter, factory);
PHP_METHOD(Dao_Config_Adapter, setBasePath);
PHP_METHOD(Dao_Config_Adapter, getBasePath);
PHP_METHOD(Dao_Config_Adapter, load);

static const zend_function_entry dao_config_adapter_method_entry[] = {
	PHP_ME(Dao_Config_Adapter, __construct, arginfo_dao_config_adapter___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Config_Adapter, factory, arginfo_dao_config_adapter_factory, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Config_Adapter, setBasePath, arginfo_dao_config_adapterinterface_setbasepath, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Config_Adapter, getBasePath, arginfo_empty, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Config_Adapter, load, arginfo_dao_config_adapterinterface_load, ZEND_ACC_PUBLIC)

	ZEND_FENTRY(read, NULL, arginfo_dao_config_adapter_read, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
	PHP_FE_END
};

/**
 * Dao\Config\Adapter initializer
 */
DAO_INIT_CLASS(Dao_Config_Adapter){

	DAO_REGISTER_CLASS_EX(Dao\\Config, Adapter, config_adapter, dao_config_ce, dao_config_adapter_method_entry, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	zend_declare_property_string(dao_config_adapter_ce, SL("_basePath"), "", ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);
	zend_declare_property_null(dao_config_adapter_ce, SL("_instances"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);

	zend_class_implements(dao_config_adapter_ce, 1, dao_config_adapterinterface_ce);

	return SUCCESS;
}

/**
 * Dao\Config\Adapter constructor
 *
 * @param string $filePath
 * @param string $absolutePath
 */
PHP_METHOD(Dao_Config_Adapter, __construct){

	zval *file_path = NULL, *absolute_path = NULL;

	dao_fetch_params(0, 0, 2, &file_path, &absolute_path);

	if (absolute_path == NULL) {
		absolute_path = &DAO_GLOBAL(z_false);
	}

	if (file_path) {
		DAO_CALL_METHOD(NULL, getThis(), "read", file_path, absolute_path);
	}
}

/**
 * Dao\Config\Adapter factory
 *
 * @param string $filePath
 * @param string $absolutePath
 */
PHP_METHOD(Dao_Config_Adapter, factory){

	zval *file_path = NULL, *absolute_path = NULL, instances = {}, class_name = {};
	zend_class_entry *ce0;

	dao_fetch_params(0, 0, 2, &file_path, &absolute_path);

	dao_get_called_class(&class_name);
	ce0 = dao_fetch_class(&class_name, ZEND_FETCH_CLASS_DEFAULT);
	zval_ptr_dtor(&class_name);

	if (ce0 == dao_config_adapter_ce) {
		ce0 = dao_config_adapter_php_ce;
	}

	if (!file_path || DAO_IS_EMPTY(file_path)) {
		object_init_ex(return_value, ce0);
		DAO_CALL_METHOD(NULL, return_value, "__construct");
	} else {
		dao_read_static_property_ce(&instances, dao_config_adapter_ce, SL("_instances"), PH_READONLY);

		if (dao_array_isset_fetch(return_value, &instances, file_path, PH_COPY)) {
			return;
		}
		object_init_ex(return_value, ce0);

		if (absolute_path == NULL) {
			absolute_path = &DAO_GLOBAL(z_false);
		}

		DAO_CALL_METHOD(NULL, return_value, "__construct", file_path, absolute_path);

		dao_update_static_property_array_multi_ce(dao_config_adapter_ce, SL("_instances"), return_value, SL("z"), 1, file_path);
	}
}

/**
 * Sets base path
 *
 * @param string $basePath
 * @return Dao\Config\Adapter
 */
PHP_METHOD(Dao_Config_Adapter, setBasePath){

	zval *base_path, path = {};

	dao_fetch_params(0, 1, 0, &base_path);

	dao_add_trailing_slash(&path, base_path);
	dao_update_static_property_ce(dao_config_adapter_ce, SL("_basePath"), &path);
	zval_ptr_dtor(&path);
}

/**
 * Gets base path
 *
 * @return string
 */
PHP_METHOD(Dao_Config_Adapter, getBasePath){


	dao_read_static_property_ce(return_value, dao_config_adapter_ce, SL("_basePath"), PH_COPY);
}

/**
 * Load a configuration
 *
 * @param string $filePath
 * @param string $absolutePath
 */
PHP_METHOD(Dao_Config_Adapter, load){

	zval *file_path = NULL, *absolute_path = NULL;

	dao_fetch_params(0, 0, 2, &file_path, &absolute_path);

	if (file_path) {
		if (absolute_path == NULL) {
			absolute_path = &DAO_GLOBAL(z_false);
		}

		DAO_RETURN_CALL_METHOD(getThis(), "factory", file_path, absolute_path);
	} else {
		DAO_RETURN_CALL_METHOD(getThis(), "factory");
	}
}
