<?php 

namespace Dao\Di\FactoryDefault {

	/**
	 * Dao\Di\FactoryDefault\Cli
	 *
	 * This is a variant of the standard Dao\Di. By default it automatically
	 * registers all the services provided by the framework.
	 * Thanks to this, the developer does not need to register each service individually.
	 * This class is specially suitable for CLI applications
	 */
	
	class Cli extends \Dao\Di implements \Dao\DiInterface, \ArrayAccess {

		/**
		 * \Dao\Di\FactoryDefault\Cli constructor
		 */
		public function __construct(){ }

	}
}
