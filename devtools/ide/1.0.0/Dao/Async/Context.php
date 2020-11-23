<?php 

namespace Dao\Async {

	final class Context {

		private function __construct(){ }


		public function __wakeup(){ }


		public function isCancelled(){ }


		public function with(\Dao\Async\ContextVar $var, $value){ }


		public function setVar($name, $value){ }


		public function getVar($name=null){ }


		public function withIsolatedOutput(){ }


		public function withTimeout($milliseconds){ }


		public function withCancel($cancel){ }


		public function shield(){ }


		public function throwIfCancelled(){ }


		public function run($callback, $arguments=null){ }


		public static function current(){ }


		public static function background(){ }

	}
}
