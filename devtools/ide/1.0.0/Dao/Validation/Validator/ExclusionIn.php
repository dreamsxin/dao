<?php 

namespace Dao\Validation\Validator {

	/**
	 * Dao\Validation\Validator\ExclusionIn
	 *
	 * Check if a value is not included into a list of values
	 *
	 *<code>
	 *use Dao\Validation\Validator\ExclusionIn;
	 *
	 *$validator->add('status', new ExclusionIn(array(
	 *   'message' => 'The status must not be A or B',
	 *   'domain' => array('A', 'B')
	 *)));
	 *</code>
	 */
	
	class ExclusionIn extends \Dao\Validation\Validator implements \Dao\Validation\ValidatorInterface {

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
