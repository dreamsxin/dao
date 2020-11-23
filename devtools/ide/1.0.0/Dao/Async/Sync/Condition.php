<?php 

namespace Dao\Async\Sync {

	final class Condition {

		public function __wakeup(){ }


		public function close(\Throwable $error=null){ }


		public function wait(){ }


		public function signal(){ }


		public function broadcast(){ }

	}
}
