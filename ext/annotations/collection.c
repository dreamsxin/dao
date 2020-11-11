
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

#include "annotations/collection.h"
#include "annotations/annotation.h"
#include "annotations/exception.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/exception.h"
#include "kernel/fcall.h"
#include "kernel/array.h"
#include "kernel/object.h"
#include "kernel/concat.h"
#include "kernel/operators.h"

/**
 * Dao\Annotations\Collection
 *
 * Represents a collection of annotations. This class allows to traverse a group of annotations easily
 *
 *<code>
 * //Traverse annotations
 * foreach ($classAnnotations as $annotation) {
 *     echo 'Name=', $annotation->getName(), PHP_EOL;
 * }
 *
 * //Check if the annotations has a specific
 * var_dump($classAnnotations->has('Cacheable'));
 *
 * //Get an specific annotation in the collection
 * $annotation = $classAnnotations->get('Cacheable');
 *</code>
 */
zend_class_entry *dao_annotations_collection_ce;

PHP_METHOD(Dao_Annotations_Collection, __construct);
PHP_METHOD(Dao_Annotations_Collection, count);
PHP_METHOD(Dao_Annotations_Collection, rewind);
PHP_METHOD(Dao_Annotations_Collection, current);
PHP_METHOD(Dao_Annotations_Collection, key);
PHP_METHOD(Dao_Annotations_Collection, next);
PHP_METHOD(Dao_Annotations_Collection, valid);
PHP_METHOD(Dao_Annotations_Collection, getAnnotations);
PHP_METHOD(Dao_Annotations_Collection, get);
PHP_METHOD(Dao_Annotations_Collection, getAll);
PHP_METHOD(Dao_Annotations_Collection, has);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_annotations_collection___construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, reflectionData, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_annotations_collection_get, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_annotations_collection_getall, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_annotations_collection_has, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_annotations_collection_method_entry[] = {
	PHP_ME(Dao_Annotations_Collection, __construct, arginfo_dao_annotations_collection___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Annotations_Collection, count, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Annotations_Collection, rewind, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Annotations_Collection, current, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Annotations_Collection, key, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Annotations_Collection, next, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Annotations_Collection, valid, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Annotations_Collection, getAnnotations, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Annotations_Collection, get, arginfo_dao_annotations_collection_get, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Annotations_Collection, getAll, arginfo_dao_annotations_collection_getall, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Annotations_Collection, has, arginfo_dao_annotations_collection_has, ZEND_ACC_PUBLIC)
	PHP_FE_END
};


/**
 * Dao\Annotations\Collection initializer
 */
DAO_INIT_CLASS(Dao_Annotations_Collection){

	DAO_REGISTER_CLASS(Dao\\Annotations, Collection, annotations_collection, dao_annotations_collection_method_entry, 0);

	zend_declare_property_long(dao_annotations_collection_ce, SL("_position"), 0, ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_annotations_collection_ce, SL("_annotations"), ZEND_ACC_PROTECTED);

	zend_class_implements(dao_annotations_collection_ce, 2, zend_ce_iterator, spl_ce_Countable);

	return SUCCESS;
}

/**
 * Dao\Annotations\Collection constructor
 *
 * @param array $reflectionData
 */
PHP_METHOD(Dao_Annotations_Collection, __construct){

	zval *reflection_data = NULL, annotations = {}, *annotation_data;

	dao_fetch_params(0, 0, 1, &reflection_data);

	if (reflection_data && Z_TYPE_P(reflection_data) != IS_NULL) {
		array_init(&annotations);

		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(reflection_data), annotation_data) {
			zval annotation = {};
			object_init_ex(&annotation, dao_annotations_annotation_ce);
			DAO_CALL_METHOD(NULL, &annotation, "__construct", annotation_data);
			dao_array_append(&annotations, &annotation, 0);
		} ZEND_HASH_FOREACH_END();

		dao_update_property(getThis(), SL("_annotations"), &annotations);
		zval_ptr_dtor(&annotations);
	}
}

/**
 * Returns the number of annotations in the collection
 *
 * @return int
 */
PHP_METHOD(Dao_Annotations_Collection, count){

	zval annotations = {};

	dao_read_property(&annotations, getThis(), SL("_annotations"), PH_NOISY|PH_READONLY);
	if (Z_TYPE(annotations) == IS_ARRAY) {
		RETURN_LONG(zend_hash_num_elements(Z_ARRVAL(annotations)));
	}

	RETURN_LONG(0);
}

/**
 * Rewinds the internal iterator
 */
PHP_METHOD(Dao_Annotations_Collection, rewind){


	dao_update_property_long(getThis(), SL("_position"), 0);

}

