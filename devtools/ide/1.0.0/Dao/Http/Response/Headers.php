<?php 

namespace Dao\Http\Response {

	/**
	 * Dao\Http\Response\Headers
	 *
	 * This class is a bag to manage the response headers
	 */
	
	class Headers implements \Dao\Http\Response\HeadersInterface {

		protected $_headers;

		/**
		 * Sets a header to be sent at the end of the request
		 *
		 * @param string $name
		 * @param string $value
		 */
		public function set($name, $value){ }


		/**
		 * Gets a header value from the internal bag
		 *
		 * @param string $name
		 * @return string
		 */
		public function get($name){ }


		/**
		 * Sets a raw header to be sent at the end of the request
		 *
		 * @param string $header
		 */
		public function setRaw($header){ }


		/**
		 * Removes a header to be sent at the end of the request
		 *
		 * @param string $header Header name
		 */
		public function remove($header_index){ }


		/**
		 * Sends the headers to the client
		 *
		 * @return boolean
		 */
		public function send(){ }


		/**
		 * Reset set headers
		 *
		 */
		public function reset(){ }


		/**
		 * Returns the current headers as an array
		 *
		 * @return array
		 */
		public function toArray(){ }


		/**
		 * Returns the current headers as an string
		 *
		 * @return string
		 */
		public function toString(){ }


		/**
		 * Restore a \Dao\Http\Response\Headers object
		 *
		 * @param array $data
		 * @return \Dao\Http\Response\Headers
		 */
		public static function __set_state($data){ }

	}
}
