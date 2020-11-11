
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

#include "mvc/model/transaction/manager.h"
#include "mvc/model/transaction/managerinterface.h"
#include "mvc/model/transaction.h"
#include "mvc/model/transaction/exception.h"
#include "di.h"
#include "diinterface.h"
#include "di/injectable.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/exception.h"
#include "kernel/array.h"
#include "kernel/file.h"
#include "kernel/operators.h"

/**
 * Dao\Mvc\Model\Transaction\Manager
 *
 * A transaction acts on a single database connection. If you have multiple class-specific
 * databases, the transaction will not protect interaction among them.
 *
 * This class manages the objects that compose a transaction.
 * A trasaction produces a unique connection that is passed to every
 * object part of the transaction.
 *
  *<code>
 *try {
 *
 *  use Dao\Mvc\Model\Transaction\Manager as TransactionManager;
 *
 *  $transactionManager = new TransactionManager();
 *
 *  $transaction = $transactionManager->get();
 *
 *  $robot = new Robots();
 *  $robot->setTransaction($transaction);
 *  $robot->name = 'WALL·E';
 *  $robot->created_at = date('Y-m-d');
 *  if($robot->save()==false){
 *    $transaction->rollback("Can't save robot");
 *  }
 *
 *  $robotPart = new RobotParts();
 *  $robotPart->setTransaction($transaction);
 *  $robotPart->type = 'head';
 *  if($robotPart->save()==false){
 *    $transaction->rollback("Can't save robot part");
 *  }
 *
 *  $transaction->commit();
 *
 *}
 *catch(Dao\Mvc\Model\Transaction\Failed $e){
 *  echo 'Failed, reason: ', $e->getMessage();
 *}
 *
 *</code>
 *
 */
zend_class_entry *dao_mvc_model_transaction_manager_ce;

