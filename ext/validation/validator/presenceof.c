
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

#include "validation/validator/presenceof.h"
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
 * Dao\Validation\Validator\PresenceOf
 *
 * Validates that a value is not null or empty string
 *
 *<code>
 *use Dao\Validation\Validator\PresenceOf;
 *
 *$validator->add('name', new PresenceOf(array(
 *   'message' => 'The name is required'
 *)));
 *</code>
 */
zend_class_entry *dao_validation_validator_presenceof_ce;

PHP_METHOD(Dao_Validation_Validator_PresenceOf, validate);
PHP_METHOD(Dao_Validation_Validator_PresenceOf, valid);

static const zend_function_entry dao_validation_validator_presenceof_method_entry[] = {
	PHP_ME(Dao_Validation_Validator_PresenceOf, validate, arginfo_dao_validation_validatorinterface_validate, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Validation_Validator_PresenceOf, valid, arginfo_dao_validation_validatorinterface_valid, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Validation\Validator\PresenceOf initializer
 */
DAO_INIT_CLASS(Dao_Validation_Validator_PresenceOf){

	DAO_REGISTER_CLASS_EX(Dao\\Validation\\Validator, PresenceOf, validation_validator_presenceof, dao_validation_validator_ce, dao_validation_validator_presenceof_method_entry, 0);

	zend_class_implements(dao_validation_validator_presenceof_ce, 1, dao_validation_validatorinterface_ce);

	return SUCCESS;
}

/**
 * Executes the validation
 *
 * @param Dao\Validation $validator
 * @param string $attribute
 * @return boolean
 */
PHP_METHOD(Dao_Validation_Validator_PresenceOf, validate){

	zval *validaton, *attribute, value = {}, valid = {}, code = {}, message_str = {}, message = {}, label = {}, pairs = {}, prepared = {};
	zend_class_entry *ce = Z_OBJCE_P(getThis());

	dao_fetch_params(0, 2, 0, &validaton, &attribute);
	DAO_VERIFY_INTERFACE_EX(validaton, dao_validationinterface_ce, dao_validation_exception_ce);

	DAO_CALL_METHOD(&value, validaton, "getvalue", attribute);
	DAO_CALL_METHOD(&valid, getThis(), "valid", &value);
	zval_ptr_dtor(&value);

	if (DAO_IS_FALSE(&valid)) {
		RETURN_ON_FAILURE(dao_validation_validator_getoption_helper(&label, ce, getThis(), ISV(label)));
		if (!zend_is_true(&label)) {
			DAO_CALL_METHOD(&label, validaton, "getlabel", attribute);
		}

		array_init_size(&pairs, 1);
		dao_array_update_str(&pairs, SL(":field"), &label, PH_COPY);
		zval_ptr_dtor(&label);

		RETURN_ON_FAILURE(dao_validation_validator_getoption_helper(&message_str, ce, getThis(), ISV(message)));
		if (!zend_is_true(&message_str)) {
			RETURN_ON_FAILURE(dao_validation_getdefaultmessage_helper(&message_str, Z_OBJCE_P(validaton), validaton, "PresenceOf"));
		}

		RETURN_ON_FAILURE(dao_validation_validator_getoption_helper(&code, ce, getThis(), ISV(code)));
		if (Z_TYPE(code) == IS_NULL) {
			ZVAL_LONG(&code, 0);
		}

		DAO_CALL_FUNCTION(&prepared, "strtr", &message_str, &pairs);
		zval_ptr_dtor(&message_str);
		zval_ptr_dtor(&pairs);

		dao_validation_message_construct_helper(&message, &prepared, attribute, "PresenceOf", &code);
		zval_ptr_dtor(&prepared);

		DAO_CALL_METHOD(NULL, validaton, "appendmessage", &message);
		zval_ptr_dtor(&message);
		RETURN_FALSE;
	}

	RETURN_TRUE;
}

/**
 * Executes the validation
 *
 * @param string $value
 * @return boolean
 */
PHP_METHOD(Dao_Validation_Validator_PresenceOf, valid){

	zval *value;

	dao_fetch_params(0, 1, 0, &value);

	if (DAO_IS_NOT_EMPTY(value)) {
		RETURN_TRUE;
	}

	RETURN_FALSE;
}
