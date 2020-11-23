<?php 

namespace Dao\Logger\Adapter {

	/**
	 * Dao\Logger\Adapter\Direct
	 *
	 *<code>
	 *	$logger = new \Dao\Logger\Adapter\Direct;
	 *	$logger->log(Dao\Logger::INFO, "This is a message");
	 *	$logger->log(Dao\Logger::ERROR, "This is an error");
	 *	$logger->error("This is another error");
	 *</code>
	 */
	
	class Direct extends \Dao\Logger\Adapter implements \Dao\Logger\AdapterInterface {

		/**
		 * Returns the internal formatter
		 *
		 * @return \Dao\Logger\Formatter\Line
		 */
		public function getFormatter(){ }


		/**
		 * Writes the log to the stream itself
		 *
		 * @param string $message
		 * @param int $type
		 * @param int $time
		 * @param array $context
		 */
		protected function logInternal($message, $type, $time, $context){ }


		/**
		 * Closes the logger
		 *
		 * @return boolean
		 */
		public function close(){ }

	}
}
