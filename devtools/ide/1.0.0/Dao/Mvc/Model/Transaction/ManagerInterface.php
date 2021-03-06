<?php 

namespace Dao\Mvc\Model\Transaction {

	interface ManagerInterface {

		public function has();


		public function get($autoBegin=null);


		public function rollbackPendent();


		public function commit();


		public function rollback($collect=null);


		public function notifyRollback($transaction);


		public function notifyCommit($transaction);


		public function collectTransactions();

	}
}
