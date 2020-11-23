<?php 

namespace Dao\Async {

	final class TimerEvent {

		private function __construct(){ }


		public function __wakeup(){ }


		public function close(){ }


		public function start($delay, $periodic=null){ }


		public function stop(){ }


		public function ref(){ }


		public function unref(){ }

	}
}
