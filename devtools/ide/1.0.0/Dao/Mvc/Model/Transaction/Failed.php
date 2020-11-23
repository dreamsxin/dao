<?php 

namespace Dao\Mvc\Model\Transaction {

	/**
	 * Dao\Mvc\Model\Transaction\Failed
	 *
	 * This class will be thrown to exit a try/catch block for isolated transactions
	 */
	
	class Failed extends \Dao\Mvc\Model\Transaction\Exception implements \Throwable, \Stringable {

		protected $_record;

		/**
		 * \Dao\Mvc\Model\Transaction\Failed constructor
		 *
		 * @param string $message
		 * @param \Dao\Mvc\ModelInterface $record
		 */
		public function __construct($message, $record){ }


		/**
		 * Returns validation record messages which stop the transaction
		 *
		 * @return \Dao\Mvc\Model\MessageInterface[]
		 */
		public function getRecordMessages(){ }


		/**
		 * Returns validation record messages which stop the transaction
		 *
		 * @return \Dao\Mvc\ModelInterface
		 */
		public function getRecord(){ }

	}
}
