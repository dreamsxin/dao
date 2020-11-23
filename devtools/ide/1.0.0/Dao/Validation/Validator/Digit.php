<?php 

namespace Dao\Validation\Validator {

	/**
	 * Dao\Validation\Validator\Digit
	 *
	 * Checks if a value has a correct DIGIT format
	 *
	 *<code>
	 *use Dao\Validation\Validator\Digit as DigitValidator;
	 *
	 *$validator->add('digit', new DigitValidator(array(
	 *   'message' => 'The digit is not valid'
	 *)));
	 *</code>
	 */
	
	class Digit extends \Dao\Validation\Validator implements \Dao\Validation\ValidatorInterface {

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
