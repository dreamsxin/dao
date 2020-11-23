<?php 

namespace Dao\Async {

	final class CancellationHandler {

		private function __construct(){ }


		public function __wakeup(){ }


		public function __invoke(\Throwable $error=null){ }

	}
}
