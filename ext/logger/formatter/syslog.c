
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

#include "logger/formatter/syslog.h"
#include "logger/formatter.h"
#include "logger/formatterinterface.h"

#include "kernel/main.h"
#include "kernel/array.h"
#include "kernel/fcall.h"

/**
 * Dao\Logger\Formatter\Syslog
 *
 * Prepares a message to be used in a Syslog backend
 */
zend_class_entry *dao_logger_formatter_syslog_ce;

PHP_METHOD(Dao_Logger_Formatter_Syslog, format);

static const zend_function_entry dao_logger_formatter_syslog_method_entry[] = {
	PHP_ME(Dao_Logger_Formatter_Syslog, format, arginfo_dao_logger_formatterinterface_format, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Logger\Formatter\Syslog initializer
 */
DAO_INIT_CLASS(Dao_Logger_Formatter_Syslog){

	DAO_REGISTER_CLASS_EX(Dao\\Logger\\Formatter, Syslog, logger_formatter_syslog, dao_logger_formatter_ce, dao_logger_formatter_syslog_method_entry, 0);

	zend_class_implements(dao_logger_formatter_syslog_ce, 1, dao_logger_formatterinterface_ce);

	return SUCCESS;
}

/**
 * Applies a format to a message before sent it to the internal log
 *
 * @param string $message
 * @param int $type
 * @param int $timestamp
 * @param array $context
 * @return array
 */
PHP_METHOD(Dao_Logger_Formatter_Syslog, format){

	zval *message, *type, *timestamp, *context, interpolated = {};

	dao_fetch_params(0, 4, 0, &message, &type, &timestamp, &context);

	if (Z_TYPE_P(context) == IS_ARRAY) {
		DAO_CALL_METHOD(&interpolated, getThis(), "interpolate", message, context);
	} else {
		ZVAL_COPY_VALUE(&interpolated, message);
	}

	array_init_size(return_value, 2);
	dao_array_append(return_value, type, PH_COPY);
	dao_array_append(return_value, &interpolated, PH_COPY);
}
