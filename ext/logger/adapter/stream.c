
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

#include "logger/adapter/stream.h"
#include "logger/adapter.h"
#include "logger/adapterinterface.h"
#include "logger/exception.h"
#include "logger/formatter/line.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/string.h"
#include "kernel/exception.h"
#include "kernel/fcall.h"
#include "kernel/concat.h"
#include "kernel/object.h"

/**
 * Dao\Logger\Adapter\Stream
 *
 * Sends logs to a valid PHP stream
 *
 *<code>
 *	$logger = new \Dao\Logger\Adapter\Stream("php://stderr");
 *	$logger->log(Dao\Logger::INFO, "This is a message");
 *	$logger->log(Dao\Logger::ERROR, "This is an error");
 *	$logger->error("This is another error");
 *</code>
 */
zend_class_entry *dao_logger_adapter_stream_ce;

PHP_METHOD(Dao_Logger_Adapter_Stream, __construct);
PHP_METHOD(Dao_Logger_Adapter_Stream, getFormatter);
PHP_METHOD(Dao_Logger_Adapter_Stream, logInternal);
PHP_METHOD(Dao_Logger_Adapter_Stream, close);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_logger_adapter_stream___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, options)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_logger_adapter_stream_method_entry[] = {
	PHP_ME(Dao_Logger_Adapter_Stream, __construct, arginfo_dao_logger_adapter_stream___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Logger_Adapter_Stream, getFormatter, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Logger_Adapter_Stream, logInternal, arginfo_dao_logger_adapter_loginternal, ZEND_ACC_PROTECTED)
	PHP_ME(Dao_Logger_Adapter_Stream, close, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Logger\Adapter\Stream initializer
 */
DAO_INIT_CLASS(Dao_Logger_Adapter_Stream){

	DAO_REGISTER_CLASS_EX(Dao\\Logger\\Adapter, Stream, logger_adapter_stream, dao_logger_adapter_ce, dao_logger_adapter_stream_method_entry, 0);

	zend_declare_property_null(dao_logger_adapter_stream_ce, SL("_stream"), ZEND_ACC_PROTECTED);

	zend_class_implements(dao_logger_adapter_stream_ce, 1, dao_logger_adapterinterface_ce);

	return SUCCESS;
}

/**
 * Dao\Logger\Adapter\Stream constructor
 *
 * @param string $name
 * @param array $options
 */
PHP_METHOD(Dao_Logger_Adapter_Stream, __construct){

	zval *name, *options = NULL, mode = {}, stream = {};

	dao_fetch_params(0, 1, 1, &name, &options);
	DAO_ENSURE_IS_STRING(name);

	if (!options) {
		options = &DAO_GLOBAL(z_null);
	}

	if (dao_array_isset_fetch_str(&mode, options, SL("mode"), PH_COPY)) {
		if (dao_memnstr_str(&mode, SL("r"))) {
			DAO_THROW_EXCEPTION_STR(dao_logger_exception_ce, "Stream must be opened in append or write mode");
			zval_ptr_dtor(&mode);
			return;
		}
	} else {
		ZVAL_STRING(&mode, "ab");
	}

	/**
	 * We use 'fopen' to respect to open-basedir directive
	 */
	DAO_CALL_FUNCTION(&stream, "fopen", name, &mode);
	zval_ptr_dtor(&mode);
	if (Z_TYPE(stream) != IS_RESOURCE) {
		zend_throw_exception_ex(dao_logger_exception_ce, 0, "Cannot open stream '%s'", Z_STRVAL_P(name));
	} else {
		dao_update_property(getThis(), SL("_stream"), &stream);
	}
	zval_ptr_dtor(&stream);
}

/**
 * Returns the internal formatter
 *
 * @return Dao\Logger\Formatter\Line
 */
PHP_METHOD(Dao_Logger_Adapter_Stream, getFormatter)
{
	zval formatter = {};

	dao_read_property(&formatter, getThis(), SL("_formatter"), PH_READONLY);
	if (Z_TYPE(formatter) == IS_OBJECT) {
		RETURN_CTOR(&formatter);
	}
	object_init_ex(return_value, dao_logger_formatter_line_ce);
	DAO_CALL_METHOD(NULL, return_value, "__construct");

	dao_update_property(getThis(), SL("_formatter"), return_value);
}

/**
 * Writes the log to the stream itself
 *
 * @param string $message
 * @param int $type
 * @param int $time
 * @param array $context
 */
PHP_METHOD(Dao_Logger_Adapter_Stream, logInternal){

	zval *message, *type, *time, *context, stream = {}, formatter = {}, applied_format = {};

	dao_fetch_params(0, 4, 0, &message, &type, &time, &context);

	dao_read_property(&stream, getThis(), SL("_stream"), PH_NOISY|PH_READONLY);
	if (Z_TYPE(stream) != IS_RESOURCE) {
		DAO_THROW_EXCEPTION_STR(dao_logger_exception_ce, "Cannot send message to the log because it is invalid");
		return;
	}

	DAO_CALL_METHOD(&formatter, getThis(), "getformatter");
	DAO_CALL_METHOD(&applied_format, &formatter, "format", message, type, time, context);
	zval_ptr_dtor(&formatter);
	DAO_CALL_FUNCTION(NULL, "fwrite", &stream, &applied_format);
	zval_ptr_dtor(&applied_format);
}

/**
 * Closes the logger
 *
 * @return boolean
 */
PHP_METHOD(Dao_Logger_Adapter_Stream, close)
{
	zval stream = {};

	dao_read_property(&stream, getThis(), SL("_stream"), PH_NOISY|PH_READONLY);
	DAO_RETURN_CALL_FUNCTION("fclose", &stream);
}
