<?php 

namespace Dao\Async {

	final class Timer {

		public function __construct($milliseconds){ }


		public function __wakeup(){ }


		public function close(\Throwable $error=null){ }


		public function awaitTimeout(){ }


		public static function timeout($milliseconds){ }

	}
}
