<?php 

namespace Dao\Async {

	final class Deferred {

		public $status;

		public $file;

		public $line;

		public function __construct($cancel=null){ }


		public function __wakeup(){ }


		public function awaitable(){ }


		public function resolve($value=null){ }


		public function fail(\Throwable $error=null){ }


		public static function value($value=null){ }


		public static function error(\Throwable $error){ }


		public static function combine($awaitables, $continuation){ }


		public static function transform(\Dao\Async\Awaitable $awaitable, $continuation){ }

	}
}
