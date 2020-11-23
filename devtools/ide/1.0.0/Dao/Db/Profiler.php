<?php 

namespace Dao\Db {

	/**
	 * Dao\Db\Profiler
	 *
	 * Instances of Dao\Db can generate execution profiles
	 * on SQL statements sent to the relational database. Profiled
	 * information includes execution time in miliseconds.
	 * This helps you to identify bottlenecks in your applications.
	 *
	 *<code>
	 *
	 *	$profiler = new Dao\Db\Profiler();
	 *
	 *	//Set the connection profiler
	 *	$connection->setProfiler($profiler);
	 *
	 *	$sql = "SELECT buyer_name, quantity, product_name
	 *	FROM buyers LEFT JOIN products ON
	 *	buyers.pid=products.id";
	 *
	 *	//Execute a SQL statement
	 *	$connection->query($sql);
	 *
	 *	//Get the last profile in the profiler
	 *	$profile = $profiler->getLastProfile();
	 *
	 *	echo "SQL Statement: ", $profile->getSQLStatement(), "\n";
	 *	echo "Start Time: ", $profile->getInitialTime(), "\n";
	 *	echo "Final Time: ", $profile->getFinalTime(), "\n";
	 *	echo "Total Elapsed Time: ", $profile->getTotalElapsedSeconds(), "\n";
	 *
	 *</code>
	 *
	 */
	
	class Profiler extends \Dao\Profiler implements \Dao\ProfilerInterface {

		/**
		 * Starts the profile of a SQL sentence
		 *
		 * @param string $name
		 * @param array $data
		 * @param boolean $unique
		 * @return \Dao\Db\Profiler\Item
		 */
		public function startProfile($name, $data=null){ }


		/**
		 * Returns the total number of SQL statements processed
		 *
		 * @return integer
		 */
		public function getNumberTotalStatements(){ }

	}
}
