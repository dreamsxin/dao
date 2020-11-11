
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

#include "translate/adapter/gettext.h"
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
 * Dao\Translate\Adapter\Gettext
 *
 * Allows to define translation lists using gettext
 *
 *
 *<code>
 *  $t = new \Dao\Translate\Adapter\Gettext(array(
 *      'locale' => 'en_US.utf8',
 *      'defaultDomain' => 'messages',
 *      'directory' => __DIR__ . DIRECTORY_SEPARATOR . 'locale'
 * ));
 */
zend_class_entry *dao_translate_adapter_gettext_ce;

PHP_METHOD(Dao_Translate_Adapter_Gettext, __construct);
PHP_METHOD(Dao_Translate_Adapter_Gettext, query);
PHP_METHOD(Dao_Translate_Adapter_Gettext, exists);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_translate_adapter_gettext___construct, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, options, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_translate_adapter_gettext_method_entry[] = {
	PHP_ME(Dao_Translate_Adapter_Gettext, __construct, arginfo_dao_translate_adapter_gettext___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Translate_Adapter_Gettext, query, arginfo_dao_translate_adapterinterface_query, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Translate_Adapter_Gettext, exists, arginfo_dao_translate_adapterinterface_exists, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Translate\Adapter\Gettext initializer
 */
DAO_INIT_CLASS(Dao_Translate_Adapter_Gettext){

	DAO_REGISTER_CLASS_EX(Dao\\Translate\\Adapter, Gettext, translate_adapter_gettext, dao_translate_adapter_ce, dao_translate_adapter_gettext_method_entry, 0);

	zend_declare_property_null(dao_translate_adapter_gettext_ce, SL("_locale"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_translate_adapter_gettext_ce, SL("_defaultDomain"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_translate_adapter_gettext_ce, SL("_directory"), ZEND_ACC_PROTECTED);

	zend_class_implements(dao_translate_adapter_gettext_ce, 1, dao_translate_adapterinterface_ce);

	return SUCCESS;
}

/**
 * Dao\Translate\Adapter\Gettext constructor
 *
 * @param array $options
 * @throws \Dao\Translate\Exception
 */
PHP_METHOD(Dao_Translate_Adapter_Gettext, __construct){

	zval *options, locale, default_domain = {}, directory = {}, setting = {}, category = {}, *constant, *value;
	zend_string *str_key;
	ulong idx;

	dao_fetch_params(1, 1, 0, &options);

	if (!dao_array_isset_fetch_str(&locale, options, SL("locale"), PH_READONLY)) {
		DAO_MM_THROW_EXCEPTION_STR(dao_translate_exception_ce, "Parameter \"locale\" is required");
		return;
	}

	if (!dao_array_isset_fetch_str(&default_domain, options, SL("defaultDomain"), PH_READONLY)) {
		DAO_MM_THROW_EXCEPTION_STR(dao_translate_exception_ce, "Parameter \"defaultDomain\" is required");
		return;
	}

	if (!dao_array_isset_fetch_str(&directory, options, SL("directory"), PH_READONLY)) {
		DAO_MM_THROW_EXCEPTION_STR(dao_translate_exception_ce, "Parameter \"directory\" is required");
		return;
	}

	dao_update_property(getThis(), SL("_locale"), &locale);
	dao_update_property(getThis(), SL("_defaultDomain"), &default_domain);
	dao_update_property(getThis(), SL("_directory"), &directory);

	DAO_CONCAT_SV(&setting, "LC_ALL=", &locale);
	DAO_MM_ADD_ENTRY(&setting);
	DAO_MM_CALL_FUNCTION(NULL, "putenv", &setting);

	DAO_CONCAT_SV(&setting, "LANGUAGE=", &locale);
	DAO_MM_ADD_ENTRY(&setting);
	DAO_MM_CALL_FUNCTION(NULL, "putenv", &setting);

	if (!dao_array_isset_fetch_str(&category, options, SL("category"), PH_READONLY)) {
		/*
		if ((constant = zend_get_constant_str(SL("LC_MESSAGES"))) != NULL) {
			DAO_CALL_FUNCTION(NULL, "setlocale", constant, &locale);
		} else
		*/
		if ((constant = zend_get_constant_str(SL("LC_ALL"))) != NULL) {
			DAO_MM_CALL_FUNCTION(NULL, "setlocale", constant, &locale);
		}
	} else {
		DAO_MM_CALL_FUNCTION(NULL, "setlocale", &category, &locale);
	}

	if (Z_TYPE(directory) == IS_ARRAY) {
		ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL(directory), idx, str_key, value) {
			zval key = {};
			if (str_key) {
				ZVAL_STR(&key, str_key);
			} else {
				ZVAL_LONG(&key, idx);
			}
			DAO_MM_CALL_FUNCTION(NULL, "bindtextdomain", &key, value);
		} ZEND_HASH_FOREACH_END();
	} else {
		DAO_MM_CALL_FUNCTION(NULL, "bindtextdomain", &default_domain, &directory);
	}

	DAO_MM_CALL_FUNCTION(NULL, "textdomain", &default_domain);
	RETURN_MM();
}

/**
 * Returns the translation related to the given key
 *
 * @param string $index
 * @param array $placeholders
 * @param string $domain
 * @return string
 */
PHP_METHOD(Dao_Translate_Adapter_Gettext, query){

	zval *index, *placeholders = NULL, *domain = NULL, translation = {}, *value;
	zend_string *str_key;
	ulong idx;

	dao_fetch_params(1, 1, 2, &index, &placeholders, &domain);

	if (!domain) {
		DAO_MM_CALL_FUNCTION(&translation, "gettext", index);
	} else {
		DAO_MM_CALL_FUNCTION(&translation, "dgettext", domain, index);
	}
	DAO_MM_ADD_ENTRY(&translation);

	if (placeholders && Z_TYPE_P(placeholders) == IS_ARRAY && zend_hash_num_elements(Z_ARRVAL_P(placeholders))) {
		ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(placeholders), idx, str_key, value) {
			zval key_placeholder = {}, replaced = {}, key = {};
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
PHP_METHOD(Dao_Translate_Adapter_Gettext, exists){

	zval *index, *domain = NULL, translation = {};

	dao_fetch_params(1, 1, 1, &index, &domain);

	if (!domain) {
		DAO_MM_CALL_FUNCTION(&translation, "gettext", index);
	} else {
		DAO_MM_CALL_FUNCTION(&translation, "dgettext", domain, index);
	}
	DAO_MM_ADD_ENTRY(&translation);

	if (Z_STRLEN(translation) > 0) {
		RETURN_MM_TRUE;
	}

	RETURN_MM_FALSE;
}
