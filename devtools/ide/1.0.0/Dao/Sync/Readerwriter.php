<?php 

namespace Dao\Sync {

	/**
	 * Dao\Sync\Readerwriter
	 *
	 */
	
	class Readerwriter {

		/**
		 * \Dao\Sync\Readerwriter constructor
		 *
		 * @param string $name
		 * @param boolean $autounlock
		 */
		public function __construct($name=null, $autounlock=null){ }


		/**
		 * Read locks a reader-writer object
		 */
		public function readlock($wait=null){ }


		/**
		 * Read unlocks a reader-writer object
		 *
		 */
		public function readunlock(){ }


		/**
		 * Write locks a reader-writer object
		 */
		public function writelock($wait=null){ }


		/**
		 * Write unlocks a reader-writer object
		 *
		 */
		public function writeunlock(){ }

	}
}
