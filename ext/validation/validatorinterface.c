
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

#include "validation/validatorinterface.h"
#include "kernel/main.h"

zend_class_entry *dao_validation_validatorinterface_ce;

static const zend_function_entry dao_validation_validatorinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Validation_ValidatorInterface, isSetOption, arginfo_dao_validation_validatorinterface_issetoption)
	PHP_ABSTRACT_ME(Dao_Validation_ValidatorInterface, getOption, arginfo_dao_validation_validatorinterface_getoption)
	PHP_ABSTRACT_ME(Dao_Validation_ValidatorInterface, setOption, arginfo_dao_validation_validatorinterface_setoption)
	PHP_ABSTRACT_ME(Dao_Validation_ValidatorInterface, validate, arginfo_dao_validation_validatorinterface_validate)
	ZEND_FENTRY(valid, NULL, arginfo_dao_validation_validatorinterface_valid, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
	PHP_FE_END
};

/**
 * Dao\Validation\ValidatorInterface initializer
 */
DAO_INIT_CLASS(Dao_Validation_ValidatorInterface){

	DAO_REGISTER_INTERFACE(Dao\\Validation, ValidatorInterface, validation_validatorinterface, dao_validation_validatorinterface_method_entry);

	return SUCCESS;
}

/**
 * Checks if an option is defined
 *
 * @param string $key
 * @return mixed
 */
DAO_DOC_METHOD(Dao_Validation_ValidatorInterface, isSetOption);

/**
 * Returns an option in the validator's options
 * Returns null if the option hasn't been set
 *
 * @param string $key
 * @return mixed
 */
DAO_DOC_METHOD(Dao_Validation_ValidatorInterface, getOption);

/**
 * Sets the validator's option
 *
 * @param string $key
 * @param mixed $value
 */
DAO_DOC_METHOD(Dao_Validation_ValidatorInterface, setOption);

/**
 * Executes the validation
 *
 * @param Dao\ValidationInterface $validator
 * @param string|array $attribute
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Validation_ValidatorInterface, validate);

/**
 * Executes the validation
 *
 * @param mixed ... Variable list
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Validation_ValidatorInterface, valid);
