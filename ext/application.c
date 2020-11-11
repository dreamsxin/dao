
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

#include "mvc/../application.h"
#include "application/exception.h"
#include "di/injectable.h"

#include <Zend/zend_closures.h>

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/exception.h"
#include "kernel/operators.h"
#include "kernel/fcall.h"
#include "kernel/array.h"

/**
 * Dao\Application
 *
 * Base class for Dao\Cli\Console and Dao\Mvc\Application
 *
 *</code>
 */
zend_class_entry *dao_application_ce;

PHP_METHOD(Dao_Application, registerModules);
PHP_METHOD(Dao_Application, getModules);
PHP_METHOD(Dao_Application, setDefaultModule);
PHP_METHOD(Dao_Application, getDefaultModule);

static const zend_function_entry dao_application_method_entry[] = {
	PHP_ME(Dao_Application, registerModules, arginfo_dao_application_registermodules, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Application, getModules, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Application, setDefaultModule, arginfo_dao_application_setdefaultmodule, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Application, getDefaultModule, arginfo_empty, ZEND_ACC_PUBLIC)
	ZEND_FENTRY(handle, NULL, arginfo_dao_application_handle, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
	PHP_FE_END
};

/**
 * Dao\Application initializer
 */
DAO_INIT_CLASS(Dao_Application){

	DAO_REGISTER_CLASS_EX(Dao, Application, application, dao_di_injectable_ce, dao_application_method_entry, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	zend_declare_property_null(dao_application_ce, SL("_defaultModule"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_application_ce, SL("_modules"), ZEND_ACC_PROTECTED);
	zend_declare_property_bool(dao_application_ce, SL("_implicitView"), 1, ZEND_ACC_PROTECTED);

	return SUCCESS;
}

/**
 * Register an array of modules present in the application
 *
 *<code>
 *	$this->registerModules(array(
 *		'frontend' => array(
 *			'className' => 'Multiple\Frontend\Module',
 *			'path' => '../apps/frontend/Module.php'
 *		),
 *		'backend' => array(
 *			'className' => 'Multiple\Backend\Module',
 *			'path' => '../apps/backend/Module.php'
 *		)
 *	));
 *</code>
 *
 * @param array $modules
 * @param boolean $merge
 * @param Dao\Application
 */
PHP_METHOD(Dao_Application, registerModules){

	zval *modules, *merge = NULL, registered_modules = {}, merged_modules = {};

	dao_fetch_params(0, 1, 1, &modules, &merge);

	if (!merge) {
		merge = &DAO_GLOBAL(z_false);
	}

	if (DAO_IS_FALSE(merge)) {
		dao_update_property(getThis(), SL("_modules"), modules);
	} else {
		dao_read_property(&registered_modules, getThis(), SL("_modules"), PH_NOISY|PH_READONLY);
		if (Z_TYPE(registered_modules) == IS_ARRAY) {
			dao_fast_array_merge(&merged_modules, &registered_modules, modules);
		} else {
			ZVAL_COPY(&merged_modules, modules);
		}

		dao_update_property(getThis(), SL("_modules"), &merged_modules);
		zval_ptr_dtor(&merged_modules);
	}

	RETURN_THIS();
}

/**
 * Return the modules registered in the application
 *
 * @return array
 */
PHP_METHOD(Dao_Application, getModules){


	RETURN_MEMBER(getThis(), "_modules");
}

/**
 * Sets the module name to be used if the router doesn't return a valid module
 *
 * @param string $defaultModule
 * @return Dao\Application
 */
PHP_METHOD(Dao_Application, setDefaultModule){

	zval *default_module;

	dao_fetch_params(0, 1, 0, &default_module);

	dao_update_property(getThis(), SL("_defaultModule"), default_module);
	RETURN_THIS();
}

/**
 * Returns the default module name
 *
 * @return string
 */
PHP_METHOD(Dao_Application, getDefaultModule){


	RETURN_MEMBER(getThis(), "_defaultModule");
}
