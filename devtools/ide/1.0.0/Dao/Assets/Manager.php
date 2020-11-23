<?php 

namespace Dao\Assets {

	/**
	 * Dao\Assets\Manager
	 *
	 * Manages collections of CSS/Javascript assets
	 */
	
	class Manager {

		protected $_collections;

		protected $_implicitOutput;

		protected $_sourceBasePath;

		protected $_targetBasePath;

		protected $_targetBaseUri;

		/**
		 * \Dao\Assets\Manager constructor
		 *
		 * @param array $options
		 */
		public function __construct($options=null){ }


		/**
		 * Sets the source base path
		 *
		 * @param string $basePath
		 * @return \Dao\Assets\Manager
		 */
		public function setSourceBasePath($basePath){ }


		/**
		 * Returns the source base path
		 *
		 * @return string
		 */
		public function getSourceBasePath(){ }


		/**
		 * Sets the target base path
		 *
		 * @param string $basePath
		 * @return \Dao\Assets\Manager
		 */
		public function setTargetBasePath($basePath){ }


		/**
		 * Returns the target base path
		 *
		 * @return string
		 */
		public function getTargetBasePath(){ }


		/**
		 * Sets the target base uri
		 *
		 * @param string $baseUri
		 * @return \Dao\Assets\Manager
		 */
		public function setTargetBaseUri($baseUri){ }


		/**
		 * Returns the target base uri
		 *
		 * @return string
		 */
		public function getTargetBaseUri(){ }


		/**
		 * Sets if the HTML generated must be directly printed or returned
		 *
		 * @param boolean $implicitOutput
		 * @return \Dao\Assets\Manager
		 */
		public function useImplicitOutput($implicitOutput){ }


		/**
		 * Adds a Css resource to the 'css' collection
		 *
		 *<code>
		 *	$assets->addCss('css/bootstrap.css');
		 *	$assets->addCss('http://bootstrap.my-cdn.com/style.css', false);
		 *</code>
		 *
		 * @param string $path
		 * @param boolean $local
		 * @param boolean $filter
		 * @param array $attributes
		 * @return \Dao\Assets\Manager
		 */
		public function addCss($path, $local=null, $filter=null, $attributes=null){ }


		/**
		 * Adds a javascript resource to the 'js' collection
		 *
		 *<code>
		 *	$assets->addJs('scripts/jquery.js');
		 *	$assets->addJs('http://jquery.my-cdn.com/jquery.js', true);
		 *</code>
		 *
		 * @param string $path
		 * @param boolean $local
		 * @param boolean $filter
		 * @param array $attributes
		 * @return \Dao\Assets\Manager
		 */
		public function addJs($path, $local=null, $filter=null, $attributes=null){ }


		/**
		 * Adds a resource by its type
		 *
		 *<code>
		 *	$assets->addResourceByType('css', new \Dao\Assets\Resource\Css('css/style.css'));
		 *</code>
		 *
		 * @param string $type
		 * @param \Dao\Assets\Resource $resource
		 * @return \Dao\Assets\Manager
		 */
		public function addResourceByType($type, $resource){ }


		/**
		 * Adds a raw resource to the manager
		 *
		 *<code>
		 * $assets->addResource(new \Dao\Assets\Resource('css', 'css/style.css'));
		 *</code>
		 *
		 * @param \Dao\Assets\Resource $resource
		 * @return \Dao\Assets\Manager
		 */
		public function addResource($resource){ }


		/**
		 * Sets a collection in the Assets Manager
		 *
		 *<code>
		 * $assets->get('js', $collection);
		 *</code>
		 *
		 * @param string $id
		 * @param \Dao\Assets\Collection $collection
		 * @return \Dao\Assets\Manager
		 */
		public function set($id, $collection){ }


		/**
		 * Returns a collection by its id
		 *
		 *<code>
		 * $scripts = $assets->get('js');
		 *</code>
		 *
		 * @param string $id
		 * @return \Dao\Assets\Collection
		 */
		public function get($id){ }


		/**
		 * Returns the CSS collection of assets
		 *
		 * @return \Dao\Assets\Collection
		 */
		public function getCss(){ }


		/**
		 * Returns the CSS collection of assets
		 *
		 * @return \Dao\Assets\Collection
		 */
		public function getJs(){ }


		/**
		 * Creates/Returns a collection of resources
		 *
		 * @param string $name
		 * @return \Dao\Assets\Collection
		 */
		public function collection($name){ }


		/**
		 * Traverses a collection calling the callback to generate its HTML
		 *
		 * @param \Dao\Assets\Collection $collection
		 * @param callback $callback
		 * @param string $type
		 * @param array $args
		 */
		public function output($collection, $callback, $type=null, $args=null){ }


		/**
		 * Prints the HTML for CSS resources
		 *
		 * @param string $collectionName
		 * @param array $args
		 */
		public function outputCss($collectionName=null, $args=null){ }


		/**
		 * Prints the HTML for JS resources
		 *
		 * @param string $collectionName
		 * @param array $args
		 */
		public function outputJs($collectionName=null, $args=null){ }

	}
}
