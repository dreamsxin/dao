<?php 

namespace Dao\Forms\Element {

	/**
	 * Dao\Forms\Element\Text
	 *
	 * Component INPUT[type=text] for forms
	 */
	
	class Text extends \Dao\Forms\Element implements \Dao\Forms\ElementInterface {

		/**
		 * \Dao\Forms\Element\Text constructor
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
