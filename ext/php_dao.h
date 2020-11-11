
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

#ifndef PHP_DAO_H
#define PHP_DAO_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#if DAO_USE_PYTHON
#include <Python.h>
#endif

#include <main/php.h>
#ifdef ZTS
#include <TSRM/TSRM.h>
#endif

#define PHP_DAO_VERSION "Dao-1.0.0"
#define PHP_DAO_VERSION_MAJOR           1
#define PHP_DAO_VERSION_MED             0
#define PHP_DAO_VERSION_MIN             0
#define PHP_DAO_VERSION_RELEASE         DAO_VERSION_BETA
#define PHP_DAO_VERSION_RELEASE_VERSION 1
#define PHP_DAO_EXTNAME "Dao"

/** DEBUG options */
typedef struct _dao_debug_options {
	int debug_level;
	zend_bool enable_debug;
} dao_debug_options;

/** MVC options */
typedef struct _dao_mvc_options {
	zend_bool enable_view_strict;
} dao_mvc_options;

/** ORM options */
typedef struct _dao_orm_options {
	HashTable *ast_cache;
	int cache_level;
	zend_bool events;
	zend_bool virtual_foreign_keys;
	zend_bool not_null_validations;
	zend_bool length_validations;
	zend_bool use_mb_strlen;
	zend_bool exception_on_failed_save;
	zend_bool enable_literals;
	zend_bool enable_ast_cache;
	zend_bool enable_property_method;
	zend_bool enable_auto_convert;
	zend_bool allow_update_primary;
	zend_bool enable_strict;
	zend_bool must_column;
} dao_orm_options;

/** Validation options */
typedef struct _dao_validation_options {
	zend_bool allow_empty;
} dao_validation_options;

/** Security options */
typedef struct _dao_security_options {
	zend_bool crypt_std_des_supported;
	zend_bool crypt_ext_des_supported;
	zend_bool crypt_md5_supported;
	zend_bool crypt_blowfish_supported;
	zend_bool crypt_blowfish_y_supported;
	zend_bool crypt_sha256_supported;
	zend_bool crypt_sha512_supported;
} dao_security_options;

/** DB options */
typedef struct _dao_db_options {
	zend_bool escape_identifiers;
} dao_db_options;

/** Cache options */
typedef struct _dao_cache_options {
	zend_bool enable_yac;
	zend_bool enable_yac_cli;
	size_t yac_keys_size;
	size_t yac_values_size;
} dao_cache_options;

/** Xhprof options */

#define DAO_XHPROF_CALLGRAPH_COUNTER_SIZE 1024
#define DAO_XHPROF_CALLGRAPH_SLOTS 8192

#if !defined(uint32)
typedef unsigned int uint32;
#endif

#if !defined(uint64)
typedef unsigned long long uint64;
#endif

typedef struct xhprof_frame_t xhprof_frame_t;

typedef struct xhprof_callgraph_bucket_t {
    zend_ulong key;
    zend_string *parent_class;
    zend_string *parent_function;
    int parent_recurse_level;
    zend_string *child_class;
    zend_string *child_function;
    int child_recurse_level;
    struct xhprof_callgraph_bucket_t *next;
    zend_long count;
    zend_long wall_time;
    zend_long cpu_time;
    zend_long memory;
    zend_long memory_peak;
	long int num_alloc, num_free;
	long int amount_alloc;
} xhprof_callgraph_bucket;

/* Tracer maintains a stack of entries being profiled.
 *
 * This structure is a convenient place to track start time of a particular
 * profile operation, recursion depth, and the name of the function being
 * profiled. */
struct xhprof_frame_t {
    struct xhprof_frame_t   *previous_frame;        /* ptr to prev entry being profiled */
    zend_string         *function_name;
    zend_string         *class_name;
    uint64              wt_start;           /* start value for wall clock timer */
    uint64              cpu_start;         /* start value for CPU clock timer */
    long int            mu_start;                    /* memory usage */
    long int            pmu_start;              /* peak memory usage */
	long int            num_alloc, num_free;
	long int            amount_alloc;
    int                 recurse_level;
    zend_ulong          hash_code;          /* hash_code for the function name  */
};

typedef struct _dao_xhprof_options {
	zend_bool enable_xhprof;
    int enabled;
    uint64 start_timestamp;
    uint64 start_time;
    int clock_source;
    zend_bool clock_use_rdtsc;
    double timebase_factor;
    zend_string *root;
    xhprof_frame_t *callgraph_frames;
    xhprof_frame_t *frame_free_list;
    zend_ulong function_hash_counters[DAO_XHPROF_CALLGRAPH_COUNTER_SIZE];
    xhprof_callgraph_bucket* callgraph_buckets[DAO_XHPROF_CALLGRAPH_SLOTS];
    zend_long flags;
    long int num_alloc;
    long int num_free;
	long int amount_alloc;
	long int nesting_current_level;
	long int nesting_maximum_level;
} dao_xhprof_options;

