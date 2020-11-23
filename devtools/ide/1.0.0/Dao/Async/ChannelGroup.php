<?php 

namespace Dao\Async {

	final class ChannelGroup implements \Countable {

		public function __construct($channels, $shuffle=null){ }


		public function __wakeup(){ }


		public function count(){ }


		public function select($timeout=null){ }


		public function send($value, $timeout=null){ }

	}
}
