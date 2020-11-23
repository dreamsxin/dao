<?php 

namespace Dao\Mvc\Model\Behavior {

	/**
	 * Dao\Mvc\Model\Behavior\Timestampable
	 *
	 * Allows to automatically update a model’s attribute saving the
	 * datetime when a record is created or updated
	 */
	
	class Timestampable extends \Dao\Mvc\Model\Behavior implements \Dao\Mvc\Model\BehaviorInterface {

		/**
		 * Listens for notifications from the models manager
		 *
		 * @param string $type
		 * @param \Dao\Mvc\ModelInterface $model
		 */
		public function notify($type, $model){ }

	}
}
