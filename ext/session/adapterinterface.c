
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

#include "session/adapterinterface.h"
#include "kernel/main.h"

zend_class_entry *dao_session_adapterinterface_ce;

static const zend_function_entry dao_session_adapterinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Session_AdapterInterface, start, arginfo_dao_session_adapterinterface_start)
	PHP_ABSTRACT_ME(Dao_Session_AdapterInterface, setOptions, arginfo_dao_session_adapterinterface_setoptions)
	PHP_ABSTRACT_ME(Dao_Session_AdapterInterface, getOptions, arginfo_dao_session_adapterinterface_getoptions)
	PHP_ABSTRACT_ME(Dao_Session_AdapterInterface, get, arginfo_dao_session_adapterinterface_get)
	PHP_ABSTRACT_ME(Dao_Session_AdapterInterface, set, arginfo_dao_session_adapterinterface_set)
	PHP_ABSTRACT_ME(Dao_Session_AdapterInterface, sets, arginfo_dao_session_adapterinterface_sets)
	PHP_ABSTRACT_ME(Dao_Session_AdapterInterface, has, arginfo_dao_session_adapterinterface_has)
	PHP_ABSTRACT_ME(Dao_Session_AdapterInterface, remove, arginfo_dao_session_adapterinterface_remove)
	PHP_ABSTRACT_ME(Dao_Session_AdapterInterface, getId, arginfo_dao_session_adapterinterface_getid)
	PHP_ABSTRACT_ME(Dao_Session_AdapterInterface, isStarted, arginfo_dao_session_adapterinterface_isstarted)
	PHP_ABSTRACT_ME(Dao_Session_AdapterInterface, destroy, arginfo_dao_session_adapterinterface_destroy)
	PHP_FE_END
};

/**
 * Dao\Session\AdapterInterface initializer
 */
DAO_INIT_CLASS(Dao_Session_AdapterInterface){

	DAO_REGISTER_INTERFACE(Dao\\Session, AdapterInterface, session_adapterinterface, dao_session_adapterinterface_method_entry);

	return SUCCESS;
}

/**
 * Starts session, optionally using an adapter
 *
 * @param array $options
 */
DAO_DOC_METHOD(Dao_Session_AdapterInterface, start);

/**
 * Sets session options
 *
 * @param array $options
 */
DAO_DOC_METHOD(Dao_Session_AdapterInterface, setOptions);

/**
 * Get internal options
 *
 * @return array
 */
DAO_DOC_METHOD(Dao_Session_AdapterInterface, getOptions);

/**
 * Gets a session variable from an application context
 *
 * @param string $index
 * @param mixed $defaultValue
 * @return mixed
 */
DAO_DOC_METHOD(Dao_Session_AdapterInterface, get);

/**
 * Sets a session variable in an application context
 *
 * @param string $index
 * @param string $value
 */
DAO_DOC_METHOD(Dao_Session_AdapterInterface, set);

/**
 * Sets a session variables in an application context
 *
 * @param array $data
 */
DAO_DOC_METHOD(Dao_Session_AdapterInterface, set);

/**
 * Check whether a session variable is set in an application context
 *
 * @param string $index
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Session_AdapterInterface, has);

/**
 * Removes a session variable from an application context
 *
 * @param string $index
 */
DAO_DOC_METHOD(Dao_Session_AdapterInterface, remove);

/**
 * Returns active session id
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Session_AdapterInterface, getId);

/**
 * Check whether the session has been started
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Session_AdapterInterface, isStarted);

/**
 * Destroys the active session
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Session_AdapterInterface, destroy);
