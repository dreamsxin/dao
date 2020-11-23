<?php 

namespace Dao\Translate\Adapter {

	/**
	 * Dao\Translate\Adapter\NativeArray
	 *
	 * Allows to define translation lists using PHP arrays
	 *
	 */
	
	class NativeArray extends \Dao\Translate\Adapter implements \Dao\Translate\AdapterInterface, \ArrayAccess {

		protected $_translate;

		/**
		 * \Dao\Translate\Adapter\NativeArray constructor
		 *
		 * @param array $options
		 */
		public function __construct($options){ }


		/**
		 * Returns the translation related to the given key
		 *
		 * @param string $index
		 * @param array $placeholders
		 * @return string
		 */
		public function query($index, $placeholders=null){ }


		/**
		 * Check whether is defined a translation key in the internal array
		 *
		 * @param string $index
		 * @return boolean
		 */
		public function exists($index){ }

	}
}
