<?php 

namespace Dao\Async\Network {

	final class TlsInfo {

		public $protocol;

		public $cipher_name;

		public $cipher_bits;

		public $alpn_protocol;

		private function __construct(){ }


		public function __wakeup(){ }

	}
}
