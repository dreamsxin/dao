<?php 

namespace Dao {

	/**
	 * Dao\Profiler
	 *
	 * Instances of Dao\Db can generate execution profiles
	 * on SQL statements sent to the relational database. Profiled
	 * information includes execution time in miliseconds.
	 * This helps you to identify bottlenecks in your applications.
	 *
	 *<code>
	 *
	 *	$profiler = new Dao\Profiler();
	 *
	 *	//Set the connection profiler
	 *	$connection->setProfiler($profiler);
	 *
	 *	//Get the last profile in the profiler
	 *	$profile = $profiler->getLastProfile();
	 *
	 *	echo "Data: ", $profile->getData(), "\n";
	 *	echo "Start Time: ", $profile->getInitialTime(), "\n";
	 *	echo "Final Time: ", $profile->getFinalTime(), "\n";
	 *	echo "Total Elapsed Time: ", $profile->getTotalElapsedSeconds(), "\n";
	 *
	 *</code>
	 *
	 */
	
	class Profiler implements \Dao\ProfilerInterface {

		protected $_unique;

		protected $_queue;

		protected $_allProfiles;

		protected $_currentProfile;

		protected $_activeProfile;

		protected $_totalSeconds;

		protected $_totalMemory;

		/**
		 * Constructor for \Dao\Profiler\Item
		 *
		 * @param array $data
		 */
		public function __construct($unique=null){ }


		/**
		 * Starts the profile
		 *
		 * @param string $name
		 * @param array $data
		 * @param boolean $unique
		 * @return \Dao\Profiler\ItemInterface
		 */
		public function startProfile($name, $data=null){ }


		/**
		 * Stops the active profile
		 *
		 * @return \Dao\Profiler
		 */
		public function stopProfile($name=null){ }


		/**
		 * Returns the total time in seconds spent by the profiles
		 *
		 * @return double
		 */
		public function getTotalElapsedSeconds(){ }


		/**
		 * Returns the total time in seconds spent by the profiles
		 *
		 * @return int
		 */
		public function getTotalUsageMemory(){ }


		/**
		 * Returns all the processed profiles
		 *
		 * @return \Dao\Profiler\Item[]
		 */
		public function getProfiles(){ }


		/**
		 * Returns the last profile executed in the profiler
		 *
		 * @return \Dao\Profiler\Item
		 */
		public function getLastProfile(){ }


		/**
		 * Returns the current profile executed in the profiler
		 *
		 * @return \Dao\Profiler\Item
		 */
		public function getCurrentProfile(){ }


		/**
		 * Resets the profiler, cleaning up all the profiles
		 *
		 * @return \Dao\Profiler
		 */
		public function reset(){ }

	}
}
