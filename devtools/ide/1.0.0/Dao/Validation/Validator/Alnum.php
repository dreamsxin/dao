<?php 

namespace Dao\Validation\Validator {

	/**
	 * Dao\Validation\Validator\Alnum
	 *
	 * Checks if a value has a correct ALNUM format
	 *
	 *<code>
	 *use Dao\Validation\Validator\Alnum as AlnumValidator;
	 *
	 *$validator->add('alnum', new AlnumValidator(array(
	 *   'message' => 'The alnum is not valid'
	 *)));
	 *</code>
	 */
	
	class Alnum extends \Dao\Validation\Validator implements \Dao\Validation\ValidatorInterface {

		/**
		 * Executes the validation
		 *
		 * @param \Dao\Validation $validaton
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
