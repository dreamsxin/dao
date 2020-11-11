
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

#include "annotations/adapter.h"
#include "annotations/adapterinterface.h"
#include "annotations/collection.h"
#include "annotations/reader.h"
#include "annotations/reflection.h"
#include "annotations/exception.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/exception.h"
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/array.h"
#include "kernel/hash.h"
#include "kernel/operators.h"

/**
 * Dao\Annotations\Adapter
 *
 * This is the base class for Dao\Annotations adapters
 */
zend_class_entry *dao_annotations_adapter_ce;

PHP_METHOD(Dao_Annotations_Adapter, setReader);
PHP_METHOD(Dao_Annotations_Adapter, getReader);
PHP_METHOD(Dao_Annotations_Adapter, get);
PHP_METHOD(Dao_Annotations_Adapter, getMethods);
PHP_METHOD(Dao_Annotations_Adapter, getMethod);
PHP_METHOD(Dao_Annotations_Adapter, getProperties);
PHP_METHOD(Dao_Annotations_Adapter, getProperty);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_annotations_adapter_setreader, 0, 0, 1)
	ZEND_ARG_INFO(0, reader)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_annotations_adapter_get, 0, 0, 1)
	ZEND_ARG_INFO(0, className)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_annotations_adapter_getmethods, 0, 0, 1)
	ZEND_ARG_INFO(0, className)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_annotations_adapter_getmethod, 0, 0, 2)
	ZEND_ARG_INFO(0, className)
	ZEND_ARG_INFO(0, methodName)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_annotations_adapter_getproperties, 0, 0, 1)
	ZEND_ARG_INFO(0, className)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_annotations_adapter_getproperty, 0, 0, 2)
	ZEND_ARG_INFO(0, className)
	ZEND_ARG_INFO(0, propertyName)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_annotations_adapter_method_entry[] = {
	PHP_ME(Dao_Annotations_Adapter, setReader, arginfo_dao_annotations_adapter_setreader, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Annotations_Adapter, getReader, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Annotations_Adapter, get, arginfo_dao_annotations_adapter_get, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Annotations_Adapter, getMethods, arginfo_dao_annotations_adapter_getmethods, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Annotations_Adapter, getMethod, arginfo_dao_annotations_adapter_getmethod, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Annotations_Adapter, getProperties, arginfo_dao_annotations_adapter_getproperties, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Annotations_Adapter, getProperty, arginfo_dao_annotations_adapter_getproperty, ZEND_ACC_PUBLIC)
	PHP_FE_END
};


/**
 * Dao\Annotations\Adapter initializer
 */
