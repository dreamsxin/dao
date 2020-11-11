
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

#include "session/adapter/memcached.h"
#include "session/adapter.h"
#include "session/adapterinterface.h"
#include "session/exception.h"
#include "cache/backend/memcached.h"
#include "cache/frontend/data.h"

#ifdef DAO_USE_PHP_SESSION
#include <ext/session/php_session.h>
#endif

#include "kernel/main.h"
#include "kernel/exception.h"
#include "kernel/fcall.h"
#include "kernel/string.h"
#include "kernel/concat.h"
#include "kernel/array.h"
#include "kernel/object.h"

/**
 * Dao\Session\Adapter\Memcached
 *
 * This adapter store sessions in memcached
 *
 *<code>
 * $session = new Dao\Session\Adapter\Memcached(array(
 *     'servers' => array(
 *         array('host' => 'localhost', 'port' => 11211, 'weight' => 1),
 *     ),
 *     'client' => array(
 *         Memcached::OPT_HASH => Memcached::HASH_MD5,
 *         Memcached::OPT_PREFIX_KEY => 'prefix.',
 *     ),
 *    'lifetime' => 3600,
 *    'prefix' => 'my_'
 * ));
 *
 * $session->start();
 *
 * $session->set('var', 'some-value');
 *
 * echo $session->get('var');
 *</code>
 */
zend_class_entry *dao_session_adapter_memcached_ce;

PHP_METHOD(Dao_Session_Adapter_Memcached, start);
PHP_METHOD(Dao_Session_Adapter_Memcached, open);
PHP_METHOD(Dao_Session_Adapter_Memcached, close);
PHP_METHOD(Dao_Session_Adapter_Memcached, read);
PHP_METHOD(Dao_Session_Adapter_Memcached, write);
PHP_METHOD(Dao_Session_Adapter_Memcached, destroy);
PHP_METHOD(Dao_Session_Adapter_Memcached, gc);

