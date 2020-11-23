<?php 

namespace Dao\Mvc\Micro {

	interface MiddlewareInterface {

		public function call($application);

	}
}
