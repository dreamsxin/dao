
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

#include "logger/adapter/syslog.h"
#include "logger/adapter.h"
#include "logger/adapterinterface.h"
#include "logger/exception.h"
#include "logger/formatter/syslog.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/fcall.h"
#include "kernel/object.h"
#include "kernel/exception.h"

/**
 * Dao\Logger\Adapter\Syslog
 *
 * Sends logs to the system logger
 *
 *<code>
 *	$logger = new \Dao\Logger\Adapter\Syslog("ident", array(
 *		'option' => LOG_NDELAY,
 *		'facility' => LOG_MAIL
 *	));
 *	$logger->log(Dao\Logger::INFO, "This is a message");
 *	$logger->log(Dao\Logger::ERROR, "This is an error");
 *	$logger->error("This is another error");
 *</code>
 */
zend_class_entry *dao_logger_adapter_syslog_ce;

PHP_METHOD(Dao_Logger_Adapter_Syslog, __construct);
PHP_METHOD(Dao_Logger_Adapter_Syslog, getFormatter);
PHP_METHOD(Dao_Logger_Adapter_Syslog, logInternal);
PHP_METHOD(Dao_Logger_Adapter_Syslog, close);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_logger_adapter_syslog___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, options)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_logger_adapter_syslog_method_entry[] = {
	PHP_ME(Dao_Logger_Adapter_Syslog, __construct, arginfo_dao_logger_adapter_syslog___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Logger_Adapter_Syslog, getFormatter, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Logger_Adapter_Syslog, logInternal, arginfo_dao_logger_adapter_loginternal, ZEND_ACC_PROTECTED)
	PHP_ME(Dao_Logger_Adapter_Syslog, close, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Logger\Adapter\Syslog initializer
 */
DAO_INIT_CLASS(Dao_Logger_Adapter_Syslog){

	DAO_REGISTER_CLASS_EX(Dao\\Logger\\Adapter, Syslog, logger_adapter_syslog, dao_logger_adapter_ce, dao_logger_adapter_syslog_method_entry, 0);

	zend_declare_property_bool(dao_logger_adapter_syslog_ce, SL("_opened"), 0, ZEND_ACC_PROTECTED);

	zend_class_implements(dao_logger_adapter_syslog_ce, 1, dao_logger_adapterinterface_ce);

	return SUCCESS;
}

/**
 * Dao\Logger\Adapter\Syslog constructor
 *
 * @param string $name
 * @param array $options
 */
PHP_METHOD(Dao_Logger_Adapter_Syslog, __construct){

	zval *name, *options = NULL, option = {}, facility = {};

	dao_fetch_params(0, 1, 1, &name, &options);

	if (!options) {
		options = &DAO_GLOBAL(z_null);
	}

	/**
	 * We use 'fopen' to respect to open-basedir directive
	 */
	if (zend_is_true(name)) {
		if (!dao_array_isset_fetch_str(&option, options, SL("option"), PH_READONLY)) {
			/**
			 * Open the log in LOG_ODELAY mode
			 */
			ZVAL_LONG(&option, 4);
		}
		if (!dao_array_isset_fetch_str(&facility, options, SL("facility"), PH_READONLY)) {
			/**
			 * By default the facility is LOG_USER
			 */
			ZVAL_LONG(&facility, 8);
		}

		DAO_CALL_FUNCTION(NULL, "openlog", name, &option, &facility);
		dao_update_property_bool(getThis(), SL("_opened"), 1);
	}
}

/**
 * Returns the internal formatter
 *
 * @return Dao\Logger\Formatter\Line
 */
PHP_METHOD(Dao_Logger_Adapter_Syslog, getFormatter){

	zval formatter = {};

	dao_read_property(&formatter, getThis(), SL("_formatter"), PH_COPY);
	if (Z_TYPE(formatter) != IS_OBJECT) {
		object_init_ex(&formatter, dao_logger_formatter_syslog_ce);
		dao_update_property(getThis(), SL("_formatter"), &formatter);
	}

	RETURN_NCTOR(&formatter);
}

/**
 * Writes the log to the stream itself
 *
 * @param string $message
 * @param int $type
 * @param int $time
 * @param array $context
 */
PHP_METHOD(Dao_Logger_Adapter_Syslog, logInternal){

	zval *message, *type, *time, *context, formatter = {}, applied_format = {}, syslog_type = {}, syslog_message = {};

	dao_fetch_params(0, 4, 0, &message, &type, &time, &context);

	DAO_CALL_METHOD(&formatter, getThis(), "getformatter");
	DAO_CALL_METHOD(&applied_format, &formatter, "format", message, type, time, context);
	zval_ptr_dtor(&formatter);
	if (Z_TYPE(applied_format) != IS_ARRAY) {
		ZVAL_COPY_VALUE(&syslog_type, type);
		ZVAL_COPY_VALUE(&syslog_message, &applied_format);
	} else {
		dao_array_fetch_long(&syslog_type, &applied_format, 0, PH_NOISY|PH_READONLY);
		dao_array_fetch_long(&syslog_message, &applied_format, 1, PH_NOISY|PH_READONLY);
	}

	DAO_CALL_FUNCTION(NULL, "syslog", &syslog_type, &syslog_message);
	zval_ptr_dtor(&applied_format);
}

/**
 * Closes the logger
 *
 * @return boolean
 */
PHP_METHOD(Dao_Logger_Adapter_Syslog, close){

	zval opened = {};

	dao_read_property(&opened, getThis(), SL("_opened"), PH_NOISY|PH_READONLY);
	if (zend_is_true(&opened)) {
		DAO_CALL_FUNCTION(NULL, "closelog");
	}

	RETURN_TRUE;
}
