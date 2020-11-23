<?php 

namespace Dao\Validation\Validator {

	/**
	 * Dao\Validation\Validator\Confirmation
	 *
	 * Checks that two values have the same value
	 *
	 *<code>
	 *use Dao\Validation\Validator\Confirmation;
	 *
	 *$validator->add('password', new Confirmation(array(
	 *   'message' => 'Password doesn\'t match confirmation',
	 *   'with' => 'confirmPassword'
	 *)));
	 *</code>
	 */
	
	class Confirmation extends \Dao\Validation\Validator implements \Dao\Validation\ValidatorInterface {

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
		 * @param string $with_value
		 * @return boolean
		 */
		public function valid($value){ }

	}
}
