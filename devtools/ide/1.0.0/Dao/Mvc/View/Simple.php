<?php 

namespace Dao\Mvc\View {

	/**
	 * Dao\Mvc\View\Simple
	 *
	 * This component allows to render views without hicherquical levels
	 *
	 *<code>
	 * $view = new Dao\Mvc\View\Simple();
	 * echo $view->render('templates/my-view', array('content' => $html));
	 *</code>
	 *
	 */
	
	class Simple extends \Dao\Di\Injectable implements \Dao\Events\EventsAwareInterface, \Dao\Di\InjectionAwareInterface {

		protected $_options;

		protected $_viewsDir;

		protected $_partialsDir;

		protected $_viewParams;

		protected $_engines;

		protected $_registeredEngines;

		protected $_activeRenderPath;

		protected $_content;

		protected $_sections;

		protected $_cache;

		protected $_cacheOptions;

		/**
		 * \Dao\Mvc\View constructor
		 *
		 * @param array $options
		 */
		public function __construct($options=null){ }


		/**
		 * Sets views directory. Depending of your platform, always add a trailing slash or backslash
		 *
		 * @param string|array $viewsDir
		 */
		public function setViewsDir($viewsDir){ }


		/**
		 * Gets views directory
		 *
		 * @return string
		 */
		public function getViewsDir(){ }


		/**
		 * Register templating engines
		 *
		 *<code>
		 *$this->view->registerEngines(array(
		 *  ".phtml" => "Dao\Mvc\View\Engine\Php",
		 *  ".mhtml" => "MyCustomEngine"
		 *));
		 *</code>
		 *
		 * @param array $engines
		 */
		public function registerEngines($engines){ }


		/**
		 * Returns the registered templating engines
		 *
		 * @brief array \Dao\Mvc\View\Simple::getRegisteredEngines()
		 */
		public function getRegisteredEngines(){ }


		/**
		 * Loads registered template engines, if none is registered it will use \Dao\Mvc\View\Engine\Php
		 *
		 * @return array
		 */
		protected function _loadTemplateEngines(){ }


		/**
		 * Tries to render the view with every engine registered in the component
		 *
		 * @param string $path
		 * @param array $params
		 */
		protected function _internalRender(){ }


		/**
		 * Renders a view
		 *
		 * @param string $path
		 * @param array $params
		 * @return string
		 */
		public function render($path, $params=null, $absolutePath=null){ }


		/**
		 * Renders a partial view
		 *
		 * <code>
		 * 	//Show a partial inside another view
		 * 	$this->partial('shared/footer');
		 * </code>
		 *
		 * <code>
		 * 	//Show a partial inside another view with parameters
		 * 	$this->partial('shared/footer', array('content' => $html));
		 * </code>
		 *
		 * @param string $partialPath
		 * @param array $params
		 */
		public function partial($partialPath, $params=null, $absolutePath=null){ }


		/**
		 * Sets the cache options
		 *
		 * @param array $options
		 * @return \Dao\Mvc\View\Simple
		 */
		public function setCacheOptions($options){ }


		/**
		 * Returns the cache options
		 *
		 * @return array
		 */
		public function getCacheOptions(){ }


		/**
		 * Create a \Dao\Cache based on the internal cache options
		 *
		 * @return \Dao\Cache\BackendInterface
		 */
		protected function _createCache(){ }


		/**
		 * Returns the cache instance used to cache
		 *
		 * @return \Dao\Cache\BackendInterface
		 */
		public function getCache(){ }


		/**
		 * Cache the actual view render to certain level
		 *
		 *<code>
		 *  $this->view->cache(array('key' => 'my-key', 'lifetime' => 86400));
		 *</code>
		 *
		 * @param boolean|array $options
		 * @return \Dao\Mvc\View\Simple
		 */
		public function cache($options=null){ }


		/**
		 * Adds parameters to views (alias of setVar)
		 *
		 *<code>
		 *	$this->view->setParamToView('products', $products);
		 *</code>
		 *
		 * @param string $key
		 * @param mixed $value
		 * @return \Dao\Mvc\View\Simple
		 */
		public function setParamToView($key, $value){ }


		/**
		 * Set all the render params
		 *
		 *<code>
		 *	$this->view->setVars(array('products' => $products));
		 *</code>
		 *
		 * @param array $params
		 * @param boolean $merge
		 * @return \Dao\Mvc\View\Simple
		 */
		public function setVars($params, $merge=null){ }


		/**
		 * Set a single view parameter
		 *
		 *<code>
		 *	$this->view->setVar('products', $products);
		 *</code>
		 *
		 * @param string $key
		 * @param mixed $value
		 * @return \Dao\Mvc\View\Simple
		 */
		public function setVar($key, $value){ }


		public function getVar($key){ }


		/**
		 * Returns parameters to views
		 *
		 * @return array
		 */
		public function getParamsToView(){ }


		/**
		 * Externally sets the view content
		 *
		 *<code>
		 *	$this->view->setContent("<h1>hello</h1>");
		 *</code>
		 *
		 * @param string $content
		 * @return \Dao\Mvc\View\Simple
		 */
		public function setContent($content){ }


		/**
		 * Returns cached ouput from another view stage
		 *
		 * @return string
		 */
		public function getContent(){ }


		/**
		 * Start a new section block
		 *
		 * @param string $name
		 */
		public function startSection($name){ }


		/**
		 * Stop the current section block
		 *
		 * @return string
		 */
		public function stopSection(){ }


		/**
		 * Returns the content for a section block
		 *
		 * @param string $name
		 * @param string $default
		 * @return string|null
		 */
		public function section($name, $defaultValue=null){ }


		/**
		 * Returns the path of the view that is currently rendered
		 *
		 * @return string
		 */
		public function getActiveRenderPath(){ }


		/**
		 * Magic method to pass variables to the views
		 *
		 *<code>
		 *	$this->view->products = $products;
		 *</code>
		 *
		 * @param string $key
		 * @param mixed $value
		 */
		public function __set($key, $value){ }


		/**
		 * Magic method to retrieve a variable passed to the view
		 *
		 *<code>
		 *	echo $this->view->products;
		 *</code>
		 *
		 * @param string $key
		 * @return mixed
		 */
		public function __get($key){ }

	}
}
