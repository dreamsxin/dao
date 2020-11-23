<?php 

namespace Dao\Profile {

	interface ItemInterface {

		public function getName();


		public function setData($data);


		public function getData();


		public function setInitialTime($initialTime);


		public function getInitialTime();


		public function setFinalTime($finalTime);


		public function getFinalTime();


		public function getTotalElapsedSeconds();


		public function setStartMemory($startMemory);


		public function getStartMemory();


		public function setEndMemory($endMemory);


		public function getEndMemory();


		public function getTotalUsageMemory();

	}
}
