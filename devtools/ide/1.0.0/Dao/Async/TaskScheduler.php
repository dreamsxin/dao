<?php 

namespace Dao\Async {

	final class TaskScheduler {

		private function __construct(){ }


		public function __wakeup(){ }


		public static function run($callback, $finalizer=null){ }


		public static function runWithContext(\Dao\Async\Context $context, $callback, $finalizer=null){ }


		public static function get($type){ }


		public static function register($type, $factory){ }


		public function tick($callback){ }


		public function timer($callback){ }


		public function poll($stream, $callback){ }

	}
}
