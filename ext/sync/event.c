
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

#include "sync/event.h"
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
 * Dao\Sync\Event
 *
 */
zend_class_entry *dao_sync_event_ce;

PHP_METHOD(Dao_Sync_Event, __construct);
PHP_METHOD(Dao_Sync_Event, wait);
PHP_METHOD(Dao_Sync_Event, fire);
PHP_METHOD(Dao_Sync_Event, reset);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_sync_event___construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 1)
	ZEND_ARG_TYPE_INFO(0, manual, _IS_BOOL, 1)
	ZEND_ARG_TYPE_INFO(0, prefire, _IS_BOOL, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_sync_event_wait, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, wait, IS_LONG, 1)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_sync_event_method_entry[] = {
	PHP_ME(Dao_Sync_Event, __construct, arginfo_dao_sync_event___construct, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Sync_Event, wait, arginfo_dao_sync_event_wait, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Sync_Event, fire, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Sync_Event, reset, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

zend_object_handlers dao_sync_event_object_handlers;
zend_object* dao_sync_event_object_create_handler(zend_class_entry *ce)
{
	dao_sync_event_object *intern = ecalloc(1, sizeof(dao_sync_event_object) + zend_object_properties_size(ce));
	intern->std.ce = ce;

	zend_object_std_init(&intern->std, ce);
	object_properties_init(&intern->std, ce);
	intern->std.handlers = &dao_sync_event_object_handlers;

	intern->MxNamed = 0;
	intern->MxMem = NULL;

	return &intern->std;
}

void dao_sync_event_object_free_handler(zend_object *object)
{
	dao_sync_event_object *intern = dao_sync_event_object_from_obj(object);

	if (intern->MxMem != NULL)
	{
		if (intern->MxNamed) {
			dao_namedmem_unmap(intern->MxMem, dao_event_getsize());
		} else {
			dao_event_free(&intern->MxPthreadEvent);

			efree(intern->MxMem);
		}
	}
}

/**
 * Dao\Sync\Event initializer
 */
DAO_INIT_CLASS(Dao_Sync_Event){

	DAO_REGISTER_CLASS_CREATE_OBJECT(Dao\\Sync, Event, sync_event, dao_sync_event_method_entry, 0);

	return SUCCESS;
}

/**
 * Dao\Sync\Event constructor
 *
 * @param string $name
 * @param boolean $manual
 * @param boolean $prefire
 */
PHP_METHOD(Dao_Sync_Event, __construct){

	zval *name = NULL, *manual = NULL, *prefire = NULL;
	dao_sync_event_object *intern;
	size_t Pos, TempSize;
	int Result;

	dao_fetch_params(0, 0, 3, &name, &manual, &prefire);

	intern = dao_sync_event_object_from_obj(Z_OBJ_P(getThis()));

	if (!name || DAO_IS_EMPTY(name)) {
		intern->MxNamed = 0;
	} else {
		intern->MxNamed = 1;
	}

	TempSize = dao_event_getsize();
	Result = dao_namedmem_init(&intern->MxMem, &Pos, "/Sync_Event", intern->MxNamed ? Z_STRVAL_P(name) : NULL, TempSize);

	if (Result < 0) {
		DAO_THROW_EXCEPTION_STR(dao_sync_exception_ce, "Event object could not be created");
		return;
	}

	dao_event_get(&intern->MxPthreadEvent, intern->MxMem + Pos);

	/* Handle the first time this event has been opened. */
	if (Result == 0) {
		dao_event_init(&intern->MxPthreadEvent, intern->MxNamed, (manual ? 1 : 0), (prefire ? 1 : 0));
		if (intern->MxNamed) dao_namedmem_ready(intern->MxMem);
	}
}

/**
 * Waits for an event object to fire
 *
 * @param int $wait
 * @return boolean
 */
PHP_METHOD(Dao_Sync_Event, wait){

	zval *_wait = NULL;
	uint32_t wait = -1;
	dao_sync_event_object *intern;

	dao_fetch_params(0, 0, 1, &_wait);

	if (_wait && Z_TYPE_P(_wait) == IS_LONG) {
		wait = (uint32_t)Z_TYPE_P(_wait);
	}
	if (wait < 0) {
		wait = INFINITE;
	}

	intern = dao_sync_event_object_from_obj(Z_OBJ_P(getThis()));

	if (!dao_event_wait(&intern->MxPthreadEvent, wait)) {
		RETURN_FALSE;
	}

	RETURN_TRUE;
}

/**
 * Lets a thread through that is waiting.  Lets multiple threads through that are waiting if the event object is 'manual'
 *
 * @return boolean
 */
PHP_METHOD(Dao_Sync_Event, fire){

	dao_sync_event_object *intern;

	intern = dao_sync_event_object_from_obj(Z_OBJ_P(getThis()));

	if (!dao_event_fire(&intern->MxPthreadEvent)) {
		RETURN_FALSE;
	}

	RETURN_TRUE;
}

/**
 * Resets the event object state.  Only use when the event object is 'manual'
 *
 * @return boolean
 */
PHP_METHOD(Dao_Sync_Event, reset){

	dao_sync_event_object *intern;

	intern = dao_sync_event_object_from_obj(Z_OBJ_P(getThis()));

	if (!dao_event_reset(&intern->MxPthreadEvent)) {
		RETURN_FALSE;
	}

	RETURN_TRUE;
}