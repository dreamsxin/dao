<?php 

namespace Dao\Async {

	final class Signal {

		const SIGHUP = 1;

		const SIGINT = 2;

		const SIGQUIT = 3;

		const SIGKILL = 9;

		const SIGTERM = 15;

		const SIGUSR1 = 10;

		const SIGUSR2 = 12;

		public function __construct($signum){ }


		public function __wakeup(){ }


		public function close(\Throwable $error=null){ }


		public function awaitSignal(){ }


		public static function isSupported($signum){ }


		public static function raise($signum){ }


		public static function signal($pid, $signum){ }

	}
}
