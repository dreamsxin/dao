<?php 

namespace Dao\Mvc\Model\Query {

	interface BuilderInterface {

		public function getType();


		public function compile();


		public function getPhql();


		public function getQuery();

	}
}
