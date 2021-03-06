
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

#include "mvc/micro/middlewareinterface.h"
#include "kernel/main.h"

zend_class_entry *dao_mvc_micro_middlewareinterface_ce;

static const zend_function_entry dao_mvc_micro_middlewareinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Mvc_Micro_MiddlewareInterface, call, arginfo_dao_mvc_micro_middlewareinterface_call)
	PHP_FE_END
};

/**
 * Dao\Mvc\Micro\MiddlewareInterface initializer
 */
DAO_INIT_CLASS(Dao_Mvc_Micro_MiddlewareInterface){

	DAO_REGISTER_INTERFACE(Dao\\Mvc\\Micro, MiddlewareInterface, mvc_micro_middlewareinterface, dao_mvc_micro_middlewareinterface_method_entry);

	return SUCCESS;
}

/**
 * Calls the middleware
 *
 * @param Dao\Mvc\Micro $application
 */
DAO_DOC_METHOD(Dao_Mvc_Micro_MiddlewareInterface, call);