DAO_INIT_CLASS(Dao_Annotations_Adapter){

	DAO_REGISTER_CLASS(Dao\\Annotations, Adapter, annotations_adapter, dao_annotations_adapter_method_entry, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	zend_declare_property_null(dao_annotations_adapter_ce, SL("_reader"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_annotations_adapter_ce, SL("_annotations"), ZEND_ACC_PROTECTED);

	zend_class_implements(dao_annotations_adapter_ce, 1, dao_annotations_adapterinterface_ce);

	return SUCCESS;
}

/**
 * Sets the annotations parser
 *
 * @param Dao\Annotations\ReaderInterface $reader
 */
PHP_METHOD(Dao_Annotations_Adapter, setReader){

	zval *reader;

	dao_fetch_params(0, 1, 0, &reader);

	if (Z_TYPE_P(reader) != IS_OBJECT) {
		DAO_THROW_EXCEPTION_STR(dao_annotations_exception_ce, "Invalid annotations reader");
		return;
	}
	dao_update_property(getThis(), SL("_reader"), reader);

}

/**
 * Returns the annotation reader
 *
 * @return Dao\Annotations\ReaderInterface
 */
PHP_METHOD(Dao_Annotations_Adapter, getReader){

	zval reader = {};

	dao_read_property(&reader, getThis(), SL("_reader"), PH_READONLY);
	if (Z_TYPE(reader) == IS_OBJECT) {
		RETURN_CTOR(&reader);
	}

	object_init_ex(return_value, dao_annotations_reader_ce);
	dao_update_property(getThis(), SL("_reader"), return_value);
}

/**
 * Parses or retrieves all the annotations found in a class
 *
 * @param string|object $className
 * @return Dao\Annotations\Reflection
 */
PHP_METHOD(Dao_Annotations_Adapter, get){

	zval *class_name, real_class_name = {}, annotations = {}, class_annotations = {}, reader = {}, parsed_annotations = {};
	zend_class_entry *ce;

	dao_fetch_params(0, 1, 0, &class_name);

	/**
	 * Get the class name if it's an object
	 */
	if (Z_TYPE_P(class_name) == IS_OBJECT) {
		ce = Z_OBJCE_P(class_name);
		ZVAL_STR(&real_class_name, zend_string_dup(ce->name, 0));
	} else {
		ZVAL_COPY(&real_class_name, class_name);
	}

	dao_read_property(&annotations, getThis(), SL("_annotations"), PH_READONLY);
	if (dao_array_isset_fetch(&class_annotations, &annotations, &real_class_name, PH_READONLY)) {
		zval_ptr_dtor(&real_class_name);
		RETURN_CTOR(&class_annotations);
	}

	/**
	 * Try to read the annotations from the adapter
	 */
	DAO_CALL_METHOD(&class_annotations, getThis(), "read", &real_class_name);
	if (Z_TYPE(class_annotations) == IS_NULL) {
		/**
		 * Get the annotations reader
		 */
		DAO_CALL_METHOD(&reader, getThis(), "getreader");
		DAO_CALL_METHOD(&parsed_annotations, &reader, "parse", &real_class_name);
		zval_ptr_dtor(&reader);

		/**
		 * If the reader returns a
		 */
		if (Z_TYPE(parsed_annotations) == IS_ARRAY) {
			object_init_ex(&class_annotations, dao_annotations_reflection_ce);
			DAO_CALL_METHOD(NULL, &class_annotations, "__construct", &parsed_annotations);

			dao_update_property_array(getThis(), SL("_annotations"), &real_class_name, &class_annotations);

			DAO_CALL_METHOD(NULL, getThis(), "write", &real_class_name, &class_annotations);
		}
		zval_ptr_dtor(&parsed_annotations);
	}
	zval_ptr_dtor(&real_class_name);

	RETURN_ZVAL(&class_annotations, 0, 0);
}

/**
 * Returns the annotations found in all the class' methods
 *
 * @param string $className
 * @return array
 */
PHP_METHOD(Dao_Annotations_Adapter, getMethods){

	zval *class_name, class_annotations = {};

	dao_fetch_params(0, 1, 0, &class_name);

	/**
	 * Get the full annotations from the class
	 */
	DAO_CALL_METHOD(&class_annotations, getThis(), "get", class_name);

	/**
	 * A valid annotations reflection is an object
	 */
	if (Z_TYPE(class_annotations) == IS_OBJECT) {
		DAO_RETURN_CALL_METHOD(&class_annotations, "getmethodsannotations");
	} else {
		RETURN_EMPTY_ARRAY();
	}
	zval_ptr_dtor(&class_annotations);
}

/**
 * Returns the annotations found in a specific method
 *
 * @param string $className
 * @param string $methodName
 * @return Dao\Annotations\Collection
 */
PHP_METHOD(Dao_Annotations_Adapter, getMethod){

	zval *class_name, *method_name, class_annotations = {}, methods = {}, *method;
	zend_string *str_key;
	ulong idx;

	dao_fetch_params(0, 2, 0, &class_name, &method_name);

	/**
	 * Get the full annotations from the class
	 */
	DAO_CALL_METHOD(&class_annotations, getThis(), "get", class_name);

	/**
	 * A valid annotations reflection is an object
	 */
	if (Z_TYPE(class_annotations) == IS_OBJECT) {
		DAO_CALL_METHOD(&methods, &class_annotations, "getmethodsannotations");
		if (Z_TYPE(methods) == IS_ARRAY) {
			ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL(methods), idx, str_key, method) {
				zval name = {};
				if (str_key) {
					ZVAL_STR(&name, str_key);
				} else {
					ZVAL_LONG(&name, idx);
				}

				if (DAO_IS_EQUAL(&name, method_name)) {
					zval_ptr_dtor(&class_annotations);
					zval_ptr_dtor(&methods);
					RETURN_CTOR(method);
				}
			} ZEND_HASH_FOREACH_END();
		}
		zval_ptr_dtor(&methods);
	}
	zval_ptr_dtor(&class_annotations);

	/**
	 * Returns a collection anyways
	 */
	object_init_ex(return_value, dao_annotations_collection_ce);
	DAO_CALL_METHOD(NULL, return_value, "__construct");
}

/**
 * Returns the annotations found in all the class' methods
 *
 * @param string $className
 * @return array
 */
PHP_METHOD(Dao_Annotations_Adapter, getProperties){

	zval *class_name, class_annotations = {};

	dao_fetch_params(0, 1, 0, &class_name);

	/**
	 * Get the full annotations from the class
	 */
	DAO_CALL_METHOD(&class_annotations, getThis(), "get", class_name);

	/**
	 * A valid annotations reflection is an object
	 */
	if (Z_TYPE(class_annotations) == IS_OBJECT) {
		DAO_RETURN_CALL_METHOD(&class_annotations, "getpropertiesannotations");
	} else {
		RETURN_EMPTY_ARRAY();
	}
	zval_ptr_dtor(&class_annotations);
}

/**
 * Returns the annotations found in a specific property
 *
 * @param string $className
 * @param string $propertyName
 * @return Dao\Annotations\Collection
 */
PHP_METHOD(Dao_Annotations_Adapter, getProperty){

	zval *class_name, *property_name, class_annotations = {}, properties = {}, *property;
	zend_string *str_key;
	ulong idx;

	dao_fetch_params(1, 2, 0, &class_name, &property_name);

	/**
	 * Get the full annotations from the class
	 */
	DAO_MM_CALL_METHOD(&class_annotations, getThis(), "get", class_name);
	DAO_MM_ADD_ENTRY(&class_annotations);

	/**
	 * A valid annotations reflection is an object
	 */
	if (Z_TYPE(class_annotations) == IS_OBJECT) {
		DAO_MM_CALL_METHOD(&properties, &class_annotations, "getpropertiesannotations");
		DAO_MM_ADD_ENTRY(&properties);

		if (Z_TYPE(properties) == IS_ARRAY) {
			ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL(properties), idx, str_key, property) {
				zval name = {};
				if (str_key) {
					ZVAL_STR(&name, str_key);
				} else {
					ZVAL_LONG(&name, idx);
				}

				if (DAO_IS_EQUAL(&name, property_name)) {
					RETURN_MM_CTOR(property);
				}
			} ZEND_HASH_FOREACH_END();
		}
	}

	/**
	 * Returns a collection anyways
	 */
	object_init_ex(return_value, dao_annotations_collection_ce);
	DAO_MM_CALL_METHOD(NULL, return_value, "__construct");
	RETURN_MM();
}
