
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

#include "config/adapter/yaml.h"
#include "config/adapter.h"
#include "config/adapterinterface.h"
#include "pconfig.h"

#include "kernel/main.h"
#include "kernel/file.h"
#include "kernel/string.h"
#include "kernel/concat.h"
#include "kernel/fcall.h"
#include "kernel/object.h"

/**
 * Dao\Config\Adapter\Yaml
 *
 * Reads YAML files and converts them to Dao\Config objects.
 *
 * Given the following configuration file:
 *
 *<code>
 *phalcon
 *  baseuri: /phalcon/
 *models:
 *  metadata: memory
 *</code>
 *
 * You can read it as follows:
 *
 *<code>
 *	$config = new Dao\Config\Adapter\Yaml("path/config.yaml");
 *	echo $config->phalcon->baseuri;
 *	echo $config->models->metadata;
 *</code>
 *
 */
zend_class_entry *dao_config_adapter_yaml_ce;

PHP_METHOD(Dao_Config_Adapter_Yaml, read);

static const zend_function_entry dao_config_adapter_yaml_method_entry[] = {
	PHP_ME(Dao_Config_Adapter_Yaml, read, arginfo_dao_config_adapter_read, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Config\Adapter\Yaml initializer
 */
DAO_INIT_CLASS(Dao_Config_Adapter_Yaml){

	DAO_REGISTER_CLASS_EX(Dao\\Config\\Adapter, Yaml, config_adapter_yaml, dao_config_adapter_ce, dao_config_adapter_yaml_method_entry, 0);

	zend_class_implements(dao_config_adapter_yaml_ce, 1, dao_config_adapterinterface_ce);

	return SUCCESS;
}

/**
 * Load config file
 *
 * @param string $filePath
 */
PHP_METHOD(Dao_Config_Adapter_Yaml, read){

	zval *file_path, *absolute_path = NULL, config_dir_path = {}, base_path = {}, config = {};

	dao_fetch_params(0, 1, 1, &file_path, &absolute_path);
	DAO_ENSURE_IS_STRING(file_path);

	if (absolute_path == NULL) {
		absolute_path = &DAO_GLOBAL(z_false);
	}

	if (zend_is_true(absolute_path)) {
		ZVAL_COPY(&config_dir_path, file_path);
	} else {
		dao_read_static_property_ce(&base_path, dao_config_adapter_ce, SL("_basePath"), PH_READONLY);
		DAO_CONCAT_VV(&config_dir_path, &base_path, file_path);
	}

	DAO_CALL_FUNCTION(&config, "yaml_parse_file", &config_dir_path);
	zval_ptr_dtor(&config_dir_path);

	if (Z_TYPE(config) == IS_ARRAY) {
		DAO_CALL_METHOD(NULL, getThis(), "val", &config);
	}
	zval_ptr_dtor(&config);

	RETURN_THIS();
}
