<?php 

namespace Dao\Annotations\Adapter {

	/**
	 * Dao\Annotations\Adapter\Cache
	 *
	 * Stores the parsed annotations in cache. This adapter is suitable for production
	 *
	 *<code>
	 * $annotations = new \Dao\Annotations\Adapter\Cache();
	 *</code>
	 */
	
	class Cache extends \Dao\Annotations\Adapter implements \Dao\Annotations\AdapterInterface {

		protected $_lifetime;

		protected $_cache;

		/**
		 * Constructor for \Dao\Session\Adapter\Cache
		 *
		 * @param array $options
		 */
		public function __construct($options){ }


		/**
		 * Reads parsed annotations from cache
		 *
		 * @param string $key
		 * @return \Dao\Annotations\Reflection
		 */
		public function read($key){ }


		/**
		 * Writes parsed annotations to cache
		 *
		 * @param string $key
		 * @param \Dao\Annotations\Reflection $data
		 */
		public function write($key, $data){ }

	}
}
