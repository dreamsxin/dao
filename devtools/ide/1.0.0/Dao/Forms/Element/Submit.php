<?php 

namespace Dao\Forms\Element {

	/**
	 * Dao\Forms\Element\Submit
	 *
	 * Component INPUT[type=submit] for forms
	 */
	
	class Submit extends \Dao\Forms\Element implements \Dao\Forms\ElementInterface {

		/**
		 * \Dao\Forms\Element\Submit constructor
		 *
		 * @param string $name
		 * @param array $attributes
		 * @param array $options
		 * @param array $optionsValues
		 */
		public function __construct($name, $attributes=null, $options=null, $optionsValues=null, $type=null){ }


		/**
		 * Renders the element widget
		 *
		 * @param array $attributes
		 * @return string
		 */
		public function render($attributes=null){ }

	}
}
