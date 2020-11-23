<?php 

namespace Dao\Storage\Frontend {

	/**
	 * Dao\Storage\Frontend\Igbinary
	 *
	 * Allows to cache native PHP data in a serialized form using igbinary extension
	 */
	
	class Igbinary implements \Dao\Storage\FrontendInterface {

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
