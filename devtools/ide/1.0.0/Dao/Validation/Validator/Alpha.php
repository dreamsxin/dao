<?php 

namespace Dao\Validation\Validator {

	/**
	 * Dao\Validation\Validator\Alpha
	 *
	 * Checks if a value has a correct ALPHA format
	 *
	 *<code>
	 *use Dao\Validation\Validator\Alpha as AlphaValidator;
	 *
	 *$validator->add('alpha', new AlphaValidator(array(
	 *   'message' => 'The alpha is not valid'
	 *)));
	 *</code>
	 */
	
	class Alpha extends \Dao\Validation\Validator implements \Dao\Validation\ValidatorInterface {

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
