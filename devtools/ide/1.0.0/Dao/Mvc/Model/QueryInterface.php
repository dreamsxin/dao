<?php 

namespace Dao\Mvc\Model {

	interface QueryInterface {

		public function parse();


		public function execute($bindParams=null, $bindTypes=null, $useRawsql=null);

	}
}
