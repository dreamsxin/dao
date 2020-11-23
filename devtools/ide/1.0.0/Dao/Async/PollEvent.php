<?php 

namespace Dao\Async {

	final class PollEvent {

		const READABLE = 1;

		const WRITABLE = 2;

		const DISCONNECT = 4;

		private function __construct(){ }


		public function __wakeup(){ }


		public function close(){ }


		public function start($mode){ }


		public function stop(){ }


		public function ref(){ }


		public function unref(){ }

	}
}