#if DAO_USE_PYTHON
/** Python options */
typedef struct _dao_python_options {
    zend_bool isInitialized;
    PyThreadState *mtstate;
    PyThreadState *tstate;
} dao_python_options;
#endif

/** Snowflake options */
typedef struct _dao_snowflake_options {
	uint32_t node;
	uint64_t epoch;
} dao_snowflake_options;

/** AOP options */
typedef struct {
    zend_array *read;
    zend_array *write;
    zend_array *func;
} dao_aop_object_cache;

typedef struct _dao_aop_options {
	zend_bool enable_aop;
#if DAO_USE_AOP_PROPERTY
	zend_bool enable_property;
#endif
	zend_array *pointcuts_table;
	int pointcut_version;
	int overloaded;

	zend_array *function_cache;

	dao_aop_object_cache **object_cache;
	int object_cache_size;

	int lock_read_property;
	int lock_write_property;

	zval *property_value;
} dao_aop_options;

#if DAO_USE_ASYNC

typedef struct _async_cancel_cb                     async_cancel_cb;
typedef struct _async_cancellation_handler          async_cancellation_handler;
typedef struct _async_context                       async_context;
typedef struct _async_context_cancellation          async_context_cancellation;
typedef struct _async_context_timeout               async_context_timeout;
typedef struct _async_context_var                   async_context_var;
typedef struct _async_fiber                         async_fiber;
typedef struct _async_op                            async_op;
typedef struct _async_task                          async_task;
typedef struct _async_task_scheduler                async_task_scheduler;
typedef struct _async_tick_event                    async_tick_event;

typedef struct _dao_async_options {
	zend_long nc;

	/* Root fiber context used by the VM. */
	async_fiber *root;
	
	/* Root task scheduler used by the VM. */
	async_task_scheduler *executor;
	
	/* Reference to the running fiber (might be root). */
	async_fiber *fiber;

	/* Running task scheduler. */
	async_task_scheduler *scheduler;

	/* Call trace that triggered running the current scheduler. */
	zend_execute_data *exec;

	/* Current task being run. */
	async_task *task;
	
	/* Current async context. */
	async_context *context;

	/* Root context for all foreground contexts. */
	async_context *foreground;
	
	/* Root context for all background contexts. */
	async_context *background;

	/* Is set when the SAPI is cli. */
	zend_bool cli;

	/* Will be populated when bailout is requested. */
	zend_bool exit;
	
	/* Point to the active thread (NULL when not running within worker thread). */
	zend_object *thread;
	
	/* Holds a dummy awaitable object. */
	zend_object *awaitable;

	HashTable *factories;

	/* INI settings. */
	zend_bool dns_enabled;
	zend_bool forked;
	zend_bool fs_enabled;
	zend_long stack_size;
	zend_bool tcp_enabled;
	zend_long threads;
	zend_bool timer_enabled;
	zend_bool udp_enabled;
	zend_bool unix_enabled;
} dao_async_options;
#endif

/** JWT options */
typedef struct _dao_jwt_options {
  time_t expiration;
  time_t not_before;
  char *iss;
  time_t iat;
  char *jti;
  zval *aud;
  char *sub;
  size_t leeway;
  char *algorithm;
} dao_jwt_options;

ZEND_BEGIN_MODULE_GLOBALS(dao)

	/* Controls double initialization of memory frames */
	int initialized;

	/** Frequently used zvals */
	zval z_null;
	zval z_true;
	zval z_false;
	zval z_zero;
	zval z_one;
	zval z_two;

	/** DEBUG */
	dao_debug_options debug;

	/** MVC */
	dao_mvc_options mvc;

	/** ORM */
	dao_orm_options orm;

	/** Validation */
	dao_validation_options validation;

	/** Max recursion control */
	unsigned int recursive_lock;

	/** Security */
	dao_security_options security;

	/** DB */
	dao_db_options db;

	/** Cache */
	dao_cache_options cache;

	/** Xhprof */
	dao_xhprof_options xhprof;

#if DAO_USE_PYTHON
	/** Python */
	dao_python_options python;
#endif

	dao_snowflake_options snowflake;

	dao_aop_options aop;

#if DAO_USE_ASYNC
	/** Async */
	dao_async_options async;
#endif

	dao_jwt_options jwt;

ZEND_END_MODULE_GLOBALS(dao)


ZEND_EXTERN_MODULE_GLOBALS(dao)

#ifdef ZTS
	#define DAO_GLOBAL(v) TSRMG(dao_globals_id, zend_dao_globals *, v)
	#define DAO_VGLOBAL   ((zend_dao_globals *) (*((void ***) tsrm_get_ls_cache()))[TSRM_UNSHUFFLE_RSRC_ID(dao_globals_id)])
#else
	#define DAO_GLOBAL(v) (dao_globals.v)
	#define DAO_VGLOBAL &(dao_globals)
