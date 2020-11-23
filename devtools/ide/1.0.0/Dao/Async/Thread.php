<?php 

namespace Dao\Async {

	final class Thread {

		public function __construct($file){ }


		public function __wakeup(){ }


		public static function isAvailable(){ }


		public static function isWorker(){ }


		public static function connect(){ }


		public function getIpc(){ }


		public function kill(){ }


		public function join(){ }

	}
}
