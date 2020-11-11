
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

#include "di/service.h"
#include "di/serviceinterface.h"
#include "di/service/builder.h"
#include "di/exception.h"
#include "events/managerinterface.h"

#include <Zend/zend_closures.h>

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/concat.h"
#include "kernel/exception.h"
#include "kernel/array.h"
#include "kernel/operators.h"

#include "interned-strings.h"

/**
 * Dao\Di\Service
 *
 * Represents individually a service in the services container
 *
 *<code>
 * $service = new Dao\Di\Service('request', 'Dao\Http\Request');
 * $request = $service->resolve();
 *<code>
 *
 */
zend_class_entry *dao_di_service_ce;

PHP_METHOD(Dao_Di_Service, __construct);
PHP_METHOD(Dao_Di_Service, getName);
PHP_METHOD(Dao_Di_Service, setShared);
PHP_METHOD(Dao_Di_Service, isShared);
PHP_METHOD(Dao_Di_Service, setSharedInstance);
PHP_METHOD(Dao_Di_Service, setDefinition);
PHP_METHOD(Dao_Di_Service, getDefinition);
PHP_METHOD(Dao_Di_Service, resolve);
PHP_METHOD(Dao_Di_Service, setParameter);
PHP_METHOD(Dao_Di_Service, getParameter);
PHP_METHOD(Dao_Di_Service, isResolved);
PHP_METHOD(Dao_Di_Service, __set_state);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_di_service___construct, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_INFO(0, definition)
	ZEND_ARG_INFO(0, shared)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_di_service_method_entry[] = {
	PHP_ME(Dao_Di_Service, __construct, arginfo_dao_di_service___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Di_Service, getName, arginfo_dao_di_serviceinterface_getname, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Di_Service, setShared, arginfo_dao_di_serviceinterface_setshared, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Di_Service, isShared, arginfo_dao_di_serviceinterface_isshared, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Di_Service, setSharedInstance, arginfo_dao_di_service_setsharedinstance, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Di_Service, setDefinition, arginfo_dao_di_serviceinterface_setdefinition, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Di_Service, getDefinition, arginfo_dao_di_serviceinterface_getdefinition, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Di_Service, resolve, arginfo_dao_di_serviceinterface_resolve, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Di_Service, setParameter, arginfo_dao_di_service_setparameter, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Di_Service, getParameter, arginfo_dao_di_service_getparameter, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Di_Service, isResolved, arginfo_dao_di_serviceinterface_isresolved, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Di_Service, __set_state, arginfo___set_state, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};

/**
 * Dao\Di\Service initializer
 */
DAO_INIT_CLASS(Dao_Di_Service){

	DAO_REGISTER_CLASS(Dao\\Di, Service, di_service, dao_di_service_method_entry, 0);

	zend_declare_property_null(dao_di_service_ce, SL("_name"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_di_service_ce, SL("_definition"), ZEND_ACC_PROTECTED);
	zend_declare_property_bool(dao_di_service_ce, SL("_shared"), 0, ZEND_ACC_PROTECTED);
	zend_declare_property_bool(dao_di_service_ce, SL("_resolved"), 0, ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_di_service_ce, SL("_sharedInstance"), ZEND_ACC_PROTECTED);

	zend_class_implements(dao_di_service_ce, 1, dao_di_serviceinterface_ce);

	return SUCCESS;
}

/**
 * Dao\Di\Service
 *
 * @param string $name
 * @param mixed $definition
 * @param boolean $shared
 */
PHP_METHOD(Dao_Di_Service, __construct){

	zval *name, *definition, *shared = NULL;

	dao_fetch_params(0, 2, 1, &name, &definition, &shared);

	dao_update_property(getThis(), SL("_name"), name);
	dao_update_property(getThis(), SL("_definition"), definition);
	if (shared) {
		DAO_ENSURE_IS_BOOL(shared);
		dao_update_property(getThis(), SL("_shared"), shared);
	}
}

/**
 * Returns the service's name
 *
 * @param string
 */
PHP_METHOD(Dao_Di_Service, getName){

	RETURN_MEMBER(getThis(), "_name");
}

/**
 * Sets if the service is shared or not
 *
 * @param boolean $shared
 */
PHP_METHOD(Dao_Di_Service, setShared){

	zval *shared;
	dao_fetch_params(0, 1, 0, &shared);
	dao_update_property(getThis(), SL("_shared"), shared);
}

/**
 * Check whether the service is shared or not
 *
 * @return boolean
 */
PHP_METHOD(Dao_Di_Service, isShared){

	RETURN_MEMBER(getThis(), "_shared");
}

/**
 * Sets/Resets the shared instance related to the service
 *
 * @param mixed $sharedInstance
 */
PHP_METHOD(Dao_Di_Service, setSharedInstance){
	zval *shared_instance;
	dao_fetch_params(0, 1, 0, &shared_instance);
	dao_update_property(getThis(), SL("_sharedInstance"), shared_instance);
}

/**
 * Set the service definition
 *
 * @param mixed $definition
 */
PHP_METHOD(Dao_Di_Service, setDefinition)
{
	zval *definition;
	dao_fetch_params(0, 1, 0, &definition);
	dao_update_property(getThis(), SL("_definition"), definition);
}

/**
 * Returns the service definition
 *
 * @return mixed
 */
PHP_METHOD(Dao_Di_Service, getDefinition)
{
	RETURN_MEMBER(getThis(), "_definition");
}

/**
 * Resolves the service
 *
 * @param array $parameters
 * @param Dao\DiInterface $dependencyInjector
 * @return object
 */
PHP_METHOD(Dao_Di_Service, resolve){

	zval *parameters = NULL, *dependency_injector = NULL, name = {}, shared = {}, shared_instance = {}, definition = {}, builder = {};
	int found = 0, ishared = 0;

	dao_fetch_params(0, 0, 2, &parameters, &dependency_injector);

	if (!parameters) {
		parameters = &DAO_GLOBAL(z_null);
	}

	if (!dependency_injector) {
		dependency_injector = &DAO_GLOBAL(z_null);
	}

	dao_read_property(&shared, getThis(), SL("_shared"), PH_READONLY);
	dao_read_property(&shared_instance, getThis(), SL("_sharedInstance"), PH_READONLY);

	ishared = zend_is_true(&shared);

	/* Check if the service is shared */
	if (ishared && Z_TYPE(shared_instance) != IS_NULL) {
		RETURN_CTOR(&shared_instance);
	}

	dao_read_property(&definition, getThis(), SL("_definition"), PH_READONLY);

	if (Z_TYPE(definition) == IS_STRING) {
		/* String definitions can be class names without implicit parameters */
		if (dao_class_exists(&definition, 1) != NULL) {
			found = 1;
			if (Z_TYPE_P(parameters) == IS_ARRAY) {
				RETURN_ON_FAILURE(dao_create_instance_params(return_value, &definition, parameters));
			} else {
				RETURN_ON_FAILURE(dao_create_instance(return_value, &definition));
			}
		}
	} else if (likely(Z_TYPE(definition) == IS_OBJECT)) {
		/* Object definitions can be a Closure or an already resolved instance */
		found = 1;
		if (instanceof_function_ex(Z_OBJCE(definition), zend_ce_closure, 0)) {
			zval closure = {};
			if (likely(Z_TYPE_P(dependency_injector) == IS_OBJECT)) {
				DAO_CALL_CE_STATIC(&closure, zend_ce_closure, "bind", &definition, dependency_injector);
			} else {
				ZVAL_COPY(&closure, &definition);
			}
			if (Z_TYPE_P(parameters) == IS_ARRAY) {
				DAO_CALL_USER_FUNC_ARRAY(return_value, &closure, parameters);
			} else {
				DAO_CALL_USER_FUNC(return_value, &closure);
			}
			zval_ptr_dtor(&closure);
		} else {
			ZVAL_COPY(return_value, &definition);
		}
	} else if (Z_TYPE(definition) == IS_ARRAY) {
		found = 1;
		/* Array definitions require a 'className' parameter */
		object_init_ex(&builder, dao_di_service_builder_ce);

		DAO_CALL_METHOD(return_value, &builder, "build", dependency_injector, &definition, parameters);
	}

	if (!EG(exception)) {
		if (found) {
			if (ishared) {
				dao_update_property(getThis(), SL("_sharedInstance"), return_value);
			}
			/* Update the shared instance if the service is shared */
			dao_update_property_bool(getThis(), SL("_resolved"), 1);
		} else {
			dao_read_property(&name, getThis(), SL("_name"), PH_READONLY);
			DAO_THROW_EXCEPTION_FORMAT(dao_di_exception_ce, "Service '%s' cannot be resolved", Z_STRVAL(name));
		}
	}
}

/**
 * Changes a parameter in the definition without resolve the service
 *
 * @param long $position
 * @param array $parameter
 * @return Dao\Di\Service
 */
PHP_METHOD(Dao_Di_Service, setParameter){

	zval *position, *parameter, definition = {}, arguments = {};

	dao_fetch_params(0, 2, 0, &position, &parameter);
	DAO_ENSURE_IS_LONG(position);

	dao_read_property(&definition, getThis(), SL("_definition"), PH_READONLY);

	if (unlikely(Z_TYPE(definition) != IS_ARRAY)) {
		DAO_THROW_EXCEPTION_STR(dao_di_exception_ce, "Definition must be an array to update its parameters");
		return;
	}

	if (unlikely(Z_TYPE_P(parameter) != IS_ARRAY)) {
		DAO_THROW_EXCEPTION_STR(dao_di_exception_ce, "The parameter must be an array");
		return;
	}

	/* Update the parameter */
	if (dao_array_isset_fetch_str(&arguments, &definition, SL("arguments"), PH_READONLY)) {
		dao_array_update(&arguments, position, parameter, PH_COPY);
	} else {
		array_init_size(&arguments, 1);
		dao_array_update(&arguments, position, parameter, PH_COPY);
	}

	dao_array_update_str(&definition, SL("arguments"), &arguments, PH_COPY);

	dao_update_property(getThis(), SL("_definition"), &definition);

	RETURN_THIS();
}

/**
 * Returns a parameter in a specific position
 *
 * @param int $position
 * @return array
 */
PHP_METHOD(Dao_Di_Service, getParameter){

	zval *position, definition = {}, arguments = {};

	dao_fetch_params(0, 1, 0, &position);
	DAO_ENSURE_IS_LONG(position);

	dao_read_property(&definition, getThis(), SL("_definition"), PH_READONLY);
	if (Z_TYPE(definition) != IS_ARRAY) {
		DAO_THROW_EXCEPTION_STR(dao_di_exception_ce, "Definition must be an array to obtain its parameters");
		return;
	}

	/* Update the parameter */
	if (
		!dao_array_isset_fetch_str(&arguments, &definition, SL("arguments"), PH_READONLY) ||
		!dao_array_isset_fetch(return_value, &arguments, position, PH_COPY)
	) {
		RETURN_NULL();
	}
}

/**
 * Returns true if the service was resolved
 *
 * @return boolean
 */
PHP_METHOD(Dao_Di_Service, isResolved)
{
	RETURN_MEMBER(getThis(), "_resolved");
}

/**
 * Restore the internal state of a service
 *
 * @param array $attributes
 * @return Dao\Di\Service
 */
PHP_METHOD(Dao_Di_Service, __set_state){

	zval *attributes, name = {}, definition = {}, shared = {};

	dao_fetch_params(0, 1, 0, &attributes);

	if (
			!dao_array_isset_fetch_str(&name, attributes, SL("_name"), PH_READONLY)
		 || !dao_array_isset_fetch_str(&definition, attributes, SL("_definition"), PH_READONLY)
		 || !dao_array_isset_fetch_str(&shared, attributes, SL("_shared"), PH_READONLY)
	) {
		DAO_THROW_EXCEPTION_STR(spl_ce_BadMethodCallException, "Bad parameters passed to Dao\\Di\\Service::__set_state()");
		return;
	}

	object_init_ex(return_value, dao_di_service_ce);
	DAO_CALL_METHOD(NULL, return_value, "__construct", &name, &definition, &shared);
}
