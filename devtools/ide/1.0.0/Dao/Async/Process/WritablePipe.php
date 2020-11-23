<?php 

namespace Dao\Async\Process {

	final class WritablePipe implements \Dao\Async\Stream\WritableStream {

		private function __construct(){ }


		public function __wakeup(){ }


		public function close(\Throwable $error=null){ }


		public function write($data){ }

	}
}
