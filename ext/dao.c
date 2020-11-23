
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

#include "php_dao.h"

#include <main/php_ini.h>
#include <ext/standard/info.h>
#include <Zend/zend_extensions.h>
#include <main/SAPI.h>

#include "py.h"

#if DAO_USE_ASYNC
#include "async/async_helper.h"
#endif

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"
#include "kernel/mbstring.h"
#include "kernel/time.h"

#include "interned-strings.h"

#include "dao.h"

ZEND_API void (*original_zend_execute_internal)(zend_execute_data *execute_data, zval *return_value);
ZEND_API void (*original_zend_execute_ex)(zend_execute_data *execute_data);

ZEND_API void (*xhprof_orig_zend_execute_internal)(zend_execute_data*, zval*);
ZEND_API void (*xhprof_orig_zend_execute_ex)(zend_execute_data*);

ZEND_API void (*async_orig_execute_ex)(zend_execute_data *exec);

ZEND_DECLARE_MODULE_GLOBALS(dao)

#if DAO_USE_ASYNC
static PHP_INI_MH(OnUpdateFiberStackSize)
{
	OnUpdateLong(entry, new_value, mh_arg1, mh_arg2, mh_arg3, stage);

	if (ASYNC_G(stack_size) < 0) {
		ASYNC_G(stack_size) = 0;
	}

	return SUCCESS;
}

static PHP_INI_MH(OnUpdateThreadCount)
{
	OnUpdateLong(entry, new_value, mh_arg1, mh_arg2, mh_arg3, stage);

	if (ASYNC_G(threads) < 4) {
		ASYNC_G(threads) = 4;
	}

	if (ASYNC_G(threads) > 128) {
		ASYNC_G(threads) = 128;
	}

	return SUCCESS;
}
#endif

PHP_INI_BEGIN()
	/* Enables/Disables debug */
	STD_PHP_INI_BOOLEAN("dao.debug.enable_debug",           "0",    PHP_INI_ALL,    OnUpdateBool, debug.enable_debug,           zend_dao_globals, dao_globals)
	/* Enables/Disables view strict mode */
	STD_PHP_INI_BOOLEAN("dao.mvc.enable_view_strict",       "1",    PHP_INI_ALL,    OnUpdateBool, mvc.enable_view_strict,       zend_dao_globals, dao_globals)
	/* Enables/Disables router internal events */
	STD_PHP_INI_BOOLEAN("dao.mvc.enable_router_events",     "1",    PHP_INI_ALL,    OnUpdateBool, mvc.enable_router_events,     zend_dao_globals, dao_globals)
	/* Enables/Disables globally the internal events */
	STD_PHP_INI_BOOLEAN("dao.orm.events",                   "1",    PHP_INI_ALL,    OnUpdateBool, orm.events,                   zend_dao_globals, dao_globals)
	/* Enables/Disables virtual foreign keys */
	STD_PHP_INI_BOOLEAN("dao.orm.virtual_foreign_keys",     "1",    PHP_INI_ALL,    OnUpdateBool, orm.virtual_foreign_keys,     zend_dao_globals, dao_globals)
	/* Enables/Disables automatic NOT NULL validation */
	STD_PHP_INI_BOOLEAN("dao.orm.not_null_validations",     "1",    PHP_INI_ALL,    OnUpdateBool, orm.not_null_validations,     zend_dao_globals, dao_globals)
	/* Enables/Disables automatic length validation */
	STD_PHP_INI_BOOLEAN("dao.orm.length_validations",       "1",    PHP_INI_ALL,    OnUpdateBool, orm.length_validations,       zend_dao_globals, dao_globals)
	STD_PHP_INI_BOOLEAN("dao.orm.use_mb_strlen",            "1",    PHP_INI_ALL,    OnUpdateBool, orm.use_mb_strlen,            zend_dao_globals, dao_globals)
	/* Enables/Disables throwing an exception if save fails */
	STD_PHP_INI_BOOLEAN("dao.orm.exception_on_failed_save", "0",    PHP_INI_ALL,    OnUpdateBool, orm.exception_on_failed_save, zend_dao_globals, dao_globals)
	/* Enables/Disables literals in PHQL */
	STD_PHP_INI_BOOLEAN("dao.orm.enable_literals",          "1",    PHP_INI_ALL,    OnUpdateBool, orm.enable_literals,          zend_dao_globals, dao_globals)
	/* Enables/Disables property method */
	STD_PHP_INI_BOOLEAN("dao.orm.enable_property_method",   "1",    PHP_INI_ALL,    OnUpdateBool, orm.enable_property_method,   zend_dao_globals, dao_globals)
	/* Enables/Disables auto convert column value follow database data type */
	STD_PHP_INI_BOOLEAN("dao.orm.enable_auto_convert",      "1",    PHP_INI_ALL,    OnUpdateBool, orm.enable_auto_convert,      zend_dao_globals, dao_globals)
	STD_PHP_INI_BOOLEAN("dao.orm.allow_update_primary",     "0",    PHP_INI_ALL,    OnUpdateBool, orm.allow_update_primary,     zend_dao_globals, dao_globals)
	STD_PHP_INI_BOOLEAN("dao.orm.enable_strict",            "0",    PHP_INI_ALL,    OnUpdateBool, orm.enable_strict,            zend_dao_globals, dao_globals)
	STD_PHP_INI_BOOLEAN("dao.orm.must_column",              "1",    PHP_INI_ALL,    OnUpdateBool, orm.must_column,              zend_dao_globals, dao_globals)
	/* Enables/Disables allow empty */
	STD_PHP_INI_BOOLEAN("dao.validation.allow_empty",       "0",    PHP_INI_ALL,    OnUpdateBool, validation.allow_empty,       zend_dao_globals, dao_globals)
	/* Enables/Disables auttomatic escape */
	STD_PHP_INI_BOOLEAN("dao.db.escape_identifiers",        "1",    PHP_INI_ALL,    OnUpdateBool, db.escape_identifiers,        zend_dao_globals, dao_globals)

	/* Enables/Disables xhprof */
	STD_PHP_INI_ENTRY("dao.xhprof.nesting_max_level", "0",  PHP_INI_ALL, OnUpdateLong, xhprof.nesting_maximum_level,	zend_dao_globals, dao_globals)
	STD_PHP_INI_BOOLEAN("dao.xhprof.enable_xhprof",   "0",  PHP_INI_SYSTEM, OnUpdateBool, xhprof.enable_xhprof,	zend_dao_globals, dao_globals)
    STD_PHP_INI_ENTRY("dao.xhprof.clock_use_rdtsc",   "0",  PHP_INI_ALL, OnUpdateBool, xhprof.clock_use_rdtsc,	zend_dao_globals, dao_globals)
	STD_PHP_INI_ENTRY("dao.snowflake.node", "0", PHP_INI_SYSTEM, OnUpdateLong, snowflake.node,  zend_dao_globals, dao_globals)

	STD_PHP_INI_BOOLEAN("dao.aop.enable_aop",      "0", PHP_INI_SYSTEM, OnUpdateBool, aop.enable_aop, zend_dao_globals, dao_globals)
