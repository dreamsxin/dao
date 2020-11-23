<?php 

namespace Dao\Assets\Filters {

	/**
	 * Dao\Assets\Filters\Jsmin
	 *
	 * Deletes the characters which are insignificant to JavaScript. Comments will be removed. Tabs will be
	 * replaced with spaces. Carriage returns will be replaced with linefeeds.
	 * Most spaces and linefeeds will be removed.
	 */
	
	class Jsmin implements \Dao\Assets\FilterInterface {

		/**
		 * Filters the content using JSMIN
		 *
		 * @param string $content
		 * @return $content
		 */
		public function filter($content){ }

	}
}
