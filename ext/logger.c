
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

#include "logger.h"
#include "logger/exception.h"

#include "di.h"
#include "diinterface.h"

#include "kernel/main.h"
#include "kernel/exception.h"
#include "kernel/array.h"
#include "kernel/fcall.h"

#include "interned-strings.h"

/**
 * Dao\Logger
 *
 * Dao\Logger is a component whose purpose is create logs using
 * different backends via adapters, generating options, formats and filters
 * also implementing transactions.
 *
 *<code>
 *	$logger = new Dao\Logger\Adapter\File("app/logs/test.log");
 *	$logger->log(Dao\Logger::INFO, "This is a message");
 *	$logger->log(Dao\Logger::ERROR, "This is an error");
 *	$logger->error("This is another error");
 *</code>
 */
zend_class_entry *dao_logger_ce;

PHP_METHOD(Dao_Logger, getTypeString);
PHP_METHOD(Dao_Logger, __callStatic);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_logger_gettypestring, 0, 0, 1)
	ZEND_ARG_INFO(0, type)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_logger___callstatic, 0, 0, 1)
	ZEND_ARG_INFO(0, method)
	ZEND_ARG_INFO(0, arguments)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_logger_method_entry[] = {
	PHP_ME(Dao_Logger, getTypeString, arginfo_dao_logger_gettypestring, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(Dao_Logger, __callStatic, arginfo_dao_logger___callstatic, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};

/**
 * Dao\Logger initializer
 */
DAO_INIT_CLASS(Dao_Logger){

	DAO_REGISTER_CLASS(Dao, Logger, logger, dao_logger_method_entry, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	zend_declare_class_constant_long(dao_logger_ce, SL("SPECIAL"),   DAO_LOGGER_SPECIAL  );
	zend_declare_class_constant_long(dao_logger_ce, SL("CUSTOM"),    DAO_LOGGER_CUSTOM   );
	zend_declare_class_constant_long(dao_logger_ce, SL("DEBUG"),     DAO_LOGGER_DEBUG    );
	zend_declare_class_constant_long(dao_logger_ce, SL("INFO"),      DAO_LOGGER_INFO     );
	zend_declare_class_constant_long(dao_logger_ce, SL("NOTICE"),    DAO_LOGGER_NOTICE   );
	zend_declare_class_constant_long(dao_logger_ce, SL("WARNING"),   DAO_LOGGER_WARNING  );
	zend_declare_class_constant_long(dao_logger_ce, SL("ERROR"),     DAO_LOGGER_ERROR    );
	zend_declare_class_constant_long(dao_logger_ce, SL("ALERT"),     DAO_LOGGER_ALERT    );
	zend_declare_class_constant_long(dao_logger_ce, SL("CRITICAL"),  DAO_LOGGER_CRITICAL );
	zend_declare_class_constant_long(dao_logger_ce, SL("EMERGENCE"), DAO_LOGGER_EMERGENCY);
	zend_declare_class_constant_long(dao_logger_ce, SL("EMERGENCY"), DAO_LOGGER_EMERGENCY);

	return SUCCESS;
}

/**
 * Returns the string meaning of a logger constant
 *
 * @param  integer $type
 * @return string
 */
PHP_METHOD(Dao_Logger, getTypeString){

	static const char *lut[10] = {
		"EMERGENCY", "CRITICAL", "ALERT", "ERROR",  "WARNING",
		"NOTICE",    "INFO",     "DEBUG", "CUSTOM", "SPECIAL"
	};

	zval *type;
	int itype;

	dao_fetch_params(0, 1, 0, &type);
	DAO_ENSURE_IS_LONG(type);
	
	itype = Z_LVAL_P(type);
	if (itype >= 0 && itype < 10) {
		RETURN_STRING(lut[itype]);
	}
	
	RETURN_STRING("CUSTOM");
}

/**
 * Handles method calls when a static method is not implemented
 *
 * @param string $method
 * @param array $arguments
 * @return mixed
 */
PHP_METHOD(Dao_Logger, __callStatic){

	zval *method, *arguments = NULL, dependency_injector = {}, service_name = {}, logger = {}, message = {}, context = {};

	dao_fetch_params(1, 1, 1, &method, &arguments);
	
	DAO_CALL_CE_STATIC(&dependency_injector, dao_di_ce, "getdefault");
	DAO_MM_ADD_ENTRY(&dependency_injector);

	if (Z_TYPE(dependency_injector) != IS_OBJECT) {
		DAO_MM_THROW_EXCEPTION_STR(dao_logger_exception_ce, "A dependency injector container is required to obtain the services related to the ORM");
		return;
	}

	ZVAL_STR(&service_name, IS(logger));

	DAO_MM_CALL_METHOD(&logger, &dependency_injector, "getshared", &service_name);
	DAO_MM_ADD_ENTRY(&logger);

	if (!dao_array_isset_fetch_long(&message, arguments, 0, PH_READONLY)) {
		RETURN_MM_FALSE;
	}

	if (!dao_array_isset_fetch_long(&context, arguments, 1, PH_READONLY)) {
		DAO_CALL_METHOD(NULL, &logger, "log", method, &message, &context);
	} else {
		DAO_CALL_METHOD(NULL, &logger, "log", method, &message);
	}
	RETURN_MM_TRUE;
}
