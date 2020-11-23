<?php 

namespace Dao\Async {

	final class AwaitableImpl implements \Dao\Async\Awaitable {

		public $status;

		public $file;

		public $line;
	}
}
