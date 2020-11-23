<?php 

namespace Dao\Sync {

	/**
	 * Dao\Sync\Sharedmemory
	 *
	 */
	
	class Sharedmemory {

		/**
		 * \Dao\Sync\Sharedmemory constructor
		 *
		 * @param string $name
		 * @param int $size
		 */
		public function __construct($name, $size){ }


		/**
		 * Returns whether or not this shared memory segment is the first time accessed (i.e. not initialized)
		 *
		 * @return boolean
		 */
		public function first(){ }


		public function size(){ }


		/**
		 * Copies data from shared memory
		 *
		 * @param int $start
		 * @param int $length
		 * @return string
		 */
		public function read($start=null, $length=null){ }


		/**
		 * Copies data to shared memory
		 *
		 * @param string $str
		 * @param int $start
		 * @return int
		 */
		public function write($string, $start=null){ }

	}
}
