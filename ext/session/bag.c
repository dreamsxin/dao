
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

#include "session/bag.h"
#include "session/baginterface.h"
#include "session/exception.h"
#include "session/adapterinterface.h"
#include "di.h"
#include "diinterface.h"
#include "di/injectable.h"

#include <ext/spl/spl_array.h>

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/exception.h"
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "kernel/array.h"

#include "interned-strings.h"

/**
 * Dao\Session\Bag
 *
 * This component helps to separate session data into "namespaces". Working by this way
 * you can easily create groups of session variables into the application
 *
 *<code>
 *	$user = new \Dao\Session\Bag('user');
 *	$user->name = "Kimbra Johnson";
 *	$user->age = 22;
 *</code>
 */
zend_class_entry *dao_session_bag_ce;

PHP_METHOD(Dao_Session_Bag, __construct);
PHP_METHOD(Dao_Session_Bag, initialize);
PHP_METHOD(Dao_Session_Bag, destroy);
PHP_METHOD(Dao_Session_Bag, set);
PHP_METHOD(Dao_Session_Bag, get);
PHP_METHOD(Dao_Session_Bag, has);
PHP_METHOD(Dao_Session_Bag, __get);
PHP_METHOD(Dao_Session_Bag, remove);
PHP_METHOD(Dao_Session_Bag, getIterator);
PHP_METHOD(Dao_Session_Bag, count);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_session_bag___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_session_bag_method_entry[] = {
	PHP_ME(Dao_Session_Bag, __construct, arginfo_dao_session_bag___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Session_Bag, initialize, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Session_Bag, destroy, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Session_Bag, set, arginfo_dao_session_baginterface_set, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Session_Bag, get, arginfo_dao_session_baginterface_get, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Session_Bag, has, arginfo_dao_session_baginterface_has, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Session_Bag, remove, arginfo_dao_session_baginterface_remove, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Session_Bag, getIterator, arginfo_iteratoraggregate_getiterator, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Session_Bag, __get, arginfo___get, ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Session_Bag, __set, set, arginfo___set, ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Session_Bag, __isset, has, arginfo___isset, ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Session_Bag, __unset, remove, arginfo___unset, ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Session_Bag, offsetGet, __get, arginfo_arrayaccess_offsetget, ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Session_Bag, offsetSet, set, arginfo_arrayaccess_offsetset, ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Session_Bag, offsetExists, has, arginfo_arrayaccess_offsetexists, ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Session_Bag, offsetUnset, remove, arginfo_arrayaccess_offsetunset, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Session_Bag, count, arginfo_countable_count, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

static int dao_session_bag_maybe_initialize(zval *this_ptr)
{
	zval initialized = {};

	dao_read_property(&initialized, this_ptr, SL("_initialized"), PH_NOISY|PH_READONLY);
	if (DAO_IS_FALSE(&initialized)) {
		return dao_call_method(NULL, this_ptr, "initialize", 0, NULL);
	}

	return SUCCESS;
}

static zend_object_iterator* dao_session_bag_get_iterator(zend_class_entry *ce, zval *object, int by_ref)
{
	zval iterator = {}, data = {}, *params[1];
	zend_object_iterator *ret;

	if (FAILURE == dao_session_bag_maybe_initialize(object)) {
		return NULL;
	}

	dao_read_property(&data, object, SL("_data"), PH_NOISY|PH_READONLY);

	object_init_ex(&iterator, spl_ce_ArrayIterator);
	params[0] = &data;
	if (FAILURE == dao_call_method(NULL, &iterator, "__construct", 1, params)) {
		ret = NULL;
	}
	else if (Z_TYPE(iterator) == IS_OBJECT) {
		ret = spl_ce_ArrayIterator->get_iterator(spl_ce_ArrayIterator, &iterator, by_ref);
	} else {
		ret = NULL;
	}
	zval_ptr_dtor(&iterator);

	return ret;
}

/**
 * Dao\Session\Bag initializer
 */
DAO_INIT_CLASS(Dao_Session_Bag){

	DAO_REGISTER_CLASS_EX(Dao\\Session, Bag, session_bag, dao_di_injectable_ce, dao_session_bag_method_entry, 0);

	zend_declare_property_null(dao_session_bag_ce, SL("_name"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_session_bag_ce, SL("_data"), ZEND_ACC_PROTECTED);
	zend_declare_property_bool(dao_session_bag_ce, SL("_initialized"), 0, ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_session_bag_ce, SL("_session"), ZEND_ACC_PROTECTED);

	dao_session_bag_ce->get_iterator = dao_session_bag_get_iterator;

	zend_class_implements(dao_session_bag_ce, 4, dao_session_baginterface_ce, zend_ce_aggregate, zend_ce_arrayaccess, spl_ce_Countable);

	return SUCCESS;
}

/**
 * Dao\Session\Bag constructor
 *
 * @param string $name
 */
PHP_METHOD(Dao_Session_Bag, __construct){

	zval *name;

	dao_fetch_params(0, 1, 0, &name);
	DAO_ENSURE_IS_STRING(name);
	dao_update_property(getThis(), SL("_name"), name);
}

/**
 * Initializes the session bag. This method must not be called directly, the class calls it when its internal data is accesed
 */
PHP_METHOD(Dao_Session_Bag, initialize){

	zval session = {}, dependency_injector = {}, service = {}, name = {}, data = {};

	DAO_MM_INIT();

	dao_read_property(&session, getThis(), SL("_session"), PH_READONLY);
	if (Z_TYPE(session) != IS_OBJECT) {
		DAO_MM_CALL_METHOD(&dependency_injector, getThis(), "getdi");
		DAO_MM_ADD_ENTRY(&dependency_injector);
		if (Z_TYPE(dependency_injector) != IS_OBJECT) {
			DAO_MM_THROW_EXCEPTION_STR(dao_session_exception_ce, "A dependency injection object is required to access the 'session' service");
			return;
		}

		ZVAL_STR(&service, IS(session));

		DAO_MM_CALL_METHOD(&session, &dependency_injector, "getshared", &service);
		DAO_MM_ADD_ENTRY(&session);
		DAO_MM_VERIFY_INTERFACE(&session, dao_session_adapterinterface_ce);
		dao_update_property(getThis(), SL("_session"), &session);
	}

	dao_read_property(&name, getThis(), SL("_name"), PH_READONLY);

	DAO_MM_CALL_METHOD(&data, &session, "__get", &name);
	DAO_MM_ADD_ENTRY(&data);

	if (Z_TYPE(data) != IS_ARRAY) {
		dao_update_property_empty_array(getThis(), SL("_data"));
	} else {
		dao_update_property(getThis(), SL("_data"), &data);
	}

	dao_update_property_bool(getThis(), SL("_initialized"), 1);
	RETURN_MM();
}

/**
 * Destroys the session bag
 *
 *<code>
 * $user->destroy();
 *</code>
 */
PHP_METHOD(Dao_Session_Bag, destroy){

	zval name = {}, session = {};

	RETURN_ON_FAILURE(dao_session_bag_maybe_initialize(getThis()));

	dao_read_property(&name, getThis(), SL("_name"), PH_NOISY|PH_READONLY);
	dao_read_property(&session, getThis(), SL("_session"), PH_NOISY|PH_READONLY);

	DAO_CALL_METHOD(NULL, &session, "__unset", &name);
}

/**
 * Sets a value in the session bag
 *
 *<code>
 * $user->set('name', 'Kimbra');
 *</code>
 *
 * @param string $property
 * @param string $value
 */
PHP_METHOD(Dao_Session_Bag, set){

	zval *property, *value, session = {}, name = {}, data = {};

	dao_fetch_params(0, 2, 0, &property, &value);

	RETURN_ON_FAILURE(dao_session_bag_maybe_initialize(getThis()));

	dao_update_property_array(getThis(), SL("_data"), property, value);

	dao_read_property(&data, getThis(), SL("_data"), PH_NOISY|PH_READONLY);
	dao_read_property(&name, getThis(), SL("_name"), PH_NOISY|PH_READONLY);
	dao_read_property(&session, getThis(), SL("_session"), PH_NOISY|PH_READONLY);

	DAO_CALL_METHOD(NULL, &session, "__set", &name, &data);
}

/**
 * Magic setter to assign values to the session bag.
 * Alias for Dao\Session\Bag::set()
 *
 *<code>
 * $user->name = "Kimbra";
 *</code>
 *
 * @param string $property
 * @param string $value
 */
DAO_DOC_METHOD(Dao_Session_Bag, __set);

/**
 * Obtains a value from the session bag optionally setting a default value
 *
 *<code>
 * echo $user->get('name', 'Kimbra');
 *</code>
 *
 * @param string $property
 * @param string $defaultValue
 * @return mixed
 */
PHP_METHOD(Dao_Session_Bag, get){

	zval *property, *default_value = NULL, data = {}, value = {};

	dao_fetch_params(0, 1, 1, &property, &default_value);

	if (!default_value) {
		default_value = &DAO_GLOBAL(z_null);
	}

	/* Check first if the bag is initialized */
	RETURN_ON_FAILURE(dao_session_bag_maybe_initialize(getThis()));

	/* Retrieve the data */
	dao_read_property(&data, getThis(), SL("_data"), PH_NOISY|PH_READONLY);
	if (dao_array_isset_fetch(&value, &data, property, PH_READONLY)) {
		RETURN_CTOR(&value);
	}

	RETURN_CTOR(default_value);
}

/**
 * Magic getter to obtain values from the session bag.
 *
 *<code>
 * echo $user->name;
 *</code>
 *
 * @param string $property
 * @return string
 */
PHP_METHOD(Dao_Session_Bag, __get)
{
	zval *property, data = {}, name = {}, session = {};

	dao_fetch_params(0, 1, 0, &property);

	/* Check first if the bag is initialized */
	RETURN_ON_FAILURE(dao_session_bag_maybe_initialize(getThis()));

	/* Retrieve the data */
	dao_read_property(&data, getThis(), SL("_data"), PH_NOISY|PH_READONLY);

	if (!dao_array_isset_fetch(return_value, &data, property, 0)) {
		ZVAL_NULL(return_value);
		ZVAL_MAKE_REF(return_value);

		dao_update_property_array(getThis(), SL("_data"), property, return_value);

		dao_read_property(&data, getThis(), SL("_data"), PH_NOISY|PH_READONLY);
		dao_read_property(&name, getThis(), SL("_name"), PH_NOISY|PH_READONLY);
		dao_read_property(&session, getThis(), SL("_session"), PH_NOISY|PH_READONLY);

		DAO_CALL_METHOD(NULL, &session, "__set", &name, &data);
	} else {
		ZVAL_MAKE_REF(return_value);
	}

}


/**
 * Check whether a property is defined in the internal bag
 *
 *<code>
 * var_dump($user->has('name'));
 *</code>
 *
 * @param string $property
 * @return boolean
 */
PHP_METHOD(Dao_Session_Bag, has){

	zval *property, data = {};

	dao_fetch_params(0, 1, 0, &property);

	RETURN_ON_FAILURE(dao_session_bag_maybe_initialize(getThis()));

	dao_read_property(&data, getThis(), SL("_data"), PH_NOISY|PH_READONLY);
	RETURN_BOOL(dao_array_isset(&data, property));
}

/**
 * Magic isset to check whether a property is defined in the bag.
 * Alias for Dao\Session\Bag::has()
 *
 *<code>
 * var_dump(isset($user['name']));
 *</code>
 *
 * @param string $property
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Session_Bag, __isset);

/**
 * Removes a property from the internal bag
 *
 *<code>
 * $user->remove('name');
 *</code>
 *
 * @param string $property
 * @return boolean
 */
PHP_METHOD(Dao_Session_Bag, remove){

	zval *property, data = {}, name = {}, session = {};

	dao_fetch_params(0, 1, 0, &property);

	RETURN_ON_FAILURE(dao_session_bag_maybe_initialize(getThis()));

	dao_read_property(&data, getThis(), SL("_data"), PH_NOISY|PH_READONLY);
	if (dao_array_isset(&data, property)) {
		dao_unset_property_array(getThis(), SL("_data"), property);

		dao_read_property(&data, getThis(), SL("_data"), PH_NOISY|PH_READONLY);
		dao_read_property(&name, getThis(), SL("_name"), PH_NOISY|PH_READONLY);
		dao_read_property(&session, getThis(), SL("_session"), PH_NOISY|PH_READONLY);

		DAO_CALL_METHOD(NULL, &session, "__set", &name, &data);

		RETURN_TRUE;
	}

	RETURN_FALSE;
}

/**
 * Magic unset to remove items using the property syntax.
 * Alias for Dao\Session\Bag::remove()
 *
 *<code>
 * unset($user['name']);
 *</code>
 *
 * @param string $property
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Session_Bag, __unset);

PHP_METHOD(Dao_Session_Bag, getIterator)
{
	zval data = {};

	RETURN_ON_FAILURE(dao_session_bag_maybe_initialize(getThis()));

	dao_read_property(&data, getThis(), SL("_data"), PH_NOISY|PH_READONLY);
	object_init_ex(return_value, spl_ce_ArrayIterator);
	DAO_CALL_METHOD(NULL, return_value, "__construct", &data);
}

PHP_METHOD(Dao_Session_Bag, count)
{
	zval data = {};
	long int count;

	RETURN_ON_FAILURE(dao_session_bag_maybe_initialize(getThis()));

	dao_read_property(&data, getThis(), SL("_data"), PH_NOISY|PH_READONLY);
	count = (Z_TYPE(data) == IS_ARRAY) ? zend_hash_num_elements(Z_ARRVAL(data)) : 0;
	RETURN_LONG(count);
}
