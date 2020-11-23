<?php 

namespace Dao\Mvc {

	/**
	 * Dao\Mvc\Application
	 *
	 * This component encapsulates all the complex operations behind instantiating every component
	 * needed and integrating it with the rest to allow the MVC pattern to operate as desired.
	 *
	 *<code>
	 *
	 * class Application extends \Dao\Mvc\Application
	 * {
	 *
	 *		/\**
	 *		 * Register the services here to make them general or register
	 *		 * in the ModuleDefinition to make them module-specific
	 *		 *\/
	 *		protected function _registerServices()
	 *		{
	 *
	 *		}
	 *
	 *		/\**
	 *		 * This method registers all the modules in the application
	 *		 *\/
	 *		public function main()
	 *		{
	 *			$this->registerModules(array(
	 *				'frontend' => array(
	 *					'className' => 'Multiple\Frontend\Module',
	 *					'path' => '../apps/frontend/Module.php'
	 *				),
	 *				'backend' => array(
	 *					'className' => 'Multiple\Backend\Module',
	 *					'path' => '../apps/backend/Module.php'
	 *				)
	 *			));
	 *		}
	 *	}
	 *
	 *	$application = new Application();
	 *	$application->main();
	 *
	 *</code>
	 */
	
	class Application extends \Dao\Application implements \Dao\Di\InjectionAwareInterface, \Dao\Events\EventsAwareInterface {

		protected $_implicitView;

		protected $_autoSendHeader;

		/**
		 * \Dao\Mvc\Application
		 *
		 * @param \Dao\Di $dependencyInjector
		 */
		public function __construct(\Dao\DiInterface $dependencyInjector=null){ }


		/**
		 * By default. The view is implicitly buffering all the output
		 * You can full disable the view component using this method
		 *
		 * @param boolean $implicitView
		 * @return \Dao\Mvc\Application
		 */
		public function useImplicitView($implicitView){ }


		/**
		 * Enable or disable sending cookies by each request
		 *
		 * @param boolean $autoSendHeader
		 * @return \Dao\Mvc\Application
		 */
		public function autoSendHeader($autoSendHeader){ }


		/**
		 * Handles a MVC request
		 *
		 * @param string $uri
		 * @return \Dao\Http\ResponseInterface
		 */
		public function handle($data=null){ }


		/**
		 * Does a HMVC request in the application
		 *
		 * @param array $location
		 * @param array $data
		 * @return mixed
		 */
		public function request($uri, $data=null, \Dao\DiInterface $dependencyInjector=null){ }

	}
}
