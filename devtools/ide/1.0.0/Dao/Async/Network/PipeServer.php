<?php 

namespace Dao\Async\Network {

	final class PipeServer implements \Dao\Async\Network\Server, \Dao\Async\Network\Socket {

		private function __construct(){ }


		public function __wakeup(){ }


		public static function bind($name, $ipc=null){ }


		public static function listen($name, $ipc=null){ }


		public static function import(\Dao\Async\Network\Pipe $pipe, $ipc=null){ }


		public function close(\Throwable $error=null){ }


		public function getAddress(){ }


		public function getPort(){ }


		public function setOption($option, $value){ }


		public function accept(){ }


		public function export(\Dao\Async\Network\Pipe $pipe){ }

	}
}
