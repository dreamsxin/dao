
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

#include "annotations/reader.h"
#include "annotations/readerinterface.h"
#include "annotations/annot.h"
#include "annotations/exception.h"
#include "annotations/scanner.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/exception.h"
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/array.h"
#include "kernel/file.h"
#include "kernel/reflection.h"

/**
 * Dao\Annotations\Reader
 *
 * Parses docblocks returning an array with the found annotations
 */
zend_class_entry *dao_annotations_reader_ce;

PHP_METHOD(Dao_Annotations_Reader, parse);
PHP_METHOD(Dao_Annotations_Reader, parseDocBlock);

static const zend_function_entry dao_annotations_reader_method_entry[] = {
	PHP_ME(Dao_Annotations_Reader, parse, arginfo_dao_annotations_readerinterface_parse, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Annotations_Reader, parseDocBlock, arginfo_dao_annotations_readerinterface_parsedocblock, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};

/**
 * Dao\Annotations\Reader initializer
 */
DAO_INIT_CLASS(Dao_Annotations_Reader){

	DAO_REGISTER_CLASS(Dao\\Annotations, Reader, annotations_reader, dao_annotations_reader_method_entry, 0);

	zend_class_implements(dao_annotations_reader_ce, 1, dao_annotations_readerinterface_ce);

	return SUCCESS;
}

/**
 * Reads annotations from the class dockblocks, its methods and/or properties
 *
 * @param string $className
 * @return array
 */
PHP_METHOD(Dao_Annotations_Reader, parse){

	zval *class_name, class_annotations = {}, annotations_properties = {}, annotations_methods = {};
	zend_property_info *property;
	zend_class_entry *class_ce;
	zend_function *method;
	zend_string *cmt;
	HashTable *props, *methods;
	const char *file;
	uint32_t line;

	dao_fetch_params(1, 1, 0, &class_name);

	if (unlikely(Z_TYPE_P(class_name) != IS_STRING)) {
		DAO_MM_THROW_EXCEPTION_STR(dao_annotations_exception_ce, "The class name must be a string");
		return;
	}

	class_ce = zend_fetch_class(Z_STR_P(class_name), ZEND_FETCH_CLASS_AUTO | ZEND_FETCH_CLASS_SILENT);
	if (!class_ce) {
		DAO_MM_THROW_EXCEPTION_FORMAT(dao_annotations_exception_ce, "Class %s does not exist", Z_STRVAL_P(class_name));
		return;
	}

	array_init(return_value);
	if (class_ce->type != ZEND_USER_CLASS) {
		RETURN_MM();
	}

	file = ZSTR_VAL(dao_get_class_filename(class_ce));
	if (!file) {
		file = "(unknown)";
	}

	/* Class info */
	if ((cmt = dao_get_class_doc_comment(class_ce)) != NULL) {
		line = dao_get_class_startline(class_ce);

		RETURN_MM_ON_FAILURE(phannot_parse_annotations(&class_annotations, cmt, file, line));
		DAO_MM_ADD_ENTRY(&class_annotations);

		if (Z_TYPE(class_annotations) == IS_ARRAY) {
			dao_array_update_str(return_value, SL("class"), &class_annotations, PH_COPY);
		}
	}

	/* Get class properties */
	props = &class_ce->properties_info;
	if (zend_hash_num_elements(props) > 0) {
		array_init_size(&annotations_properties, zend_hash_num_elements(props));
		DAO_MM_ADD_ENTRY(&annotations_properties);
		ZEND_HASH_FOREACH_PTR(props, property) {
			zval property_annotations = {};
			zend_string *cmt;
			const char *prop_name, *class_name;

			if ((cmt = dao_get_property_doc_comment(property)) != NULL) {
				if (FAILURE == phannot_parse_annotations(&property_annotations, cmt, file, 0)) {
					return;
				}
				DAO_MM_ADD_ENTRY(&property_annotations);

				if (Z_TYPE(property_annotations) == IS_ARRAY) {
					if (zend_unmangle_property_name(property->name, &class_name, &prop_name) == SUCCESS) {
						dao_array_update_str(&annotations_properties, prop_name, strlen(prop_name), &property_annotations, PH_COPY);
						continue;
					}
				}
			}
		} ZEND_HASH_FOREACH_END();

		if (zend_hash_num_elements(Z_ARRVAL(annotations_properties))) {
			dao_array_update_str(return_value, SL("properties"), &annotations_properties, PH_COPY);
		}
	}

	/* Get class methods */
	methods = &class_ce->function_table;
	if (zend_hash_num_elements(methods) > 0) {
		array_init_size(&annotations_methods, zend_hash_num_elements(methods));
		DAO_MM_ADD_ENTRY(&annotations_methods);
		ZEND_HASH_FOREACH_PTR(methods, method) {
			zval method_annotations = {};
			zend_string *cmt;

			if ((cmt = dao_get_function_doc_comment(method)) != NULL) {
				line = dao_get_function_startline(method);

				if (FAILURE == phannot_parse_annotations(&method_annotations, cmt, file, line)) {
					return;
				}
				DAO_MM_ADD_ENTRY(&method_annotations);

				if (Z_TYPE(method_annotations) == IS_ARRAY) {
					dao_array_update_string(&annotations_methods, method->common.function_name, &method_annotations, PH_COPY);
				}
			}
		} ZEND_HASH_FOREACH_END();

		if (zend_hash_num_elements(Z_ARRVAL(annotations_methods))) {
			dao_array_update_str(return_value, SL("methods"), &annotations_methods, PH_COPY);
		}
	}
	RETURN_MM();
}

/**
 * Parses a raw doc block returning the annotations found
 *
 * @param string $docBlock
 * @param string $file
 * @param int $line
 * @return array
 */
PHP_METHOD(Dao_Annotations_Reader, parseDocBlock)
{
	zval *doc_block, *file = NULL, *line = NULL;

	dao_fetch_params(0, 3, 0, &doc_block, &file, &line);

	DAO_ENSURE_IS_STRING(doc_block);
	DAO_ENSURE_IS_STRING(file);
	DAO_ENSURE_IS_LONG(line);

	RETURN_ON_FAILURE(phannot_parse_annotations(return_value, Z_STR_P(doc_block), Z_STRVAL_P(file), Z_LVAL_P(line)));
}
