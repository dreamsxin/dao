
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

#include "php_dao.h"

#include "annotations/reflection.h"
#include "annotations/collection.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/array.h"
#include "kernel/fcall.h"
#include "kernel/file.h"
#include "kernel/hash.h"
#include "kernel/operators.h"

/**
 * Dao\Annotations\Reflection
 *
 * Allows to manipulate the annotations reflection in an OO manner
 *
 *<code>
 * //Parse the annotations in a class
 * $reader = new \Dao\Annotations\Reader();
 * $parsing = $reader->parse('MyComponent');
 *
 * //Create the reflection
 * $reflection = new \Dao\Annotations\Reflection($parsing);
 *
 * //Get the annotations in the class docblock
 * $classAnnotations = $reflection->getClassAnnotations();
 *</code>
 */
zend_class_entry *dao_annotations_reflection_ce;

PHP_METHOD(Dao_Annotations_Reflection, __construct);
PHP_METHOD(Dao_Annotations_Reflection, getClassAnnotations);
PHP_METHOD(Dao_Annotations_Reflection, getMethodsAnnotations);
PHP_METHOD(Dao_Annotations_Reflection, getPropertiesAnnotations);
PHP_METHOD(Dao_Annotations_Reflection, getReflectionData);
PHP_METHOD(Dao_Annotations_Reflection, __set_state);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_annotations_reflection___construct, 0, 0, 0)
	ZEND_ARG_INFO(0, reflectionData)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_annotations_reflection___set_state, 0, 0, 1)
	ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_annotations_reflection_method_entry[] = {
	PHP_ME(Dao_Annotations_Reflection, __construct, arginfo_dao_annotations_reflection___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Annotations_Reflection, getClassAnnotations, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Annotations_Reflection, getMethodsAnnotations, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Annotations_Reflection, getPropertiesAnnotations, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Annotations_Reflection, getReflectionData, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Annotations_Reflection, __set_state, arginfo_dao_annotations_reflection___set_state, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};


/**
 * Dao\Annotations\Reflection initializer
 */
DAO_INIT_CLASS(Dao_Annotations_Reflection){

	DAO_REGISTER_CLASS(Dao\\Annotations, Reflection, annotations_reflection, dao_annotations_reflection_method_entry, 0);

	zend_declare_property_null(dao_annotations_reflection_ce, SL("_reflectionData"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_annotations_reflection_ce, SL("_classAnnotations"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_annotations_reflection_ce, SL("_methodAnnotations"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_annotations_reflection_ce, SL("_propertyAnnotations"), ZEND_ACC_PROTECTED);

	return SUCCESS;
}

/**
 * Dao\Annotations\Reflection constructor
 *
 * @param array $reflectionData
 */
PHP_METHOD(Dao_Annotations_Reflection, __construct){

	zval *reflection_data = NULL;

	dao_fetch_params(0, 0, 1, &reflection_data);

	if (reflection_data && Z_TYPE_P(reflection_data) == IS_ARRAY) {
		dao_update_property(getThis(), SL("_reflectionData"), reflection_data);
	}
}

/**
 * Returns the annotations found in the class docblock
 *
 * @return Dao\Annotations\Collection
 */
PHP_METHOD(Dao_Annotations_Reflection, getClassAnnotations){

	zval annotations = {}, reflection_data = {}, reflection_class = {};

	dao_read_property(&annotations, getThis(), SL("_classAnnotations"), PH_READONLY);
	if (Z_TYPE(annotations) != IS_OBJECT) {
		dao_read_property(&reflection_data, getThis(), SL("_reflectionData"), PH_READONLY);
		if (dao_array_isset_fetch_str(&reflection_class, &reflection_data, SL("class"), PH_READONLY)) {
			object_init_ex(return_value, dao_annotations_collection_ce);
			DAO_CALL_METHOD(NULL, return_value, "__construct", &reflection_class);

			dao_update_property(getThis(), SL("_classAnnotations"), return_value);
			return;
		}

		dao_update_property(getThis(), SL("_classAnnotations"), &DAO_GLOBAL(z_false));
		RETURN_FALSE;
	}

	RETURN_CTOR(&annotations);
}

/**
 * Returns the annotations found in the methods' docblocks
 *
 * @return Dao\Annotations\Collection[]
 */
PHP_METHOD(Dao_Annotations_Reflection, getMethodsAnnotations){

	zval annotations = {}, reflection_data = {}, reflection_methods = {}, *reflection_method;
	zend_string *str_key;
	ulong idx;

	dao_read_property(&annotations, getThis(), SL("_methodAnnotations"), PH_READONLY);
	if (Z_TYPE(annotations) != IS_OBJECT) {
		dao_read_property(&reflection_data, getThis(), SL("_reflectionData"), PH_READONLY);
		if (dao_array_isset_fetch_str(&reflection_methods, &reflection_data, SL("methods"), PH_READONLY)) {
			if (dao_fast_count_ev(&reflection_methods)) {
				array_init(return_value);

				ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL(reflection_methods), idx, str_key, reflection_method) {
					zval method_name = {}, collection = {};
					if (str_key) {
						ZVAL_STR(&method_name, str_key);
					} else {
						ZVAL_LONG(&method_name, idx);
					}

					object_init_ex(&collection, dao_annotations_collection_ce);
					DAO_CALL_METHOD(NULL, &collection, "__construct", reflection_method);

					dao_array_update(return_value, &method_name, &collection, 0);
				} ZEND_HASH_FOREACH_END();

				dao_update_property(getThis(), SL("_methodAnnotations"), return_value);
				return;
			}
		}

		dao_update_property_bool(getThis(), SL("_methodAnnotations"), 0);
		RETURN_FALSE;
	}

	RETURN_CTOR(&annotations);
}

/**
 * Returns the annotations found in the properties' docblocks
 *
 * @return Dao\Annotations\Collection[]
 */
PHP_METHOD(Dao_Annotations_Reflection, getPropertiesAnnotations){

	zval annotations = {}, reflection_data = {}, reflection_properties = {}, *reflection_property;
	zend_string *str_key;
	ulong idx;

	dao_read_property(&annotations, getThis(), SL("_propertyAnnotations"), PH_READONLY);
	if (Z_TYPE(annotations) != IS_OBJECT) {
		dao_read_property(&reflection_data, getThis(), SL("_reflectionData"), PH_READONLY);
		if (dao_array_isset_fetch_str(&reflection_properties, &reflection_data, SL("properties"), PH_READONLY)) {
			if (dao_fast_count_ev(&reflection_properties)) {
				array_init(return_value);

				ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL(reflection_properties), idx, str_key, reflection_property) {
					zval property = {}, collection = {};
					if (str_key) {
						ZVAL_STR(&property, str_key);
					} else {
						ZVAL_LONG(&property, idx);
					}

					object_init_ex(&collection, dao_annotations_collection_ce);
					DAO_CALL_METHOD(NULL, &collection, "__construct", reflection_property);

					dao_array_update(return_value, &property, &collection, 0);
				} ZEND_HASH_FOREACH_END();

				dao_update_property(getThis(), SL("_propertyAnnotations"), return_value);
				return;
			}
		}

		dao_update_property_bool(getThis(), SL("_propertyAnnotations"), 0);
		RETURN_FALSE;
	}

	RETURN_CTOR(&annotations);
}

/**
 * Returns the raw parsing intermediate definitions used to construct the reflection
 *
 * @return array
 */
PHP_METHOD(Dao_Annotations_Reflection, getReflectionData){


	RETURN_MEMBER(getThis(), "_reflectionData");
}

/**
 * Restores the state of a Dao\Annotations\Reflection variable export
 *
 * @return array $data
 */
PHP_METHOD(Dao_Annotations_Reflection, __set_state){

	zval *data, reflection_data = {};

	dao_fetch_params(0, 1, 0, &data);

	if (Z_TYPE_P(data) == IS_ARRAY) {
		/**
		 * Check for a '_reflectionData' in the array to build the Reflection
		 */
		if (dao_array_isset_fetch_str(&reflection_data, data, SL("_reflectionData"), PH_READONLY)) {
			object_init_ex(return_value, dao_annotations_reflection_ce);
			DAO_CALL_METHOD(NULL, return_value, "__construct", &reflection_data);
			return;
		}
	}
	object_init_ex(return_value, dao_annotations_reflection_ce);
	DAO_CALL_METHOD(NULL, return_value, "__construct");
}
