<?php 

namespace Dao\Async\Network {

	final class UdpSocket implements \Dao\Async\Network\Socket {

		const TTL = 200;

		const MULTICAST_LOOP = 250;

		const MULTICAST_TTL = 251;

		private function __construct(){ }


		public function __wakeup(){ }


		public static function bind($host=null, $port=null){ }


		public static function connect($remote_host, $remote_port, $host=null, $port=null){ }


		public static function multicast($group, $port){ }


		public function close(\Throwable $error=null){ }


		public function flush(){ }


		public function getAddress(){ }


		public function getPort(){ }


		public function setOption($option, $value){ }


		public function receive($size=null){ }


		public function send(\Dao\Async\Network\UdpDatagram $datagram){ }

	}
}
