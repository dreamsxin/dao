
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

#include "mvc/micro/lazyloader.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/exception.h"
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/array.h"

/**
 * Dao\Mvc\Micro\LazyLoader
 *
 * Lazy-Load of handlers for Mvc\Micro using auto-loading
 */
zend_class_entry *dao_mvc_micro_lazyloader_ce;

PHP_METHOD(Dao_Mvc_Micro_LazyLoader, __construct);
PHP_METHOD(Dao_Mvc_Micro_LazyLoader, __call);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_micro_lazyloader___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, definition)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_micro_lazyloader___call, 0, 0, 2)
	ZEND_ARG_INFO(0, method)
	ZEND_ARG_INFO(0, arguments)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_mvc_micro_lazyloader_method_entry[] = {
	PHP_ME(Dao_Mvc_Micro_LazyLoader, __construct, arginfo_dao_mvc_micro_lazyloader___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Mvc_Micro_LazyLoader, __call, arginfo_dao_mvc_micro_lazyloader___call, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Mvc\Micro\LazyLoader initializer
 */
DAO_INIT_CLASS(Dao_Mvc_Micro_LazyLoader){

	DAO_REGISTER_CLASS(Dao\\Mvc\\Micro, LazyLoader, mvc_micro_lazyloader, dao_mvc_micro_lazyloader_method_entry, 0);

	zend_declare_property_null(dao_mvc_micro_lazyloader_ce, SL("_handler"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_micro_lazyloader_ce, SL("_definition"), ZEND_ACC_PROTECTED);

	return SUCCESS;
}

/**
 * Dao\Mvc\Micro\LazyLoader constructor
 *
 * @param string $definition
 */
PHP_METHOD(Dao_Mvc_Micro_LazyLoader, __construct){

	zval *definition;

	dao_fetch_params(0, 1, 0, &definition);
	DAO_ENSURE_IS_STRING(definition);

	dao_update_property(getThis(), SL("_definition"), definition);
}

/**
 * Initializes the internal handler, calling functions on it
 *
 * @param string $method
 * @param array $arguments
 * @return mixed
 */
PHP_METHOD(Dao_Mvc_Micro_LazyLoader, __call){

	zval *method, *arguments, handler = {}, definition = {}, call_handler = {};
	zend_class_entry *ce0;

	dao_fetch_params(0, 2, 0, &method, &arguments);

	dao_read_property(&handler, getThis(), SL("_handler"), PH_READONLY);
	if (Z_TYPE(handler) != IS_OBJECT) {
		dao_read_property(&definition, getThis(), SL("_definition"), PH_NOISY|PH_READONLY);
		ce0 = dao_fetch_class(&definition, ZEND_FETCH_CLASS_DEFAULT);

		DAO_OBJECT_INIT(&handler, ce0);
		if (dao_has_constructor(&handler)) {
			DAO_CALL_METHOD(NULL, &handler, "__construct");
		}
		dao_update_property(getThis(), SL("_handler"), &handler);
	}

	array_init_size(&call_handler, 2);
	dao_array_append(&call_handler, &handler, PH_COPY);
	dao_array_append(&call_handler, method, PH_COPY);

	/**
	 * Call the handler
	 */
	DAO_CALL_USER_FUNC_ARRAY(return_value, &call_handler, arguments);
}
