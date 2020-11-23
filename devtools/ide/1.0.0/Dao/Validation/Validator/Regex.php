<?php 

namespace Dao\Validation\Validator {

	/**
	 * Dao\Validation\Validator\Regex
	 *
	 * Allows validate if the value of a field matches a regular expression
	 *
	 *<code>
	 *use Dao\Validation\Validator\Regex as RegexValidator;
	 *
	 *$validator->add('created_at', new RegexValidator(array(
	 *   'pattern' => '/^[0-9]{4}[-\/](0[1-9]|1[12])[-\/](0[1-9]|[12][0-9]|3[01])$/',
	 *   'message' => 'The creation date is invalid'
	 *)));
	 *</code>
	 */
	
	class Regex extends \Dao\Validation\Validator implements \Dao\Validation\ValidatorInterface {

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
