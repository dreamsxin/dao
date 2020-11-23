<?php 

namespace Dao\Async\Network {

	final class UdpDatagram {

		public $data;

		public $address;

		public $port;

		public function __construct($data, $address=null, $port=null){ }


		public function __wakeup(){ }


		public function withData($data){ }


		public function withPeer($address, $port){ }


		public function withoutPeer(){ }

	}
}
