<?php 

namespace Dao\Http\Client\Adapter {

	class Curl extends \Dao\Http\Client\Adapter implements \Dao\Http\Client\AdapterInterface {

		const VERSION = 0.0.1;

		const AUTH_TYPE_ANY = any;

		const AUTH_TYPE_BASIC = basic;

		const AUTH_TYPE_DIGEST = digest;

		protected $_curl;

		public function __construct($uri, $method){ }


		protected function sendInternal(){ }

	}
}
