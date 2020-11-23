<?php 

namespace Dao\Cache\Backend {

	/**
	 * Dao\Cache\Backend\Yac
	 *
	 * Allows to cache output fragments, PHP data or raw data to a yac backend
	 *
	 * This adapter uses the special yacd key "_PHCY" to store all the keys internally used by the adapter
	 *
	 *<code>
	 *
	 * // Cache data for 2 days
	 * $frontCache = new Dao\Cache\Frontend\Data(array(
	 *    "lifetime" => 172800
	 * ));
	 *
	 * //Create the Cache setting yacd connection options
	 * $cache = new Dao\Cache\Backend\Yac($frontCache, array(
	 *		'prefix' => "myproduct_"
	 * ));
	 *
	 * //Cache arbitrary data
	 * $cache->save('my-data', array(1, 2, 3, 4, 5));
	 *
	 * //Get data
	 * $data = $cache->get('my-data');
	 *
	 *</code>
	 */
	
	class Yac extends \Dao\Cache\Backend implements \Dao\Cache\BackendInterface, \Dao\Di\InjectionAwareInterface, \Dao\Events\EventsAwareInterface {

		protected $_yac;

		/**
		 * \Dao\Cache\Backend\Yac constructor
		 *
		 * @param \Dao\Cache\FrontendInterface $frontend
		 * @param array $options
		 */
		public function __construct($frontend, $options=null){ }


		/**
		 * Create internal connection to yacd
		 */
		protected function _connect(){ }


		/**
		 * Returns a cached content
		 *
		 * @param int|string $keyName
		 * @return mixed
		 */
		public function get($keyName, $lifetime=null){ }


		/**
		 * Stores cached content into the Yacd backend and stops the frontend
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
		 * @param  string $keyName
		 * @param  long $value
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


		public function getTrackingKey(){ }


		public function setTrackingKey($key){ }

	}
}
