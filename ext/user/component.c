
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

#include "user/component.h"
#include "di/injectable.h"
#include "kernel/main.h"

/**
 * Dao\User\Component
 *
 * This class can be used to provide user components easy access to services
 * in the application
 */
zend_class_entry *dao_user_component_ce;

/**
 * Dao\User\Component initializer
 */
DAO_INIT_CLASS(Dao_User_Component){

	DAO_REGISTER_CLASS_EX(Dao\\User, Component, user_component, dao_di_injectable_ce, NULL, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	return SUCCESS;
}
