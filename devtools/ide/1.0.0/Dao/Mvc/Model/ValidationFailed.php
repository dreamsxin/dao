<?php 

namespace Dao\Mvc\Model {

	/**
	 * Dao\Mvc\Model\ValidationFailed
	 *
	 * This exception is generated when a model fails to save a record
	 * Dao\Mvc\Model must be set up to have this behavior
	 */
	
	class ValidationFailed extends \Dao\Mvc\Model\Exception implements \Stringable, \Throwable {

		protected $_model;

		protected $_messages;

		/**
		 * \Dao\Mvc\Model\ValidationFailed constructor
		 *
		 * @param \Dao\Mvc\Model $model
		 * @param \Dao\Mvc\Model\Message[] $validationMessages
		 */
		public function __construct($model, $validationMessages){ }


		/**
		 * Returns the complete group of messages produced in the validation
		 *
		 * @return \Dao\Mvc\Model\Message[]
		 */
		public function getMessages(){ }


		/**
		 * Returns the model that generated the messages
		 *
		 * @return \Dao\Mvc\Model
		 */
		public function getModel(){ }

	}
}
