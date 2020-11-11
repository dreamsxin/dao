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

#include "events/manager.h"
#include "events/managerinterface.h"
#include "events/event.h"
#include "events/eventinterface.h"
#include "events/listener.h"
#include "events/exception.h"
#include "events/../debug.h"

#include <Zend/zend_closures.h>
#include <ext/spl/spl_heap.h>

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/exception.h"
#include "kernel/object.h"
#include "kernel/array.h"
#include "kernel/fcall.h"
#include "kernel/string.h"
#include "kernel/concat.h"
#include "kernel/operators.h"
#include "kernel/hash.h"
#include "kernel/debug.h"

/**
 * Dao\Events\Manager
 *
 * Dao Events Manager, offers an easy way to intercept and manipulate, if needed,
 * the normal flow of operation. With the EventsManager the developer can create hooks or
 * plugins that will offer monitoring of data, manipulation, conditional execution and much more.
 *
 */
zend_class_entry *dao_events_manager_ce;

PHP_METHOD(Dao_Events_Manager, attach);
PHP_METHOD(Dao_Events_Manager, enablePriorities);
PHP_METHOD(Dao_Events_Manager, arePrioritiesEnabled);
PHP_METHOD(Dao_Events_Manager, collectResponses);
PHP_METHOD(Dao_Events_Manager, isCollecting);
PHP_METHOD(Dao_Events_Manager, getResponses);
PHP_METHOD(Dao_Events_Manager, detach);
PHP_METHOD(Dao_Events_Manager, detachAll);
PHP_METHOD(Dao_Events_Manager, fireQueue);
PHP_METHOD(Dao_Events_Manager, createEvent);
PHP_METHOD(Dao_Events_Manager, fire);
PHP_METHOD(Dao_Events_Manager, hasListeners);
PHP_METHOD(Dao_Events_Manager, getListeners);
PHP_METHOD(Dao_Events_Manager, getEvents);
PHP_METHOD(Dao_Events_Manager, getCurrentEvent);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_events_manager_enablepriorities, 0, 0, 1)
	ZEND_ARG_INFO(0, enablePriorities)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_events_manager_collectresponses, 0, 0, 1)
	ZEND_ARG_INFO(0, collect)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_events_manager_firequeue, 0, 0, 2)
	ZEND_ARG_INFO(0, queue)
	ZEND_ARG_INFO(0, event)
	ZEND_ARG_INFO(0, flag)
	ZEND_ARG_INFO(0, prevData)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_events_manager_haslisteners, 0, 0, 1)
	ZEND_ARG_INFO(0, type)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_events_manager_method_entry[] = {
	PHP_ME(Dao_Events_Manager, attach, arginfo_dao_events_managerinterface_attach, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Events_Manager, enablePriorities, arginfo_dao_events_manager_enablepriorities, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Events_Manager, arePrioritiesEnabled, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Events_Manager, collectResponses, arginfo_dao_events_manager_collectresponses, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Events_Manager, isCollecting, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Events_Manager, getResponses, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Events_Manager, detach, arginfo_dao_events_managerinterface_detach, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Events_Manager, detachAll, arginfo_dao_events_managerinterface_detachall, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Events_Manager, fireQueue, arginfo_dao_events_manager_firequeue, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Events_Manager, createEvent, arginfo_dao_events_managerinterface_createevent, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Events_Manager, fire, arginfo_dao_events_managerinterface_fire, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Events_Manager, hasListeners, arginfo_dao_events_manager_haslisteners, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Events_Manager, getListeners, arginfo_dao_events_managerinterface_getlisteners, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Events_Manager, getEvents, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Events_Manager, getCurrentEvent, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Events_Manager, dettachAll, detachAll, arginfo_dao_events_managerinterface_detachall, ZEND_ACC_PUBLIC | ZEND_ACC_DEPRECATED)
	PHP_MALIAS(Dao_Events_Manager, clearListeners, detachAll, arginfo_dao_events_managerinterface_detachall, ZEND_ACC_PUBLIC | ZEND_ACC_DEPRECATED)
	PHP_FE_END
};


/**
 * Dao\Events\Manager initializer
 */
