
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

#include "forms/element/check.h"
#include "forms/element.h"
#include "forms/elementinterface.h"
#include "forms/element/helpers.h"

#include "kernel/main.h"
#include "kernel/string.h"
#include "kernel/operators.h"

/**
 * Dao\Forms\Element\Check
 *
 * Component INPUT[type=check] for forms
 */
zend_class_entry *dao_forms_element_check_ce;

PHP_METHOD(Dao_Forms_Element_Check, __construct);
PHP_METHOD(Dao_Forms_Element_Check, render);

static const zend_function_entry dao_forms_element_check_method_entry[] = {
	PHP_ME(Dao_Forms_Element_Check, __construct, arginfo_dao_forms_elementinterface___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Forms_Element_Check, render, arginfo_dao_forms_elementinterface_render, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Forms\Element\Check initializer
 */
DAO_INIT_CLASS(Dao_Forms_Element_Check){

	DAO_REGISTER_CLASS_EX(Dao\\Forms\\Element, Check, forms_element_check, dao_forms_element_ce, dao_forms_element_check_method_entry, 0);

	zend_class_implements(dao_forms_element_check_ce, 1, dao_forms_elementinterface_ce);

	return SUCCESS;
}

/**
 * Dao\Forms\Element\Check constructor
 *
 * @param string $name
 * @param array $attributes
 * @param array $options
 * @param array $optionsValues
 */
PHP_METHOD(Dao_Forms_Element_Check, __construct){

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
		ZVAL_STRING(&type, "check");
	} else {
		ZVAL_COPY_VALUE(&type, _type);
	}

	DAO_CALL_PARENT(NULL, dao_forms_element_check_ce, getThis(), "__construct", name, attributes, options, options_values, &type);
}

/**
 * Renders the element widget returning html
 *
 * @param array $attributes
 * @return string
 */
PHP_METHOD(Dao_Forms_Element_Check, render){

	dao_forms_element_render_helper("checkfield", 1, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}

