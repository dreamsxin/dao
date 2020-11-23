<?php 

namespace Dao\Forms\Element {

	/**
	 * Dao\Forms\Element\Select
	 *
	 * Component SELECT (choice) for forms
	 */
	
	class Select extends \Dao\Forms\Element implements \Dao\Forms\ElementInterface {

		/**
		 * \Dao\Forms\Element\Select constructor
		 *
		 * @param string $name
		 * @param array $attributes
		 * @param array $options
		 * @param array $optionsValues
		 */
		public function __construct($name, $attributes=null, $options=null, $optionsValues=null, $type=null){ }


		/**
		 * Set the choice's options
		 *
		 * @param array|object $options
		 * @return \Dao\Forms\Element
		 */
		public function setOptions($options){ }


		/**
		 * Returns the choices' options
		 *
		 * @return array|object
		 */
		public function getOptions(){ }


		/**
		 * Adds an option to the current options
		 *
		 * @param array $option
		 * @return $this
		 */
		public function addOption($option){ }


		/**
		 * Renders the element widget returning html
		 *
		 * @param array $attributes
		 * @return string
		 */
		public function render($attributes=null){ }

	}
}
