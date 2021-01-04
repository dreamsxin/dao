
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

#include "user/plugin.h"
#include "di/injectable.h"
#include "kernel/main.h"

/**
 * Dao\User\Plugin
 *
 * This class can be used to provide user plugins an easy access to services
 * in the application
 */
zend_class_entry *dao_user_plugin_ce;

/**
 * Dao\User\Plugin initializer
 */
DAO_INIT_CLASS(Dao_User_Plugin){

	DAO_REGISTER_CLASS_EX(Dao\\User, Plugin, user_plugin, dao_di_injectable_ce, NULL, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	return SUCCESS;
}