DAO_INIT_CLASS(Dao_Events_Manager){

	DAO_REGISTER_CLASS(Dao\\Events, Manager, events_manager, dao_events_manager_method_entry, 0);

	zend_declare_property_null(dao_events_manager_ce, SL("_events"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_events_manager_ce, SL("_currentEvent"), ZEND_ACC_PROTECTED);
	zend_declare_property_bool(dao_events_manager_ce, SL("_collect"), 0, ZEND_ACC_PROTECTED);
	zend_declare_property_bool(dao_events_manager_ce, SL("_enablePriorities"), 0, ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_events_manager_ce, SL("_responses"), ZEND_ACC_PROTECTED);

	zend_class_implements(dao_events_manager_ce, 1, dao_events_managerinterface_ce);

	return SUCCESS;
}

/**
 * Attach a listener to the events manager
 *
 * @param string $eventType
 * @param callable $handler
 * @param int $priority
 */
PHP_METHOD(Dao_Events_Manager, attach){

	zval *event_type, *handler, *_priority = NULL, event = {}, priority = {}, events = {}, listener = {}, enable_priorities = {}, priority_queue = {};

	dao_fetch_params(1, 2, 1, &event_type, &handler, &_priority);

	if (!_priority) {
		ZVAL_LONG(&priority, 100);
	} else {
		ZVAL_COPY_VALUE(&priority, _priority);
	}

	if (Z_TYPE_P(handler) != IS_OBJECT && !dao_is_callable(handler)) {
		DAO_MM_THROW_EXCEPTION_STR(dao_events_exception_ce, "Event handler must be an object or callable");
		return;
	}

	if (!dao_memnstr_str(event_type, SL(":"))) {
		ZVAL_COPY_VALUE(&event, event_type);
	} else {
		zval event_parts ={}, name = {}, type = {};
		dao_fast_explode_str(&event_parts, SL(":"), event_type);
		dao_array_fetch_long(&name, &event_parts, 0, PH_READONLY);
		dao_array_fetch_long(&type, &event_parts, 1, PH_READONLY);
		if (DAO_IS_STRING(&type, "*") || DAO_IS_EMPTY_STRING(&type)) {
			DAO_MM_ZVAL_COPY(&event, &name);
		} else {
			DAO_MM_ZVAL_COPY(&event, event_type);
		}
		zval_ptr_dtor(&event_parts);
	}

	if (dao_instance_of_ev(handler, dao_events_listener_ce)) {
		ZVAL_COPY_VALUE(&listener, handler);
		DAO_MM_CALL_METHOD(NULL, &listener, "setpriority", &priority);
		DAO_MM_CALL_METHOD(NULL, &listener, "setevent", &event);
	} else {
		object_init_ex(&listener, dao_events_listener_ce);
		DAO_MM_ADD_ENTRY(&listener);
		DAO_MM_CALL_METHOD(NULL, &listener, "__construct", handler, &priority, &event);
	}

	dao_read_property(&events, getThis(), SL("_events"), PH_READONLY);
	if (Z_TYPE(events) != IS_ARRAY) {
		array_init(&events);
		dao_update_property(getThis(), SL("_events"), &events);
		zval_ptr_dtor(&events);
	}

	if (!dao_array_isset_fetch(&priority_queue, &events, &event, PH_READONLY)) {
		dao_read_property(&enable_priorities, getThis(), SL("_enablePriorities"), PH_READONLY);
		if (zend_is_true(&enable_priorities)) {
			/**
			 * Create a SplPriorityQueue to store the events with priorities
			 */
			object_init_ex(&priority_queue, spl_ce_SplPriorityQueue);
			DAO_MM_ADD_ENTRY(&priority_queue);
			if (dao_has_constructor(&priority_queue)) {
				DAO_MM_CALL_METHOD(NULL, &priority_queue, "__construct");
			}

			/**
			 * Extract only the Data, Set extraction flags
			 */
			DAO_MM_CALL_METHOD(NULL, &priority_queue, "setextractflags", &DAO_GLOBAL(z_one));
		} else {
			array_init(&priority_queue);
			DAO_MM_ADD_ENTRY(&priority_queue);
		}
	}

	/**
	 * Insert the handler in the queue
	 */
	if (unlikely(Z_TYPE(priority_queue) == IS_OBJECT)) {
		DAO_MM_CALL_METHOD(NULL, &priority_queue, "insert", &listener, &priority);
	} else {
		dao_array_append(&priority_queue, &listener, PH_COPY);
	}

	/**
	 * Append the events to the queue
	 */
	dao_array_update(&events, &event, &priority_queue, PH_COPY);

	RETURN_MM();
}

/**
 * Set if priorities are enabled in the EventsManager
 *
 * @param boolean $enablePriorities
 */
PHP_METHOD(Dao_Events_Manager, enablePriorities){

	zval *enable_priorities;

	dao_fetch_params(0, 1, 0, &enable_priorities);

	dao_update_property(getThis(), SL("_enablePriorities"), enable_priorities);

}

/**
 * Returns if priorities are enabled
 *
 * @return boolean
 */
PHP_METHOD(Dao_Events_Manager, arePrioritiesEnabled){


	RETURN_MEMBER(getThis(), "_enablePriorities");
}

/**
 * Tells the event manager if it needs to collect all the responses returned by every
 * registered listener in a single fire
 *
 * @param boolean $collect
 */
PHP_METHOD(Dao_Events_Manager, collectResponses){

	zval *collect;

	dao_fetch_params(0, 1, 0, &collect);

	dao_update_property(getThis(), SL("_collect"), collect);

}

/**
 * Check if the events manager is collecting all all the responses returned by every
 * registered listener in a single fire
 */
PHP_METHOD(Dao_Events_Manager, isCollecting){


	RETURN_MEMBER(getThis(), "_collect");
}

/**
 * Returns all the responses returned by every handler executed by the last 'fire' executed
 *
 * @return array
 */
PHP_METHOD(Dao_Events_Manager, getResponses){


	RETURN_MEMBER(getThis(), "_responses");
}

/**
 * Detach a listener from the events manager
 *
 * @param object|callable $handler
 */
PHP_METHOD(Dao_Events_Manager, detach){

	zval *type, *handler, events = {}, queue = {}, priority_queue = {}, *listener;
	zend_string *str_key;
	ulong idx;

	dao_fetch_params(0, 2, 0, &type, &handler);

	if (Z_TYPE_P(handler) != IS_OBJECT && !dao_is_callable(handler)) {
		DAO_THROW_EXCEPTION_STR(dao_events_exception_ce, "Event handler must be an object or callable");
		return;
	}

	dao_read_property(&events, getThis(), SL("_events"), PH_READONLY);
	if (Z_TYPE(events) != IS_ARRAY) {
		RETURN_FALSE;
	}

	if (!dao_array_isset_fetch(&queue, &events, type, PH_READONLY)) {
		RETURN_FALSE;
	}

	if (Z_TYPE(queue) == IS_OBJECT) {
		object_init_ex(&priority_queue, spl_ce_SplPriorityQueue);
		if (dao_has_constructor(&priority_queue)) {
			DAO_CALL_METHOD(NULL, &priority_queue, "__construct");
		}

		DAO_CALL_METHOD(NULL, &queue, "top");

		while (1) {
			zval r0 = {}, listener0 = {}, handler_embeded = {}, priority = {};
			DAO_CALL_METHOD(&r0, &queue, "valid");
			if (!zend_is_true(&r0)) {
				break;
			}

			DAO_CALL_METHOD(&listener0, &queue, "current");
			DAO_CALL_METHOD(&handler_embeded, &listener0, "getlistener");

			if (!dao_is_equal(&handler_embeded, handler)) {
				DAO_CALL_METHOD(&priority, &listener0, "getpriority");
				DAO_CALL_METHOD(NULL, &priority_queue, "insert", &listener0, &priority);
				zval_ptr_dtor(&priority);
			}
			zval_ptr_dtor(&listener0);
			zval_ptr_dtor(&handler_embeded);

			DAO_CALL_METHOD(NULL, &queue, "next");
		}
	} else {
		ZVAL_DUP(&priority_queue, &queue);
		ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL(queue), idx, str_key, listener) {
			zval key = {}, handler_embeded = {};
			if (str_key) {
				ZVAL_STR(&key, str_key);
			} else {
				ZVAL_LONG(&key, idx);
			}

			DAO_CALL_METHOD(&handler_embeded, listener, "getlistener");

			if (dao_is_equal_object(&handler_embeded, handler)) {
				dao_array_unset(&priority_queue, &key, 0);
			}
			zval_ptr_dtor(&handler_embeded);

		} ZEND_HASH_FOREACH_END();
	}

	dao_array_update(&events, type, &priority_queue, 0);
}

