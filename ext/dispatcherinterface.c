
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

#include "dispatcherinterface.h"
#include "kernel/main.h"

zend_class_entry *dao_dispatcherinterface_ce;

static const zend_function_entry dao_dispatcherinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_DispatcherInterface, setActionSuffix, arginfo_dao_dispatcherinterface_setactionsuffix)
	PHP_ABSTRACT_ME(Dao_DispatcherInterface, setDefaultModule, arginfo_dao_dispatcherinterface_setdefaultmodule)
	PHP_ABSTRACT_ME(Dao_DispatcherInterface, getDefaultModule, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_DispatcherInterface, setDefaultNamespace, arginfo_dao_dispatcherinterface_setdefaultnamespace)
	PHP_ABSTRACT_ME(Dao_DispatcherInterface, getDefaultNamespace, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_DispatcherInterface, setDefaultHandler, arginfo_dao_dispatcherinterface_setdefaulthandler)
	PHP_ABSTRACT_ME(Dao_DispatcherInterface, getDefaultHandler, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_DispatcherInterface, setDefaultAction, arginfo_dao_dispatcherinterface_setdefaultaction)
	PHP_ABSTRACT_ME(Dao_DispatcherInterface, getDefaultAction, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_DispatcherInterface, setModuleName, arginfo_dao_dispatcherinterface_setmodulename)
	PHP_ABSTRACT_ME(Dao_DispatcherInterface, getModuleName, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_DispatcherInterface, setNamespaceName, arginfo_dao_dispatcherinterface_setnamespacename)
	PHP_ABSTRACT_ME(Dao_DispatcherInterface, getNamespaceName, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_DispatcherInterface, setHandlerName, arginfo_dao_dispatcherinterface_sethandlername)
	PHP_ABSTRACT_ME(Dao_DispatcherInterface, getHandlerName, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_DispatcherInterface, setActionName, arginfo_dao_dispatcherinterface_setactionname)
	PHP_ABSTRACT_ME(Dao_DispatcherInterface, getActionName, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_DispatcherInterface, setParams, arginfo_dao_dispatcherinterface_setparams)
	PHP_ABSTRACT_ME(Dao_DispatcherInterface, getParams, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_DispatcherInterface, hasParam, arginfo_dao_dispatcherinterface_hasparam)
	PHP_ABSTRACT_ME(Dao_DispatcherInterface, setParam, arginfo_dao_dispatcherinterface_setparam)
	PHP_ABSTRACT_ME(Dao_DispatcherInterface, getParam, arginfo_dao_dispatcherinterface_getparam)
	PHP_ABSTRACT_ME(Dao_DispatcherInterface, isFinished, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_DispatcherInterface, getReturnedValue, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_DispatcherInterface, dispatch, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_DispatcherInterface, forward, arginfo_dao_dispatcherinterface_forward)
	PHP_ABSTRACT_ME(Dao_DispatcherInterface, camelizeNamespace, arginfo_dao_dispatcherinterface_camelizenamespace)
	PHP_ABSTRACT_ME(Dao_DispatcherInterface, setErrorHandler, arginfo_dao_dispatcherinterface_seterrorhandler)
	PHP_ABSTRACT_ME(Dao_DispatcherInterface, getErrorHandler, arginfo_dao_dispatcherinterface_geterrorhandler)
	PHP_FE_END
};

/**
 * Dao\DispatcherInterface initializer
 */
DAO_INIT_CLASS(Dao_DispatcherInterface){

	DAO_REGISTER_INTERFACE(Dao, DispatcherInterface, dispatcherinterface, dao_dispatcherinterface_method_entry);

	return SUCCESS;
}

/**
 * Sets the default action suffix
 *
 * @param string $actionSuffix
 */
DAO_DOC_METHOD(Dao_DispatcherInterface, setActionSuffix);

/**
 * Sets the default module
 *
 * @param string $module
 */
DAO_DOC_METHOD(Dao_DispatcherInterface, setDefaultModule);

