<?php 

namespace Dao\Annotations\Adapter {

	/**
	 * Dao\Annotations\Adapter\Apc
	 *
	 * Stores the parsed annotations in APC. This adapter is suitable for production
	 *
	 *<code>
	 * $annotations = new \Dao\Annotations\Adapter\Apc();
	 *</code>
	 */
	
	class Apc extends \Dao\Annotations\Adapter implements \Dao\Annotations\AdapterInterface {

		/**
		 * Reads parsed annotations from APC
		 *
		 * @param string $key
		 * @return \Dao\Annotations\Reflection
		 */
		public function read($key){ }


		/**
		 * Writes parsed annotations to APC
		 *
		 * @param string $key
		 * @param \Dao\Annotations\Reflection $data
		 */
		public function write($key, $data){ }

	}
}
