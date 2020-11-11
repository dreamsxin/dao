
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

#include "forms/manager.h"
#include "forms/form.h"
#include "forms/exception.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/exception.h"
#include "kernel/fcall.h"
#include "kernel/object.h"
#include "kernel/array.h"
#include "kernel/concat.h"
#include "kernel/hash.h"

/**
 * Dao\Forms\Manager
 *
 * Manages forms within the application. Allowing the developer to access them from
 * any part of the application
 */
zend_class_entry *dao_forms_manager_ce;

PHP_METHOD(Dao_Forms_Manager, __construct);
PHP_METHOD(Dao_Forms_Manager, create);
PHP_METHOD(Dao_Forms_Manager, get);
PHP_METHOD(Dao_Forms_Manager, has);
PHP_METHOD(Dao_Forms_Manager, set);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_forms_manager_create, 0, 0, 0)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, entity)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_forms_manager_get, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_forms_manager_has, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_forms_manager_set, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_INFO(0, form)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_forms_manager_method_entry[] = {
	PHP_ME(Dao_Forms_Manager, __construct, arginfo_empty, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(Dao_Forms_Manager, create, arginfo_dao_forms_manager_create, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Forms_Manager, get, arginfo_dao_forms_manager_get, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Forms_Manager, has, arginfo_dao_forms_manager_has, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Forms_Manager, set, arginfo_dao_forms_manager_set, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

static zend_object_handlers dao_forms_manager_object_handlers;

static zval* dao_forms_manager_read_dimension(zend_object *obj, zval *offset, int type, zval *rv)
{
	zval object = {}, forms = {}, *res, tmp = {};

	if (UNEXPECTED(!offset)) {
		return &EG(uninitialized_zval);
	}
	ZVAL_OBJ(&object, obj);

	dao_read_property(&forms, &object, SL("_forms"), PH_NOISY|PH_READONLY);
	if (UNEXPECTED(Z_TYPE(forms)) != IS_ARRAY) {
		return &EG(uninitialized_zval);
	}

	if (type == BP_VAR_RW) {
		type = BP_VAR_R;
	}
	else if (type == BP_VAR_W) {
		type = BP_VAR_IS;
	}

	res = dao_hash_get(Z_ARRVAL(forms), offset, type);
	if (!res || res == &EG(uninitialized_zval)) {
		if (UNEXPECTED(Z_TYPE_P(offset) != IS_STRING)) {
			ZVAL_ZVAL(&tmp, offset, 1, 0);
			convert_to_string(&tmp);
			offset = &tmp;
		}

		zend_throw_exception_ex(dao_forms_exception_ce, 0, "There is no form with name='%s'", Z_STRVAL_P(offset));

		if (UNEXPECTED(offset == &tmp)) {
			zval_dtor(&tmp);
		}

		return NULL;
	}

	return res;
}

/**
 * Dao\Forms\Manager initializer
 */
DAO_INIT_CLASS(Dao_Forms_Manager){

	DAO_REGISTER_CLASS(Dao\\Forms, Manager, forms_manager, dao_forms_manager_method_entry, 0);

	zend_declare_property_null(dao_forms_manager_ce, SL("_forms"), ZEND_ACC_PROTECTED);

	dao_forms_manager_object_handlers                = *zend_get_std_object_handlers();
	dao_forms_manager_object_handlers.read_dimension = dao_forms_manager_read_dimension;

	return SUCCESS;
}

PHP_METHOD(Dao_Forms_Manager, __construct)
{
	dao_update_property_empty_array(getThis(), SL("_forms"));

	Z_OBJ_HT_P(getThis()) = &dao_forms_manager_object_handlers;
}

/**
 * Creates a form registering it in the forms manager
 *
 * @param string $name
 * @param object $entity
 * @return Dao\Forms\Form
 */
PHP_METHOD(Dao_Forms_Manager, create){

	zval *name = NULL, *entity = NULL, form = {};

	dao_fetch_params(0, 0, 2, &name, &entity);

	if (!name) {
		name = &DAO_GLOBAL(z_null);
	}

	if (!entity) {
		entity = &DAO_GLOBAL(z_null);
	}

	if (Z_TYPE_P(name) != IS_STRING) {
		DAO_THROW_EXCEPTION_STR(dao_forms_exception_ce, "The form name must be string");
		return;
	}

	object_init_ex(&form, dao_forms_form_ce);
	DAO_CALL_METHOD(NULL, &form, "__construct", entity);

	dao_update_property_array(getThis(), SL("_forms"), name, &form);

	RETURN_ZVAL(&form, 0, 0);
}

/**
 * Returns a form by its name
 *
 * @param string $name
 * @return Dao\Forms\Form
 */
PHP_METHOD(Dao_Forms_Manager, get){

	zval *name, forms = {}, form = {};

	dao_fetch_params(0, 1, 0, &name);

	dao_read_property(&forms, getThis(), SL("_forms"), PH_NOISY|PH_READONLY);
	if (!dao_array_isset_fetch(&form, &forms, name, PH_READONLY)) {
		zend_throw_exception_ex(dao_forms_exception_ce, 0, "There is no form with name='%s'", Z_STRVAL_P(name));
		return;
	}

	RETURN_CTOR(&form);
}

/**
 * Checks if a form is registered in the forms manager
 *
 * @param string $name
 * @return boolean
 */
PHP_METHOD(Dao_Forms_Manager, has){

	zval *name, forms = {};

	dao_fetch_params(0, 1, 0, &name);

	dao_read_property(&forms, getThis(), SL("_forms"), PH_NOISY|PH_READONLY);
	RETURN_BOOL(dao_array_isset(&forms, name));
}

/**
 * Registers a form in the Forms Manager
 *
 * @param string $name
 * @param Dao\Forms\Form $form
 * @return Dao\Forms\Manager
 */
PHP_METHOD(Dao_Forms_Manager, set){

	zval *name, *form;

	dao_fetch_params(0, 2, 0, &name, &form);
	DAO_VERIFY_CLASS_EX(form, dao_forms_form_ce, dao_forms_exception_ce);

	dao_update_property_array(getThis(), SL("_forms"), name, form);
	RETURN_THIS();
}
