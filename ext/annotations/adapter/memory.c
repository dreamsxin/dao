
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

#include "annotations/adapter/memory.h"
#include "annotations/adapter.h"
#include "annotations/adapterinterface.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/string.h"
#include "kernel/array.h"

/**
 * Dao\Annotations\Adapter\Memory
 *
 * Stores the parsed annotations in memory. This adapter is the suitable development/testing
 */
zend_class_entry *dao_annotations_adapter_memory_ce;

PHP_METHOD(Dao_Annotations_Adapter_Memory, read);
PHP_METHOD(Dao_Annotations_Adapter_Memory, write);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_annotations_adapter_memory_read, 0, 0, 1)
	ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_annotations_adapter_memory_write, 0, 0, 2)
	ZEND_ARG_INFO(0, key)
	ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_annotations_adapter_memory_method_entry[] = {
	PHP_ME(Dao_Annotations_Adapter_Memory, read, arginfo_dao_annotations_adapter_memory_read, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Annotations_Adapter_Memory, write, arginfo_dao_annotations_adapter_memory_write, ZEND_ACC_PUBLIC)
	PHP_FE_END
};


/**
 * Dao\Annotations\Adapter\Memory initializer
 */
DAO_INIT_CLASS(Dao_Annotations_Adapter_Memory){

	DAO_REGISTER_CLASS_EX(Dao\\Annotations\\Adapter, Memory, annotations_adapter_memory, dao_annotations_adapter_ce, dao_annotations_adapter_memory_method_entry, 0);

	zend_declare_property_null(dao_annotations_adapter_memory_ce, SL("_data"), ZEND_ACC_PROTECTED);

	zend_class_implements(dao_annotations_adapter_memory_ce, 1, dao_annotations_adapterinterface_ce);

	return SUCCESS;
}

/**
 * Reads parsed annotations from memory
 *
 * @param string $key
 * @return Dao\Annotations\Reflection
 */
PHP_METHOD(Dao_Annotations_Adapter_Memory, read){

	zval *key, data = {}, lowercased_key = {};

	dao_fetch_params(0, 1, 0, &key);

	dao_read_property(&data, getThis(), SL("_data"), PH_READONLY);

	dao_fast_strtolower(&lowercased_key, key);
	if (!dao_array_isset_fetch(return_value, &data, &lowercased_key, PH_COPY)) {
		RETURN_NULL();
	}
}

/**
 * Writes parsed annotations to memory
 *
 * @param string $key
 * @param Dao\Annotations\Reflection $data
 */
PHP_METHOD(Dao_Annotations_Adapter_Memory, write){

	zval *key, *data, lowercased_key = {};

	dao_fetch_params(0, 2, 0, &key, &data);

	dao_fast_strtolower(&lowercased_key, key);
	dao_update_property_array(getThis(), SL("_data"), &lowercased_key, data);
}
