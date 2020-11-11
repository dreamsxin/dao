
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
  |          Nikolaos Dimopoulos <nikos@niden.net>                         |
  |          ZhuZongXin <dreamsxin@qq.com>                                 |
  +------------------------------------------------------------------------+
*/

#include "di.h"
#include "diinterface.h"
#include "di/exception.h"
#include "di/injectionawareinterface.h"
#include "di/service.h"
#include "di/serviceinterface.h"
#include "di/factorydefault.h"
#include "events/managerinterface.h"
#include "async/core.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/exception.h"
#include "kernel/fcall.h"
#include "kernel/array.h"
#include "kernel/concat.h"
#include "kernel/file.h"
#include "kernel/string.h"
#include "kernel/hash.h"
#include "kernel/operators.h"

#include "interned-strings.h"

/**
 * Dao\Di
 *
 * Dao\Di is a component that implements Dependency Injection/Service Location
 * of services and it's itself a container for them.
 *
 * Since Dao is highly decoupled, Dao\Di is essential to integrate the different
 * components of the framework. The developer can also use this component to inject dependencies
 * and manage global instances of the different classes used in the application.
 *
 * Basically, this component implements the `Inversion of Control` pattern. Applying this,
 * the objects do not receive their dependencies using setters or constructors, but requesting
 * a service dependency injector. This reduces the overall complexity, since there is only one
 * way to get the required dependencies within a component.
 *
 * Additionally, this pattern increases testability in the code, thus making it less prone to errors.
 *
 *<code>
 * $di = new Dao\Di();
 *
 * //Using a string definition
 * $di->set('request', 'Dao\Http\Request', true);
 *
 * //Using an anonymous function
 * $di->set('request', function(){
 *	  return new Dao\Http\Request();
 * }, true);
 *
 * $request = $di->getRequest();
 *
 *</code>
 */
zend_class_entry *dao_di_ce;

