
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

#include "logger/formatter/json.h"
#include "logger/formatter.h"
#include "logger/formatterinterface.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/string.h"
#include "kernel/fcall.h"
#include "kernel/array.h"
#include "kernel/concat.h"
#include "kernel/object.h"

/**
 * Dao\Logger\Formatter\Json
 *
 * Formats messages using JSON encoding
 */
zend_class_entry *dao_logger_formatter_json_ce;

PHP_METHOD(Dao_Logger_Formatter_Json, __construct);
PHP_METHOD(Dao_Logger_Formatter_Json, format);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_logger_formatter_json___construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, options, IS_LONG, 1)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_logger_formatter_json_method_entry[] = {
	PHP_ME(Dao_Logger_Formatter_Json, __construct, arginfo_dao_logger_formatter_json___construct, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Logger_Formatter_Json, format, arginfo_dao_logger_formatterinterface_format, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Logger\Formatter\Json initializer
 */
DAO_INIT_CLASS(Dao_Logger_Formatter_Json){

	DAO_REGISTER_CLASS_EX(Dao\\Logger\\Formatter, Json, logger_formatter_json, dao_logger_formatter_ce, dao_logger_formatter_json_method_entry, 0);

	zend_declare_property_long(dao_logger_formatter_json_ce, SL("_options"), 0, ZEND_ACC_PROTECTED);

	zend_class_implements(dao_logger_formatter_json_ce, 1, dao_logger_formatterinterface_ce);

	return SUCCESS;
}

/**
 * Dao\Logger\Formatter\Json constructor
 *
 * @param int $options
 */
PHP_METHOD(Dao_Logger_Formatter_Json, __construct){

	zval *options = NULL;

	dao_fetch_params(0, 0, 1, &options);

	if (options && Z_TYPE_P(options) == IS_LONG) {
		dao_update_property(getThis(), SL("_options"), options);
	}

}

/**
 * Applies a format to a message before sent it to the internal log
 *
 * @param string $message
 * @param int $type
 * @param int $timestamp
 * @param array $context
 * @return string
 */
PHP_METHOD(Dao_Logger_Formatter_Json, format){

	zval *message, *type, *timestamp, *context, interpolated = {}, type_str = {}, log = {}, options = {}, json = {};

	dao_fetch_params(1, 4, 0, &message, &type, &timestamp, &context);

	if (Z_TYPE_P(context) == IS_ARRAY) {
		DAO_MM_CALL_METHOD(&interpolated, getThis(), "interpolate", message, context);
		DAO_MM_ADD_ENTRY(&interpolated);
	} else {
		ZVAL_COPY_VALUE(&interpolated, message);
	}

	DAO_MM_CALL_METHOD(&type_str, getThis(), "gettypestring", type);
	DAO_MM_ADD_ENTRY(&type_str);

	array_init_size(&log, 3);
	DAO_MM_ADD_ENTRY(&log);
	dao_array_update_str(&log, SL("type"), &type_str, PH_COPY);
	dao_array_update_str(&log, SL("message"), &interpolated, PH_COPY);
	dao_array_update_str(&log, SL("timestamp"), timestamp, PH_COPY);

	dao_read_property(&options, getThis(), SL("_options"), PH_READONLY);

	RETURN_MM_ON_FAILURE(dao_json_encode(&json, &log, Z_LVAL(options)));
	DAO_MM_ADD_ENTRY(&json);

	DAO_CONCAT_VS(return_value, &json, PHP_EOL);
	RETURN_MM();
}
