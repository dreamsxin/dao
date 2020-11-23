<?php 

namespace Dao\Async\Stream {

	interface ReadableStream {

		public function close(\Throwable $error=null);


		public function read($length=null, $timeout=null);

	}
}
