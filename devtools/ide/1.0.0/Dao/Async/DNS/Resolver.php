<?php 

namespace Dao\Async\DNS {

	interface Resolver {

		public function search(\Dao\Async\DNS\Query $query);

	}
}
