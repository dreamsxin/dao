<?php 

namespace Dao\Async\Process {

	final class ReadablePipe implements \Dao\Async\Stream\ReadableStream {

		private function __construct(){ }


		public function __wakeup(){ }


		public function close(\Throwable $error=null){ }


		public function read($length=null, $timeout=null){ }

	}
}
