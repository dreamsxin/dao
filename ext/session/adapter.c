
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

#include "session/adapter.h"
#include "session/adapterinterface.h"
#include "di/injectable.h"

#include <main/SAPI.h>
#include <ext/spl/spl_array.h>
#ifdef DAO_USE_PHP_SESSION
#include <ext/session/php_session.h>
#endif

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "kernel/object.h"
#include "kernel/exception.h"
#include "kernel/array.h"
#include "kernel/concat.h"
#include "kernel/session.h"
#include "kernel/hash.h"

/**
 * Dao\Session\Adapter
 *
 * Base class for Dao\Session adapters
 */
zend_class_entry *dao_session_adapter_ce;

PHP_METHOD(Dao_Session_Adapter, __construct);
PHP_METHOD(Dao_Session_Adapter, __destruct);
PHP_METHOD(Dao_Session_Adapter, start);
PHP_METHOD(Dao_Session_Adapter, setOptions);
PHP_METHOD(Dao_Session_Adapter, getOptions);
PHP_METHOD(Dao_Session_Adapter, get);
PHP_METHOD(Dao_Session_Adapter, set);
PHP_METHOD(Dao_Session_Adapter, sets);
PHP_METHOD(Dao_Session_Adapter, has);
PHP_METHOD(Dao_Session_Adapter, remove);
PHP_METHOD(Dao_Session_Adapter, getId);
PHP_METHOD(Dao_Session_Adapter, isStarted);
PHP_METHOD(Dao_Session_Adapter, regenerate);
PHP_METHOD(Dao_Session_Adapter, destroy);
PHP_METHOD(Dao_Session_Adapter, __get);
PHP_METHOD(Dao_Session_Adapter, count);
PHP_METHOD(Dao_Session_Adapter, getIterator);
PHP_METHOD(Dao_Session_Adapter, setId);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_session_adapter___construct, 0, 0, 0)
	ZEND_ARG_INFO(0, options)
	ZEND_ARG_INFO(0, expire)
	ZEND_ARG_INFO(0, path)
	ZEND_ARG_INFO(0, secure)
	ZEND_ARG_INFO(0, domain)
	ZEND_ARG_INFO(0, httpOnly)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_session_adapter_setid, 0, 0, 1)
	ZEND_ARG_INFO(0, sid)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_session_adapter_regenerate, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, delete_old_session, _IS_BOOL, 1)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_session_adapter_method_entry[] = {
	PHP_ME(Dao_Session_Adapter, __construct, arginfo_dao_session_adapter___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Session_Adapter, __destruct, arginfo___destruct, ZEND_ACC_PUBLIC | ZEND_ACC_DTOR)
	PHP_ME(Dao_Session_Adapter, start, arginfo_dao_session_adapterinterface_start, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Session_Adapter, setOptions, arginfo_dao_session_adapterinterface_setoptions, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Session_Adapter, getOptions, arginfo_dao_session_adapterinterface_getoptions, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Session_Adapter, get, arginfo_dao_session_adapterinterface_get, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Session_Adapter, set, arginfo_dao_session_adapterinterface_set, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Session_Adapter, sets, arginfo_dao_session_adapterinterface_sets, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Session_Adapter, has, arginfo_dao_session_adapterinterface_has, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Session_Adapter, remove, arginfo_dao_session_adapterinterface_remove, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Session_Adapter, getId, arginfo_dao_session_adapterinterface_getid, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Session_Adapter, isStarted, arginfo_dao_session_adapterinterface_isstarted, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Session_Adapter, regenerate, arginfo_dao_session_adapter_regenerate, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Session_Adapter, destroy, arginfo_dao_session_adapterinterface_destroy, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Session_Adapter, __get, arginfo___get, ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Session_Adapter, __set, set, arginfo___set, ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Session_Adapter, __isset, has, arginfo___isset, ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Session_Adapter, __unset, remove, arginfo___unset, ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Session_Adapter, offsetGet, __get, arginfo_arrayaccess_offsetget, ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Session_Adapter, offsetSet, set, arginfo_arrayaccess_offsetset, ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Session_Adapter, offsetExists, has, arginfo_arrayaccess_offsetexists, ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Session_Adapter, offsetUnset, remove, arginfo_arrayaccess_offsetunset, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Session_Adapter, count, arginfo_countable_count, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Session_Adapter, getIterator, arginfo_iteratoraggregate_getiterator, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Session_Adapter, setId, arginfo_dao_session_adapter_setid, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Session\Adapter initializer
 */
DAO_INIT_CLASS(Dao_Session_Adapter){

	DAO_REGISTER_CLASS_EX(Dao\\Session, Adapter, session_adapter, dao_di_injectable_ce, dao_session_adapter_method_entry, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	zend_declare_property_null(dao_session_adapter_ce, SL("_uniqueId"), ZEND_ACC_PROTECTED);
	zend_declare_property_bool(dao_session_adapter_ce, SL("_started"), 0, ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_session_adapter_ce, SL("_options"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_session_adapter_ce, SL("_expire"), ZEND_ACC_PROTECTED);
	zend_declare_property_string(dao_session_adapter_ce, SL("_path"), "/", ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_session_adapter_ce, SL("_secure"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_session_adapter_ce, SL("_domain"), ZEND_ACC_PROTECTED);
	zend_declare_property_bool(dao_session_adapter_ce, SL("_httpOnly"), 0, ZEND_ACC_PROTECTED);

	zend_class_implements(
		dao_session_adapter_ce, 4,
		dao_session_adapterinterface_ce,
		spl_ce_Countable,
		zend_ce_aggregate,
		zend_ce_arrayaccess
	);
	return SUCCESS;
}

/**
 * Dao\Session\Adapter constructor
 *
 * @param array $options
 * @param int $expire
 * @param string $path
 * @param boolean $secure
 * @param string $domain
 * @param boolean $httpOnly
 */
PHP_METHOD(Dao_Session_Adapter, __construct){

	zval *options = NULL, *_expire = NULL, *_path = NULL, *_secure = NULL, *_domain = NULL, *_http_only = NULL;
	zval expire = {}, path = {}, secure = {}, domain = {}, http_only = {};

	dao_fetch_params(0, 0, 6, &options, &_expire, &_path, &_secure, &_domain, &_http_only);

	if (options && Z_TYPE_P(options) == IS_ARRAY) {
		DAO_CALL_METHOD(NULL, getThis(), "setoptions", options);
	}

	if (!_expire) {
		dao_read_property(&expire, getThis(), SL("_expire"), PH_NOISY|PH_READONLY);
	} else {
		ZVAL_COPY_VALUE(&expire, _expire);
	}

	if (!_path) {
		dao_read_property(&path, getThis(), SL("_path"), PH_NOISY|PH_READONLY);
	} else {
		ZVAL_COPY_VALUE(&path, _path);
	}

	if (!_secure) {
		dao_read_property(&secure, getThis(), SL("_secure"), PH_NOISY|PH_READONLY);
	} else {
		ZVAL_COPY_VALUE(&secure, _secure);
	}

	if (!_domain) {
		dao_read_property(&domain, getThis(), SL("_domain"), PH_NOISY|PH_READONLY);
	} else {
		ZVAL_COPY_VALUE(&domain, _domain);
	}

	if (!_http_only) {
		dao_read_property(&http_only, getThis(), SL("_httpOnly"), PH_NOISY|PH_READONLY);
	} else {
		ZVAL_COPY_VALUE(&http_only, _http_only);
	}

	if (_expire || _path || _secure || _domain || _http_only) {
		DAO_CALL_FUNCTION(NULL, "session_set_cookie_params", &expire, &path, &secure, &domain, &http_only);
	}
}

PHP_METHOD(Dao_Session_Adapter, __destruct) {

	zval started = {};

	dao_read_property(&started, getThis(), SL("_started"), PH_NOISY|PH_READONLY);
	if (zend_is_true(&started)) {
		RETURN_ON_FAILURE(dao_session_write_close());
		dao_update_property_bool(getThis(), SL("_started"), 0);
	}
}

/**
 * Starts the session (if headers are already sent the session will not be started)
 *
 * @return boolean
 */
PHP_METHOD(Dao_Session_Adapter, start){

	zval started = {};

	dao_read_property(&started, getThis(), SL("_started"), PH_NOISY|PH_READONLY);
	if (zend_is_true(&started)) {
		RETURN_FALSE;
	}

	if (!SG(headers_sent)) {
		RETURN_ON_FAILURE(dao_session_start());
		dao_update_property_bool(getThis(), SL("_started"), 1);
		RETURN_TRUE;
	}

	RETURN_FALSE;
}

/**
 * Sets session's options
 *
 *<code>
 *	$session->setOptions(array(
 *		'uniqueId' => 'my-private-app'
 *	));
 *</code>
 *
 * @param array $options
 */
PHP_METHOD(Dao_Session_Adapter, setOptions){

	zval *options, unique_id = {};

	dao_fetch_params(0, 1, 0, &options);

	if (Z_TYPE_P(options) == IS_ARRAY) {
		if (dao_array_isset_fetch_str(&unique_id, options, SL("uniqueId"), PH_READONLY)) {
			dao_update_property(getThis(), SL("_uniqueId"), &unique_id);
		}

		dao_update_property(getThis(), SL("_options"), options);
	}
}

/**
 * Get internal options
 *
 * @return array
 */
PHP_METHOD(Dao_Session_Adapter, getOptions){

	RETURN_MEMBER(getThis(), "_options");
}

/**
 * Gets a session variable from an application context
 *
 * @param string $index
 * @param mixed $defaultValue
 * @param bool $remove
 * @return mixed
 */
PHP_METHOD(Dao_Session_Adapter, get){

	zval *index, *default_value = NULL, *remove = NULL, unique_id = {}, key = {}, *_SESSION;

	dao_fetch_params(0, 1, 2, &index, &default_value, &remove);

	if (!default_value) {
		default_value = &DAO_GLOBAL(z_null);
	}

	dao_read_property(&unique_id, getThis(), SL("_uniqueId"), PH_NOISY|PH_READONLY);

	DAO_CONCAT_VV(&key, &unique_id, index);

	_SESSION = dao_get_global_str(SL("_SESSION"));
	if (dao_array_isset_fetch(return_value, _SESSION, &key, PH_COPY)) {
		if (remove && zend_is_true(remove)) {
			dao_array_unset(_SESSION, &key, 0);
			if (Z_ISREF_P(&PS(http_session_vars)) && Z_TYPE_P(Z_REFVAL(PS(http_session_vars))) == IS_ARRAY) {
				zend_hash_del(Z_ARRVAL_P(Z_REFVAL(PS(http_session_vars))), Z_STR(key));
			}
		}
		zval_ptr_dtor(&key);
		return;
	}
	zval_ptr_dtor(&key);

	RETURN_CTOR(default_value);
}

/**
 * Sets a session variable in an application context
 *
 *<code>
 *	$session->set('auth', 'yes');
 *</code>
 *
 * @param string $index
 * @param string $value
 */
PHP_METHOD(Dao_Session_Adapter, set){

	zval *index, *value, unique_id = {}, key = {};

	dao_fetch_params(0, 2, 0, &index, &value);

	dao_read_property(&unique_id, getThis(), SL("_uniqueId"), PH_NOISY|PH_READONLY);

	DAO_CONCAT_VV(&key, &unique_id, index);

	dao_session_set(&key, value);
	zval_ptr_dtor(&key);
}

/**
 * Sets a session variables in an application context
 *
 *<code>
 *	$session->sets(array('auth', 'yes'));
 *</code>
 *
 * @param array $data
 */
PHP_METHOD(Dao_Session_Adapter, sets){

	zval *data, *value;
	zend_string *str_key;
	ulong idx;

	dao_fetch_params(0, 1, 0, &data);

	if (Z_TYPE_P(data) == IS_ARRAY) {
		ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(data), idx, str_key, value) {
			zval index = {};
			if (str_key) {
				ZVAL_STR(&index, str_key);
			} else {
				ZVAL_LONG(&index, idx);
			}

			DAO_CALL_SELF(NULL, "set", &index, value);
		} ZEND_HASH_FOREACH_END();
	}
}

/**
 * Check whether a session variable is set in an application context
 *
 *<code>
 *	var_dump($session->has('auth'));
 *</code>
 *
 * @param string $index
 * @return boolean
 */
PHP_METHOD(Dao_Session_Adapter, has){

	zval *index, unique_id = {}, key = {};

	dao_fetch_params(0, 1, 0, &index);
	dao_read_property(&unique_id, getThis(), SL("_uniqueId"), PH_NOISY|PH_READONLY);

	DAO_CONCAT_VV(&key, &unique_id, index);

	if(dao_session_get(&key)) {
		RETVAL_TRUE;
	} else {
		RETVAL_FALSE;
	}
	zval_ptr_dtor(&key);
}

/**
 * Removes a session variable from an application context
 *
 *<code>
 *	$session->remove('auth');
 *</code>
 *
 * @param string $index
 */
PHP_METHOD(Dao_Session_Adapter, remove){

	zval *index, unique_id = {}, key = {}, *_SESSION;

	dao_fetch_params(0, 1, 0, &index);
	dao_read_property(&unique_id, getThis(), SL("_uniqueId"), PH_NOISY|PH_READONLY);

	DAO_CONCAT_VV(&key, &unique_id, index);

	_SESSION = dao_get_global_str(SL("_SESSION"));
	dao_array_unset(_SESSION, &key, 0);

	if (Z_ISREF_P(&PS(http_session_vars)) && Z_TYPE_P(Z_REFVAL(PS(http_session_vars))) == IS_ARRAY) {
		zend_hash_del(Z_ARRVAL_P(Z_REFVAL(PS(http_session_vars))), Z_STR(key));
	}
	zval_ptr_dtor(&key);
}

/**
 * Returns active session id
 *
 *<code>
 *	echo $session->getId();
 *</code>
 *
 * @return string
 */
PHP_METHOD(Dao_Session_Adapter, getId){

	RETURN_ON_FAILURE(dao_get_session_id(return_value));
}

/**
 * Check whether the session has been started
 *
 *<code>
 *	var_dump($session->isStarted());
 *</code>
 *
 * @return boolean
 */
PHP_METHOD(Dao_Session_Adapter, isStarted){


	RETURN_MEMBER(getThis(), "_started");
}

/**
 * Update the current session id with a newly generated one 
 *
 *<code>
 *	var_dump($session->regenerate());
 *</code>
 *
 * @return boolean
 */
PHP_METHOD(Dao_Session_Adapter, regenerate){

	zval *delete_old_session = NULL;
	zend_bool del;

	dao_fetch_params(0, 0, 1, &delete_old_session);

	del = delete_old_session ? zend_is_true(delete_old_session) : 1;
	RETURN_ON_FAILURE(dao_session_regenerate_id(del));
	RETURN_TRUE;
}

/**
 * Destroys the active session
 *
 *<code>
 *	var_dump($session->destroy());
 *</code>
 *
 * @return boolean
 */
PHP_METHOD(Dao_Session_Adapter, destroy){

	zval *regenerate = NULL;

	dao_fetch_params(0, 0, 1, &regenerate);

	dao_update_property_bool(getThis(), SL("_started"), 0);

	if (regenerate && zend_is_true(regenerate)) {
		RETURN_ON_FAILURE(dao_session_regenerate_id(1));
	} else {
		RETURN_ON_FAILURE(dao_session_destroy());
	}
	RETURN_TRUE;
}

PHP_METHOD(Dao_Session_Adapter, __get)
{
	zval *property;

	dao_fetch_params(0, 1, 0, &property);

	DAO_RETURN_CALL_SELF("get", property);
}

PHP_METHOD(Dao_Session_Adapter, count)
{
	zval *_SESSION = dao_get_global_str(SL("_SESSION"));

	RETURN_LONG(dao_fast_count_int(_SESSION));
}

PHP_METHOD(Dao_Session_Adapter, getIterator)
{
	zval *_SESSION;

	_SESSION = dao_get_global_str(SL("_SESSION"));
	object_init_ex(return_value, spl_ce_ArrayIterator);
	DAO_CALL_METHOD(NULL, return_value, "__construct", _SESSION);
}

/**
 * Set the current session id
 *
 *<code>
 *	$session->setId($id);
 *</code>
 */
PHP_METHOD(Dao_Session_Adapter, setId){

	zval *sid;

	dao_fetch_params(0, 1, 0, &sid);

	RETURN_ON_FAILURE(dao_set_session_id(sid));
}
