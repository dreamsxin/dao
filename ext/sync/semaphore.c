
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

#include "sync/semaphore.h"
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
 * Dao\Sync\Semaphore
 *
 */
zend_class_entry *dao_sync_semaphore_ce;

PHP_METHOD(Dao_Sync_Semaphore, __construct);
PHP_METHOD(Dao_Sync_Semaphore, lock);
PHP_METHOD(Dao_Sync_Semaphore, unlock);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_sync_semaphore___construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 1)
	ZEND_ARG_TYPE_INFO(0, initialval, IS_LONG, 1)
	ZEND_ARG_TYPE_INFO(0, autounlock, _IS_BOOL, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_sync_semaphore_lock, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, wait, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_sync_semaphore_unlock, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(1, prevcount, IS_LONG, 1)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_sync_semaphore_method_entry[] = {
	PHP_ME(Dao_Sync_Semaphore, __construct, arginfo_dao_sync_semaphore___construct, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Sync_Semaphore, lock, arginfo_dao_sync_semaphore_lock, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Sync_Semaphore, unlock, arginfo_dao_sync_semaphore_unlock, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

zend_object_handlers dao_sync_semaphore_object_handlers;
zend_object* dao_sync_semaphore_object_create_handler(zend_class_entry *ce)
{
	dao_sync_semaphore_object *intern = ecalloc(1, sizeof(dao_sync_semaphore_object) + zend_object_properties_size(ce));
	intern->std.ce = ce;

	zend_object_std_init(&intern->std, ce);
	object_properties_init(&intern->std, ce);
	intern->std.handlers = &dao_sync_semaphore_object_handlers;

	intern->MxNamed = 0;
	intern->MxMem = NULL;
	intern->MxAutoUnlock = 0;
	intern->MxCount = 0;

	return &intern->std;
}

void dao_sync_semaphore_object_free_handler(zend_object *object)
{
	dao_sync_semaphore_object *intern = dao_sync_semaphore_object_from_obj(object);

	if (intern->MxAutoUnlock)
	{
		while (intern->MxCount)
		{
			dao_semaphore_release(&intern->MxPthreadSemaphore, NULL);
			intern->MxCount--;
		}
	}

	if (intern->MxMem != NULL) {
		if (intern->MxNamed) {
			dao_namedmem_unmap(intern->MxMem, dao_semaphore_getsize());
		} else {
			dao_semaphore_free(&intern->MxPthreadSemaphore);

			efree(intern->MxMem);
		}
	}
}

/**
 * Dao\Sync\Semaphore initializer
 */
DAO_INIT_CLASS(Dao_Sync_Semaphore){

	DAO_REGISTER_CLASS_CREATE_OBJECT(Dao\\Sync, Semaphore, sync_semaphore, dao_sync_semaphore_method_entry, 0);

	return SUCCESS;
}

/**
 * Dao\Sync\Semaphore constructor
 *
 * @param string $name
 * @param int $initialval
 * @param boolean $autounlock
 */
PHP_METHOD(Dao_Sync_Semaphore, __construct){

	zval *name = NULL, *initialval = NULL, *autounlock = NULL;
	dao_sync_semaphore_object *intern;
	size_t Pos, TempSize;
	int result;

	dao_fetch_params(0, 0, 3, &name, &initialval, &autounlock);

	intern = dao_sync_semaphore_object_from_obj(Z_OBJ_P(getThis()));

	if (!name || DAO_IS_EMPTY(name)) {
		intern->MxNamed = 0;
	} else {
		intern->MxNamed = 1;
	}

	if (!autounlock) {
		autounlock = &DAO_GLOBAL(z_true);
	}

	intern->MxAutoUnlock = zend_is_true(autounlock) ? 1 : 0;

	TempSize = dao_semaphore_getsize();
	result = dao_namedmem_init(&intern->MxMem, &Pos, "/Sync_Semaphore", intern->MxNamed ? Z_STRVAL_P(name) : NULL, TempSize);

	if (result < 0) {
		DAO_THROW_EXCEPTION_STR(dao_sync_exception_ce, "Semaphore could not be created");
		return;
	}

	dao_semaphore_get(&intern->MxPthreadSemaphore, intern->MxMem + Pos);

	/* Handle the first time this semaphore has been opened. */
	if (result == 0) {
		dao_semaphore_init(&intern->MxPthreadSemaphore, intern->MxNamed, (uint32_t)Z_LVAL_P(initialval), (uint32_t)Z_LVAL_P(initialval));
		if (intern->MxNamed) dao_namedmem_ready(intern->MxMem);
	}
}

/**
 * Locks a semaphore object
 *
 * @return boolean
 */
PHP_METHOD(Dao_Sync_Semaphore, lock){

	zval *_wait = NULL;
	zend_long wait = -1;
	dao_sync_semaphore_object *intern;

	dao_fetch_params(0, 0, 1, &_wait);

	if (_wait && Z_TYPE_P(_wait) == IS_LONG) {
		wait = Z_LVAL_P(_wait);
	}

	intern = dao_sync_semaphore_object_from_obj(Z_OBJ_P(getThis()));

	if (!dao_semaphore_wait(&intern->MxPthreadSemaphore, (uint32_t)(wait > -1 ? wait : INFINITE))) {
		RETURN_FALSE;
	}

	if (intern->MxAutoUnlock) intern->MxCount++;

	RETURN_TRUE;
}

/**
 * Unlocks a semaphore object
 *
 * @return boolean
 */
PHP_METHOD(Dao_Sync_Semaphore, unlock){

	zval *_prevcount = NULL;
	uint32_t prevcount = 0;
	dao_sync_semaphore_object *intern;

	dao_fetch_params(0, 0, 1, &_prevcount);

	intern = dao_sync_semaphore_object_from_obj(Z_OBJ_P(getThis()));

	dao_semaphore_release(&intern->MxPthreadSemaphore, &prevcount);

	if (_prevcount != NULL) {
		ZVAL_LONG(_prevcount, (zend_long)prevcount);
	}

	if (intern->MxAutoUnlock) intern->MxCount--;

	RETURN_TRUE;
}