#if DAO_USE_AOP_PROPERTY
	STD_PHP_INI_BOOLEAN("dao.aop.enable_property", "0", PHP_INI_SYSTEM, OnUpdateBool, aop.enable_property, zend_dao_globals, dao_globals)
#endif

#if DAO_USE_ASYNC
	STD_PHP_INI_ENTRY("dao.async.dns",        "0", PHP_INI_SYSTEM | PHP_INI_PERDIR, OnUpdateBool, async.dns_enabled, zend_dao_globals, dao_globals)
	STD_PHP_INI_ENTRY("dao.async.filesystem", "0", PHP_INI_SYSTEM | PHP_INI_PERDIR, OnUpdateBool, async.fs_enabled, zend_dao_globals, dao_globals)
	STD_PHP_INI_ENTRY("dao.async.fs_enabled", "0", PHP_INI_SYSTEM | PHP_INI_PERDIR, OnUpdateBool, async.fs_enabled, zend_dao_globals, dao_globals)
	STD_PHP_INI_ENTRY("dao.async.forked",     "0", PHP_INI_SYSTEM | PHP_INI_PERDIR, OnUpdateBool, async.forked, zend_dao_globals, dao_globals)
	STD_PHP_INI_ENTRY("dao.async.stack_size", "0", PHP_INI_SYSTEM | PHP_INI_PERDIR, OnUpdateFiberStackSize, async.stack_size, zend_dao_globals, dao_globals)
	STD_PHP_INI_ENTRY("dao.async.tcp",        "0", PHP_INI_SYSTEM | PHP_INI_PERDIR, OnUpdateBool, async.tcp_enabled, zend_dao_globals, dao_globals)
	STD_PHP_INI_ENTRY("dao.async.threads",    "4", PHP_INI_SYSTEM | PHP_INI_PERDIR, OnUpdateThreadCount, async.threads, zend_dao_globals, dao_globals)
	STD_PHP_INI_ENTRY("dao.async.timer",      "0", PHP_INI_SYSTEM | PHP_INI_PERDIR, OnUpdateBool, async.timer_enabled, zend_dao_globals, dao_globals)
	STD_PHP_INI_ENTRY("dao.async.udp",        "0", PHP_INI_SYSTEM | PHP_INI_PERDIR, OnUpdateBool, async.udp_enabled, zend_dao_globals, dao_globals)
	STD_PHP_INI_ENTRY("dao.async.unix",       "0", PHP_INI_SYSTEM | PHP_INI_PERDIR, OnUpdateBool, async.unix_enabled, zend_dao_globals, dao_globals)
#endif

#ifdef DAO_JWT
	STD_PHP_INI_ENTRY("dao.jwt.algorithm", "HS256", PHP_INI_SYSTEM | PHP_INI_PERDIR, OnUpdateString, jwt.algorithm, zend_dao_globals, dao_globals)
#endif
PHP_INI_END()

#if DAO_USE_ASYNC
ASYNC_CALLBACK init_threads(uv_work_t *req) { }

ASYNC_CALLBACK after_init_threads(uv_work_t *req, int status)
{
	ASYNC_G(threads) = 0;

	free(req);
}
#endif

