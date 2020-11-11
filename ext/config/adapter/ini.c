
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

#include "config/adapter/ini.h"
#include "config/adapter.h"
#include "config/adapterinterface.h"
#include "config/exception.h"
#include "pconfig.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"
#include "kernel/concat.h"
#include "kernel/exception.h"
#include "kernel/string.h"
#include "kernel/array.h"
#include "kernel/hash.h"
#include "kernel/operators.h"
#include "kernel/object.h"

/**
 * Dao\Config\Adapter\Ini
 *
 * Reads ini files and converts them to Dao\Config objects.
 *
 * Given the next configuration file:
 *
 *<code>
 *[database]
 *adapter = Mysql
 *host = localhost
 *username = scott
 *password = cheetah
 *dbname = test_db
 *
 *[phalcon]
 *controllersDir = "../app/controllers/"
 *modelsDir = "../app/models/"
 *viewsDir = "../app/views/"
 *</code>
 *
 * You can read it as follows:
 *
 *<code>
 *	$config = new Dao\Config\Adapter\Ini("path/config.ini");
 *	echo $config->phalcon->controllersDir;
 *	echo $config->database->username;
 *</code>
 *
 */
zend_class_entry *dao_config_adapter_ini_ce;

PHP_METHOD(Dao_Config_Adapter_Ini, __construct);
PHP_METHOD(Dao_Config_Adapter_Ini, read);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_config_adapter_ini___construct, 0, 0, 0)
	ZEND_ARG_INFO(0, filePath)
	ZEND_ARG_INFO(0, absolutePath)
	ZEND_ARG_INFO(0, scannerMode)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_config_adapter_ini_read, 0, 0, 1)
	ZEND_ARG_INFO(0, filePath)
	ZEND_ARG_INFO(0, absolutePath)
	ZEND_ARG_INFO(0, scannerMode)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_config_adapter_ini_method_entry[] = {
	PHP_ME(Dao_Config_Adapter_Ini, __construct, arginfo_dao_config_adapter_ini___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Config_Adapter_Ini, read, arginfo_dao_config_adapter_ini_read, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Config\Adapter\Ini initializer
 */
DAO_INIT_CLASS(Dao_Config_Adapter_Ini){

	DAO_REGISTER_CLASS_EX(Dao\\Config\\Adapter, Ini, config_adapter_ini, dao_config_adapter_ce, dao_config_adapter_ini_method_entry, 0);

	zend_class_implements(dao_config_adapter_ini_ce, 1, dao_config_adapterinterface_ce);

	return SUCCESS;
}

/**
 * Dao\Config\Adapter constructor
 *
 * @param string $filePath
 * @param string $absolutePath
 * @param string $scannerMode
 */
PHP_METHOD(Dao_Config_Adapter_Ini, __construct){

	zval *file_path = NULL, *absolute_path = NULL, *scanner_mode = NULL;

	dao_fetch_params(0, 0, 3, &file_path, &absolute_path, &scanner_mode);

	if (!absolute_path) {
		absolute_path = &DAO_GLOBAL(z_false);
	}

	if (file_path && Z_TYPE_P(file_path) != IS_NULL) {
		if (scanner_mode) {
			DAO_CALL_METHOD(NULL, getThis(), "read", file_path, absolute_path, scanner_mode);
		} else {
			DAO_CALL_METHOD(NULL, getThis(), "read", file_path, absolute_path);
		}
	}
}

static void dao_config_adapter_ini_update_zval_directive(zval *arr, zval *section, zval *directive, zval *value)
{
	zval t1 = {}, tmp = {}, index = {};
	int i, n;

	assert(Z_TYPE_P(arr) == IS_ARRAY);
	assert(Z_TYPE_P(directive) == IS_ARRAY);

	n = zend_hash_num_elements(Z_ARRVAL_P(directive));
	assert(n > 1);

	if (!dao_array_isset_fetch(&t1, arr, section, PH_READONLY)) {
		array_init(&t1);
		dao_array_update(arr, section, &t1, 0);
	} else if (Z_TYPE(t1) != IS_ARRAY) {
		convert_to_array_ex(&t1);
	}
	ZVAL_COPY_VALUE(&tmp, &t1);

	for (i = 0; i < n - 1; i++) {
		zval t2 = {};
		dao_array_fetch_long(&index, directive, i, PH_NOISY|PH_READONLY);
		if (dao_is_long_ex(&index)) {
			Z_TRY_ADDREF(index);
			convert_to_long(&index);
		}
		if (!dao_array_isset_fetch(&t2, &tmp, &index, PH_READONLY)) {
			array_init(&t2);
			dao_array_update(&tmp, &index, &t2, 0);
		} else if (Z_TYPE(t2) != IS_ARRAY) {
			convert_to_array_ex(&t2);
		}

		ZVAL_COPY_VALUE(&tmp, &t2);
	}

	dao_array_fetch_long(&index, directive, n - 1, PH_NOISY|PH_READONLY);
	dao_array_update(&tmp, &index, value, PH_COPY);
}

/**
 * Load config file
 *
 * @param string $filePath
 */
PHP_METHOD(Dao_Config_Adapter_Ini, read){

	zval *file_path, *absolute_path = NULL, *scanner_mode = NULL, config_dir_path = {}, base_path = {}, ini_config = {}, config = {}, *directives;
	zend_string *str_key;
	ulong idx;

	dao_fetch_params(0, 1, 2, &file_path, &absolute_path, &scanner_mode);
	DAO_ENSURE_IS_STRING(file_path);

	if (!absolute_path) {
		absolute_path = &DAO_GLOBAL(z_false);
	}

	if (zend_is_true(absolute_path)) {
		ZVAL_COPY(&config_dir_path, file_path);
	} else {
		dao_read_static_property_ce(&base_path, dao_config_adapter_ce, SL("_basePath"), PH_READONLY);

		DAO_CONCAT_VV(&config_dir_path, &base_path, file_path);
	}

	/**
	 * Use the standard parse_ini_file
	 */
	if (scanner_mode && Z_TYPE_P(scanner_mode) == IS_LONG) {
		DAO_CALL_FUNCTION(&ini_config, "parse_ini_file", &config_dir_path, &DAO_GLOBAL(z_true), scanner_mode);
	} else {
		DAO_CALL_FUNCTION(&ini_config, "parse_ini_file", &config_dir_path, &DAO_GLOBAL(z_true));
	}

	/**
	 * Check if the file had errors
	 */
	if (Z_TYPE(ini_config) != IS_ARRAY) {
		zend_throw_exception_ex(dao_config_exception_ce, 0, "Configuration file '%s' cannot be read", Z_STRVAL(config_dir_path));
		zval_ptr_dtor(&config_dir_path);
		return;
	}
	zval_ptr_dtor(&config_dir_path);

	array_init(&config);

	ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL(ini_config), idx, str_key, directives) {
		zval section = {}, *value;
		if (str_key) {
			ZVAL_STR(&section, str_key);
		} else {
			ZVAL_LONG(&section, idx);
		}

		if (unlikely(Z_TYPE_P(directives) != IS_ARRAY) || zend_hash_num_elements(Z_ARRVAL_P(directives)) == 0) {
			dao_array_update(&config, &section, directives, PH_COPY);
		} else {
			ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(directives), idx, str_key, value) {
				zval key = {}, directive_parts = {};
				if (str_key) {
					ZVAL_STR(&key, str_key);
				} else {
					ZVAL_LONG(&key, idx);
				}

				if (str_key && memchr(Z_STRVAL(key), '.', Z_STRLEN(key))) {
					dao_fast_explode_str(&directive_parts, SL("."), &key);
					dao_config_adapter_ini_update_zval_directive(&config, &section, &directive_parts, value);
					zval_ptr_dtor(&directive_parts);
				} else {
					dao_array_update_multi_2(&config, &section, &key, value, PH_COPY);
				}
			} ZEND_HASH_FOREACH_END();
		}
	} ZEND_HASH_FOREACH_END();
	zval_ptr_dtor(&ini_config);

	if (Z_TYPE(config) == IS_ARRAY) {
		DAO_CALL_METHOD(NULL, getThis(), "val", &config);
	}
	zval_ptr_dtor(&config);

	RETURN_THIS();
}