/**
 * Gets the default module
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_DispatcherInterface, getDefaultModule);

/**
 * Sets the default namespace
 *
 * @param string $namespace
 */
DAO_DOC_METHOD(Dao_DispatcherInterface, setDefaultNamespace);

/**
 * Gets the default namespace
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_DispatcherInterface, getDefaultNamespace);

/**
 * Sets the default handler name
 *
 * @param string $handlerName
 */
DAO_DOC_METHOD(Dao_DispatcherInterface, setDefaultHandler);

/**
 * Gets the default handler name
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_DispatcherInterface, getDefaultHandler);

/**
 * Sets the default action name
 *
 * @param string $actionName
 */
DAO_DOC_METHOD(Dao_DispatcherInterface, setDefaultAction);

/**
 * Gets the default action name
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_DispatcherInterface, getDefaultAction);

/**
 * Sets the module name to be dispatched
 *
 * @param string $moduleName
 */
DAO_DOC_METHOD(Dao_DispatcherInterface, setModuleName);

/**
 * Gets last dispatched module name
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_DispatcherInterface, getModuleName);

/**
 * Sets the namespace to be dispatched
 *
 * @param string $namespaceName
 */
DAO_DOC_METHOD(Dao_DispatcherInterface, setNamespaceName);

/**
 * Gets last dispatched namespace
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_DispatcherInterface, getNamespaceName);

/**
 * Sets the handler name to be dispatched
 *
 * @param string $handlerName
 */
DAO_DOC_METHOD(Dao_DispatcherInterface, setHandlerName);

/**
 * Gets last dispatched handler name
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_DispatcherInterface, getHandlerName);

/**
 * Sets the action name to be dispatched
 *
 * @param string $actionName
 */
DAO_DOC_METHOD(Dao_DispatcherInterface, setActionName);

/**
 * Gets last dispatched action name
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_DispatcherInterface, getActionName);

/**
 * Sets action params to be dispatched
 *
 * @param array $params
 */
DAO_DOC_METHOD(Dao_DispatcherInterface, setParams);

/**
 * Gets action params
 *
 * @return array
 */
DAO_DOC_METHOD(Dao_DispatcherInterface, getParams);

/**
 * Check if a param exists
 *
 * @param mixed param
 * @return boolean
 */
DAO_DOC_METHOD(Dao_DispatcherInterface, setParam);

/**
 * Set a param by its name or numeric index
 *
 * @param  mixed $param
 * @param  mixed $value
 */
DAO_DOC_METHOD(Dao_DispatcherInterface, setParam);

/**
 * Gets a param by its name or numeric index
 *
 * @param  mixed $param
 * @param  string|array $filters
 * @return mixed
 */
DAO_DOC_METHOD(Dao_DispatcherInterface, getParam);

/**
 * Checks if the dispatch loop is finished or has more pendent controllers/tasks to disptach
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_DispatcherInterface, isFinished);

/**
 * Returns value returned by the lastest dispatched action
 *
 * @return mixed
 */
DAO_DOC_METHOD(Dao_DispatcherInterface, getReturnedValue);

/**
 * Dispatches a handle action taking into account the routing parameters
 *
 * @return object
 */
DAO_DOC_METHOD(Dao_DispatcherInterface, dispatch);

/**
 * Forwards the execution flow to another controller/action
 *
 * @param array $forward
 */
DAO_DOC_METHOD(Dao_DispatcherInterface, forward);

/**
 * Forwards the execution flow to another controller/action
 *
 * @param array $forward
 */
DAO_DOC_METHOD(Dao_DispatcherInterface, camelizeNamespace);

/**
 * Set error handler
 *
 * @param mixed $handler
 * @param int $exception_code
 * @return Dao\DispatcherInterface
 */
DAO_DOC_METHOD(Dao_DispatcherInterface, setErrorHandler);

/**
 * Get error handler
 *
 * @param int $exception_code
 * @return mixed
 */
DAO_DOC_METHOD(Dao_DispatcherInterface, getErrorHandler);
