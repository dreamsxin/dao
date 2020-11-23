<?php 

namespace Dao\Config\Adapter {

	/**
	 * Dao\Config\Adapter\Ini
	 *
	 * Reads ini files and converts them to Dao\Config objects.
	 *
	 * Given the next configuration file:
	 *
	 *<code>
	 *[database]
	 *adapter = Mysql
	 *host = localhost
	 *username = scott
	 *password = cheetah
	 *dbname = test_db
	 *
	 *[phalcon]
	 *controllersDir = "../app/controllers/"
	 *modelsDir = "../app/models/"
	 *viewsDir = "../app/views/"
	 *</code>
	 *
	 * You can read it as follows:
	 *
	 *<code>
	 *	$config = new Dao\Config\Adapter\Ini("path/config.ini");
	 *	echo $config->phalcon->controllersDir;
	 *	echo $config->database->username;
	 *</code>
	 *
	 */
	
	class Ini extends \Dao\Config\Adapter implements \Dao\Config\AdapterInterface, \ArrayAccess, \Countable {

		/**
		 * \Dao\Config\Adapter constructor
		 *
		 * @param string $filePath
		 * @param string $absolutePath
		 * @param string $scannerMode
		 */
		public function __construct($filePath=null, $absolutePath=null, $scannerMode=null){ }


		/**
		 * Load config file
		 *
		 * @param string $filePath
		 */
		public function read($filePath, $absolutePath=null, $scannerMode=null){ }

	}
}
