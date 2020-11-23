<?php 

namespace Dao\Validation\Validator {

	/**
	 * Dao\Validation\Validator\InclusionIn
	 *
	 * Check if a value is included into a list of values
	 *
	 *<code>
	 *use Dao\Validation\Validator\InclusionIn;
	 *
	 *$validator->add('status', new InclusionIn(array(
	 *   'message' => 'The status must be A or B',
	 *   'domain' => array('A', 'B')
	 *)));
	 *</code>
	 */
	
	class InclusionIn extends \Dao\Validation\Validator implements \Dao\Validation\ValidatorInterface {

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
