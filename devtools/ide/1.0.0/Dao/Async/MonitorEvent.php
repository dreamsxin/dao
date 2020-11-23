<?php 

namespace Dao\Async {

	final class MonitorEvent {

		const RENAMED = 1;

		const CHANGED = 2;

		public $events;

		public $path;

		private function __construct(){ }


		public function __wakeup(){ }

	}
}
