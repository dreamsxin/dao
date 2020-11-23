<?php 

namespace Dao\Events {

	/**
	 * Dao\Events\Event
	 *
	 * This class offers contextual information of a fired event in the EventsManager
	 */
	
	class Event implements \Dao\Events\EventInterface {

		protected $_name;

		protected $_type;

		protected $_source;

		protected $_data;

		protected $_stopped;

		protected $_cancelable;

		protected $_flag;

		/**
		 * \Dao\Events\Event constructor
		 *
		 * @param string $type
		 * @param object $source
		 * @param mixed $data
		 * @param boolean $cancelable
		 */
		public function __construct($type, $source, $data=null, $cancelable=null, $flag=null){ }


		/**
		 * Set the event's name
		 *
		 * @param string $eventName
		 */
		public function setName($eventName){ }


		/**
		 * Returns the event's name
		 *
		 * @return string
		 */
		public function getName(){ }


		/**
		 * Set the event's type
		 *
		 * @param string $eventType
		 */
		public function setType($eventType){ }


		/**
		 * Returns the event's type
		 *
		 * @return string
		 */
		public function getType(){ }


		/**
		 * Sets the event's source
		 *
		 * @return object
		 */
		public function setSource($source){ }


		/**
		 * Returns the event's source
		 *
		 * @return object
		 */
		public function getSource(){ }


		/**
		 * Set the event's data
		 *
		 * @param string $data
		 */
		public function setData($data){ }


		/**
		 * Returns the event's data
		 *
		 * @return mixed
		 */
		public function getData(){ }


		/**
		 * Sets if the event is cancelable
		 *
		 * @param boolean $cancelable
		 */
		public function setCancelable($cancelable){ }


		/**
		 * Check whether the event is cancelable
		 *
		 * @return boolean
		 */
		public function isCancelable(){ }


		/**
		 * Returns the event's flag
		 *
		 * @return mixed
		 */
		public function getFlag(){ }


		/**
		 * Stops the event preventing propagation
		 */
		public function stop(){ }


		/**
		 * Check whether the event is currently stopped
		 */
		public function isStopped(){ }


		public function getCancelable(){ }

	}
}
