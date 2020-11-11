
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

#include "logger/adapterinterface.h"
#include "kernel/main.h"

zend_class_entry *dao_logger_adapterinterface_ce;

static const zend_function_entry dao_logger_adapterinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Logger_AdapterInterface, setFormatter, arginfo_dao_logger_adapterinterface_setformatter)
	PHP_ABSTRACT_ME(Dao_Logger_AdapterInterface, getFormatter, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Logger_AdapterInterface, setLogLevel, arginfo_dao_logger_adapterinterface_setloglevel)
	PHP_ABSTRACT_ME(Dao_Logger_AdapterInterface, getLogLevel, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Logger_AdapterInterface, begin, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Logger_AdapterInterface, commit, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Logger_AdapterInterface, rollback, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Logger_AdapterInterface, close, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Logger_AdapterInterface, log, arginfo_dao_logger_adapterinterface_log)
	PHP_ABSTRACT_ME(Dao_Logger_AdapterInterface, debug, arginfo_dao_logger_adapterinterface_debug)
	PHP_ABSTRACT_ME(Dao_Logger_AdapterInterface, info, arginfo_dao_logger_adapterinterface_info)
	PHP_ABSTRACT_ME(Dao_Logger_AdapterInterface, notice, arginfo_dao_logger_adapterinterface_notice)
	PHP_ABSTRACT_ME(Dao_Logger_AdapterInterface, warning, arginfo_dao_logger_adapterinterface_warning)
	PHP_ABSTRACT_ME(Dao_Logger_AdapterInterface, error, arginfo_dao_logger_adapterinterface_error)
	PHP_ABSTRACT_ME(Dao_Logger_AdapterInterface, critical, arginfo_dao_logger_adapterinterface_critical)
	PHP_ABSTRACT_ME(Dao_Logger_AdapterInterface, alert, arginfo_dao_logger_adapterinterface_alert)
	PHP_ABSTRACT_ME(Dao_Logger_AdapterInterface, emergency, arginfo_dao_logger_adapterinterface_emergency)
	PHP_FE_END
};

/**
 * Dao\Logger\AdapterInterface initializer
 */
DAO_INIT_CLASS(Dao_Logger_AdapterInterface){

	DAO_REGISTER_INTERFACE(Dao\\Logger, AdapterInterface, logger_adapterinterface, dao_logger_adapterinterface_method_entry);

	return SUCCESS;
}

/**
 * Sets the message formatter
 *
 * @param Dao\Logger\FormatterInterface $formatter
 * @return Dao\Logger\Adapter
 */
DAO_DOC_METHOD(Dao_Logger_AdapterInterface, setFormatter);

/**
 * Returns the internal formatter
 *
 * @return Dao\Logger\FormatterInterface
 */
DAO_DOC_METHOD(Dao_Logger_AdapterInterface, getFormatter);

/**
 * Filters the logs sent to the handlers to be greater or equals than a specific level
 *
 * @param int $level
 * @return Dao\Logger\Adapter
 */
DAO_DOC_METHOD(Dao_Logger_AdapterInterface, setLogLevel);

/**
 * Returns the current log level
 *
 * @return int
 */
DAO_DOC_METHOD(Dao_Logger_AdapterInterface, getLogLevel);

/**
 * Sends/Writes messages to the file log
 *
 * @param int|string $type
 * @param string $message
 * @param array $context
 * @return Dao\Logger\AdapterInterface
 */
DAO_DOC_METHOD(Dao_Logger_AdapterInterface, log);

/**
 * Starts a transaction
 *
 * @return Dao\Logger\Adapter
 */
DAO_DOC_METHOD(Dao_Logger_AdapterInterface, begin);

/**
 * Commits the internal transaction
 *
 * @return Dao\Logger\Adapter
 */
DAO_DOC_METHOD(Dao_Logger_AdapterInterface, commit);

/**
 * Rollbacks the internal transaction
 *
 * @return Dao\Logger\Adapter
 */
DAO_DOC_METHOD(Dao_Logger_AdapterInterface, rollback);

/**
 * Closes the logger
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Logger_AdapterInterface, close);

/**
 * Sends/Writes a debug message to the log
 *
 * @param string $message
 * @param array $context
 * @return Dao\Logger\AdapterInterface
 */
DAO_DOC_METHOD(Dao_Logger_AdapterInterface, debug);

/**
 * Sends/Writes an info message to the log
 *
 * @param string $message
 * @param array $context
 * @return Dao\Logger\AdapterInterface
 */
DAO_DOC_METHOD(Dao_Logger_AdapterInterface, info);

/**
 * Sends/Writes a notice message to the log
 *
 * @param string $message
 * @return Dao\Logger\AdapterInterface
 */
DAO_DOC_METHOD(Dao_Logger_AdapterInterface, notice);

/**
 * Sends/Writes a warning message to the log
 *
 * @param string $message
 * @param array $context
 * @return Dao\Logger\AdapterInterface
 */
DAO_DOC_METHOD(Dao_Logger_AdapterInterface, warning);

/**
 * Sends/Writes an error message to the log
 *
 * @param string $message
 * @param array $context
 * @return Dao\Logger\AdapterInterface
 */
DAO_DOC_METHOD(Dao_Logger_AdapterInterface, error);

/**
 * Sends/Writes a critical message to the log
 *
 * @param string $message
 * @param array $context
 * @return Dao\Logger\AdapterInterface
 */
DAO_DOC_METHOD(Dao_Logger_AdapterInterface, critical);

/**
 * Sends/Writes an alert message to the log
 *
 * @param string $message
 * @param array $context
 * @return Dao\Logger\AdapterInterface
 */
DAO_DOC_METHOD(Dao_Logger_AdapterInterface, alert);

/**
 * Sends/Writes an emergency message to the log
 *
 * @param string $message
 * @param array $context
 * @return Dao\Logger\AdapterInterface
 */
DAO_DOC_METHOD(Dao_Logger_AdapterInterface, emergency);