static PHP_MINIT_FUNCTION(dao)
{
	REGISTER_INI_ENTRIES();

	DAO_GLOBAL(cli) = !strcmp(sapi_module.name, "cli");

    if (DAO_GLOBAL(snowflake).node < 0) {
		php_error_docref(NULL, E_WARNING, "snowflake.node must greater than 0");
		DAO_GLOBAL(snowflake).node = 0;
    }

    if (DAO_GLOBAL(snowflake).node > 0x3FF) {
		php_error_docref(NULL, E_WARNING, "snowflake.node must less than %d", 0x3FF);
		DAO_GLOBAL(snowflake).node = 0;
    }

	if (DAO_GLOBAL(aop).enable_aop) {
		// overload zend_execute_ex and zend_execute_internal
		original_zend_execute_internal = zend_execute_internal;
		zend_execute_internal = dao_aop_execute_internal;

		original_zend_execute_ex = zend_execute_ex;
		zend_execute_ex = dao_aop_execute_ex;

#if DAO_USE_AOP_PROPERTY
		if (DAO_GLOBAL(aop).enable_property) {

			zend_object_handlers *handlers = (zend_object_handlers *)zend_get_std_object_handlers();
			// overload zend_std_read_property and zend_std_write_property
			original_zend_std_read_property = handlers->read_property;
			handlers->read_property = dao_aop_read_property;

			original_zend_std_write_property = handlers->write_property;
			handlers->write_property = dao_aop_write_property;

			original_zend_std_get_property_ptr_ptr = handlers->get_property_ptr_ptr;
			handlers->get_property_ptr_ptr = dao_aop_get_property_ptr_ptr;

		}
#endif
	}

	if (DAO_GLOBAL(xhprof).enable_xhprof) {
		xhprof_orig_zend_execute_internal = zend_execute_internal;
		zend_execute_internal = dao_xhprof_execute_internal;

		xhprof_orig_zend_execute_ex = zend_execute_ex;
		zend_execute_ex = dao_xhprof_execute_ex;
	}

#if DAO_USE_MONGOC
	mongoc_init();
#endif

#if DAO_USE_PYTHON
	Py_SetProgramName("Dao");
	Py_InitializeEx(0); // Py_Initialize();
	PyEval_InitThreads();
	python_streams_init();
	DAO_GLOBAL(python).mtstate = PyEval_SaveThread();
	DAO_GLOBAL(python).isInitialized = Py_IsInitialized();
	PyEval_ReleaseLock();
#endif

#ifdef DAO_USE_ASYNC
	ASYNC_G(cli) = !strcmp(sapi_module.name, "cli");
	if (ASYNC_G(cli)) {
		uv_work_t *req = malloc(sizeof(uv_work_t));
		char entry[4];

		sprintf(entry, "%d", (int) MAX(4, MIN(128, ASYNC_G(threads))));
		uv_os_setenv("UV_THREADPOOL_SIZE", (const char *) entry);

		uv_queue_work(uv_default_loop(), req, init_threads, after_init_threads);
		uv_cancel((uv_req_t *) req);
		uv_run(uv_default_loop(), UV_RUN_DEFAULT);
	}

#ifdef HAVE_ASYNC_SSL
#if OPENSSL_VERSION_NUMBER < 0x10100000L || defined (LIBRESSL_VERSION_NUMBER)
	SSL_library_init();
	OPENSSL_config(NULL);
	SSL_load_error_strings();
#else
	OPENSSL_init_ssl(OPENSSL_INIT_LOAD_CONFIG, NULL);
#endif
#endif

	async_task_ce_register();
	async_stream_ce_register();
	async_socket_ce_register();

	async_channel_ce_register();
	async_console_ce_register();
	async_context_ce_register();
	async_deferred_ce_register();
	async_dns_ce_register();
	async_event_ce_register();
	async_monitor_ce_register();
	async_pipe_ce_register();
	async_poll_ce_register();
	async_process_ce_register();
	async_signal_ce_register();
	async_ssl_ce_register();
	async_sync_ce_register();
	async_tcp_ce_register();
	async_thread_ce_register();
	async_timer_ce_register();
	async_udp_socket_ce_register();

#ifdef HAVE_ASYNC_SSL
	REGISTER_LONG_CONSTANT("DAO_ASYNC_SSL_SUPPORTED", 1, CONST_CS|CONST_PERSISTENT);
	
#ifdef ASYNC_TLS_SNI
	REGISTER_LONG_CONSTANT("DAO_ASYNC_SSL_SNI_SUPPORTED", 1, CONST_CS|CONST_PERSISTENT);
#else
	REGISTER_LONG_CONSTANT("DAO_ASYNC_SSL_SNI_SUPPORTED", 0, CONST_CS|CONST_PERSISTENT);
#endif
	
#ifdef ASYNC_TLS_ALPN
	REGISTER_LONG_CONSTANT("DAO_ASYNC_SSL_ALPN_SUPPORTED", 1, CONST_CS|CONST_PERSISTENT);
#else
	REGISTER_LONG_CONSTANT("DAO_ASYNC_SSL_ALPN_SUPPORTED", 0, CONST_CS|CONST_PERSISTENT);
#endif

#else
	REGISTER_LONG_CONSTANT("DAO_ASYNC_SSL_SUPPORTED", 0, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DAO_ASYNC_SSL_SNI_SUPPORTED", 0, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DAO_ASYNC_SSL_ALPN_SUPPORTED", 0, CONST_CS|CONST_PERSISTENT);
#endif

	async_orig_execute_ex = zend_execute_ex;
	zend_execute_ex = async_execute_ex;
#endif

	/* 1. Register exceptions */
	DAO_INIT(Dao_Exception);
	DAO_INIT(Dao_ContinueException);
	DAO_INIT(Dao_ExitException);
	DAO_INIT(Dao_Profiler_Exception);
	DAO_INIT(Dao_Debug_Exception);
	DAO_INIT(Dao_Acl_Exception);
	DAO_INIT(Dao_Annotations_Exception);
	DAO_INIT(Dao_Assets_Exception);
	DAO_INIT(Dao_Cache_Exception);
	DAO_INIT(Dao_Crypt_Exception);
	DAO_INIT(Dao_Db_Exception);
	DAO_INIT(Dao_Db_Builder_Exception);
	DAO_INIT(Dao_Di_Exception);
	DAO_INIT(Dao_Escaper_Exception);
	DAO_INIT(Dao_Events_Exception);
	DAO_INIT(Dao_Filter_Exception);
	DAO_INIT(Dao_Flash_Exception);
	DAO_INIT(Dao_Forms_Exception);
	DAO_INIT(Dao_Http_Cookie_Exception);
	DAO_INIT(Dao_Http_Request_Exception);
	DAO_INIT(Dao_Http_Response_Exception);
	DAO_INIT(Dao_Http_Client_Exception);
	DAO_INIT(Dao_Image_Exception);

	DAO_INIT(Dao_Router_Exception);
	DAO_INIT(Dao_Application_Exception);
	DAO_INIT(Dao_Cli_Console_Exception);
	DAO_INIT(Dao_Cli_Dispatcher_Exception);
	DAO_INIT(Dao_Cli_Router_Exception);
	DAO_INIT(Dao_Cli_Options_Exception);
	DAO_INIT(Dao_Mvc_Application_Exception);
	DAO_INIT(Dao_Mvc_Dispatcher_Exception);
	DAO_INIT(Dao_Mvc_Micro_Exception);
	DAO_INIT(Dao_Mvc_Model_Exception);
	DAO_INIT(Dao_Mvc_Model_Query_Exception);
	DAO_INIT(Dao_Mvc_Model_Transaction_Exception);
	DAO_INIT(Dao_Mvc_Router_Exception);
	DAO_INIT(Dao_Mvc_Url_Exception);
	DAO_INIT(Dao_Mvc_View_Exception);
	DAO_INIT(Dao_Paginator_Exception);
	DAO_INIT(Dao_Tag_Exception);
	DAO_INIT(Dao_Validation_Exception);
	DAO_INIT(Dao_Security_Exception);
	DAO_INIT(Dao_Session_Exception);
	DAO_INIT(Dao_Config_Exception);
	DAO_INIT(Dao_Loader_Exception);
	DAO_INIT(Dao_Logger_Exception);
	DAO_INIT(Dao_Translate_Exception);
	DAO_INIT(Dao_Mvc_Micro_Exception);
	DAO_INIT(Dao_Binary_Exception);

	DAO_INIT(Dao_Storage_Exception);

#if DAO_USE_SHM_OPEN
	DAO_INIT(Dao_Sync_Exception);
#endif
#if DAO_USE_PYTHON
	DAO_INIT(Dao_Py_Exception);
#endif

	DAO_INIT(Dao_Aop_Exception);

	/* 2. Register interfaces */
	DAO_INIT(Dao_DiInterface);
	DAO_INIT(Dao_Di_InjectionAwareInterface);
	DAO_INIT(Dao_Di_ServiceInterface);
	DAO_INIT(Dao_Events_EventInterface);
	DAO_INIT(Dao_Events_EventsAwareInterface);

	DAO_INIT(Dao_ProfilerInterface);
	DAO_INIT(Dao_Profiler_ItemInterface);

	DAO_INIT(Dao_Acl_AdapterInterface);
	DAO_INIT(Dao_Acl_ResourceInterface);
	DAO_INIT(Dao_Acl_ResourceAware);
	DAO_INIT(Dao_Acl_RoleInterface);
	DAO_INIT(Dao_Acl_RoleAware);
	DAO_INIT(Dao_Annotations_AdapterInterface);
	DAO_INIT(Dao_Annotations_ReaderInterface);
	DAO_INIT(Dao_Assets_FilterInterface);
	DAO_INIT(Dao_Cache_BackendInterface);
	DAO_INIT(Dao_Cache_FrontendInterface);
	DAO_INIT(Dao_CryptInterface);
	DAO_INIT(Dao_Db_AdapterInterface);
	DAO_INIT(Dao_Db_ColumnInterface);
	DAO_INIT(Dao_Db_DialectInterface);
	DAO_INIT(Dao_Db_IndexInterface);
	DAO_INIT(Dao_Db_ReferenceInterface);
	DAO_INIT(Dao_Db_ResultInterface);
	DAO_INIT(Dao_DispatcherInterface);
	DAO_INIT(Dao_Config_AdapterInterface);
	DAO_INIT(Dao_EscaperInterface);
	DAO_INIT(Dao_Events_ManagerInterface);
	DAO_INIT(Dao_FlashInterface);
	DAO_INIT(Dao_FilterInterface);
	DAO_INIT(Dao_Filter_UserFilterInterface);
	DAO_INIT(Dao_Forms_ElementInterface);
	DAO_INIT(Dao_Http_RequestInterface);
	DAO_INIT(Dao_Http_Request_FileInterface);
	DAO_INIT(Dao_Http_ResponseInterface);
	DAO_INIT(Dao_Http_Response_CookiesInterface);
	DAO_INIT(Dao_Http_Response_HeadersInterface);
	DAO_INIT(Dao_Http_Client_AdapterInterface);
	DAO_INIT(Dao_Image_AdapterInterface);
	DAO_INIT(Dao_Logger_AdapterInterface);
	DAO_INIT(Dao_Logger_FormatterInterface);

	DAO_INIT(Dao_RouterInterface);
	DAO_INIT(Dao_Db_BuilderInterface);
	DAO_INIT(Dao_Mvc_ControllerInterface);
	DAO_INIT(Dao_Mvc_DispatcherInterface);
	DAO_INIT(Dao_Mvc_Micro_CollectionInterface);
	DAO_INIT(Dao_Mvc_Micro_MiddlewareInterface);
	DAO_INIT(Dao_Mvc_ModelInterface);
	DAO_INIT(Dao_Mvc_Model_BehaviorInterface);
	DAO_INIT(Dao_Mvc_Model_CriteriaInterface);
	DAO_INIT(Dao_Mvc_Model_ManagerInterface);
	DAO_INIT(Dao_Mvc_Model_MetaDataInterface);
	DAO_INIT(Dao_Mvc_Model_QueryInterface);
	DAO_INIT(Dao_Mvc_Model_Query_BuilderInterface);
	DAO_INIT(Dao_Mvc_Model_Query_StatusInterface);
	DAO_INIT(Dao_Mvc_Model_RelationInterface);
	DAO_INIT(Dao_Mvc_Model_ResultInterface);
	DAO_INIT(Dao_Mvc_Model_ResultsetInterface);
	DAO_INIT(Dao_Mvc_Model_TransactionInterface);
	DAO_INIT(Dao_Mvc_Model_Transaction_ManagerInterface);
	DAO_INIT(Dao_Mvc_ModuleDefinitionInterface);
	DAO_INIT(Dao_Mvc_RouterInterface);
	DAO_INIT(Dao_Mvc_Router_RouteInterface);
	DAO_INIT(Dao_Mvc_UrlInterface);
	DAO_INIT(Dao_Mvc_ViewInterface);
	DAO_INIT(Dao_Mvc_View_EngineInterface);
	DAO_INIT(Dao_Mvc_View_ModelInterface);
	DAO_INIT(Dao_Paginator_AdapterInterface);
	DAO_INIT(Dao_Session_AdapterInterface);
	DAO_INIT(Dao_Session_BagInterface);
	DAO_INIT(Dao_Translate_AdapterInterface);
	DAO_INIT(Dao_ValidationInterface);
	DAO_INIT(Dao_Validation_ValidatorInterface);
	DAO_INIT(Dao_Validation_MessageInterface);

	/* 4. Register everything else */
	DAO_INIT(Dao_Xhprof);
	DAO_INIT(Dao_Di);
	DAO_INIT(Dao_Di_Injectable);
	DAO_INIT(Dao_Di_FactoryDefault);
	DAO_INIT(Dao_Di_FactoryDefault_Cli);
	DAO_INIT(Dao_Di_Service);
	DAO_INIT(Dao_Di_Service_Builder);

	DAO_INIT(Dao_Profiler);
	DAO_INIT(Dao_Profiler_Item);

	DAO_INIT(Dao_Forms_Element);
	DAO_INIT(Dao_Annotations_Adapter);
	DAO_INIT(Dao_Logger_Adapter);
	DAO_INIT(Dao_Logger_Formatter);
	DAO_INIT(Dao_Assets_Resource);
	DAO_INIT(Dao_Flash);
	DAO_INIT(Dao_Dispatcher);
	DAO_INIT(Dao_Translate_Adapter);
	DAO_INIT(Dao_Config);
	DAO_INIT(Dao_Config_Adapter);
	DAO_INIT(Dao_Config_Adapter_Ini);
	DAO_INIT(Dao_Config_Adapter_Json);
	DAO_INIT(Dao_Config_Adapter_Php);
	DAO_INIT(Dao_Config_Adapter_Yaml);
	DAO_INIT(Dao_Acl);
	DAO_INIT(Dao_Acl_Adapter);
	DAO_INIT(Dao_Acl_Role);
	DAO_INIT(Dao_Acl_Resource);
	DAO_INIT(Dao_Acl_Adapter_Memory);
	DAO_INIT(Dao_Session_Adapter);

	DAO_INIT(Dao_Cache_Backend);
	DAO_INIT(Dao_Cache_Frontend_Data);
	DAO_INIT(Dao_Cache_Multiple);
	DAO_INIT(Dao_Cache_Backend_Apc);
	DAO_INIT(Dao_Cache_Backend_File);
	DAO_INIT(Dao_Cache_Backend_Memory);
#ifdef DAO_USE_MONGOC
	DAO_INIT(Dao_Cache_Backend_Mongo);
#endif
	DAO_INIT(Dao_Cache_Backend_Memcached);
	DAO_INIT(Dao_Cache_Backend_Redis);
	DAO_INIT(Dao_Cache_Backend_Yac);
#if DAO_USE_WIREDTIGER
	DAO_INIT(Dao_Cache_Backend_Wiredtiger);
#endif
#if DAO_USE_LMDB
	DAO_INIT(Dao_Cache_Backend_Lmdb);
#endif

	DAO_INIT(Dao_Cache_Frontend_Json);
	DAO_INIT(Dao_Cache_Frontend_Output);
	DAO_INIT(Dao_Cache_Frontend_None);
	DAO_INIT(Dao_Cache_Frontend_Base64);
	DAO_INIT(Dao_Cache_Frontend_Igbinary);
	DAO_INIT(Dao_Tag);
	DAO_INIT(Dao_Tag_Select);
	DAO_INIT(Dao_Paginator_Adapter);
	DAO_INIT(Dao_Paginator_Adapter_Model);
	DAO_INIT(Dao_Paginator_Adapter_NativeArray);
	DAO_INIT(Dao_Paginator_Adapter_QueryBuilder);
	DAO_INIT(Dao_Paginator_Adapter_DbBuilder);
	DAO_INIT(Dao_Paginator_Adapter_Sql);
	DAO_INIT(Dao_Validation);
	DAO_INIT(Dao_Validation_Validator);
	DAO_INIT(Dao_Validation_Message);
	DAO_INIT(Dao_Validation_Message_Group);
	DAO_INIT(Dao_Validation_Validator_Regex);
	DAO_INIT(Dao_Validation_Validator_Email);
	DAO_INIT(Dao_Validation_Validator_Between);
	DAO_INIT(Dao_Validation_Validator_Identical);
	DAO_INIT(Dao_Validation_Validator_PresenceOf);
	DAO_INIT(Dao_Validation_Validator_InclusionIn);
	DAO_INIT(Dao_Validation_Validator_StringLength);
	DAO_INIT(Dao_Validation_Validator_ExclusionIn);
	DAO_INIT(Dao_Validation_Validator_Confirmation);
	DAO_INIT(Dao_Validation_Validator_Url);
	DAO_INIT(Dao_Validation_Validator_File);
	DAO_INIT(Dao_Validation_Validator_Numericality);
	DAO_INIT(Dao_Validation_Validator_Json);
	DAO_INIT(Dao_Validation_Validator_Uniqueness);
	DAO_INIT(Dao_Validation_Validator_Alnum);
	DAO_INIT(Dao_Validation_Validator_Alpha);
	DAO_INIT(Dao_Validation_Validator_Digit);
	DAO_INIT(Dao_Validation_Validator_Date);
	DAO_INIT(Dao_Db);
	DAO_INIT(Dao_Db_Adapter);
	DAO_INIT(Dao_Db_Adapter_Pdo);
	DAO_INIT(Dao_Db_Adapter_Pdo_Sqlite);
	DAO_INIT(Dao_Db_Adapter_Pdo_Mysql);
	DAO_INIT(Dao_Db_Adapter_Pdo_Postgresql);
	DAO_INIT(Dao_Db_Index);
	DAO_INIT(Dao_Db_Column);
	DAO_INIT(Dao_Db_Dialect);
	DAO_INIT(Dao_Db_Dialect_Sqlite);
	DAO_INIT(Dao_Db_Dialect_Mysql);
	DAO_INIT(Dao_Db_Dialect_Postgresql);
	DAO_INIT(Dao_Db_Profiler);
	DAO_INIT(Dao_Db_Profiler_Item);
	DAO_INIT(Dao_Db_RawValue);
	DAO_INIT(Dao_Db_Reference);
	DAO_INIT(Dao_Db_Result_Pdo);
	DAO_INIT(Dao_Db_Builder);
	DAO_INIT(Dao_Db_Builder_Where);
	DAO_INIT(Dao_Db_Builder_Join);
	DAO_INIT(Dao_Db_Builder_Select);
	DAO_INIT(Dao_Db_Builder_Update);
	DAO_INIT(Dao_Db_Builder_Insert);
	DAO_INIT(Dao_Db_Builder_Delete);
	DAO_INIT(Dao_Kernel);
	DAO_INIT(Dao_Debug);
	DAO_INIT(Dao_Debug_Dump);
	DAO_INIT(Dao_Text);
	DAO_INIT(Dao_Date);
	DAO_INIT(Dao_Date_DateTime);
	DAO_INIT(Dao_Random);
	DAO_INIT(Dao_Security);
	DAO_INIT(Dao_Security_Random);
	DAO_INIT(Dao_Version);
	DAO_INIT(Dao_Session_Bag);
	DAO_INIT(Dao_Session_Adapter_Files);
	DAO_INIT(Dao_Session_Adapter_Memcached);
	DAO_INIT(Dao_Session_Adapter_Cache);
	DAO_INIT(Dao_Filter);
	DAO_INIT(Dao_Flash_Direct);
	DAO_INIT(Dao_Flash_Session);
	DAO_INIT(Dao_Annotations_Reader);
	DAO_INIT(Dao_Annotations_Annotation);
	DAO_INIT(Dao_Annotations_Adapter_Apc);
	DAO_INIT(Dao_Annotations_Collection);
	DAO_INIT(Dao_Annotations_Reflection);
	DAO_INIT(Dao_Annotations_Adapter_Files);
	DAO_INIT(Dao_Annotations_Adapter_Memory);
	DAO_INIT(Dao_Annotations_Adapter_Cache);
	DAO_INIT(Dao_Loader);
	DAO_INIT(Dao_Logger);
	DAO_INIT(Dao_Logger_Item);
	DAO_INIT(Dao_Logger_Multiple);
	DAO_INIT(Dao_Logger_Formatter_Json);
	DAO_INIT(Dao_Logger_Formatter_Line);
	DAO_INIT(Dao_Logger_Formatter_Firephp);
	DAO_INIT(Dao_Logger_Formatter_Syslog);
	DAO_INIT(Dao_Logger_Adapter_Stream);
	DAO_INIT(Dao_Logger_Adapter_File);
	DAO_INIT(Dao_Logger_Adapter_Firephp);
	DAO_INIT(Dao_Logger_Adapter_Syslog);
	DAO_INIT(Dao_Logger_Adapter_Direct);
	DAO_INIT(Dao_Forms_Form);
	DAO_INIT(Dao_Forms_Manager);
	DAO_INIT(Dao_Forms_Element_Text);
	DAO_INIT(Dao_Forms_Element_Date);
	DAO_INIT(Dao_Forms_Element_File);
	DAO_INIT(Dao_Forms_Element_Hidden);
	DAO_INIT(Dao_Forms_Element_Select);
	DAO_INIT(Dao_Forms_Element_Check);
	DAO_INIT(Dao_Forms_Element_Numeric);
	DAO_INIT(Dao_Forms_Element_Email);
	DAO_INIT(Dao_Forms_Element_Submit);
	DAO_INIT(Dao_Forms_Element_Password);
	DAO_INIT(Dao_Forms_Element_TextArea);
	DAO_INIT(Dao_Forms_Element_Radio);
	DAO_INIT(Dao_Crypt);
	DAO_INIT(Dao_Translate_Adapter_NativeArray);
	DAO_INIT(Dao_Translate_Adapter_Gettext);
	DAO_INIT(Dao_Translate_Adapter_Php);
	DAO_INIT(Dao_Escaper);
	DAO_INIT(Dao_Assets_Manager);
	DAO_INIT(Dao_Assets_Resource_Js);
	DAO_INIT(Dao_Assets_Collection);
	DAO_INIT(Dao_Assets_Filters_None);
	DAO_INIT(Dao_Assets_Filters_Cssmin);
	DAO_INIT(Dao_Assets_Filters_Jsmin);
	DAO_INIT(Dao_Assets_Resource_Css);
	DAO_INIT(Dao_Http_Parser);
	DAO_INIT(Dao_Http_Request);
	DAO_INIT(Dao_Http_Cookie);
	DAO_INIT(Dao_Http_Response);
	DAO_INIT(Dao_Http_Request_File);
	DAO_INIT(Dao_Http_Response_Cookies);
	DAO_INIT(Dao_Http_Response_Headers);
	DAO_INIT(Dao_Http_Uri);
	DAO_INIT(Dao_Http_Client);
	DAO_INIT(Dao_Http_Client_Header);
	DAO_INIT(Dao_Http_Client_Response);
	DAO_INIT(Dao_Http_Client_Adapter);
	DAO_INIT(Dao_Http_Client_Adapter_Curl);
	DAO_INIT(Dao_Http_Client_Adapter_Stream);
	DAO_INIT(Dao_Queue_Beanstalk);
	DAO_INIT(Dao_Queue_Beanstalk_Job);

	DAO_INIT(Dao_User_Module);
	DAO_INIT(Dao_User_Plugin);
	DAO_INIT(Dao_User_Component);
	DAO_INIT(Dao_User_Logic);

	DAO_INIT(Dao_Application);
	DAO_INIT(Dao_Router);
	DAO_INIT(Dao_Cli_Task);
	DAO_INIT(Dao_Cli_Router);
	DAO_INIT(Dao_Cli_Console);
	DAO_INIT(Dao_Cli_Dispatcher);
	DAO_INIT(Dao_Cli_Options);
	DAO_INIT(Dao_Cli_Color);
	DAO_INIT(Dao_Mvc_Router);
	DAO_INIT(Dao_Mvc_View_Engine);
	DAO_INIT(Dao_Mvc_View_Model);
	DAO_INIT(Dao_Mvc_View);
	DAO_INIT(Dao_Mvc_Url);
	DAO_INIT(Dao_Mvc_Micro);
	DAO_INIT(Dao_Mvc_Application);
	DAO_INIT(Dao_Mvc_Controller);
	DAO_INIT(Dao_Mvc_Dispatcher);
	DAO_INIT(Dao_Mvc_Model);
	DAO_INIT(Dao_Mvc_Model_Resultset);
	DAO_INIT(Dao_Mvc_Model_Behavior);
	DAO_INIT(Dao_Mvc_Model_Row);
	DAO_INIT(Dao_Mvc_Model_Query);
	DAO_INIT(Dao_Mvc_Micro_Collection);
	DAO_INIT(Dao_Mvc_Micro_LazyLoader);
	DAO_INIT(Dao_Mvc_Model_Criteria);
	DAO_INIT(Dao_Mvc_Model_Manager);
	DAO_INIT(Dao_Mvc_Model_Relation);
	DAO_INIT(Dao_Mvc_Model_Query_Lang);
	DAO_INIT(Dao_Mvc_Model_Query_Status);
	DAO_INIT(Dao_Mvc_Model_Query_Builder);
	DAO_INIT(Dao_Mvc_Model_Query_Builder_Where);
	DAO_INIT(Dao_Mvc_Model_Query_Builder_Join);
	DAO_INIT(Dao_Mvc_Model_Query_Builder_Select);
	DAO_INIT(Dao_Mvc_Model_Query_Builder_Update);
	DAO_INIT(Dao_Mvc_Model_Query_Builder_Insert);
	DAO_INIT(Dao_Mvc_Model_Query_Builder_Delete);
	DAO_INIT(Dao_Mvc_Model_ValidationFailed);
	DAO_INIT(Dao_Mvc_Model_Resultset_Simple);
	DAO_INIT(Dao_Mvc_Model_Resultset_Complex);
	DAO_INIT(Dao_Mvc_Model_MetaData);
	DAO_INIT(Dao_Mvc_Model_MetaData_Apc);
	DAO_INIT(Dao_Mvc_Model_MetaData_Files);
	DAO_INIT(Dao_Mvc_Model_MetaData_Session);
	DAO_INIT(Dao_Mvc_Model_MetaData_Memcached);
	DAO_INIT(Dao_Mvc_Model_MetaData_Redis);
#if DAO_USE_MONGOC
	DAO_INIT(Dao_Mvc_Model_MetaData_Mongo);
#endif
	DAO_INIT(Dao_Mvc_Model_MetaData_Cache);
	DAO_INIT(Dao_Mvc_Model_MetaData_Memory);
	DAO_INIT(Dao_Mvc_Model_MetaData_Strategy_Annotations);
	DAO_INIT(Dao_Mvc_Model_MetaData_Strategy_Introspection);
	DAO_INIT(Dao_Mvc_Model_Transaction);
	DAO_INIT(Dao_Mvc_Model_Transaction_Manager);
	DAO_INIT(Dao_Mvc_Model_Transaction_Failed);
	DAO_INIT(Dao_Mvc_Model_Behavior_SoftDelete);
	DAO_INIT(Dao_Mvc_Model_Behavior_Timestampable);
	DAO_INIT(Dao_Mvc_Router_Route);
	DAO_INIT(Dao_Mvc_Router_Group);
	DAO_INIT(Dao_Mvc_Router_Annotations);
	DAO_INIT(Dao_Mvc_User_Module);
	DAO_INIT(Dao_Mvc_User_Plugin);
	DAO_INIT(Dao_Mvc_User_Component);
	DAO_INIT(Dao_Mvc_User_Logic);
	DAO_INIT(Dao_Mvc_User_Logic_Model);
	DAO_INIT(Dao_Mvc_View_Simple);
	DAO_INIT(Dao_Mvc_View_Engine_Php);
	DAO_INIT(Dao_Events_Event);
	DAO_INIT(Dao_Events_Manager);
	DAO_INIT(Dao_Events_Listener);
	DAO_INIT(Dao_Image);

	DAO_INIT(Dao_Image_Adapter);
	DAO_INIT(Dao_Image_Adapter_GD);
	DAO_INIT(Dao_Image_Adapter_Imagick);
	DAO_INIT(Dao_Registry);
	DAO_INIT(Dao_Arr);
	DAO_INIT(Dao_Matrix);

	DAO_INIT(Dao_Async);

#if DAO_USE_SHM_OPEN
	DAO_INIT(Dao_Sync_Mutex);
	DAO_INIT(Dao_Sync_Readerwriter);
	DAO_INIT(Dao_Sync_Event);
	DAO_INIT(Dao_Sync_Semaphore);
	DAO_INIT(Dao_Sync_Sharedmemory);
#endif

	DAO_INIT(Dao_Binary);
	DAO_INIT(Dao_Binary_Reader);
	DAO_INIT(Dao_Binary_Writer);

	DAO_INIT(Dao_Storage_FrontendInterface);
	DAO_INIT(Dao_Storage_Frontend_Json);
	DAO_INIT(Dao_Storage_Frontend_Base64);
	DAO_INIT(Dao_Storage_Frontend_Igbinary);

#ifdef DAO_STORAGE_BTREE
	DAO_INIT(Dao_Storage_Btree);
#endif

#if DAO_USE_WIREDTIGER
	DAO_INIT(Dao_Storage_Wiredtiger);
	DAO_INIT(Dao_Storage_Wiredtiger_Cursor);
#endif

#if DAO_USE_LMDB
	DAO_INIT(Dao_Storage_Lmdb);
	DAO_INIT(Dao_Storage_Lmdb_Cursor);
#endif
#if DAO_USE_LIBMDBX
	DAO_INIT(Dao_Storage_Libmdbx);
	DAO_INIT(Dao_Storage_Libmdbx_Cursor);
#endif

#if DAO_USE_LEVELDB
	DAO_INIT(Dao_Storage_Leveldb);
	DAO_INIT(Dao_Storage_Leveldb_Iterator);
	DAO_INIT(Dao_Storage_Leveldb_Writebatch);
#endif

#if DAO_USE_BLOOMFILTER
	DAO_INIT(Dao_Storage_Bloomfilter);
# ifdef ZEND_ENABLE_ZVAL_LONG64
	DAO_INIT(Dao_Storage_Bloomfilter_Counting);
# endif
#endif

#ifdef DAO_USE_DATRIE
	DAO_INIT(Dao_Storage_Datrie);
#endif

	DAO_INIT(Dao_Snowflake);

#if DAO_USE_PYTHON
	DAO_INIT(Dao_Py);
	DAO_INIT(Dao_Py_Object);
	DAO_INIT(Dao_Py_Matplot);
#endif

	DAO_INIT(Dao_Aop);
	DAO_INIT(Dao_Aop_Joinpoint);

#ifdef DAO_JWT
	DAO_INIT(Dao_JWT);
#endif

	return SUCCESS;
}

