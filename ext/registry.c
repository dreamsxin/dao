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
  |          Vladimir Kolesnikov <vladimir@extrememember.com>              |
  |          ZhuZongXin <dreamsxin@qq.com>                                 |
  +------------------------------------------------------------------------+
*/

#include "registry.h"

#include <Zend/zend_interfaces.h>
#include <Zend/zend_smart_str.h>
#include <ext/spl/spl_array.h>
#include <ext/standard/php_var.h>

#include "kernel/main.h"
#include "kernel/hash.h"
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"

/**
 * Dao\Registry
 *
 * A registry is a container for storing objects and values in the application space.
 * By storing the value in a registry, the same object is always available throughout
 * your application.
 *
 * <code>
 * 	$registry = new \Dao\Registry();
 *
 * 	// Set value
 * 	$registry->something = 'something';
 * 	// or
 * 	$registry['something'] = 'something';
 *
 * 	// Get value
 * 	$value = $registry->something;
 * 	// or
 * 	$value = $registry['something'];
 *
 * 	// Check if the key exists
 * 	$exists = isset($registry->something);
 * 	// or
 * 	$exists = isset($registry['something']);
 *
 * 	// Unset
 * 	unset($registry->something);
 * 	// or
 * 	unset($registry['something']);
 * </code>
 *
 * In addition to ArrayAccess, Dao\Registry also implements Countable
 * (count($registry) will return the number of elements in the registry),
 * Serializable and Iterator (you can iterate over the registry
 * using a foreach loop) interfaces. For PHP 5.4 and higher, JsonSerializable
 * interface is implemented.
 *
 * Dao\Registry is very fast (it is typically faster than any userspace
 * implementation of the registry); however, this comes at a price:
 * Dao\Registry is a final class and cannot be inherited from.
 *
 * Though Dao\Registry exposes methods like __get(), offsetGet(), count() etc,
 * it is not recommended to invoke them manually (these methods exist mainly to
 * match the interfaces the registry implements): $registry->__get('property')
 * is several times slower than $registry->property.
 *
 * Internally all the magic methods (and interfaces except JsonSerializable)
 * are implemented using object handlers or similar techniques: this allows
 * to bypass relatively slow method calls.
 */
zend_class_entry *dao_registry_ce;

PHP_METHOD(Dao_Registry, __construct);
PHP_METHOD(Dao_Registry, __get);
PHP_METHOD(Dao_Registry, __set);
PHP_METHOD(Dao_Registry, __isset);
PHP_METHOD(Dao_Registry, __unset);
PHP_METHOD(Dao_Registry, __call);
PHP_METHOD(Dao_Registry, count);
PHP_METHOD(Dao_Registry, offsetGet);
PHP_METHOD(Dao_Registry, offsetSet);
PHP_METHOD(Dao_Registry, offsetUnset);
PHP_METHOD(Dao_Registry, offsetExists);
PHP_METHOD(Dao_Registry, current);
PHP_METHOD(Dao_Registry, key);
PHP_METHOD(Dao_Registry, next);
PHP_METHOD(Dao_Registry, rewind);
PHP_METHOD(Dao_Registry, valid);
PHP_METHOD(Dao_Registry, jsonSerialize);
PHP_METHOD(Dao_Registry, serialize);
PHP_METHOD(Dao_Registry, unserialize);
PHP_METHOD(Dao_Registry, __wakeup);

