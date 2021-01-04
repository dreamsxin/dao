
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

#include "validation/validator/stringlength.h"
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
#include "kernel/string.h"
#include "kernel/exception.h"
#include "kernel/object.h"
#include "kernel/array.h"

#include "interned-strings.h"

/**
 * Dao\Validation\Validator\StringLength
 *
 * Validates that a string has the specified maximum and minimum constraints
 *
 *<code>
 *use Dao\Validation\Validator\StringLength as StringLength;
 *
 *$validation->add('name_last', new StringLength(array(
 *      'max' => 50,
 *      'min' => 2,
 *      'messageMaximum' => 'We don\'t like really long names',
 *      'messageMinimum' => 'We want more than just their initials'
 *)));
 *</code>
 *
 */
zend_class_entry *dao_validation_validator_stringlength_ce;

PHP_METHOD(Dao_Validation_Validator_StringLength, validate);
PHP_METHOD(Dao_Validation_Validator_StringLength, valid);

static const zend_function_entry dao_validation_validator_stringlength_method_entry[] = {
	PHP_ME(Dao_Validation_Validator_StringLength, validate, arginfo_dao_validation_validatorinterface_validate, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Validation_Validator_StringLength, valid, arginfo_dao_validation_validatorinterface_valid, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Validation\Validator\StringLength initializer
 */
DAO_INIT_CLASS(Dao_Validation_Validator_StringLength){

	DAO_REGISTER_CLASS_EX(Dao\\Validation\\Validator, StringLength, validation_validator_stringlength, dao_validation_validator_ce, dao_validation_validator_stringlength_method_entry, 0);

	zend_class_implements(dao_validation_validator_stringlength_ce, 1, dao_validation_validatorinterface_ce);

	return SUCCESS;
}

/**
 * Executes the validation
 *
 * @param Dao\Validation $validator
 * @param string $attribute
 * @return boolean
 */
PHP_METHOD(Dao_Validation_Validator_StringLength, validate)
{
	zval *validaton, *attribute, *_allow_empty = NULL, value = {}, allow_empty = {}, valid = {}, type = {}, maximum = {}, minimum = {}, label = {};
	zval code = {}, pairs = {}, message_str = {}, prepared = {}, message = {};
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

	RETURN_ON_FAILURE(dao_validation_validator_getoption_helper(&maximum, ce, getThis(), "max"));
	RETURN_ON_FAILURE(dao_validation_validator_getoption_helper(&minimum, ce, getThis(), "min"));

	DAO_CALL_SELF(&valid, "valid", &value, &minimum, &maximum);

	if (DAO_IS_FALSE(&valid)) {
		dao_read_property(&type, getThis(), SL("_type"), PH_READONLY);

		RETURN_ON_FAILURE(dao_validation_validator_getoption_helper(&label, ce, getThis(), ISV(label)));
		if (!zend_is_true(&label)) {
			DAO_CALL_METHOD(&label, validaton, "getlabel", attribute);
		}

		RETURN_ON_FAILURE(dao_validation_validator_getoption_helper(&code, ce, getThis(), ISV(code)));
		if (Z_TYPE(code) <= IS_NULL) {
			ZVAL_LONG(&code, 0);
		}

		array_init(&pairs);
		dao_array_update_str(&pairs, SL(":field"), &label, PH_COPY);
		zval_ptr_dtor(&label);

		if (dao_compare_strict_string(&type, SL("TooLong"))) {
			dao_array_update_str(&pairs, SL(":max"), &maximum, PH_COPY);

			RETURN_ON_FAILURE(dao_validation_validator_getoption_helper(&message_str, ce, getThis(), "messageMaximum"));
			if (!zend_is_true(&message_str)) {
				RETURN_ON_FAILURE(dao_validation_getdefaultmessage_helper(&message_str, Z_OBJCE_P(validaton), validaton, "TooLong"));
			}

			DAO_CALL_FUNCTION(&prepared, "strtr", &message_str, &pairs);

			dao_validation_message_construct_helper(&message, &prepared, attribute, "TooLong", &code);
		} else {
			dao_array_update_str(&pairs, SL(":min"), &minimum, PH_COPY);

			RETURN_ON_FAILURE(dao_validation_validator_getoption_helper(&message_str, ce, getThis(), "messageMinimum"));
			if (!zend_is_true(&message_str)) {
				RETURN_ON_FAILURE(dao_validation_getdefaultmessage_helper(&message_str, Z_OBJCE_P(validaton), validaton, "TooShort"));
			}

			DAO_CALL_FUNCTION(&prepared, "strtr", &message_str, &pairs);

			dao_validation_message_construct_helper(&message, &prepared, attribute, "TooShort", &code);
		}
		zval_ptr_dtor(&message_str);
		zval_ptr_dtor(&prepared);
		zval_ptr_dtor(&pairs);

		DAO_CALL_METHOD(NULL, validaton, "appendmessage", &message);
		zval_ptr_dtor(&message);
		RETVAL_FALSE;
	} else {
		RETVAL_TRUE;
	}
	zval_ptr_dtor(&minimum);
	zval_ptr_dtor(&maximum);
}

/**
 * Executes the validation
 *
 * @param string $value
 * @param int $minimum
 * @param int $maximum
 * @return boolean
 */
PHP_METHOD(Dao_Validation_Validator_StringLength, valid){

	zval *value, *minimum = NULL, *maximum = NULL, length = {}, valid = {};

	dao_fetch_params(0, 1, 2, &value, &minimum, &maximum);

	if (!minimum) {
		minimum = &DAO_GLOBAL(z_null);
	}

	if (!maximum) {
		maximum = &DAO_GLOBAL(z_null);
	}

	/* At least one of 'min' or 'max' must be set */
	if (Z_TYPE_P(minimum) == IS_NULL && Z_TYPE_P(maximum) == IS_NULL) {
		DAO_THROW_EXCEPTION_STR(dao_validation_exception_ce, "A minimum or maximum must be set");
		return;
	}

#ifdef DAO_USE_PHP_MBSTRING
	dao_strlen(&length, &value);
#else
	/* Check if mbstring is available to calculate the correct length */
	if (dao_function_exists_ex(SL("mb_strlen")) == SUCCESS) {
		DAO_CALL_FUNCTION(&length, "mb_strlen", value);
	} else {
		dao_fast_strlen(&length, value);
	}
#endif

	/* Maximum length */
	if (Z_TYPE_P(maximum) != IS_NULL) {
		is_smaller_function(&valid, maximum, &length);
		if (DAO_IS_TRUE(&valid)) {
			dao_update_property_str(getThis(), SL("_type"), SL("TooLong"));
			RETURN_FALSE;
		}
	}

	/* Minimum length */
	if (Z_TYPE_P(minimum) != IS_NULL) {
		is_smaller_function(&valid, &length, minimum);
		if (DAO_IS_TRUE(&valid)) {
			dao_update_property_str(getThis(), SL("_type"), SL("TooShort"));
			RETURN_FALSE;
		}
	}

	RETURN_TRUE;
}
