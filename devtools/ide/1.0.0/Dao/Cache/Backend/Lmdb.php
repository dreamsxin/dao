<?php 

namespace Dao\Cache\Backend {

	/**
	 * Dao\Cache\Backend\Lmdb
	 *
	 * Allows to cache output fragments, PHP data or raw data to a lmdb backend
	 *
	 * This adapter uses the special lmdbd key "_PHCY" to store all the keys internally used by the adapter
	 *
	 *<code>
	 *
	 * // Cache data for 2 days
	 * $frontCache = new Dao\Cache\Frontend\Data(array(
	 *    "lifetime" => 172800
	 * ));
	 *
	 * // Create the Cache setting lmdb connection options
	 * $cache = new Dao\Cache\Backend\Lmdb($frontCache, array(
	 * 	'path' => __DIR__.'/lmdb',
	 *	'name' => 'dao_test'
	 *));
	 *
	 * // Or
	 * $cache = new Dao\Cache\Backend\Lmdb($frontCache, array(
	 *		'lmdb' => new Dao\Storage\Lmdb(__DIR__.'/lmdb')
	 * ));
	 *
	 * // Cache arbitrary data
	 * $cache->save('my-data', array(1, 2, 3, 4, 5));
	 *
	 * // Get data
	 * $data = $cache->get('my-data');
	 *
	 *</code>
	 */
	
	class Lmdb extends \Dao\Cache\Backend implements \Dao\Cache\BackendInterface, \Dao\Di\InjectionAwareInterface, \Dao\Events\EventsAwareInterface {

		protected $_config;

		protected $_lmdb;

		/**
		 * \Dao\Cache\Backend\Lmdb constructor
		 *
		 * @param \Dao\Cache\FrontendInterface $frontend
		 * @param array $options
		 */
		public function __construct($frontend, $options=null){ }


		/**
		 * Returns a cached content
		 *
		 * @param int|string $keyName
		 * @param int $lifetime
		 * @return mixed
		 */
		public function get($keyName, $lifetime=null){ }


		/**
		 * Stores cached content into the Lmdbd backend and stops the frontend
		 *
		 * @param int|string $keyName
		 * @param string $content
		 * @param long $lifetime
		 * @param boolean $stopBuffer
		 */
		public function save($keyName=null, $value=null, $lifetime=null, $stopBuffer=null){ }


		/**
		 * Deletes a value from the cache by its key
		 *
		 * @param int|string $keyName
		 * @return boolean
		 */
		public function delete($keyName){ }


		/**
		 * Query the existing cached keys
		 *
		 * @return array
		 */
		public function queryKeys($prefix=null){ }


		/**
		 * Checks if cache exists and it hasn't expired
		 *
		 * @param  string $keyName
		 * @param  long $lifetime
		 * @return boolean
		 */
		public function exists($keyName){ }


		/**
		 * Atomic increment of a given key, by number $value
		 *
		 * @param string $keyName
		 * @param long $value
		 * @return mixed
		 */
		public function increment($keyName, $value=null){ }


		/**
		 * Atomic decrement of a given key, by number $value
		 *
		 * @param  string $keyName
		 * @param  long $value
		 * @return mixed
		 */
		public function decrement($keyName, $value=null){ }


		/**
		 * Immediately invalidates all existing items.
		 *
		 * @return boolean
		 */
		public function flush(){ }

	}
}