const zend_function_entry dao_registry_method_entry[] = {
	PHP_ME(Dao_Registry, __construct, arginfo_empty, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Registry, __get, arginfo___getref, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Registry, __set, arginfo___set, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Registry, __isset, arginfo___isset, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Registry, __unset, arginfo___unset, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Registry, __call, arginfo___call, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Registry, count, arginfo_countable_count, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Registry, offsetGet, arginfo_arrayaccess_offsetgetref, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Registry, offsetSet, arginfo_arrayaccess_offsetset, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Registry, offsetUnset, arginfo_arrayaccess_offsetunset, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Registry, offsetExists, arginfo_arrayaccess_offsetexists, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Registry, current, arginfo_iterator_current, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Registry, key, arginfo_iterator_key, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Registry, next, arginfo_iterator_next, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Registry, rewind, arginfo_iterator_rewind, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Registry, valid, arginfo_iterator_valid, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Registry, serialize, arginfo_serializable_serialize, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(Dao_Registry, unserialize, arginfo_serializable_unserialize, ZEND_ACC_PUBLIC | ZEND_ACC_DTOR)
	PHP_ME(Dao_Registry, __wakeup, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

DAO_INIT_CLASS(Dao_Registry){
	DAO_REGISTER_CLASS(Dao, Registry, registry, dao_registry_method_entry, 0);

	zend_declare_property_null(dao_registry_ce, SL("_data"), ZEND_ACC_PROTECTED);

	zend_class_implements(dao_registry_ce, 4, zend_ce_arrayaccess, zend_ce_iterator, zend_ce_serializable, spl_ce_Countable);

	return SUCCESS;
}

/**
 * Dao\Di constructor
 *
 */
PHP_METHOD(Dao_Registry, __construct){

	dao_update_property_empty_array(getThis(), SL("_data"));
}

/**
 * Returns an index in the registry
 */
PHP_METHOD(Dao_Registry, __get){
	zval *property;

	dao_fetch_params(0, 1, 0, &property);

	DAO_RETURN_CALL_SELF("offsetget", property);
}

/**
 * Sets an element in the registry
 */
PHP_METHOD(Dao_Registry, __set){
	zval *property, *value;

	dao_fetch_params(0, 2, 0, &property, &value);

	DAO_CALL_SELF(NULL, "offsetset", property, value);
}

PHP_METHOD(Dao_Registry, __isset){
	zval *key;

	dao_fetch_params(0, 1, 0, &key);

	DAO_RETURN_CALL_SELF("offsetexists", key);
}

PHP_METHOD(Dao_Registry, __unset){
	zval *property;

	dao_fetch_params(0, 1, 0, &property);

	DAO_CALL_SELF(NULL, "offsetunset", property);
}

/**
 * @brief void Dao\Registry::__call(string $name, array $arguments)
 */
PHP_METHOD(Dao_Registry, __call){

	zval *name, *arguments, callback = {};

	dao_fetch_params(0, 2, 0, &name, &arguments);
	DAO_ENSURE_IS_STRING(name);

	if (dao_isset_property_array(getThis(), SL("_data"), name)) {
		dao_read_property_array(&callback, getThis(), SL("_data"), name, PH_READONLY);
		DAO_CALL_ZVAL_FUNCTION(return_value, &callback, arguments);
	} else {
		zend_throw_exception_ex(spl_ce_BadMethodCallException, 0, "Call to undefined method Dao\\Registry::%s", Z_STRVAL_P(name));
	}
}

/**
 * @brief int Dao\Registry::count()
 */
PHP_METHOD(Dao_Registry, count){

	zval data = {};

	dao_read_property(&data, getThis(), SL("_data"), PH_NOISY|PH_READONLY);

	dao_fast_count(return_value, &data);
}

/**
 * @brief mixed& Dao\Registry::offsetGet(mixed $offset)
 */
PHP_METHOD(Dao_Registry, offsetGet){

	zval *property;

	dao_fetch_params(0, 1, 0, &property);

	dao_read_property_array(return_value, getThis(), SL("_data"), property, PH_COPY);
}

/**
 * @brief void Dao\Registry::offsetSet(mixed $offset, mixed $value)
 */
PHP_METHOD(Dao_Registry, offsetSet){

	zval *property, *callback;

	dao_fetch_params(0, 2, 0, &property, &callback);

	dao_update_property_array(getThis(), SL("_data"), property, callback);
}

/**
 * @brief void Dao\Registry::offsetUnset(mixed $offset)
 */
PHP_METHOD(Dao_Registry, offsetUnset){

	zval *property;

	dao_fetch_params(0, 1, 0, &property);

	dao_unset_property_array(getThis(), SL("_data"), property);
}

/**
 * @brief void Dao\Registry::offsetExists(mixed $offset)
 */
PHP_METHOD(Dao_Registry, offsetExists){

	zval *property;

	dao_fetch_params(0, 1, 0, &property);

	if (dao_isset_property_array(getThis(), SL("_data"), property)) {
		RETURN_TRUE;
	}

	RETURN_FALSE;
}

/**
 * @brief mixed& Dao\Registry::current()
 */
PHP_METHOD(Dao_Registry, current){

	zval data = {}, *callback;

	dao_read_property(&data, getThis(), SL("_data"), PH_NOISY|PH_READONLY);
	if ((callback = zend_hash_get_current_data(Z_ARRVAL(data))) != NULL) {
		RETURN_ZVAL(callback, 1, 0);
	}

	RETURN_NULL();
}

/**
 * @brief string|int|null Dao\Registry::key()
 */
PHP_METHOD(Dao_Registry, key){

	zval data = {};

	dao_read_property(&data, getThis(), SL("_data"), PH_NOISY|PH_READONLY);
	zend_hash_get_current_key_zval(Z_ARRVAL(data), return_value);
}

/**
 * @brief void Dao\Registry::next()
 */
PHP_METHOD(Dao_Registry, next){

	zval data = {};

	dao_read_property(&data, getThis(), SL("_data"), PH_NOISY|PH_READONLY);
	zend_hash_move_forward(Z_ARRVAL(data));
}

/**
 * @brief void Dao\Registry::rewind()
 */
PHP_METHOD(Dao_Registry, rewind){

	zval data = {};

	dao_read_property(&data, getThis(), SL("_data"), PH_NOISY|PH_READONLY);
	zend_hash_internal_pointer_reset(Z_ARRVAL(data));
}

/**
 * @brief bool Dao\Registry::valid()
 */
PHP_METHOD(Dao_Registry, valid){

	zval data = {};

	dao_read_property(&data, getThis(), SL("_data"), PH_NOISY|PH_READONLY);

	RETURN_BOOL(zend_hash_has_more_elements(Z_ARRVAL(data)));
}

/**
 * @brief string|null Dao\Registry::serialize()
 */
PHP_METHOD(Dao_Registry, serialize){

	zval data = {};

	dao_read_property(&data, getThis(), SL("_data"), PH_NOISY|PH_READONLY);

	smart_str buf = { 0 };
	php_serialize_data_t var_hash;

	PHP_VAR_SERIALIZE_INIT(var_hash);
	php_var_serialize(&buf, &data, &var_hash);
	PHP_VAR_SERIALIZE_DESTROY(var_hash);

	if (buf.s) {
		RETURN_NEW_STR(buf.s);
	}

	RETURN_NULL();
}

/**
 * @brief Dao\Registry Dao\Registry::unserialize(string $str)
 */
PHP_METHOD(Dao_Registry, unserialize){
	zval data = {}, *str, zv = {};
	php_unserialize_data_t var_hash;
	const unsigned char *buf, *max;

	dao_fetch_params(0, 1, 0, &str);
	DAO_ENSURE_IS_STRING(str);

	dao_read_property(&data, getThis(), SL("_data"), PH_NOISY|PH_READONLY);

	if (zend_hash_num_elements(Z_ARRVAL(data))) {
		zend_throw_exception_ex(spl_ce_BadMethodCallException, 0, "Cannot call unserialize() on an already constructed object");
		return;
	}

	buf = (unsigned char*)(Z_STRVAL_P(str));
	max = buf + Z_STRLEN_P(str);

	PHP_VAR_UNSERIALIZE_INIT(var_hash);
	if (php_var_unserialize(&zv, &buf, max, &var_hash) && Z_TYPE(zv) == IS_ARRAY) {
		if (zend_hash_num_elements(Z_ARRVAL(zv)) != 0) {
			zend_hash_copy(Z_ARRVAL(data), Z_ARRVAL(zv), (copy_ctor_func_t) zval_add_ref);
		}
	} else {
		zend_throw_exception_ex(spl_ce_BadMethodCallException, 0, "Bad parameters passed to Dao\\Registry::unserialize()");
	}

	zval_dtor(&zv);
	PHP_VAR_UNSERIALIZE_DESTROY(var_hash);
}

/**
 * @brief void Dao\Registry::__wakeup()
 */
PHP_METHOD(Dao_Registry, __wakeup)
{
}
