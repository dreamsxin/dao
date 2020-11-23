<?php 

namespace Dao\Config {

	/**
	 * Dao\Config\Adapter
	 *
	 * Base class for Dao\Config adapters
	 */
	
	abstract class Adapter extends \Dao\Config implements \Countable, \ArrayAccess, \Dao\Config\AdapterInterface {

		protected static $_basePath;

		protected static $_instances;

		/**
		 * \Dao\Config\Adapter constructor
		 *
		 * @param string $filePath
		 * @param string $absolutePath
		 */
		public function __construct($filePath=null, $absolutePath=null){ }


		/**
		 * \Dao\Config\Adapter factory
		 *
		 * @param string $filePath
		 * @param string $absolutePath
		 */
		public static function factory($filePath=null, $absolutePath=null){ }


		/**
		 * Sets base path
		 *
		 * @param string $basePath
		 * @return \Dao\Config\Adapter
		 */
		public static function setBasePath($basePath){ }


		/**
		 * Gets base path
		 *
		 * @return string
		 */
		public static function getBasePath(){ }


		/**
		 * Load a configuration
		 *
		 * @param string $filePath
		 * @param string $absolutePath
		 */
		public function load($filePath, $absolutePath=null, $recursive=null){ }


		abstract public function read($filePath, $absolutePath=null);

	}
}