static PHP_MSHUTDOWN_FUNCTION(dao){

	dao_deinitialize_memory();

	assert(DAO_GLOBAL(orm).ast_cache == NULL);

#ifdef DAO_USE_MONGOC
	mongoc_cleanup();
#endif

#if DAO_USE_PYTHON
	PyEval_RestoreThread(DAO_GLOBAL(python).mtstate);
	Py_Finalize();
#endif

#if DAO_USE_ASYNC
	async_channel_ce_unregister();
	async_deferred_ce_unregister();
	async_dns_ce_unregister();
	async_monitor_ce_unregister();
	async_ssl_ce_unregister();
	async_tcp_ce_unregister();
	async_udp_socket_ce_unregister();

	async_task_ce_unregister();
	async_thread_ce_unregister();
#endif

	UNREGISTER_INI_ENTRIES();

#if DAO_USE_ASYNC
	zend_execute_ex = async_orig_execute_ex;

	if (ASYNC_G(cli)) {
		uv_tty_reset_mode();
	}
#endif

	return SUCCESS;
}

static PHP_RINIT_FUNCTION(dao)
{
#if defined(ZTS) && defined(COMPILE_DL_DAO)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	zend_dao_globals *dao_globals_ptr = DAO_VGLOBAL;

	php_dao_init_globals(dao_globals_ptr);
	dao_init_interned_strings();

	dao_initialize_memory(dao_globals_ptr);

	if (DAO_GLOBAL(aop).enable_aop) {
		DAO_GLOBAL(aop).overloaded = 0;
		DAO_GLOBAL(aop).pointcut_version = 0;

		DAO_GLOBAL(aop).object_cache_size = 1024;
		DAO_GLOBAL(aop).object_cache = ecalloc(1024, sizeof(dao_aop_object_cache*));

		DAO_GLOBAL(aop).property_value = NULL;

		DAO_GLOBAL(aop).lock_read_property = 0;
		DAO_GLOBAL(aop).lock_write_property = 0;

		ALLOC_HASHTABLE(DAO_GLOBAL(aop).pointcuts_table);
		zend_hash_init(DAO_GLOBAL(aop).pointcuts_table, 16, NULL, dao_aop_free_pointcut, 0);

		ALLOC_HASHTABLE(DAO_GLOBAL(aop).function_cache);
		zend_hash_init(DAO_GLOBAL(aop).function_cache, 16, NULL, dao_aop_free_pointcut_cache, 0);
	}

	if (DAO_GLOBAL(xhprof).enable_xhprof) {
		tracing_request_init();
		tracing_determine_clock_source();
	}

#if DAO_USE_PYTHON
	DAO_GLOBAL(python).tstate = interpreter_python_init_thread();
#endif

#if DAO_USE_ASYNC
	async_context_init();
	async_task_scheduler_init();
	async_helper_init();

	if (ASYNC_G(dns_enabled)) {
		async_dns_init();
	}
	
	if (ASYNC_G(timer_enabled)) {
		async_timer_init();
	}
	
	async_filesystem_init();
	async_tcp_socket_init();
	async_udp_socket_init();
	async_unix_socket_init();
#endif
	return SUCCESS;
}

