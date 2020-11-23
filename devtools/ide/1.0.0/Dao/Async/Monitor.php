<?php 

namespace Dao\Async {

	final class Monitor {

		public function __construct($path, $recursive=null){ }


		public function __wakeup(){ }


		public function close(\Throwable $error=null){ }


		public function awaitEvent(){ }

	}
}
