<?php 

namespace Dao\Async {

	final class Channel implements \IteratorAggregate, \Traversable {

		public function __construct($capacity=null){ }


		public function __wakeup(){ }


		public function getIterator(){ }


		public function close(\Throwable $error=null){ }


		public function isClosed(){ }


		public function send($message, $timeout=null){ }

	}
}