static PHP_RSHUTDOWN_FUNCTION(dao){

	if (DAO_GLOBAL(aop).enable_aop) {
		int i;
		zend_array_destroy(DAO_GLOBAL(aop).pointcuts_table);
		zend_array_destroy(DAO_GLOBAL(aop).function_cache);
		for (i = 0; i < DAO_GLOBAL(aop).object_cache_size; i++) {
			if (DAO_GLOBAL(aop).object_cache[i] != NULL) {
				dao_aop_object_cache *_cache = DAO_GLOBAL(aop).object_cache[i];
				if (_cache->write!=NULL) {
					zend_hash_destroy(_cache->write);
					FREE_HASHTABLE(_cache->write);
				}
				if (_cache->read!=NULL) {
					zend_hash_destroy(_cache->read);
					FREE_HASHTABLE(_cache->read);
				}
				if (_cache->func!=NULL) {
					zend_hash_destroy(_cache->func);
					FREE_HASHTABLE(_cache->func);
				}
				efree(_cache);
			}
		}
		efree(DAO_GLOBAL(aop).object_cache);

		if (DAO_GLOBAL(aop).property_value != NULL) {
			zval_ptr_dtor(DAO_GLOBAL(aop).property_value);
			efree(DAO_GLOBAL(aop).property_value);
		}
	}

	if (DAO_GLOBAL(xhprof).enable_xhprof) {
		int i = 0;
		xhprof_callgraph_bucket *bucket;

		tracing_end();

		for (i = 0; i < DAO_XHPROF_CALLGRAPH_SLOTS; i++) {
			bucket = DAO_GLOBAL(xhprof).callgraph_buckets[i];

			while (bucket) {
				DAO_GLOBAL(xhprof).callgraph_buckets[i] = bucket->next;
				tracing_callgraph_bucket_free(bucket);
				bucket = DAO_GLOBAL(xhprof).callgraph_buckets[i];
			}
		}

		tracing_request_shutdown();
	}

	dao_deinitialize_memory();
	dao_release_interned_strings();

#if DAO_USE_PYTHON
	interpreter_python_shutdown_thread(DAO_GLOBAL(python).tstate);
#endif

#if DAO_USE_ASYNC
	if (ASYNC_G(dns_enabled)) {
		async_dns_shutdown();
	}

	if (ASYNC_G(timer_enabled)) {
		async_timer_shutdown();
	}

	async_filesystem_shutdown();
	async_tcp_socket_shutdown();
	async_udp_socket_shutdown();
	async_unix_socket_shutdown();

	async_task_scheduler_shutdown();
	async_context_shutdown();
#endif
	return SUCCESS;
}

