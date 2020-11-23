<?php 

namespace Dao\Validation\Validator {

	/**
	 * Dao\Validation\Validator\Identical
	 *
	 * Checks if a value is identical to other
	 *
	 *<code>
	 *use Dao\Validation\Validator\Identical;
	 *
	 *$validator->add('terms', new Identical(array(
	 *   'value'   => 'yes',
	 *   'message' => 'Terms and conditions must be accepted'
	 *)));
	 *</code>
	 *
	 */
	
	class Identical extends \Dao\Validation\Validator implements \Dao\Validation\ValidatorInterface {

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
		 * @param string $value
		 * @return boolean
		 */
		public function valid($value){ }

	}
}
