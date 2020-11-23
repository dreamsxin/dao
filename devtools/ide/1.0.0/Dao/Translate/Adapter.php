<?php 

namespace Dao\Translate {

	/**
	 * Dao\Translate\Adapter
	 *
	 * Base class for Dao\Translate adapters
	 */
	
	abstract class Adapter implements \ArrayAccess, \Dao\Translate\AdapterInterface {

		/**
		 * Returns the translation string of the given key
		 *
		 * @param string $translateKey
		 * @param array $placeholders
		 * @return string
		 */
		public function t($translateKey, $placeholders=null){ }


		/**
		 * Sets a translation value
		 *
		 * @param 	string $offset
		 * @param 	string $value
		 */
		public function offsetSet($property, $value){ }


		/**
		 * Check whether a translation key exists
		 *
		 * @param string $translateKey
		 * @return boolean
		 */
		public function offsetExists($property){ }


		/**
		 * Unsets a translation from the dictionary
		 *
		 * @param string $offset
		 */
		public function offsetUnset($property){ }


		/**
		 * Returns the translation related to the given key
		 *
		 * @param string $translateKey
		 * @return string
		 */
		public function offsetGet($property){ }


		public function _($translateKey, $placeholders=null){ }

	}
}
