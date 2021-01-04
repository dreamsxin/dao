
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

#include "forms/element.h"
#include "forms/elementinterface.h"
#include "forms/exception.h"
#include "forms/element/helpers.h"
#include "validation/message/group.h"
#include "tag.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/exception.h"
#include "kernel/object.h"
#include "kernel/array.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "kernel/concat.h"
#include "kernel/file.h"
#include "kernel/hash.h"
#include "kernel/string.h"

/**
 * Dao\Forms\Element
 *
 * This is a base class for form elements
 */
zend_class_entry *dao_forms_element_ce;

PHP_METHOD(Dao_Forms_Element, __construct);
PHP_METHOD(Dao_Forms_Element, setForm);
PHP_METHOD(Dao_Forms_Element, getForm);
PHP_METHOD(Dao_Forms_Element, setName);
PHP_METHOD(Dao_Forms_Element, getName);
PHP_METHOD(Dao_Forms_Element, setFilters);
PHP_METHOD(Dao_Forms_Element, addFilter);
PHP_METHOD(Dao_Forms_Element, getFilters);
PHP_METHOD(Dao_Forms_Element, addValidators);
PHP_METHOD(Dao_Forms_Element, addValidator);
PHP_METHOD(Dao_Forms_Element, getValidators);
PHP_METHOD(Dao_Forms_Element, prepareAttributes);
PHP_METHOD(Dao_Forms_Element, setAttribute);
PHP_METHOD(Dao_Forms_Element, getAttribute);
PHP_METHOD(Dao_Forms_Element, setAttributes);
PHP_METHOD(Dao_Forms_Element, getAttributes);
PHP_METHOD(Dao_Forms_Element, setUserOption);
PHP_METHOD(Dao_Forms_Element, getUserOption);
PHP_METHOD(Dao_Forms_Element, setUserOptions);
PHP_METHOD(Dao_Forms_Element, getUserOptions);
PHP_METHOD(Dao_Forms_Element, setLabel);
PHP_METHOD(Dao_Forms_Element, getLabel);
PHP_METHOD(Dao_Forms_Element, label);
PHP_METHOD(Dao_Forms_Element, setDefault);
PHP_METHOD(Dao_Forms_Element, getDefault);
PHP_METHOD(Dao_Forms_Element, getValue);
PHP_METHOD(Dao_Forms_Element, setValue);
PHP_METHOD(Dao_Forms_Element, getMessages);
PHP_METHOD(Dao_Forms_Element, hasMessages);
PHP_METHOD(Dao_Forms_Element, setMessages);
PHP_METHOD(Dao_Forms_Element, appendMessage);
PHP_METHOD(Dao_Forms_Element, clear);
PHP_METHOD(Dao_Forms_Element, render);
PHP_METHOD(Dao_Forms_Element, toArray);
PHP_METHOD(Dao_Forms_Element, __toString);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_forms_element_label, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, attributes, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_forms_element_setvalue, 0, 0, 0)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_forms_element_method_entry[] = {
	PHP_ME(Dao_Forms_Element, __construct, arginfo_dao_forms_elementinterface___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Forms_Element, setForm, arginfo_dao_forms_elementinterface_setform, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Forms_Element, getForm, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Forms_Element, setName, arginfo_dao_forms_elementinterface_setname, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Forms_Element, getName, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Forms_Element, setFilters, arginfo_dao_forms_elementinterface_setfilters, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Forms_Element, addFilter, arginfo_dao_forms_elementinterface_addfilter, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Forms_Element, getFilters, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Forms_Element, addValidators, arginfo_dao_forms_elementinterface_addvalidators, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Forms_Element, addValidator, arginfo_dao_forms_elementinterface_addvalidator, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Forms_Element, getValidators, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Forms_Element, prepareAttributes, arginfo_dao_forms_elementinterface_prepareattributes, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Forms_Element, setAttribute, arginfo_dao_forms_elementinterface_setattribute, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Forms_Element, getAttribute, arginfo_dao_forms_elementinterface_getattribute, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Forms_Element, setAttributes, arginfo_dao_forms_elementinterface_setattributes, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Forms_Element, getAttributes, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Forms_Element, setUserOption, arginfo_dao_forms_elementinterface_setuseroption, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Forms_Element, getUserOption, arginfo_dao_forms_elementinterface_getuseroption, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Forms_Element, setUserOptions, arginfo_dao_forms_elementinterface_setuseroptions, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Forms_Element, getUserOptions, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Forms_Element, setLabel, arginfo_dao_forms_elementinterface_setlabel, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Forms_Element, getLabel, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Forms_Element, label, arginfo_dao_forms_element_label, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Forms_Element, setDefault, arginfo_dao_forms_elementinterface_setdefault, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Forms_Element, getDefault, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Forms_Element, getValue, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Forms_Element, setValue, arginfo_dao_forms_element_setvalue, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Forms_Element, getMessages, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Forms_Element, hasMessages, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Forms_Element, setMessages, arginfo_dao_forms_elementinterface_setmessages, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Forms_Element, appendMessage, arginfo_dao_forms_elementinterface_appendmessage, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Forms_Element, clear, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Forms_Element, render, arginfo_dao_forms_elementinterface_render, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Forms_Element, toArray, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Forms_Element, __toString, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Forms\Element initializer
 */
DAO_INIT_CLASS(Dao_Forms_Element){

	DAO_REGISTER_CLASS(Dao\\Forms, Element, forms_element, dao_forms_element_method_entry, 0);

	zend_declare_property_null(dao_forms_element_ce, SL("_form"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_forms_element_ce, SL("_name"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_forms_element_ce, SL("_type"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_forms_element_ce, SL("_value"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_forms_element_ce, SL("_label"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_forms_element_ce, SL("_attributes"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_forms_element_ce, SL("_validators"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_forms_element_ce, SL("_filters"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_forms_element_ce, SL("_options"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_forms_element_ce, SL("_optionsValues"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_forms_element_ce, SL("_messages"), ZEND_ACC_PROTECTED);

	zend_class_implements(dao_forms_element_ce, 1, dao_forms_elementinterface_ce);

	return SUCCESS;
}

/**
 * Dao\Forms\Element constructor
 *
 * @param string $name
 * @param array $attributes
 * @param array $options
 * @param array $optionsValues
 */
PHP_METHOD(Dao_Forms_Element, __construct){

	zval *name, *attributes = NULL, *options = NULL, *options_values = NULL, *type = NULL;

	dao_fetch_params(0, 1, 4, &name, &attributes, &options, &options_values, &type);

	if (Z_TYPE_P(name) != IS_STRING) {
		DAO_THROW_EXCEPTION_STR(dao_forms_exception_ce, "The element's name must be a string");
		return;
	}

	if (attributes && Z_TYPE_P(attributes) == IS_ARRAY) {
		dao_update_property(getThis(), SL("_attributes"), attributes);
	}

	if (options && Z_TYPE_P(options) == IS_ARRAY) {
		dao_update_property(getThis(), SL("_options"), options);
	}

	if (options_values && Z_TYPE_P(options_values) == IS_ARRAY) {
		dao_update_property(getThis(), SL("_optionsValues"), options_values);
	}

	if (type && Z_TYPE_P(type) == IS_STRING) {
		dao_update_property(getThis(), SL("_type"), type);
	}

	dao_update_property(getThis(), SL("_name"), name);
}

/**
 * Sets the parent form to the element
 *
 * @param Dao\Forms\Form $form
 * @return Dao\Forms\ElementInterface
 */
PHP_METHOD(Dao_Forms_Element, setForm){

	zval *form;

	dao_fetch_params(0, 1, 0, &form);

	dao_update_property(getThis(), SL("_form"), form);
	RETURN_THIS();
}

/**
 * Returns the parent form to the element
 *
 * @return Dao\Forms\ElementInterface
 */
PHP_METHOD(Dao_Forms_Element, getForm){


	RETURN_MEMBER(getThis(), "_form");
}

/**
 * Sets the element's name
 *
 * @param string $name
 * @return Dao\Forms\ElementInterface
 */
PHP_METHOD(Dao_Forms_Element, setName){

	zval *name;

	dao_fetch_params(0, 1, 0, &name);

	dao_update_property(getThis(), SL("_name"), name);
	RETURN_THIS();
}

/**
 * Returns the element's name
 *
 * @return string
 */
PHP_METHOD(Dao_Forms_Element, getName){


	RETURN_MEMBER(getThis(), "_name");
}

/**
 * Sets the element's filters
 *
 * @param array|string $filters
 * @return Dao\Forms\ElementInterface
 */
PHP_METHOD(Dao_Forms_Element, setFilters){

	zval *filters;

	dao_fetch_params(0, 1, 0, &filters);

	dao_update_property(getThis(), SL("_filters"), filters);
	RETURN_THIS();
}

/**
 * Adds a filter to current list of filters
 *
 * @param string $filter
 * @return Dao\Forms\ElementInterface
 */
PHP_METHOD(Dao_Forms_Element, addFilter){

	zval *filter, filters = {};

	dao_fetch_params(0, 1, 0, &filter);

	dao_read_property(&filters, getThis(), SL("_filters"), PH_NOISY|PH_READONLY);
	if (Z_TYPE(filters) == IS_ARRAY) {
		dao_update_property_array_append(getThis(), SL("_filters"), filter);
	} else {
		zval new_filters = {};
		array_init_size(&new_filters, 2);
		if (Z_TYPE(filters) == IS_STRING) {
			dao_array_append(&new_filters, &filters, PH_COPY);
		}

		dao_array_append(&new_filters, filter, PH_COPY);
		dao_update_property(getThis(), SL("_filters"), &new_filters);
		zval_ptr_dtor(&new_filters);
	}

	RETURN_THIS();
}

/**
 * Returns the element's filters
 *
 * @return mixed
 */
PHP_METHOD(Dao_Forms_Element, getFilters){


	RETURN_MEMBER(getThis(), "_filters");
}

/**
 * Adds a group of validators
 *
 * @param Dao\Validation\ValidatorInterface[]
 * @return Dao\Forms\ElementInterface
 */
PHP_METHOD(Dao_Forms_Element, addValidators){

	zval *validators, *merge = NULL, current_validators = {};

	dao_fetch_params(0, 1, 1, &validators, &merge);

	if (!merge) {
		merge = &DAO_GLOBAL(z_true);
	}

	if (Z_TYPE_P(validators) != IS_ARRAY) {
		DAO_THROW_EXCEPTION_STR(dao_forms_exception_ce, "The validators parameter must be an array");
		return;
	}

	if (zend_is_true(merge)) {
		zval merged_validators = {};
		dao_read_property(&current_validators, getThis(), SL("_validators"), PH_NOISY|PH_READONLY);
		if (Z_TYPE(current_validators) == IS_ARRAY) {
			dao_fast_array_merge(&merged_validators, &current_validators, validators);
		} else {
			ZVAL_COPY(&merged_validators, validators);
		}

		dao_update_property(getThis(), SL("_validators"), &merged_validators);
		zval_ptr_dtor(&merged_validators);
	} else {
		dao_update_property(getThis(), SL("_validators"), validators);
	}

	RETURN_THIS();
}

/**
 * Adds a validator to the element
 *
 * @param Dao\Validation\ValidatorInterface
 * @return Dao\Forms\ElementInterface
 */
PHP_METHOD(Dao_Forms_Element, addValidator){

	zval *validator;

	dao_fetch_params(0, 1, 0, &validator);

	if (Z_TYPE_P(validator) != IS_OBJECT) {
		DAO_THROW_EXCEPTION_STR(dao_forms_exception_ce, "The validators parameter must be an object");
		return;
	}
	dao_update_property_array_append(getThis(), SL("_validators"), validator);

	RETURN_THIS();
}

/**
 * Returns the validators registered for the element
 *
 * @return Dao\Validation\ValidatorInterface[]
 */
PHP_METHOD(Dao_Forms_Element, getValidators){


	RETURN_MEMBER(getThis(), "_validators");
}

/**
 * Returns an array of prepared attributes for Dao\Tag helpers
 * according to the element's parameters
 *
 * @param array $attributes
 * @param boolean $useChecked
 * @return array
 */
PHP_METHOD(Dao_Forms_Element, prepareAttributes){

	zval *attributes = NULL, *use_checked = NULL, name = {}, widget_attributes = {}, default_attributes = {}, merged_attributes = {}, value = {}, current_value = {};

	dao_fetch_params(1, 0, 2, &attributes, &use_checked);

	if (!attributes || Z_TYPE_P(attributes) == IS_NULL) {
		array_init(&widget_attributes);
		DAO_MM_ADD_ENTRY(&widget_attributes);
	} else {
		DAO_SEPARATE_PARAM(attributes);
		ZVAL_COPY_VALUE(&widget_attributes, attributes);
	}

	if (!use_checked) {
		use_checked = &DAO_GLOBAL(z_false);
	}

	dao_read_property(&name, getThis(), SL("_name"), PH_NOISY|PH_READONLY);

	/**
	 * Create an array of parameters
	 */
	dao_array_update_long(&widget_attributes, 0, &name, PH_COPY);

	/**
	 * Merge passed parameters with default ones
	 */
	dao_read_property(&default_attributes, getThis(), SL("_attributes"), PH_NOISY|PH_READONLY);
	if (Z_TYPE(default_attributes) == IS_ARRAY) {
		dao_fast_array_merge(&merged_attributes, &default_attributes, &widget_attributes);
		DAO_MM_ADD_ENTRY(&merged_attributes);
	} else {
		ZVAL_COPY_VALUE(&merged_attributes, &widget_attributes);
	}

	/**
	 * Get the current element's value
	 */
	DAO_MM_CALL_METHOD(&value, getThis(), "getvalue");
	DAO_MM_ADD_ENTRY(&value);

	/**
	 * If the widget has a value set it as default value
	 */
	if (Z_TYPE(value) != IS_NULL) {
		if (zend_is_true(use_checked)) {
			/**
			 * Check if the element already has a default value, compare it with the one in the
			 * attributes, if they are the same mark the element as checked
			 */
			if (dao_array_isset_fetch_str(&current_value, &merged_attributes, SL("value"), PH_READONLY)) {
				if (DAO_IS_EQUAL(&current_value, &value)) {
					dao_array_update_str_str(&merged_attributes, SL("checked"), SL("checked"), 0);
				}
			} else {
				/**
				 * Evaluate the current value and mark the check as checked
				 */
				if (zend_is_true(&value)) {
					dao_array_update_str_str(&merged_attributes, SL("checked"), SL("checked"), 0);
				}
				dao_array_update_str(&merged_attributes, SL("value"), &value, PH_COPY);
			}
		} else {
			dao_array_update_str(&merged_attributes, SL("value"), &value, PH_COPY);
		}
	}

	RETURN_MM_CTOR(&merged_attributes);
}

/**
 * Sets a default attribute for the element
 *
 * @param string $attribute
 * @param mixed $value
 * @return Dao\Forms\ElementInterface
 */
PHP_METHOD(Dao_Forms_Element, setAttribute){

	zval *attribute, *value;

	dao_fetch_params(0, 2, 0, &attribute, &value);

	dao_update_property_array(getThis(), SL("_attributes"), attribute, value);
	RETURN_THIS();
}

/**
 * Returns the value of an attribute if present
 *
 * @param string $attribute
 * @param mixed $defaultValue
 * @return mixed
 */
PHP_METHOD(Dao_Forms_Element, getAttribute){

	zval *attribute, *default_value = NULL, attributes = {}, value = {};

	dao_fetch_params(0, 1, 1, &attribute, &default_value);

	if (!default_value) {
		default_value = &DAO_GLOBAL(z_null);
	}

	dao_read_property(&attributes, getThis(), SL("_attributes"), PH_NOISY|PH_READONLY);
	if (dao_array_isset_fetch(&value, &attributes, attribute, PH_READONLY)) {
		RETURN_CTOR(&value);
	}

	RETURN_CTOR(default_value);
}

/**
 * Sets default attributes for the element
 *
 * @param array $attributes
 * @return Dao\Forms\ElementInterface
 */
PHP_METHOD(Dao_Forms_Element, setAttributes){

	zval *attributes;

	dao_fetch_params(0, 1, 0, &attributes);

	if (Z_TYPE_P(attributes) != IS_ARRAY) {
		DAO_THROW_EXCEPTION_STR(dao_forms_exception_ce, "Parameter 'attributes' must be an array");
		return;
	}
	DAO_SEPARATE_PARAM(attributes);
	dao_update_property(getThis(), SL("_attributes"), attributes);

	RETURN_THIS();
}

/**
 * Returns the default attributes for the element
 *
 * @return array
 */
PHP_METHOD(Dao_Forms_Element, getAttributes){

	zval attributes = {};

	dao_read_property(&attributes, getThis(), SL("_attributes"), PH_NOISY|PH_READONLY);
	if (Z_TYPE(attributes) != IS_ARRAY) {
		array_init(return_value);
		return;
	}

	RETURN_ZVAL(&attributes, 1, 0);
}

/**
 * Sets an option for the element
 *
 * @param string $option
 * @param mixed $value
 * @return Dao\Forms\ElementInterface
 */
PHP_METHOD(Dao_Forms_Element, setUserOption){

	zval *option, *value;

	dao_fetch_params(0, 2, 0, &option, &value);

	dao_update_property_array(getThis(), SL("_options"), option, value);
	RETURN_THIS();
}

/**
 * Returns the value of an option if present
 *
 * @param string $option
 * @param mixed $defaultValue
 * @return mixed
 */
PHP_METHOD(Dao_Forms_Element, getUserOption){

	zval *option, *default_value = NULL, options = {}, value = {};

	dao_fetch_params(0, 1, 1, &option, &default_value);

	if (!default_value) {
		default_value = &DAO_GLOBAL(z_null);
	}

	dao_read_property(&options, getThis(), SL("_options"), PH_NOISY|PH_READONLY);
	if (dao_array_isset_fetch(&value, &options, option, PH_READONLY)) {
		RETURN_CTOR(&value);
	}

	RETURN_CTOR(default_value);
}

/**
 * Sets options for the element
 *
 * @param array $options
 * @return Dao\Forms\ElementInterface
 */
PHP_METHOD(Dao_Forms_Element, setUserOptions){

	zval *options;

	dao_fetch_params(0, 1, 0, &options);

	if (Z_TYPE_P(options) != IS_ARRAY) {
		DAO_THROW_EXCEPTION_STR(dao_forms_exception_ce, "Parameter 'options' must be an array");
		return;
	}
	dao_update_property(getThis(), SL("_options"), options);

	RETURN_THIS();
}

/**
 * Returns the options for the element
 *
 * @return array
 */
PHP_METHOD(Dao_Forms_Element, getUserOptions){


	RETURN_MEMBER(getThis(), "_options");
}

/**
 * Sets the element label
 *
 * @param string $label
 * @return Dao\Forms\ElementInterface
 */
PHP_METHOD(Dao_Forms_Element, setLabel){

	zval *label;

	dao_fetch_params(0, 1, 0, &label);

	dao_update_property(getThis(), SL("_label"), label);
	RETURN_THIS();
}

/**
 * Returns the element's label
 *
 * @return string
 */
PHP_METHOD(Dao_Forms_Element, getLabel){


	RETURN_MEMBER(getThis(), "_label");
}

/**
 * Generate the HTML to label the element
 *
 * @return string
 */
PHP_METHOD(Dao_Forms_Element, label){

	zval *attributes = NULL, label = {}, name = {}, escaped = {}, html = {}, *value;
	zend_string *str_key;
	ulong idx;

	dao_fetch_params(0, 0, 1, &attributes);

	dao_read_property(&label, getThis(), SL("_label"), PH_READONLY);

	/**
	 * Check if there is an 'id' attribute defined
	 */
	if (!attributes || !dao_array_isset_fetch_str(&name, attributes, SL("id"), PH_READONLY)) {
		 dao_read_property(&name, getThis(), SL("_name"), PH_READONLY);
	}

	dao_htmlspecialchars(&escaped, &name, NULL, NULL);

	DAO_CONCAT_SVS(&html, "<label for=\"", &escaped, "\"");
	zval_ptr_dtor(&escaped);

	if (attributes && Z_TYPE_P(attributes) == IS_ARRAY) {
		ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(attributes), idx, str_key, value) {
			zval key = {}, escaped_value = {};
			if (str_key) {
				ZVAL_STR(&key, str_key);
			} else {
				ZVAL_LONG(&key, idx);
			}
			if (Z_TYPE(key) != IS_LONG) {
				dao_htmlspecialchars(&escaped_value, value, NULL, NULL);
				DAO_SCONCAT_SVSVS(&html, " ", &key, "=\"", &escaped_value, "\"");
				zval_ptr_dtor(&escaped_value);
			}
		} ZEND_HASH_FOREACH_END();
	}

	/**
	 * Use the default label or leave the same name as label
	 */
	if (zend_is_true(&label)) {
		DAO_CONCAT_VSVS(return_value, &html, ">", &label, "</label>");
	} else {
		DAO_CONCAT_VSVS(return_value, &html, ">", &name, "</label>");
	}
	zval_ptr_dtor(&html);
}

/**
 * Sets a default value in case the form does not use an entity
 * or there is no value available for the element in $_POST
 *
 * @param mixed $value
 * @return Dao\Forms\ElementInterface
 */
PHP_METHOD(Dao_Forms_Element, setDefault){

	zval *value;

	dao_fetch_params(0, 1, 0, &value);

	dao_update_property(getThis(), SL("_value"), value);
	RETURN_THIS();
}

/**
 * Returns the default value assigned to the element
 *
 * @return mixed
 */
PHP_METHOD(Dao_Forms_Element, getDefault){


	RETURN_MEMBER(getThis(), "_value");
}

/**
 * Returns the element's value
 *
 * @return mixed
 */
PHP_METHOD(Dao_Forms_Element, getValue){

	zval name = {}, form = {}, has_default_value = {}, value = {};

	dao_read_property(&name, getThis(), SL("_name"), PH_NOISY|PH_READONLY);

	/**
	 * Get the related form
	 */
	dao_read_property(&form, getThis(), SL("_form"), PH_NOISY|PH_READONLY);
	if (Z_TYPE(form) == IS_OBJECT) {
		/**
		 * Check if the tag has a default value
		 */
		DAO_CALL_CE_STATIC(&has_default_value, dao_tag_ce, "hasvalue", &name);
		if (!zend_is_true(&has_default_value)) {
			/**
			 * Gets the possible value for the widget
			 */
			DAO_CALL_METHOD(&value, &form, "getvalue", &name);
		}
	}

	/**
	 * Assign the default value if there is no form available
	 */
	if (Z_TYPE(value) <= IS_NULL) {
		 dao_read_property(&value, getThis(), SL("_value"), PH_COPY);
	}

	RETURN_ZVAL(&value, 0, 0);
}

/**
 * Sets the element's value
 *
 */
PHP_METHOD(Dao_Forms_Element, setValue){

	zval *value, name = {}, form = {};

	dao_fetch_params(0, 1, 0, &value);

	dao_read_property(&name, getThis(), SL("_name"), PH_NOISY|PH_READONLY);

	/**
	 * Get the related form
	 */
	dao_read_property(&form, getThis(), SL("_form"), PH_NOISY|PH_READONLY);
	if (Z_TYPE(form) == IS_OBJECT) {
		DAO_CALL_METHOD(NULL, &form, "setvalue", &name, value);
		return;
	}
	dao_update_property(getThis(), SL("_value"), value);
}

/**
 * Returns the messages that belongs to the element
 * The element needs to be attached to a form
 *
 * @return Dao\Validation\Message\Group
 */
PHP_METHOD(Dao_Forms_Element, getMessages){

	zval messages = {};

	/**
	 * Get the related form
	 */
	dao_read_property(&messages, getThis(), SL("_messages"), PH_NOISY|PH_READONLY);
	if (Z_TYPE(messages) == IS_OBJECT) {
		RETURN_CTOR(&messages);
	}

	object_init_ex(return_value, dao_validation_message_group_ce);
	DAO_CALL_METHOD(NULL, return_value, "__construct");

	dao_update_property(getThis(), SL("_messages"), return_value);
}

/**
 * Checks whether there are messages attached to the element
 *
 * @return boolean
 */
PHP_METHOD(Dao_Forms_Element, hasMessages){

	zval messages = {};

	/**
	 * Get the related form
	 */
	dao_read_property(&messages, getThis(), SL("_messages"), PH_NOISY|PH_READONLY);
	if (Z_TYPE(messages) == IS_OBJECT) {
		if (dao_fast_count_ev(&messages)) {
			RETURN_TRUE;
		}
	}

	RETURN_FALSE;
}

/**
 * Sets the validation messages related to the element
 *
 * @param Dao\Validation\Message\Group $group
 * @return Dao\Forms\ElementInterface
 */
PHP_METHOD(Dao_Forms_Element, setMessages){

	zval *group;

	dao_fetch_params(0, 1, 0, &group);

	if (Z_TYPE_P(group) != IS_OBJECT) {
		DAO_THROW_EXCEPTION_STR(dao_forms_exception_ce, "The message group is not valid");
		return;
	}
	dao_update_property(getThis(), SL("_messages"), group);

	RETURN_THIS();
}

/**
 * Appends a message to the internal message list
 *
 * @param Dao\Validation\Message $message
 * @return Dao\Forms\ElementInterface
 */
PHP_METHOD(Dao_Forms_Element, appendMessage){

	zval *message, messages;

	dao_fetch_params(0, 1, 0, &message);

	dao_read_property(&messages, getThis(), SL("_messages"), PH_COPY);
	if (Z_TYPE(messages) != IS_OBJECT) {
		object_init_ex(&messages, dao_validation_message_group_ce);
		DAO_CALL_METHOD(NULL, &messages, "__construct");

		dao_update_property(getThis(), SL("_messages"), &messages);
	}

	DAO_CALL_METHOD(NULL, &messages, "appendmessage", message);
	zval_ptr_dtor(&messages);

	RETURN_THIS();
}

/**
 * Clears every element in the form to its default value
 *
 * @return Dao\Forms\Element
 */
PHP_METHOD(Dao_Forms_Element, clear)
{
	zval name = {};
	dao_read_property(&name, getThis(), SL("_name"), PH_NOISY|PH_READONLY);
	DAO_CALL_METHOD(NULL, getThis(), "setvalue", &name);
	DAO_CALL_CE_STATIC(NULL, dao_tag_ce, "setdefault", &name, &DAO_GLOBAL(z_null));
	RETURN_THIS();
}

/**
 * Returns a element all attributes
 *
 * @return array
 */
PHP_METHOD(Dao_Forms_Element, toArray)
{
	zval name = {}, type = {}, value = {}, label = {}, attributes = {}, validators = {}, filters = {}, options = {}, options_values = {}, messages = {};

	dao_read_property(&name, getThis(), SL("_name"), PH_NOISY|PH_READONLY);
	dao_read_property(&type, getThis(), SL("_type"), PH_NOISY|PH_READONLY);
	dao_read_property(&value, getThis(), SL("_value"), PH_NOISY|PH_READONLY);
	dao_read_property(&label, getThis(), SL("_label"), PH_NOISY|PH_READONLY);
	dao_read_property(&attributes, getThis(), SL("_attributes"), PH_NOISY|PH_READONLY);
	dao_read_property(&validators, getThis(), SL("_validators"), PH_NOISY|PH_READONLY);
	dao_read_property(&filters, getThis(), SL("_filters"), PH_NOISY|PH_READONLY);
	dao_read_property(&options, getThis(), SL("_options"), PH_NOISY|PH_READONLY);
	dao_read_property(&options_values, getThis(), SL("_optionsValues"), PH_NOISY|PH_READONLY);
	dao_read_property(&messages, getThis(), SL("_messages"), PH_NOISY|PH_READONLY);

	array_init(return_value);
	if (DAO_IS_NOT_EMPTY(&name)) {
		dao_array_update_str(return_value, SL("name"), &name, PH_COPY);
	}
	if (DAO_IS_NOT_EMPTY(&type)) {
		dao_array_update_str(return_value, SL("type"), &type, PH_COPY);
	}
	if (DAO_IS_NOT_EMPTY(&value)) {
		dao_array_update_str(return_value, SL("value"), &value, PH_COPY);
	}
	if (DAO_IS_NOT_EMPTY(&label)) {
		dao_array_update_str(return_value, SL("label"), &label, PH_COPY);
	}
	if (DAO_IS_NOT_EMPTY(&attributes)) {
		dao_array_update_str(return_value, SL("attributes"), &attributes, PH_COPY);
	}
	if (DAO_IS_NOT_EMPTY(&validators)) {
		dao_array_update_str(return_value, SL("validators"), &validators, PH_COPY);
	}
	if (DAO_IS_NOT_EMPTY(&filters)) {
		dao_array_update_str(return_value, SL("filters"), &filters, PH_COPY);
	}
	if (DAO_IS_NOT_EMPTY(&options)) {
		dao_array_update_str(return_value, SL("options"), &options, PH_COPY);
	}
	if (DAO_IS_NOT_EMPTY(&options_values)) {
		dao_array_update_str(return_value, SL("optionsValues"), &options_values, PH_COPY);
	}
	if (DAO_IS_NOT_EMPTY(&messages)) {
		dao_array_update_str(return_value, SL("messages"), &messages, PH_COPY);
	}
}

/**
 * Renders the element widget returning html
 *
 * @param array $attributes
 * @return string
 */
PHP_METHOD(Dao_Forms_Element, render){

	zval type = {};

	dao_read_property(&type, getThis(), SL("_type"), PH_NOISY|PH_READONLY);
	dao_forms_element_render_helper(Z_STRVAL(type), 1, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}

/**
 * Magic method __toString renders the widget without attributes
 *
 * @return string
 */
PHP_METHOD(Dao_Forms_Element, __toString)
{
	zval *m;

	if (FAILURE == dao_call_method(return_value, getThis(), "render", 0, NULL)) {
		if (EG(exception)) {
#if PHP_VERSION_ID >= 80000
			m = zend_read_property(EG(exception)->ce, EG(exception), SL("message"), 1, NULL);
#else
			zval e = {};
			ZVAL_OBJ(&e, EG(exception));
			m = zend_read_property(Z_OBJCE(e), &e, SL("message"), 1, NULL);
#endif
			Z_TRY_ADDREF_P(m);
			if (Z_TYPE_P(m) != IS_STRING) {
				convert_to_string_ex(m);
			}

			zend_clear_exception();
			zend_error(E_ERROR, "%s", Z_STRVAL_P(m));
		}
	}
}
