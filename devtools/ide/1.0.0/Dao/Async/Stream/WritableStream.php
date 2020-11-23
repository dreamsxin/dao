<?php 

namespace Dao\Async\Stream {

	interface WritableStream {

		public function close(\Throwable $error=null);


		public function write($data);

	}
}
