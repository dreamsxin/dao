
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

#include "annotations/adapter/apc.h"
#include "annotations/adapter.h"
#include "annotations/adapterinterface.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/concat.h"
#include "kernel/fcall.h"
#include "kernel/string.h"

/**
 * Dao\Annotations\Adapter\Apc
 *
 * Stores the parsed annotations in APC. This adapter is suitable for production
 *
 *<code>
 * $annotations = new \Dao\Annotations\Adapter\Apc();
 *</code>
 */
zend_class_entry *dao_annotations_adapter_apc_ce;

PHP_METHOD(Dao_Annotations_Adapter_Apc, read);
PHP_METHOD(Dao_Annotations_Adapter_Apc, write);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_annotations_adapter_apc_read, 0, 0, 1)
	ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_annotations_adapter_apc_write, 0, 0, 2)
	ZEND_ARG_INFO(0, key)
	ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_annotations_adapter_apc_method_entry[] = {
	PHP_ME(Dao_Annotations_Adapter_Apc, read, arginfo_dao_annotations_adapter_apc_read, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Annotations_Adapter_Apc, write, arginfo_dao_annotations_adapter_apc_write, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Annotations\Adapter\Apc initializer
 */
DAO_INIT_CLASS(Dao_Annotations_Adapter_Apc){

	DAO_REGISTER_CLASS_EX(Dao\\Annotations\\Adapter, Apc, annotations_adapter_apc, dao_annotations_adapter_ce, dao_annotations_adapter_apc_method_entry, 0);

	zend_class_implements(dao_annotations_adapter_apc_ce, 1, dao_annotations_adapterinterface_ce);

	return SUCCESS;
}

/**
 * Reads parsed annotations from APC
 *
 * @param string $key
 * @return Dao\Annotations\Reflection
 */
PHP_METHOD(Dao_Annotations_Adapter_Apc, read)
{
	zval *key, prefixed_key = {};

	dao_fetch_params(0, 1, 0, &key);

	DAO_CONCAT_SV(&prefixed_key, "_PHAN", key);
	dao_strtolower_inplace(&prefixed_key);

	DAO_RETURN_CALL_FUNCTION("apc_fetch", &prefixed_key);
	if (Z_TYPE_P(return_value) != IS_OBJECT) {
		RETURN_NULL();
	}
}

/**
 * Writes parsed annotations to APC
 *
 * @param string $key
 * @param Dao\Annotations\Reflection $data
 */
PHP_METHOD(Dao_Annotations_Adapter_Apc, write){

	zval *key, *data, prefixed_key = {};

	dao_fetch_params(0, 2, 0, &key, &data);

	DAO_CONCAT_SV(&prefixed_key, "_PHAN", key);

	dao_strtolower_inplace(&prefixed_key);
	DAO_CALL_FUNCTION(NULL, "apc_store", &prefixed_key, data);
}
