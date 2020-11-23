<?php 

namespace Dao\Assets\Filters {

	/**
	 * Dao\Assets\Filters\None
	 *
	 * Returns the content without make any modification to the original source
	 */
	
	class None implements \Dao\Assets\FilterInterface {

		/**
		 * Returns the content without touching
		 *
		 * @param string $content
		 * @return $content
		 */
		public function filter($content){ }

	}
}