/**
 * Removes all events from the EventsManager
 *
 * @param string $type
 */
PHP_METHOD(Dao_Events_Manager, detachAll){

	zval *type = NULL, events = {};

	dao_fetch_params(0, 0, 1, &type);

	if (!type) {
		type = &DAO_GLOBAL(z_null);
	}

	dao_read_property(&events, getThis(), SL("_events"), PH_READONLY);
	if (Z_TYPE_P(type) != IS_NULL && dao_array_isset(&events, type)) {
		dao_array_unset(&events, type, 0);
	}
}

/**
 * Internal handler to call a queue of events
 *
 * @param \SplPriorityQueue $queue
 * @param Dao\Events\Event $event
 * @param boolean $flag
 * @param mixed $prevData
 * @return mixed
 */
PHP_METHOD(Dao_Events_Manager, fireQueue){

	zval *queue, *event, *flag = NULL, *_prev_data = NULL, event_name = {}, source = {}, data = {}, cancelable = {}, collect = {}, iterator = {}, *listener;
	zval status = {};
	zend_class_entry *ce, *weakref_ce;

	dao_fetch_params(0, 2, 1, &queue, &event, &flag, &_prev_data);

	if (!flag) {
		flag = &DAO_GLOBAL(z_false);
	}

	if (_prev_data) {
		ZVAL_COPY(&status, _prev_data);
	} else {
		ZVAL_NULL(&status);
	}

	if (unlikely(Z_TYPE_P(queue) != IS_ARRAY)) {
		if (Z_TYPE_P(queue) == IS_OBJECT) {
			ce = Z_OBJCE_P(queue);
			if (!instanceof_function(ce, dao_events_event_ce) && !instanceof_function(ce, spl_ce_SplPriorityQueue)) {
				DAO_THROW_EXCEPTION_FORMAT(dao_events_exception_ce, "Unexpected value type: expected object of type Dao\\Events\\Event or SplPriorityQueue, %s given", ce->name->val);
				return;
			}
		} else {
			DAO_THROW_EXCEPTION_FORMAT(dao_events_exception_ce, "Unexpected value type: expected object of type Dao\\Events\\Event or SplPriorityQueue, %s given", zend_zval_type_name(queue));
			return;
		}
	}

	DAO_VERIFY_CLASS_EX(event, dao_events_event_ce, dao_events_exception_ce);

	weakref_ce = dao_class_str_exists(SL("WeakRef"), 0);

	/**
	 * Get the event type
	 */
	DAO_CALL_METHOD(&event_name, event, "gettype");

	if (unlikely(Z_TYPE(event_name) != IS_STRING)) {
		DAO_THROW_EXCEPTION_STR(dao_events_exception_ce, "The event type not valid");
		return;
	}

	/**
	 * Get the object who triggered the event
	 */
	DAO_CALL_METHOD(&source, event, "getsource");

	/**
	 * Get extra data passed to the event
	 */
	DAO_CALL_METHOD(&data, event, "getdata");

	/**
	 * Tell if the event is cancelable
	 */
	DAO_CALL_METHOD(&cancelable, event, "iscancelable");

	/**
	 * Responses need to be traced?
	 */
	dao_read_property(&collect, getThis(), SL("_collect"), PH_READONLY);
	if (Z_TYPE_P(queue) == IS_OBJECT) {
		/**
		 * We need to clone the queue before iterate over it
		 */
		if (dao_clone(&iterator, queue) == FAILURE) {
			return;
		}

		/**
		 * Move the queue to the top
		 */
		DAO_CALL_METHOD(NULL, &iterator, "top");

		while (1) {
			zval r0 = {}, listener0 = {}, handler_embeded = {}, handler_referenced = {}, handler = {}, arguments = {}, is_stopped = {};

			DAO_CALL_METHOD(&r0, &iterator, "valid");
			if (!zend_is_true(&r0)) {
				break;
			}

			/**
			 * Get the current data
			 */
			DAO_CALL_METHOD(&listener0, &iterator, "current");
			DAO_CALL_METHOD(&handler_embeded, &listener0, "getlistener");

			/**
			 * Only handler objects are valid
			 */
			if (Z_TYPE(handler_embeded) == IS_OBJECT) {
				/**
				 * Check if the event is a weak reference.
				 */
				if (weakref_ce && instanceof_function(Z_OBJCE(handler_embeded), weakref_ce)) {
					/**
					 * Checks whether the object referenced still exists.
					 */
					DAO_CALL_METHOD(&handler_referenced, &handler_embeded, "valid");

					if (zend_is_true(&handler_referenced)) {
						DAO_CALL_METHOD(&handler, &handler_embeded, "get");
					} else {
						/**
						 * Move the queue to the next handler
						 */
						DAO_CALL_METHOD(NULL, &iterator, "next");
						zval_ptr_dtor(&handler_embeded);
						continue;
					}

				} else {
					ZVAL_COPY(&handler, &handler_embeded);
				}

				/**
				 * Check if the event is a closure
				 */
				assert(Z_TYPE(handler) == IS_OBJECT);
				if (instanceof_function(Z_OBJCE(handler), zend_ce_closure)) {
					/**
					 * Create the closure arguments
					 */
					array_init_size(&arguments, 4);
					dao_array_append(&arguments, event, PH_COPY);
					dao_array_append(&arguments, &source, PH_COPY);
					dao_array_append(&arguments, &data, PH_COPY);
					dao_array_append(&arguments, &status, 0);

					/**
					 * Call the function in the PHP userland
					 */
					DAO_CALL_USER_FUNC_ARRAY(&status, &handler, &arguments);
					zval_ptr_dtor(&arguments);
					if (zend_is_true(flag) && DAO_IS_FALSE(&status)){
						break;
					}

					/**
					 * Trace the response
					 */
					if (zend_is_true(&collect)) {
						dao_update_property_array_append(getThis(), SL("_responses"), &status);
					}

					if (zend_is_true(&cancelable)) {
						/**
						 * Check if the event was stopped by the user
						 */
						DAO_CALL_METHOD(&is_stopped, event, "isstopped");
						if (zend_is_true(&is_stopped)) {
							break;
						}
					}
				} else {
					/**
					 * Check if the listener has implemented an event with the same name
					 */
					if (dao_method_exists(&handler, &event_name) == SUCCESS || dao_method_exists_ex(&handler, SL("__call")) == SUCCESS) {
						zval prev_data = {};
						ZVAL_COPY(&prev_data, &status);
						zval_ptr_dtor(&status);
						/**
						 * Call the function in the PHP userland
						 */
						DAO_CALL_METHOD(&status, &handler, Z_STRVAL(event_name), event, &source, &data, &prev_data);
						zval_ptr_dtor(&prev_data);
						if (zend_is_true(flag) && DAO_IS_FALSE(&status)){
							break;
						}

						/**
						 * Collect the response
						 */
						if (zend_is_true(&collect)) {
							dao_update_property_array_append(getThis(), SL("_responses"), &status);
						}

						if (zend_is_true(&cancelable)) {

							/**
							 * Check if the event was stopped by the user
							 */
							DAO_CALL_METHOD(&is_stopped, event, "isstopped");
							if (zend_is_true(&is_stopped)) {
								zval_ptr_dtor(&handler_embeded);
								zval_ptr_dtor(&listener0);
								break;
							}
						}
					}
				}
				zval_ptr_dtor(&handler);
			}
			zval_ptr_dtor(&handler_embeded);
			zval_ptr_dtor(&listener0);

			/**
			 * Move the queue to the next handler
			 */
			DAO_CALL_METHOD(NULL, &iterator, "next");
		}
	} else {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(queue), listener) {
			zval handler_embeded = {}, handler_referenced = {}, handler = {}, arguments = {}, is_stopped = {};

			DAO_CALL_METHOD(&handler_embeded, listener, "getlistener");
			/**
			 * Only handler objects are valid
			 */
			if (Z_TYPE(handler_embeded) == IS_OBJECT) {

				/**
				  * Check if the event is a weak reference.
				  */
				if (weakref_ce && instanceof_function(Z_OBJCE(handler_embeded), weakref_ce)) {
					/**
					 * Checks whether the object referenced still exists.
					 */
					DAO_CALL_METHOD(&handler_referenced, &handler_embeded, "valid");

					if (zend_is_true(&handler_referenced)) {
						DAO_CALL_METHOD(&handler, &handler_embeded, "get");
					} else {
						continue;
					}

				} else {
					ZVAL_COPY(&handler, &handler_embeded);
				}

				/**
				 * Check if the event is a closure
				 */
				assert(Z_TYPE(handler) == IS_OBJECT);
				if (instanceof_function(Z_OBJCE(handler), zend_ce_closure)) {
					/**
					 * Create the closure arguments
					 */
					array_init_size(&arguments, 4);
					dao_array_append(&arguments, event, PH_COPY);
					dao_array_append(&arguments, &source, PH_COPY);
					dao_array_append(&arguments, &data, PH_COPY);
					dao_array_append(&arguments, &status, 0);

					/**
					 * Call the function in the PHP userland
					 */
					DAO_CALL_USER_FUNC_ARRAY(&status, &handler, &arguments);
					zval_ptr_dtor(&arguments);
					if (zend_is_true(flag) && DAO_IS_FALSE(&status)){
						break;
					}

					/**
					 * Trace the response
					 */
					if (zend_is_true(&collect)) {
						dao_update_property_array_append(getThis(), SL("_responses"), &status);
					}

					if (zend_is_true(&cancelable)) {

						/**
						 * Check if the event was stopped by the user
						 */
						DAO_CALL_METHOD(&is_stopped, event, "isstopped");
						if (zend_is_true(&is_stopped)) {
							break;
						}
					}
				} else {
					/**
					 * Check if the listener has implemented an event with the same name
					 */
					if (dao_method_exists(&handler, &event_name) == SUCCESS || dao_method_exists_ex(&handler, SL("__call")) == SUCCESS) {
						zval prev_data = {};
						ZVAL_COPY(&prev_data, &status);
						zval_ptr_dtor(&status);
						/**
						 * Call the function in the PHP userland
						 */
						DAO_CALL_METHOD(&status, &handler, Z_STRVAL(event_name), event, &source, &data, &prev_data);
						zval_ptr_dtor(&prev_data);
						if (zend_is_true(flag) && DAO_IS_FALSE(&status)){
							break;
						}

						/**
						 * Collect the response
						 */
						if (zend_is_true(&collect)) {
							dao_update_property_array_append(getThis(), SL("_responses"), &status);
						}

						if (zend_is_true(&cancelable)) {
							/**
							 * Check if the event was stopped by the user
							 */
							DAO_CALL_METHOD(&is_stopped, event, "isstopped");
							if (zend_is_true(&is_stopped)) {
								break;
							}
						}
					}
				}
				zval_ptr_dtor(&handler);
			}
			zval_ptr_dtor(&handler_embeded);
		} ZEND_HASH_FOREACH_END();
	}
	zval_ptr_dtor(&event_name);
	zval_ptr_dtor(&source);
	zval_ptr_dtor(&data);
	zval_ptr_dtor(&cancelable);
	RETURN_NCTOR(&status);
}

