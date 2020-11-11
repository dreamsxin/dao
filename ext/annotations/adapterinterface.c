
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

#include "annotations/adapterinterface.h"
#include "kernel/main.h"

zend_class_entry *dao_annotations_adapterinterface_ce;

static const zend_function_entry dao_annotations_adapterinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Annotations_AdapterInterface, read, arginfo_dao_annotations_adapterinterface_read)
	PHP_ABSTRACT_ME(Dao_Annotations_AdapterInterface, write, arginfo_dao_annotations_adapterinterface_write)
	PHP_ABSTRACT_ME(Dao_Annotations_AdapterInterface, setReader, arginfo_dao_annotations_adapterinterface_setreader)
	PHP_ABSTRACT_ME(Dao_Annotations_AdapterInterface, getReader, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Annotations_AdapterInterface, get, arginfo_dao_annotations_adapterinterface_get)
	PHP_ABSTRACT_ME(Dao_Annotations_AdapterInterface, getMethods, arginfo_dao_annotations_adapterinterface_getmethods)
	PHP_ABSTRACT_ME(Dao_Annotations_AdapterInterface, getMethod, arginfo_dao_annotations_adapterinterface_getmethod)
	PHP_ABSTRACT_ME(Dao_Annotations_AdapterInterface, getProperties, arginfo_dao_annotations_adapterinterface_getproperties)
	PHP_ABSTRACT_ME(Dao_Annotations_AdapterInterface, getProperty, arginfo_dao_annotations_adapterinterface_getproperty)
	PHP_FE_END
};

/**
 * Dao\Annotations\AdapterInterface initializer
 */
DAO_INIT_CLASS(Dao_Annotations_AdapterInterface){

	DAO_REGISTER_INTERFACE(Dao\\Annotations, AdapterInterface, annotations_adapterinterface, dao_annotations_adapterinterface_method_entry);

	return SUCCESS;
}

/**
 * Read parsed annotations
 * 
 * @param string $key
 * @return Dao\Annotations\Reflection
*/
DAO_DOC_METHOD(Dao_Annotations_AdapterInterface, read);

/**
 * Write parsed annotations
 * 
 * @param string $key
 * @param Dao\Annotations\Reflection $data
*/
DAO_DOC_METHOD(Dao_Annotations_AdapterInterface, write);

/**
 * Sets the annotations parser
 *
 * @param Dao\Annotations\ReaderInterface $reader
 */
DAO_DOC_METHOD(Dao_Annotations_AdapterInterface, setReader);

/**
 * Returns the annotation reader
 *
 * @return Dao\Annotations\ReaderInterface
 */
DAO_DOC_METHOD(Dao_Annotations_AdapterInterface, getReader);

/**
 * Parses or retrieves all the annotations found in a class
 *
 * @param string|object $className
 * @return Dao\Annotations\Reflection
 */
DAO_DOC_METHOD(Dao_Annotations_AdapterInterface, get);

/**
 * Returns the annotations found in all the class' methods
 *
 * @param string $className
 * @return array
 */
DAO_DOC_METHOD(Dao_Annotations_AdapterInterface, getMethods);

/**
 * Returns the annotations found in a specific method
 *
 * @param string $className
 * @param string $methodName
 * @return Dao\Annotations\Collection
 */
DAO_DOC_METHOD(Dao_Annotations_AdapterInterface, getMethod);

/**
 * Returns the annotations found in all the class' methods
 *
 * @param string $className
 * @return array
 */
DAO_DOC_METHOD(Dao_Annotations_AdapterInterface, getProperties);

/**
 * Returns the annotations found in a specific property
 *
 * @param string $className
 * @param string $propertyName
 * @return Dao\Annotations\Collection
 */
DAO_DOC_METHOD(Dao_Annotations_AdapterInterface, getProperty);

