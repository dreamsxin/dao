<?php 

namespace Dao\Events {

	interface EventInterface {

		public function setType($eventType);


		public function getType();


		public function setSource($source);


		public function getSource();


		public function setData($data);


		public function getData();


		public function setCancelable($cancelable);


		public function isCancelable();


		public function stop();


		public function isStopped();

	}
}
