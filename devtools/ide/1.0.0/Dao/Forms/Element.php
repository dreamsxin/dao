<?php 

namespace Dao\Forms {

	/**
	 * Dao\Forms\Element
	 *
	 * This is a base class for form elements
	 */
	
	class Element implements \Dao\Forms\ElementInterface {

		protected $_form;

		protected $_name;

		protected $_type;

		protected $_value;

		protected $_label;

		protected $_attributes;

		protected $_validators;

		protected $_filters;

		protected $_options;

		protected $_optionsValues;

		protected $_messages;

		/**
		 * \Dao\Forms\Element constructor
		 *
		 * @param string $name
		 * @param array $attributes
		 * @param array $options
		 * @param array $optionsValues
		 */
		public function __construct($name, $attributes=null, $options=null, $optionsValues=null, $type=null){ }


		/**
		 * Sets the parent form to the element
		 *
		 * @param \Dao\Forms\Form $form
		 * @return \Dao\Forms\ElementInterface
		 */
		public function setForm($form){ }


		/**
		 * Returns the parent form to the element
		 *
		 * @return \Dao\Forms\ElementInterface
		 */
		public function getForm(){ }


		/**
		 * Sets the element's name
		 *
		 * @param string $name
		 * @return \Dao\Forms\ElementInterface
		 */
		public function setName($name){ }


		/**
		 * Returns the element's name
		 *
		 * @return string
		 */
		public function getName(){ }


		/**
		 * Sets the element's filters
		 *
		 * @param array|string $filters
		 * @return \Dao\Forms\ElementInterface
		 */
		public function setFilters($filters){ }


		/**
		 * Adds a filter to current list of filters
		 *
		 * @param string $filter
		 * @return \Dao\Forms\ElementInterface
		 */
		public function addFilter($filter){ }


		/**
		 * Returns the element's filters
		 *
		 * @return mixed
		 */
		public function getFilters(){ }


		/**
		 * Adds a group of validators
		 *
		 * @param \Dao\Validation\ValidatorInterface[]
		 * @return \Dao\Forms\ElementInterface
		 */
		public function addValidators($validators, $merge=null){ }


		/**
		 * Adds a validator to the element
		 *
		 * @param \Dao\Validation\ValidatorInterface
		 * @return \Dao\Forms\ElementInterface
		 */
		public function addValidator($validator){ }


		/**
		 * Returns the validators registered for the element
		 *
		 * @return \Dao\Validation\ValidatorInterface[]
		 */
		public function getValidators(){ }


		/**
		 * Returns an array of prepared attributes for \Dao\Tag helpers
		 * according to the element's parameters
		 *
		 * @param array $attributes
		 * @param boolean $useChecked
		 * @return array
		 */
		public function prepareAttributes($attributes=null, $useChecked=null){ }


		/**
		 * Sets a default attribute for the element
		 *
		 * @param string $attribute
		 * @param mixed $value
		 * @return \Dao\Forms\ElementInterface
		 */
		public function setAttribute($attribute, $value){ }


		/**
		 * Returns the value of an attribute if present
		 *
		 * @param string $attribute
		 * @param mixed $defaultValue
		 * @return mixed
		 */
		public function getAttribute($attribute, $defaultValue=null){ }


		/**
		 * Sets default attributes for the element
		 *
		 * @param array $attributes
		 * @return \Dao\Forms\ElementInterface
		 */
		public function setAttributes($attributes){ }


		/**
		 * Returns the default attributes for the element
		 *
		 * @return array
		 */
		public function getAttributes(){ }


		/**
		 * Sets an option for the element
		 *
		 * @param string $option
		 * @param mixed $value
		 * @return \Dao\Forms\ElementInterface
		 */
		public function setUserOption($option, $value){ }


		/**
		 * Returns the value of an option if present
		 *
		 * @param string $option
		 * @param mixed $defaultValue
		 * @return mixed
		 */
		public function getUserOption($option, $defaultValue=null){ }


		/**
		 * Sets options for the element
		 *
		 * @param array $options
		 * @return \Dao\Forms\ElementInterface
		 */
		public function setUserOptions($options){ }


		/**
		 * Returns the options for the element
		 *
		 * @return array
		 */
		public function getUserOptions(){ }


		/**
		 * Sets the element label
		 *
		 * @param string $label
		 * @return \Dao\Forms\ElementInterface
		 */
		public function setLabel($label){ }


		/**
		 * Returns the element's label
		 *
		 * @return string
		 */
		public function getLabel(){ }


		/**
		 * Generate the HTML to label the element
		 *
		 * @return string
		 */
		public function label($attributes=null){ }


		/**
		 * Sets a default value in case the form does not use an entity
		 * or there is no value available for the element in $_POST
		 *
		 * @param mixed $value
		 * @return \Dao\Forms\ElementInterface
		 */
		public function setDefault($value){ }


		/**
		 * Returns the default value assigned to the element
		 *
		 * @return mixed
		 */
		public function getDefault(){ }


		/**
		 * Returns the element's value
		 *
		 * @return mixed
		 */
		public function getValue(){ }


		/**
		 * Sets the element's value
		 *
		 */
		public function setValue($value=null){ }


		/**
		 * Returns the messages that belongs to the element
		 * The element needs to be attached to a form
		 *
		 * @return \Dao\Validation\Message\Group
		 */
		public function getMessages(){ }


		/**
		 * Checks whether there are messages attached to the element
		 *
		 * @return boolean
		 */
		public function hasMessages(){ }


		/**
		 * Sets the validation messages related to the element
		 *
		 * @param \Dao\Validation\Message\Group $group
		 * @return \Dao\Forms\ElementInterface
		 */
		public function setMessages($group){ }


		/**
		 * Appends a message to the internal message list
		 *
		 * @param \Dao\Validation\Message $message
		 * @return \Dao\Forms\ElementInterface
		 */
		public function appendMessage($message){ }


		/**
		 * Clears every element in the form to its default value
		 *
		 * @return \Dao\Forms\Element
		 */
		public function clear(){ }


		/**
		 * Renders the element widget returning html
		 *
		 * @param array $attributes
		 * @return string
		 */
		public function render($attributes=null){ }


		/**
		 * Returns a element all attributes
		 *
		 * @return array
		 */
		public function toArray(){ }


		/**
		 * Magic method __toString renders the widget without attributes
		 *
		 * @return string
		 */
		public function __toString(){ }

	}
}