/**
 * Create an event
 *
 * @param string $event_type
 * @param object $source
 * @param mixed $data
 * @param boolean $cancelable
 * @param boolean $flag
 * @return Dao\Events\Event
 */
PHP_METHOD(Dao_Events_Manager, createEvent){

	zval *event_type, *source, *data = NULL, *cancelable = NULL, *flag = NULL, exception_message = {};
	zval event_parts = {}, name = {}, type = {};

	dao_fetch_params(0, 2, 3, &event_type, &source, &data, &cancelable, &flag);

	if (!data) {
		data = &DAO_GLOBAL(z_null);
	}

	if (!cancelable) {
		cancelable = &DAO_GLOBAL(z_true);
	}

	if (!flag) {
		flag = &DAO_GLOBAL(z_false);
	}

	/**
	 * All valid events must have a colon separator
	 */
	if (!dao_memnstr_str(event_type, SL(":"))) {
		DAO_CONCAT_SV(&exception_message, "Invalid event type ", event_type);
		DAO_THROW_EXCEPTION_ZVAL(dao_events_exception_ce, &exception_message);
		zval_ptr_dtor(&exception_message);
		return;
	}

	dao_fast_explode_str(&event_parts, SL(":"), event_type);
	dao_array_fetch_long(&name, &event_parts, 0, PH_READONLY);
	dao_array_fetch_long(&type, &event_parts, 1, PH_READONLY);

	object_init_ex(return_value, dao_events_event_ce);
	DAO_CALL_METHOD(NULL, return_value, "__construct", &type, source, data, cancelable, flag);
	DAO_CALL_METHOD(NULL, return_value, "setname", &name);
	zval_ptr_dtor(&event_parts);
}

