<?php 

namespace Dao\Mvc\View\Engine {

	/**
	 * Dao\Mvc\View\Engine\Php
	 *
	 * Adapter to use PHP itself as templating engine
	 */
	
	class Php extends \Dao\Mvc\View\Engine implements \Dao\Mvc\View\EngineInterface, \Dao\Di\InjectionAwareInterface, \Dao\Events\EventsAwareInterface {

		/**
		 * Renders a view using the template engine
		 *
		 * @param string $path
		 * @param array $params
		 * @param boolean $mustClean
		 */
		public function render($path, $params, $mustClean=null){ }

	}
}
