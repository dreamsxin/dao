
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

#ifndef DAO_SYNC_READERWRITER_H
#define DAO_SYNC_READERWRITER_H

#include "php_dao.h"
#if DAO_USE_SHM_OPEN
#include "sync/common.h"

/* Reader-Writer */
typedef struct _dao_sync_readerwriter_object {
	int MxNamed;
	char *MxMem;
	dao_semaphore_wrapper MxPthreadRCountMutex;
	volatile uint32_t *MxRCount;
	dao_event_wrapper MxPthreadRWaitEvent;
	dao_semaphore_wrapper MxPthreadWWaitMutex;

	int MxAutoUnlock;
	volatile unsigned int MxReadLocks, MxWriteLock;

	zend_object std;
} dao_sync_readerwriter_object;

static inline dao_sync_readerwriter_object *dao_sync_readerwriter_object_from_obj(zend_object *obj) {
	return (dao_sync_readerwriter_object*)((char*)(obj) - XtOffsetOf(dao_sync_readerwriter_object, std));
}

static inline int dao_readerwriter_readunlock_internal(dao_sync_readerwriter_object *obj)
{
	if (obj->MxMem == NULL)  return 0;

	/* Acquire the counter mutex. */
	if (!dao_semaphore_wait(&obj->MxPthreadRCountMutex, INFINITE))  return 0;

	if (obj->MxReadLocks)  obj->MxReadLocks--;

	/* Decrease the number of readers. */
	if (obj->MxRCount[0])  obj->MxRCount[0]--;
	else
	{
		dao_semaphore_release(&obj->MxPthreadRCountMutex, NULL);

		return 0;
	}

	/* Update the event state. */
	if (!obj->MxRCount[0] && !dao_event_fire(&obj->MxPthreadRWaitEvent))
	{
		dao_semaphore_release(&obj->MxPthreadRCountMutex, NULL);

		return 0;
	}

	/* Release the counter mutex. */
	dao_semaphore_release(&obj->MxPthreadRCountMutex, NULL);

	return 1;
}

static inline int dao_readerwriter_writeunlock_internal(dao_sync_readerwriter_object *obj)
{

	if (obj->MxMem == NULL)  return 0;

	obj->MxWriteLock = 0;

	/* Release the write lock. */
	dao_semaphore_release(&obj->MxPthreadWWaitMutex, NULL);

	return 1;
}

extern zend_class_entry *dao_sync_readerwriter_ce;

DAO_INIT_CLASS(Dao_Sync_Readerwriter);

#endif
#endif /* DAO_SYNC_READERWRITER_H */