PHP_METHOD(Dao_Mvc_Model_Transaction_Manager, __construct);
PHP_METHOD(Dao_Mvc_Model_Transaction_Manager, setDbService);
PHP_METHOD(Dao_Mvc_Model_Transaction_Manager, getDbService);
PHP_METHOD(Dao_Mvc_Model_Transaction_Manager, setRollbackPendent);
PHP_METHOD(Dao_Mvc_Model_Transaction_Manager, getRollbackPendent);
PHP_METHOD(Dao_Mvc_Model_Transaction_Manager, has);
PHP_METHOD(Dao_Mvc_Model_Transaction_Manager, get);
PHP_METHOD(Dao_Mvc_Model_Transaction_Manager, getOrCreateTransaction);
PHP_METHOD(Dao_Mvc_Model_Transaction_Manager, rollbackPendent);
PHP_METHOD(Dao_Mvc_Model_Transaction_Manager, commit);
PHP_METHOD(Dao_Mvc_Model_Transaction_Manager, rollback);
PHP_METHOD(Dao_Mvc_Model_Transaction_Manager, notifyRollback);
PHP_METHOD(Dao_Mvc_Model_Transaction_Manager, notifyCommit);
PHP_METHOD(Dao_Mvc_Model_Transaction_Manager, _collectTransaction);
PHP_METHOD(Dao_Mvc_Model_Transaction_Manager, collectTransactions);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_transaction_manager___construct, 0, 0, 0)
	ZEND_ARG_INFO(0, dependencyInjector)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_transaction_manager_setdbservice, 0, 0, 1)
	ZEND_ARG_INFO(0, service)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_transaction_manager_setrollbackpendent, 0, 0, 1)
	ZEND_ARG_INFO(0, rollbackPendent)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_transaction_manager_getorcreatetransaction, 0, 0, 0)
	ZEND_ARG_INFO(0, autoBegin)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_mvc_model_transaction_manager_method_entry[] = {
	PHP_ME(Dao_Mvc_Model_Transaction_Manager, __construct, arginfo_dao_mvc_model_transaction_manager___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Mvc_Model_Transaction_Manager, setDbService, arginfo_dao_mvc_model_transaction_manager_setdbservice, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Transaction_Manager, getDbService, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Transaction_Manager, setRollbackPendent, arginfo_dao_mvc_model_transaction_manager_setrollbackpendent, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Transaction_Manager, getRollbackPendent, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Transaction_Manager, has, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Transaction_Manager, get, arginfo_dao_mvc_model_transaction_managerinterface_get, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Transaction_Manager, getOrCreateTransaction, arginfo_dao_mvc_model_transaction_manager_getorcreatetransaction, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Transaction_Manager, rollbackPendent, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Transaction_Manager, commit, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Transaction_Manager, rollback, arginfo_dao_mvc_model_transaction_managerinterface_rollback, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Transaction_Manager, notifyRollback, arginfo_dao_mvc_model_transaction_managerinterface_notifyrollback, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Transaction_Manager, notifyCommit, arginfo_dao_mvc_model_transaction_managerinterface_notifycommit, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Transaction_Manager, _collectTransaction, arginfo_empty, ZEND_ACC_PROTECTED)
	PHP_ME(Dao_Mvc_Model_Transaction_Manager, collectTransactions, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Mvc\Model\Transaction\Manager initializer
 */
DAO_INIT_CLASS(Dao_Mvc_Model_Transaction_Manager){

	DAO_REGISTER_CLASS_EX(Dao\\Mvc\\Model\\Transaction, Manager, mvc_model_transaction_manager, dao_di_injectable_ce, dao_mvc_model_transaction_manager_method_entry, 0);

	zend_declare_property_bool(dao_mvc_model_transaction_manager_ce, SL("_initialized"), 0, ZEND_ACC_PROTECTED);
	zend_declare_property_bool(dao_mvc_model_transaction_manager_ce, SL("_rollbackPendent"), 1, ZEND_ACC_PROTECTED);
	zend_declare_property_long(dao_mvc_model_transaction_manager_ce, SL("_number"), 0, ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_model_transaction_manager_ce, SL("_service"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_model_transaction_manager_ce, SL("_transactions"), ZEND_ACC_PROTECTED);

	zend_class_implements(dao_mvc_model_transaction_manager_ce, 1, dao_mvc_model_transaction_managerinterface_ce);

	return SUCCESS;
}

/**
 * Dao\Mvc\Model\Transaction\Manager constructor
 *
 * @param Dao\DiInterface $dependencyInjector
 */
PHP_METHOD(Dao_Mvc_Model_Transaction_Manager, __construct){

	zval *dependency_injector = NULL;

	dao_fetch_params(0, 0, 1, &dependency_injector);

	if (dependency_injector && Z_TYPE_P(dependency_injector) != IS_NULL) {
		DAO_CALL_METHOD(NULL, getThis(), "setdi", dependency_injector);
	}
}

/**
 * Sets the database service used to run the isolated transactions
 *
 * @param string $service
 * @return Dao\Mvc\Model\Transaction\Manager
 */
PHP_METHOD(Dao_Mvc_Model_Transaction_Manager, setDbService){

	zval *service;

	dao_fetch_params(0, 1, 0, &service);

	dao_update_property(getThis(), SL("_service"), service);
	RETURN_THIS();
}

/**
 * Returns the database service used to isolate the transaction
 *
 * @return string
 */
PHP_METHOD(Dao_Mvc_Model_Transaction_Manager, getDbService){


	RETURN_MEMBER(getThis(), "_service");
}

/**
 * Set if the transaction manager must register a shutdown function to clean up pendent transactions
 *
 * @param boolean $rollbackPendent
 * @return Dao\Mvc\Model\Transaction\Manager
 */
PHP_METHOD(Dao_Mvc_Model_Transaction_Manager, setRollbackPendent){

	zval *rollback_pendent;

	dao_fetch_params(0, 1, 0, &rollback_pendent);

	dao_update_property(getThis(), SL("_rollbackPendent"), rollback_pendent);

}

/**
 * Check if the transaction manager is registering a shutdown function to clean up pendent transactions
 *
 * @return boolean
 */
PHP_METHOD(Dao_Mvc_Model_Transaction_Manager, getRollbackPendent){


	RETURN_MEMBER(getThis(), "_rollbackPendent");
}

/**
 * Checks whether the manager has an active transaction
 *
 * @return boolean
 */
PHP_METHOD(Dao_Mvc_Model_Transaction_Manager, has){

	zval number = {};

	dao_read_property(&number, getThis(), SL("_number"), PH_NOISY|PH_READONLY);
	is_smaller_function(return_value, &DAO_GLOBAL(z_zero), &number);
}

/**
 * Returns a new Dao\Mvc\Model\Transaction or an already created once
 * This method registers a shutdown function to rollback active connections
 *
 * @param boolean $autoBegin
 * @return Dao\Mvc\Model\TransactionInterface
 */
PHP_METHOD(Dao_Mvc_Model_Transaction_Manager, get){

	zval *auto_begin = NULL, initialized = {}, rollback_pendent = {};

	dao_fetch_params(0, 0, 1, &auto_begin);

	if (!auto_begin) {
		auto_begin = &DAO_GLOBAL(z_true);
	}

	dao_read_property(&initialized, getThis(), SL("_initialized"), PH_NOISY|PH_READONLY);
	if (zend_is_true(&initialized)) {
		dao_read_property(&rollback_pendent, getThis(), SL("_rollbackPendent"), PH_READONLY);
		if (zend_is_true(&rollback_pendent)) {
			array_init_size(&rollback_pendent, 2);
			dao_array_append(&rollback_pendent, getThis(), PH_COPY);
			add_next_index_stringl(&rollback_pendent, SL("rollbackPendent"));
			DAO_CALL_FUNCTION(NULL, "register_shutdown_function", &rollback_pendent);
			zval_ptr_dtor(&rollback_pendent);
		}

		dao_update_property_bool(getThis(), SL("_initialized"), 1);
	}

	DAO_RETURN_CALL_METHOD(getThis(), "getorcreatetransaction");
}

/**
 * Create/Returns a new transaction or an existing one
 *
 * @param boolean $autoBegin
 * @return Dao\Mvc\Model\TransactionInterface
 */
PHP_METHOD(Dao_Mvc_Model_Transaction_Manager, getOrCreateTransaction){

	zval *auto_begin = NULL, dependency_injector = {}, number = {}, transactions = {}, *transaction, service = {};

	dao_fetch_params(1, 0, 1, &auto_begin);

	if (!auto_begin) {
		auto_begin = &DAO_GLOBAL(z_true);
	}

	dao_read_property(&number, getThis(), SL("_number"), PH_NOISY|PH_READONLY);
	if (zend_is_true(&number)) {
		dao_read_property(&transactions, getThis(), SL("_transactions"), PH_NOISY|PH_READONLY);
		if (Z_TYPE(transactions) == IS_ARRAY) {
			ZEND_HASH_FOREACH_VAL(Z_ARRVAL(transactions), transaction) {
				if (Z_TYPE_P(transaction) == IS_OBJECT) {
					DAO_MM_CALL_METHOD(NULL, transaction, "setisnewtransaction", &DAO_GLOBAL(z_false));
					RETURN_MM_CTOR(transaction);
				}
			} ZEND_HASH_FOREACH_END();

		}
	}

	DAO_MM_CALL_METHOD(&dependency_injector, getThis(), "getdi");
	DAO_MM_ADD_ENTRY(&dependency_injector);
	if (Z_TYPE(dependency_injector) != IS_OBJECT) {
		DAO_MM_THROW_EXCEPTION_STR(dao_mvc_model_transaction_exception_ce, "A dependency injector container is required to obtain the services related to the ORM");
		return;
	}

	dao_read_property(&service, getThis(), SL("_service"), PH_NOISY|PH_READONLY);

	object_init_ex(return_value, dao_mvc_model_transaction_ce);
	DAO_MM_CALL_METHOD(NULL, return_value, "__construct", &dependency_injector, auto_begin, &service);

	DAO_MM_CALL_METHOD(NULL, return_value, "settransactionmanager", getThis());
	dao_update_property_array_append(getThis(), SL("_transactions"), return_value);
	dao_property_incr(getThis(), SL("_number"));
	RETURN_MM();
}

/**
 * Rollbacks active transactions within the manager
 *
 */
PHP_METHOD(Dao_Mvc_Model_Transaction_Manager, rollbackPendent){


	DAO_CALL_METHOD(NULL, getThis(), "rollback");
}

/**
 * Commmits active transactions within the manager
 *
 */
PHP_METHOD(Dao_Mvc_Model_Transaction_Manager, commit){

	zval transactions = {}, *transaction, connection = {}, is_under_transaction = {};

	dao_read_property(&transactions, getThis(), SL("_transactions"), PH_NOISY|PH_READONLY);
	if (Z_TYPE(transactions) == IS_ARRAY) {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL(transactions), transaction) {
			DAO_CALL_METHOD(&connection, transaction, "getconnection");
			DAO_CALL_METHOD(&is_under_transaction, &connection, "isundertransaction");
			if (zend_is_true(&is_under_transaction)) {
				DAO_CALL_METHOD(NULL, &connection, "commit");
			}
			zval_ptr_dtor(&connection);
		} ZEND_HASH_FOREACH_END();

	}
}

/**
 * Rollbacks active transactions within the manager
 * Collect will remove transaction from the manager
 *
 * @param boolean $collect
 */
PHP_METHOD(Dao_Mvc_Model_Transaction_Manager, rollback){

	zval *collect = NULL, transactions = {}, *transaction, connection = {}, is_under_transaction = {};

	dao_fetch_params(0, 0, 1, &collect);

	if (!collect) {
		collect = &DAO_GLOBAL(z_true);
	}

	dao_read_property(&transactions, getThis(), SL("_transactions"), PH_NOISY|PH_READONLY);
	if (Z_TYPE(transactions) == IS_ARRAY) {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL(transactions), transaction) {
			DAO_CALL_METHOD(&connection, transaction, "getconnection");
			DAO_CALL_METHOD(&is_under_transaction, &connection, "isundertransaction");
			if (zend_is_true(&is_under_transaction)) {
				DAO_CALL_METHOD(NULL, &connection, "rollback");
				DAO_CALL_METHOD(NULL, &connection, "close");
			}
			zval_ptr_dtor(&connection);

			if (zend_is_true(collect)) {
				DAO_CALL_METHOD(NULL, getThis(), "_collecttransaction", transaction);
			}
		} ZEND_HASH_FOREACH_END();

	}
}

/**
 * Notifies the manager about a rollbacked transaction
 *
 * @param Dao\Mvc\Model\TransactionInterface $transaction
 */
PHP_METHOD(Dao_Mvc_Model_Transaction_Manager, notifyRollback){

	zval *transaction;

	dao_fetch_params(0, 1, 0, &transaction);

	DAO_CALL_METHOD(NULL, getThis(), "_collecttransaction", transaction);
}

/**
 * Notifies the manager about a commited transaction
 *
 * @param Dao\Mvc\Model\TransactionInterface $transaction
 */
PHP_METHOD(Dao_Mvc_Model_Transaction_Manager, notifyCommit){

	zval *transaction;

	dao_fetch_params(0, 1, 0, &transaction);

	DAO_CALL_METHOD(NULL, getThis(), "_collecttransaction", transaction);
}

/**
 * Removes transactions from the TransactionManager
 *
 * @param Dao\Mvc\Model\TransactionInterface $transaction
 */
PHP_METHOD(Dao_Mvc_Model_Transaction_Manager, _collectTransaction){

	zval *transaction, transactions = {}, new_transactions = {}, *managed_transaction;

	dao_fetch_params(0, 1, 0, &transaction);

	dao_read_property(&transactions, getThis(), SL("_transactions"), PH_NOISY|PH_READONLY);
	if (dao_fast_count_ev(&transactions)) {
		array_init(&new_transactions);

		ZEND_HASH_FOREACH_VAL(Z_ARRVAL(transactions), managed_transaction) {
			if (DAO_IS_EQUAL(managed_transaction, transaction)) {
				dao_array_append(&new_transactions, transaction, PH_COPY);
				dao_property_decr(getThis(), SL("_number"));
			}
		} ZEND_HASH_FOREACH_END();

		dao_update_property(getThis(), SL("_transactions"), &new_transactions);
		zval_ptr_dtor(&new_transactions);
	}
}

/**
 * Remove all the transactions from the manager
 *
 */
PHP_METHOD(Dao_Mvc_Model_Transaction_Manager, collectTransactions){

	zval transactions = {};

	dao_read_property(&transactions, getThis(), SL("_transactions"), PH_NOISY|PH_READONLY);
	if (Z_TYPE(transactions) == IS_ARRAY) {
		ZEND_HASH_FOREACH(Z_ARRVAL(transactions), 0) {
			dao_property_decr(getThis(), SL("_number"));
		} ZEND_HASH_FOREACH_END();

		dao_update_property_null(getThis(), SL("_transactions"));
	}
}
