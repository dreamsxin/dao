<?php 

namespace Dao\Cache {

	/**
	 * Dao\Cache\Multiple
	 *
	 * Allows to read to chained backends writing to multiple backends
	 *
	 *<code>
	 *   use Dao\Cache\Frontend\Data as DataFrontend,
	 *       Dao\Cache\Multiple,
	 *       Dao\Cache\Backend\Apc as ApcCache,
	 *       Dao\Cache\Backend\Memcache as MemcacheCache,
	 *       Dao\Cache\Backend\File as FileCache;
	 *
	 *   $ultraFastFrontend = new DataFrontend(array(
	 *       "lifetime" => 3600
	 *   ));
	 *
	 *   $fastFrontend = new DataFrontend(array(
	 *       "lifetime" => 86400
	 *   ));
	 *
	 *   $slowFrontend = new DataFrontend(array(
	 *       "lifetime" => 604800
	 *   ));
	 *
	 *   //Backends are registered from the fastest to the slower
	 *   $cache = new Multiple(array(
	 *       new ApcCache($ultraFastFrontend, array(
	 *           "prefix" => 'cache',
	 *       )),
	 *       new MemcacheCache($fastFrontend, array(
	 *           "prefix" => 'cache',
	 *           "host" => "localhost",
	 *           "port" => "11211"
	 *       )),
	 *       new FileCache($slowFrontend, array(
	 *           "prefix" => 'cache',
	 *           "cacheDir" => "../app/cache/"
	 *       ))
	 *   ));
	 *
	 *   //Save, saves in every backend
	 *   $cache->save('my-key', $data);
	 *</code>
	 */
	
	class Multiple {

		protected $_backends;

		/**
		 * \Dao\Cache\Multiple constructor
		 *
		 * @param \Dao\Cache\BackendInterface[] $backends
		 */
		public function __construct($backends=null){ }


		/**
		 * Adds a backend
		 *
		 * @param \Dao\Cache\BackendInterface $backend
		 * @return \Dao\Cache\Multiple
		 */
		public function push($backend){ }


		/**
		 * Returns a cached content reading the internal backends
		 *
		 * @param 	string $keyName
		 * @param   long $lifetime
		 * @return  mixed
		 */
		public function get($keyName, $lifetime=null){ }


		/**
		 * Starts every backend
		 *
		 * @param int|string $keyName
		 * @param   long $lifetime
		 * @return  mixed
		 */
		public function start($keyName, $lifetime=null){ }


		/**
		 * Stores cached content into all backends and stops the frontend
		 *
		 * @param string $keyName
		 * @param string $content
		 * @param long $lifetime
		 * @param boolean $stopBuffer
		 */
		public function save($keyName=null, $content=null, $lifetime=null, $stopBuffer=null){ }


		/**
		 * Deletes a value from each backend
		 *
		 * @param int|string $keyName
		 * @return boolean
		 */
		public function delete($keyName){ }


		/**
		 * Checks if cache exists in at least one backend
		 *
		 * @param  string $keyName
		 * @param  long $lifetime
		 * @return boolean
		 */
		public function exists($keyName=null, $lifetime=null){ }

	}
}
