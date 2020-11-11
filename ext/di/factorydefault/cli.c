
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

#include "di.h"
#include "di/factorydefault/cli.h"
#include "di/service.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/string.h"
#include "kernel/fcall.h"
#include "kernel/array.h"
#include "kernel/object.h"

#include "interned-strings.h"

/**
 * Dao\Di\FactoryDefault\Cli
 *
 * This is a variant of the standard Dao\Di. By default it automatically
 * registers all the services provided by the framework.
 * Thanks to this, the developer does not need to register each service individually.
 * This class is specially suitable for CLI applications
 */
zend_class_entry *dao_di_factorydefault_cli_ce;

PHP_METHOD(Dao_Di_FactoryDefault_Cli, __construct);

static const zend_function_entry dao_di_factorydefault_cli_method_entry[] = {
	PHP_ME(Dao_Di_FactoryDefault_Cli, __construct, arginfo_empty, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_FE_END
};

/**
 * Dao\Di\FactoryDefault\Cli initializer
 */
DAO_INIT_CLASS(Dao_Di_FactoryDefault_Cli){

	DAO_REGISTER_CLASS_EX(Dao\\Di\\FactoryDefault, Cli, di_factorydefault_cli, dao_di_ce, dao_di_factorydefault_cli_method_entry, 0);

	return SUCCESS;
}

/**
 * Dao\Di\FactoryDefault\Cli constructor
 */
PHP_METHOD(Dao_Di_FactoryDefault_Cli, __construct){

	zval *name = NULL, servicename = {}, definition = {};

	dao_fetch_params(0, 0, 1, &name);

	if (name) {
		DAO_CALL_PARENT(NULL, dao_di_factorydefault_cli_ce, getThis(), "__construct", name);
	} else {
		DAO_CALL_PARENT(NULL, dao_di_factorydefault_cli_ce, getThis(), "__construct");
	}

	ZVAL_STR(&servicename, IS(router));
	ZVAL_STRING(&definition, "Dao\\Cli\\Router");
	DAO_CALL_SELF(NULL, "set", &servicename, &definition, &DAO_GLOBAL(z_true));
	zval_ptr_dtor(&definition);

	ZVAL_STR(&servicename, IS(dispatcher));
	ZVAL_STRING(&definition, "Dao\\Cli\\Dispatcher");
	DAO_CALL_SELF(NULL, "set", &servicename, &definition, &DAO_GLOBAL(z_true));
	zval_ptr_dtor(&definition);

	/**
	 * Mvc Url
	 */
	ZVAL_STR(&servicename, IS(url));
	ZVAL_STRING(&definition, "Dao\\Mvc\\Url");
	DAO_CALL_SELF(NULL, "set", &servicename, &definition, &DAO_GLOBAL(z_true));
	zval_ptr_dtor(&definition);

	/**
	 * Models manager for ORM
	 */
	ZVAL_STR(&servicename, IS(modelsManager));
	ZVAL_STRING(&definition, "Dao\\Mvc\\Model\\Manager");
	DAO_CALL_SELF(NULL, "set", &servicename, &definition, &DAO_GLOBAL(z_true));
	zval_ptr_dtor(&definition);

	/**
	 * Models meta-data using the Memory adapter
	 */
	ZVAL_STR(&servicename, IS(modelsMetadata));
	ZVAL_STRING(&definition, "Dao\\Mvc\\Model\\MetaData\\Memory");
	DAO_CALL_SELF(NULL, "set", &servicename, &definition, &DAO_GLOBAL(z_true));
	zval_ptr_dtor(&definition);

	/**
	 * Models Query for ORM
	 */
	ZVAL_STR(&servicename, IS(modelsQuery));
	ZVAL_STRING(&definition, "Dao\\Mvc\\Model\\Query");
	DAO_CALL_SELF(NULL, "set", &servicename, &definition, &DAO_GLOBAL(z_false));
	zval_ptr_dtor(&definition);

	/**
	 * Models Query Select Builder for ORM
	 */
	ZVAL_STR(&servicename, IS(modelsQueryBuilderForSelect));
	ZVAL_STRING(&definition, "Dao\\Mvc\\Model\\Query\\Builder\\Select");
	DAO_CALL_SELF(NULL, "set", &servicename, &definition, &DAO_GLOBAL(z_false));
	zval_ptr_dtor(&definition);

	/**
	 * Models Criteria for ORM
	 */
	ZVAL_STR(&servicename, IS(modelsCriteria));
	ZVAL_STRING(&definition, "Dao\\Mvc\\Model\\Criteria");
	DAO_CALL_SELF(NULL, "set", &servicename, &definition, &DAO_GLOBAL(z_false));
	zval_ptr_dtor(&definition);

	/**
	 * Request/Response are always &DAO_GLOBAL(z_true)
	 */
	ZVAL_STR(&servicename, IS(response));
	ZVAL_STRING(&definition, "Dao\\Http\\Response");
	DAO_CALL_SELF(NULL, "set", &servicename, &definition, &DAO_GLOBAL(z_true));
	zval_ptr_dtor(&definition);

	/**
	 * Http Response Cookies
	 */
	ZVAL_STR(&servicename, IS(cookies));
	ZVAL_STRING(&definition, "Dao\\Http\\Response\\Cookies");
	DAO_CALL_SELF(NULL, "set", &servicename, &definition, &DAO_GLOBAL(z_true));
	zval_ptr_dtor(&definition);

	/**
	 * Http Request
	 */
	ZVAL_STR(&servicename, IS(request));
	ZVAL_STRING(&definition, "Dao\\Http\\Request");
	DAO_CALL_SELF(NULL, "set", &servicename, &definition, &DAO_GLOBAL(z_true));
	zval_ptr_dtor(&definition);

	/**
	 * Filter/Escaper services are always &DAO_GLOBAL(z_true)
	 */
	ZVAL_STR(&servicename, IS(filter));
	ZVAL_STRING(&definition, "Dao\\Filter");
	DAO_CALL_SELF(NULL, "set", &servicename, &definition, &DAO_GLOBAL(z_true));
	zval_ptr_dtor(&definition);

	/**
	 * Escaper
	 */
	ZVAL_STR(&servicename, IS(escaper));
	ZVAL_STRING(&definition, "Dao\\Escaper");
	DAO_CALL_SELF(NULL, "set", &servicename, &definition, &DAO_GLOBAL(z_true));
	zval_ptr_dtor(&definition);

	/**
	 * Default annotations service
	 */
	ZVAL_STR(&servicename, IS(annotations));
	ZVAL_STRING(&definition, "Dao\\Annotations\\Adapter\\Memory");
	DAO_CALL_SELF(NULL, "set", &servicename, &definition, &DAO_GLOBAL(z_true));
	zval_ptr_dtor(&definition);

	/**
	 * Security doesn't need to be &DAO_GLOBAL(z_true), but anyways we register it as &DAO_GLOBAL(z_true)
	 */
	ZVAL_STR(&servicename, IS(security));
	ZVAL_STRING(&definition, "Dao\\Security");
	DAO_CALL_SELF(NULL, "set", &servicename, &definition, &DAO_GLOBAL(z_true));
	zval_ptr_dtor(&definition);

	/**
	 * Crypt Service
	 */
	ZVAL_STR(&servicename, IS(crypt));
	ZVAL_STRING(&definition, "Dao\\Crypt");
	DAO_CALL_SELF(NULL, "set", &servicename, &definition, &DAO_GLOBAL(z_true));
	zval_ptr_dtor(&definition);

	/**
	 * Flash services are always &DAO_GLOBAL(z_true)
	 */
	ZVAL_STR(&servicename, IS(flash));
	ZVAL_STRING(&definition, "Dao\\Flash\\Direct");
	DAO_CALL_SELF(NULL, "set", &servicename, &definition, &DAO_GLOBAL(z_true));
	zval_ptr_dtor(&definition);

	/**
	 * Flash Session
	 */
	ZVAL_STR(&servicename, IS(flashSession));
	ZVAL_STRING(&definition, "Dao\\Flash\\Session");
	DAO_CALL_SELF(NULL, "set", &servicename, &definition, &DAO_GLOBAL(z_true));
	zval_ptr_dtor(&definition);

	/**
	 * Tag/Helpers
	 */
	ZVAL_STR(&servicename, IS(tag));
	ZVAL_STRING(&definition, "Dao\\Tag");
	DAO_CALL_SELF(NULL, "set", &servicename, &definition, &DAO_GLOBAL(z_true));
	zval_ptr_dtor(&definition);

	/**
	 * Session is always &DAO_GLOBAL(z_true)
	 */
	ZVAL_STR(&servicename, IS(session));
	ZVAL_STRING(&definition, "Dao\\Session\\Adapter\\Files");
	DAO_CALL_SELF(NULL, "set", &servicename, &definition, &DAO_GLOBAL(z_true));
	zval_ptr_dtor(&definition);

	/**
	 * Session/Bag
	 */
	ZVAL_STR(&servicename, IS(sessionBag));
	ZVAL_STRING(&definition, "Dao\\Session\\Bag");
	DAO_CALL_SELF(NULL, "set", &servicename, &definition, &DAO_GLOBAL(z_true));
	zval_ptr_dtor(&definition);

	/**
	 * Events Manager is always &DAO_GLOBAL(z_true)
	 */
	ZVAL_STR(&servicename, IS(eventsManager));
	ZVAL_STRING(&definition, "Dao\\Events\\Manager");
	DAO_CALL_SELF(NULL, "set", &servicename, &definition, &DAO_GLOBAL(z_true));
	zval_ptr_dtor(&definition);

	/**
	 * Model Transaction Manager
	 */
	ZVAL_STR(&servicename, IS(transactionManager));
	ZVAL_STRING(&definition, "Dao\\Mvc\\Model\\Transaction\\Manager");
	DAO_CALL_SELF(NULL, "set", &servicename, &definition, &DAO_GLOBAL(z_true));
	zval_ptr_dtor(&definition);
}
