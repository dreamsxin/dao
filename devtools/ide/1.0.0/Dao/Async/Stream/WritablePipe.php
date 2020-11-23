<?php 

namespace Dao\Async\Stream {

	final class WritablePipe implements \Dao\Async\Stream\WritableStream {

		private function __construct(){ }


		public function __wakeup(){ }


		public static function getStdout(){ }


		public static function getStderr(){ }


		public function isTerminal(){ }


		public function close(\Throwable $error=null){ }


		public function write($data){ }

	}
}
