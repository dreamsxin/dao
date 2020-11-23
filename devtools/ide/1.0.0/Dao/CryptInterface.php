<?php 

namespace Dao {

	interface CryptInterface {

		public function setMethod($method);


		public function getMethod();


		public function setKey($key);


		public function getKey();


		public function encrypt($text, $key=null, $options=null);


		public function decrypt($text, $key=null, $options=null);


		public function encryptBase64($text, $key=null, $safe=null);


		public function decryptBase64($text, $key=null, $safe=null);


		public function getAvailableMethods();

	}
}
