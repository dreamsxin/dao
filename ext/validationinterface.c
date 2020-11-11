
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

#include "validationinterface.h"
#include "kernel/main.h"

zend_class_entry *dao_validationinterface_ce;

static const zend_function_entry dao_validationinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_ValidationInterface, validate, arginfo_dao_validationinterface_validate)
	PHP_ABSTRACT_ME(Dao_ValidationInterface, add, arginfo_dao_validationinterface_add)
	PHP_ABSTRACT_ME(Dao_ValidationInterface, getLabel, arginfo_dao_validationinterface_getlabel)
	PHP_ABSTRACT_ME(Dao_ValidationInterface, getValue, arginfo_dao_validationinterface_getvalue)
	PHP_ABSTRACT_ME(Dao_ValidationInterface, appendMessage, arginfo_dao_validationinterface_appendmessage)
	PHP_ABSTRACT_ME(Dao_ValidationInterface, getMessages, arginfo_empty)
	PHP_FE_END
};

/**
 * Dao\Mvc\Model\ValidatorInterface initializer
 */
DAO_INIT_CLASS(Dao_ValidationInterface){

	DAO_REGISTER_INTERFACE(Dao, ValidationInterface, validationinterface, dao_validationinterface_method_entry);

	return SUCCESS;
}

/**
 * Validate a set of data according to a set of rules
 *
 * @param array|object $data
 * @param object $entity
 * @return Dao\Validation\Message\Group
 */
DAO_DOC_METHOD(Dao_ValidationInterface, validate);

/**
 * Adds a validator to a field
 *
 * @param string $attribute
 * @param Dao\Validation\ValidatorInterface
 * @return Dao\Validation
 */
DAO_DOC_METHOD(Dao_ValidationInterface, add);

/**
 * Get label for field
 *
 * @param string|array field
 * @return string
 */
DAO_DOC_METHOD(Dao_ValidationInterface, getLabel);

/**
 * Gets the a value to validate in the array/object data source
 *
 * @param string $attribute
 * @return mixed
 */
DAO_DOC_METHOD(Dao_ValidationInterface, getValue);

/**
 * Appends a message to the messages list
 *
 * @param Dao\Validation\MessageInterface $message
 * @return Dao\Validation
 */
DAO_DOC_METHOD(Dao_ValidationInterface, appendMessages);

/**
 * Returns the registered validators
 *
 * @return Dao\Validation\Message\Group
 */
DAO_DOC_METHOD(Dao_ValidationInterface, getMessages);
