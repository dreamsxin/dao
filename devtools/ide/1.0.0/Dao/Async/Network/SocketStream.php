<?php 

namespace Dao\Async\Network {

	interface SocketStream {

		public function getRemoteAddress();


		public function getRemotePort();


		public function isAlive();


		public function flush();


		public function getWriteQueueSize();

	}
}