static PHP_MINFO_FUNCTION(dao)
{
#ifdef DAO_USE_ASYNC
	char uv_version[20];
#endif
	php_info_print_table_start();
	php_info_print_table_row(2, "Dao Framework", "enabled");
	php_info_print_table_row(2, "Dao Version", PHP_DAO_VERSION);
	php_info_print_table_row(2, "Build Date", __DATE__ " " __TIME__ );
#ifdef DAO_USE_ASYNC
	php_info_print_table_row(2, "Async", "enabled");
	sprintf(uv_version, "%d.%d", UV_VERSION_MAJOR, UV_VERSION_MINOR);
	php_info_print_table_row(2, "Libuv version", uv_version);
#endif

	if (DAO_GLOBAL(aop).enable_aop) {
		php_info_print_table_header(2, "AOP support", "enabled");
#if DAO_USE_AOP_PROPERTY
		if (DAO_GLOBAL(aop).enable_property) {
			php_info_print_table_header(2, "AOP property support", "enabled");
		} else {
			php_info_print_table_header(2, "AOP property support", "disabled");
		}
#endif
	}

	if (DAO_GLOBAL(xhprof).enable_xhprof) {
		php_info_print_table_row(2, "Xhprof", "enabled");
		php_info_print_table_row(2, "Function Nesting Limit", "enabled");
		switch (DAO_GLOBAL(xhprof).clock_source) {
			case DAO_CLOCK_TSC:
				php_info_print_table_row(2, "Clock Source", "tsc");
				break;
			case DAO_CLOCK_CGT:
				php_info_print_table_row(2, "Clock Source", "clock_gettime");
				break;
			case DAO_CLOCK_GTOD:
				php_info_print_table_row(2, "Clock Source", "gettimeofday");
				break;
			case DAO_CLOCK_MACH:
				php_info_print_table_row(2, "Clock Source", "mach");
				break;
			case DAO_CLOCK_QPC:
				php_info_print_table_row(2, "Clock Source", "Query Performance Counter");
				break;
			case DAO_CLOCK_NONE:
				php_info_print_table_row(2, "Clock Source", "none");
				break;
		}
	}

#ifdef DAO_USE_MONGOC
	php_info_print_table_row(2, "Cache Backend Mongo", "enabled");
#endif

#if DAO_USE_SHM_OPEN
	php_info_print_table_row(2, "Sync", "enabled");
#endif

#ifdef DAO_STORAGE_BTREE
	php_info_print_table_row(2, "Storage B+Tree", "enabled");
#endif

#if DAO_USE_WIREDTIGER
	php_info_print_table_row(2, "Storage Wiredtiger", "enabled");
#endif

#if DAO_USE_LEVELDB
	php_info_print_table_row(2, "Storage Leveldb", "enabled");
#endif

#if DAO_USE_LMDB
	php_info_print_table_row(2, "Storage Lmdb", "enabled");
#endif

#if DAO_USE_LIBMDBX
	php_info_print_table_row(2, "Storage Libmdbx", "enabled");
#endif

#if DAO_USE_BLOOMFILTER
	php_info_print_table_row(2, "Storage Bloomfilter", "enabled");
#endif

#if DAO_USE_DATRIE
	php_info_print_table_row(2, "Storage Datrie", "enabled");
#endif

#if DAO_USE_PYTHON
	php_info_print_table_row(2, "Python", "enabled");
#endif

	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();
}

