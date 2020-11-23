<?php 

namespace Dao\Validation\Validator {

	/**
	 * Dao\Validation\Validator\File
	 *
	 * Checks if a value has a correct FILE format
	 *
	 *<code>
	 *use Dao\Validation\Validator\File as FileValidator;
	 *
	 *$validator->add('file', new FileValidator(array(
	 *	 'mimes' => array('image/png', 'image/gif'),
	 *	 'minsize' => 100,
	 *	 'maxsize' => 10000,
	 *   'message' => 'The file is not valid'
	 *)));
	 *</code>
	 */
	
	class File extends \Dao\Validation\Validator implements \Dao\Validation\ValidatorInterface {

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
		 * @param string $file
		 * @param int $minsize
		 * @param int $maxsize
		 * @param array $mimes
		 * @param int $minwidth
		 * @param int $maxwidth
		 * @param int $minheight
		 * @param int $maxheight
		 * @return boolean
		 */
		public function valid($value){ }

	}
}
