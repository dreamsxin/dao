<?php 

namespace Dao {

	/**
	 * Dao\Application
	 *
	 * Base class for Dao\Cli\Console and Dao\Mvc\Application
	 *
	 *</code>
	 */
	
	abstract class Application extends \Dao\Di\Injectable implements \Dao\Events\EventsAwareInterface, \Dao\Di\InjectionAwareInterface {

		protected $_defaultModule;

		protected $_modules;

		protected $_implicitView;

		/**
		 * Register an array of modules present in the application
		 *
		 *<code>
		 *	$this->registerModules(array(
		 *		'frontend' => array(
		 *			'className' => 'Multiple\Frontend\Module',
		 *			'path' => '../apps/frontend/Module.php'
		 *		),
		 *		'backend' => array(
		 *			'className' => 'Multiple\Backend\Module',
		 *			'path' => '../apps/backend/Module.php'
		 *		)
		 *	));
		 *</code>
		 *
		 * @param array $modules
		 * @param boolean $merge
		 * @param \Dao\Application
		 */
		public function registerModules($modules, $merge=null){ }


		/**
		 * Return the modules registered in the application
		 *
		 * @return array
		 */
		public function getModules(){ }


		/**
		 * Sets the module name to be used if the router doesn't return a valid module
		 *
		 * @param string $defaultModule
		 * @return \Dao\Application
		 */
		public function setDefaultModule($defaultModule){ }


		/**
		 * Returns the default module name
		 *
		 * @return string
		 */
		public function getDefaultModule(){ }


		abstract public function handle($data=null);

	}
}
