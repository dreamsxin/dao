<?php 

namespace Dao\Validation\Validator {

	/**
	 * Dao\Validation\Validator\StringLength
	 *
	 * Validates that a string has the specified maximum and minimum constraints
	 *
	 *<code>
	 *use Dao\Validation\Validator\StringLength as StringLength;
	 *
	 *$validation->add('name_last', new StringLength(array(
	 *      'max' => 50,
	 *      'min' => 2,
	 *      'messageMaximum' => 'We don\'t like really long names',
	 *      'messageMinimum' => 'We want more than just their initials'
	 *)));
	 *</code>
	 *
	 */
	
	class StringLength extends \Dao\Validation\Validator implements \Dao\Validation\ValidatorInterface {

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
		 * @param int $minimum
		 * @param int $maximum
		 * @return boolean
		 */
		public function valid($value){ }

	}
}
