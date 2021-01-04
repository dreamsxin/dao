
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

#include "mvc/user/logic.h"
#include "mvc/../user/logic.h"

#include "kernel/main.h"
#include "kernel/object.h"
#include "kernel/fcall.h"

/**
 * Dao\Mvc\User\Logic
 *
 * This class can be used to provide user business logic an easy access to services in the application
 */
zend_class_entry *dao_mvc_user_logic_ce;

/**
 * Dao\Mvc\User\Logic initializer
 */
DAO_INIT_CLASS(Dao_Mvc_User_Logic){

	DAO_REGISTER_CLASS_EX(Dao\\Mvc\\User, Logic, mvc_user_logic, dao_user_logic_ce, NULL, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	return SUCCESS;
}
