<?php 

namespace Dao\Config\Adapter {

	/**
	 * Dao\Config\Adapter\Php
	 *
	 * Reads php files and converts them to Dao\Config objects.
	 *
	 * Given the next configuration file:
	 *
	 *<code>
	 *<?php
	 *return array(
	 *	'database' => array(
	 *		'adapter' => 'Mysql',
	 *		'host' => 'localhost',
	 *		'username' => 'scott',
	 *		'password' => 'cheetah',
	 *		'dbname' => 'test_db'
	 *	),
	 *
	 *	'phalcon' => array(
	 *		'controllersDir' => '../app/controllers/',
	 *		'modelsDir' => '../app/models/',
	 *		'viewsDir' => '../app/views/'
	 *));
	 *</code>
	 *
	 * You can read it as follows:
	 *
	 *<code>
	 *	$config = new Dao\Config\Adapter\Php("path/config.php");
	 *	echo $config->phalcon->controllersDir;
	 *	echo $config->database->username;
	 *</code>
	 *
	 */
	
	class Php extends \Dao\Config\Adapter implements \Dao\Config\AdapterInterface, \ArrayAccess, \Countable {

		/**
		 * Load config file
		 *
		 * @param string $filePath
		 */
		public function read($filePath, $absolutePath=null){ }

	}
}