static const zend_function_entry dao_session_adapter_memcached_method_entry[] = {
	PHP_ME(Dao_Session_Adapter_Memcached, start, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Session_Adapter_Memcached, open, arginfo_dao_session_adapterinterface_open, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Session_Adapter_Memcached, close, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Session_Adapter_Memcached, read, arginfo_dao_session_adapterinterface_read, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Session_Adapter_Memcached, write, arginfo_dao_session_adapterinterface_write, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Session_Adapter_Memcached, destroy, arginfo_dao_session_adapterinterface_destroy, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Session_Adapter_Memcached, gc, arginfo_dao_session_adapterinterface_gc, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Session\Adapter\Memcached initializer
 */
DAO_INIT_CLASS(Dao_Session_Adapter_Memcached){

	DAO_REGISTER_CLASS_EX(Dao\\Session\\Adapter, Memcached, session_adapter_memcached, dao_session_adapter_ce, dao_session_adapter_memcached_method_entry, 0);

	zend_declare_property_long(dao_session_adapter_memcached_ce, SL("_lifetime"), 8600, ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_session_adapter_memcached_ce, SL("_memcached"), ZEND_ACC_PROTECTED);

//#ifdef DAO_USE_PHP_SESSION
//	zend_class_implements(
//		dao_session_adapter_memcached_ce, 1,
//		php_session_iface_entry
//	);
//#endif
	return SUCCESS;
}

/**
 * Starts the session (if headers are already sent the session will not be started)
 *
 * @return boolean
 */
PHP_METHOD(Dao_Session_Adapter_Memcached, start){

	zval options = {}, servers = {}, client = {}, lifetime = {}, prefix = {}, backend_option = {}, frontend_option = {}, frontend_data = {}, memcached = {};
//#ifndef DAO_USE_PHP_SESSION
	zval callable_open = {}, callable_close = {}, callable_read = {}, callable_write = {}, callable_destroy = {}, callable_gc = {};
//#endif

	dao_read_property(&options, getThis(), SL("_options"), PH_NOISY|PH_READONLY);

	if (Z_TYPE(options) != IS_ARRAY) {
		DAO_THROW_EXCEPTION_STR(dao_session_exception_ce, "The options must be an array");
		return;
	}

	if (!dao_array_isset_fetch_str(&servers, &options, SL("servers"), PH_READONLY)) {
		DAO_THROW_EXCEPTION_STR(dao_session_exception_ce, "No servers given in options");
		return;
	}

	array_init(&backend_option);

	dao_array_update_str(&backend_option, SL("servers"), &servers, PH_COPY);

	if (dao_array_isset_fetch_str(&client, &options, SL("client"), PH_READONLY)) {
		dao_array_update_str(&backend_option, SL("client"), &client, PH_COPY);
	}

	if (!dao_array_isset_fetch_str(&lifetime, &options, SL("lifetime"), PH_READONLY)) {
		ZVAL_LONG(&lifetime, 8600);
	} else {
		dao_update_property(getThis(), SL("_lifetime"), &lifetime);
	}

	if (dao_array_isset_fetch_str(&prefix, &options, SL("prefix"), PH_READONLY)) {
        dao_array_update_str(&backend_option, SL("prefix"), &prefix, PH_COPY);
	}

	/* create memcached instance */
	array_init_size(&frontend_option, 1);

	dao_array_update_str(&frontend_option, SL("lifetime"), &lifetime, PH_COPY);

	object_init_ex(&frontend_data, dao_cache_frontend_data_ce);

	DAO_CALL_METHOD(NULL, &frontend_data, "__construct", &frontend_option);
	zval_ptr_dtor(&frontend_option);

	object_init_ex(&memcached, dao_cache_backend_memcached_ce);

	DAO_CALL_METHOD(NULL, &memcached, "__construct", &frontend_data, &backend_option);
	zval_ptr_dtor(&frontend_data);
	zval_ptr_dtor(&backend_option);

	dao_update_property(getThis(), SL("_memcached"), &memcached);
	zval_ptr_dtor(&memcached);

//#ifdef DAO_USE_PHP_SESSION
//	DAO_CALL_FUNCTION(return_value, "session_set_save_handler", getThis(), &DAO_GLOBAL(z_true));
//#else
	/* open callback */
	array_init_size(&callable_open, 2);
	dao_array_append(&callable_open, getThis(), PH_COPY);
	dao_array_append_str(&callable_open, SL("open"), 0);

	/* close callback */
	array_init_size(&callable_close, 2);
	dao_array_append(&callable_close, getThis(), PH_COPY);
	dao_array_append_str(&callable_close, SL("close"), 0);

	/* read callback */
	array_init_size(&callable_read, 2);
	dao_array_append(&callable_read, getThis(), PH_COPY);
	dao_array_append_str(&callable_read, SL("read"), 0);

	/* write callback */
	array_init_size(&callable_write, 2);
	dao_array_append(&callable_write, getThis(), PH_COPY);
	dao_array_append_str(&callable_write, SL("write"), 0);

	/* destroy callback */
	array_init_size(&callable_destroy, 2);
	dao_array_append(&callable_destroy, getThis(), PH_COPY);
	dao_array_append_str(&callable_destroy, SL("destroy"), 0);

	/* gc callback */
	array_init_size(&callable_gc, 2);
	dao_array_append(&callable_gc, getThis(), PH_COPY);
	dao_array_append_str(&callable_gc, SL("gc"), 0);

	DAO_CALL_FUNCTION(return_value, "session_set_save_handler", &callable_open, &callable_close, &callable_read, &callable_write, &callable_destroy, &callable_gc);
	zval_ptr_dtor(&callable_open);
	zval_ptr_dtor(&callable_close);
	zval_ptr_dtor(&callable_read);
	zval_ptr_dtor(&callable_write);
	zval_ptr_dtor(&callable_destroy);
	zval_ptr_dtor(&callable_gc);
	DAO_CALL_FUNCTION(NULL, "session_register_shutdown");
//#endif
	if (!zend_is_true(return_value)) {
		DAO_THROW_EXCEPTION_STR(dao_session_exception_ce, "Sets user-level session storage functions failed");
		RETURN_FALSE;
	}

	DAO_CALL_PARENT(return_value, dao_session_adapter_memcached_ce, getThis(), "start");
}

/**
 *
 * @return boolean
 */
PHP_METHOD(Dao_Session_Adapter_Memcached, open){

	RETURN_TRUE;
}

/**
 *
 * @return boolean
 */
PHP_METHOD(Dao_Session_Adapter_Memcached, close){

	RETURN_TRUE;
}

/**
 *
 * @param string $sessionId
 * @return mixed
 */
PHP_METHOD(Dao_Session_Adapter_Memcached, read){

	zval *sid, lifetime = {}, memcached = {};

	dao_fetch_params(0, 1, 0, &sid);

	dao_read_property(&lifetime, getThis(), SL("_lifetime"), PH_NOISY|PH_READONLY);
	dao_read_property(&memcached, getThis(), SL("_memcached"), PH_NOISY|PH_READONLY);

	if (Z_TYPE(memcached) == IS_OBJECT) {
		DAO_CALL_METHOD(return_value, &memcached, "get", sid, &lifetime);
		if (Z_TYPE_P(return_value)!=IS_STRING) {
			RETURN_EMPTY_STRING();
		}
	} else {
		RETURN_EMPTY_STRING();
	}
}

/**
 *
 * @param string $sessionId
 * @param string $data
 */
PHP_METHOD(Dao_Session_Adapter_Memcached, write){

	zval *sid, *data, lifetime = {}, memcached = {};

	dao_fetch_params(0, 2, 0, &sid, &data);

	dao_read_property(&lifetime, getThis(), SL("_lifetime"), PH_NOISY|PH_READONLY);
	dao_read_property(&memcached, getThis(), SL("_memcached"), PH_NOISY|PH_READONLY);

	if (Z_TYPE(memcached) == IS_OBJECT) {
		DAO_CALL_METHOD(return_value, &memcached, "save", sid, data, &lifetime);
	} else {
		RETURN_FALSE;
	}
}

/**
 *
 * @param string $session_id optional, session id
 *
 * @return boolean
 */
PHP_METHOD(Dao_Session_Adapter_Memcached, destroy){

	zval *_sid = NULL, sid = {}, memcached = {};

	dao_fetch_params(0, 0, 1, &_sid);

	if (!_sid) {
		DAO_CALL_SELF(&sid, "getid");
	} else {
		ZVAL_COPY_VALUE(&sid, _sid);
	}

	dao_read_property(&memcached, getThis(), SL("_memcached"), PH_NOISY|PH_READONLY);

	if (Z_TYPE(memcached) == IS_OBJECT) {
		DAO_RETURN_CALL_METHOD(&memcached, "delete", &sid);
	} else {
		RETURN_FALSE;
	}
}

/**
 *
 * @return boolean
 */
PHP_METHOD(Dao_Session_Adapter_Memcached, gc){

	RETURN_TRUE;
}
