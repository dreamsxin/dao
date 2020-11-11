
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

#include "mvc/model/query/builderinterface.h"
#include "kernel/main.h"

zend_class_entry *dao_db_builderinterface_ce;

static const zend_function_entry dao_db_builderinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Db_BuilderInterface, execute, arginfo_empty)
	PHP_FE_END
};

/**
 * Dao\Mvc\Model\Query\BuilderInterface initializer
 */
DAO_INIT_CLASS(Dao_Db_BuilderInterface){

	DAO_REGISTER_INTERFACE(Dao\\Db, BuilderInterface, db_builderinterface, dao_db_builderinterface_method_entry);

	return SUCCESS;
}

/**
 * Execute query
 *
 * @return Dao\Db\ResultInterface|boolean
 */
DAO_DOC_METHOD(Dao_Db_BuilderInterface, execute);
