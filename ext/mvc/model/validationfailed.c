
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

#include "mvc/model/validationfailed.h"
#include "mvc/model/exception.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/file.h"
#include "kernel/array.h"
#include "kernel/fcall.h"
#include "kernel/object.h"
#include "kernel/operators.h"

/**
 * Dao\Mvc\Model\ValidationFailed
 *
 * This exception is generated when a model fails to save a record
 * Dao\Mvc\Model must be set up to have this behavior
 */
zend_class_entry *dao_mvc_model_validationfailed_ce;

PHP_METHOD(Dao_Mvc_Model_ValidationFailed, __construct);
PHP_METHOD(Dao_Mvc_Model_ValidationFailed, getMessages);
PHP_METHOD(Dao_Mvc_Model_ValidationFailed, getModel);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_validationfailed___construct, 0, 0, 2)
	ZEND_ARG_INFO(0, model)
	ZEND_ARG_INFO(0, validationMessages)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_mvc_model_validationfailed_method_entry[] = {
	PHP_ME(Dao_Mvc_Model_ValidationFailed, __construct, arginfo_dao_mvc_model_validationfailed___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Mvc_Model_ValidationFailed, getMessages, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_ValidationFailed, getModel, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Mvc\Model\ValidationFailed initializer
 */
DAO_INIT_CLASS(Dao_Mvc_Model_ValidationFailed){

	DAO_REGISTER_CLASS_EX(Dao\\Mvc\\Model, ValidationFailed, mvc_model_validationfailed, dao_mvc_model_exception_ce, dao_mvc_model_validationfailed_method_entry, 0);

	zend_declare_property_null(dao_mvc_model_validationfailed_ce, SL("_model"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_model_validationfailed_ce, SL("_messages"), ZEND_ACC_PROTECTED);

	return SUCCESS;
}

/**
 * Dao\Mvc\Model\ValidationFailed constructor
 *
 * @param Dao\Mvc\Model $model
 * @param Dao\Mvc\Model\Message[] $validationMessages
 */
PHP_METHOD(Dao_Mvc_Model_ValidationFailed, __construct){

	zval *model, *validation_messages, message = {}, message_str = {};

	dao_fetch_params(0, 2, 0, &model, &validation_messages);

	if (dao_fast_count_ev(validation_messages)) {
		/**
		 * Get the first message in the array
		 */
		dao_array_fetch_long(&message, validation_messages, 0, PH_NOISY|PH_READONLY);

		/**
		 * Get the message to use it in the exception
		 */
		DAO_CALL_METHOD(&message_str, &message, "getmessage");
	} else {
		ZVAL_STRING(&message_str, "Validation failed");
	}

	dao_update_property(getThis(), SL("_model"), model);
	dao_update_property(getThis(), SL("_messages"), validation_messages);

	DAO_CALL_PARENT(NULL, dao_mvc_model_validationfailed_ce, getThis(), "__construct", &message_str);
}

/**
 * Returns the complete group of messages produced in the validation
 *
 * @return Dao\Mvc\Model\Message[]
 */
PHP_METHOD(Dao_Mvc_Model_ValidationFailed, getMessages){


	RETURN_MEMBER(getThis(), "_messages");
}

/**
 * Returns the model that generated the messages
 *
 * @return Dao\Mvc\Model
 */
PHP_METHOD(Dao_Mvc_Model_ValidationFailed, getModel){


	RETURN_MEMBER(getThis(), "_model");
}
