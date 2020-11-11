
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

#include "translate/adapter/nativearray.h"
#include "translate/adapter.h"
#include "translate/adapterinterface.h"
#include "translate/exception.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/exception.h"
#include "kernel/array.h"
#include "kernel/object.h"
#include "kernel/hash.h"
#include "kernel/concat.h"
#include "kernel/fcall.h"
#include "kernel/string.h"

/**
 * Dao\Translate\Adapter\NativeArray
 *
 * Allows to define translation lists using PHP arrays
 *
 */
zend_class_entry *dao_translate_adapter_nativearray_ce;

PHP_METHOD(Dao_Translate_Adapter_NativeArray, __construct);
PHP_METHOD(Dao_Translate_Adapter_NativeArray, query);
PHP_METHOD(Dao_Translate_Adapter_NativeArray, exists);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_translate_adapter_nativearray___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, options)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_translate_adapter_nativearray_method_entry[] = {
	PHP_ME(Dao_Translate_Adapter_NativeArray, __construct, arginfo_dao_translate_adapter_nativearray___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Translate_Adapter_NativeArray, query, arginfo_dao_translate_adapterinterface_query, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Translate_Adapter_NativeArray, exists, arginfo_dao_translate_adapterinterface_exists, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Translate\Adapter\NativeArray initializer
 */
DAO_INIT_CLASS(Dao_Translate_Adapter_NativeArray){

	DAO_REGISTER_CLASS_EX(Dao\\Translate\\Adapter, NativeArray, translate_adapter_nativearray, dao_translate_adapter_ce, dao_translate_adapter_nativearray_method_entry, 0);

	zend_declare_property_null(dao_translate_adapter_nativearray_ce, SL("_translate"), ZEND_ACC_PROTECTED);

	zend_class_implements(dao_translate_adapter_nativearray_ce, 1, dao_translate_adapterinterface_ce);
	return SUCCESS;
}

/**
 * Dao\Translate\Adapter\NativeArray constructor
 *
 * @param array $options
 */
PHP_METHOD(Dao_Translate_Adapter_NativeArray, __construct){

	zval *options, data = {};

	dao_fetch_params(0, 1, 0, &options);

	if (Z_TYPE_P(options) != IS_ARRAY) {
		DAO_THROW_EXCEPTION_STR(dao_translate_exception_ce, "Invalid options");
		return;
	}

	if (!dao_array_isset_fetch_str(&data, options, SL("content"), PH_READONLY)) {
		DAO_THROW_EXCEPTION_STR(dao_translate_exception_ce, "Translation content was not provided");
		return;
	}

	if (Z_TYPE(data) != IS_ARRAY) {
		DAO_THROW_EXCEPTION_STR(dao_translate_exception_ce, "Translation data must be an array");
		return;
	}

	dao_update_property(getThis(), SL("_translate"), &data);
}

/**
 * Returns the translation related to the given key
 *
 * @param string $index
 * @param array $placeholders
 * @return string
 */
PHP_METHOD(Dao_Translate_Adapter_NativeArray, query){

	zval *index, *placeholders = NULL, translate = {}, translation = {};
	zval *value;
	zend_string *str_key;
	ulong idx;

	dao_fetch_params(1, 1, 1, &index, &placeholders);

	if (!placeholders) {
		placeholders = &DAO_GLOBAL(z_null);
	}

	dao_read_property(&translate, getThis(), SL("_translate"), PH_NOISY|PH_READONLY);
	if (!dao_array_isset_fetch(&translation, &translate, index, PH_READONLY)) {
		ZVAL_COPY_VALUE(&translation, index);
	}

	if (Z_TYPE_P(placeholders) == IS_ARRAY) {
		ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(placeholders), idx, str_key, value) {
			zval key = {}, key_placeholder = {}, replaced = {};
			if (str_key) {
				ZVAL_STR(&key, str_key);
			} else {
				ZVAL_LONG(&key, idx);
			}

			DAO_CONCAT_SVS(&key_placeholder, "%", &key, "%");
			DAO_MM_ADD_ENTRY(&key_placeholder);

			DAO_STR_REPLACE(&replaced, &key_placeholder, value, &translation);
			DAO_MM_ADD_ENTRY(&replaced);

			ZVAL_COPY_VALUE(&translation, &replaced);
		} ZEND_HASH_FOREACH_END();
	}

	RETURN_MM_CTOR(&translation);
}

/**
 * Check whether is defined a translation key in the internal array
 *
 * @param string $index
 * @return boolean
 */
PHP_METHOD(Dao_Translate_Adapter_NativeArray, exists){

	zval *index, translate = {};

	dao_fetch_params(0, 1, 0, &index);

	dao_read_property(&translate, getThis(), SL("_translate"), PH_NOISY|PH_READONLY);
	if (dao_array_isset(&translate, index)) {
		RETURN_TRUE;
	}

	RETURN_FALSE;
}
