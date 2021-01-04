
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

#include "mvc/model/behavior.h"
#include "mvc/model/behaviorinterface.h"

#include "kernel/main.h"
#include "kernel/object.h"
#include "kernel/array.h"

/**
 * Dao\Mvc\Model\Behavior
 *
 * This is an optional base class for ORM behaviors
 */
zend_class_entry *dao_mvc_model_behavior_ce;

PHP_METHOD(Dao_Mvc_Model_Behavior, __construct);
PHP_METHOD(Dao_Mvc_Model_Behavior, mustTakeAction);
PHP_METHOD(Dao_Mvc_Model_Behavior, getOptions);
PHP_METHOD(Dao_Mvc_Model_Behavior, notify);
PHP_METHOD(Dao_Mvc_Model_Behavior, missingMethod);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_behavior___construct, 0, 0, 0)
	ZEND_ARG_INFO(0, options)
ZEND_END_ARG_INFO()


static const zend_function_entry dao_mvc_model_behavior_method_entry[] = {
	PHP_ME(Dao_Mvc_Model_Behavior, __construct, arginfo_dao_mvc_model_behavior___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Mvc_Model_Behavior, mustTakeAction, arginfo_empty, ZEND_ACC_PROTECTED)
	PHP_ME(Dao_Mvc_Model_Behavior, getOptions, arginfo_empty, ZEND_ACC_PROTECTED)
	PHP_ME(Dao_Mvc_Model_Behavior, notify, arginfo_dao_mvc_model_behaviorinterface_notify, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Behavior, missingMethod, arginfo_dao_mvc_model_behaviorinterface_missingmethod, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Mvc\Model\Behavior initializer
 */
DAO_INIT_CLASS(Dao_Mvc_Model_Behavior){

	DAO_REGISTER_CLASS(Dao\\Mvc\\Model, Behavior, mvc_model_behavior, dao_mvc_model_behavior_method_entry, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	zend_declare_property_null(dao_mvc_model_behavior_ce, SL("_options"), ZEND_ACC_PROTECTED);

	zend_class_implements(dao_mvc_model_behavior_ce, 1, dao_mvc_model_behaviorinterface_ce);

	return SUCCESS;
}

/**
 * Dao\Mvc\Model\Behavior
 *
 * @param array $options
 */
PHP_METHOD(Dao_Mvc_Model_Behavior, __construct){

	zval *options = NULL;

	dao_fetch_params(0, 0, 1, &options);

	if (!options) {
		options = &DAO_GLOBAL(z_null);
	}

	dao_update_property(getThis(), SL("_options"), options);
}

/**
 * Checks whether the behavior must take action on certain event
 *
 * @param string $eventName
 */
PHP_METHOD(Dao_Mvc_Model_Behavior, mustTakeAction){

	zval *eventname, options = {};

	dao_fetch_params(0, 1, 0, &eventname);

	dao_read_property(&options, getThis(), SL("_options"), PH_NOISY|PH_READONLY);
	if (dao_array_isset(&options, eventname)) {
		RETURN_TRUE;
	}

	RETURN_FALSE;
}

/**
 * Returns the behavior options related to an event
 *
 * @param string $eventName
 * @return array
 */
PHP_METHOD(Dao_Mvc_Model_Behavior, getOptions){

	zval *eventname = NULL, options = {}, event_options = {};

	dao_fetch_params(0, 0, 1, &eventname);

	dao_read_property(&options, getThis(), SL("_options"), PH_NOISY|PH_READONLY);
	if (eventname && Z_TYPE_P(eventname) != IS_NULL) {
		if (dao_array_isset_fetch(&event_options, &options, eventname, PH_READONLY)) {
			RETURN_CTOR(&event_options);
		}
		RETURN_NULL();
	}

	RETURN_ZVAL(&options, 1, 0);
}

/**
 * This method receives the notifications from the EventsManager
 *
 * @param string $type
 * @param Dao\Mvc\ModelInterface $model
 */
PHP_METHOD(Dao_Mvc_Model_Behavior, notify){

	zval *type, *model;

	dao_fetch_params(0, 2, 0, &type, &model);

	RETURN_NULL();
}

/**
 * Acts as fallbacks when a missing method is called on the model
 *
 * @param Dao\Mvc\ModelInterface $model
 * @param string $method
 * @param array $arguments
 */
PHP_METHOD(Dao_Mvc_Model_Behavior, missingMethod){

	zval *model, *method, *arguments = NULL;

	dao_fetch_params(0, 2, 1, &model, &method, &arguments);

	RETURN_NULL();
}
