
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

#include "routerinterface.h"
#include "kernel/main.h"

zend_class_entry *dao_routerinterface_ce;

static const zend_function_entry dao_routerinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_RouterInterface, setDefaultModule, arginfo_dao_routerinterface_setdefaultmodule)
	PHP_ABSTRACT_ME(Dao_RouterInterface, getDefaultModule, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_RouterInterface, setDefaultNamespace, arginfo_dao_routerinterface_setdefaultnamespace)
	PHP_ABSTRACT_ME(Dao_RouterInterface, getDefaultNamespace, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_RouterInterface, setDefaultHandler, arginfo_dao_routerinterface_setdefaulthandler)
	PHP_ABSTRACT_ME(Dao_RouterInterface, getDefaultHandler, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_RouterInterface, setDefaultAction, arginfo_dao_routerinterface_setdefaultaction)
	PHP_ABSTRACT_ME(Dao_RouterInterface, getDefaultAction, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_RouterInterface, setDefaultParams, arginfo_dao_routerinterface_setdefaultparams)
	PHP_ABSTRACT_ME(Dao_RouterInterface, getDefaultParams, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_RouterInterface, setCaseSensitive, arginfo_dao_routerinterface_setcasesensitive)
	PHP_ABSTRACT_ME(Dao_RouterInterface, getCaseSensitive, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_RouterInterface, setMode, arginfo_dao_routerinterface_setmode)
	PHP_ABSTRACT_ME(Dao_RouterInterface, getMode, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_RouterInterface, setModuleName, arginfo_dao_routerinterface_setmodulename)
	PHP_ABSTRACT_ME(Dao_RouterInterface, getModuleName, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_RouterInterface, setNamespaceName, arginfo_dao_routerinterface_setnamespacename)
	PHP_ABSTRACT_ME(Dao_RouterInterface, getNamespaceName, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_RouterInterface, setHandlerName, arginfo_dao_routerinterface_sethandlername)
	PHP_ABSTRACT_ME(Dao_RouterInterface, getHandlerName, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_RouterInterface, setActionName, arginfo_dao_routerinterface_setactionname)
	PHP_ABSTRACT_ME(Dao_RouterInterface, getActionName, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_RouterInterface, setParams, arginfo_dao_routerinterface_setparams)
	PHP_ABSTRACT_ME(Dao_RouterInterface, getParams, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_RouterInterface, handle, arginfo_dao_routerinterface_handle)
	PHP_FE_END
};

/**
 * Dao\Mvc\RouterInterface initializer
 */
DAO_INIT_CLASS(Dao_RouterInterface){

	DAO_REGISTER_INTERFACE(Dao, RouterInterface, routerinterface, dao_routerinterface_method_entry);

	return SUCCESS;
}

/**
 * Sets the name of the default module
 *
 * @param string $moduleName
 */
DAO_DOC_METHOD(Dao_RouterInterface, setDefaultModule);

/**
 * Gets the name of the default module
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_RouterInterface, getDefaultModule);

/**
 * Sets the default handle name
 *
 * @param string $handlerName
 */
DAO_DOC_METHOD(Dao_RouterInterface, setDefaultHandler);

/**
 * Gets the default handle name
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_RouterInterface, getDefaultHandler);

/**
 * Sets the default action name
 *
 * @param string $actionName
 */
DAO_DOC_METHOD(Dao_RouterInterface, setDefaultAction);

/**
 * Gets the default action name
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_RouterInterface, getDefaultAction);


/**
 * Sets the default extra params
 *
 * @param array $actionName
 */
DAO_DOC_METHOD(Dao_RouterInterface, setDefaultParams);

/**
 * Gets the default extra params
 *
 * @return array
 */
DAO_DOC_METHOD(Dao_RouterInterface, getDefaultParams);

/**
 * Sets the case sensitive
 *
 * @param boolean $caseSensitive
 */
DAO_DOC_METHOD(Dao_RouterInterface, setCaseSensitive);

/**
 * Gets the case sensitive
 *
 * @return int
 */
DAO_DOC_METHOD(Dao_RouterInterface, getCaseSensitive);

/**
 * Sets the mode
 *
 * @param int $mode
 */
DAO_DOC_METHOD(Dao_RouterInterface, setMode);

/**
 * Gets the mode
 *
 * @return int
 */
DAO_DOC_METHOD(Dao_RouterInterface, getMode);

/**
 * Sets processed module name
 *
 * @param string $moduleName
 */
DAO_DOC_METHOD(Dao_RouterInterface, setModuleName);

/**
 * Returns processed module name
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_RouterInterface, getModuleName);

/**
 * Sets processed namespace name
 *
 * @param string $namespaceName
 */
DAO_DOC_METHOD(Dao_RouterInterface, setNamespaceName);

/**
 * Returns processed namespace name
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_RouterInterface, getNamespaceName);

/**
 * Sets processed handle name
 *
 * @param string $handleName
 */
DAO_DOC_METHOD(Dao_RouterInterface, setHandlerName);

/**
 * Returns processed handle name
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_RouterInterface, getHandlerName);

/**
 * Sets processed action name
 *
 * @param string $actionName
 */
DAO_DOC_METHOD(Dao_RouterInterface, setActionName);

/**
 * Returns processed action name
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_RouterInterface, getActionName);

/**
 * Sets processed extra params
 *
 * @param array $params
 */
DAO_DOC_METHOD(Dao_RouterInterface, setParams);

/**
 * Returns processed extra params
 *
 * @return array
 */
DAO_DOC_METHOD(Dao_RouterInterface, getParams);

/**
 * Handles routing information received from the rewrite engine
 *
 * @param string $uri
 */
DAO_DOC_METHOD(Dao_RouterInterface, handle);
