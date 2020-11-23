<?php 

namespace Dao\Forms\Element {

	/**
	 * Dao\Forms\Element\Date
	 *
	 * Component INPUT[type=date] for forms
	 */
	
	class Date extends \Dao\Forms\Element implements \Dao\Forms\ElementInterface {

		/**
		 * \Dao\Forms\Element\Date constructor
		 *
		 * @param string $name
		 * @param array $attributes
		 * @param array $options
		 * @param array $optionsValues
		 */
		public function __construct($name, $attributes=null, $options=null, $optionsValues=null, $type=null){ }


		/**
		 * Renders the element widget returning html
		 *
		 * @param array $attributes
		 * @return string
		 */
		public function render($attributes=null){ }

	}
}
