<?php 

namespace Dao\Events {

	/**
	 * Dao\Events\Manager
	 *
	 * Dao Events Manager, offers an easy way to intercept and manipulate, if needed,
	 * the normal flow of operation. With the EventsManager the developer can create hooks or
	 * plugins that will offer monitoring of data, manipulation, conditional execution and much more.
	 *
	 */
	
	class Manager implements \Dao\Events\ManagerInterface {

		protected $_events;

		protected $_currentEvent;

		protected $_collect;

		protected $_enablePriorities;

		protected $_responses;

		/**
		 * Attach a listener to the events manager
		 *
		 * @param string $eventType
		 * @param callable $handler
		 * @param int $priority
		 */
		public function attach($eventType, $handler, $priority=null){ }


		/**
		 * Set if priorities are enabled in the EventsManager
		 *
		 * @param boolean $enablePriorities
		 */
		public function enablePriorities($enablePriorities){ }


		/**
		 * Returns if priorities are enabled
		 *
		 * @return boolean
		 */
		public function arePrioritiesEnabled(){ }


		/**
		 * Tells the event manager if it needs to collect all the responses returned by every
		 * registered listener in a single fire
		 *
		 * @param boolean $collect
		 */
		public function collectResponses($collect){ }


		/**
		 * Check if the events manager is collecting all all the responses returned by every
		 * registered listener in a single fire
		 */
		public function isCollecting(){ }


		/**
		 * Returns all the responses returned by every handler executed by the last 'fire' executed
		 *
		 * @return array
		 */
		public function getResponses(){ }


		/**
		 * Detach a listener from the events manager
		 *
		 * @param object|callable $handler
		 */
		public function detach($eventType, $handler){ }


		/**
		 * Removes all events from the EventsManager
		 *
		 * @param string $type
		 */
		public function detachAll($eventType=null){ }


		/**
		 * Internal handler to call a queue of events
		 *
		 * @param \SplPriorityQueue $queue
		 * @param \Dao\Events\Event $event
		 * @param boolean $flag
		 * @param mixed $prevData
		 * @return mixed
		 */
		public function fireQueue($queue, $event, $flag=null, $prevData=null){ }


		/**
		 * Create an event
		 *
		 * @param string $event_type
		 * @param object $source
		 * @param mixed $data
		 * @param boolean $cancelable
		 * @param boolean $flag
		 * @return \Dao\Events\Event
		 */
		public function createEvent($eventType, $source, $data=null, $cancelable=null, $flag=null){ }


		/**
		 * Fires an event in the events manager causing that active listeners be notified about it
		 *
		 *<code>
		 *	$eventsManager->fire('db', $connection);
		 *</code>
		 *
		 * @param string|Dao\Events\Event $event_type
		 * @param object $source
		 * @param mixed $data
		 * @param int $cancelable
		 * @param int $flag
		 * @return mixed
		 */
		public function fire($event, $source, $data=null, $cancelable=null, $flag=null){ }


		/**
		 * Check whether certain type of event has listeners
		 *
		 * @param string $type
		 * @return boolean
		 */
		public function hasListeners($type){ }


		/**
		 * Returns all the attached listeners of a certain type
		 *
		 * @param string $type
		 * @param boolean $full
		 * @return array
		 */
		public function getListeners($eventType, $full=null){ }


		/**
		 * Retrieve all registered events
		 *
		 * @return array
		 */
		public function getEvents(){ }


		/**
		 * Gets current event
		 *
		 * @return \Dao\Events\Event
		 */
		public function getCurrentEvent(){ }


		public function dettachAll($eventType=null){ }


		public function clearListeners($eventType=null){ }

	}
}
