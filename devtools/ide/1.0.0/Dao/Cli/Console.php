<?php 

namespace Dao\Cli {

	/**
	 * Dao\Cli\Console
	 *
	 * This component allows to create CLI applications using Dao
	 */
	
	class Console extends \Dao\Application implements \Dao\Di\InjectionAwareInterface, \Dao\Events\EventsAwareInterface {

		protected $_modules;

		protected $_moduleObject;

		/**
		 * \Dao\Cli\Console constructor
		 */
		public function __construct($dependencyInjector=null){ }


		/**
		 * Merge modules with the existing ones
		 *
		 *<code>
		 *	$application->addModules(array(
		 *		'admin' => array(
		 *			'className' => 'Multiple\Admin\Module',
		 *			'path' => '../apps/admin/Module.php'
		 *		)
		 *	));
		 *</code>
		 *
		 * @param array $modules
		 */
		public function addModules($modules){ }


		/**
		 * Handle the command-line arguments.
		 *
		 *
		 * <code>
		 * 	$arguments = array(
		 * 		'task' => 'taskname',
		 * 		'action' => 'action',
		 * 		'params' => array('parameter1', 'parameter2')
		 * 	);
		 * 	$console->handle($arguments);
		 * </code>
		 *
		 * @param array $arguments
		 * @return mixed
		 */
		public function handle($data=null){ }

	}
}
