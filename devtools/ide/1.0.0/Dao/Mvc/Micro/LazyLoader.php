<?php 

namespace Dao\Mvc\Micro {

	/**
	 * Dao\Mvc\Micro\LazyLoader
	 *
	 * Lazy-Load of handlers for Mvc\Micro using auto-loading
	 */
	
	class LazyLoader {

		protected $_handler;

		protected $_definition;

		/**
		 * \Dao\Mvc\Micro\LazyLoader constructor
		 *
		 * @param string $definition
		 */
		public function __construct($definition){ }


		/**
		 * Initializes the internal handler, calling functions on it
		 *
		 * @param string $method
		 * @param array $arguments
		 * @return mixed
		 */
		public function __call($method, $arguments){ }

	}
}
