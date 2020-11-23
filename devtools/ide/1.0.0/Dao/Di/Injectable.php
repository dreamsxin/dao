<?php 

namespace Dao\Di {

	/**
	 * Dao\Di\Injectable
	 *
	 * This class allows to access services in the services container by just only accessing a public property
	 * with the same name of a registered service
	 */
	
	abstract class Injectable implements \Dao\Di\InjectionAwareInterface, \Dao\Events\EventsAwareInterface {

		protected $_dependencyInjector;

		protected $_eventsManager;

		protected $_eventCallbacks;

		/**
 		 * @var \Dao\Mvc\Dispatcher|\Dao\Mvc\DispatcherInterface
 		 */
		public $dispatcher;

		/**
 		 * @var \Dao\Mvc\Router|\Dao\Mvc\RouterInterface
 		 */
		public $router;

		/**
 		 * @var \Dao\Mvc\Url|\Dao\Mvc\UrlInterface
 		 */
		public $url;

		/**
 		 * @var \Dao\Http\Request|\Dao\HTTP\RequestInterface
 		 */
		public $request;

		/**
 		 * @var \Dao\Http\Response|\Dao\HTTP\ResponseInterface
 		 */
		public $response;

		/**
 		 * @var \Dao\Http\Response\Cookies|\Dao\Http\Response\CookiesInterface
 		 */
		public $cookies;

		/**
 		 * @var \Dao\Filter|\Dao\FilterInterface
 		 */
		public $filter;

		/**
 		 * @var \Dao\Flash\Direct
 		 */
		public $flash;

		/**
 		 * @var \Dao\Flash\Session
 		 */
		public $flashSession;

		/**
 		 * @var \Dao\Session\Adapter\Files|\Dao\Session\Adapter|\Dao\Session\AdapterInterface
 		 */
		public $session;

		/**
 		 * @var \Dao\Events\Manager
 		 */
		public $eventsManager;

		/**
 		 * @var \Dao\Db
 		 */
		public $db;

		/**
 		 * @var \Dao\Security
 		 */
		public $security;

		/**
 		 * @var \Dao\Crypt
 		 */
		public $crypt;

		/**
 		 * @var \Dao\Tag
 		 */
		public $tag;

		/**
 		 * @var \Dao\Escaper|\Dao\EscaperInterface
 		 */
		public $escaper;

		/**
 		 * @var \Dao\Annotations\Adapter\Memory|\Dao\Annotations\Adapter
 		 */
		public $annotations;

		/**
 		 * @var \Dao\Mvc\Model\Manager|\Dao\Mvc\Model\ManagerInterface
 		 */
		public $modelsManager;

		/**
 		 * @var \Dao\Mvc\Model\MetaData\Memory|\Dao\Mvc\Model\MetadataInterface
 		 */
		public $modelsMetadata;

		/**
 		 * @var \Dao\Mvc\Model\Transaction\Manager
 		 */
		public $transactionManager;

		/**
 		 * @var \Dao\Assets\Manager
 		 */
		public $assets;

		/**
		 * @var \Dao\Di|\Dao\DiInterface
	 	 */
		public $di;

		/**
		 * @var \Dao\Session\Bag
	 	 */
		public $persistent;

		/**
 		 * @var \Dao\Mvc\View|\Dao\Mvc\ViewInterface
 		 */
		public $view;
		
		/**
		 * Sets the dependency injector
		 *
		 * @param \Dao\DiInterface $dependencyInjector
		 * @throw \Dao\Di\Exception
		 */
		public function setDI(\Dao\DiInterface $dependencyInjector){ }


		/**
		 * Returns the internal dependency injector
		 *
		 * @return \Dao\DiInterface
		 */
		public function getDI($error=null, $notUseDefault=null){ }


		/**
		 * Sets the event manager
		 *
		 * @param \Dao\Events\ManagerInterface $eventsManager
		 */
		public function setEventsManager(\Dao\Events\ManagerInterface $eventsManager){ }


		/**
		 * Returns the internal event manager
		 *
		 * @return \Dao\Events\ManagerInterface
		 */
		public function getEventsManager(){ }


		/**
		 * Fires an event, implicitly calls behaviors and listeners in the events manager are notified
		 *
		 * @param string $eventName
		 * @param mixed $data
		 * @return boolean
		 */
		public function fireEvent($eventName, $data=null, $cancelable=null){ }


		/**
		 * Fires an event, can stop the event by returning to the false
		 *
		 * @param string $eventName
		 * @param mixed $data
		 * @return mixed
		 */
		public function fireEventCancel($eventName, $data=null, $cancelable=null){ }


		/**
		 * Check whether the DI contains a service by a name
		 *
		 * @param string $name
		 * @return boolean
		 */
		public function hasService($name){ }


		/**
		 * Sets a service from the DI
		 *
		 * @param string $serviceName
		 * @param mixed $definition
		 * @param boolean $shared
		 * @return \Dao\Di\ServiceInterface
		 */
		public function setService($name){ }


		/**
		 * Obtains a service from the DI
		 *
		 * @param string $serviceName
		 * @return object|null
		 */
		public function getService($name){ }


		/**
		 * Resolves the service based on its configuration
		 *
		 * @param string $name
		 * @param array $parameters
		 * @param boolean $noError
		 * @param boolean $noShared
		 * @return mixed
		 */
		public function getResolveService($name, $args=null, $noerror=null, $noshared=null){ }


		/**
		 * Attach a listener to the events
		 *
		 * @param string $eventType
		 * @param Closure $callback
		 */
		public function attachEvent($eventType, \Closure $callback){ }


		/**
		 * Magic method __get
		 *
		 * @param string $propertyName
		 */
		public function __get($property){ }


		public function __sleep(){ }


		public function __debugInfo(){ }

	}
}
