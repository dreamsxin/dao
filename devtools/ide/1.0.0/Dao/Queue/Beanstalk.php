<?php 

namespace Dao\Queue {

	/**
	 * Dao\Queue\Beanstalk
	 *
	 * Class to access the beanstalk queue service.
	 * Partially implements the protocol version 1.2
	 *
	 * @see http://www.igvita.com/2010/05/20/scalable-work-queues-with-beanstalk/
	 */
	
	class Beanstalk {

		protected $_connection;

		protected $_parameters;

		/**
		 * \Dao\Queue\Beanstalk
		 *
		 * @param array $options
		 */
		public function __construct($options=null){ }


		public function connect(){ }


		/**
		 * Inserts jobs into the queue
		 *
		 * @param string $data
		 * @param array $options
		 * @return string|boolean
		 */
		public function put($data, $options=null){ }


		/**
		 * Reserves a job in the queue
		 *
		 * @return boolean|\Dao\Queue\Beanstalk\Job
		 */
		public function reserve($timeout=null){ }


		/**
		 * Change the active tube. By default the tube is 'default'
		 *
		 * @param string $tube
		 * @return string|boolean
		 */
		public function choose($tube){ }


		/**
		 * Change the active tube. By default the tube is 'default'
		 *
		 * @param string $tube
		 * @return string|boolean
		 */
		public function watch($tube){ }


		/**
		 * Get stats of the Beanstalk server.
		 *
		 * @return boolean|array
		 */
		public function stats(){ }


		/**
		 * Get stats of a tube
		 *
		 * @param string $tube
		 * @return boolean|array
		 */
		public function statsTube($tube){ }


		/**
		 * Inspect the next ready job.
		 *
		 * @return boolean|\Dao\Queue\Beanstalk\Job
		 */
		public function peekReady(){ }


		/**
		 * Return the delayed job with the shortest delay left
		 *
		 * @return boolean|Dao\Queue\Beanstalk\Job
		 */
		public function peekDelayed(){ }


		/**
		 * Return the next job in the list of buried jobs
		 *
		 * @return boolean|Dao\Queue\Beanstalk\Job
		 */
		public function peekBuried(){ }


		/**
		 * The peek commands let the client inspect a job in the system.
		 *
		 * @return boolean|\Dao\Queue\Beanstalk\Job
		 */
		public function jobPeek($id){ }


		/**
		 * Reads the latest status from the Beanstalkd server
		 *
		 * @return array
		 */
		public function readStatus(){ }


		/**
		 * Fetch a YAML payload from the Beanstalkd server
		 */
		public function readYaml(){ }


		/**
		 * Reads a packet from the socket. Prior to reading from the socket will
		 * check for availability of the connection.
		 *
		 * @param int $length Number of bytes to read.
		 * @return string|boolean Data or `false` on error.
		 */
		public function read($length=null){ }


		/**
		 * Writes data to the socket. Performs a connection if none is available
		 *
		 * @param string $data
		 * @return integer|boolean
		 */
		public function write($data){ }


		/**
		 * Closes the connection to the beanstalk server.
		 *
		 * @return boolean
		 */
		public function disconnect(){ }


		/**
		 * Simply closes the connection.
		 *
		 * @return boolean
		 */
		public function quit(){ }


		/**
		 * Returns a list of all existing tubes.
		 *
		 * @return array
		 */
		public function listTubes(){ }


		/**
		 * Returns the tube currently being used by the client.
		 *
		 * @return array
		 */
		public function listTubeUsed(){ }


		/**
		 * Returns a list tubes currently being watched by the client.
		 *
		 * @return array
		 */
		public function listTubesWatched(){ }


		/**
		 * It removes the named tube from the watch list for the current connection.
		 *
		 * @return boolean|int
		 */
		public function ignore($tube){ }


		public function __sleep(){ }


		public function __wakeup(){ }

	}
}
