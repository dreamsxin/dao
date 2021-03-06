<?php 

namespace Dao\Validation {

	/**
	 * Dao\Validation\Validator
	 *
	 * This is a base class for validators
	 */
	
	abstract class Validator implements \Dao\Validation\ValidatorInterface {

		protected $_options;

		protected $_type;

		/**
		 * \Dao\Validation\Validator constructor
		 *
		 * @param array $options
		 */
		public function __construct($options=null){ }


		/**
		 * Checks if an option is defined
		 *
		 * @param string $key
		 * @return mixed
		 */
		public function isSetOption($key){ }


		/**
		 * Returns an option in the validator's options
		 * Returns null if the option hasn't been set
		 *
		 * @param string $key
		 * @return mixed
		 */
		public function getOption($key){ }


		/**
		 * Sets an option in the validator
		 *
		 * @param string $key
		 * @param mixed $value
		 */
		public function setOption($key, $value){ }


		/**
		 * Gets an type in the validator
		 *
		 * @return mixed
		 */
		public function getType(){ }

	}
}
