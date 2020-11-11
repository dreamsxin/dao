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

#include "mvc/model/query/status.h"
#include "mvc/model/query/statusinterface.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/fcall.h"

/**
 * Dao\Mvc\Model\Query\Status
 *
 * This class represents the status returned by a PHQL
 * statement like INSERT, UPDATE or DELETE. It offers context
 * information and the related messages produced by the
 * model which finally executes the operations when it fails
 *
 *<code>
 *$phql = "UPDATE Robots SET name = :name:, type = :type:, year = :year: WHERE id = :id:";
 *$status = $app->modelsManager->executeQuery($phql, array(
 *   'id' => 100,
 *   'name' => 'Astroy Boy',
 *   'type' => 'mechanical',
 *   'year' => 1959
 *));
 *
 * //Check if the update was successful
 * if ($status->success() == true) {
 *   echo 'OK';
 * }
 *</code>
 */
zend_class_entry *dao_mvc_model_query_status_ce;

PHP_METHOD(Dao_Mvc_Model_Query_Status, __construct);
PHP_METHOD(Dao_Mvc_Model_Query_Status, getModel);
PHP_METHOD(Dao_Mvc_Model_Query_Status, getMessages);
PHP_METHOD(Dao_Mvc_Model_Query_Status, success);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_query_status___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, success)
	ZEND_ARG_INFO(0, model)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_mvc_model_query_status_method_entry[] = {
	PHP_ME(Dao_Mvc_Model_Query_Status, __construct, arginfo_dao_mvc_model_query_status___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Mvc_Model_Query_Status, getModel, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Query_Status, getMessages, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Query_Status, success, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Mvc\Model\Query\Status initializer
 */
DAO_INIT_CLASS(Dao_Mvc_Model_Query_Status){

	DAO_REGISTER_CLASS(Dao\\Mvc\\Model\\Query, Status, mvc_model_query_status, dao_mvc_model_query_status_method_entry, 0);

	zend_declare_property_null(dao_mvc_model_query_status_ce, SL("_success"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_model_query_status_ce, SL("_model"), ZEND_ACC_PROTECTED);

	zend_class_implements(dao_mvc_model_query_status_ce, 1, dao_mvc_model_query_statusinterface_ce);

	return SUCCESS;
}

/**
 * Dao\Mvc\Model\Query\Status
 *
 * @param boolean $success
 * @param Dao\Mvc\ModelInterface $model
 */
PHP_METHOD(Dao_Mvc_Model_Query_Status, __construct){

	zval *success, *model = NULL;

	dao_fetch_params(0, 1, 1, &success, &model);

	dao_update_property(getThis(), SL("_success"), success);

	if (model) {
		dao_update_property(getThis(), SL("_model"), model);
	}

}

/**
 * Returns the model that executed the action
 *
 * @return Dao\Mvc\ModelInterface
 */
PHP_METHOD(Dao_Mvc_Model_Query_Status, getModel){


	RETURN_MEMBER(getThis(), "_model");
}

/**
 * Returns the messages produced by a failed operation
 *
 * @return Dao\Mvc\Model\MessageInterface[]
 */
PHP_METHOD(Dao_Mvc_Model_Query_Status, getMessages){

	zval model = {};

	dao_read_property(&model, getThis(), SL("_model"), PH_NOISY|PH_READONLY);
	if (Z_TYPE(model) == IS_OBJECT) {
		DAO_RETURN_CALL_METHOD(&model, "getmessages");
		return;
	}

	array_init(return_value);
}

/**
 * Allows to check if the executed operation was successful
 *
 * @return boolean
 */
PHP_METHOD(Dao_Mvc_Model_Query_Status, success){


	RETURN_MEMBER(getThis(), "_success");
}
