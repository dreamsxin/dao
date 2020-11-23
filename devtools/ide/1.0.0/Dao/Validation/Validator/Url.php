<?php 

namespace Dao\Validation\Validator {

	/**
	 * Dao\Validation\Validator\Url
	 *
	 * Checks if a value has a correct URL format
	 *
	 *<code>
	 *use Dao\Validation\Validator\Url as UrlValidator;
	 *
	 *$validator->add('url', new UrlValidator(array(
	 *   'message' => 'The url is not valid'
	 *)));
	 *</code>
	 */
	
	class Url extends \Dao\Validation\Validator implements \Dao\Validation\ValidatorInterface {

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