static PHP_GINIT_FUNCTION(dao)
{
#if defined(ZTS) && defined(COMPILE_DL_DAO)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	memset(dao_globals, 0, sizeof(zend_dao_globals));
	php_dao_init_globals(dao_globals);

	dao_globals->xhprof.root = NULL;
	dao_globals->xhprof.callgraph_frames = NULL;
	dao_globals->xhprof.frame_free_list = NULL;
	dao_globals->xhprof.nesting_current_level = -1;
	dao_globals->xhprof.nesting_maximum_level =  0;

	dao_globals->snowflake.epoch = 1420864633000ULL;

#ifdef DAO_JWT
    dao_globals->jwt.expiration = 0;
    dao_globals->jwt.not_before = 0;
    dao_globals->jwt.iss = NULL;
    dao_globals->jwt.iat = 0;
    dao_globals->jwt.jti = NULL;
    dao_globals->jwt.aud = NULL;
    dao_globals->jwt.sub = NULL;
    dao_globals->jwt.leeway = 0;
    dao_globals->jwt.algorithm = "HS256";
#endif
}

static PHP_GSHUTDOWN_FUNCTION(dao)
{
	dao_deinitialize_memory();
}

static const zend_module_dep dao_deps[] = {
	ZEND_MOD_REQUIRED("spl")
	ZEND_MOD_REQUIRED("date")
#if DAO_JWT
	ZEND_MOD_REQUIRED("json")
#elif DAO_USE_PHP_JSON
	ZEND_MOD_REQUIRED("json")
#else
	ZEND_MOD_OPTIONAL("json")
#endif
#if DAO_USE_PHP_SESSION
	ZEND_MOD_REQUIRED("session")
#else
	ZEND_MOD_OPTIONAL("session")
#endif
#if DAO_USE_PHP_PCRE
	ZEND_MOD_REQUIRED("pcre")
#else
	ZEND_MOD_OPTIONAL("pcre")
#endif
#if DAO_USE_PHP_HASH
	ZEND_MOD_REQUIRED("hash")
#else
	ZEND_MOD_OPTIONAL("hash")
#endif

	ZEND_MOD_OPTIONAL("sockets")

#if DAO_USE_PHP_MBSTRING
	ZEND_MOD_REQUIRED("mbstring")
#else
	ZEND_MOD_OPTIONAL("mbstring")
#endif
	ZEND_MOD_OPTIONAL("apc")
	ZEND_MOD_OPTIONAL("apcu")
	ZEND_MOD_OPTIONAL("memcached")
	ZEND_MOD_OPTIONAL("filter")
	ZEND_MOD_OPTIONAL("iconv")
	ZEND_MOD_OPTIONAL("libxml")
	ZEND_MOD_OPTIONAL("mbstring")
	ZEND_MOD_OPTIONAL("openssl")
	ZEND_MOD_OPTIONAL("pdo")
	ZEND_MOD_OPTIONAL("gd")
	ZEND_MOD_OPTIONAL("imagick")
	ZEND_MOD_END
};

zend_module_entry dao_module_entry = {
	STANDARD_MODULE_HEADER_EX,
	NULL,
	dao_deps,
	PHP_DAO_EXTNAME,
	NULL,
	PHP_MINIT(dao),
	PHP_MSHUTDOWN(dao),
	PHP_RINIT(dao),
	PHP_RSHUTDOWN(dao),
	PHP_MINFO(dao),
	PHP_DAO_VERSION,
	ZEND_MODULE_GLOBALS(dao),
	PHP_GINIT(dao),
	PHP_GSHUTDOWN(dao),
	NULL,
	STANDARD_MODULE_PROPERTIES_EX
};

#ifdef COMPILE_DL_DAO
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(dao)
#endif
