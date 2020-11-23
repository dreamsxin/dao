<?php 

namespace Dao\Storage\Frontend {

	/**
	 * Dao\Storage\Frontend\Base64
	 *
	 * Allows to cache data converting/deconverting them to base64.
	 *
	 * This adapters uses the base64_encode/base64_decode PHP's functions
	 */
	
	class Base64 implements \Dao\Storage\FrontendInterface {

		/**
		 * Serializes data before storing them
		 *
		 * @param mixed $data
		 * @return string
		 */
		public function beforeStore($data){ }


		/**
		 * Unserializes data after retrieval
		 *
		 * @param mixed $data
		 * @return mixed
		 */
		public function afterRetrieve($data){ }

	}
}
