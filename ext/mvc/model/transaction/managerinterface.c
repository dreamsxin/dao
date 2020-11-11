
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

#include "mvc/model/transaction/managerinterface.h"
#include "kernel/main.h"

zend_class_entry *dao_mvc_model_transaction_managerinterface_ce;

static const zend_function_entry dao_mvc_model_transaction_managerinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Mvc_Model_Transaction_ManagerInterface, has, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_Transaction_ManagerInterface, get, arginfo_dao_mvc_model_transaction_managerinterface_get)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_Transaction_ManagerInterface, rollbackPendent, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_Transaction_ManagerInterface, commit, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_Transaction_ManagerInterface, rollback, arginfo_dao_mvc_model_transaction_managerinterface_rollback)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_Transaction_ManagerInterface, notifyRollback, arginfo_dao_mvc_model_transaction_managerinterface_notifyrollback)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_Transaction_ManagerInterface, notifyCommit, arginfo_dao_mvc_model_transaction_managerinterface_notifycommit)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_Transaction_ManagerInterface, collectTransactions, arginfo_empty)
	PHP_FE_END
};

/**
 * Dao\Mvc\Model\Transaction\ManagerInterface initializer
 */
DAO_INIT_CLASS(Dao_Mvc_Model_Transaction_ManagerInterface){

	DAO_REGISTER_INTERFACE(Dao\\Mvc\\Model\\Transaction, ManagerInterface, mvc_model_transaction_managerinterface, dao_mvc_model_transaction_managerinterface_method_entry);

	return SUCCESS;
}

/**
 * Checks whether manager has an active transaction
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Mvc_Model_Transaction_ManagerInterface, has);

/**
 * Returns a new Dao\Mvc\Model\Transaction or an already created once
 *
 * @param boolean $autoBegin
 * @return Dao\Mvc\Model\TransactionInterface
 */
DAO_DOC_METHOD(Dao_Mvc_Model_Transaction_ManagerInterface, get);

/**
 * Rollbacks active transactions within the manager
 *
 */
DAO_DOC_METHOD(Dao_Mvc_Model_Transaction_ManagerInterface, rollbackPendent);

/**
 * Commmits active transactions within the manager
 *
 */
DAO_DOC_METHOD(Dao_Mvc_Model_Transaction_ManagerInterface, commit);

/**
 * Rollbacks active transactions within the manager
 * Collect will remove transaction from the manager
 *
 * @param boolean $collect
 */
DAO_DOC_METHOD(Dao_Mvc_Model_Transaction_ManagerInterface, rollback);

/**
 * Notifies the manager about a rollbacked transaction
 *
 * @param Dao\Mvc\Model\TransactionInterface $transaction
 */
DAO_DOC_METHOD(Dao_Mvc_Model_Transaction_ManagerInterface, notifyRollback);

/**
 * Notifies the manager about a commited transaction
 *
 * @param Dao\Mvc\Model\TransactionInterface $transaction
 */
DAO_DOC_METHOD(Dao_Mvc_Model_Transaction_ManagerInterface, notifyCommit);

/**
 * Remove all the transactions from the manager
 *
 */
DAO_DOC_METHOD(Dao_Mvc_Model_Transaction_ManagerInterface, collectTransactions);

