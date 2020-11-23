<?php 

namespace Dao\Validation\Validator {

	/**
	 * Dao\Validation\Validator\Uniqueness
	 *
	 * Check that a field is unique in the related table
	 *
	 *<code>
	 *use Dao\Validation\Validator\Uniqueness as UniquenessValidator;
	 *
	 *$validator->add('name', new UniquenessValidator(array(
	 *   'model' => new Users(),
	 *   'message' => ':field must be unique'
	 *)));
	 *
	 *$validator->add(array('user_id', 'created'), new UniquenessValidator(array(
	 *   'model' => new Users(),
	 *   'message' => ':field must be unique'
	 *)));
	 *</code>
	 */
	
	class Uniqueness extends \Dao\Validation\Validator implements \Dao\Validation\ValidatorInterface {

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
		 * @param \Dao\Mvc\ModelInterface $record
		 * @param array $values
		 * @return boolean
		 */
		public function valid($value){ }

	}
}
