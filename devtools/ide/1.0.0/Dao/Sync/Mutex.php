<?php 

namespace Dao\Sync {

	/**
	 * Dao\Sync\Mutex
	 *
	 */
	
	class Mutex {

		/**
		 * \Dao\Sync\Mutex constructor
		 *
		 * @param string $name
		 */
		public function __construct($name=null){ }


		/**
		 * Locks a mutex object
		 */
		public function lock($wait=null){ }


		/**
		 * Unlocks a mutex object
		 *
		 * @param boolean $all
		 */
		public function unlock($all=null){ }

	}
}
