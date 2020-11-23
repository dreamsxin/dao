<?php 

namespace Dao\Db\Adapter\Pdo {

	/**
	 * Dao\Db\Adapter\Pdo\Sqlite
	 *
	 * Specific functions for the Sqlite database system
	 * <code>
	 *
	 * $config = array(
	 *  "dbname" => "/tmp/test.sqlite"
	 * );
	 *
	 * $connection = new Dao\Db\Adapter\Pdo\Sqlite($config);
	 *
	 * </code>
	 */
	
	class Sqlite extends \Dao\Db\Adapter\Pdo implements \Dao\Events\EventsAwareInterface, \Dao\Di\InjectionAwareInterface, \Dao\Db\AdapterInterface {

		protected $_type;

		protected $_dialectType;

		/**
		 * This method is automatically called in \Dao\Db\Adapter\Pdo constructor.
		 * Call it when you need to restore a database connection.
		 *
		 * @param array $descriptor
		 * @return boolean
		 */
		public function connect($descriptor=null){ }


		/**
		 * Returns an array of \Dao\Db\Column objects describing a table
		 *
		 * <code>
		 * print_r($connection->describeColumns("posts")); ?>
		 * </code>
		 *
		 * @param string $table
		 * @param string $schema
		 * @return \Dao\Db\Column[]
		 */
		public function describeColumns($table, $schema=null){ }


		/**
		 * Lists table indexes
		 *
		 * @param string $table
		 * @param string $schema
		 * @return \Dao\Db\Index[]
		 */
		public function describeIndexes($table, $schema=null){ }


		/**
		 * Lists table references
		 *
		 * @param string $table
		 * @param string $schema
		 * @return \Dao\Db\Reference[]
		 */
		public function describeReferences($table, $schema=null){ }


		/**
		 * Check whether the database system requires an explicit value for identity columns
		 *
		 * @return boolean
		 */
		public function useExplicitIdValue(){ }


		/**
		 * Convert php bytea to database bytea
		 *
		 * @param string $value
		 * @return string
		 * @return string
		 */
		public function escapeBytea($value){ }


		/**
		 * Convert database bytea to php bytea
		 *
		 * @param string $value
		 * @return string
		 */
		public function unescapeBytea($value){ }


		/**
		 * Convert php array to database array
		 *
		 * @param array $value
		 * @return string
		 */
		public function escapeArray($value, $type=null){ }


		/**
		 * Convert database array to php array
		 *
		 * @param string $value
		 * @return array
		 */
		public function unescapeArray($value, $type=null){ }

	}
}
