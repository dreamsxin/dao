<?php 

namespace Dao\Logger {

	/**
	 * Dao\Logger\Formatter
	 *
	 * This is a base class for logger formatters
	 */
	
	abstract class Formatter implements \Dao\Logger\FormatterInterface {

		protected $_typeStrings;

		/**
		 * Sets the string meaning of a logger constant
		 *
		 * @param array $types
		 * @return \Dao\Logger\Formatter
		 */
		public function setTypeStrings($types){ }


		/**
		 * Returns the type strings
		 *
		 * @return array
		 */
		public function getTypeStrings(){ }


		/**
		 * Sets the type strings
		 *
		 * @param string $type
		 * @param string $name
		 * @return \Dao\Logger\Formatter
		 */
		public function setTypeString($type, $name){ }


		/**
		 * Returns the string meaning of a logger constant
		 *
		 * @param  integer $type
		 * @return string
		 */
		public function getTypeString($type){ }


		/**
		 * Interpolates context values into the message placeholders
		 *
		 * @see http://www.php-fig.org/psr/psr-3/ Section 1.2 Message
		 * @param string $message
		 * @param array $context
		 */
		protected function interpolate($message, $context){ }

	}
}
