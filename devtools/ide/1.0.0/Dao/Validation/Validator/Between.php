<?php 

namespace Dao\Validation\Validator {

	/**
	 * Dao\Validation\Validator\Between
	 *
	 * Validates that a value is between a range of two values
	 *
	 *<code>
	 *use Dao\Validation\Validator\Between;
	 *
	 *$validator->add('name', new Between(array(
	 *   'minimum' => 0,
	 *   'maximum' => 100,
	 *   'message' => 'The price must be between 0 and 100'
	 *)));
	 *</code>
	 */
	
	class Between extends \Dao\Validation\Validator implements \Dao\Validation\ValidatorInterface {

		/**
		 * Executes the validation
		 *
		 * @param \Dao\Validation $validator
		 * @param string $attribute
		 * @return boolean
		 */
		public function validate(\Dao\ValidationInterface $validator, $attribute, $allowEmpty=null){ }


		/**
		 * Executes the validation
		 *
		 * @param int $value
		 * @param int $minimum
		 * @param int $maximum
		 * @return boolean
		 */
		public function valid($value){ }

	}
}
