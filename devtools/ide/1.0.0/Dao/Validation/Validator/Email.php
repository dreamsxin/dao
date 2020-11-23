<?php 

namespace Dao\Validation\Validator {

	/**
	 * Dao\Validation\Validator\Email
	 *
	 * Checks if a value has a correct e-mail format
	 *
	 *<code>
	 *use Dao\Validation\Validator\Email as EmailValidator;
	 *
	 *$validator->add('email', new EmailValidator(array(
	 *   'message' => 'The e-mail is not valid'
	 *)));
	 *</code>
	 */
	
	class Email extends \Dao\Validation\Validator implements \Dao\Validation\ValidatorInterface {

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
