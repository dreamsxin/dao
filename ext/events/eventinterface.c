
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

#include "events/eventinterface.h"
#include "kernel/main.h"

zend_class_entry *dao_events_eventinterface_ce;

static const zend_function_entry dao_events_eventinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Events_EventInterface, setType, arginfo_dao_events_eventinterface_settype)
	PHP_ABSTRACT_ME(Dao_Events_EventInterface, getType, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Events_EventInterface, setSource, arginfo_dao_events_eventinterface_setsource)
	PHP_ABSTRACT_ME(Dao_Events_EventInterface, getSource, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Events_EventInterface, setData, arginfo_dao_events_eventinterface_setdata)
	PHP_ABSTRACT_ME(Dao_Events_EventInterface, getData, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Events_EventInterface, setCancelable, arginfo_dao_events_eventinterface_setcancelable)
	PHP_ABSTRACT_ME(Dao_Events_EventInterface, isCancelable, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Events_EventInterface, stop, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Events_EventInterface, isStopped, arginfo_empty)
	PHP_FE_END
};


/**
 * Dao\Events\EventInterface initializer
 */
DAO_INIT_CLASS(Dao_Events_EventInterface){

	DAO_REGISTER_INTERFACE(Dao\\Events, EventInterface, events_eventinterface, dao_events_eventinterface_method_entry);

	return SUCCESS;
}

/**
 * Sets event type
 */
DAO_DOC_METHOD(Dao_Events_EventInterface, setType);

/**
 * Gets event type
 */
DAO_DOC_METHOD(Dao_Events_EventInterface, getType);

/**
 * Sets event source
 */
DAO_DOC_METHOD(Dao_Events_EventInterface, setSource);

/**
 * Gets event source
 */
DAO_DOC_METHOD(Dao_Events_EventInterface, getSource);

/**
 * Sets event data
 */
DAO_DOC_METHOD(Dao_Events_EventInterface, setData);

/**
 * Gets event data
 */
DAO_DOC_METHOD(Dao_Events_EventInterface, getData);

/**
 * Sets event is cancelable
 */
DAO_DOC_METHOD(Dao_Events_EventInterface, setCancelable);

/**
 * Check whether the event is cancelable
 */
DAO_DOC_METHOD(Dao_Events_EventInterface, isCancelable);

/**
 * Stops the event preventing propagation
 */
DAO_DOC_METHOD(Dao_Events_EventInterface, stop);

/**
 * Check whether the event is currently stopped
 */
DAO_DOC_METHOD(Dao_Events_EventInterface, isStopped);

