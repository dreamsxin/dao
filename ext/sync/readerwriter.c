
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

#include "sync/readerwriter.h"
#include "sync/exception.h"

#include "kernel/main.h"
#include "kernel/object.h"
#include "kernel/array.h"
#include "kernel/string.h"
#include "kernel/fcall.h"
#include "kernel/concat.h"
#include "kernel/operators.h"
#include "kernel/exception.h"
#include "kernel/time.h"

/**
 * Dao\Sync\Readerwriter
 *
 */
zend_class_entry *dao_sync_readerwriter_ce;

PHP_METHOD(Dao_Sync_Readerwriter, __construct);
PHP_METHOD(Dao_Sync_Readerwriter, readlock);
PHP_METHOD(Dao_Sync_Readerwriter, readunlock);
PHP_METHOD(Dao_Sync_Readerwriter, writelock);
PHP_METHOD(Dao_Sync_Readerwriter, writeunlock);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_sync_readerwriter___construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 1)
	ZEND_ARG_TYPE_INFO(0, autounlock, _IS_BOOL, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_sync_readerwriter_readlock, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, wait, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_sync_readerwriter_writelock, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, wait, _IS_BOOL, 1)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_sync_readerwriter_method_entry[] = {
	PHP_ME(Dao_Sync_Readerwriter, __construct, arginfo_dao_sync_readerwriter___construct, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Sync_Readerwriter, readlock, arginfo_dao_sync_readerwriter_readlock, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Sync_Readerwriter, readunlock, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Sync_Readerwriter, writelock, arginfo_dao_sync_readerwriter_writelock, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Sync_Readerwriter, writeunlock, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

zend_object_handlers dao_sync_readerwriter_object_handlers;
zend_object* dao_sync_readerwriter_object_create_handler(zend_class_entry *ce)
{
	dao_sync_readerwriter_object *intern = ecalloc(1, sizeof(dao_sync_readerwriter_object) + zend_object_properties_size(ce));
	intern->std.ce = ce;

	zend_object_std_init(&intern->std, ce);
	object_properties_init(&intern->std, ce);
	intern->std.handlers = &dao_sync_readerwriter_object_handlers;

	intern->MxNamed = 0;
	intern->MxMem = NULL;
	intern->MxRCount = NULL;

	intern->MxAutoUnlock = 1;
	intern->MxReadLocks = 0;
	intern->MxWriteLock = 0;

	return &intern->std;
}

void dao_sync_readerwriter_object_free_handler(zend_object *object)
{
	dao_sync_readerwriter_object *intern = dao_sync_readerwriter_object_from_obj(object);

	if (intern->MxAutoUnlock) {
		while (intern->MxReadLocks) {
			dao_readerwriter_readunlock_internal(intern);
		}

		if (intern->MxWriteLock) dao_readerwriter_writeunlock_internal(intern);
	}
	
	if (intern->MxMem != NULL) {
		if (intern->MxNamed) {
			dao_namedmem_unmap(intern->MxMem, dao_semaphore_getsize() + dao_getalignsize(sizeof(uint32_t)) + dao_event_getsize() + dao_semaphore_getsize());
		} else {
			dao_semaphore_free(&intern->MxPthreadRCountMutex);
			dao_event_free(&intern->MxPthreadRWaitEvent);
			dao_semaphore_free(&intern->MxPthreadWWaitMutex);

			efree(intern->MxMem);
		}
	}
}

/**
 * Dao\Sync\Readerwriter initializer
 */
DAO_INIT_CLASS(Dao_Sync_Readerwriter){

	DAO_REGISTER_CLASS_CREATE_OBJECT(Dao\\Sync, Readerwriter, sync_readerwriter, dao_sync_readerwriter_method_entry, 0);

	return SUCCESS;
}

/**
 * Dao\Sync\Readerwriter constructor
 *
 * @param string $name
 * @param boolean $autounlock
 */
PHP_METHOD(Dao_Sync_Readerwriter, __construct){

	zval *name = NULL, *autounlock = NULL;
	size_t Pos, TempSize;
	dao_sync_readerwriter_object *intern;
	int Result;
	char *MemPtr;

	dao_fetch_params(0, 0, 2, &name, &autounlock);

	if (!autounlock) {
		autounlock = &DAO_GLOBAL(z_true);
	}

	intern = dao_sync_readerwriter_object_from_obj(Z_OBJ_P(getThis()));

	if (!name || DAO_IS_EMPTY(name)) {
		intern->MxNamed = 0;
	} else {
		intern->MxNamed = 1;
	}

	intern->MxAutoUnlock = zend_is_true(autounlock) ? 1 : 0;

	TempSize = dao_semaphore_getsize() + dao_getalignsize(sizeof(uint32_t)) + dao_event_getsize() + dao_semaphore_getsize();

	Result = dao_namedmem_init(&intern->MxMem, &Pos, "/Sync_ReadWrite", intern->MxNamed ? Z_STRVAL_P(name) : NULL, TempSize);

	if (Result < 0) {
		DAO_THROW_EXCEPTION_STR(dao_sync_exception_ce, "Reader-Writer object could not be created");
		return;
	}

	/* Load the pointers. */
	MemPtr = intern->MxMem + Pos;
	dao_semaphore_get(&intern->MxPthreadRCountMutex, MemPtr);
	MemPtr += dao_semaphore_getsize();

	intern->MxRCount = (volatile uint32_t *)(MemPtr);
	MemPtr += dao_getalignsize(sizeof(uint32_t));

	dao_event_get(&intern->MxPthreadRWaitEvent, MemPtr);
	MemPtr += dao_event_getsize();

	dao_semaphore_get(&intern->MxPthreadWWaitMutex, MemPtr);

	/* Handle the first time this reader/writer lock has been opened. */
	if (Result == 0) {
		dao_semaphore_init(&intern->MxPthreadRCountMutex, intern->MxNamed, 1, 1);
		intern->MxRCount[0] = 0;
		dao_event_init(&intern->MxPthreadRWaitEvent, intern->MxNamed, 1, 1);
		dao_semaphore_init(&intern->MxPthreadWWaitMutex, intern->MxNamed, 1, 1);

		if (intern->MxNamed) dao_namedmem_ready(intern->MxMem);
	}
}

/**
 * Read locks a reader-writer object
 */
PHP_METHOD(Dao_Sync_Readerwriter, readlock){

	zval *wait = NULL;
	uint32_t waitamt;
	uint64_t starttime, currtime;
	dao_sync_readerwriter_object *intern;

	dao_fetch_params(0, 0, 1, &wait);

	intern = dao_sync_readerwriter_object_from_obj(Z_OBJ_P(getThis()));

	if (wait && Z_TYPE_P(wait) == IS_LONG && Z_LVAL_P(wait) > -1) {
		waitamt = Z_LVAL_P(wait);
	} else {
		waitamt = INFINITE;
	}

	/* Get current time in milliseconds. */
	starttime = (waitamt == INFINITE ? 0 : dao_getmicrosecondtime() / 1000000);

	/* Acquire the write lock mutex.  Guarantees that readers can't starve the writer. */
	if (!dao_semaphore_wait(&intern->MxPthreadWWaitMutex, waitamt))  RETURN_FALSE;

	/* Acquire the counter mutex. */
	currtime = (waitamt == INFINITE ? 0 : dao_getmicrosecondtime() / 1000000);
	if (waitamt < currtime - starttime || !dao_semaphore_wait(&intern->MxPthreadRCountMutex, waitamt - (currtime - starttime)))
	{
		dao_semaphore_release(&intern->MxPthreadWWaitMutex, NULL);

		RETURN_FALSE;
	}

	/* Update the event state. */
	if (!dao_event_reset(&intern->MxPthreadRWaitEvent)) {
		dao_semaphore_release(&intern->MxPthreadRCountMutex, NULL);
		dao_semaphore_release(&intern->MxPthreadWWaitMutex, NULL);

		RETURN_FALSE;
	}

	/* Increment the number of readers. */
	intern->MxRCount[0]++;

	intern->MxReadLocks++;

	/* Release the mutexes. */
	dao_semaphore_release(&intern->MxPthreadRCountMutex, NULL);
	dao_semaphore_release(&intern->MxPthreadWWaitMutex, NULL);

	RETURN_TRUE;
}

/**
 * Read unlocks a reader-writer object
 *
 */
PHP_METHOD(Dao_Sync_Readerwriter, readunlock){

	dao_sync_readerwriter_object *intern;

	intern = dao_sync_readerwriter_object_from_obj(Z_OBJ_P(getThis()));

	if (!dao_readerwriter_readunlock_internal(intern)) {
		RETURN_FALSE;
	}

	RETURN_TRUE;
}

/**
 * Write locks a reader-writer object
 */
PHP_METHOD(Dao_Sync_Readerwriter, writelock){

	zval *wait = NULL;
	uint32_t waitamt;
	uint64_t starttime, currtime;
	dao_sync_readerwriter_object *intern;

	dao_fetch_params(0, 0, 1, &wait);

	intern = dao_sync_readerwriter_object_from_obj(Z_OBJ_P(getThis()));

	if (wait && Z_TYPE_P(wait) == IS_LONG && Z_LVAL_P(wait) > -1) {
		waitamt = Z_LVAL_P(wait);
	} else {
		waitamt = INFINITE;
	}

	/* Get current time in milliseconds. */
	starttime = (waitamt == INFINITE ? 0 : dao_getmicrosecondtime() / 1000000);

	/* Acquire the write lock mutex. */
	if (!dao_semaphore_wait(&intern->MxPthreadWWaitMutex, waitamt))  RETURN_FALSE;

	/* Wait for readers to reach zero. */
	currtime = (waitamt == INFINITE ? 0 : dao_getmicrosecondtime() / 1000000);
	if (waitamt < currtime - starttime || !dao_event_wait(&intern->MxPthreadRWaitEvent, waitamt - (currtime - starttime)))
	{
		dao_semaphore_release(&intern->MxPthreadWWaitMutex, NULL);

		RETURN_FALSE;
	}

	intern->MxWriteLock = 1;

	RETURN_TRUE;
}

/**
 * Write unlocks a reader-writer object
 *
 */
PHP_METHOD(Dao_Sync_Readerwriter, writeunlock){

	dao_sync_readerwriter_object *intern;

	intern = dao_sync_readerwriter_object_from_obj(Z_OBJ_P(getThis()));

	if (!dao_readerwriter_writeunlock_internal(intern)) {
		RETURN_FALSE;
	}

	RETURN_TRUE;
}