/**
 * Fires an event in the events manager causing that active listeners be notified about it
 *
 *<code>
 *	$eventsManager->fire('db', $connection);
 *</code>
 *
 * @param string|Dao\Events\Event $event_type
 * @param object $source
 * @param mixed $data
 * @param int $cancelable
 * @param int $flag
 * @return mixed
 */
PHP_METHOD(Dao_Events_Manager, fire){

	zval *_event_type, *source, *data = NULL, *cancelable = NULL, *flag = NULL, debug_message = {};
	zval event_type = {}, events = {}, name = {}, type = {}, status = {}, collect = {}, any_type = {}, event = {}, fire_events = {};

	dao_fetch_params(1, 2, 3, &_event_type, &source, &data, &cancelable, &flag);

	if (unlikely(DAO_GLOBAL(debug).enable_debug)) {
		DAO_CONCAT_SV(&debug_message, "--Events Manager(fire): ", _event_type);
		DAO_DEBUG_LOG(&debug_message);
		zval_ptr_dtor(&debug_message);
	}

	if (!data) {
		data = &DAO_GLOBAL(z_null);
	}

	if (!cancelable || Z_TYPE_P(cancelable) == IS_NULL) {
		cancelable = &DAO_GLOBAL(z_true);
	}

	if (!flag || Z_TYPE_P(flag) == IS_NULL) {
		flag = &DAO_GLOBAL(z_false);
	}

	if (unlikely(Z_TYPE_P(_event_type) != IS_STRING)) {
		if (Z_TYPE_P(_event_type) != IS_OBJECT) {
			DAO_MM_THROW_EXCEPTION_STR(dao_events_exception_ce, "Event type must be a string");
			return;
		} else {
			DAO_MM_VERIFY_INTERFACE_EX(_event_type, dao_events_eventinterface_ce, dao_events_exception_ce);
			DAO_MM_ZVAL_COPY(&event, _event_type);
			DAO_MM_CALL_METHOD(&name, &event, "getname");
			DAO_MM_ADD_ENTRY(&name);
			DAO_MM_CALL_METHOD(&type, &event, "gettype");
			DAO_MM_ADD_ENTRY(&type);
			DAO_CONCAT_VSV(&event_type, &name, ":", &type);
			DAO_MM_ADD_ENTRY(&event_type);
		}
	} else {
		DAO_MM_ZVAL_COPY(&event_type, _event_type);
		DAO_MM_CALL_METHOD(&event, getThis(), "createevent", &event_type, source, data, cancelable, flag);
		DAO_MM_ADD_ENTRY(&event);
		DAO_MM_CALL_METHOD(&name, &event, "getname");
		DAO_MM_ADD_ENTRY(&name);
	}

	dao_update_property(getThis(), SL("_currentEvent"), &event);

	dao_read_property(&events, getThis(), SL("_events"), PH_READONLY);
	if (Z_TYPE(events) != IS_ARRAY) {
		RETURN_MM_NULL();
	}

	ZVAL_NULL(&status);

	/**
	 * Should responses be traced?
	 */
	dao_read_property(&collect, getThis(), SL("_collect"), PH_READONLY);
	if (zend_is_true(&collect)) {
		dao_update_property_null(getThis(), SL("_responses"));
	}

	/**
	 * Check if there are listeners for the all event type
	 */
	DAO_MM_ZVAL_STRING(&any_type, "*");
	if (dao_array_isset_fetch(&fire_events, &events, &any_type, PH_READONLY)) {
		if (Z_TYPE(fire_events) == IS_ARRAY || Z_TYPE(fire_events) == IS_OBJECT) {
			/**
			 * Call the events queue
			 */
			DAO_MM_CALL_METHOD(&status, getThis(), "firequeue", &fire_events, &event);
			if (zend_is_true(flag) && DAO_IS_FALSE(&status)) {
				RETURN_MM_FALSE;
			}
		}
	}

	/**
	 * Check if events are grouped by name
	 */
	if (dao_array_isset_fetch(&fire_events, &events, &name, PH_READONLY)) {
		if (Z_TYPE(fire_events) == IS_ARRAY || Z_TYPE(fire_events) == IS_OBJECT) {
			/**
			 * Call the events queue
			 */
			DAO_MM_CALL_METHOD(&status, getThis(), "firequeue", &fire_events, &event);
			if (zend_is_true(flag) && DAO_IS_FALSE(&status)) {
				RETURN_MM_FALSE;
			}
		}
	}

	/**
	 * Check if there are listeners for the event type itself
	 */
	if (dao_array_isset_fetch(&fire_events, &events, &event_type, PH_READONLY)) {
		if (Z_TYPE(fire_events) == IS_ARRAY || Z_TYPE(fire_events) == IS_OBJECT) {
			/**
			 * Call the events queue
			 */
			DAO_MM_CALL_METHOD(&status, getThis(), "firequeue", &fire_events, &event);
			if (zend_is_true(flag) && DAO_IS_FALSE(&status)) {
				RETURN_MM_FALSE;
			}
		}
	}
	RETURN_MM_NCTOR(&status);
}

