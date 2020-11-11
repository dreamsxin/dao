
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

#include "mvc/model/transactioninterface.h"
#include "kernel/main.h"

zend_class_entry *dao_mvc_model_transactioninterface_ce;

static const zend_function_entry dao_mvc_model_transactioninterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Mvc_Model_TransactionInterface, setTransactionManager, arginfo_dao_mvc_model_transactioninterface_settransactionmanager)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_TransactionInterface, begin, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_TransactionInterface, commit, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_TransactionInterface, rollback, arginfo_dao_mvc_model_transactioninterface_rollback)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_TransactionInterface, getConnection, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_TransactionInterface, setIsNewTransaction, arginfo_dao_mvc_model_transactioninterface_setisnewtransaction)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_TransactionInterface, setRollbackOnAbort, arginfo_dao_mvc_model_transactioninterface_setrollbackonabort)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_TransactionInterface, isManaged, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_TransactionInterface, getMessages, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_TransactionInterface, isValid, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_TransactionInterface, setRollbackedRecord, arginfo_dao_mvc_model_transactioninterface_setrollbackedrecord)
	PHP_FE_END
};

/**
 * Dao\Mvc\Model\TransactionInterface initializer
 */
DAO_INIT_CLASS(Dao_Mvc_Model_TransactionInterface){

	DAO_REGISTER_INTERFACE(Dao\\Mvc\\Model, TransactionInterface, mvc_model_transactioninterface, dao_mvc_model_transactioninterface_method_entry);

	return SUCCESS;
}

/**
 * Sets transaction manager related to the transaction
 *
 * @param Dao\Mvc\Model\Transaction\ManagerInterface $manager
 */
DAO_DOC_METHOD(Dao_Mvc_Model_TransactionInterface, setTransactionManager);

/**
 * Starts the transaction
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Mvc_Model_TransactionInterface, begin);

/**
 * Commits the transaction
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Mvc_Model_TransactionInterface, commit);

/**
 * Rollbacks the transaction
 *
 * @param  string $rollbackMessage
 * @param  Dao\Mvc\ModelInterface $rollbackRecord
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Mvc_Model_TransactionInterface, rollback);

/**
 * Returns connection related to transaction
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Mvc_Model_TransactionInterface, getConnection);

/**
 * Sets if is a reused transaction or new once
 *
 * @param boolean $isNew
 */
DAO_DOC_METHOD(Dao_Mvc_Model_TransactionInterface, setIsNewTransaction);

/**
 * Sets flag to rollback on abort the HTTP connection
 *
 * @param boolean $rollbackOnAbort
 */
DAO_DOC_METHOD(Dao_Mvc_Model_TransactionInterface, setRollbackOnAbort);

/**
 * Checks whether transaction is managed by a transaction manager
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Mvc_Model_TransactionInterface, isManaged);

/**
 * Returns validations messages from last save try
 *
 * @return array
 */
DAO_DOC_METHOD(Dao_Mvc_Model_TransactionInterface, getMessages);

/**
 * Checks whether internal connection is under an active transaction
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Mvc_Model_TransactionInterface, isValid);

/**
 * Sets object which generates rollback action
 *
 * @param Dao\Mvc\ModelInterface $record
 */
DAO_DOC_METHOD(Dao_Mvc_Model_TransactionInterface, setRollbackedRecord);

