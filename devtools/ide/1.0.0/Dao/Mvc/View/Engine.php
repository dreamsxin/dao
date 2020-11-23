<?php 

namespace Dao\Mvc\View {

	/**
	 * Dao\Mvc\View\Engine
	 *
	 * All the template engine adapters must inherit this class. This provides
	 * basic interfacing between the engine and the Dao\Mvc\View component.
	 */
	
	abstract class Engine extends \Dao\Di\Injectable implements \Dao\Events\EventsAwareInterface, \Dao\Di\InjectionAwareInterface, \Dao\Mvc\View\EngineInterface {

		protected $_view;

		protected $_layout;

		protected $_params;

		protected $_methods;

		/**
		 * \Dao\Mvc\View\Engine constructor
		 *
		 * @param \Dao\Mvc\ViewInterface $view
		 * @param \Dao\DiInterface $dependencyInjector
		 */
		public function __construct($view, $dependencyInjector=null){ }


		/**
		 * Returns cached ouput on another view stage
		 *
		 * @return array
		 */
		public function getContent(){ }


		/**
		 * Start a new section block
		 *
		 * @param string $name
		 */
		public function startSection($name){ }


		/**
		 * Stop the current section block
		 *
		 * @return string
		 */
		public function stopSection(){ }


		/**
		 * Returns the content for a section block
		 *
		 * @param string $name
		 * @param string $default
		 * @return string|null
		 */
		public function section($name, $defaultValue=null){ }


		/**
		 * Renders a partial inside another view
		 *
		 * @param string $partialPath
		 * @param array $params
		 * @return string
		 */
		public function partial($partialPath){ }


		/**
		 * Returns the view component related to the adapter
		 *
		 * @return \Dao\Mvc\ViewInterface
		 */
		public function getView(){ }


		/**
		 * Adds a user-defined method
		 *
		 * @param string $name
		 * @param closure $methodCallable
		 * @return \Dao\Mvc\View\Engine
		 */
		public function addMethod($name, \Closure $handler){ }


		/**
		 * Handles method calls when a method is not implemented
		 *
		 * @param string $method
		 * @param array $arguments
		 * @return mixed
		 */
		public function __call($method, $arguments=null){ }

	}
}
