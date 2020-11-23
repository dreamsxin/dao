<?php 

namespace Dao\Async\Network {

	final class TcpServer implements \Dao\Async\Network\Server, \Dao\Async\Network\Socket {

		const SIMULTANEOUS_ACCEPTS = 150;

		private function __construct(){ }


		public function __wakeup(){ }


		public static function bind($host=null, $port=null, \Dao\Async\Network\TlsServerEncryption $tls=null, $reuseport=null){ }


		public static function listen($host=null, $port=null, \Dao\Async\Network\TlsServerEncryption $tls=null, $reuseport=null){ }


		public static function import(\Dao\Async\Network\Pipe $pipe, \Dao\Async\Network\TlsServerEncryption $tls=null){ }


		public function close(\Throwable $error=null){ }


		public function getAddress(){ }


		public function getPort(){ }


		public function setOption($option, $value){ }


		public function accept(){ }


		public function export(\Dao\Async\Network\Pipe $pipe){ }

	}
}
