<?php 

namespace Dao\Validation\Validator {

	/**
	 * Dao\Validation\Validator\PresenceOf
	 *
	 * Validates that a value is not null or empty string
	 *
	 *<code>
	 *use Dao\Validation\Validator\PresenceOf;
	 *
	 *$validator->add('name', new PresenceOf(array(
	 *   'message' => 'The name is required'
	 *)));
	 *</code>
	 */
	
	class PresenceOf extends \Dao\Validation\Validator implements \Dao\Validation\ValidatorInterface {

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
