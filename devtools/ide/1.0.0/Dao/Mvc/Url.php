<?php 

namespace Dao\Mvc {

	/**
	 * Dao\Mvc\Url
	 *
	 * This components aids in the generation of: URIs, URLs and Paths
	 *
	 *<code>
	 *
	 * //Generate a URL appending the URI to the base URI
	 * echo $url->get('products/edit/1');
	 *
	 * //Generate a URL for a predefined route
	 * echo $url->get(array('for' => 'blog-post', 'title' => 'some-cool-stuff', 'year' => '2012'));
	 *
	 *</code>
	 */
	
	class Url extends \Dao\Di\Injectable implements \Dao\Events\EventsAwareInterface, \Dao\Di\InjectionAwareInterface, \Dao\Mvc\UrlInterface {

		protected $_baseUri;

		protected $_staticBaseUri;

		protected $_basePath;

		protected $_router;

		/**
		 * Sets a prefix for all the URIs to be generated
		 *
		 *<code>
		 *	$url->setBaseUri('/invo/');
		 *	$url->setBaseUri('/invo/index.php/');
		 *</code>
		 *
		 * @param string $baseUri
		 * @return \Dao\Mvc\Url
		 */
		public function setBaseUri($baseUri){ }


		/**
		 * Sets a prefix for all static URLs generated
		 *
		 *<code>
		 *	$url->setStaticBaseUri('/invo/');
		 *</code>
		 *
		 * @param string $staticBaseUri
		 * @return \Dao\Mvc\Url
		 */
		public function setStaticBaseUri($staticBaseUri){ }


		/**
		 * Returns the prefix for all the generated urls. By default /
		 *
		 * @return string
		 */
		public function getBaseUri(){ }


		/**
		 * Returns the prefix for all the generated static urls. By default /
		 *
		 * @return string
		 */
		public function getStaticBaseUri(){ }


		/**
		 * Sets a base path for all the generated paths
		 *
		 *<code>
		 *	$url->setBasePath('/var/www/htdocs/');
		 *</code>
		 *
		 * @param string $basePath
		 * @return \Dao\Mvc\Url
		 */
		public function setBasePath($basePath){ }


		/**
		 * Returns the base path
		 *
		 * @return string
		 */
		public function getBasePath(){ }


		/**
		 * Generates a URL
		 *
		 *<code>
		 *
		 * //Generate a URL appending the URI to the base URI
		 * echo $url->get('products/edit/1');
		 *
		 * //Generate a URL for a predefined route
		 * echo $url->get(array('for' => 'blog-post', 'title' => 'some-cool-stuff', 'year' => '2012'));
		 * echo $url->get(array('for' => 'blog-post', 'hostname' => true, 'title' => 'some-cool-stuff', 'year' => '2012'));
		 *
		 *</code>
		 *
		 * @param string|array $uri
		 * @param array|object args Optional arguments to be appended to the query string
		 * @param bool|null $local
		 * @return string
		 */
		public function get($uri=null, $args=null, $local=null){ }


		/**
		 * Generates a URL for a static resource
		 *
		 * @param string|array $uri
		 * @param array $args
		 * @return string
		 */
		public function getStatic($uri=null){ }


		/**
		 * Generates a local path
		 *
		 * @param string $path
		 * @return string
		 */
		public function path($path=null){ }


		/**
		 * Check whether the URI is local
		 *
		 * @param string $uri
		 * @return boolean
		 */
		public function isLocal($uri){ }

	}
}
