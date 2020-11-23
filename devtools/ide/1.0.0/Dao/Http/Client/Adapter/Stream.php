<?php 

namespace Dao\Http\Client\Adapter {

	class Stream extends \Dao\Http\Client\Adapter implements \Dao\Http\Client\AdapterInterface {

		const VERSION = 0.0.1;

		const AUTH_TYPE_ANY = any;

		const AUTH_TYPE_BASIC = basic;

		const AUTH_TYPE_DIGEST = digest;

		protected $_stream;

		public function __construct($uri, $method){ }


		protected function buildBody(){ }


		public function errorHandler($errno, $errstr, $errfile, $errline, $data){ }


		protected function sendInternal(){ }

	}
}
