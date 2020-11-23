<?php 

namespace Dao\Cache {

	interface BackendInterface {

		public function start($keyName, $lifetime=null, $nobuffer=null);


		public function stop($stopBuffer=null);


		public function getFrontend();


		public function getOptions();


		public function isFresh();


		public function isStarted();


		public function get($keyName, $lifetime=null);


		public function save($keyName=null, $value=null, $lifetime=null, $stopBuffer=null);


		public function delete($keyName);


		public function queryKeys($prefix=null);


		public function exists($keyName);


		public function flush();

	}
}
