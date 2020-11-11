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

#include "async.h"
#include "exception.h"

#include <Zend/zend_closures.h>

#include "kernel/main.h"
#include "kernel/operators.h"
#include "kernel/fcall.h"
#include "kernel/object.h"
#include "kernel/array.h"
#include "kernel/exception.h"
#include "kernel/debug.h"

/**
 * Dao\Async
 */
zend_class_entry *dao_async_ce;

PHP_METHOD(Dao_Async, call);
PHP_METHOD(Dao_Async, recv);
PHP_METHOD(Dao_Async, recvAll);
PHP_METHOD(Dao_Async, count);
PHP_METHOD(Dao_Async, clear);
PHP_METHOD(Dao_Async, setFilename);
PHP_METHOD(Dao_Async, getFilename);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_async_call, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, closure, Closure, 0)
	ZEND_ARG_TYPE_INFO(0, arguments, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_async_recv, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, pid, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, flag, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_async_recvall, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, flag, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_async_setfilename, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, filename, IS_STRING, 0)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_async_method_entry[] = {
	PHP_ME(Dao_Async, call, arginfo_dao_async_call, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Async, recv, arginfo_dao_async_recv, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Async, recvAll, arginfo_dao_async_recvall, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Async, count, arginfo_empty, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Async, clear, arginfo_empty, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Async, setFilename, arginfo_dao_async_setfilename, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Async, getFilename, arginfo_empty, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};

/**
 * Dao\Async initializer
 */
DAO_INIT_CLASS(Dao_Async){

	DAO_REGISTER_CLASS(Dao, Async, async, dao_async_method_entry, 0);

	zend_declare_property_long(dao_async_ce, SL("_num"), 0, ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);
	zend_declare_property_string(dao_async_ce, SL("_filename"), __FILE__, ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);
	zend_declare_property_string(dao_async_ce, SL("_proj"), "a", ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);

	zend_declare_class_constant_long(dao_async_ce, SL("NOWAIT"),		DAO_ASYNC_NOWAIT);
	zend_declare_class_constant_long(dao_async_ce, SL("MSG_NOERROR"),	DAO_ASYNC_MSG_NOERROR);
	zend_declare_class_constant_long(dao_async_ce, SL("MSG_EXCEPT"),	DAO_ASYNC_MSG_EXCEPT);

	return SUCCESS;
}

/**
 * Called asynchronous
 *
 *<code>
 *	$async = Dao\Async::call(function () {
 *		return 'one';
 *	 });
 *</code>
 *
 * @param closure $callable
 * @return int
 */
PHP_METHOD(Dao_Async, call){

	zval *callable, *_arguments = NULL, arguments = {}, pid = {}, filename = {}, proj = {}, key = {}, seg = {}, result = {}, *sig;

	dao_fetch_params(1, 1, 1, &callable, &_arguments);

	if (Z_TYPE_P(callable) != IS_OBJECT || !instanceof_function(Z_OBJCE_P(callable), zend_ce_closure)) {
		DAO_MM_THROW_EXCEPTION_STR(dao_exception_ce, "Callable must be an closure object");
		return;
	}

	if (!_arguments || Z_TYPE_P(_arguments) != IS_ARRAY) {
		array_init(&arguments);
	} else {
		ZVAL_COPY(&arguments, _arguments);
	}
	DAO_MM_ADD_ENTRY(&arguments);

	DAO_MM_CALL_FUNCTION(&pid, "pcntl_fork");
	DAO_MM_ADD_ENTRY(&pid);

	if (DAO_LT_LONG(&pid, 0)) {
		DAO_MM_THROW_EXCEPTION_STR(dao_exception_ce, "Callable must be an closure object");
		return;
	}

	if (DAO_GT_LONG(&pid, 0)) {
		dao_static_property_incr_ce(dao_async_ce, SL("_num"));
		RETURN_MM_CTOR(&pid);
	}

	dao_read_static_property_ce(&filename, dao_async_ce, SL("_filename"), PH_READONLY);
	dao_read_static_property_ce(&proj, dao_async_ce, SL("_proj"), PH_READONLY);

	DAO_MM_CALL_FUNCTION(&key, "ftok", &filename, &proj);
	DAO_MM_ADD_ENTRY(&key);
	DAO_MM_CALL_FUNCTION(&seg, "msg_get_queue", &key);
	DAO_MM_ADD_ENTRY(&seg);
	DAO_MM_CALL_USER_FUNC_ARRAY(&result, callable, &arguments);
	DAO_MM_ADD_ENTRY(&result);

	DAO_MM_CALL_FUNCTION(&pid, "posix_getpid");
	DAO_MM_ADD_ENTRY(&pid);
	DAO_MM_CALL_FUNCTION(NULL, "msg_send", &seg, &pid, &result);

	if ((sig = zend_get_constant_str(SL("SIGKILL"))) != NULL ) {
		DAO_MM_CALL_FUNCTION(NULL, "posix_kill", &pid, sig);
	}
	RETURN_MM();
}

/**
 * Gets asynchronous result
 *
 *<code>
 *	$id = Dao\Async::call(function () {
 *		return 'one';
 *	});
 *	$data = Dao\Async::recv($id);
 *</code>
 *
 * @param int $pid
 * @param int $flag
 * @return mixed
 */
PHP_METHOD(Dao_Async, recv){

	zval *pid = NULL, *_flag = NULL, flag = {}, filename = {}, proj = {}, key = {}, seg = {}, type = {}, size = {}, message = {}, result = {};

	dao_fetch_params(1, 1, 1, &pid, &_flag);

	if (!_flag || Z_TYPE_P(_flag) == IS_NULL) {
		ZVAL_LONG(&flag, 0);
	}

	dao_read_static_property_ce(&filename, dao_async_ce, SL("_filename"), PH_READONLY);
	dao_read_static_property_ce(&proj, dao_async_ce, SL("_proj"), PH_READONLY);

	DAO_MM_CALL_FUNCTION(&key, "ftok", &filename, &proj);
	DAO_MM_ADD_ENTRY(&key);
	DAO_MM_CALL_FUNCTION(&seg, "msg_get_queue", &key);
	DAO_MM_ADD_ENTRY(&seg);

	ZVAL_LONG(&size, 1024);

	ZVAL_NULL(&type);
	ZVAL_NULL(&message);

	ZVAL_MAKE_REF(&type);
	ZVAL_MAKE_REF(&message);
	DAO_MM_CALL_FUNCTION(&result, "msg_receive", &seg, pid, &type, &size, &message, &DAO_GLOBAL(z_true), &flag);
	ZVAL_UNREF(&message);
	ZVAL_UNREF(&type);
	DAO_MM_ADD_ENTRY(&result);
	DAO_MM_ADD_ENTRY(&message);
	DAO_MM_ADD_ENTRY(&type);

	if (zend_is_true(&result)) {
		dao_static_property_decr_ce(dao_async_ce, SL("_num"));
	}

	RETURN_MM_CTOR(&message);
}

/**
 * Gets all asynchronous result
 *
 *<code>
 *	$id = Dao\Async::call(function () {
 *		return 'one';
 *	});
 *	$data = Dao\Async::recvAll();
 *</code>
 *
 * @return array
 */
PHP_METHOD(Dao_Async, recvAll){

	zval *_flag = NULL, flag = {}, num = {}, filename = {}, proj = {}, key = {}, seg = {}, pid = {}, size = {};
	int i = 0;

	dao_fetch_params(1, 0, 1, &_flag);

	if (!_flag || Z_TYPE_P(_flag) == IS_NULL) {
		ZVAL_LONG(&flag, 0);
	}

	dao_read_static_property_ce(&num, dao_async_ce, SL("_num"), PH_READONLY);
	dao_read_static_property_ce(&filename, dao_async_ce, SL("_filename"), PH_READONLY);
	dao_read_static_property_ce(&proj, dao_async_ce, SL("_proj"), PH_READONLY);

	i = dao_get_intval(&num);

	DAO_MM_CALL_FUNCTION(&key, "ftok", &filename, &proj);
	DAO_MM_ADD_ENTRY(&key);
	DAO_MM_CALL_FUNCTION(&seg, "msg_get_queue", &key);
	DAO_MM_ADD_ENTRY(&seg);

	ZVAL_LONG(&pid, 0);
	ZVAL_LONG(&size, 1024);

	array_init(return_value);

	while(i--) {
		zval type = {}, message = {}, result = {};
		ZVAL_NULL(&type);
		ZVAL_NULL(&message);

		ZVAL_MAKE_REF(&type);
		ZVAL_MAKE_REF(&message);

		DAO_MM_CALL_FUNCTION(&result, "msg_receive", &seg, &pid, &type, &size, &message, &DAO_GLOBAL(z_true), &flag);
		ZVAL_UNREF(&message);
		ZVAL_UNREF(&type);
		DAO_MM_ADD_ENTRY(&result);
		DAO_MM_ADD_ENTRY(&message);
		DAO_MM_ADD_ENTRY(&type);

		if (zend_is_true(&result)) {
			dao_static_property_decr_ce(dao_async_ce, SL("_num"));
		}

		dao_array_update(return_value, &type, &message, PH_COPY);
	}
	RETURN_MM();
}

/**
 * Gets result count
 *
 *<code>
 *	Dao\Async::count();
 *</code>
 *
 * @return int
 */
PHP_METHOD(Dao_Async, count){

	zval filename = {}, proj = {}, key = {}, seg = {}, result = {}, num = {};

	DAO_MM_INIT();

	dao_read_static_property_ce(&filename, dao_async_ce, SL("_filename"), PH_READONLY);
	dao_read_static_property_ce(&proj, dao_async_ce, SL("_proj"), PH_READONLY);

	DAO_MM_CALL_FUNCTION(&key, "ftok", &filename, &proj);
	DAO_MM_ADD_ENTRY(&key);
	DAO_MM_CALL_FUNCTION(&seg, "msg_get_queue", &key);
	DAO_MM_ADD_ENTRY(&seg);

	DAO_MM_CALL_FUNCTION(&result, "msg_stat_queue", &seg);
	DAO_MM_ADD_ENTRY(&result);

	if (Z_TYPE(result) == IS_ARRAY && dao_array_isset_fetch_str(&num, &result, SL("msg_qnum"), PH_READONLY)) {
		RETURN_MM_CTOR(&num);
	}
	RETURN_MM_FALSE;
}

/**
 * Destroy asynchronous
 *
 *<code>
 *	Dao\Async::clear();
 *</code>
 *
 * @return bool
 */
PHP_METHOD(Dao_Async, clear){

	zval filename = {}, proj = {}, key = {}, seg = {};

	DAO_MM_INIT();

	dao_read_static_property_ce(&filename, dao_async_ce, SL("_filename"), PH_READONLY);
	dao_read_static_property_ce(&proj, dao_async_ce, SL("_proj"), PH_READONLY);

	DAO_MM_CALL_FUNCTION(&key, "ftok", &filename, &proj);
	DAO_MM_ADD_ENTRY(&key);
	DAO_MM_CALL_FUNCTION(&seg, "msg_get_queue", &key);
	DAO_MM_ADD_ENTRY(&seg);

	DAO_MM_CALL_FUNCTION(return_value, "msg_remove_queue", &seg);
	RETURN_MM();
}

/**
 * Sets the filename to a System V IPC key
 *
 *<code>
 * Dao\Async::setFilename('/tmp');
 *</code>
 *
 * @param string $filename
 */
PHP_METHOD(Dao_Async, setFilename){

	zval *filename;

	dao_fetch_params(0, 1, 0, &filename);

	dao_update_static_property_ce(dao_async_ce, SL("_filename"), filename);
}

/**
 * Gets the filename
 *
 *<code>
 * Dao\Async::getFilename();
 *</code>
 *
 * @return string
 */
PHP_METHOD(Dao_Async, getFilename){

	dao_read_static_property_ce(return_value, dao_async_ce, SL("_filename"), PH_COPY);
}
