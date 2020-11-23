<?php 

namespace Dao\Mvc\Router {

	/**
	 * Dao\Mvc\Router\Route
	 *
	 * This class represents every route added to the router
	 */
	
	class Route implements \Dao\Mvc\Router\RouteInterface {

		protected $_pattern;

		protected $_compiledPattern;

		protected $_paths;

		protected $_methods;

		protected $_prefix;

		protected $_hostname;

		protected $_converters;

		protected $_id;

		protected $_name;

		protected $_beforeMatch;

		protected $_group;

		protected static $_uniqueId;

		protected $_defaultNamespace;

		protected $_defaultModule;

		protected $_defaultController;

		protected $_defaultAction;

		protected $_defaultParams;

		protected $_urlGenerator;

		protected $_caseSensitive;

		protected $_mode;

		/**
		 * \Dao\Mvc\Router\Route constructor
		 *
		 * @param string $pattern
		 * @param array $paths
		 * @param array|string $httpMethods
		 */
		public function __construct($pattern, $paths=null, $httpMethods=null, $regex=null){ }


		/**
		 * Replaces placeholders from pattern returning a valid PCRE regular expression
		 *
		 * @param string $pattern
		 * @param array $paths
		 * @return string
		 */
		public function compilePattern($pattern, $regex=null){ }


		/**
		 * Set one or more HTTP methods that constraint the matching of the route
		 *
		 *<code>
		 * $route->via('GET');
		 * $route->via(array('GET', 'POST'));
		 *</code>
		 *
		 * @param string|array $httpMethods
		 * @return \Dao\Mvc\Router\Route
		 */
		public function via($httpMethods){ }


		/**
		 * Reconfigure the route adding a new pattern and a set of paths
		 *
		 * @param string $pattern
		 * @param string|array $paths
		 */
		public function reConfigure($pattern, $paths=null, $regex=null){ }


		/**
		 * Returns the route's name
		 *
		 * @return string
		 */
		public function getName(){ }


		/**
		 * Sets the route's name
		 *
		 *<code>
		 * $router->add('/about', array(
		 *     'controller' => 'about'
		 * ))->setName('about');
		 *</code>
		 *
		 * @param string $name
		 * @return \Dao\Mvc\Router\Route
		 */
		public function setName($name){ }


		/**
		 * Sets a callback that is called if the route is matched.
		 * The developer can implement any arbitrary conditions here
		 * If the callback returns false the route is treaded as not matched
		 *
		 * @param callback $callback
		 * @return \Dao\Mvc\Router\Route
		 */
		public function beforeMatch(\Closure $callback){ }


		/**
		 * Returns the 'before match' callback if any
		 *
		 * @return mixed
		 */
		public function getBeforeMatch(){ }


		/**
		 * Returns the route's id
		 *
		 * @return string
		 */
		public function getRouteId(){ }


		/**
		 * Returns the route's pattern
		 *
		 * @return string
		 */
		public function getPattern(){ }


		/**
		 * Returns the route's compiled pattern
		 *
		 * @return string
		 */
		public function getCompiledPattern(){ }


		/**
		 * Returns the paths
		 *
		 * @return array
		 */
		public function getPaths(){ }


		/**
		 * Returns the paths using positions as keys and names as values
		 *
		 * @return array
		 */
		public function getReversedPaths(){ }


		/**
		 * Sets a set of HTTP methods that constraint the matching of the route (alias of via)
		 *
		 *<code>
		 * $route->setHttpMethods('GET');
		 * $route->setHttpMethods(array('GET', 'POST'));
		 *</code>
		 *
		 * @param string|array $httpMethods
		 * @return \Dao\Mvc\Router\Route
		 */
		public function setHttpMethods($httpMethods){ }


		/**
		 * Returns the HTTP methods that constraint matching the route
		 *
		 * @return string|array
		 */
		public function getHttpMethods(){ }


		/**
		 * Set a common uri prefix for the route
		 *
		 * @param string $prefix
		 * @return \Dao\Mvc\Router\Route
		 */
		public function setPrefix($prefix){ }


		/**
		 * Returns the common prefix for the route
		 *
		 * @return string
		 */
		public function getPrefix(){ }


		/**
		 * Sets a hostname restriction to the route
		 *
		 *<code>
		 * $route->setHostname('localhost');
		 *</code>
		 *
		 * @param string|array $httpMethods
		 * @return \Dao\Mvc\Router\Route
		 */
		public function setHostname($hostname){ }


		/**
		 * Returns the hostname restriction if any
		 *
		 * @return string
		 */
		public function getHostname(){ }


		/**
		 * Sets the group associated with the route
		 *
		 * @param \Dao\Mvc\Router\Group $group
		 * @return \Dao\Mvc\RouteInterface
		 */
		public function setGroup(\Dao\Mvc\Router\Group $group){ }


		/**
		 * Returns the group associated with the route
		 *
		 * @return \Dao\Mvc\Router\Group|null
		 */
		public function getGroup(){ }


		/**
		 * Adds a converter to perform an additional transformation for certain parameter
		 *
		 * @param string $name
		 * @param callable $converter
		 * @return \Dao\Mvc\Router\Route
		 */
		public function convert($name, \Closure $converter){ }


		/**
		 * Returns the router converter
		 *
		 * @return array
		 */
		public function getConverters(){ }


		/**
		 * Resets the internal route id generator
		 */
		public static function reset(){ }


		/**
		 * Sets an array of default paths.
		 *
		 * @param array $defaults
		 * @return \Dao\Mvc\Router\Route
		 */
		public function setDefaults($defaults){ }


		/**
		 * Returns an array of default parameters
		 *
		 * @return array
		 */
		public function getDefaults(){ }


		/**
		 * Returns the name of the default namespace
		 *
		 * @return string
		 */
		public function getDefaultNamespace(){ }


		/**
		 * Returns the name of the default module
		 *
		 * @return string
		 */
		public function getDefaultModule(){ }


		/**
		 * Returns the default controller name
		 *
		 * @return string
		 */
		public function getDefaultController(){ }


		/**
		 * Returns the default action name
		 *
		 * @return string
		 */
		public function getDefaultAction(){ }


		/**
		 * Returns the default params
		 *
		 * @return mixed
		 */
		public function getDefaultParams(){ }


		/**
		 * Sets the Url Generator
		 * @param callable $generator
		 * @return string
		 */
		public function setUrlGenerator(\Closure $handler){ }


		/**
		 * Returns the Url Generator
		 *
		 * @return callable
		 */
		public function getUrlGenerator(){ }


		/**
		 * Sets the case sensitive
		 * @param boolean $caseSensitive
		 * @return string
		 */
		public function setCaseSensitive($caseSensitive){ }


		/**
		 * Returns the case sensitive
		 *
		 * @return boolean
		 */
		public function getCaseSensitive(){ }


		/**
		 * Sets the mode
		 *
		 * @param int $mode
		 */
		public function setMode($mode){ }


		/**
		 * Gets the mode
		 *
		 * @param int|null $mode
		 */
		public function getMode(){ }

	}
}
