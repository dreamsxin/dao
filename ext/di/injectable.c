
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

#include "di/injectable.h"
#include "di/exception.h"
#include "di/injectionawareinterface.h"
#include "diinterface.h"
#include "di.h"
#include "events/eventsawareinterface.h"
#include "events/managerinterface.h"
#include "events/event.h"
#include "diinterface.h"
#include "debug.h"

#include <Zend/zend_closures.h>

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/exception.h"
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/concat.h"
#include "kernel/array.h"
#include "kernel/operators.h"
#include "kernel/string.h"
#include "kernel/debug.h"

#include "interned-strings.h"

/**
 * Dao\Di\Injectable
 *
 * This class allows to access services in the services container by just only accessing a public property
 * with the same name of a registered service
 */
zend_class_entry *dao_di_injectable_ce;

PHP_METHOD(Dao_Di_Injectable, setDI);
PHP_METHOD(Dao_Di_Injectable, getDI);
PHP_METHOD(Dao_Di_Injectable, setEventsManager);
PHP_METHOD(Dao_Di_Injectable, getEventsManager);
PHP_METHOD(Dao_Di_Injectable, fireEvent);
PHP_METHOD(Dao_Di_Injectable, fireEventCancel);
PHP_METHOD(Dao_Di_Injectable, hasService);
PHP_METHOD(Dao_Di_Injectable, setService);
PHP_METHOD(Dao_Di_Injectable, getService);
PHP_METHOD(Dao_Di_Injectable, getResolveService);
PHP_METHOD(Dao_Di_Injectable, attachEvent);
PHP_METHOD(Dao_Di_Injectable, __get);
PHP_METHOD(Dao_Di_Injectable, __sleep);
PHP_METHOD(Dao_Di_Injectable, __debugInfo);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_di_injectable_attachevent, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, eventType, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, callback, Closure, 0)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_di_injectable_method_entry[] = {
	PHP_ME(Dao_Di_Injectable, setDI, arginfo_dao_di_injectionawareinterface_setdi, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Di_Injectable, getDI, arginfo_dao_di_injectionawareinterface_getdi, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Di_Injectable, setEventsManager, arginfo_dao_events_eventsawareinterface_seteventsmanager, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Di_Injectable, getEventsManager, arginfo_dao_events_eventsawareinterface_geteventsmanager, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Di_Injectable, fireEvent, arginfo_dao_di_injectable_fireevent, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Di_Injectable, fireEventCancel, arginfo_dao_di_injectable_fireeventcancel, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Di_Injectable, hasService, arginfo_dao_di_injectable_hasservice, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Di_Injectable, setService, arginfo_dao_di_injectable_setservice, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Di_Injectable, getService, arginfo_dao_di_injectable_getservice, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Di_Injectable, getResolveService, arginfo_dao_di_injectable_getresolveservice, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Di_Injectable, attachEvent, arginfo_dao_di_injectable_attachevent, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Di_Injectable, __get, arginfo___get, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Di_Injectable, __sleep, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Di_Injectable, __debugInfo, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_FE_END
};


/**
 * Dao\Di\Injectable initializer
 */
DAO_INIT_CLASS(Dao_Di_Injectable){

	DAO_REGISTER_CLASS(Dao\\Di, Injectable, di_injectable, dao_di_injectable_method_entry, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	zend_declare_property_null(dao_di_injectable_ce, SL("_dependencyInjector"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_di_injectable_ce, SL("_eventsManager"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_di_injectable_ce, SL("_eventCallbacks"), ZEND_ACC_PROTECTED);

	zend_class_implements(dao_di_injectable_ce, 2, dao_di_injectionawareinterface_ce, dao_events_eventsawareinterface_ce);

	return SUCCESS;
}

/**
 * Sets the dependency injector
 *
 * @param Dao\DiInterface $dependencyInjector
 * @throw Dao\Di\Exception
 */
PHP_METHOD(Dao_Di_Injectable, setDI){

	zval *dependency_injector, di = {};

	dao_fetch_params(0, 1, 0, &dependency_injector);

	if (Z_TYPE_P(dependency_injector) != IS_OBJECT && DAO_IS_NOT_EMPTY(dependency_injector)) {
		DAO_CALL_METHOD(&di, getThis(), "getdi", dependency_injector);
		dao_update_property(getThis(), SL("_dependencyInjector"), &di);
	} else {
		DAO_VERIFY_INTERFACE_OR_NULL_EX(dependency_injector, dao_diinterface_ce, dao_di_exception_ce);
		dao_update_property(getThis(), SL("_dependencyInjector"), dependency_injector);
	}

	RETURN_THIS();
}

/**
 * Returns the internal dependency injector
 *
 * @return Dao\DiInterface
 */
PHP_METHOD(Dao_Di_Injectable, getDI)
{
	zval *error = NULL, *not_use_default = NULL;

	dao_fetch_params(0, 0, 2, &error, &not_use_default);

	if (!error) {
		error = &DAO_GLOBAL(z_false);
	}

	if (!not_use_default) {
		not_use_default = &DAO_GLOBAL(z_false);
	}

	dao_read_property(return_value, getThis(), SL("_dependencyInjector"), PH_COPY);
	if (Z_TYPE_P(return_value) != IS_OBJECT && !zend_is_true(not_use_default)) {
		DAO_CALL_CE_STATIC(return_value, dao_di_ce, "getdefault", return_value);
	}

	if (Z_TYPE_P(return_value) != IS_OBJECT && zend_is_true(error)) {
		DAO_THROW_EXCEPTION_STR(dao_di_exception_ce, "A dependency injection container is not object");
		return;
	}
}

/**
 * Sets the event manager
 *
 * @param Dao\Events\ManagerInterface $eventsManager
 */
PHP_METHOD(Dao_Di_Injectable, setEventsManager)
{
	zval *events_manager;

	dao_fetch_params(0, 1, 0, &events_manager);
	DAO_VERIFY_INTERFACE_OR_NULL_EX(events_manager, dao_events_managerinterface_ce, dao_di_exception_ce);

	dao_update_property(getThis(), SL("_eventsManager"), events_manager);

	RETURN_THIS();
}

/**
 * Returns the internal event manager
 *
 * @return Dao\Events\ManagerInterface
 */
PHP_METHOD(Dao_Di_Injectable, getEventsManager){

	zval service_name = {};

	dao_read_property(return_value, getThis(), SL("_eventsManager"), PH_COPY);
	if (Z_TYPE_P(return_value) != IS_OBJECT) {
		ZVAL_STR(&service_name, IS(eventsManager));
		DAO_CALL_METHOD(return_value, getThis(), "getservice", &service_name);
	}
}

/**
 * Fires an event, implicitly calls behaviors and listeners in the events manager are notified
 *
 * @param string $eventName
 * @param mixed $data
 * @return boolean
 */
PHP_METHOD(Dao_Di_Injectable, fireEvent){

	zval *eventname, *data = NULL, *cancelable = NULL, *flag = NULL, eventtype = {}, callback = {}, events_manager = {}, event = {};
	zval is_stopped = {}, lower = {}, event_parts = {}, name = {}, status = {}, debug_message = {};

	dao_fetch_params(1, 1, 2, &eventname, &data, &cancelable);

	if (unlikely(DAO_GLOBAL(debug).enable_debug)) {
		DAO_CONCAT_SV(&debug_message, "--Event: ", eventname);
		DAO_DEBUG_LOG(&debug_message);
		zval_ptr_dtor(&debug_message);
	}

	if (!data) {
		data = &DAO_GLOBAL(z_null);
	}

	if (!cancelable) {
		cancelable = &DAO_GLOBAL(z_true);
	}

	if (!flag) {
		flag = &DAO_GLOBAL(z_false);
	}

	dao_fast_strtolower(&lower, eventname);
	DAO_MM_ADD_ENTRY(&lower);
	if (dao_memnstr_str(&lower, SL(":"))) {
		dao_fast_explode_str(&event_parts, SL(":"), &lower);
		dao_array_fetch_long(&name, &event_parts, 1, PH_COPY);
		DAO_MM_ADD_ENTRY(&name);
		zval_ptr_dtor(&event_parts);
		DAO_MM_ZVAL_COPY(&eventtype, eventname);
	} else {
		zval class_name = {};
		DAO_MM_ZVAL_COPY(&name, &lower);

		dao_get_called_class(&class_name);
		DAO_CONCAT_VSV(&eventtype, &class_name, ":", eventname);
		DAO_MM_ADD_ENTRY(&eventtype);
		zval_ptr_dtor(&class_name);
	}

	DAO_MM_CALL_METHOD(&events_manager, getThis(), "geteventsmanager");
	DAO_MM_ADD_ENTRY(&events_manager);

	if (Z_TYPE(events_manager) != IS_NULL) {
		DAO_MM_VERIFY_INTERFACE_EX(&events_manager, dao_events_managerinterface_ce, dao_di_exception_ce);

		/**
		 * Send a notification to the events manager
		 */
		DAO_MM_CALL_METHOD(&status, &events_manager, "fire", &eventtype, getThis(), data, cancelable);
		DAO_MM_ADD_ENTRY(&status);
		DAO_MM_CALL_METHOD(&event, &events_manager, "getcurrentevent");
		DAO_MM_ADD_ENTRY(&event);
		if (Z_TYPE(event) == IS_OBJECT) {
			DAO_MM_CALL_METHOD(&is_stopped, &event, "isstopped");
			if (zend_is_true(&is_stopped)) {
				RETURN_MM_NCTOR(&status);
			}
		}
	} else {
		ZVAL_NULL(&event);
	}

	if (dao_property_array_isset_fetch(&callback, getThis(), SL("_eventCallbacks"), &name, PH_READONLY)) {
		zval arguments = {};
		array_init_size(&arguments, 3);
		dao_array_append(&arguments, &event, PH_COPY);
		dao_array_append(&arguments, data, PH_COPY);
		dao_array_append(&arguments, &status, 0);

		DAO_MM_ADD_ENTRY(&arguments);
		DAO_MM_CALL_USER_FUNC_ARRAY(&status, &callback, &arguments);
		DAO_MM_ADD_ENTRY(&status);

		if (Z_TYPE(event) == IS_OBJECT) {
			DAO_MM_CALL_METHOD(&is_stopped, &event, "isstopped");
			if (zend_is_true(&is_stopped)) {
				RETURN_MM_NCTOR(&status);
			}
		}
	}

	/**
	 * Check if there is a method with the same name of the event
	 */
	if (dao_method_exists(getThis(), &name) == SUCCESS) {
		zval prev_data = {};
		DAO_MM_ZVAL_COPY(&prev_data, &status);
		DAO_MM_CALL_METHOD(&status, getThis(), Z_STRVAL(name), &event, data, &prev_data);
		DAO_MM_ADD_ENTRY(&status);
	}

	RETURN_MM_CTOR(&status);
}

/**
 * Fires an event, can stop the event by returning to the false
 *
 * @param string $eventName
 * @param mixed $data
 * @return mixed
 */
PHP_METHOD(Dao_Di_Injectable, fireEventCancel){

	zval *eventname, *data = NULL, *cancelable = NULL, eventtype = {}, callback = {}, events_manager = {}, lower = {}, name = {};
	zval event = {}, is_stopped = {}, status = {}, debug_message = {};

	dao_fetch_params(1, 1, 2, &eventname, &data, &cancelable);

	if (unlikely(DAO_GLOBAL(debug).enable_debug)) {
		DAO_CONCAT_SV(&debug_message, "--Event(Cancel): ", eventname);
		DAO_DEBUG_LOG(&debug_message);
		zval_ptr_dtor(&debug_message);
	}

	if (!data) {
		data = &DAO_GLOBAL(z_null);
	}

	if (!cancelable) {
		cancelable = &DAO_GLOBAL(z_true);
	}

	dao_fast_strtolower(&lower, eventname);
	DAO_MM_ADD_ENTRY(&lower);
	if (dao_memnstr_str(&lower, SL(":"))) {
		zval event_parts = {};
		dao_fast_explode_str(&event_parts, SL(":"), &lower);
		dao_array_fetch_long(&name, &event_parts, 1, PH_COPY);
		DAO_MM_ADD_ENTRY(&name);
		zval_ptr_dtor(&event_parts);
		DAO_MM_ZVAL_COPY(&eventtype, eventname);
	} else {
		zval class_name = {};
		DAO_MM_ZVAL_COPY(&name, &lower);

		dao_get_called_class(&class_name);
		DAO_CONCAT_VSV(&eventtype, &class_name, ":", eventname);
		DAO_MM_ADD_ENTRY(&eventtype);
		zval_ptr_dtor(&class_name);
	}

	DAO_MM_CALL_METHOD(&events_manager, getThis(), "geteventsmanager");
	DAO_MM_ADD_ENTRY(&events_manager);
	if (Z_TYPE(events_manager) != IS_NULL) {
		DAO_MM_VERIFY_INTERFACE_EX(&events_manager, dao_events_managerinterface_ce, dao_di_exception_ce);

		DAO_MM_CALL_METHOD(&status, &events_manager, "fire", &eventtype, getThis(), data, cancelable, &DAO_GLOBAL(z_true));
		if (DAO_IS_FALSE(&status)){
			RETURN_MM_FALSE;
		}
		DAO_MM_ADD_ENTRY(&status);

		DAO_MM_CALL_METHOD(&event, &events_manager, "getcurrentevent");
		DAO_MM_ADD_ENTRY(&event);
		if (Z_TYPE(event) == IS_OBJECT) {
			DAO_MM_CALL_METHOD(&is_stopped, &event, "isstopped");
			if (zend_is_true(&is_stopped)) {
				RETURN_MM_CTOR(&status);
			}
		}
	} else {
		ZVAL_NULL(&event);
	}

	if (dao_property_array_isset_fetch(&callback, getThis(), SL("_eventCallbacks"), &name, PH_READONLY)) {
		zval arguments = {};
		array_init_size(&arguments, 2);
		dao_array_append(&arguments, &event, PH_COPY);
		dao_array_append(&arguments, data, PH_COPY);
		dao_array_append(&arguments, &status, PH_COPY);

		DAO_MM_ADD_ENTRY(&arguments);
		DAO_MM_CALL_USER_FUNC_ARRAY(&status, &callback, &arguments);

		if (DAO_IS_FALSE(&status)){
			RETURN_MM_FALSE;
		}
		DAO_MM_ADD_ENTRY(&status);
		if (Z_TYPE(event) == IS_OBJECT) {
			DAO_MM_CALL_METHOD(&is_stopped, &event, "isstopped");
			if (zend_is_true(&is_stopped)) {
				RETURN_MM_CTOR(&status);
			}
		}
	}

	/**
	 * Check if there is a method with the same name of the event
	 */
	if (dao_method_exists(getThis(), &name) == SUCCESS) {
		zval prev_data = {};
		if (Z_TYPE(status) > IS_NULL) {
			DAO_MM_ZVAL_COPY(&prev_data, &status);
		} else {
			ZVAL_NULL(&prev_data);
		}
		DAO_MM_CALL_METHOD(&status, getThis(), Z_STRVAL(name), &event, data, &prev_data);

		if (DAO_IS_FALSE(&status)){
			RETURN_MM_FALSE;
		}
		DAO_MM_ADD_ENTRY(&status);
	}

	RETURN_MM_CTOR(&status);
}

/**
 * Check whether the DI contains a service by a name
 *
 * @param string $name
 * @return boolean
 */
PHP_METHOD(Dao_Di_Injectable, hasService){

	zval *service_name, dependency_injector = {};

	dao_fetch_params(1, 1, 0, &service_name);

	DAO_MM_CALL_METHOD(&dependency_injector, getThis(), "getdi");
	DAO_MM_ADD_ENTRY(&dependency_injector);
	if (Z_TYPE(dependency_injector) == IS_OBJECT) {
		DAO_MM_CALL_METHOD(return_value, &dependency_injector, "has", service_name);
	} else {
		RETVAL_FALSE;
	}
	RETURN_MM();
}

/**
 * Sets a service from the DI
 *
 * @param string $serviceName
 * @param mixed $definition
 * @param boolean $shared
 * @return Dao\Di\ServiceInterface
 */
PHP_METHOD(Dao_Di_Injectable, setService){

	zval *service_name, *definition, *shared = NULL, dependency_injector = {};

	dao_fetch_params(1, 2, 1, &service_name, &definition, &shared);

	if (!shared) {
		shared = &DAO_GLOBAL(z_false);
	}

	DAO_MM_CALL_METHOD(&dependency_injector, getThis(), "getdi", &DAO_GLOBAL(z_true));
	DAO_MM_ADD_ENTRY(&dependency_injector);
	DAO_MM_CALL_METHOD(return_value, &dependency_injector, "set", service_name, definition, shared);
	RETURN_MM();
}

/**
 * Obtains a service from the DI
 *
 * @param string $serviceName
 * @return object|null
 */
PHP_METHOD(Dao_Di_Injectable, getService){

	zval *service_name, dependency_injector = {};

	dao_fetch_params(1, 1, 0, &service_name);

	DAO_MM_CALL_METHOD(&dependency_injector, getThis(), "getdi", &DAO_GLOBAL(z_true));
	DAO_MM_ADD_ENTRY(&dependency_injector);
	DAO_MM_CALL_METHOD(return_value, &dependency_injector, "get", service_name, &DAO_GLOBAL(z_null), &DAO_GLOBAL(z_true));
	RETURN_MM();
}

/**
 * Resolves the service based on its configuration
 *
 * @param string $name
 * @param array $parameters
 * @param boolean $noError
 * @param boolean $noShared
 * @return mixed
 */
PHP_METHOD(Dao_Di_Injectable, getResolveService){

	zval *name, *args = NULL, *noerror = NULL, *noshared = NULL, dependency_injector = {};

	dao_fetch_params(1, 1, 3, &name, &args, &noerror, &noshared);

	if (!args) {
		args = &DAO_GLOBAL(z_null);
	}

	if (!noerror) {
		noerror = &DAO_GLOBAL(z_false);
	}

	if (!noshared) {
		noshared = &DAO_GLOBAL(z_false);
	}

	ZVAL_NULL(return_value);

	DAO_CALL_METHOD(&dependency_injector, getThis(), "getdi", noerror);
	DAO_MM_ADD_ENTRY(&dependency_injector);
	if (Z_TYPE(dependency_injector) == IS_OBJECT) {
		if (zend_is_true(noshared)) {
			DAO_MM_CALL_METHOD(return_value, &dependency_injector, "get", name, args, noerror);
		} else {
			DAO_MM_CALL_METHOD(return_value, &dependency_injector, "getshared", name, args, noerror);
		}
	}
	RETURN_MM();
}

/**
 * Attach a listener to the events
 *
 * @param string $eventType
 * @param Closure $callback
 */
PHP_METHOD(Dao_Di_Injectable, attachEvent){

	zval *event_type, *callback, prefixed = {}, new_callback = {};

	dao_fetch_params(1, 2, 0, &event_type, &callback);

	dao_fast_strtolower(&prefixed, event_type);
	DAO_MM_ADD_ENTRY(&prefixed);

	DAO_MM_CALL_CE_STATIC(&new_callback, zend_ce_closure, "bind", callback, getThis());
	DAO_MM_ADD_ENTRY(&new_callback);
	dao_update_property_array(getThis(), SL("_eventCallbacks"), &prefixed, &new_callback);

	RETURN_MM_THIS();
}

/**
 * Magic method __get
 *
 * @param string $propertyName
 */
PHP_METHOD(Dao_Di_Injectable, __get){

	zval *property_name, dependency_injector = {}, has_service = {}, service = {}, class_name = {}, arguments = {};

	dao_fetch_params(1, 1, 0, &property_name);
	DAO_ENSURE_IS_STRING(property_name);

	DAO_MM_CALL_METHOD(&dependency_injector, getThis(), "getdi");
	DAO_MM_ADD_ENTRY(&dependency_injector);
	DAO_MM_CALL_METHOD(&has_service, &dependency_injector, "has", property_name);

	if (zend_is_true(&has_service)) {
		DAO_MM_CALL_METHOD(return_value, &dependency_injector, "getshared", property_name);
		RETURN_MM();
	}

	if (Z_STRLEN_P(property_name) == sizeof("di")-1 && !memcmp(Z_STRVAL_P(property_name), "di", sizeof("di")-1)) {
		dao_update_property(getThis(), SL("di"), &dependency_injector);
		RETURN_MM_CTOR(&dependency_injector);
	}

	/**
	 * Accessing the persistent property will create a session bag in any class
	 */
	if (Z_STRLEN_P(property_name) == sizeof("persistent")-1 && !memcmp(Z_STRVAL_P(property_name), "persistent", sizeof("persistent")-1)) {
		ZVAL_STRING(&class_name, Z_OBJCE_P(getThis())->name->val);

		array_init_size(&arguments, 1);
		add_next_index_zval(&arguments, &class_name);
		DAO_MM_ADD_ENTRY(&arguments);

		ZVAL_STR(&service, IS(sessionBag));
		DAO_MM_CALL_METHOD(return_value, &dependency_injector, "get", &service, &arguments);
		dao_update_property(getThis(), SL("persistent"), return_value);
		RETURN_MM();
	}

	/**
	 * A notice is shown if the property is not defined or is not a valid service
	 */
	php_error_docref(NULL, E_WARNING, "Access to undefined property %s::%s", Z_OBJCE_P(getThis())->name->val, Z_STRVAL_P(property_name));
	RETURN_MM_NULL();
}

PHP_METHOD(Dao_Di_Injectable, __sleep){

	zval dependency_injector = {}, dependency_name = {};

	dao_read_property(&dependency_injector, getThis(), SL("_dependencyInjector"), PH_READONLY);
	if (Z_TYPE(dependency_injector) == IS_OBJECT) {
		DAO_CALL_METHOD(&dependency_name, &dependency_injector, "getname");
	} else {
		ZVAL_COPY(&dependency_name, &dependency_injector);
	}

	dao_update_property(getThis(), SL("_dependencyInjector"), &dependency_name);
	zval_ptr_dtor(&dependency_name);

	dao_get_object_members(return_value, getThis(), 0);
}

PHP_METHOD(Dao_Di_Injectable, __debugInfo){

	dao_get_object_vars(return_value, getThis(), 1);

	if (likely(!DAO_GLOBAL(debug).enable_debug)) {
		dao_array_unset_str(return_value, SL("_dependencyInjector"), 0);
		dao_array_unset_str(return_value, SL("_eventsManager"), 0);
	}
}
