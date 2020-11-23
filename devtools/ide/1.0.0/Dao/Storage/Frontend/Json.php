<?php 

namespace Dao\Storage\Frontend {

	/**
	 * Dao\Storage\Frontend\Json
	 *
	 * Allows to cache data converting/deconverting them to JSON.
	 *
	 * This adapters uses the json_encode/json_decode PHP's functions
	 *
	 * As the data is encoded in JSON other systems accessing the same backend could process them
	 *</code>
	 */
	
	class Json implements \Dao\Storage\FrontendInterface {

		/**
		 * Serializes data before storing it
		 *
		 * @param mixed $data
		 * @return string
		 */
		public function beforeStore($data){ }


		/**
		 * Unserializes data after retrieving it
		 *
		 * @param mixed $data
		 * @return mixed
		 */
		public function afterRetrieve($data){ }

	}
}
