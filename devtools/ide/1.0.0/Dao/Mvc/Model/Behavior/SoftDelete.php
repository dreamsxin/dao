<?php 

namespace Dao\Mvc\Model\Behavior {

	/**
	 * Dao\Mvc\Model\Behavior\SoftDelete
	 *
	 * Instead of permanently delete a record it marks the record as
	 * deleted changing the value of a flag column
	 */
	
	class SoftDelete extends \Dao\Mvc\Model\Behavior implements \Dao\Mvc\Model\BehaviorInterface {

		/**
		 * Listens for notifications from the models manager
		 *
		 * @param string $type
		 * @param \Dao\Mvc\ModelInterface $model
		 */
		public function notify($type, $model){ }

	}
}
