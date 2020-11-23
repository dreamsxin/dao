<?php 

namespace Dao\Validation\Validator {

	/**
	 * Dao\Validation\Validator\Date
	 *
	 * Checks if a value has a correct DATE format
	 *
	 *<code>
	 *use Dao\Validation\Validator\Date as DateValidator;
	 *
	 *$validator->add('date', new DateValidator(array(
	 *   'message' => 'The date is not valid'
	 *)));
	 *</code>
	 */
	
	class Date extends \Dao\Validation\Validator implements \Dao\Validation\ValidatorInterface {

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
