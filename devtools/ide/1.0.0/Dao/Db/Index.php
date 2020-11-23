<?php 

namespace Dao\Db {

	/**
	 * Dao\Db\Index
	 *
	 * Allows to define indexes to be used on tables. Indexes are a common way
	 * to enhance database performance. An index allows the database server to find
	 * and retrieve specific rows much faster than it could do without an index
	 */
	
	class Index implements \Dao\Db\IndexInterface {

		protected $_indexName;

		protected $_columns;

		protected $_type;

		/**
		 * \Dao\Db\Index constructor
		 *
		 * @param string $indexName
		 * @param array $columns
		 * @param string|null $type
		 */
		public function __construct($indexName, $columns, $type=null){ }


		/**
		 * Gets the index name
		 *
		 * @return string
		 */
		public function getName(){ }


		/**
		 * Gets the columns that comprends the index
		 *
		 * @return array
		 */
		public function getColumns(){ }


		/**
		 * Gets the index type
		 *
		 * @return string
		 */
		public function getType(){ }


		/**
		 * Restore a \Dao\Db\Index object from export
		 *
		 * @param array $data
		 * @return \Dao\Db\IndexInterface
		 */
		public static function __set_state($properties=null){ }

	}
}
