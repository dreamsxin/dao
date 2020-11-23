<?php 

namespace Dao\Async\Stream {

	interface DuplexStream {

		public function getReadableStream();


		public function getWritableStream();

	}
}
