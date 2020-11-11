
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

#include "events/managerinterface.h"
#include "kernel/main.h"

zend_class_entry *dao_events_managerinterface_ce;

static const zend_function_entry dao_events_managerinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Events_ManagerInterface, attach, arginfo_dao_events_managerinterface_attach)
	PHP_ABSTRACT_ME(Dao_Events_ManagerInterface, detach, arginfo_dao_events_managerinterface_detach)
	PHP_ABSTRACT_ME(Dao_Events_ManagerInterface, detachAll, arginfo_dao_events_managerinterface_detachall)
	PHP_ABSTRACT_ME(Dao_Events_ManagerInterface, fire, arginfo_dao_events_managerinterface_fire)
	PHP_ABSTRACT_ME(Dao_Events_ManagerInterface, getListeners, arginfo_dao_events_managerinterface_getlisteners)
	PHP_FE_END
};


/**
 * Dao\Events\ManagerInterface initializer
 */
DAO_INIT_CLASS(Dao_Events_ManagerInterface){

	DAO_REGISTER_INTERFACE(Dao\\Events, ManagerInterface, events_managerinterface, dao_events_managerinterface_method_entry);

	return SUCCESS;
}

/**
 * Attach a listener to the events manager
 *
 * @param string $eventType
 * @param object $handler
 */
DAO_DOC_METHOD(Dao_Events_ManagerInterface, attach);

/**
 * Detach a listener from the events manager
 *
 * @param string $type
 * @param object $handler
 */
DAO_DOC_METHOD(Dao_Events_ManagerInterface, detach);

/**
 * Removes all events from the EventsManager
 *
 * @param string $type
 */
DAO_DOC_METHOD(Dao_Events_ManagerInterface, detachAll);

/**
 * Fires a event in the events manager causing that the acive listeners will be notified about it
 *
 * @param string $eventType
 * @param object $source
 * @param mixed  $data
 * @return mixed
 */
DAO_DOC_METHOD(Dao_Events_ManagerInterface, fire);

/**
 * Returns all the attached listeners of a certain type
 *
 * @param string $type
 * @return array
 */
DAO_DOC_METHOD(Dao_Events_ManagerInterface, getListeners);
