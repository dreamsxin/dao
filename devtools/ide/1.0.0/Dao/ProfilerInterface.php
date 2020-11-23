<?php 

namespace Dao {

	interface ProfilerInterface {

		public function startProfile($name, $data=null);


		public function stopProfile($name=null);


		public function getTotalElapsedSeconds();


		public function getTotalUsageMemory();


		public function getProfiles();


		public function getLastProfile();


		public function reset();

	}
}
