
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

#include "validation/validator/json.h"
#include "validation/validator.h"
#include "validation/validatorinterface.h"
#include "validation/message.h"
#include "validation/exception.h"
#include "validation.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"
#include "kernel/concat.h"
#include "kernel/operators.h"
#include "kernel/array.h"

#include "interned-strings.h"

/**
 * Dao\Validation\Validator\Json
 *
 * Checks if a value has a correct JSON format
 *
 *<code>
 *use Dao\Validation\Validator\Json as JsonValidator;
 *
 *$validator->add('json', new JsonValidator(array(
 *   'keys' => array('name'),
 *   'message' => 'The json is not valid'
 *)));
 *</code>
 */
zend_class_entry *dao_validation_validator_json_ce;

PHP_METHOD(Dao_Validation_Validator_Json, validate);
PHP_METHOD(Dao_Validation_Validator_Json, valid);

static const zend_function_entry dao_validation_validator_json_method_entry[] = {
	PHP_ME(Dao_Validation_Validator_Json, validate, arginfo_dao_validation_validatorinterface_validate, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Validation_Validator_Json, valid, arginfo_dao_validation_validatorinterface_valid, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Validation\Validator\Json initializer
 */
DAO_INIT_CLASS(Dao_Validation_Validator_Json){

	DAO_REGISTER_CLASS_EX(Dao\\Validation\\Validator, Json, validation_validator_json, dao_validation_validator_ce, dao_validation_validator_json_method_entry, 0);

	zend_class_implements(dao_validation_validator_json_ce, 1, dao_validation_validatorinterface_ce);

	return SUCCESS;
}

/**
 * Executes the validation
 *
 * @param Dao\Validation $validator
 * @param string $attribute
 * @return boolean
 */
PHP_METHOD(Dao_Validation_Validator_Json, validate){

	zval *validaton, *attribute, *_allow_empty = NULL, value = {}, allow_empty = {}, valid = {}, label = {}, pairs = {}, message_str = {}, code = {}, prepared = {}, message = {};
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
			RETURN_ON_FAILURE(dao_validation_getdefaultmessage_helper(&message_str, Z_OBJCE_P(validaton), validaton, "Json"));
		}

		RETURN_ON_FAILURE(dao_validation_validator_getoption_helper(&code, ce, getThis(), ISV(code)));
		if (Z_TYPE(code) == IS_NULL) {
			ZVAL_LONG(&code, 0);
		}

		DAO_CALL_FUNCTION(&prepared, "strtr", &message_str, &pairs);
		zval_ptr_dtor(&message_str);
		zval_ptr_dtor(&pairs);

		dao_validation_message_construct_helper(&message, &prepared, attribute, "Json", &code);
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
PHP_METHOD(Dao_Validation_Validator_Json, valid){

	zval *value, assoc = {}, valid = {}, json = {}, *constant, ret = {}, option = {}, keys = {};

	dao_fetch_params(0, 1, 0, &value);

	ZVAL_TRUE(&valid);
	ZVAL_TRUE(&assoc);

	DAO_CALL_FUNCTION(&json, "json_decode", value, &assoc);

	if (Z_TYPE(json) == IS_NULL) {
		if ((constant = zend_get_constant_str(SL("JSON_ERROR_NONE"))) != NULL) {
			DAO_CALL_FUNCTION(&ret, "json_last_error");

			if (!DAO_IS_EQUAL(&ret, constant)) {
				ZVAL_FALSE(&valid);
			}
		}
	}

	if (!zend_is_true(&valid)) {
		zval_ptr_dtor(&json);
		RETURN_FALSE;
	}

	RETVAL_TRUE;

	ZVAL_STRING(&option, "keys");

	DAO_CALL_METHOD(&keys, getThis(), "getoption", &option);
	zval_ptr_dtor(&option);

	if (Z_TYPE(keys) != IS_NULL) {
		DAO_CALL_FUNCTION(&ret, "array_key_exists", &keys, &json);
		zval_ptr_dtor(&keys);
		if (!zend_is_true(&ret)) {
			RETVAL_FALSE;
		}
	}

	zval_ptr_dtor(&json);
}
