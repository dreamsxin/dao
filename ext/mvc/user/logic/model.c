
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

#include "mvc/user/logic/model.h"
#include "mvc/user/logic.h"
#include "di/injectable.h"

#include "kernel/main.h"
#include "kernel/object.h"
#include "kernel/fcall.h"

/**
 * Dao\Mvc\User\Logic
 *
 * This class can be used to provide user business logic an easy access to services in the application
 */
zend_class_entry *dao_mvc_user_logic_model_ce;

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_user_logic_model_method_get, 0, 0, 0)
	ZEND_ARG_INFO(0, arguments)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_user_logic_model_method_getall, 0, 0, 0)
	ZEND_ARG_INFO(0, arguments)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_user_logic_model_method_save, 0, 0, 0)
	ZEND_ARG_INFO(0, arguments)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_user_logic_model_method_create, 0, 0, 0)
	ZEND_ARG_INFO(0, arguments)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_user_logic_model_method_delete, 0, 0, 0)
	ZEND_ARG_INFO(0, arguments)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_user_logic_model_method_deleteall, 0, 0, 0)
	ZEND_ARG_INFO(0, arguments)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_user_logic_model_method_update, 0, 0, 0)
	ZEND_ARG_INFO(0, arguments)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_user_logic_model_method_updateall, 0, 0, 0)
	ZEND_ARG_INFO(0, arguments)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_user_logic_model_method_count, 0, 0, 0)
	ZEND_ARG_INFO(0, arguments)
ZEND_END_ARG_INFO()

const zend_function_entry dao_mvc_user_logic_model_method_entry[] = {
	ZEND_FENTRY(get,			NULL, arginfo_dao_mvc_user_logic_model_method_get,				ZEND_ACC_PUBLIC | ZEND_ACC_ABSTRACT)
	ZEND_FENTRY(getAll,			NULL, arginfo_dao_mvc_user_logic_model_method_getall,			ZEND_ACC_PUBLIC | ZEND_ACC_ABSTRACT)
	ZEND_FENTRY(save,			NULL, arginfo_dao_mvc_user_logic_model_method_save,				ZEND_ACC_PUBLIC | ZEND_ACC_ABSTRACT)
	ZEND_FENTRY(create,			NULL, arginfo_dao_mvc_user_logic_model_method_create,			ZEND_ACC_PUBLIC | ZEND_ACC_ABSTRACT)
	ZEND_FENTRY(delete,			NULL, arginfo_dao_mvc_user_logic_model_method_delete,			ZEND_ACC_PUBLIC | ZEND_ACC_ABSTRACT)
	ZEND_FENTRY(deleteAll,		NULL, arginfo_dao_mvc_user_logic_model_method_deleteall,		ZEND_ACC_PUBLIC | ZEND_ACC_ABSTRACT)
	ZEND_FENTRY(update,			NULL, arginfo_dao_mvc_user_logic_model_method_update,			ZEND_ACC_PUBLIC | ZEND_ACC_ABSTRACT)
	ZEND_FENTRY(updateAll,		NULL, arginfo_dao_mvc_user_logic_model_method_updateall,		ZEND_ACC_PUBLIC | ZEND_ACC_ABSTRACT)
	ZEND_FENTRY(count,			NULL, arginfo_dao_mvc_user_logic_model_method_count,			ZEND_ACC_PUBLIC | ZEND_ACC_ABSTRACT)
	PHP_FE_END
};

/**
 * Dao\Mvc\User\Logic initializer
 */
DAO_INIT_CLASS(Dao_Mvc_User_Logic_Model){

	DAO_REGISTER_CLASS_EX(Dao\\Mvc\\User\\Logic, Model, mvc_user_logic_model, dao_mvc_user_logic_ce, dao_mvc_user_logic_model_method_entry, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	return SUCCESS;
}
