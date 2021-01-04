
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

#include "validation.h"
#include "validationinterface.h"
#include "validation/exception.h"
#include "validation/message/group.h"
#include "validation/validator.h"
#include "validation/validatorinterface.h"
#include "translate/adapterinterface.h"
#include "di.h"
#include "di/injectable.h"
#include "filterinterface.h"
#include "kernel.h"
#include "arr.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/exception.h"
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/array.h"
#include "kernel/concat.h"
#include "kernel/operators.h"
#include "kernel/string.h"
#include "kernel/require.h"
#include "kernel/debug.h"

#include "interned-strings.h"

/**
 * Dao\Validation
 *
 * Allows to validate data using validators
 */
zend_class_entry *dao_validation_ce;

PHP_METHOD(Dao_Validation, __construct);
PHP_METHOD(Dao_Validation, validate);
PHP_METHOD(Dao_Validation, add);
PHP_METHOD(Dao_Validation, setFilters);
PHP_METHOD(Dao_Validation, getFilters);
PHP_METHOD(Dao_Validation, getValidators);
PHP_METHOD(Dao_Validation, setEntity);
PHP_METHOD(Dao_Validation, getEntity);
PHP_METHOD(Dao_Validation, getMessages);
PHP_METHOD(Dao_Validation, appendMessage);
PHP_METHOD(Dao_Validation, bind);
PHP_METHOD(Dao_Validation, getData);
PHP_METHOD(Dao_Validation, getValue);
PHP_METHOD(Dao_Validation, setDefaultMessages);
PHP_METHOD(Dao_Validation, getDefaultMessage);
PHP_METHOD(Dao_Validation, setLabels);
PHP_METHOD(Dao_Validation, getLabel);
PHP_METHOD(Dao_Validation, setLabelDelimiter);
PHP_METHOD(Dao_Validation, setMessageFilename);
PHP_METHOD(Dao_Validation, getMessage);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_validation___construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, validators, IS_ARRAY, 1)
	ZEND_ARG_TYPE_INFO(0, options, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_validation_setfilters, 0, 0, 2)
	ZEND_ARG_INFO(0, attribute)
	ZEND_ARG_INFO(0, filters)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_validation_getfilters, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, attribute, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_validation_setentity, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, entity, IS_OBJECT, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_validation_bind, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, entity, IS_OBJECT, 0)
	ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_validation_setdefaultmessages, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, messages, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_validation_getdefaultmessage, 0, 0, 1)
	ZEND_ARG_INFO(0, type)
	ZEND_ARG_TYPE_INFO(0, defaultValue, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_validation_setlabels, 0, 0, 1)
	ZEND_ARG_INFO(0, labels)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_validation_setlabeldelimiter, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, delimiter, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_validation_setmessagefilename, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, filename, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_validation_getmessage, 0, 0, 1)
	ZEND_ARG_INFO(0, type)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_validation_method_entry[] = {
	PHP_ME(Dao_Validation, __construct, arginfo_dao_validation___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Validation, validate, arginfo_dao_validationinterface_validate, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Validation, add, arginfo_dao_validationinterface_add, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Validation, setFilters, arginfo_dao_validation_setfilters, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Validation, getFilters, arginfo_dao_validation_getfilters, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Validation, getValidators, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Validation, setEntity, arginfo_dao_validation_setentity, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Validation, getEntity, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Validation, getMessages, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Validation, appendMessage, arginfo_dao_validationinterface_appendmessage, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Validation, bind, arginfo_dao_validation_bind, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Validation, getData, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Validation, getValue, arginfo_dao_validationinterface_getvalue, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Validation, setDefaultMessages, arginfo_dao_validation_setdefaultmessages, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Validation, getDefaultMessage, arginfo_dao_validation_getdefaultmessage, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Validation, setLabels, arginfo_dao_validation_setlabels, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Validation, getLabel, arginfo_dao_validationinterface_getlabel, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Validation, setLabelDelimiter, arginfo_dao_validation_setlabeldelimiter, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Validation, setMessageFilename, arginfo_dao_validation_setmessagefilename, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Validation, getMessage, arginfo_dao_validation_getmessage, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};

/**
 * Dao\Validation initializer
 */
DAO_INIT_CLASS(Dao_Validation){

	DAO_REGISTER_CLASS_EX(Dao, Validation, validation, dao_di_injectable_ce, dao_validation_method_entry, 0);

	zend_declare_property_null(dao_validation_ce, SL("_data"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_validation_ce, SL("_entity"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_validation_ce, SL("_validators"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_validation_ce, SL("_filters"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_validation_ce, SL("_messages"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_validation_ce, SL("_values"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_validation_ce, SL("_labels"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_validation_ce, SL("_messageFilename"), ZEND_ACC_PROTECTED);
	zend_declare_property_bool(dao_validation_ce, SL("_allowEmpty"), 0, ZEND_ACC_PROTECTED);
	zend_declare_property_string(dao_validation_ce, SL("_delimiter"), ", ", ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);
	zend_declare_property_string(dao_validation_ce, SL("_defaultMessageFilename"), "validation", ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);

	zend_class_implements(dao_validation_ce, 1, dao_validationinterface_ce);

	return SUCCESS;
}

int dao_validation_getdefaultmessage_helper(zval *retval, const zend_class_entry *ce, zval *this_ptr, const char *type)
{
	zval t = {}, *params[1];
	int ret;
	ZVAL_STRING(&t, type);
	params[0] = &t;

	ret = dao_call_method(retval, this_ptr, "getdefaultmessage", 1, params);
	zval_ptr_dtor(&t);
	return ret;
}

/**
 * Dao\Validation constructor
 *
 * @param array $validators
 * @param array $options
 */
PHP_METHOD(Dao_Validation, __construct){

	zval *validators = NULL, *options = NULL;

	dao_fetch_params(0, 0, 2, &validators, &options);

	if (validators && Z_TYPE_P(validators) == IS_ARRAY) {
		zval *scope;
		zend_string *str_key;

		ZEND_HASH_FOREACH_STR_KEY_VAL(Z_ARRVAL_P(validators), str_key, scope) {
			zval attribute = {}, validator = {};

			if (str_key) {
				ZVAL_STR(&attribute, str_key);
				DAO_CALL_METHOD(NULL, getThis(), "add", &attribute, scope);
			} else {
				if (Z_TYPE_P(scope) != IS_ARRAY) {
					DAO_THROW_EXCEPTION_STR(dao_validation_exception_ce, "Validators is invalid");
					return;
				}

				dao_array_fetch_long(&attribute, scope, 0, PH_NOISY|PH_READONLY);
				dao_array_fetch_long(&validator, scope, 1, PH_NOISY|PH_READONLY);
				DAO_CALL_METHOD(NULL, getThis(), "add", &attribute, &validator);
			}
		} ZEND_HASH_FOREACH_END();
	}

	/* Check for an 'initialize' method */
	if (dao_method_exists_ex(getThis(), SL("initialize")) == SUCCESS) {
		DAO_CALL_METHOD(NULL, getThis(), "initialize");
	}

	if (options && Z_TYPE_P(options) == IS_ARRAY) {
		zval filename = {}, allow_empty = {};
		if (dao_array_isset_fetch_str(&filename, options, SL("messageFilename"), PH_READONLY)) {
			dao_update_property(getThis(), SL("_messageFilename"), &filename);
		}
		if (dao_array_isset_fetch_str(&allow_empty, options, SL("allowEmpty"), PH_READONLY)) {
			dao_update_property(getThis(), SL("_allowEmpty"), &allow_empty);
		}
	}
}

/**
 * Validate a set of data according to a set of rules
 *
 * @param array|object $data
 * @param object $entity
 * @return Dao\Validation\Message\Group
 */
PHP_METHOD(Dao_Validation, validate){

	zval *data = NULL, *entity = NULL, validators = {}, allow_empty = {}, messages = {}, status = {}, *scope;

	dao_fetch_params(1, 0, 2, &data, &entity);

	if (!data) {
		data = &DAO_GLOBAL(z_null);
	}

	if (!entity) {
		entity = &DAO_GLOBAL(z_null);
	} else {
		DAO_MM_CALL_METHOD(NULL, getThis(), "setentity", entity);

	}

	dao_read_property(&validators, getThis(), SL("_validators"), PH_READONLY);
	if (Z_TYPE(validators) != IS_ARRAY) {
		DAO_MM_THROW_EXCEPTION_STR(dao_validation_exception_ce, "There are no validators to validate");
		return;
	}

	dao_read_property(&allow_empty, getThis(), SL("_allowEmpty"), PH_READONLY);

	/**
	 * Clear pre-calculated values
	 */
	dao_update_property_null(getThis(), SL("_values"));

	/**
	 * Implicitly creates a Dao\Validation\Message\Group object
	 */
	object_init_ex(&messages, dao_validation_message_group_ce);
	DAO_MM_ADD_ENTRY(&messages);
	DAO_MM_CALL_METHOD(NULL, &messages, "__construct");

	dao_update_property(getThis(), SL("_messages"), &messages);

	/**
	 * Validation classes can implement the 'beforeValidation' callback
	 */
	if (dao_method_exists_ex(getThis(), SL("beforevalidation")) == SUCCESS) {
		DAO_MM_CALL_METHOD(&status, getThis(), "beforevalidation", data, entity, &messages);
		if (DAO_IS_FALSE(&status)) {
			RETURN_MM_NCTOR(&status);
		}
	}

	if (Z_TYPE_P(data) == IS_ARRAY || Z_TYPE_P(data) == IS_OBJECT) {
		dao_update_property(getThis(), SL("_data"), data);
	}

	ZEND_HASH_FOREACH_VAL(Z_ARRVAL(validators), scope) {
		zval attribute = {}, validator = {}, attribute_validators = {}, *attribute_validator, must_cancel = {};
		if (Z_TYPE_P(scope) != IS_ARRAY) {
			DAO_MM_THROW_EXCEPTION_STR(dao_validation_exception_ce, "The validator scope is not valid");
			return;
		}

		dao_array_fetch_long(&attribute, scope, 0, PH_NOISY|PH_READONLY);
		dao_array_fetch_long(&validator, scope, 1, PH_NOISY|PH_READONLY);

		if (Z_TYPE(validator) != IS_ARRAY) {
			array_init_size(&attribute_validators, 1);
			dao_array_append(&attribute_validators, &validator, PH_COPY);
		} else {
			ZVAL_COPY(&attribute_validators, &validator);
		}
		DAO_MM_ADD_ENTRY(&attribute_validators);

		ZEND_HASH_FOREACH_VAL(Z_ARRVAL(attribute_validators), attribute_validator) {
			if (Z_TYPE_P(attribute_validator) != IS_OBJECT) {
				DAO_MM_THROW_EXCEPTION_STR(dao_validation_exception_ce, "One of the validators is not valid");
				return;
			}

			DAO_MM_CALL_METHOD(&status, attribute_validator, "validate", getThis(), &attribute, &allow_empty);

			/**
			 * Check if the validation must be canceled if this validator fails
			 */
			if (DAO_IS_FALSE(&status)) {
				RETURN_MM_ON_FAILURE(dao_validation_validator_getoption_helper(&must_cancel, Z_OBJCE_P(attribute_validator), attribute_validator, "cancelOnFail"));

				if (zend_is_true(&must_cancel)) {
					break;
				}
			}
		} ZEND_HASH_FOREACH_END();

		if (zend_is_true(&must_cancel)) {
			break;
		}
	} ZEND_HASH_FOREACH_END();

	/**
	 * Get the messages generated by the validators
	 */
	dao_read_property(&messages, getThis(),  SL("_messages"), PH_READONLY);
	if (dao_method_exists_ex(getThis(), SL("aftervalidation")) == SUCCESS) {
		DAO_MM_CALL_METHOD(NULL, getThis(), "aftervalidation", data, entity, &messages);
	}

	RETURN_MM_CTOR(&messages);
}

/**
 * Adds a validator to a field
 *
 * @param string|array $attribute
 * @param array|Dao\Validation\ValidatorInterface
 * @return Dao\Validation
 */
PHP_METHOD(Dao_Validation, add){

	zval *attribute, *_validator, validator = {}, validators = {}, scope = {};

	dao_fetch_params(1, 2, 0, &attribute, &_validator);

	if (Z_TYPE_P(attribute) != IS_STRING && Z_TYPE_P(attribute) != IS_ARRAY) {
		DAO_MM_THROW_EXCEPTION_STR(dao_validation_exception_ce, "Field must be passed as array of fields or string");
		return;
	}

	if (Z_TYPE_P(_validator) == IS_STRING) {
		array_init_size(&validator, 1);
		dao_array_append(&validator, _validator, PH_COPY);
	} else {
		ZVAL_COPY(&validator, _validator);
	}
	DAO_MM_ADD_ENTRY(&validator);

	array_init(&validators);
	DAO_MM_ADD_ENTRY(&validators);

	if (Z_TYPE(validator) == IS_ARRAY) {
		zval *item;
		zend_string *item_key;

		ZEND_HASH_FOREACH_STR_KEY_VAL(Z_ARRVAL(validator), item_key, item) {
			zval name = {}, has = {}, class_name = {}, options = {}, object = {};

			if (!item_key) {
				if (Z_TYPE_P(item)!= IS_STRING) {
					DAO_MM_VERIFY_INTERFACE_EX(item, dao_validation_validatorinterface_ce, dao_validation_exception_ce);
					dao_array_append(&validators, item, PH_COPY);
					continue;
				}
				ZVAL_COPY_VALUE(&name, item);
				array_init(&options);
			} else {
				ZVAL_STR(&name, item_key);
				array_init_size(&options, 1);
				dao_array_append(&options, item, PH_COPY);
			}
			DAO_MM_ADD_ENTRY(&options);

			DAO_MM_CALL_METHOD(&has, getThis(), "hasservice", &name);
			if (!zend_is_true(&has)) {
				if (!dao_memnstr_str(&name, SL("\\"))) {
					DAO_CONCAT_SV(&class_name, "Dao\\Validation\\Validator\\", &name);
					DAO_MM_ADD_ENTRY(&class_name);
				} else {
					DAO_MM_ZVAL_COPY(&class_name, &name);
				}
			} else {
				DAO_MM_ZVAL_COPY(&class_name, &name);
			}

			DAO_MM_CALL_METHOD(&object, getThis(), "getresolveservice", &class_name, &options, &DAO_GLOBAL(z_false), &DAO_GLOBAL(z_true));
			DAO_MM_ADD_ENTRY(&object);
			if (Z_TYPE(object) != IS_OBJECT) {
				DAO_MM_THROW_EXCEPTION_FORMAT(dao_validation_exception_ce, "Validator %s is invalid", Z_STRVAL(name));
				return;
			}

			DAO_MM_VERIFY_INTERFACE_EX(&object, dao_validation_validatorinterface_ce, dao_validation_exception_ce);
			dao_array_append(&validators, &object, PH_COPY);
		} ZEND_HASH_FOREACH_END();
	} else {
		DAO_MM_VERIFY_INTERFACE_EX(&validator, dao_validation_validatorinterface_ce, dao_validation_exception_ce);
		dao_array_append(&validators, &validator, PH_COPY);
	}

	array_init_size(&scope, 2);
	dao_array_append(&scope, attribute, PH_COPY);
	dao_array_append(&scope, &validators, PH_COPY);
	dao_update_property_array_append(getThis(), SL("_validators"), &scope);
	zval_ptr_dtor(&scope);

	RETURN_MM_THIS();
}

/**
 * Adds filters to the field
 *
 * @param string $attribute
 * @param array|string $attribute
 * @return Dao\Validation
 */
PHP_METHOD(Dao_Validation, setFilters){

	zval *attribute, *filters;

	dao_fetch_params(0, 2, 0, &attribute, &filters);

	dao_update_property_array(getThis(), SL("_filters"), attribute, filters);
	RETURN_THIS();
}

/**
 * Returns all the filters or a specific one
 *
 * @param string $attribute
 * @return mixed
 */
PHP_METHOD(Dao_Validation, getFilters){

	zval *attribute = NULL, filters = {}, attribute_filters = {};

	dao_fetch_params(0, 0, 1, &attribute);

	dao_read_property(&filters, getThis(), SL("_filters"), PH_READONLY);
	if (attribute && Z_TYPE_P(attribute) == IS_STRING) {
		if (dao_array_isset_fetch(&attribute_filters, &filters, attribute, PH_READONLY)) {
			RETURN_CTOR(&attribute_filters);
		}

		RETURN_NULL();
	}

	RETURN_CTOR(&filters);
}

/**
 * Returns the validators added to the validation
 *
 * @return array
 */
PHP_METHOD(Dao_Validation, getValidators){


	RETURN_MEMBER(getThis(), "_validators");
}

/**
 * Sets the bound entity
 *
 * @param object entity
 * @return Dao\Validation
 */
PHP_METHOD(Dao_Validation, setEntity){

	zval *entity;

	dao_fetch_params(0, 1, 0, &entity);

	dao_update_property(getThis(), SL("_entity"), entity);
	RETURN_THIS();
}

/**
 * Returns the bound entity
 *
 * @return object
 */
PHP_METHOD(Dao_Validation, getEntity){


	RETURN_MEMBER(getThis(), "_entity");
}

/**
 * Returns the registered validators
 *
 * @return Dao\Validation\Message\Group
 */
PHP_METHOD(Dao_Validation, getMessages){


	RETURN_MEMBER(getThis(), "_messages");
}

/**
 * Appends a message to the messages list
 *
 * @param Dao\Validation\MessageInterface $message
 * @return Dao\Validation
 */
PHP_METHOD(Dao_Validation, appendMessage){

	zval *message, messages = {};

	dao_fetch_params(0, 1, 0, &message);

	dao_read_property(&messages, getThis(), SL("_messages"), PH_NOISY|PH_COPY);
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
 * Assigns the data to an entity
 * The entity is used to obtain the validation values
 *
 * @param object $entity
 * @param object|array $data
 * @return Dao\Validation
 */
PHP_METHOD(Dao_Validation, bind){

	zval *entity, *data;

	dao_fetch_params(0, 2, 0, &entity, &data);

	if (Z_TYPE_P(data) != IS_ARRAY && Z_TYPE_P(data) != IS_OBJECT) {
		DAO_THROW_EXCEPTION_STR(dao_validation_exception_ce, "The data to validate must be an array or object");
	}

	dao_update_property(getThis(), SL("_entity"), entity);
	dao_update_property(getThis(), SL("_data"), data);

	RETURN_THIS();
}

/**
 * Gets the a array data source
 *
 * @return array|null
 */
PHP_METHOD(Dao_Validation, getData){


	RETURN_MEMBER(getThis(), "_data");
}

/**
 * Gets the a value to validate in the array/object data source
 *
 * @param string $attribute
 * @param object entity
 * @return mixed
 */
PHP_METHOD(Dao_Validation, getValue){

	zval *attribute, *_entity = NULL, entity ={}, value = {}, data = {}, values = {}, filters = {}, field_filters = {};

	dao_fetch_params(0, 1, 1, &attribute, &_entity);

	/**
	 * Check if there is a calculated value
	 */
	dao_read_property(&values, getThis(), SL("_values"), PH_READONLY);
	if (dao_array_isset_fetch(&value, &values, attribute, PH_READONLY)) {
		RETURN_CTOR(&value);
	}

	if (!_entity || Z_TYPE_P(_entity) != IS_OBJECT) {
		dao_read_property(&entity, getThis(), SL("_entity"), PH_READONLY);
	} else {
		ZVAL_COPY_VALUE(&entity, _entity);
	}

	dao_read_property(&data, getThis(), SL("_data"), PH_READONLY);
	if (Z_TYPE(data) != IS_ARRAY && Z_TYPE(data) != IS_OBJECT) {
		/**
		 * If the entity is an object use it to retrieve the values
		 */
		if (Z_TYPE(entity) == IS_OBJECT) {
			if (dao_method_exists_ex(&entity, SL("readattribute")) == SUCCESS) {
				DAO_CALL_METHOD(&value, &entity, "readattribute", attribute);
			} else {
				dao_read_property_zval(&value, &entity, attribute, PH_COPY);
			}
			RETURN_ZVAL(&value, 0, 0);
		} else {
			DAO_THROW_EXCEPTION_STR(dao_validation_exception_ce, "There are no data to validate");
		}
		return;
	}

	if (Z_TYPE(data) == IS_ARRAY) {
		 if (!dao_array_isset_fetch(&value, &data, attribute, PH_COPY)) {
			ZVAL_NULL(&value);
		 }
	} else if (Z_TYPE(data) == IS_OBJECT) {
		if (dao_isset_property_zval(&data, attribute)) {
			dao_read_property_zval(&value, &data, attribute, PH_COPY);
		} else {
			ZVAL_NULL(&value);
		}
	}

	if (Z_TYPE(value) != IS_NULL) {
		dao_read_property(&filters, getThis(), SL("_filters"), PH_READONLY);
		if (Z_TYPE(filters) == IS_ARRAY) {
			if (dao_array_isset_fetch(&field_filters, &filters, attribute, PH_READONLY)) {
				if (zend_is_true(&field_filters)) {
					zval filter_value = {}, service_name = {}, dependency_injector = {}, filter_service = {};
					ZVAL_STR(&service_name, IS(filter));

					DAO_CALL_METHOD(&dependency_injector, getThis(), "getdi");
					if (Z_TYPE(dependency_injector) != IS_OBJECT) {
						DAO_THROW_EXCEPTION_STR(dao_validation_exception_ce, "A dependency injector is required to obtain the 'filter' service");
						return;
					}

					DAO_CALL_METHOD(&filter_service, &dependency_injector, "getshared", &service_name);
					zval_ptr_dtor(&dependency_injector);
					if (Z_TYPE(filter_service) != IS_OBJECT) {
						DAO_THROW_EXCEPTION_STR(dao_validation_exception_ce, "Returned 'filter' service is invalid");
						return;
					}

					DAO_VERIFY_INTERFACE(&filter_service, dao_filterinterface_ce);
					DAO_CALL_METHOD(&filter_value, &filter_service, "sanitize", &value, &field_filters);
					zval_ptr_dtor(&filter_service);
					zval_ptr_dtor(&value);
					ZVAL_COPY_VALUE(&value, &filter_value);
				}
			}
		}

	   	if (Z_TYPE(entity) == IS_OBJECT) {
	   		if (dao_method_exists_ex(&entity, SL("writeattribute")) == SUCCESS) {
	   			DAO_CALL_METHOD(NULL, &entity, "writeattribute", attribute, &value);
	   		} else {
	   			dao_update_property_zval_zval(&entity, attribute, &value);
	   		}
	   	}

		/**
		 * Cache the calculated value
		 */
		dao_update_property_array(getThis(), SL("_values"), attribute, &value);
	}

	RETURN_ZVAL(&value, 0, 0);
}

PHP_METHOD(Dao_Validation, setDefaultMessages)
{
	zval *messages, file = {};

	dao_fetch_params(0, 1, 0, &messages);

	dao_read_static_property_ce(&file, dao_validation_ce, SL("_file"), PH_READONLY);

	dao_update_static_property_array_ce(dao_kernel_ce, SL("_defaultMessages"), &file, messages);
}

PHP_METHOD(Dao_Validation, getDefaultMessage)
{
	zval *type, *default_value = NULL, filename = {}, file = {};

	dao_fetch_params(0, 1, 1, &type, &default_value);

	if (!default_value) {
		default_value = &DAO_GLOBAL(z_null);
	}

	dao_read_property(&filename, getThis(), SL("_messageFilename"), PH_NOISY|PH_READONLY);
	if (DAO_IS_EMPTY(&filename)) {
		dao_read_static_property_ce(&file, dao_validation_ce, SL("_defaultMessageFilename"), PH_READONLY);
		DAO_CALL_CE_STATIC(return_value, dao_kernel_ce, "message", &file, type, default_value);
	} else {
		DAO_CALL_CE_STATIC(return_value, dao_kernel_ce, "message", &filename, type, default_value);
	}

}

/**
 * Adds labels for fields
 *
 * @param array labels
 */
PHP_METHOD(Dao_Validation, setLabels) {

	zval *labels;

	dao_fetch_params(0, 1, 0, &labels);

	if (Z_TYPE_P(labels) != IS_ARRAY) {
		zend_throw_exception_ex(dao_validation_exception_ce, 0, "Labels must be an array");
		return;
	}
	dao_update_property(getThis(), SL("_labels"), labels);
}

/**
 * Gets label for field
 *
 * @param string|array field
 * @return string
 */
PHP_METHOD(Dao_Validation, getLabel) {

	zval *field_param = NULL, labels = {}, entity = {};
	int exists = 0;

	dao_fetch_params(0, 1, 0, &field_param);

	if (Z_TYPE_P(field_param) == IS_NULL) {
		RETURN_NULL();
	}

	if (Z_TYPE_P(field_param) != IS_STRING && Z_TYPE_P(field_param) != IS_ARRAY) {
		zend_throw_exception_ex(dao_validation_exception_ce, 0, "Parameter 'field' must be a string or array");
		RETURN_NULL();
	}

	dao_read_property(&labels, getThis(), SL("_labels"), PH_READONLY);
	dao_read_property(&entity, getThis(), SL("_entity"), PH_READONLY);
	if (Z_TYPE(entity) == IS_OBJECT && dao_method_exists_ex(&entity, SL("getlabel")) == SUCCESS) {
		exists = 1;
	}

	if (Z_TYPE_P(field_param) == IS_ARRAY) {
		zval label_values = {}, *field, delimiter = {};
		array_init(&label_values);

		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(field_param), field) {
			zval label = {};
			if (Z_TYPE(labels) == IS_ARRAY && dao_array_isset_fetch(&label, &labels, field, PH_READONLY) && Z_TYPE(label) == IS_STRING) {
				dao_array_append(&label_values, &label, PH_COPY);
			} else if (exists) {
				DAO_CALL_METHOD(&label, &entity, "getlabel", field_param);
				if (Z_TYPE(label) == IS_STRING) {
					dao_array_append(&label_values, &label, 0);
				} else {
					dao_array_append(&label_values, field, PH_COPY);
				}
			} else {
				dao_array_append(&label_values, field, PH_COPY);
			}
		} ZEND_HASH_FOREACH_END();
		dao_read_static_property_ce(&delimiter, dao_validation_ce, SL("_delimiter"), PH_READONLY);
		dao_fast_join_str(return_value, Z_STRVAL(delimiter), Z_STRLEN(delimiter), &label_values);
		zval_ptr_dtor(&label_values);
	} else {
		if (Z_TYPE(labels) != IS_ARRAY || !dao_array_isset_fetch(return_value, &labels, field_param, PH_COPY) || Z_TYPE_P(return_value) == IS_NULL) {
			if (exists) {
				DAO_CALL_METHOD(return_value, &entity, "getlabel", field_param);
				if (Z_TYPE_P(return_value) == IS_NULL) {
					ZVAL_COPY(return_value, field_param);
				}
			} else {
				ZVAL_COPY(return_value, field_param);
			}
		}
	}
}

/**
 * Sets delimiter for label
 *
 * @param string
 */
PHP_METHOD(Dao_Validation, setLabelDelimiter)
{
	zval *delimiter;

	dao_fetch_params(0, 1, 0, &delimiter);

	dao_update_static_property_ce(dao_validation_ce, SL("_delimiter"), delimiter);
}

/**
 * Sets validation message file name
 *
 * @param string filename
 */
PHP_METHOD(Dao_Validation, setMessageFilename)
{
	zval *filename;

	dao_fetch_params(0, 1, 0, &filename);

	dao_update_static_property_ce(dao_validation_ce, SL("_defaultMessageFilename"), filename);
}

/**
 * Gets message
 *
 * @param string
 */
PHP_METHOD(Dao_Validation, getMessage)
{
	zval *type, filename = {};

	dao_fetch_params(0, 1, 0, &type);

	dao_read_static_property_ce(&filename, dao_validation_ce, SL("_defaultMessageFilename"), PH_READONLY);

	DAO_CALL_CE_STATIC(return_value, dao_kernel_ce, "message", &filename, type);
}