/**
 * Check whether certain type of event has listeners
 *
 * @param string $type
 * @return boolean
 */
PHP_METHOD(Dao_Events_Manager, hasListeners){

	zval *type, events = {};

	dao_fetch_params(0, 1, 0, &type);

	dao_read_property(&events, getThis(), SL("_events"), PH_READONLY);
	if (dao_array_isset(&events, type)) {
		RETURN_TRUE;
	}

	RETURN_FALSE;
}

/**
 * Returns all the attached listeners of a certain type
 *
 * @param string $type
 * @param boolean $full
 * @return array
 */
PHP_METHOD(Dao_Events_Manager, getListeners){

	zval *type, *full = NULL, events = {}, queue = {}, iterator = {}, *listener;

	dao_fetch_params(0, 1, 1, &type, &full);

	if (!full) {
		full = &DAO_GLOBAL(z_false);
	}

	dao_read_property(&events, getThis(), SL("_events"), PH_READONLY);
	if (Z_TYPE(events) != IS_ARRAY) {
		RETURN_EMPTY_ARRAY();
	}

	if (!dao_array_isset(&events, type)) {
		RETURN_EMPTY_ARRAY();
	}

	array_init(return_value);

	dao_array_fetch(&queue, &events, type, PH_NOISY|PH_READONLY);

	if (zend_is_true(full)) {
		RETURN_CTOR(&queue);
	}

	if (Z_TYPE(queue) == IS_OBJECT) {
		if (dao_clone(&iterator, &queue) == FAILURE) {
			return;
		}

		DAO_CALL_METHOD(NULL, &iterator, "top");

		while (1) {
			zval r0 = {}, listener0 = {}, handler_embeded = {};

			DAO_CALL_METHOD(&r0, &iterator, "valid");
			if (!zend_is_true(&r0)) {
				break;
			}

			DAO_CALL_METHOD(&listener0, &iterator, "current");
			DAO_CALL_METHOD(&handler_embeded, &listener0, "getlistener");
			zval_ptr_dtor(&listener0);

			dao_array_append(return_value, &handler_embeded, 0);

			DAO_CALL_METHOD(NULL, &iterator, "next");
		}
		zval_ptr_dtor(&iterator);
	} else {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL(queue), listener) {
			zval handler_embeded = {};
			DAO_CALL_METHOD(&handler_embeded, listener, "getlistener");
			dao_array_append(return_value, &handler_embeded, 0);
		} ZEND_HASH_FOREACH_END();
	}
}

/**
 * Retrieve all registered events
 *
 * @return array
 */
PHP_METHOD(Dao_Events_Manager, getEvents){

	zval events = {};

	dao_read_property(&events, getThis(), SL("_events"), PH_READONLY);
	dao_array_keys(return_value, &events);
}

/**
 * Gets current event
 *
 * @return Dao\Events\Event
 */
PHP_METHOD(Dao_Events_Manager, getCurrentEvent){

	RETURN_MEMBER(getThis(), "_currentEvent");
}
