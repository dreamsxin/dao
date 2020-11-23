<?php 

namespace Dao\Validation\Validator {

	/**
	 * Dao\Validation\Validator\Json
	 *
	 * Checks if a value has a correct JSON format
	 *
	 *<code>
	 *use Dao\Validation\Validator\Json as JsonValidator;
	 *
	 *$validator->add('json', new JsonValidator(array(
	 *   'keys' => array('name'),
	 *   'message' => 'The json is not valid'
	 *)));
	 *</code>
	 */
	
	class Json extends \Dao\Validation\Validator implements \Dao\Validation\ValidatorInterface {

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
