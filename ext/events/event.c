
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

#include "events/event.h"
#include "events/eventinterface.h"
#include "events/exception.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/operators.h"
#include "kernel/exception.h"

/**
 * Dao\Events\Event
 *
 * This class offers contextual information of a fired event in the EventsManager
 */
zend_class_entry *dao_events_event_ce;

PHP_METHOD(Dao_Events_Event, __construct);
PHP_METHOD(Dao_Events_Event, setName);
PHP_METHOD(Dao_Events_Event, getName);
PHP_METHOD(Dao_Events_Event, setType);
PHP_METHOD(Dao_Events_Event, getType);
PHP_METHOD(Dao_Events_Event, setSource);
PHP_METHOD(Dao_Events_Event, getSource);
PHP_METHOD(Dao_Events_Event, setData);
PHP_METHOD(Dao_Events_Event, getData);
PHP_METHOD(Dao_Events_Event, setCancelable);
PHP_METHOD(Dao_Events_Event, isCancelable);
PHP_METHOD(Dao_Events_Event, getFlag);
PHP_METHOD(Dao_Events_Event, stop);
PHP_METHOD(Dao_Events_Event, isStopped);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_events_event___construct, 0, 0, 2)
	ZEND_ARG_INFO(0, type)
	ZEND_ARG_INFO(0, source)
	ZEND_ARG_INFO(0, data)
	ZEND_ARG_INFO(0, cancelable)
	ZEND_ARG_INFO(0, flag)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_events_event_method_entry[] = {
	PHP_ME(Dao_Events_Event, __construct, arginfo_dao_events_event___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Events_Event, setName, arginfo_dao_events_eventinterface_setname, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Events_Event, getName, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Events_Event, setType, arginfo_dao_events_eventinterface_settype, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Events_Event, getType, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Events_Event, setSource, arginfo_dao_events_eventinterface_setsource, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Events_Event, getSource, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Events_Event, setData, arginfo_dao_events_eventinterface_setdata, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Events_Event, getData, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Events_Event, setCancelable, arginfo_dao_events_eventinterface_setcancelable, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Events_Event, isCancelable, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Events_Event, getFlag, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Events_Event, stop, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Events_Event, isStopped, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Events_Event, getCancelable, isCancelable, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Events\Event initializer
 */
DAO_INIT_CLASS(Dao_Events_Event){

	DAO_REGISTER_CLASS(Dao\\Events, Event, events_event, dao_events_event_method_entry, 0);

	zend_declare_property_null(dao_events_event_ce, SL("_name"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_events_event_ce, SL("_type"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_events_event_ce, SL("_source"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_events_event_ce, SL("_data"), ZEND_ACC_PROTECTED);
	zend_declare_property_bool(dao_events_event_ce, SL("_stopped"), 0, ZEND_ACC_PROTECTED);
	zend_declare_property_bool(dao_events_event_ce, SL("_cancelable"), 1, ZEND_ACC_PROTECTED);
	zend_declare_property_bool(dao_events_event_ce, SL("_flag"), 0, ZEND_ACC_PROTECTED);

	zend_class_implements(dao_events_event_ce, 1, dao_events_eventinterface_ce);

	return SUCCESS;
}

/**
 * Dao\Events\Event constructor
 *
 * @param string $type
 * @param object $source
 * @param mixed $data
 * @param boolean $cancelable
 */
PHP_METHOD(Dao_Events_Event, __construct){

	zval *type, *source, *data = NULL, *cancelable = NULL, *flag = NULL;

	dao_fetch_params(0, 2, 3, &type, &source, &data, &cancelable, &flag);

	if (!data) {
		data = &DAO_GLOBAL(z_null);
	}

	if (!cancelable) {
		cancelable = &DAO_GLOBAL(z_true);
	}

	if (!flag) {
		flag = &DAO_GLOBAL(z_false);
	}

	dao_update_property(getThis(), SL("_type"), type);
	dao_update_property(getThis(), SL("_source"), source);
	if (Z_TYPE_P(data) != IS_NULL) {
		dao_update_property(getThis(), SL("_data"), data);
	}

	if (DAO_IS_NOT_TRUE(cancelable)) {
		dao_update_property(getThis(), SL("_cancelable"), cancelable);
	}
	if (DAO_IS_TRUE(flag)) {
		dao_update_property(getThis(), SL("_flag"), flag);
	}
}

/**
 * Set the event's name
 *
 * @param string $eventName
 */
PHP_METHOD(Dao_Events_Event, setName){

	zval *event_name;

	dao_fetch_params(0, 1, 0, &event_name);

	dao_update_property(getThis(), SL("_name"), event_name);

}

/**
 * Returns the event's name
 *
 * @return string
 */
PHP_METHOD(Dao_Events_Event, getName){


	RETURN_MEMBER(getThis(), "_name");
}

/**
 * Set the event's type
 *
 * @param string $eventType
 */
PHP_METHOD(Dao_Events_Event, setType){

	zval *event_type;

	dao_fetch_params(0, 1, 0, &event_type);

	dao_update_property(getThis(), SL("_type"), event_type);

}

/**
 * Returns the event's type
 *
 * @return string
 */
PHP_METHOD(Dao_Events_Event, getType){


	RETURN_MEMBER(getThis(), "_type");
}

/**
 * Sets the event's source
 *
 * @return object
 */
PHP_METHOD(Dao_Events_Event, setSource){

	zval *event_source;

	dao_fetch_params(0, 1, 0, &event_source);

	dao_update_property(getThis(), SL("_source"), event_source);
}

/**
 * Returns the event's source
 *
 * @return object
 */
PHP_METHOD(Dao_Events_Event, getSource){


	RETURN_MEMBER(getThis(), "_source");
}

/**
 * Set the event's data
 *
 * @param string $data
 */
PHP_METHOD(Dao_Events_Event, setData){

	zval *data;

	dao_fetch_params(0, 1, 0, &data);

	dao_update_property(getThis(), SL("_data"), data);

}

/**
 * Returns the event's data
 *
 * @return mixed
 */
PHP_METHOD(Dao_Events_Event, getData){


	RETURN_MEMBER(getThis(), "_data");
}

/**
 * Sets if the event is cancelable
 *
 * @param boolean $cancelable
 */
PHP_METHOD(Dao_Events_Event, setCancelable){

	zval *cancelable;

	dao_fetch_params(0, 1, 0, &cancelable);

	dao_update_property(getThis(), SL("_cancelable"), cancelable);
}

/**
 * Check whether the event is cancelable
 *
 * @return boolean
 */
PHP_METHOD(Dao_Events_Event, isCancelable){


	RETURN_MEMBER(getThis(), "_cancelable");
}

/**
 * Returns the event's flag
 *
 * @return mixed
 */
PHP_METHOD(Dao_Events_Event, getFlag){


	RETURN_MEMBER(getThis(), "_flag");
}

/**
 * Stops the event preventing propagation
 */
PHP_METHOD(Dao_Events_Event, stop){

	zval cancelable = {};

	dao_read_property(&cancelable, getThis(), SL("_cancelable"), PH_NOISY|PH_READONLY);
	if (zend_is_true(&cancelable)) {
		dao_update_property_bool(getThis(), SL("_stopped"), 1);
	} else {
		DAO_THROW_EXCEPTION_STR(dao_events_exception_ce, "Trying to cancel a non-cancelable event");
		return;
	}
}

/**
 * Check whether the event is currently stopped
 */
PHP_METHOD(Dao_Events_Event, isStopped){


	RETURN_MEMBER(getThis(), "_stopped");
}
