<?php 

namespace Dao\Di {

	/**
	 * Dao\Di\FactoryDefault
	 *
	 * This is a variant of the standard Dao\Di. By default it automatically
	 * registers all the services provided by the framework. Thanks to this, the developer does not need
	 * to register each service individually providing a full stack framework
	 */
	
	class FactoryDefault extends \Dao\Di implements \Dao\DiInterface, \ArrayAccess {

		/**
		 * \Dao\Di\FactoryDefault constructor
		 */
		public function __construct(){ }

	}
}
