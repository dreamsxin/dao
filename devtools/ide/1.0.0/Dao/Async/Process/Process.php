<?php 

namespace Dao\Async\Process {

	final class Process {

		private function __construct(){ }


		public function __wakeup(){ }


		public static function isWorker(){ }


		public static function connect(){ }


		public function isRunning(){ }


		public function getPid(){ }


		public function getStdin(){ }


		public function getStdout(){ }


		public function getStderr(){ }


		public function getIpc(){ }


		public function signal($signum){ }


		public function join(){ }

	}
}
