<?php 

namespace Dao\Assets\Filters {

	/**
	 * Dao\Assets\Filters\Cssmin
	 *
	 * Minify the css - removes comments
	 * removes newlines and line feeds keeping
	 * removes last semicolon from last property
	 */
	
	class Cssmin implements \Dao\Assets\FilterInterface {

		/**
		 * Filters the content using CSSMIN
		 *
		 * @param string $content
		 * @return $content
		 */
		public function filter($content){ }

	}
}
