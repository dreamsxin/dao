<?php 

namespace Dao\Logger\Adapter {

	/**
	 * Dao\Logger\Adapter\Firephp
	 *
	 * Sends logs to FirePHP
	 *
	 *<code>
	 *	$logger = new \Dao\Logger\Adapter\Firephp("");
	 *	$logger->log(Dao\Logger::INFO, "This is a message");
	 *	$logger->log(Dao\Logger::ERROR, "This is an error");
	 *	$logger->error("This is another error");
	 *</code>
	 */
	
	class Firephp extends \Dao\Logger\Adapter implements \Dao\Logger\AdapterInterface {

		private static $_initialized;

		private static $_index;

		/**
		 * Returns the internal formatter
		 *
		 * @return \Dao\Logger\FormatterInterface
		 */
		public function getFormatter(){ }


		/**
		 * Writes the log to the stream itself
		 *
		 * @param string $message
		 * @param int $type
		 * @param int $time
		 * @param array $context
		 * @see http://www.firephp.org/Wiki/Reference/Protocol
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
