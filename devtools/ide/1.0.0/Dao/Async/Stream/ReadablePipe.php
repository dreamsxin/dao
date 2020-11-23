<?php 

namespace Dao\Async\Stream {

	final class ReadablePipe implements \Dao\Async\Stream\ReadableStream {

		private function __construct(){ }


		public function __wakeup(){ }


		public static function getStdin(){ }


		public function isTerminal(){ }


		public function close(\Throwable $error=null){ }


		public function read($length=null, $timeout=null){ }

	}
}