#endif

#define TXRG(v) (DAO_GLOBAL(xhprof).v)

#define JWT_G(v) (DAO_GLOBAL(jwt).v)

#ifdef DAO_USE_ASYNC
#define ASYNC_G(v) (DAO_GLOBAL(async).v)
#endif

#if defined(ZTS) && defined(COMPILE_DL_DAO)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

extern zend_module_entry dao_module_entry;
#define phpext_dao_ptr &dao_module_entry

#define INIT_ZVAL(z) z = EG(uninitialized_zval);

#define DAO_ALLOC_ZVAL(z) \
	(z) = (zval *) emalloc(sizeof(zval)); \

#define DAO_ALLOC_INIT_ZVAL(z) \
	DAO_ALLOC_ZVAL(z); \

#ifndef INIT_PZVAL
#	define INIT_PZVAL(z) \
		Z_SET_REFCOUNT_P(z, 1); \
		ZVAL_UNREF(z);
#endif

#ifndef INIT_PZVAL_COPY
#	define INIT_PZVAL_COPY(z, v) \
		ZVAL_COPY_VALUE(z, v);   \
		Z_SET_REFCOUNT_P(z, 1);  \
		ZVAL_UNREF(z);
#endif

#ifndef ZVAL_COPY_VALUE
#	define ZVAL_COPY_VALUE(z, v) \
		(z)->value  = (v)->value; \
		Z_TYPE_P(z) = Z_TYPE_P(v);
#endif

#define DAO_INIT_CLASS(name) \
	int dao_ ##name## _init(int module_number)

#define DAO_INIT(name) \
	if (dao_ ##name## _init(module_number) == FAILURE) { \
		return FAILURE; \
	}

/** Macros for branch prediction */
#define likely(x)       EXPECTED(x)
#define unlikely(x)     UNEXPECTED(x)

#if defined(__GNUC__) && (defined(__clang__) || ((__GNUC__ * 100 + __GNUC_MINOR__) >= 405))
#	define UNREACHABLE() __builtin_unreachable()
#	define ASSUME(x)     if (x) {} else __builtin_unreachable()
#else
#	define UNREACHABLE() assert(0)
#	define ASSUME(x)     assert(!!(x));
#endif

#if defined(__GNUC__) || defined(__clang__)
#	define DAO_ATTR_NONNULL            __attribute__((nonnull))
#	define DAO_ATTR_NONNULL1(x)        __attribute__((nonnull (x)))
#	define DAO_ATTR_NONNULL2(x, y)     __attribute__((nonnull (x, y)))
#	define DAO_ATTR_NONNULL3(x, y, z)  __attribute__((nonnull (x, y, z)))
#	define DAO_ATTR_PURE               __attribute__((pure))
#	define DAO_ATTR_CONST              __attribute__((const))
#	define DAO_ATTR_WARN_UNUSED_RESULT __attribute__((warn_unused_result))
#else
#	define DAO_ATTR_NONNULL
#	define DAO_ATTR_NONNULL1(x)
#	define DAO_ATTR_NONNULL2(x, y)
#	define DAO_ATTR_NONNULL3(x, y, z)
#	define DAO_ATTR_PURE
#	define DAO_ATTR_CONST
#	define DAO_ATTR_WARN_UNUSED_RESULT
#endif

#if !defined(__GNUC__) && !(defined(__SUNPRO_C) && (__SUNPRO_C >= 0x590))
#	define __builtin_constant_p(s)    (0)
#endif

#ifndef ZEND_MOD_END
#	define ZEND_MOD_END { NULL, NULL, NULL, 0 }
#endif

#ifndef __func__
#	define __func__ __FUNCTION__
#endif

#define DAO_STATIC

/* This is a temporary fix until config.w32 is updated */
#if !defined(__CYGWIN__) && defined(WIN32) && defined(HAVE_CONFIG_H)

#	if defined(HAVE_JSON) && !defined(DAO_USE_PHP_JSON)
#		define DAO_USE_PHP_JSON 1
#	endif

#	if defined(HAVE_BUNDLED_PCRE) && !defined(DAO_USE_PHP_PCRE)
#		define DAO_USE_PHP_PCRE 1
#	endif

#	if defined(HAVE_PHP_SESSION) && !defined(DAO_USE_PHP_SESSION)
#		define DAO_USE_PHP_SESSION 1
#	endif

#	if defined(HAVE_HASH_EXT) && !defined(DAO_USE_PHP_HASH)
#		define DAO_USE_PHP_HASH 1
#	endif

#endif /* !defined(__CYGWIN__) && !defined(WIN32) && defined(HAVE_CONFIG_H) */

#if !defined(__CYGWIN__) && defined(WIN32)
double round(double num) {
	return (num > 0.0) ? floor(num + 0.5) : ceil(num - 0.5);
}
#endif

#endif /* PHP_DAO_H */
