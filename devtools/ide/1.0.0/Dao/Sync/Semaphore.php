<?php 

namespace Dao\Sync {

	/**
	 * Dao\Sync\Semaphore
	 *
	 */
	
	class Semaphore {

		/**
		 * \Dao\Sync\Semaphore constructor
		 *
		 * @param string $name
		 * @param int $initialval
		 * @param boolean $autounlock
		 */
		public function __construct($name=null, $initialval=null, $autounlock=null){ }


		/**
		 * Locks a semaphore object
		 *
		 * @return boolean
		 */
		public function lock($wait=null){ }


		/**
		 * Unlocks a semaphore object
		 *
		 * @return boolean
		 */
		public function unlock($prevcount=null){ }

	}
}
