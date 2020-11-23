<?php 

namespace Dao\Async {

	final class ContextVar {

		public function __construct($name){ }


		public function __wakeup(){ }


		public function get(\Dao\Async\Context $context=null){ }

	}
}
