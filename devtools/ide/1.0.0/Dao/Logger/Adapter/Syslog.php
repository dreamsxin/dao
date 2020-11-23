<?php 

namespace Dao\Logger\Adapter {

	/**
	 * Dao\Logger\Adapter\Syslog
	 *
	 * Sends logs to the system logger
	 *
	 *<code>
	 *	$logger = new \Dao\Logger\Adapter\Syslog("ident", array(
	 *		'option' => LOG_NDELAY,
	 *		'facility' => LOG_MAIL
	 *	));
	 *	$logger->log(Dao\Logger::INFO, "This is a message");
	 *	$logger->log(Dao\Logger::ERROR, "This is an error");
	 *	$logger->error("This is another error");
	 *</code>
	 */
	
	class Syslog extends \Dao\Logger\Adapter implements \Dao\Logger\AdapterInterface {

		protected $_opened;

		/**
		 * \Dao\Logger\Adapter\Syslog constructor
		 *
		 * @param string $name
		 * @param array $options
		 */
		public function __construct($name, $options=null){ }


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
