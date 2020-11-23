<?php 

namespace Dao\Events {

	interface ManagerInterface {

		public function attach($eventType, $handler, $priority=null);


		public function detach($eventType, $handler);


		public function detachAll($eventType=null);


		public function fire($event, $source, $data=null, $cancelable=null, $flag=null);


		public function getListeners($eventType, $full=null);

	}
}
