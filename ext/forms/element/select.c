
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

#include "forms/element/select.h"
#include "forms/element.h"
#include "forms/elementinterface.h"
#include "tag/select.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/string.h"
#include "kernel/operators.h"

/**
 * Dao\Forms\Element\Select
 *
 * Component SELECT (choice) for forms
 */
zend_class_entry *dao_forms_element_select_ce;

PHP_METHOD(Dao_Forms_Element_Select, __construct);
PHP_METHOD(Dao_Forms_Element_Select, setOptions);
PHP_METHOD(Dao_Forms_Element_Select, getOptions);
PHP_METHOD(Dao_Forms_Element_Select, addOption);
PHP_METHOD(Dao_Forms_Element_Select, render);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_forms_element_select_setoptions, 0, 0, 1)
	ZEND_ARG_INFO(0, options)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_forms_element_select_addoption, 0, 0, 1)
	ZEND_ARG_INFO(0, option)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_forms_element_select_method_entry[] = {
	PHP_ME(Dao_Forms_Element_Select, __construct, arginfo_dao_forms_elementinterface___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Forms_Element_Select, setOptions, arginfo_dao_forms_element_select_setoptions, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Forms_Element_Select, getOptions, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Forms_Element_Select, addOption, arginfo_dao_forms_element_select_addoption, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Forms_Element_Select, render, arginfo_dao_forms_elementinterface_render, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Forms\Element\Select initializer
 */
DAO_INIT_CLASS(Dao_Forms_Element_Select){

	DAO_REGISTER_CLASS_EX(Dao\\Forms\\Element, Select, forms_element_select, dao_forms_element_ce, dao_forms_element_select_method_entry, 0);

	zend_class_implements(dao_forms_element_select_ce, 1, dao_forms_elementinterface_ce);

	return SUCCESS;
}

/**
 * Dao\Forms\Element\Select constructor
 *
 * @param string $name
 * @param array $attributes
 * @param array $options
 * @param array $optionsValues
 */
PHP_METHOD(Dao_Forms_Element_Select, __construct){

	zval *name, *attributes = NULL, *options = NULL, *options_values = NULL, *_type = NULL, type = {};

	dao_fetch_params(0, 1, 4, &name, &attributes, &options, &options_values, &_type);

	if (!attributes) {
		attributes = &DAO_GLOBAL(z_null);
	}

	if (!options) {
		options = &DAO_GLOBAL(z_null);
	}

	if (!options_values) {
		options_values = &DAO_GLOBAL(z_null);
	}

	if (!_type || DAO_IS_EMPTY(_type)) {
		ZVAL_STRING(&type, "select");
	} else {
		ZVAL_COPY(&type, _type);
	}

	DAO_CALL_PARENT(NULL, dao_forms_element_select_ce, getThis(), "__construct", name, attributes, options, options_values, &type);
	zval_ptr_dtor(&type);
}

/**
 * Set the choice's options
 *
 * @param array|object $options
 * @return Dao\Forms\Element
 */
PHP_METHOD(Dao_Forms_Element_Select, setOptions){

	zval *options;

	dao_fetch_params(0, 1, 0, &options);

	dao_update_property(getThis(), SL("_optionsValues"), options);
	RETURN_THIS();
}

/**
 * Returns the choices' options
 *
 * @return array|object
 */
PHP_METHOD(Dao_Forms_Element_Select, getOptions){


	RETURN_MEMBER(getThis(), "_optionsValues");
}

/**
 * Adds an option to the current options
 *
 * @param array $option
 * @return $this
 */
PHP_METHOD(Dao_Forms_Element_Select, addOption){

	zval *option, values = {}, tmp = {};

	dao_fetch_params(0, 1, 0, &option);
	DAO_ENSURE_IS_ARRAY(option);

	dao_read_property(&values, getThis(), SL("_optionsValues"), PH_NOISY|PH_READONLY);

	if (Z_TYPE(values) != IS_ARRAY) {
		ZVAL_COPY_VALUE(&tmp, option);
	} else {
		add_function(&tmp, option, &values);
	}

	dao_update_property(getThis(), SL("_optionsValues"), &tmp);
	RETURN_THIS();
}

/**
 * Renders the element widget returning html
 *
 * @param array $attributes
 * @return string
 */
PHP_METHOD(Dao_Forms_Element_Select, render){

	zval *attributes = NULL, options = {}, widget_attributes = {};

	dao_fetch_params(0, 1, 0, &attributes);

	if (!attributes) {
		attributes = &DAO_GLOBAL(z_null);
	}

	dao_read_property(&options, getThis(), SL("_optionsValues"), PH_NOISY|PH_READONLY);

	/**
	 * Merged passed attributes with previously defined ones
	 */
	DAO_CALL_METHOD(&widget_attributes, getThis(), "prepareattributes", attributes);
	DAO_RETURN_CALL_CE_STATIC(dao_tag_select_ce, "selectfield", &widget_attributes, &options);
	return;
}
