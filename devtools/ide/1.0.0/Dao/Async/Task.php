<?php 

namespace Dao\Async {

	final class Task implements \Dao\Async\Awaitable {

		public $status;

		public $file;

		public $line;

		private function __construct(){ }


		public function __wakeup(){ }


		public static function async(\callable $callback, $arguments=null){ }


		public static function asyncWithContext(\Dao\Async\Context $context, \callable $callback, $arguments=null){ }


		public static function await(\Dao\Async\Awaitable $awaitable){ }


		public function getTrace(\int $options=null, \int $limit=null){ }


		public function getId(){ }

	}
}
