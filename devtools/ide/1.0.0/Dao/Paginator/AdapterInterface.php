<?php 

namespace Dao\Paginator {

	interface AdapterInterface {

		public function setCurrentPage($page);


		public function getCurrentPage();


		public function setLimit($limit);


		public function getLimit();


		public function getPaginate();

	}
}
