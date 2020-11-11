
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
  +------------------------------------------------------------------------+
*/

#include "pconfig.h"
#include "config/exception.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/exception.h"
#include "kernel/hash.h"
#include "kernel/fcall.h"
#include "kernel/object.h"
#include "kernel/operators.h"
#include "kernel/array.h"

/**
 * Dao\Config
 *
 * Dao\Config is designed to simplify the access to, and the use of, configuration data within applications.
 * It provides a nested object property based user interface for accessing this configuration data within
 * application code.
 *
 *<code>
 *	$config = new Dao\Config(array(
 *		"database" => array(
 *			"adapter" => "Mysql",
 *			"host" => "localhost",
 *			"username" => "scott",
 *			"password" => "cheetah",
 *			"dbname" => "test_db"
 *		),
 *		"phalcon" => array(
 *			"controllersDir" => "../app/controllers/",
 *			"modelsDir" => "../app/models/",
 *			"viewsDir" => "../app/views/"
 *		)
 * ));
 *</code>
 *
 */
zend_class_entry *dao_config_ce;

PHP_METHOD(Dao_Config, __construct);
PHP_METHOD(Dao_Config, val);
PHP_METHOD(Dao_Config, offsetExists);
PHP_METHOD(Dao_Config, get);
PHP_METHOD(Dao_Config, offsetGet);
PHP_METHOD(Dao_Config, offsetSet);
PHP_METHOD(Dao_Config, offsetUnset);
PHP_METHOD(Dao_Config, merge);
PHP_METHOD(Dao_Config, toArray);
PHP_METHOD(Dao_Config, count);
PHP_METHOD(Dao_Config, setup);
PHP_METHOD(Dao_Config, __wakeup);
PHP_METHOD(Dao_Config, __set_state);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_config___construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, arrayConfig, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_config_val, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, arrayConfig, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_config_get, 0, 0, 1)
	ZEND_ARG_INFO(0, index)
	ZEND_ARG_INFO(0, defaultValue)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_config_merge, 0, 0, 1)
	ZEND_ARG_INFO(0, config)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_config_toarray, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_config_setup, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, options, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_config___wakeup, 0, 0, 0)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_config_method_entry[] = {
	PHP_ME(Dao_Config, __construct, arginfo_dao_config___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Config, val, arginfo_dao_config_val, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Config, offsetExists, arginfo_arrayaccess_offsetexists, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Config, get, arginfo_dao_config_get, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Config, offsetGet, arginfo_arrayaccess_offsetget, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Config, offsetSet, arginfo_arrayaccess_offsetset, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Config, offsetUnset, arginfo_arrayaccess_offsetunset, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Config, merge, arginfo_dao_config_merge, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Config, toArray, arginfo_dao_config_toarray, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Config, count, arginfo_countable_count, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Config, setup, arginfo_dao_config_setup, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Config, __wakeup, arginfo_dao_config___wakeup, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Config, __set_state, arginfo___set_state, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_MALIAS(Dao_Config, __get, offsetGet, arginfo___get, ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Config, __set, offsetSet, arginfo___set, ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Config, __isset, offsetExists, arginfo___isset, ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Config, __unset, offsetUnset, arginfo___unset, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Config initializer
 */
DAO_INIT_CLASS(Dao_Config){

	DAO_REGISTER_CLASS(Dao, Config, config, dao_config_method_entry, 0);

	zend_declare_property_bool(dao_config_ce, SL("_recursive"), 1, ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);

	zend_class_implements(dao_config_ce, 2, zend_ce_arrayaccess, spl_ce_Countable);

	return SUCCESS;
}

/**
 * Dao\Config constructor
 *
 * @param array $arrayConfig
 */
PHP_METHOD(Dao_Config, __construct){

	zval *array_config = NULL;

	dao_fetch_params(0, 0, 1, &array_config);

	if (array_config && Z_TYPE_P(array_config) == IS_ARRAY) {
		DAO_CALL_SELF(NULL, "val", array_config);
	}
}

/**
 * Sets values
 *
 * @param array $arrayConfig
 */
PHP_METHOD(Dao_Config, val){

	zval *array_config, *value, recursive = {};
	zend_string *str_key;
	ulong idx;

	dao_fetch_params(1, 1, 0, &array_config);

	dao_read_static_property_ce(&recursive, dao_config_ce, SL("_recursive"), PH_READONLY);

	ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(array_config), idx, str_key, value) {
		zval key = {}, instance = {};
		if (str_key) {
			ZVAL_STR(&key, str_key);
		} else {
			ZVAL_LONG(&key, idx);
		}

		if (Z_TYPE_P(value) == IS_ARRAY && zend_is_true(&recursive)) {
			object_init_ex(&instance, dao_config_ce);
			DAO_MM_ADD_ENTRY(&instance);
			DAO_MM_CALL_METHOD(NULL, &instance, "__construct", value);
			DAO_MM_CALL_METHOD(NULL, getThis(), "offsetset", &key, &instance);
		} else {
			DAO_MM_CALL_METHOD(NULL, getThis(), "offsetset", &key, value);
		}
	} ZEND_HASH_FOREACH_END();
	RETURN_MM();
}

/**
 * Allows to check whether an attribute is defined using the array-syntax
 *
 *<code>
 * var_dump(isset($config['database']));
 *</code>
 *
 * @param string $index
 * @return boolean
 */
PHP_METHOD(Dao_Config, offsetExists){

	zval *index;

	dao_fetch_params(0, 1, 0, &index);

	RETURN_BOOL(dao_isset_property_zval(getThis(), index));
}

/**
 * Gets an attribute from the configuration, if the attribute isn't defined returns null
 * If the value is exactly null or is not defined the default value will be used instead
 *
 *<code>
 * echo $config->get('controllersDir', '../app/controllers/');
 *</code>
 *
 * @param string $index
 * @param mixed $defaultValue
 * @return mixed
 */
PHP_METHOD(Dao_Config, get){

	zval *index, *default_value = NULL;

	dao_fetch_params(0, 1, 1, &index, &default_value);

	if (dao_read_property_zval(return_value, getThis(), index, PH_NOISY) == FAILURE) {
		if (default_value) {
			RETURN_CTOR(default_value);
		} else {
			RETURN_NULL();
		}
	}
}

/**
 * Gets an attribute using the array-syntax
 *
 *<code>
 * print_r($config['database']);
 *</code>
 *
 * @param string $index
 * @return string
 */
PHP_METHOD(Dao_Config, offsetGet){

	zval *index;

	dao_fetch_params(0, 1, 0, &index);
	DAO_ENSURE_IS_STRING(index);
	dao_read_property_zval(return_value, getThis(), index, PH_COPY);
}

/**
 * Sets an attribute using the array-syntax
 *
 *<code>
 * $config['database'] = array('type' => 'Sqlite');
 *</code>
 *
 * @param string $index
 * @param mixed $value
 */
PHP_METHOD(Dao_Config, offsetSet){

	zval *index, *value;

	dao_fetch_params(0, 2, 0, &index, &value);
	DAO_ENSURE_IS_STRING(index);
	dao_update_property_zval_zval(getThis(), index, value);
}

/**
 * Unsets an attribute using the array-syntax
 *
 *<code>
 * unset($config['database']);
 *</code>
 *
 * @param string $index
 */
PHP_METHOD(Dao_Config, offsetUnset){

	zval *index;

	dao_fetch_params(0, 1, 0, &index);
	DAO_ENSURE_IS_STRING(index);
	dao_update_property_zval_null(getThis(), index);
}

/**
 * Merges a configuration into the current one
 *
 * @brief void Dao\Config::merge(array|object $with)
 *
 *<code>
 *	$appConfig = new Dao\Config(array('database' => array('host' => 'localhost')));
 *	$globalConfig->merge($config2);
 *</code>
 *
 * @param Dao\Config $config
 * @return Dao\Config
 */
PHP_METHOD(Dao_Config, merge){

	zval *config, array_config = {}, *value;
	zend_string *str_key;
	ulong idx;

	dao_fetch_params(0, 1, 0, &config);

	if (Z_TYPE_P(config) != IS_OBJECT && Z_TYPE_P(config) != IS_ARRAY) {
		DAO_THROW_EXCEPTION_STR(dao_config_exception_ce, "Configuration must be an object or array");
		return;
	}

	if (Z_TYPE_P(config) == IS_OBJECT) {
		dao_get_object_vars(&array_config, config, 0);
	} else {
		ZVAL_COPY(&array_config, config);
	}

	if (Z_TYPE(array_config) == IS_ARRAY) {
		ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL(array_config), idx, str_key, value) {
			zval tmp = {}, active_value = {};
			if (str_key) {
				ZVAL_STR(&tmp, str_key);
			} else {
				ZVAL_LONG(&tmp, idx);
			}

			if (dao_property_isset_fetch_zval(&active_value, getThis(), &tmp, PH_READONLY)) {
				if ((Z_TYPE_P(value)  == IS_OBJECT || Z_TYPE_P(value) == IS_ARRAY) && Z_TYPE(active_value) == IS_OBJECT) {
					if (dao_method_exists_ex(&active_value, SL("merge")) == SUCCESS) { /* Path AAA in the test */
						zval *params[] = { value };
						if (FAILURE == dao_call_method(NULL, &active_value, "merge", 1, params)) {
							break;
						}
					} else {
						dao_update_property_zval_zval(getThis(), &tmp, value);
					}
				} else {
					dao_update_property_zval_zval(getThis(), &tmp, value);
				}
			} else {
				dao_update_property_zval_zval(getThis(), &tmp, value);
			}
		} ZEND_HASH_FOREACH_END();
	}
	zval_ptr_dtor(&array_config);

	RETURN_THIS();
}

/**
 * Converts recursively the object to an array
 *
 * @brief array Dao\Config::toArray(bool $recursive = true);
 *
 *<code>
 *	print_r($config->toArray());
 *</code>
 *
 * @param bool $recursive
 * @return array
 */
PHP_METHOD(Dao_Config, toArray){

	zval *recursive = NULL, *value, values = {};
	zend_string *key;
	ulong idx;

	dao_fetch_params(0, 0, 1, &recursive);

	if (!recursive) {
		recursive = &DAO_GLOBAL(z_true);
	}

	array_init(return_value);

	dao_get_object_vars(&values, getThis(), 1);

	ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL(values), idx, key, value) {
		zval tmp = {}, array_value = {};
		if (key) {
			ZVAL_STR(&tmp, key);
		} else {
			ZVAL_LONG(&tmp, idx);
		}
		if (zend_is_true(recursive) && Z_TYPE_P(value) == IS_OBJECT && dao_method_exists_ex(value, SL("toarray")) == SUCCESS) {
			if (SUCCESS == dao_call_method(&array_value, value, "toarray", 0, NULL)) {
				dao_array_update(return_value, &tmp, &array_value, 0);
			}
		}else {
			dao_array_update(return_value, &tmp, value, PH_COPY);
		}
	} ZEND_HASH_FOREACH_END();
	zval_ptr_dtor(&values);
}

PHP_METHOD(Dao_Config, count)
{
	zval arr = {};

	dao_get_object_vars(&arr, getThis(), 0);
	RETVAL_LONG(dao_fast_count_int(&arr));
	zval_ptr_dtor(&arr);
}

PHP_METHOD(Dao_Config, setup)
{
	zval *options, recursive = {};

	dao_fetch_params(0, 1, 0, &options);

	if (dao_array_isset_fetch_str(&recursive, options, SL("recursive"), PH_READONLY)) {
		dao_update_static_property_ce(dao_config_ce, SL("_recursive"), &recursive);
	}
}

PHP_METHOD(Dao_Config, __wakeup)
{
}

/**
 * Restores the state of a Dao\Config object
 *
 * @param array $data
 * @return Dao\Config
 */
PHP_METHOD(Dao_Config, __set_state){

	zval *data;

	dao_fetch_params(0, 1, 0, &data);

	object_init_ex(return_value, dao_config_ce);
	DAO_CALL_METHOD(NULL, return_value, "__construct", data);
}
