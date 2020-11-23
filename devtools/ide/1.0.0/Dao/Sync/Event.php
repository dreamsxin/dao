<?php 

namespace Dao\Sync {

	/**
	 * Dao\Sync\Event
	 *
	 */
	
	class Event {

		/**
		 * \Dao\Sync\Event constructor
		 *
		 * @param string $name
		 * @param boolean $manual
		 * @param boolean $prefire
		 */
		public function __construct($name=null, $manual=null, $prefire=null){ }


		/**
		 * Waits for an event object to fire
		 *
		 * @param int $wait
		 * @return boolean
		 */
		public function wait($wait=null){ }


		/**
		 * Lets a thread through that is waiting.  Lets multiple threads through that are waiting if the event object is 'manual'
		 *
		 * @return boolean
		 */
		public function fire(){ }


		/**
		 * Resets the event object state.  Only use when the event object is 'manual'
		 *
		 * @return boolean
		 */
		public function reset(){ }

	}
}
