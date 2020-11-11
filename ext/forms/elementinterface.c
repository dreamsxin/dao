
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

#include "forms/elementinterface.h"
#include "kernel/main.h"

zend_class_entry *dao_forms_elementinterface_ce;

static const zend_function_entry dao_forms_elementinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Forms_ElementInterface, __construct, arginfo_dao_forms_elementinterface___construct)
	PHP_ABSTRACT_ME(Dao_Forms_ElementInterface, setForm, arginfo_dao_forms_elementinterface_setform)
	PHP_ABSTRACT_ME(Dao_Forms_ElementInterface, getForm, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Forms_ElementInterface, setName, arginfo_dao_forms_elementinterface_setname)
	PHP_ABSTRACT_ME(Dao_Forms_ElementInterface, getName, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Forms_ElementInterface, setFilters, arginfo_dao_forms_elementinterface_setfilters)
	PHP_ABSTRACT_ME(Dao_Forms_ElementInterface, addFilter, arginfo_dao_forms_elementinterface_addfilter)
	PHP_ABSTRACT_ME(Dao_Forms_ElementInterface, getFilters, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Forms_ElementInterface, addValidators, arginfo_dao_forms_elementinterface_addvalidators)
	PHP_ABSTRACT_ME(Dao_Forms_ElementInterface, addValidator, arginfo_dao_forms_elementinterface_addvalidator)
	PHP_ABSTRACT_ME(Dao_Forms_ElementInterface, getValidators, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Forms_ElementInterface, prepareAttributes, arginfo_dao_forms_elementinterface_prepareattributes)
	PHP_ABSTRACT_ME(Dao_Forms_ElementInterface, setAttribute, arginfo_dao_forms_elementinterface_setattribute)
	PHP_ABSTRACT_ME(Dao_Forms_ElementInterface, getAttribute, arginfo_dao_forms_elementinterface_getattribute)
	PHP_ABSTRACT_ME(Dao_Forms_ElementInterface, setAttributes, arginfo_dao_forms_elementinterface_setattributes)
	PHP_ABSTRACT_ME(Dao_Forms_ElementInterface, getAttributes, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Forms_ElementInterface, setUserOption, arginfo_dao_forms_elementinterface_setuseroption)
	PHP_ABSTRACT_ME(Dao_Forms_ElementInterface, getUserOption, arginfo_dao_forms_elementinterface_getuseroption)
	PHP_ABSTRACT_ME(Dao_Forms_ElementInterface, setUserOptions, arginfo_dao_forms_elementinterface_setuseroptions)
	PHP_ABSTRACT_ME(Dao_Forms_ElementInterface, getUserOptions, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Forms_ElementInterface, setLabel, arginfo_dao_forms_elementinterface_setlabel)
	PHP_ABSTRACT_ME(Dao_Forms_ElementInterface, getLabel, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Forms_ElementInterface, label, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Forms_ElementInterface, setDefault, arginfo_dao_forms_elementinterface_setdefault)
	PHP_ABSTRACT_ME(Dao_Forms_ElementInterface, getDefault, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Forms_ElementInterface, getValue, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Forms_ElementInterface, getMessages, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Forms_ElementInterface, hasMessages, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Forms_ElementInterface, setMessages, arginfo_dao_forms_elementinterface_setmessages)
	PHP_ABSTRACT_ME(Dao_Forms_ElementInterface, appendMessage, arginfo_dao_forms_elementinterface_appendmessage)
	PHP_ABSTRACT_ME(Dao_Forms_ElementInterface, clear, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Forms_ElementInterface, render, arginfo_dao_forms_elementinterface_render)
	PHP_ABSTRACT_ME(Dao_Forms_ElementInterface, toArray, arginfo_empty)
	PHP_FE_END
};

/**
 * Dao\Forms\ElementInterface initializer
 */
DAO_INIT_CLASS(Dao_Forms_ElementInterface){

	DAO_REGISTER_INTERFACE(Dao\\Forms, ElementInterface, forms_elementinterface, dao_forms_elementinterface_method_entry);

	return SUCCESS;
}

/**
 * Sets the parent form to the element
 *
 * @param Dao\Forms\Form $form
 * @return Dao\Forms\ElementInterface
 */
DAO_DOC_METHOD(Dao_Forms_ElementInterface, setForm);

/**
 * Returns the parent form to the element
 *
 * @return Dao\Forms\ElementInterface
 */
DAO_DOC_METHOD(Dao_Forms_ElementInterface, getForm);

/**
 * Sets the element's name
 *
 * @param string $name
 * @return Dao\Forms\ElementInterface
 */
DAO_DOC_METHOD(Dao_Forms_ElementInterface, setName);

/**
 * Returns the element's name
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Forms_ElementInterface, getName);

/**
 * Sets the element's filters
 *
 * @param array|string $filters
 * @return Dao\Forms\ElementInterface
 */
DAO_DOC_METHOD(Dao_Forms_ElementInterface, setFilters);

/**
 * Adds a filter to current list of filters
 *
 * @param string $filter
 * @return Dao\Forms\ElementInterface
 */
