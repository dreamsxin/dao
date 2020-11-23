<?php 

namespace Dao\Cli {

	/**
	 * Dao\Cli\Dispatcher
	 *
	 * Dispatching is the process of taking the command-line arguments, extracting the module name,
	 * task name, action name, and optional parameters contained in it, and then
	 * instantiating a task and calling an action on it.
	 *
	 *<code>
	 *
	 *	$di = new Dao\Di();
	 *
	 *	$dispatcher = new Dao\Cli\Dispatcher();
	 *
	 *  $dispatcher->setDI($di);
	 *
	 *	$dispatcher->setTaskName('posts');
	 *	$dispatcher->setActionName('index');
	 *	$dispatcher->setParams(array());
	 *
	 *	$handle = $dispatcher->dispatch();
	 *
	 *</code>
	 */
	
	class Dispatcher extends \Dao\Dispatcher implements \Dao\DispatcherInterface, \Dao\Di\InjectionAwareInterface, \Dao\Events\EventsAwareInterface {

		const EXCEPTION_NO_DI = 1;

		const EXCEPTION_CYCLIC_ROUTING = 2;

		const EXCEPTION_HANDLER_NOT_FOUND = 4;

		const EXCEPTION_INVALID_HANDLER = 8;

		const EXCEPTION_INVALID_PARAMS = 16;

		const EXCEPTION_ACTION_NOT_FOUND = 32;

		protected $_handlerSuffix;

		protected $_defaultHandler;

		protected $_defaultAction;

		/**
		 * Sets the default task suffix
		 *
		 * @param string $taskSuffix
		 */
		public function setTaskSuffix($taskSuffix){ }


		/**
		 * Gets the default task suffix
		 *
		 * @return string
		 */
		public function getTaskSuffix(){ }


		/**
		 * Sets the default task name
		 *
		 * @param string $taskName
		 */
		public function setDefaultTask($taskName){ }


		/**
		 * Sets the task name to be dispatched
		 *
		 * @param string $taskName
		 */
		public function setTaskName($taskName){ }


		/**
		 * Gets last dispatched task name
		 *
		 * @return string
		 */
		public function getTaskName(){ }


		/**
		 * Throws an internal exception
		 *
		 * @param string $message
		 * @param int $exceptionCode
		 */
		protected function _throwDispatchException(){ }


		/**
		 * Handles a user exception
		 *
		 * @param \Exception $exception
		 */
		protected function _handleException(){ }


		/**
		 * Possible task class name that will be located to dispatch the request
		 *
		 * @return string
		 */
		public function getTaskClass(){ }


		/**
		 * Returns the lastest dispatched controller
		 *
		 * @return \Dao\Cli\Task
		 */
		public function getLastTask(){ }


		/**
		 * Returns the active task in the dispatcher
		 *
		 * @return \Dao\Cli\Task
		 */
		public function getActiveTask(){ }

	}
}
