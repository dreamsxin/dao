<?php 

namespace Dao\Async\Network {

	final class Pipe implements \Dao\Async\Network\SocketStream, \Dao\Async\Stream\ReadableStream, \Dao\Async\Stream\WritableStream, \Dao\Async\Stream\DuplexStream, \Dao\Async\Network\Socket {

		private function __construct(){ }


		public function __wakeup(){ }


		public static function connect($name, $host=null, $ipc=null){ }


		public static function import(\Dao\Async\Network\Pipe $pipe, $ipc=null){ }


		public static function pair($ipc=null){ }


		public function close(\Throwable $error=null){ }


		public function flush(){ }


		public function getAddress(){ }


		public function getPort(){ }


		public function setOption($option, $value){ }


		public function getRemoteAddress(){ }


		public function getRemotePort(){ }


		public function isAlive(){ }


		public function read($length=null, $timeout=null){ }


		public function getReadableStream(){ }


		public function write($data){ }


		public function getWriteQueueSize(){ }


		public function getWritableStream(){ }


		public function export(\Dao\Async\Network\Pipe $pipe){ }

	}
}