DAO_DOC_METHOD(Dao_Forms_ElementInterface, addFilter);

/**
 * Returns the element's filters
 *
 * @return mixed
 */
DAO_DOC_METHOD(Dao_Forms_ElementInterface, getFilters);

/**
 * Adds a group of validators
 *
 * @param Dao\Validation\ValidatorInterface[]
 * @return Dao\Forms\ElementInterface
 */
DAO_DOC_METHOD(Dao_Forms_ElementInterface, addValidators);

/**
 * Adds a validator to the element
 *
 * @param Dao\Validation\ValidatorInterface
 * @return Dao\Forms\ElementInterface
 */
DAO_DOC_METHOD(Dao_Forms_ElementInterface, addValidator);

/**
 * Returns the validators registered for the element
 *
 * @return Dao\Validation\ValidatorInterface[]
 */
DAO_DOC_METHOD(Dao_Forms_ElementInterface, getValidators);

/**
 * Returns an array of prepared attributes for Dao\Tag helpers
 * according to the element's parameters
 *
 * @param array $attributes
 * @param boolean $useChecked
 * @return array
 */
DAO_DOC_METHOD(Dao_Forms_ElementInterface, prepareAttributes);

/**
 * Sets a default attribute for the element
 *
 * @param string $attribute
 * @param mixed $value
 * @return Dao\Forms\ElementInterface
 */
DAO_DOC_METHOD(Dao_Forms_ElementInterface, setAttribute);

/**
 * Returns the value of an attribute if present
 *
 * @param string $attribute
 * @param mixed $defaultValue
 * @return mixed
 */
DAO_DOC_METHOD(Dao_Forms_ElementInterface, getAttribute);

/**
 * Sets default attributes for the element
 *
 * @param array $attributes
 * @return Dao\Forms\ElementInterface
 */
DAO_DOC_METHOD(Dao_Forms_ElementInterface, setAttributes);

/**
 * Returns the default attributes for the element
 *
 * @return array
 */
DAO_DOC_METHOD(Dao_Forms_ElementInterface, getAttributes);

/**
 * Sets an option for the element
 *
 * @param string $option
 * @param mixed $value
 * @return Dao\Forms\ElementInterface
 */
DAO_DOC_METHOD(Dao_Forms_ElementInterface, setUserOption);

/**
 * Returns the value of an option if present
 *
 * @param string $option
 * @param mixed $defaultValue
 * @return mixed
 */
DAO_DOC_METHOD(Dao_Forms_ElementInterface, getUserOption);

/**
 * Sets options for the element
 *
 * @param array $options
 * @return Dao\Forms\ElementInterface
 */
DAO_DOC_METHOD(Dao_Forms_ElementInterface, setUserOptions);

/**
 * Returns the options for the element
 *
 * @return array
 */
DAO_DOC_METHOD(Dao_Forms_ElementInterface, getUserOptions);

/**
 * Sets the element label
 *
 * @param string $label
 * @return Dao\Forms\ElementInterface
 */
DAO_DOC_METHOD(Dao_Forms_ElementInterface, setLabel);

/**
 * Returns the element's label
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Forms_ElementInterface, getLabel);

/**
 * Generate the HTML to label the element
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Forms_ElementInterface, label);

/**
 * Sets a default value in case the form does not use an entity
 * or there is no value available for the element in $_POST
 *
 * @param mixed $value
 * @return Dao\Forms\ElementInterface
 */
DAO_DOC_METHOD(Dao_Forms_ElementInterface, setDefault);

/**
 * Returns the default value assigned to the element
 *
 * @return mixed
 */
DAO_DOC_METHOD(Dao_Forms_ElementInterface, getDefault);

/**
 * Returns the element's value
 *
 * @return mixed
 */
DAO_DOC_METHOD(Dao_Forms_ElementInterface, getValue);

/**
 * Returns the messages that belongs to the element
 * The element needs to be attached to a form
 *
 * @return Dao\Validation\Message\Group
 */
DAO_DOC_METHOD(Dao_Forms_ElementInterface, getMessages);

/**
 * Checks whether there are messages attached to the element
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Forms_ElementInterface, hasMessages);

/**
 * Sets the validation messages related to the element
 *
 * @param Dao\Validation\Message\Group $group
 * @return Dao\Forms\ElementInterface
 */
DAO_DOC_METHOD(Dao_Forms_ElementInterface, setMessages);

/**
 * Appends a message to the internal message list
 *
 * @param Dao\Validation\Message $message
 * @return Dao\Forms\ElementInterface
 */
DAO_DOC_METHOD(Dao_Forms_ElementInterface, appendMessage);

/**
 * Clears every element in the form to its default value
 *
 * @return Dao\Forms\Element
 */
DAO_DOC_METHOD(Dao_Forms_ElementInterface, clear);

/**
 * Renders the element widget
 *
 * @param array $attributes
 * @return string
 */
DAO_DOC_METHOD(Dao_Forms_ElementInterface, render);

/**
 * Returns the element as an array
 *
 * @return array
 */
DAO_DOC_METHOD(Dao_Forms_ElementInterface, toArray);

