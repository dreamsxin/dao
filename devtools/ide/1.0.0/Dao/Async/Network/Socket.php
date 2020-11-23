<?php 

namespace Dao\Async\Network {

	interface Socket {

		public function close(\Throwable $error=null);


		public function getAddress();


		public function getPort();


		public function setOption($option, $value);

	}
}
