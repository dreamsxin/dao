<?php 

namespace Dao\User {

	/**
	 * Dao\User\Logic
	 *
	 * This class can be used to provide user business logic an easy access to services in the application
	 */
	
	abstract class Logic extends \Dao\Di\Injectable implements \Dao\Events\EventsAwareInterface, \Dao\Di\InjectionAwareInterface {

		protected $_actionName;

		protected $_actionParams;

		protected $_content;

		/**
		 * Constructor for \Dao\User\Logic
		 *
		 * @param string $actionName
		 * @param array $params
		 */
		public function __construct($actionName=null, $arguments=null){ }


		/**
		 * Gets the action name
		 *
		 * @return string
		 */
		public function getActionName(){ }


		/**
		 * Gets action params
		 *
		 * @return array
		 */
		public function getActionParams(){ }


		/**
		 * Sets content
		 *
		 * @param mixed $content
		 */
		public function setContent($content){ }


		/**
		 * Gets content
		 *
		 * @return mixed
		 */
		public function getContent(){ }


		/**
		 * Loads an logic and prepares it for manipulation
		 *
		 * @param string $actionName
		 * @param array $params
		 * @return object
		 */
		public static function call($actionName=null, $arguments=null){ }

	}
}
