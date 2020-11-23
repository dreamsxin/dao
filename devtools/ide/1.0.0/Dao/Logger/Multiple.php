<?php 

namespace Dao\Logger {

	/**
	 * Dao\Logger\Multiple
	 *
	 * Handles multiples logger handlers
	 */
	
	class Multiple {

		protected $_loggers;

		protected $_formatter;

		/**
		 * Pushes a logger to the logger tail
		 *
		 * @param \Dao\Logger\AdapterInterface $logger
		 */
		public function push($logger){ }


		/**
		 * Returns the registered loggers
		 *
		 * @return \Dao\Logger\AdapterInterface[]
		 */
		public function getLoggers(){ }


		/**
		 * Sets a global formatter
		 *
		 * @param \Dao\Logger\FormatterInterface $formatter
		 */
		public function setFormatter($formatter){ }


		/**
		 * Returns a formatter
		 *
		 * @return \Dao\Logger\FormatterInterface
		 */
		public function getFormatter(){ }


		/**
		 * Sends a message to each registered logger
		 *
		 * @param string $message
		 * @param int $type
		 * @param array $context
		 */
		public function log($type, $message, $context=null){ }


		/**
		 * Sends/Writes an emergency message to the log
		 *
		 * @param string $message
		 * @param array $context
		 */
		public function emergency($message, $context=null){ }


		public function emergence($message, $context=null){ }


		/**
		 * Sends/Writes a debug message to the log
		 *
		 * @param string $message
		 * @param array $context
		 */
		public function debug($message, $context=null){ }


		/**
		 * Sends/Writes an error message to the log
		 *
		 * @param string $message
		 * @param array $context
		 */
		public function error($message, $context=null){ }


		/**
		 * Sends/Writes an info message to the log
		 *
		 * @param string $message
		 * @param array $context
		 */
		public function info($message, $context=null){ }


		/**
		 * Sends/Writes a notice message to the log
		 *
		 * @param string $message
		 * @param array $context
		 */
		public function notice($message, $context=null){ }


		/**
		 * Sends/Writes a warning message to the log
		 *
		 * @param string $message
		 * @param array $context
		 */
		public function warning($message, $context=null){ }


		/**
		 * Sends/Writes an alert message to the log
		 *
		 * @param string $message
		 * @param array $context
		 */
		public function alert($message, $context=null){ }

	}
}