/**
 * Returns the current annotation in the iterator
 *
 * @return Dao\Annotations\Annotation
 */
PHP_METHOD(Dao_Annotations_Collection, current){

	zval position = {}, annotations = {};

	dao_read_property(&position, getThis(), SL("_position"), PH_NOISY|PH_READONLY);
	dao_read_property(&annotations, getThis(), SL("_annotations"), PH_NOISY|PH_READONLY);
	if (!dao_array_isset_fetch(return_value, &annotations, &position, PH_COPY)) {
		RETURN_NULL();
	}
}

/**
 * Returns the current position/key in the iterator
 *
 * @return int
 */
PHP_METHOD(Dao_Annotations_Collection, key){


	RETURN_MEMBER(getThis(), "_position");
}

/**
 * Moves the internal iteration pointer to the next position
 *
 */
PHP_METHOD(Dao_Annotations_Collection, next){


	dao_property_incr(getThis(), SL("_position"));

}

/**
 * Check if the current annotation in the iterator is valid
 *
 * @return boolean
 */
PHP_METHOD(Dao_Annotations_Collection, valid){

	zval position = {}, annotations = {};

	dao_read_property(&position, getThis(), SL("_position"), PH_NOISY|PH_READONLY);
	dao_read_property(&annotations, getThis(), SL("_annotations"), PH_NOISY|PH_READONLY);
	if (dao_array_isset(&annotations, &position)) {
		RETURN_TRUE;
	}

	RETURN_FALSE;
}

/**
 * Returns the internal annotations as an array
 *
 * @return Dao\Annotations\Annotation[]
 */
PHP_METHOD(Dao_Annotations_Collection, getAnnotations){


	RETURN_MEMBER(getThis(), "_annotations");
}

/**
 * Returns the first annotation that match a name
 *
 * @param string $name
 * @return Dao\Annotations\Annotation
 */
PHP_METHOD(Dao_Annotations_Collection, get){

	zval *name, annotations = {}, *annotation, exception_message = {};

	dao_fetch_params(0, 1, 0, &name);

	dao_read_property(&annotations, getThis(), SL("_annotations"), PH_READONLY);
	if (Z_TYPE(annotations) == IS_ARRAY) {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL(annotations), annotation) {
			zval annotation_name = {};
			DAO_CALL_METHOD(&annotation_name, annotation, "getname");
			if (DAO_IS_EQUAL(name, &annotation_name)) {
				zval_ptr_dtor(&annotation_name);
				RETURN_CTOR(annotation);
			}
			zval_ptr_dtor(&annotation_name);
		} ZEND_HASH_FOREACH_END();

	}

	DAO_CONCAT_SVS(&exception_message, "The collection doesn't have an annotation called '", name, "'");
	DAO_THROW_EXCEPTION_ZVAL(dao_annotations_exception_ce, &exception_message);
}

/**
 * Returns all the annotations that match a name
 *
 * @param string $name
 * @return Dao\Annotations\Annotation[]
 */
PHP_METHOD(Dao_Annotations_Collection, getAll){

	zval *name, annotations = {}, *annotation;

	dao_fetch_params(0, 1, 0, &name);

	array_init(return_value);

	dao_read_property(&annotations, getThis(), SL("_annotations"), PH_READONLY);
	if (Z_TYPE(annotations) == IS_ARRAY) {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL(annotations), annotation) {
			zval annotation_name = {};
			DAO_CALL_METHOD(&annotation_name, annotation, "getname");
			if (DAO_IS_EQUAL(name, &annotation_name)) {
				dao_array_append(return_value, annotation, PH_COPY);
			}
			zval_ptr_dtor(&annotation_name);
		} ZEND_HASH_FOREACH_END();
	}
}

/**
 * Check if an annotation exists in a collection
 *
 * @param string $name
 * @return boolean
 */
PHP_METHOD(Dao_Annotations_Collection, has){

	zval *name, annotations = {}, *annotation;

	dao_fetch_params(0, 1, 0, &name);

	dao_read_property(&annotations, getThis(), SL("_annotations"), PH_READONLY);
	if (Z_TYPE(annotations) == IS_ARRAY) {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL(annotations), annotation) {
			zval annotation_name = {};
			DAO_CALL_METHOD(&annotation_name, annotation, "getname");
			if (DAO_IS_EQUAL(name, &annotation_name)) {
				zval_ptr_dtor(&annotation_name);
				RETURN_TRUE;
			}
			zval_ptr_dtor(&annotation_name);
		} ZEND_HASH_FOREACH_END();
	}

	RETURN_FALSE;
}
