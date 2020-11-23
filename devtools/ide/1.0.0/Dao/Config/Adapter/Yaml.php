<?php 

namespace Dao\Config\Adapter {

	/**
	 * Dao\Config\Adapter\Yaml
	 *
	 * Reads YAML files and converts them to Dao\Config objects.
	 *
	 * Given the following configuration file:
	 *
	 *<code>
	 *phalcon
	 *  baseuri: /phalcon/
	 *models:
	 *  metadata: memory
	 *</code>
	 *
	 * You can read it as follows:
	 *
	 *<code>
	 *	$config = new Dao\Config\Adapter\Yaml("path/config.yaml");
	 *	echo $config->phalcon->baseuri;
	 *	echo $config->models->metadata;
	 *</code>
	 *
	 */
	
	class Yaml extends \Dao\Config\Adapter implements \Dao\Config\AdapterInterface, \ArrayAccess, \Countable {

		/**
		 * Load config file
		 *
		 * @param string $filePath
		 */
		public function read($filePath, $absolutePath=null){ }

	}
}
