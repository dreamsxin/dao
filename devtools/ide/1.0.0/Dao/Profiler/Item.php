<?php 

namespace Dao\Profiler {

	/**
	 * Dao\Profiler\Item
	 *
	 * This class identifies each profile in a Dao\Profiler
	 *
	 */
	
	class Item implements \Dao\Profile\ItemInterface {

		protected $_name;

		protected $_data;

		protected $_initialTime;

		protected $_finalTime;

		protected $_startMemory;

		protected $_endMemory;

		/**
		 * Constructor for \Dao\Profiler\Item
		 *
		 * @param array $data
		 */
		public function __construct($name, $data=null){ }


		/**
		 * Returns the name
		 *
		 * @return string
		 */
		public function getName(){ }


		/**
		 * Sets the data related to the profile
		 *
		 * @param array $data
		 */
		public function setData($data){ }


		/**
		 * Returns the data related to the profile
		 *
		 * @return array
		 */
		public function getData(){ }


		/**
		 * Sets the timestamp on when the profile started
		 *
		 * @param double $initialTime
		 */
		public function setInitialTime($initialTime){ }


		/**
		 * Returns the initial time in milseconds on when the profile started
		 *
		 * @return double
		 */
		public function getInitialTime(){ }


		/**
		 * Sets the timestamp on when the profile ended
		 *
		 * @param double $finalTime
		 */
		public function setFinalTime($finalTime){ }


		/**
		 * Returns the initial time in milseconds on when the profile ended
		 *
		 * @return double
		 */
		public function getFinalTime(){ }


		/**
		 * Returns the total time in seconds spent by the profile
		 *
		 * @return double|null
		 */
		public function getTotalElapsedSeconds(){ }


		/**
		 * Sets the amount of memory allocated on when the profile started
		 *
		 * @param double $startMemory
		 */
		public function setStartMemory($startMemory){ }


		/**
		 * Returns the amount of memory allocated on when the profile started
		 *
		 * @return double
		 */
		public function getStartMemory(){ }


		/**
		 * Sets the amount of memory allocated on when the profile ended
		 *
		 * @param double $endMemory
		 */
		public function setEndMemory($endMemory){ }


		/**
		 * Returns the amount of memory allocated on when the profile ended
		 *
		 * @return int
		 */
		public function getEndMemory(){ }


		/**
		 * Returns the total time in seconds spent by the profile
		 *
		 * @return int
		 */
		public function getTotalUsageMemory(){ }

	}
}
