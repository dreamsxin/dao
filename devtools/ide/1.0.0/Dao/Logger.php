<?php 

namespace Dao {

	/**
	 * Dao\Logger
	 *
	 * Dao\Logger is a component whose purpose is create logs using
	 * different backends via adapters, generating options, formats and filters
	 * also implementing transactions.
	 *
	 *<code>
	 *	$logger = new Dao\Logger\Adapter\File("app/logs/test.log");
	 *	$logger->log(Dao\Logger::INFO, "This is a message");
	 *	$logger->log(Dao\Logger::ERROR, "This is an error");
	 *	$logger->error("This is another error");
	 *</code>
	 */
	
	abstract class Logger {

		const SPECIAL = 9;

		const CUSTOM = 8;

		const DEBUG = 7;

		const INFO = 6;

		const NOTICE = 5;

		const WARNING = 4;

		const ERROR = 3;

		const ALERT = 2;

		const CRITICAL = 1;

		const EMERGENCE = 0;

		const EMERGENCY = 0;

		/**
		 * Returns the string meaning of a logger constant
		 *
		 * @param  integer $type
		 * @return string
		 */
		public static function getTypeString($type){ }


		/**
		 * Handles method calls when a static method is not implemented
		 *
		 * @param string $method
		 * @param array $arguments
		 * @return mixed
		 */
		public static function __callStatic($method, $arguments=null){ }

	}
}
