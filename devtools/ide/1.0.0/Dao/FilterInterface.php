<?php 

namespace Dao {

	interface FilterInterface {

		public function add($name, $handler);


		public function sanitize($value, $filters, $recursive=null, $options=null, $recursiveLevel=null);


		public function getFilters();

	}
}
