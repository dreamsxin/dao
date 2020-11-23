<?php 

namespace Dao\Http\Client {

	class Response {

		protected $_header;

		protected $_body;

		public function __construct($header=null, $body=null){ }


		public function setHeader($header){ }


		public function getHeader(){ }


		public function setBody($body){ }


		public function getBody(){ }


		public function getJsonBody(){ }


		public function setStatusCode($status_code){ }


		public function getStatusCode(){ }

	}
}
