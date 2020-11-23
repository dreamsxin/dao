<?php 

namespace Dao\Annotations\Adapter {

	/**
	 * Dao\Annotations\Adapter\Memory
	 *
	 * Stores the parsed annotations in memory. This adapter is the suitable development/testing
	 */
	
	class Memory extends \Dao\Annotations\Adapter implements \Dao\Annotations\AdapterInterface {

		protected $_data;

		/**
		 * Reads parsed annotations from memory
		 *
		 * @param string $key
		 * @return \Dao\Annotations\Reflection
		 */
		public function read($key){ }


		/**
		 * Writes parsed annotations to memory
		 *
		 * @param string $key
		 * @param \Dao\Annotations\Reflection $data
		 */
		public function write($key, $data){ }

	}
}
