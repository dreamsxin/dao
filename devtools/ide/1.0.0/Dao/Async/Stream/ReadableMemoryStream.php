<?php 

namespace Dao\Async\Stream {

	final class ReadableMemoryStream implements \Dao\Async\Stream\ReadableStream {

		public function __construct($buffer=null){ }


		public function __wakeup(){ }


		public function isClosed(){ }


		public function close(\Throwable $error=null){ }


		public function read($length=null, $timeout=null){ }

	}
}
