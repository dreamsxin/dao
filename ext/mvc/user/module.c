
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

#include "mvc/user/module.h"
#include "mvc/../user/module.h"
#include "kernel/main.h"

/**
 * Dao\Mvc\User\Module
 *
 * This class can be used to provide user modules easy access to services
 * in the application
 */
zend_class_entry *dao_mvc_user_module_ce;

/**
 * Dao\Mvc\User\Module initializer
 */
DAO_INIT_CLASS(Dao_Mvc_User_Module){

	DAO_REGISTER_CLASS_EX(Dao\\Mvc\\User, Module, mvc_user_module, dao_user_module_ce, NULL, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	return SUCCESS;
}