PHP_METHOD(Dao_Di, __construct);
PHP_METHOD(Dao_Di, getName);
PHP_METHOD(Dao_Di, setEventsManager);
PHP_METHOD(Dao_Di, getEventsManager);
PHP_METHOD(Dao_Di, set);
PHP_METHOD(Dao_Di, setShared);
PHP_METHOD(Dao_Di, remove);
PHP_METHOD(Dao_Di, attempt);
PHP_METHOD(Dao_Di, setRaw);
PHP_METHOD(Dao_Di, getRaw);
PHP_METHOD(Dao_Di, setService);
PHP_METHOD(Dao_Di, getService);
PHP_METHOD(Dao_Di, get);
PHP_METHOD(Dao_Di, getShared);
PHP_METHOD(Dao_Di, has);
PHP_METHOD(Dao_Di, wasFreshInstance);
PHP_METHOD(Dao_Di, getServices);
PHP_METHOD(Dao_Di, __call);
PHP_METHOD(Dao_Di, setDefault);
PHP_METHOD(Dao_Di, getDefault);
PHP_METHOD(Dao_Di, reset);
PHP_METHOD(Dao_Di, __clone);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_di___construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_di___call, 0, 0, 1)
	ZEND_ARG_INFO(0, method)
	ZEND_ARG_INFO(0, arguments)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_di_setshared, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_INFO(0, definition)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_di_attempt, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_INFO(0, definition)
	ZEND_ARG_INFO(0, shared)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_di_setraw, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_INFO(0, definition)
	ZEND_ARG_INFO(0, shared)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_di_getraw, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_di_method_entry[] = {
	PHP_ME(Dao_Di, __construct, arginfo_dao_di___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Di, getName, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Di, setEventsManager, arginfo_empty, ZEND_ACC_PROTECTED)
	PHP_ME(Dao_Di, getEventsManager, arginfo_empty, ZEND_ACC_PROTECTED)
	/* Dao\DiInterface*/
	PHP_ME(Dao_Di, set, arginfo_dao_diinterface_set, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Di, remove, arginfo_dao_diinterface_remove, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Di, setRaw, arginfo_dao_di_setraw, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Di, getRaw, arginfo_dao_di_getraw, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Di, getService, arginfo_dao_diinterface_getservice, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Di, setService, arginfo_dao_diinterface_setservice, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Di, get, arginfo_dao_diinterface_get, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Di, getShared, arginfo_dao_diinterface_getshared, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Di, has, arginfo_dao_diinterface_has, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Di, wasFreshInstance, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Di, getServices, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Di, setDefault, arginfo_dao_diinterface_setdefault, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Di, getDefault, arginfo_empty, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Di, reset, arginfo_empty, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)

	/* Convenience methods */
	PHP_ME(Dao_Di, attempt, arginfo_dao_di_attempt, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Di, setShared, arginfo_dao_di_setshared, ZEND_ACC_PUBLIC)

	/* Syntactic sugar */
	PHP_MALIAS(Dao_Di, offsetExists, has, arginfo_arrayaccess_offsetexists, ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Di, offsetSet, setShared, arginfo_arrayaccess_offsetset, ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Di, offsetGet, getShared, arginfo_arrayaccess_offsetget, ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Di, offsetUnset, remove, arginfo_arrayaccess_offsetunset, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Di, __call, arginfo_dao_di___call, ZEND_ACC_PUBLIC)

	/* Misc */
	PHP_ME(Dao_Di, __clone, arginfo_empty, ZEND_ACC_PUBLIC)

	PHP_MALIAS(Dao_Di, __set, set, arginfo___set, ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Di, __get, get, arginfo___get, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Di initializer
 */
DAO_INIT_CLASS(Dao_Di){

	DAO_REGISTER_CLASS(Dao, Di, di, dao_di_method_entry, 0);

	zend_declare_property_null(dao_di_ce, SL("_name"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_di_ce, SL("_services"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_di_ce, SL("_sharedInstances"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_di_ce, SL("_freshInstance"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_di_ce, SL("_default"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);
	zend_declare_property_null(dao_di_ce, SL("_list"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);
	zend_declare_property_null(dao_di_ce, SL("_eventsManager"), ZEND_ACC_PROTECTED);

	zend_class_implements(dao_di_ce, 2, zend_ce_arrayaccess, dao_diinterface_ce);

	return SUCCESS;
}

/**
 * Dao\Di constructor
 *
 */
PHP_METHOD(Dao_Di, __construct){

	zval *_name = NULL, name = {};

	dao_fetch_params(0, 0, 1, &_name);

	DAO_CALL_METHOD(NULL, getThis(), "setdefault", getThis(), &DAO_GLOBAL(z_true));

	if (!_name || Z_TYPE_P(_name) != IS_STRING) {
		ZVAL_STR(&name, IS(di));
	} else {
		ZVAL_COPY_VALUE(&name, _name);
	}

	dao_update_property(getThis(), SL("_name"), &name);

	dao_update_static_property_array_ce(dao_di_ce, SL("_list"), &name, getThis());
}

/**
 * Returns the name
 *
 * @return String
 */
PHP_METHOD(Dao_Di, getName){

	RETURN_MEMBER(getThis(), "_name");
}


/**
 * Sets a custom events manager
 *
 * @param Dao\Events\ManagerInterface $eventsManager
 */
PHP_METHOD(Dao_Di, setEventsManager){

	zval *events_manager;

	dao_fetch_params(0, 1, 0, &events_manager);
	DAO_VERIFY_INTERFACE_EX(events_manager, dao_events_managerinterface_ce, dao_di_exception_ce);

	dao_update_property(getThis(), SL("_eventsManager"), events_manager);
}

/**
 * Returns the custom events manager
 *
 * @return Dao\Events\ManagerInterface
 */
PHP_METHOD(Dao_Di, getEventsManager){

	RETURN_MEMBER(getThis(), "_eventsManager");
}

/**
 * Registers a service in the services container
 *
 * @param string $name
 * @param mixed $definition
 * @param boolean $shared
 * @return Dao\Di\ServiceInterface
 */
PHP_METHOD(Dao_Di, set) {

	zval *name, *definition, *shared = NULL;

	dao_fetch_params(0, 2, 1, &name, &definition, &shared);

	if (!shared) {
		shared = &DAO_GLOBAL(z_false);
	}

	DAO_CALL_METHOD(return_value, getThis(), "setraw", name, definition, shared);
}

/**
 * Registers an "always shared" service in the services container
 *
 * @param string $name
 * @param mixed $definition
 * @return Dao\Di\ServiceInterface
 */
PHP_METHOD(Dao_Di, setShared){

	zval *name, *definition;

	dao_fetch_params(0, 2, 0, &name, &definition);

	DAO_CALL_METHOD(return_value, getThis(), "setraw", name, definition, &DAO_GLOBAL(z_true));
}

/**
 * Removes a service in the services container
 *
 * @param string $name
 */
PHP_METHOD(Dao_Di, remove){

	zval *name;

	dao_fetch_params(0, 1, 0, &name);
	DAO_ENSURE_IS_STRING(name);

	dao_unset_property_array(getThis(), SL("_services"), name);
}

/**
 * Attempts to register a service in the services container
 * Only is successful if a service hasn't been registered previously
 * with the same name
 *
 * @param string $name
 * @param mixed $definition
 * @param boolean $shared
 * @return Dao\Di\ServiceInterface
 */
PHP_METHOD(Dao_Di, attempt){

	zval *name, *definition, *shared = NULL;

	dao_fetch_params(0, 2, 1, &name, &definition, &shared);
	DAO_ENSURE_IS_STRING(name);

	if (!dao_property_array_isset_fetch(return_value, getThis(), SL("_services"), name, PH_COPY)) {
		if (!shared) {
			shared = &DAO_GLOBAL(z_false);
		}

		DAO_CALL_METHOD(return_value, getThis(), "setraw", name, definition, shared);
	}
}

/**
 * Returns a service definition without resolving
 *
 * @param string $name
 * @param mixed $definition
 * @param boolean $shared
 * @return Dao\Di\ServiceInterface
 */
PHP_METHOD(Dao_Di, setRaw)
{
	zval *name, *raw_definition, *shared = NULL;

	dao_fetch_params(0, 2, 1, &name, &raw_definition, &shared);

	if (!shared) {
		shared = &DAO_GLOBAL(z_false);
	}

	object_init_ex(return_value, dao_di_service_ce);
	DAO_CALL_METHOD(NULL, return_value, "__construct", name, raw_definition, shared);

	dao_update_property_array(getThis(), SL("_services"), name, return_value);

	if (zend_is_true(shared)) {
		dao_unset_property_array(getThis(), SL("_sharedInstances"), name);
	}
}

/**
 * Returns a service definition without resolving
 *
 * @param string $name
 * @return mixed
 */
PHP_METHOD(Dao_Di, getRaw){

	zval *name, service = {};

	dao_fetch_params(0, 1, 0, &name);
	DAO_ENSURE_IS_STRING(name);

	if (dao_isset_property_array(getThis(), SL("_services"), name)) {
		dao_read_property_array(&service, getThis(), SL("_services"), name, PH_READONLY);
		DAO_RETURN_CALL_METHOD(&service, "getdefinition");
		return;
	}

	zend_throw_exception_ex(dao_di_exception_ce, 0, "Service '%s' was not found in the dependency injection container", Z_STRVAL_P(name));
}

/**
 * Sets a service using a raw Dao\Di\Service definition
 *
 * @param string $name
 * @param Dao\Di\ServiceInterface $service
 * @return Dao\Di\ServiceInterface
 */
PHP_METHOD(Dao_Di, setService)
{
	zval *name, *service;

	dao_fetch_params(0, 2, 0, &name, &service);

	dao_update_property_array(getThis(), SL("_services"), name, service);

	RETURN_CTOR(service);
}

/**
 * Returns a Dao\Di\Service instance
 *
 * @param string $name
 * @return Dao\Di\ServiceInterface
 */
PHP_METHOD(Dao_Di, getService){

	zval *name;

	dao_fetch_params(0, 1, 0, &name);
	DAO_ENSURE_IS_STRING(name);

	if (dao_isset_property_array(getThis(), SL("_services"), name)) {
		dao_read_property_array(return_value, getThis(), SL("_services"), name, PH_COPY);
		return;
	}

	zend_throw_exception_ex(dao_di_exception_ce, 0, "Service '%s' was not found in the dependency injection container", Z_STRVAL_P(name));
}

/**
 * Resolves the service based on its configuration
 *
 * @param string $name
 * @param array $parameters
 * @param boolean $noError
 * @return mixed
 */
PHP_METHOD(Dao_Di, get){

	zval *_name, *parameters = NULL, *noerror = NULL, events_manager = {}, event_name = {}, event_data = {}, name = {}, service = {};
	zend_class_entry *ce;

	dao_fetch_params(1, 1, 2, &_name, &parameters, &noerror);

	if (!parameters) {
		parameters = &DAO_GLOBAL(z_null);
	}

	if (!noerror) {
		noerror = &DAO_GLOBAL(z_false);
	}

	dao_read_property(&events_manager, getThis(), SL("_eventsManager"), PH_READONLY);
	if (Z_TYPE(events_manager) == IS_OBJECT) {
		DAO_MM_ZVAL_STRING(&event_name, "di:beforeServiceResolve");

		array_init(&event_data);
		dao_array_update_str(&event_data, SL("name"), _name, PH_COPY);
		dao_array_update_str(&event_data, SL("parameters"), parameters, PH_COPY);

		DAO_MM_ADD_ENTRY(&event_data);

		DAO_MM_CALL_METHOD(&name, &events_manager, "fire", &event_name, getThis(), &event_data);
		DAO_MM_ADD_ENTRY(&name);
	}

	if (Z_TYPE(name) != IS_STRING || !Z_STRLEN(name)) {
		ZVAL_COPY_VALUE(&name, _name);
	}

	if (dao_property_array_isset_fetch(&service, getThis(), SL("_services"), &name, PH_READONLY)) {
		DAO_MM_CALL_METHOD(return_value, &service, "resolve", parameters, getThis());
		ce = (Z_TYPE_P(return_value) == IS_OBJECT) ? Z_OBJCE_P(return_value) : NULL;
	} else {
		/* The DI also acts as builder for any class even if it isn't defined in the DI */
		if ((ce = dao_class_exists_ex(&name, 1)) != NULL) {
			if (FAILURE == dao_create_instance_params_ce(return_value, ce, parameters)) {
				return;
			}
		} else {
			if(!zend_is_true(noerror)) {
				zend_throw_exception_ex(dao_di_exception_ce, 0, "Service '%s' was not found in the dependency injection container", Z_STRVAL_P(_name));
			}
			RETURN_MM_NULL();
		}
	}

	/* Pass the DI itself if the instance implements Dao\Di\InjectionAwareInterface */
	if (ce && instanceof_function_ex(ce, dao_di_injectionawareinterface_ce, 1)) {
		DAO_MM_CALL_METHOD(NULL, return_value, "setdi", getThis());
	} else if (dao_method_exists_ex(return_value, SL("setdi")) == SUCCESS) {
		DAO_MM_CALL_METHOD(NULL, return_value, "setdi", getThis());
	}

	if (Z_TYPE(events_manager) == IS_OBJECT) {
		DAO_MM_ZVAL_STRING(&event_name, "di:afterServiceResolve");

		array_init(&event_data);
		dao_array_update_str(&event_data, SL("name"), _name, PH_COPY);
		dao_array_update_str(&event_data, SL("parameters"), parameters, PH_COPY);
		dao_array_update_str(&event_data, SL("instance"), return_value, PH_COPY);

		DAO_MM_ADD_ENTRY(&event_data);
		DAO_MM_CALL_METHOD(NULL, &events_manager, "fire", &event_name, getThis(), &event_data);
	}
	RETURN_MM();
}

/**
 * Resolves a service, the resolved service is stored in the DI, subsequent requests for this service will return the same instance
 *
 * @param string $name
 * @param array $parameters
 * @param boolean $noError
 * @return mixed
 */
PHP_METHOD(Dao_Di, getShared){

	zval *name, *parameters = NULL, *noerror = NULL;

	dao_fetch_params(0, 1, 2, &name, &parameters, &noerror);

	if (!parameters) {
		parameters = &DAO_GLOBAL(z_null);
	}

	if (!noerror) {
		noerror = &DAO_GLOBAL(z_null);
	}

	if (dao_property_array_isset_fetch(return_value, getThis(), SL("_sharedInstances"), name, PH_COPY)) {
		if (Z_TYPE_P(return_value) == IS_OBJECT && instanceof_function_ex(Z_OBJCE_P(return_value), dao_di_injectionawareinterface_ce, 1)) {
			DAO_CALL_METHOD(NULL, return_value, "setdi", getThis());
		}
		dao_update_property_bool(getThis(), SL("_freshInstance"), 0);
	} else {
		DAO_CALL_SELF(return_value, "get", name, parameters, noerror);
		if (zend_is_true(return_value)) {
			dao_update_property_bool(getThis(), SL("_freshInstance"), 1);
			dao_update_property_array(getThis(), SL("_sharedInstances"), name, return_value);
		}
	}
}

/**
 * Check whether the DI contains a service by a name
 *
 * @param string $name
 * @return boolean
 */
PHP_METHOD(Dao_Di, has){

	zval *name;

	dao_fetch_params(0, 1, 0, &name);

	if (dao_isset_property_array(getThis(), SL("_services"), name)) {
		RETURN_TRUE;
	}

	RETURN_FALSE;
}

/**
 * Check whether the last service obtained via getShared produced a fresh instance or an existing one
 *
 * @return boolean
 */
PHP_METHOD(Dao_Di, wasFreshInstance){


	RETURN_MEMBER(getThis(), "_freshInstance");
}

/**
 * Return the services registered in the DI
 *
 * @return Dao\Di\Service[]
 */
PHP_METHOD(Dao_Di, getServices){

	RETURN_MEMBER(getThis(), "_services");
}

/**
 * Check if a service is registered using the array syntax.
 * Alias for Dao\Di::has()
 *
 * @param string $name
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Di, offsetExists);

/**
 * Allows to register a shared service using the array syntax.
 * Alias for Dao\Di::setShared()
 *
 *<code>
 *	$di['request'] = new Dao\Http\Request();
 *</code>
 *
 * @param string $name
 * @param mixed $definition
 */
DAO_DOC_METHOD(Dao_Di, offsetSet);

/**
 * Allows to obtain a shared service using the array syntax.
 * Alias for Dao\Di::getShared()
 *
 *<code>
 *	var_dump($di['request']);
 *</code>
 *
 * @param string $name
 * @return mixed
 */
DAO_DOC_METHOD(Dao_Di, offsetGet);

/**
 * Removes a service from the services container using the array syntax.
 * Alias for Dao\Di::remove()
 *
 * @param string $name
 */
DAO_DOC_METHOD(Dao_Di, offsetUnset);

/**
 * Magic method to get or set services using setters/getters
 *
 * @param string $method
 * @param array $arguments
 * @return mixed
 */
PHP_METHOD(Dao_Di, __call){

	zval *method, *arguments = NULL, possible_service = {}, name = {}, definition = {};

	dao_fetch_params(0, 1, 1, &method, &arguments);
	DAO_ENSURE_IS_STRING(method);

	if (!arguments) {
		arguments = &DAO_GLOBAL(z_null);
	}

	dao_substr(&possible_service, method, 3, 0);
	dao_lcfirst(&name, &possible_service);
	zval_ptr_dtor(&possible_service);

	/**
	 * If the magic method starts with "get" we try to get a service with that name
	 */
	if (dao_start_with_str(method, SL("get"))) {
		if (dao_isset_property_array(getThis(), SL("_services"), &name)) {
			DAO_RETURN_CALL_SELF("get", &name, arguments);
			zval_ptr_dtor(&name);
			return;
		}
	}

	/**
	 * If the magic method starts with "set" we try to set a service using that name
	 */
	if (dao_start_with_str(method, SL("set"))) {
		if (dao_array_isset_fetch_long(&definition, arguments, 0, PH_READONLY)) {
			DAO_CALL_SELF(NULL, "set", &name, &definition);
			zval_ptr_dtor(&name);
			return;
		}
	}

	/**
	 * The method doesn't start with set/get throw an exception
	 */
	zend_throw_exception_ex(dao_di_exception_ce, 0, "Call to undefined method or service '%s'", Z_STRVAL_P(method));
}

/**
 * Set a default dependency injection container to be obtained into static methods
 *
 * @param Dao\DiInterface $dependencyInjector
 */
PHP_METHOD(Dao_Di, setDefault){

	zval *dependency_injector, *flag = NULL;
	zval tmp = {};
#if DAO_USE_ASYNC
	async_task *task;
#endif

	dao_fetch_params(0, 1, 1, &dependency_injector, &flag);
	DAO_VERIFY_INTERFACE_EX(dependency_injector, dao_diinterface_ce, dao_di_exception_ce);

	if (!flag) {
		flag = &DAO_GLOBAL(z_false);
	}
#if DAO_USE_ASYNC
	task = ASYNC_G(task);
	if (task && Z_TYPE(task->di) != IS_OBJECT) {
		ZVAL_COPY(&task->di, dependency_injector);
	}
#endif
	dao_read_static_property_ce(&tmp, dao_di_ce, SL("_default"), PH_NOISY | PH_READONLY);
	if (!zend_is_true(flag) || Z_TYPE(tmp) != IS_OBJECT) {
		dao_update_static_property_ce(dao_di_ce, SL("_default"), dependency_injector);
	}
}

/**
 * Return the lastest DI created
 *
 * @return Dao\DiInterface
 */
PHP_METHOD(Dao_Di, getDefault){

	zval *name = NULL;

	dao_fetch_params(1, 0, 1, &name);

	if (name && DAO_IS_NOT_EMPTY(name)) {
		dao_read_static_property_array_ce(return_value, dao_di_ce, SL("_list"), name, PH_COPY);
	} else {
#if DAO_USE_ASYNC
		async_task *task;

		task = ASYNC_G(task);
		if (task && Z_TYPE(task->di) == IS_OBJECT) {
			RETURN_MM_CTOR(&task->di);
		}
#endif
		dao_read_static_property_ce(return_value, dao_di_ce, SL("_default"), PH_COPY);
		if (Z_TYPE_P(return_value) != IS_OBJECT) {
			object_init_ex(return_value, dao_di_factorydefault_ce);
			DAO_MM_CALL_METHOD(NULL, return_value, "__construct");
		}
	}
	RETURN_MM();
}

/**
 * Resets the internal default DI
 */
PHP_METHOD(Dao_Di, reset){

	zend_update_static_property_null(dao_di_ce, SL("_default"));
}

PHP_METHOD(Dao_Di, __clone) {

	dao_update_property_null(getThis(), SL("_sharedInstances"));
}
