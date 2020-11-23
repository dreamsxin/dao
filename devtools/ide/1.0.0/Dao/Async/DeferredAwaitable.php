<?php 

namespace Dao\Async {

	final class DeferredAwaitable implements \Dao\Async\Awaitable {

		public $status;

		public $file;

		public $line;

		private function __construct(){ }


		public function __wakeup(){ }

	}
}
