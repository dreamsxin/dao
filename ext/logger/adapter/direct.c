
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

#include "logger/adapter/direct.h"
#include "logger/adapter.h"
#include "logger/adapterinterface.h"
#include "logger/exception.h"
#include "logger/formatter/line.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/fcall.h"
#include "kernel/object.h"
#include "kernel/exception.h"

/**
 * Dao\Logger\Adapter\Direct
 *
 *<code>
 *	$logger = new \Dao\Logger\Adapter\Direct;
 *	$logger->log(Dao\Logger::INFO, "This is a message");
 *	$logger->log(Dao\Logger::ERROR, "This is an error");
 *	$logger->error("This is another error");
 *</code>
 */
zend_class_entry *dao_logger_adapter_direct_ce;

PHP_METHOD(Dao_Logger_Adapter_Direct, getFormatter);
PHP_METHOD(Dao_Logger_Adapter_Direct, logInternal);
PHP_METHOD(Dao_Logger_Adapter_Direct, close);

static const zend_function_entry dao_logger_adapter_direct_method_entry[] = {
	PHP_ME(Dao_Logger_Adapter_Direct, getFormatter, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Logger_Adapter_Direct, logInternal, arginfo_dao_logger_adapter_loginternal, ZEND_ACC_PROTECTED)
	PHP_ME(Dao_Logger_Adapter_Direct, close, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Logger\Adapter\Direct initializer
 */
DAO_INIT_CLASS(Dao_Logger_Adapter_Direct){

	DAO_REGISTER_CLASS_EX(Dao\\Logger\\Adapter, Direct, logger_adapter_direct, dao_logger_adapter_ce, dao_logger_adapter_direct_method_entry, 0);

	zend_class_implements(dao_logger_adapter_direct_ce, 1, dao_logger_adapterinterface_ce);

	return SUCCESS;
}

/**
 * Returns the internal formatter
 *
 * @return Dao\Logger\Formatter\Line
 */
PHP_METHOD(Dao_Logger_Adapter_Direct, getFormatter){

	zval formatter = {};

	dao_read_property(&formatter, getThis(), SL("_formatter"), PH_COPY);
	if (Z_TYPE(formatter) != IS_OBJECT) {
		object_init_ex(&formatter, dao_logger_formatter_line_ce);
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
PHP_METHOD(Dao_Logger_Adapter_Direct, logInternal){

	zval *message, *type, *time, *context, formatter = {}, applied_format = {}, direct_type = {}, direct_message = {};

	dao_fetch_params(0, 4, 0, &message, &type, &time, &context);

	DAO_CALL_METHOD(&formatter, getThis(), "getformatter");
	DAO_CALL_METHOD(&applied_format, &formatter, "format", message, type, time, context);
	zval_ptr_dtor(&formatter);
	if (Z_TYPE(applied_format) != IS_ARRAY) {
		ZVAL_COPY_VALUE(&direct_type, type);
		ZVAL_COPY_VALUE(&direct_message, &applied_format);
	} else {
		dao_array_fetch_long(&direct_type, &applied_format, 0, PH_NOISY|PH_READONLY);
		dao_array_fetch_long(&direct_message, &applied_format, 1, PH_NOISY|PH_READONLY);
	}

	zend_print_zval_r(&direct_message, 0);
	zval_ptr_dtor(&applied_format);
}

/**
 * Closes the logger
 *
 * @return boolean
 */
PHP_METHOD(Dao_Logger_Adapter_Direct, close){

	RETURN_TRUE;
}
