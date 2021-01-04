
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

#include "forms/element/email.h"
#include "forms/element.h"
#include "forms/elementinterface.h"
#include "forms/element/helpers.h"

#include "kernel/main.h"
#include "kernel/string.h"
#include "kernel/operators.h"

/**
 * Dao\Forms\Element\Email
 *
 * Component INPUT[type=email] for forms
 */
zend_class_entry *dao_forms_element_email_ce;

PHP_METHOD(Dao_Forms_Element_Email, __construct);
PHP_METHOD(Dao_Forms_Element_Email, render);

static const zend_function_entry dao_forms_element_email_method_entry[] = {
	PHP_ME(Dao_Forms_Element_Email, __construct, arginfo_dao_forms_elementinterface___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Forms_Element_Email, render, arginfo_dao_forms_elementinterface_render, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Forms\Element\Email initializer
 */
DAO_INIT_CLASS(Dao_Forms_Element_Email){

	DAO_REGISTER_CLASS_EX(Dao\\Forms\\Element, Email, forms_element_email, dao_forms_element_ce, dao_forms_element_email_method_entry, 0);

	zend_class_implements(dao_forms_element_email_ce, 1, dao_forms_elementinterface_ce);

	return SUCCESS;
}

/**
 * Dao\Forms\Element\Email constructor
 *
 * @param string $name
 * @param array $attributes
 * @param array $options
 * @param array $optionsValues
 */
PHP_METHOD(Dao_Forms_Element_Email, __construct){

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
		ZVAL_STRING(&type, "email");
	} else {
		ZVAL_COPY(&type, _type);
	}

	DAO_CALL_PARENT(NULL, dao_forms_element_email_ce, getThis(), "__construct", name, attributes, options, options_values, &type);
	zval_ptr_dtor(&type);
}

/**
 * Renders the element widget returning html
 *
 * @param array $attributes
 * @return string
 */
PHP_METHOD(Dao_Forms_Element_Email, render){

	dao_forms_element_render_helper("emailfield", 0, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
