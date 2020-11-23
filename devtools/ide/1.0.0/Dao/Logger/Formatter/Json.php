<?php 

namespace Dao\Logger\Formatter {

	/**
	 * Dao\Logger\Formatter\Json
	 *
	 * Formats messages using JSON encoding
	 */
	
	class Json extends \Dao\Logger\Formatter implements \Dao\Logger\FormatterInterface {

		protected $_options;

		/**
		 * \Dao\Logger\Formatter\Json constructor
		 *
		 * @param int $options
		 */
		public function __construct($options=null){ }


		/**
		 * Applies a format to a message before sent it to the internal log
		 *
		 * @param string $message
		 * @param int $type
		 * @param int $timestamp
		 * @param array $context
		 * @return string
		 */
		public function format($message, $type, $timestamp, $context){ }

	}
}
