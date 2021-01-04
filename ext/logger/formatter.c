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

#include "logger/formatter.h"
#include "logger/formatterinterface.h"
#include "logger.h"

#include <main/spprintf.h>

#include "kernel/main.h"
#include "kernel/fcall.h"
#include "kernel/object.h"
#include "kernel/array.h"

/**
 * Dao\Logger\Formatter
 *
 * This is a base class for logger formatters
 */
zend_class_entry *dao_logger_formatter_ce;

PHP_METHOD(Dao_Logger_Formatter, setTypeStrings);
PHP_METHOD(Dao_Logger_Formatter, getTypeStrings);
PHP_METHOD(Dao_Logger_Formatter, setTypeString);
PHP_METHOD(Dao_Logger_Formatter, getTypeString);
PHP_METHOD(Dao_Logger_Formatter, interpolate);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_logger_formatter_settypestrings, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, types, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_logger_formatter_settypestring, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, type, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_logger_formatter_method_entry[] = {
	PHP_ME(Dao_Logger_Formatter, setTypeStrings, arginfo_dao_logger_formatter_settypestrings, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Logger_Formatter, getTypeStrings, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Logger_Formatter, setTypeString, arginfo_dao_logger_formatter_settypestring, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Logger_Formatter, getTypeString, arginfo_dao_logger_formatter_gettypestring, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Logger_Formatter, interpolate, arginfo_dao_logger_formatter_interpolate, ZEND_ACC_PROTECTED)
	PHP_FE_END
};

/**
 * Dao\Logger\Formatter initializer
 */
DAO_INIT_CLASS(Dao_Logger_Formatter){

	DAO_REGISTER_CLASS(Dao\\Logger, Formatter, logger_formatter, dao_logger_formatter_method_entry, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	zend_declare_property_null(dao_logger_formatter_ce, SL("_typeStrings"), ZEND_ACC_PROTECTED);

	zend_class_implements(dao_logger_formatter_ce, 1, dao_logger_formatterinterface_ce);

	return SUCCESS;
}

/**
 * Sets the string meaning of a logger constant
 *
 * @param array $types
 * @return Dao\Logger\Formatter
 */
PHP_METHOD(Dao_Logger_Formatter, setTypeStrings){

	zval *types, *v;
	zend_string *str_key;
	ulong idx;

	dao_fetch_params(0, 1, 0, &types);

	ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(types), idx, str_key, v) {
		zval key = {};
		if (str_key) {
			ZVAL_STR(&key, str_key);
		} else {
			ZVAL_LONG(&key, idx);
		}
		DAO_CALL_METHOD(NULL, getThis(), "settypestring", &key, v);
	} ZEND_HASH_FOREACH_END();

	RETURN_THIS();
}

/**
 * Returns the type strings
 *
 * @return array
 */
PHP_METHOD(Dao_Logger_Formatter, getTypeStrings){

	RETURN_MEMBER(getThis(), "_typeStrings");
}

/**
 * Sets the type strings
 *
 * @param string $type
 * @param string $name
 * @return Dao\Logger\Formatter
 */
PHP_METHOD(Dao_Logger_Formatter, setTypeString){

	zval *type, *name;

	dao_fetch_params(0, 2, 0, &type, &name);

	dao_update_property_array(getThis(), SL("_typeStrings"), type, name);
	RETURN_THIS();
}

/**
 * Returns the string meaning of a logger constant
 *
 * @param  integer $type
 * @return string
 */
PHP_METHOD(Dao_Logger_Formatter, getTypeString){

	zval *type, types = {};

	dao_fetch_params(0, 1, 0, &type);

	dao_read_property(&types, getThis(), SL("_typeStrings"), PH_NOISY|PH_READONLY);
	if (!dao_array_isset_fetch(return_value, &types, type, PH_COPY)) {
		DAO_CALL_CE_STATIC(return_value, dao_logger_ce, "gettypestring", type);
	}
}

/**
 * Interpolates context values into the message placeholders
 *
 * @see http://www.php-fig.org/psr/psr-3/ Section 1.2 Message
 * @param string $message
 * @param array $context
 */
PHP_METHOD(Dao_Logger_Formatter, interpolate)
{
	zval *message, *context, replace = {}, *val;
	zend_string *str_key;
	ulong idx;

	dao_fetch_params(0, 2, 0, &message, &context);

	if (Z_TYPE_P(context) == IS_ARRAY && zend_hash_num_elements(Z_ARRVAL_P(context)) > 0) {
		array_init(&replace);

		ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(context), idx, str_key, val) {
			zval index = {};
			char *tmp;
			uint str_length;

			if (str_key) {;
				str_length = spprintf(&tmp, 0, "{%s}", str_key->val);
				ZVAL_STRINGL(&index, tmp, str_length);
			} else {
				str_length = spprintf(&tmp, 0, "{%ld}", idx);
				ZVAL_STRINGL(&index, tmp, str_length);
			}
			efree(tmp);

			Z_TRY_ADDREF_P(val);
			zend_hash_add(Z_ARRVAL(replace), Z_STR(index), val);
			zval_ptr_dtor(&index);
		} ZEND_HASH_FOREACH_END();

		DAO_RETURN_CALL_FUNCTION("strtr", message, &replace);
		return;
	}

	RETURN_ZVAL(message, 1, 0);
}
