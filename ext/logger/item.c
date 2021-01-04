
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

#include "logger/item.h"

#include "kernel/main.h"
#include "kernel/object.h"

/**
 * Dao\Logger\Item
 *
 * Represents each item in a logging transaction
 *
 */
zend_class_entry *dao_logger_item_ce;

PHP_METHOD(Dao_Logger_Item, __construct);
PHP_METHOD(Dao_Logger_Item, getMessage);
PHP_METHOD(Dao_Logger_Item, getType);
PHP_METHOD(Dao_Logger_Item, getTime);
PHP_METHOD(Dao_Logger_Item, getContext);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_logger_item___construct, 0, 0, 2)
	ZEND_ARG_INFO(0, message)
	ZEND_ARG_INFO(0, type)
	ZEND_ARG_INFO(0, time)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_logger_item_method_entry[] = {
	PHP_ME(Dao_Logger_Item, __construct, arginfo_dao_logger_item___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Logger_Item, getMessage, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Logger_Item, getType, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Logger_Item, getTime, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Logger_Item, getContext, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Logger\Item initializer
 */
DAO_INIT_CLASS(Dao_Logger_Item){

	DAO_REGISTER_CLASS(Dao\\Logger, Item, logger_item, dao_logger_item_method_entry, 0);

	zend_declare_property_null(dao_logger_item_ce, SL("_type"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_logger_item_ce, SL("_message"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_logger_item_ce, SL("_time"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_logger_item_ce, SL("_context"), ZEND_ACC_PROTECTED);

	return SUCCESS;
}

/**
 * Dao\Logger\Item constructor
 *
 * @param string $message
 * @param integer $type
 * @param integer $time
 */
PHP_METHOD(Dao_Logger_Item, __construct){

	zval *message, *type, *time = NULL, *context = NULL;

	dao_fetch_params(0, 2, 2, &message, &type, &time, &context);

	if (!time) {
		time = &DAO_GLOBAL(z_zero);
	}

	dao_update_property(getThis(), SL("_message"), message);
	dao_update_property(getThis(), SL("_type"), type);
	dao_update_property(getThis(), SL("_time"), time);

	if (context && Z_TYPE_P(context) == IS_ARRAY) {
		dao_update_property(getThis(), SL("_context"), context);
	}
}

/**
 * Returns the message
 *
 * @return string
 */
PHP_METHOD(Dao_Logger_Item, getMessage){


	RETURN_MEMBER(getThis(), "_message");
}

/**
 * Returns the log type
 *
 * @return integer
 */
PHP_METHOD(Dao_Logger_Item, getType){


	RETURN_MEMBER(getThis(), "_type");
}

/**
 * Returns log timestamp
 *
 * @return integer
 */
PHP_METHOD(Dao_Logger_Item, getTime){


	RETURN_MEMBER(getThis(), "_time");
}

PHP_METHOD(Dao_Logger_Item, getContext) {

	zval context = {};

	dao_read_property(&context, getThis(), SL("_context"), PH_NOISY|PH_READONLY);

	if (Z_TYPE(context) == IS_ARRAY) {
		RETURN_CTOR(&context);
	}

	array_init(return_value);
}
