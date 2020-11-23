<?php 

namespace Dao\Async\Stream {

	final class WritableMemoryStream implements \Dao\Async\Stream\WritableStream {

		public function __wakeup(){ }


		public function isClosed(){ }


		public function getContents(){ }


		public function close(\Throwable $error=null){ }


		public function write($data){ }

	}
}
