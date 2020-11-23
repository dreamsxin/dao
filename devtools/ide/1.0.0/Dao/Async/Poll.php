<?php 

namespace Dao\Async {

	final class Poll {

		public function __construct($resource){ }


		public function __wakeup(){ }


		public function close(\Throwable $error=null){ }


		public function awaitReadable(){ }


		public function awaitWritable(){ }

	}
}
