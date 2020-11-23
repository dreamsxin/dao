<?php 

namespace Dao {

	/**
	 * Dao\Debug
	 *
	 * Provides debug capabilities to Dao applications
	 */
	
	class Debug {

		protected $_hideDocumentRoot;

		protected $_showBackTrace;

		protected $_showFiles;

		protected $_showFileFragment;

		protected $_data;

		protected static $_isActive;

		protected static $_charset;

		protected $_beforeContext;

		protected $_afterContext;

		protected static $_logger;

		protected static $_listen;

		protected static $_logs;

		/**
		 * Sets if files the exception's backtrace must be showed
		 *
		 * @param boolean $showBackTrace
		 * @return \Dao\Debug
		 */
		public function setShowBackTrace($showBackTrace){ }


		/**
		 * Set if files part of the backtrace must be shown in the output
		 *
		 * @param boolean $showFiles
		 * @return \Dao\Debug
		 */
		public function setShowFiles($showFiles){ }


		/**
		 * Sets if files must be completely opened and showed in the output
		 * or just the fragment related to the exception
		 *
		 * @param boolean $showFileFragment
		 * @return \Dao\Debug
		 */
		public function setShowFileFragment($showFileFragment){ }


		/**
		 * Listen for uncaught exceptions and unsilent notices or warnings
		 *
		 * @param boolean $exceptions
		 * @param boolean $lowSeverity
		 * @return \Dao\Debug
		 */
		public function listen($exceptions=null, $lowSeverity=null){ }


		/**
		 * Listen for uncaught exceptions
		 *
		 * @return \Dao\Debug
		 */
		public function listenExceptions(){ }


		/**
		 * Listen for unsilent notices or warnings or user-defined error
		 *
		 * @return \Dao\Debug
		 */
		public function listenLowSeverity(){ }


		/**
		 * Halts the request showing a backtrace
		 */
		public function halt(){ }


		/**
		 * Adds a variable to the debug output
		 *
		 * @param mixed $var
		 * @param string $key
		 * @return \Dao\Debug
		 */
		public function debugVar($var, $key=null){ }


		/**
		 * Clears are variables added previously
		 *
		 * @return \Dao\Debug
		 */
		public function clearVars(){ }


		/**
		 * Escapes a string with htmlentities
		 *
		 * @param string $value
		 * @return string
		 */
		protected function _escapeString(){ }


		/**
		 * Produces a recursive representation of an array
		 *
		 * @param array $argument
		 * @return string
		 */
		protected function _getArrayDump(){ }


		/**
		 * Produces an string representation of a variable
		 *
		 * @param mixed $variable
		 * @return string
		 */
		protected function _getVarDump(){ }


		/**
		 * Returns the major framework's version
		 *
		 * @return string
		 */
		public function getMajorVersion(){ }


		/**
		 * Generates a link to the current version documentation
		 *
		 * @return string
		 */
		public function getVersion(){ }


		/**
		 * Returns the css sources
		 *
		 * @return string
		 */
		public function getCssSources(){ }


		/**
		 * Returns the javascript sources
		 *
		 * @return string
		 */
		public function getJsSources(){ }


		/**
		 * Shows a backtrace item
		 *
		 * @param int $n
		 * @param array $trace
		 */
		protected function showTraceItem(){ }


		/**
		 * Handles uncaught exceptions
		 *
		 * @param \Exception $exception
		 * @return boolean
		 */
		public function onUncaughtException($exception){ }


		/**
		 * Handles user-defined error
		 *
		 * @param int $severity
		 * @param string $message
		 * @param string $file
		 * @param string $line
		 * @param array $context
		 * @return boolean
		 */
		public function onUserDefinedError($severity, $message, $file=null, $line=null, $context=null){ }


		/**
		 * Handles user-defined error
		 *
		 * @return boolean
		 */
		public function onShutdown(){ }


		/**
		 * Returns the number of lines deplayed before the error line
		 *
		 * @brief int \Dao\Debug::getLinesBeforeContext(void)
		 * @return int
		 */
		public function getLinesBeforeContext(){ }


		/**
		 * Sets the number of lines deplayed before the error line
		 *
		 * @brief \Dao\Debug \Dao\Debug::setLinesBeforeContext(int $lines)
		 * @param int $lines
		 * @return \Dao\Debug
		 */
		public function setLinesBeforeContext($lines){ }


		/**
		 * Returns the number of lines deplayed after the error line
		 *
		 * @brief int \Dao\Debug::getLinesAfterContext(void)
		 * @return int
		 */
		public function getLinesAfterContext(){ }


		/**
		 * Sets the number of lines deplayed after the error line
		 *
		 * @brief \Dao\Debug \Dao\Debug::setLinesAfterContext(int $lines)
		 * @param int $lines
		 * @return \Dao\Debug
		 */
		public function setLinesAfterContext($lines){ }


		protected function getFileLink($file, $line, $format){ }


		/**
		 * Sets logger
		 *
		 * @param \Dao\Logger\AdapterInterface $logger
		 */
		public static function setLogger($logger){ }


		/**
		 * Enable simple debug mode
		 *
		 * @param \Dao\Logger\AdapterInterface $logger
		 */
		public static function enable($logger=null){ }


		/**
		 * Disable simple debug mode
		 *
		 */
		public static function disable(){ }


		/**
		 * Check if debug mode
		 *
		 * @return boolean
		 */
		public static function isEnable(){ }


		/**
		 * Logs messages
		 *
		 * @param string $message
		 * @param mixed $type
		 * @param array $context
		 */
		public static function log($message, $type=null, $context=null){ }


		/**
		 * Dumps a string representation of variable to output
		 *
		 * @param mixed $var
		 */
		public static function dumpVar($var, $level=null){ }

	}
}
