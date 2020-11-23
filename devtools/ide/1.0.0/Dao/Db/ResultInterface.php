<?php 

namespace Dao\Db {

	interface ResultInterface {

		public function execute();


		public function fetch($fetchMode=null, $fetchArgument=null, $ctorArgs=null);


		public function fetchArray();


		public function fetchAll();


		public function numRows();


		public function dataSeek($number);


		public function setFetchMode($fetchMode, $fetchArg1=null, $fetchArg2=null);


		public function getInternalResult();

	}
}
