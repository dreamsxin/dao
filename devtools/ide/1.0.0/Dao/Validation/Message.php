<?php 

namespace Dao\Validation {

	/**
	 * Dao\Validation\Message
	 *
	 * Encapsulates validation info generated in the validation process
	 */
	
	class Message implements \Dao\Validation\MessageInterface, \JsonSerializable {

		protected $_type;

		protected $_message;

		protected $_field;

		protected $_code;

		/**
		 * \Dao\Validation\Message constructor
		 *
		 * @param string $message
		 * @param string $field
		 * @param string $type
		 * @param int    $code
		 */
		public function __construct($message, $field=null, $type=null, $code=null){ }


		/**
		 * Sets message type
		 *
		 * @param string $type
		 * @return \Dao\Validation\Message
		 */
		public function setType($type){ }


		/**
		 * Returns message type
		 *
		 * @return string
		 */
		public function getType(){ }


		/**
		 * Sets message code
		 *
		 * @param string $code
		 * @return \Dao\Validation\Message
		 */
		public function setCode($code){ }


		/**
		 * Returns message code
		 *
		 * @return string
		 */
		public function getCode(){ }


		/**
		 * Sets verbose message
		 *
		 * @param string $message
		 * @return \Dao\Validation\Message
		 */
		public function setMessage($message){ }


		/**
		 * Returns verbose message
		 *
		 * @return string
		 */
		public function getMessage(){ }


		/**
		 * Sets field name related to message
		 *
		 * @param string $field
		 * @return \Dao\Validation\Message
		 */
		public function setField($field){ }


		/**
		 * Returns field name related to message
		 *
		 * @return string
		 */
		public function getField(){ }


		/**
		 * Magic __toString method returns verbose message
		 *
		 * @return string
		 */
		public function __toString(){ }


		/**
		 * Magic __set_state helps to recover messsages from serialization
		 *
		 * @param array $message
		 * @return \Dao\Validation\Message
		 */
		public static function __set_state($message){ }


		/**
		 * Returns serialised model as array for json_encode.
		 *
		 * @return array
		 */
		public function jsonSerialize(){ }

	}
}
