<?php 

namespace Dao\Mvc\Model {

	/**
	 * Dao\Mvc\Model\Behavior
	 *
	 * This is an optional base class for ORM behaviors
	 */
	
	abstract class Behavior implements \Dao\Mvc\Model\BehaviorInterface {

		protected $_options;

		/**
		 * \Dao\Mvc\Model\Behavior
		 *
		 * @param array $options
		 */
		public function __construct($options=null){ }


		/**
		 * Checks whether the behavior must take action on certain event
		 *
		 * @param string $eventName
		 */
		protected function mustTakeAction(){ }


		/**
		 * Returns the behavior options related to an event
		 *
		 * @param string $eventName
		 * @return array
		 */
		protected function getOptions(){ }


		/**
		 * This method receives the notifications from the EventsManager
		 *
		 * @param string $type
		 * @param \Dao\Mvc\ModelInterface $model
		 */
		public function notify($type, $model){ }


		/**
		 * Acts as fallbacks when a missing method is called on the model
		 *
		 * @param \Dao\Mvc\ModelInterface $model
		 * @param string $method
		 * @param array $arguments
		 */
		public function missingMethod($model, $method, $arguments=null){ }

	}
}
