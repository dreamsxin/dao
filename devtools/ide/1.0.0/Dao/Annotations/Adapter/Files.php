<?php 

namespace Dao\Annotations\Adapter {

	/**
	 * Dao\Annotations\Adapter\Files
	 *
	 * Stores the parsed annotations in files. This adapter is suitable for production
	 *
	 *<code>
	 * $annotations = new \Dao\Annotations\Adapter\Files(array(
	 *    'annotationsDir' => 'app/cache/annotations/'
	 * ));
	 *</code>
	 */
	
	class Files extends \Dao\Annotations\Adapter implements \Dao\Annotations\AdapterInterface {

		protected $_annotationsDir;

		/**
		 * \Dao\Annotations\Adapter\Files constructor
		 *
		 * @param array $options
		 */
		public function __construct($options=null){ }


		/**
		 * Reads parsed annotations from files
		 *
		 * @param string $key
		 * @return \Dao\Annotations\Reflection
		 */
		public function read($key){ }


		/**
		 * Writes parsed annotations to files
		 *
		 * @param string $key
		 * @param \Dao\Annotations\Reflection $data
		 */
		public function write($key, $data){ }

	}
}
