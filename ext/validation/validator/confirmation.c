
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

#include "validation/validator/confirmation.h"
#include "validation/validator.h"
#include "validation/validatorinterface.h"
#include "validation/message.h"
#include "validation/exception.h"
#include "validation.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "kernel/concat.h"
#include "kernel/array.h"

#include "interned-strings.h"

/**
 * Dao\Validation\Validator\Confirmation
 *
 * Checks that two values have the same value
 *
 *<code>
 *use Dao\Validation\Validator\Confirmation;
 *
 *$validator->add('password', new Confirmation(array(
 *   'message' => 'Password doesn\'t match confirmation',
 *   'with' => 'confirmPassword'
 *)));
 *</code>
 */
zend_class_entry *dao_validation_validator_confirmation_ce;

PHP_METHOD(Dao_Validation_Validator_Confirmation, validate);
PHP_METHOD(Dao_Validation_Validator_Confirmation, valid);

static const zend_function_entry dao_validation_validator_confirmation_method_entry[] = {
	PHP_ME(Dao_Validation_Validator_Confirmation, validate, arginfo_dao_validation_validatorinterface_validate, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Validation_Validator_Confirmation, valid, arginfo_dao_validation_validatorinterface_valid, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Validation\Validator\Confirmation initializer
 */
DAO_INIT_CLASS(Dao_Validation_Validator_Confirmation){

	DAO_REGISTER_CLASS_EX(Dao\\Validation\\Validator, Confirmation, validation_validator_confirmation, dao_validation_validator_ce, dao_validation_validator_confirmation_method_entry, 0);

	zend_class_implements(dao_validation_validator_confirmation_ce, 1, dao_validation_validatorinterface_ce);

	return SUCCESS;
}

/**
 * Executes the validation
 *
 * @param Dao\Validation $validator
 * @param string $attribute
 * @return boolean
 */
PHP_METHOD(Dao_Validation_Validator_Confirmation, validate){

	zval *validaton, *attribute, *_allow_empty = NULL, value = {}, allow_empty = {}, with_attribute = {}, with_value = {}, valid = {}, message_str = {}, message = {}, code = {}, label = {}, with_label = {}, pairs = {}, prepared = {};
	zend_class_entry *ce = Z_OBJCE_P(getThis());

	dao_fetch_params(0, 2, 1, &validaton, &attribute, &_allow_empty);
	DAO_VERIFY_INTERFACE_EX(validaton, dao_validationinterface_ce, dao_validation_exception_ce);

	DAO_CALL_METHOD(&value, validaton, "getvalue", attribute);

	RETURN_ON_FAILURE(dao_validation_validator_getoption_helper(&allow_empty, ce, getThis(), ISV(allowEmpty)));
	if (Z_TYPE(allow_empty) == IS_NULL) {
		if (_allow_empty && zend_is_true(_allow_empty)) {
			ZVAL_COPY(&allow_empty, _allow_empty);
		}
	}
	if (zend_is_true(&allow_empty) && DAO_IS_EMPTY_STRING(&value)) {
		zval_ptr_dtor(&allow_empty);
		zval_ptr_dtor(&value);
		RETURN_TRUE;
	}
	zval_ptr_dtor(&allow_empty);

	RETURN_ON_FAILURE(dao_validation_validator_getoption_helper(&with_attribute, ce, getThis(), "with"));

	DAO_CALL_METHOD(&with_value, validaton, "getvalue", &with_attribute);
	DAO_CALL_METHOD(&valid, getThis(), "valid", &value, &with_value);
	zval_ptr_dtor(&value);
	zval_ptr_dtor(&with_value);

	if (DAO_IS_FALSE(&valid)) {
		RETURN_ON_FAILURE(dao_validation_validator_getoption_helper(&label, ce, getThis(), ISV(label)));
		if (!zend_is_true(&label)) {
			DAO_CALL_METHOD(&label, validaton, "getlabel", attribute);
		}

		RETURN_ON_FAILURE(dao_validation_validator_getoption_helper(&with_label, ce, getThis(), ISV(label)));
		if (!zend_is_true(&with_label)) {
			DAO_CALL_METHOD(&with_label, validaton, "getlabel", &with_attribute);
		}

		array_init_size(&pairs, 2);
		dao_array_update_str(&pairs, SL(":field"), &label, PH_COPY);
		dao_array_update_str(&pairs, SL(":with"), &with_label, PH_COPY);
		zval_ptr_dtor(&label);
		zval_ptr_dtor(&with_label);

		RETURN_ON_FAILURE(dao_validation_validator_getoption_helper(&message_str, ce, getThis(), ISV(message)));
		if (!zend_is_true(&message_str)) {
			RETURN_ON_FAILURE(dao_validation_getdefaultmessage_helper(&message_str, Z_OBJCE_P(validaton), validaton, "Confirmation"));
		}

		RETURN_ON_FAILURE(dao_validation_validator_getoption_helper(&code, ce, getThis(), ISV(code)));
		if (Z_TYPE(code) == IS_NULL) {
			ZVAL_LONG(&code, 0);
		}

		DAO_CALL_FUNCTION(&prepared, "strtr", &message_str, &pairs);
		zval_ptr_dtor(&message_str);
		zval_ptr_dtor(&pairs);

		dao_validation_message_construct_helper(&message, &prepared, attribute, "Confirmation", &code);
		zval_ptr_dtor(&prepared);

		DAO_CALL_METHOD(NULL, validaton, "appendmessage", &message);
		zval_ptr_dtor(&message);
		RETVAL_FALSE;
	} else {
		RETVAL_TRUE;
	}
	zval_ptr_dtor(&with_attribute);
}

/**
 * Executes the validation
 *
 * @param string $value
 * @param string $with_value
 * @return boolean
 */
PHP_METHOD(Dao_Validation_Validator_Confirmation, valid){

	zval *value, *with_value;

	dao_fetch_params(0, 2, 0, &value, &with_value);

	if (!DAO_IS_EQUAL(value, with_value)) {
		RETURN_FALSE;
	}

	RETURN_TRUE;
}
