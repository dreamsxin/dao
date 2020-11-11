
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

#include "logger/multiple.h"
#include "logger/adapterinterface.h"
#include "logger/exception.h"
#include "logger.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/exception.h"
#include "kernel/object.h"
#include "kernel/fcall.h"

/**
 * Dao\Logger\Multiple
 *
 * Handles multiples logger handlers
 */
zend_class_entry *dao_logger_multiple_ce;

PHP_METHOD(Dao_Logger_Multiple, push);
PHP_METHOD(Dao_Logger_Multiple, getLoggers);
PHP_METHOD(Dao_Logger_Multiple, setFormatter);
PHP_METHOD(Dao_Logger_Multiple, getFormatter);
PHP_METHOD(Dao_Logger_Multiple, log);
PHP_METHOD(Dao_Logger_Multiple, emergency);
PHP_METHOD(Dao_Logger_Multiple, debug);
PHP_METHOD(Dao_Logger_Multiple, error);
PHP_METHOD(Dao_Logger_Multiple, info);
PHP_METHOD(Dao_Logger_Multiple, notice);
PHP_METHOD(Dao_Logger_Multiple, warning);
PHP_METHOD(Dao_Logger_Multiple, alert);
PHP_METHOD(Dao_Logger_Multiple, critical);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_logger_multiple_push, 0, 0, 1)
	ZEND_ARG_INFO(0, logger)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_logger_multiple_method_entry[] = {
	PHP_ME(Dao_Logger_Multiple, push, arginfo_dao_logger_multiple_push, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Logger_Multiple, getLoggers, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Logger_Multiple, setFormatter, arginfo_dao_logger_adapterinterface_setformatter, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Logger_Multiple, getFormatter, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Logger_Multiple, log, arginfo_dao_logger_adapterinterface_log, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Logger_Multiple, emergency, arginfo_dao_logger_adapterinterface_emergency, ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Logger_Multiple, emergence, emergency, arginfo_dao_logger_adapterinterface_emergency, ZEND_ACC_PUBLIC | ZEND_ACC_DEPRECATED)
	PHP_ME(Dao_Logger_Multiple, debug, arginfo_dao_logger_adapterinterface_debug, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Logger_Multiple, error, arginfo_dao_logger_adapterinterface_error, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Logger_Multiple, info, arginfo_dao_logger_adapterinterface_info, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Logger_Multiple, notice, arginfo_dao_logger_adapterinterface_notice, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Logger_Multiple, warning, arginfo_dao_logger_adapterinterface_warning, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Logger_Multiple, alert, arginfo_dao_logger_adapterinterface_alert, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Logger\Multiple initializer
 */
DAO_INIT_CLASS(Dao_Logger_Multiple){

	DAO_REGISTER_CLASS(Dao\\Logger, Multiple, logger_multiple, dao_logger_multiple_method_entry, 0);

	zend_declare_property_null(dao_logger_multiple_ce, SL("_loggers"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_logger_multiple_ce, SL("_formatter"), ZEND_ACC_PROTECTED);

	return SUCCESS;
}

/**
 * Pushes a logger to the logger tail
 *
 * @param Dao\Logger\AdapterInterface $logger
 */
PHP_METHOD(Dao_Logger_Multiple, push){

	zval *logger;

	dao_fetch_params(0, 1, 0, &logger);

	DAO_VERIFY_INTERFACE_EX(logger, dao_logger_adapterinterface_ce, dao_logger_exception_ce)
	dao_update_property_array_append(getThis(), SL("_loggers"), logger);
}

/**
 * Returns the registered loggers
 *
 * @return Dao\Logger\AdapterInterface[]
 */
PHP_METHOD(Dao_Logger_Multiple, getLoggers){


	RETURN_MEMBER(getThis(), "_loggers");
}

/**
 * Sets a global formatter
 *
 * @param Dao\Logger\FormatterInterface $formatter
 */
PHP_METHOD(Dao_Logger_Multiple, setFormatter){

	zval *formatter, loggers = {}, *logger;

	dao_fetch_params(0, 1, 0, &formatter);

	dao_read_property(&loggers, getThis(), SL("_loggers"), PH_NOISY|PH_READONLY);
	if (Z_TYPE(loggers) == IS_ARRAY) {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL(loggers), logger) {
			DAO_CALL_METHOD(NULL, logger, "setformatter", formatter);
		} ZEND_HASH_FOREACH_END();

	}

	dao_update_property(getThis(), SL("_formatter"), formatter);
}

/**
 * Returns a formatter
 *
 * @return Dao\Logger\FormatterInterface
 */
PHP_METHOD(Dao_Logger_Multiple, getFormatter){


	RETURN_MEMBER(getThis(), "_formatter");
}

/**
 * Sends a message to each registered logger
 *
 * @param string $message
 * @param int $type
 * @param array $context
 */
PHP_METHOD(Dao_Logger_Multiple, log){

	zval *message, *_type = NULL, *context = NULL, type = {}, loggers = {}, *logger;

	dao_fetch_params(0, 1, 2, &message, &type, &context);

	if (!_type) {
		ZVAL_LONG(&type, DAO_LOGGER_DEBUG);
	} else {
		ZVAL_COPY_VALUE(&type, _type);
	}

	if (!context) {
		context = &DAO_GLOBAL(z_null);
	}

	dao_read_property(&loggers, getThis(), SL("_loggers"), PH_NOISY|PH_READONLY);
	if (Z_TYPE(loggers) == IS_ARRAY) {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL(loggers), logger) {
			DAO_CALL_METHOD(NULL, logger, "log", message, &type, context);
		} ZEND_HASH_FOREACH_END();
	}
}

/**
 * Sends/Writes an emergency message to the log
 *
 * @param string $message
 * @param array $context
 */
PHP_METHOD(Dao_Logger_Multiple, emergency){

	zval *message, *context = NULL, type = {};

	dao_fetch_params(0, 1, 1, &message, &context);

	if (!context) {
		context = &DAO_GLOBAL(z_null);
	}

	ZVAL_LONG(&type, DAO_LOGGER_EMERGENCY);
	DAO_CALL_METHOD(NULL, getThis(), "log", message, &type, context);
}

/**
 * Sends/Writes a debug message to the log
 *
 * @param string $message
 * @param array $context
 */
PHP_METHOD(Dao_Logger_Multiple, debug){

	zval *message, *context = NULL, type = {};

	dao_fetch_params(0, 1, 1, &message, &context);

	if (!context) {
		context = &DAO_GLOBAL(z_null);
	}

	ZVAL_LONG(&type, DAO_LOGGER_DEBUG);
	DAO_CALL_METHOD(NULL, getThis(), "log", message, &type, context);
}

/**
 * Sends/Writes an error message to the log
 *
 * @param string $message
 * @param array $context
 */
PHP_METHOD(Dao_Logger_Multiple, error){

	zval *message, *context = NULL, type = {};

	dao_fetch_params(0, 1, 1, &message, &context);

	if (!context) {
		context = &DAO_GLOBAL(z_null);
	}

	ZVAL_LONG(&type, DAO_LOGGER_ERROR);
	DAO_CALL_METHOD(NULL, getThis(), "log", message, &type, context);
}

/**
 * Sends/Writes an info message to the log
 *
 * @param string $message
 * @param array $context
 */
PHP_METHOD(Dao_Logger_Multiple, info){

	zval *message, *context = NULL, type = {};

	dao_fetch_params(0, 1, 1, &message, &context);

	if (!context) {
		context = &DAO_GLOBAL(z_null);
	}

	ZVAL_LONG(&type, DAO_LOGGER_INFO);
	DAO_CALL_METHOD(NULL, getThis(), "log", message, &type, context);
}

/**
 * Sends/Writes a notice message to the log
 *
 * @param string $message
 * @param array $context
 */
PHP_METHOD(Dao_Logger_Multiple, notice){

	zval *message, *context = NULL, type = {};

	dao_fetch_params(0, 1, 1, &message, &context);

	if (!context) {
		context = &DAO_GLOBAL(z_null);
	}

	ZVAL_LONG(&type, DAO_LOGGER_NOTICE);
	DAO_CALL_METHOD(NULL, getThis(), "log", message, &type, context);
}

/**
 * Sends/Writes a warning message to the log
 *
 * @param string $message
 * @param array $context
 */
PHP_METHOD(Dao_Logger_Multiple, warning){

	zval *message, *context = NULL, type = {};

	dao_fetch_params(0, 1, 1, &message, &context);

	if (!context) {
		context = &DAO_GLOBAL(z_null);
	}

	ZVAL_LONG(&type, DAO_LOGGER_WARNING);
	DAO_CALL_METHOD(NULL, getThis(), "log", message, &type, context);
}

/**
 * Sends/Writes an alert message to the log
 *
 * @param string $message
 * @param array $context
 */
PHP_METHOD(Dao_Logger_Multiple, alert){

	zval *message, *context = NULL, type = {};

	dao_fetch_params(0, 1, 1, &message, &context);

	if (!context) {
		context = &DAO_GLOBAL(z_null);
	}

	ZVAL_LONG(&type, DAO_LOGGER_ALERT);
	DAO_CALL_METHOD(NULL, getThis(), "log", message, &type, context);
}

/**
 * Sends/Writes an critical message to the log
 *
 * @param string $message
 * @param array $context
 */
PHP_METHOD(Dao_Logger_Multiple, critical){

	zval *message, *context = NULL, type = {};

	dao_fetch_params(0, 1, 1, &message, &context);

	if (!context) {
		context = &DAO_GLOBAL(z_null);
	}

	ZVAL_LONG(&type, DAO_LOGGER_CRITICAL);
	DAO_CALL_METHOD(NULL, getThis(), "log", message, &type, context);
}
