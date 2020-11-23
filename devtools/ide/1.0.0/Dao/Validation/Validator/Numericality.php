<?php 

namespace Dao\Validation\Validator {

	/**
	 * Dao\Validation\Validator\Numericality
	 *
	 * Check for a valid numeric value
	 *
	 *<code>
	 *use Dao\Validation\Validator\Numericality;
	 *
	 *$validator->add('price', new Numericality(array(
	 *   'message' => ':field is not numeric'
	 *)));
	 *</code>
	 */
	
	class Numericality extends \Dao\Validation\Validator implements \Dao\Validation\ValidatorInterface {

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
