<?php 

namespace Dao {

	/**
	 * Dao\Validation
	 *
	 * Allows to validate data using validators
	 */
	
	class Validation extends \Dao\Di\Injectable implements \Dao\Events\EventsAwareInterface, \Dao\Di\InjectionAwareInterface, \Dao\ValidationInterface {

		protected $_data;

		protected $_entity;

		protected $_validators;

		protected $_filters;

		protected $_messages;

		protected $_values;

		protected $_labels;

		protected $_messageFilename;

		protected $_allowEmpty;

		protected static $_delimiter;

		protected static $_defaultMessageFilename;

		/**
		 * \Dao\Validation constructor
		 *
		 * @param array $validators
		 * @param array $options
		 */
		public function __construct($validators=null, $options=null){ }


		/**
		 * Validate a set of data according to a set of rules
		 *
		 * @param array|object $data
		 * @param object $entity
		 * @return \Dao\Validation\Message\Group
		 */
		public function validate($data, $entity=null){ }


		/**
		 * Adds a validator to a field
		 *
		 * @param string|array $attribute
		 * @param array|Dao\Validation\ValidatorInterface
		 * @return \Dao\Validation
		 */
		public function add($attribute, $validator){ }


		/**
		 * Adds filters to the field
		 *
		 * @param string $attribute
		 * @param array|string $attribute
		 * @return \Dao\Validation
		 */
		public function setFilters($attribute, $filters){ }


		/**
		 * Returns all the filters or a specific one
		 *
		 * @param string $attribute
		 * @return mixed
		 */
		public function getFilters($attribute=null){ }


		/**
		 * Returns the validators added to the validation
		 *
		 * @return array
		 */
		public function getValidators(){ }


		/**
		 * Sets the bound entity
		 *
		 * @param object entity
		 * @return \Dao\Validation
		 */
		public function setEntity($entity){ }


		/**
		 * Returns the bound entity
		 *
		 * @return object
		 */
		public function getEntity(){ }


		/**
		 * Returns the registered validators
		 *
		 * @return \Dao\Validation\Message\Group
		 */
		public function getMessages(){ }


		/**
		 * Appends a message to the messages list
		 *
		 * @param \Dao\Validation\MessageInterface $message
		 * @return \Dao\Validation
		 */
		public function appendMessage(\Dao\Validation\MessageInterface $message){ }


		/**
		 * Assigns the data to an entity
		 * The entity is used to obtain the validation values
		 *
		 * @param object $entity
		 * @param object|array $data
		 * @return \Dao\Validation
		 */
		public function bind($entity, $data){ }


		/**
		 * Gets the a array data source
		 *
		 * @return array|null
		 */
		public function getData(){ }


		/**
		 * Gets the a value to validate in the array/object data source
		 *
		 * @param string $attribute
		 * @param object entity
		 * @return mixed
		 */
		public function getValue($attribute, $entity=null){ }


		public function setDefaultMessages($messages){ }


		public function getDefaultMessage($type, $defaultValue=null){ }


		/**
		 * Adds labels for fields
		 *
		 * @param array labels
		 */
		public function setLabels($labels){ }


		/**
		 * Gets label for field
		 *
		 * @param string|array field
		 * @return string
		 */
		public function getLabel($field){ }


		/**
		 * Sets delimiter for label
		 *
		 * @param string
		 */
		public static function setLabelDelimiter($delimiter){ }


		/**
		 * Sets validation message file name
		 *
		 * @param string filename
		 */
		public static function setMessageFilename($filename){ }


		/**
		 * Gets message
		 *
		 * @param string
		 */
		public static function getMessage($type){ }

	}
}
