
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

#include "logger/formatter/line.h"
#include "logger/formatter.h"
#include "logger/formatterinterface.h"

#include <ext/date/php_date.h>

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/string.h"
#include "kernel/fcall.h"
#include "kernel/concat.h"

/**
 * Dao\Logger\Formatter\Line
 *
 * Formats messages using an one-line string
 */
zend_class_entry *dao_logger_formatter_line_ce;

PHP_METHOD(Dao_Logger_Formatter_Line, __construct);
PHP_METHOD(Dao_Logger_Formatter_Line, setFormat);
PHP_METHOD(Dao_Logger_Formatter_Line, getFormat);
PHP_METHOD(Dao_Logger_Formatter_Line, setDateFormat);
PHP_METHOD(Dao_Logger_Formatter_Line, getDateFormat);
PHP_METHOD(Dao_Logger_Formatter_Line, format);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_logger_formatter_line___construct, 0, 0, 0)
	ZEND_ARG_INFO(0, format)
	ZEND_ARG_INFO(0, dateFormat)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_logger_formatter_line_setformat, 0, 0, 1)
	ZEND_ARG_INFO(0, format)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_logger_formatter_line_setdateformat, 0, 0, 1)
	ZEND_ARG_INFO(0, date)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_logger_formatter_line_method_entry[] = {
	PHP_ME(Dao_Logger_Formatter_Line, __construct, arginfo_dao_logger_formatter_line___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Logger_Formatter_Line, setFormat, arginfo_dao_logger_formatter_line_setformat, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Logger_Formatter_Line, getFormat, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Logger_Formatter_Line, setDateFormat, arginfo_dao_logger_formatter_line_setdateformat, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Logger_Formatter_Line, getDateFormat, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Logger_Formatter_Line, format, arginfo_dao_logger_formatterinterface_format, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Logger\Formatter\Line initializer
 */
DAO_INIT_CLASS(Dao_Logger_Formatter_Line){

	DAO_REGISTER_CLASS_EX(Dao\\Logger\\Formatter, Line, logger_formatter_line, dao_logger_formatter_ce, dao_logger_formatter_line_method_entry, 0);

	zend_declare_property_string(dao_logger_formatter_line_ce, SL("_dateFormat"), "D, d M y H:i:s O", ZEND_ACC_PROTECTED);
	zend_declare_property_string(dao_logger_formatter_line_ce, SL("_format"), "[%date%][%type%] %message%", ZEND_ACC_PROTECTED);

	zend_class_implements(dao_logger_formatter_line_ce, 1, dao_logger_formatterinterface_ce);

	return SUCCESS;
}

/**
 * Dao\Logger\Formatter\Line construct
 *
 * @param string $format
 * @param string $dateFormat
 */
PHP_METHOD(Dao_Logger_Formatter_Line, __construct){

	zval *format = NULL, *date_format = NULL;

	dao_fetch_params(0, 0, 2, &format, &date_format);

	if (format && Z_TYPE_P(format) != IS_NULL) {
		dao_update_property(getThis(), SL("_format"), format);
	}

	if (date_format && Z_TYPE_P(date_format) != IS_NULL) {
		dao_update_property(getThis(), SL("_dateFormat"), date_format);
	}
}

/**
 * Set the log format
 *
 * @param string $format
 */
PHP_METHOD(Dao_Logger_Formatter_Line, setFormat){

	zval *format;

	dao_fetch_params(0, 1, 0, &format);

	dao_update_property(getThis(), SL("_format"), format);

}

/**
 * Returns the log format
 *
 * @return format
 */
PHP_METHOD(Dao_Logger_Formatter_Line, getFormat){


	RETURN_MEMBER(getThis(), "_format");
}

/**
 * Sets the internal date format
 *
 * @param string $date
 */
PHP_METHOD(Dao_Logger_Formatter_Line, setDateFormat){

	zval *date;

	dao_fetch_params(0, 1, 0, &date);

	dao_update_property(getThis(), SL("_dateFormat"), date);

}

/**
 * Returns the internal date format
 *
 * @return string
 */
PHP_METHOD(Dao_Logger_Formatter_Line, getDateFormat){


	RETURN_MEMBER(getThis(), "_dateFormat");
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
PHP_METHOD(Dao_Logger_Formatter_Line, format){

	zval *message, *type, *timestamp, *context, format = {}, date_format = {}, date = {}, date_wildcard = {}, new_format = {}, type_string = {}, type_wildcard = {}, message_wildcard = {};

	dao_fetch_params(0, 4, 0, &message, &type, &timestamp, &context);

	dao_read_property(&format, getThis(), SL("_format"), PH_COPY);

	/**
	 * Check if the format has the %date% placeholder
	 */
	if (dao_memnstr_str(&format, SL("%date%"))) {
		dao_read_property(&date_format, getThis(), SL("_dateFormat"), PH_READONLY);

		dao_date(&date, &date_format, timestamp);

		ZVAL_STRING(&date_wildcard, "%date%");

		DAO_STR_REPLACE(&new_format, &date_wildcard, &date, &format);

		zval_ptr_dtor(&date);
		zval_ptr_dtor(&date_wildcard);
	} else {
		ZVAL_COPY(&new_format, &format);
	}
	zval_ptr_dtor(&format);

	/**
	 * Check if the format has the %type% placeholder
	 */
	if (dao_memnstr_str(&new_format, SL("%type%"))) {
		DAO_CALL_METHOD(&type_string, getThis(), "gettypestring", type);

		ZVAL_STRING(&type_wildcard, "%type%");

		DAO_STR_REPLACE(&format, &type_wildcard, &type_string, &new_format);
		zval_ptr_dtor(&type_string);
		zval_ptr_dtor(&type_wildcard);
	} else {
		ZVAL_COPY(&format, &new_format);
	}
	zval_ptr_dtor(&new_format);

	ZVAL_STRING(&message_wildcard, "%message%");

	if (Z_TYPE_P(message) != IS_STRING) {
		zval tmp = {};
		DAO_CALL_FUNCTION(&tmp, "var_export", message, &DAO_GLOBAL(z_true));
		DAO_STR_REPLACE(&new_format, &message_wildcard, &tmp, &format);
		zval_ptr_dtor(&tmp);
	} else {
		DAO_STR_REPLACE(&new_format, &message_wildcard, message, &format);
	}
	zval_ptr_dtor(&format);
	zval_ptr_dtor(&message_wildcard);

	if (Z_TYPE_P(context) == IS_ARRAY && zend_hash_num_elements(Z_ARRVAL_P(context)) > 0) {
		DAO_CALL_METHOD(&format, getThis(), "interpolate", &new_format, context);
	} else {
		ZVAL_COPY(&format, &new_format);
	}
	zval_ptr_dtor(&new_format);

	DAO_CONCAT_VS(return_value, &format, PHP_EOL);
	zval_ptr_dtor(&format);
}
