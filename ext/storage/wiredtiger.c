
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

#include "storage/wiredtiger.h"
#include "storage/wiredtiger/cursor.h"
#include "storage/exception.h"

#include <ext/standard/file.h>

#include <wiredtiger.h>

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "kernel/file.h"
#include "kernel/exception.h"

/**
 * Dao\Storage\Wiredtiger
 *
 * It can be used to replace APC or local memstoraged.
 */
zend_class_entry *dao_storage_wiredtiger_ce;

PHP_METHOD(Dao_Storage_Wiredtiger, __construct);
PHP_METHOD(Dao_Storage_Wiredtiger, create);
PHP_METHOD(Dao_Storage_Wiredtiger, open);
PHP_METHOD(Dao_Storage_Wiredtiger, alter);
PHP_METHOD(Dao_Storage_Wiredtiger, drop);
PHP_METHOD(Dao_Storage_Wiredtiger, begin);
PHP_METHOD(Dao_Storage_Wiredtiger, commit);
PHP_METHOD(Dao_Storage_Wiredtiger, rollback);
PHP_METHOD(Dao_Storage_Wiredtiger, sync);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_wiredtiger___construct, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, home, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, config, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_wiredtiger_create, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, uri, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, config, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_wiredtiger_open, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, uri, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, config, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_wiredtiger_alter, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, uri, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, config, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_wiredtiger_drop, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, uri, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, config, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_wiredtiger_begin, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, config, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_wiredtiger_commit, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, config, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_wiredtiger_rollback, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, config, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_wiredtiger_sync, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, config, IS_STRING, 1)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_storage_wiredtiger_method_entry[] = {
	PHP_ME(Dao_Storage_Wiredtiger, __construct, arginfo_dao_storage_wiredtiger___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Storage_Wiredtiger, create, arginfo_dao_storage_wiredtiger_create, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Wiredtiger, open, arginfo_dao_storage_wiredtiger_open, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Wiredtiger, alter, arginfo_dao_storage_wiredtiger_alter, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Wiredtiger, drop, arginfo_dao_storage_wiredtiger_drop, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Wiredtiger, begin, arginfo_dao_storage_wiredtiger_begin, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Wiredtiger, commit, arginfo_dao_storage_wiredtiger_commit, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Wiredtiger, rollback, arginfo_dao_storage_wiredtiger_rollback, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Wiredtiger, sync, arginfo_dao_storage_wiredtiger_sync, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

static int dao_storage_wiredtiger_handle_error(WT_EVENT_HANDLER *handler, WT_SESSION *session, int error, const char *message)
{
	(void)(handler);
	(void)(session);

	if (message) {
		php_error_docref(NULL, E_STRICT, "%s", message);
	}

	return 0;
}

static int dao_storage_wiredtiger_handle_message(WT_EVENT_HANDLER *handler, WT_SESSION *session, const char *message)
{
	(void)(handler);
	(void)(session);

	if (message) {
		php_error_docref(NULL, E_STRICT, "%p: %s", session, message);
	}

	return 0;
}

static int dao_storage_wiredtiger_handle_progress(WT_EVENT_HANDLER *handler, WT_SESSION *session, const char *operation, uint64_t progress)
{
	static int lastlen = 0;
	int len;
	char msg[128];

	(void)(handler);
	(void)(session);

	if (operation == NULL) {
		return 0;
	}

	if (progress == 0) {
		len = snprintf(msg, sizeof(msg), "%s", operation);
	} else {
		len = snprintf(msg, sizeof(msg), "%s: %" PRIu64, operation, progress);
	}

	if (lastlen > len) {
		memset(msg + len, ' ', (size_t)(lastlen - len));
		msg[lastlen] = '\0';
	}

	lastlen = len;

	php_error_docref(NULL, E_STRICT, "%s", msg);

	return 0;
}

static int dao_storage_wiredtiger_handle_close(WT_EVENT_HANDLER *handler, WT_SESSION *session, WT_CURSOR *cursor)
{
	(void)(handler);
	(void)(session);
	(void)(cursor);
	return 0;
}

static WT_EVENT_HANDLER dao_storage_wiredtiger_handler = {
	dao_storage_wiredtiger_handle_error,
	dao_storage_wiredtiger_handle_message,
	dao_storage_wiredtiger_handle_progress,
	dao_storage_wiredtiger_handle_close
};

zend_object_handlers dao_storage_wiredtiger_object_handlers;
zend_object* dao_storage_wiredtiger_object_create_handler(zend_class_entry *ce)
{
	dao_storage_wiredtiger_object *intern = ecalloc(1, sizeof(dao_storage_wiredtiger_object) + zend_object_properties_size(ce));
	intern->std.ce = ce;

	zend_object_std_init(&intern->std, ce);
	object_properties_init(&intern->std, ce);
	intern->std.handlers = &dao_storage_wiredtiger_object_handlers;

	intern->connection = NULL;
	intern->session = NULL;

	return &intern->std;
}

void dao_storage_wiredtiger_object_free_handler(zend_object *object)
{
	dao_storage_wiredtiger_object *intern = dao_storage_wiredtiger_object_from_obj(object);

	if (intern->session) {
		intern->session->close(intern->session, NULL);
	}

	if (intern->connection) {
		intern->connection->close(intern->connection, NULL);
	}
}

/**
 * Dao\Storage\Wiredtiger initializer
 */
DAO_INIT_CLASS(Dao_Storage_Wiredtiger){

	DAO_REGISTER_CLASS_CREATE_OBJECT(Dao\\Storage, Wiredtiger, storage_wiredtiger, dao_storage_wiredtiger_method_entry, 0);

	zend_declare_property_null(dao_storage_wiredtiger_ce, SL("_home"), ZEND_ACC_PROTECTED);
	zend_declare_property_string(dao_storage_wiredtiger_ce, SL("_config"), "create", ZEND_ACC_PROTECTED);

	return SUCCESS;
}

/**
 * Dao\Storage\Wiredtiger constructor
 *
 * @param string $home
 * @param string $config
 */
PHP_METHOD(Dao_Storage_Wiredtiger, __construct)
{
	zval *home, *_config = NULL, config = {};
	dao_storage_wiredtiger_object *intern;
	int ret;
	long mode = 0777;

	dao_fetch_params(0, 1, 1, &home, &_config);

	/* Create home directory */
	if (dao_file_exists(home) == FAILURE) {
		php_stream_mkdir(Z_STRVAL_P(home), (int)mode, PHP_STREAM_MKDIR_RECURSIVE | REPORT_ERRORS, php_stream_context_from_zval(NULL, 0));
	}

	dao_update_property(getThis(), SL("_home"), home);

	if (_config && Z_TYPE_P(_config) == IS_STRING) {
		dao_update_property(getThis(), SL("_config"), _config);
	}

	dao_read_property(&config, getThis(), SL("_config"), PH_NOISY|PH_READONLY);

	intern = dao_storage_wiredtiger_object_from_obj(Z_OBJ_P(getThis()));

	ret = wiredtiger_open(Z_STRVAL_P(home), &dao_storage_wiredtiger_handler, Z_STRVAL(config), &intern->connection);

	if (ret != DAO_STORAGE_WIREDTIGER_OK) {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Error connecting to %s: %s", Z_STRVAL_P(home), wiredtiger_strerror(ret));
		return;
	}

	ret = intern->connection->open_session(intern->connection, NULL, NULL, &intern->session);
	if (ret != DAO_STORAGE_WIREDTIGER_OK) {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Error opening a session on %s: %s", Z_STRVAL_P(home), wiredtiger_strerror(ret));
		return;
	}
}

/**
 * Create table
 *
 * @param string $uri
 * @param string $config
 */
PHP_METHOD(Dao_Storage_Wiredtiger, create)
{
	zval *uri, *_config = NULL, config = {};
	dao_storage_wiredtiger_object *intern;
	int ret;

	dao_fetch_params(0, 1, 1, &uri, &_config);


	if (!_config || Z_TYPE_P(_config) != IS_STRING) {
		ZVAL_STRING(&config, "key_format=S,value_format=S");
	} else {
		ZVAL_COPY_VALUE(&config, _config);
	}

	intern = dao_storage_wiredtiger_object_from_obj(Z_OBJ_P(getThis()));

	ret = intern->session->create(intern->session, Z_STRVAL_P(uri), Z_STRVAL(config));
	if (ret != DAO_STORAGE_WIREDTIGER_OK) {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Error create table %s: %s", Z_STRVAL_P(uri), wiredtiger_strerror(ret));
		return;
	}
	RETURN_TRUE;
}

/**
 * Open table
 *
 * @param string $uri
 * @param string $config
 * @return Dao\Storage\Wiredtiger\Cursor
 */
PHP_METHOD(Dao_Storage_Wiredtiger, open)
{
	zval *uri, *_config = NULL, config = {};

	dao_fetch_params(0, 1, 1, &uri, &_config);


	if (!_config || Z_TYPE_P(_config) != IS_STRING) {
		ZVAL_NULL(&config);
	} else {
		ZVAL_COPY_VALUE(&config, _config);
	}

	object_init_ex(return_value, dao_storage_wiredtiger_cursor_ce);
	DAO_CALL_METHOD(NULL, return_value, "__construct", getThis(), uri, &config);
}

/**
 * Alter table
 *
 * @param string $uri
 * @param string $config
 */
PHP_METHOD(Dao_Storage_Wiredtiger, alter)
{
	zval *uri, *_config = NULL;
	dao_storage_wiredtiger_object *intern;
	int ret;
	char *config = NULL;

	dao_fetch_params(0, 1, 1, &uri, &_config);

	if (_config && Z_TYPE_P(_config) == IS_STRING) {
		config = Z_STRVAL_P(_config);
	}
	intern = dao_storage_wiredtiger_object_from_obj(Z_OBJ_P(getThis()));

	ret = intern->session->alter(intern->session, Z_STRVAL_P(uri), config);
	if (ret != DAO_STORAGE_WIREDTIGER_OK) {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Error alter table %s: %s", Z_STRVAL_P(uri), wiredtiger_strerror(ret));
		return;
	}
	RETURN_TRUE;
}

/**
 * Drop table
 *
 * @param string $uri
 * @param string $config
 */
PHP_METHOD(Dao_Storage_Wiredtiger, drop)
{
	zval *uri, *_config = NULL;
	dao_storage_wiredtiger_object *intern;
	int ret;
	char *config = NULL;

	dao_fetch_params(0, 1, 1, &uri, &_config);

	if (_config && Z_TYPE_P(_config) == IS_STRING) {
		config = Z_STRVAL_P(_config);
	}
	intern = dao_storage_wiredtiger_object_from_obj(Z_OBJ_P(getThis()));

	ret = intern->session->drop(intern->session, Z_STRVAL_P(uri), config);
	if (ret != DAO_STORAGE_WIREDTIGER_OK) {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Error drop table %s: %s", Z_STRVAL_P(uri), wiredtiger_strerror(ret));
		return;
	}
	RETURN_TRUE;
}

/**
 * Open a transaction
 *
 * @param string $config
 */
PHP_METHOD(Dao_Storage_Wiredtiger, begin)
{
	zval *_config = NULL;
	dao_storage_wiredtiger_object *intern;
	int ret;
	char *config = NULL;

	dao_fetch_params(0, 0, 1, &_config);

	if (_config && Z_TYPE_P(_config) == IS_STRING) {
		config = Z_STRVAL_P(_config);
	}
	intern = dao_storage_wiredtiger_object_from_obj(Z_OBJ_P(getThis()));

	ret = intern->session->begin_transaction(intern->session, config);
	if (ret != DAO_STORAGE_WIREDTIGER_OK) {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Error open a transaction %s", wiredtiger_strerror(ret));
		return;
	}
	RETURN_TRUE;
}

/**
 * Commit a transaction
 *
 * @param string $config
 */
PHP_METHOD(Dao_Storage_Wiredtiger, commit)
{
	zval *_config = NULL;
	dao_storage_wiredtiger_object *intern;
	int ret;
	char *config = NULL;

	dao_fetch_params(0, 0, 1, &_config);

	if (_config && Z_TYPE_P(_config) == IS_STRING) {
		config = Z_STRVAL_P(_config);
	}
	intern = dao_storage_wiredtiger_object_from_obj(Z_OBJ_P(getThis()));

	ret = intern->session->commit_transaction(intern->session, config);
	if (ret != DAO_STORAGE_WIREDTIGER_OK) {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Error commit a transaction %s", wiredtiger_strerror(ret));
		return;
	}
	RETURN_TRUE;
}

/**
 * Rollback a transaction
 *
 * @param string $config
 */
PHP_METHOD(Dao_Storage_Wiredtiger, rollback)
{
	zval *_config = NULL;
	dao_storage_wiredtiger_object *intern;
	int ret;
	char *config = NULL;

	dao_fetch_params(0, 0, 1, &_config);

	if (_config && Z_TYPE_P(_config) == IS_STRING) {
		config = Z_STRVAL_P(_config);
	}
	intern = dao_storage_wiredtiger_object_from_obj(Z_OBJ_P(getThis()));

	ret = intern->session->rollback_transaction(intern->session, config);
	if (ret != DAO_STORAGE_WIREDTIGER_OK) {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Error rollback a transaction %s", wiredtiger_strerror(ret));
		return;
	}
	RETURN_TRUE;
}

/**
 * Transaction sync
 *
 * @param string $config
 */
PHP_METHOD(Dao_Storage_Wiredtiger, sync)
{
	zval *_config = NULL;
	dao_storage_wiredtiger_object *intern;
	int ret;
	char *config = NULL;

	dao_fetch_params(0, 0, 1, &_config);

	if (_config && Z_TYPE_P(_config) == IS_STRING) {
		config = Z_STRVAL_P(_config);
	}
	intern = dao_storage_wiredtiger_object_from_obj(Z_OBJ_P(getThis()));

	ret = intern->session->transaction_sync(intern->session, config);
	if (ret != DAO_STORAGE_WIREDTIGER_OK) {
		DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Error transaction sync %s", wiredtiger_strerror(ret));
		return;
	}
	RETURN_TRUE;
}
