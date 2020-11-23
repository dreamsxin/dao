<?php 

namespace Dao\Async\Stream {

	final class StreamWriter implements \Dao\Async\Stream\WritableStream {

		private function __construct(){ }


		public function __wakeup(){ }


		public function close(\Throwable $error=null){ }


		public function write($data){ }

	}
}
