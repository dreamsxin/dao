
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

#include "annotations/readerinterface.h"
#include "kernel/main.h"

zend_class_entry *dao_annotations_readerinterface_ce;

static const zend_function_entry dao_annotations_readerinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Annotations_ReaderInterface, parse, arginfo_dao_annotations_readerinterface_parse)
	ZEND_FENTRY(parseDocBlock, NULL, arginfo_dao_annotations_readerinterface_parsedocblock, ZEND_ACC_STATIC|ZEND_ACC_ABSTRACT|ZEND_ACC_PUBLIC)
	PHP_FE_END
};


/**
 * Dao\Annotations\ReaderInterface initializer
 */
DAO_INIT_CLASS(Dao_Annotations_ReaderInterface){

	DAO_REGISTER_INTERFACE(Dao\\Annotations, ReaderInterface, annotations_readerinterface, dao_annotations_readerinterface_method_entry);

	return SUCCESS;
}

/**
 * Reads annotations from the class dockblocks, its methods and/or properties
 *
 * @param string $className
 * @return array
 */
DAO_DOC_METHOD(Dao_Annotations_ReaderInterface, parse);

/**
 * Parses a raw doc block returning the annotations found
 *
 * @param string $docBlock
 * @return array
 */
DAO_DOC_METHOD(Dao_Annotations_ReaderInterface, parseDocBlock);

