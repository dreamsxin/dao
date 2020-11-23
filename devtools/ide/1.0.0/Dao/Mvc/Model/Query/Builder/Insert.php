<?php 

namespace Dao\Mvc\Model\Query\Builder {

	/**
	 * Dao\Mvc\Model\Query\Builder\Insert
	 *
	 *<code>
	 *$resultset = Dao\Mvc\Model\Query\Builder::createInsertBuilder()
	 *   ->table('Robots')
	 *   ->columns(array('name'))
	 *   ->values(array(array('name' => 'Google'), array('name' => 'Baidu')))
	 *   ->getQuery()
	 *   ->execute();
	 *</code>
	 */
	
	class Insert extends \Dao\Mvc\Model\Query\Builder implements \Dao\Mvc\Model\Query\BuilderInterface, \Dao\Di\InjectionAwareInterface, \Dao\Events\EventsAwareInterface {

		protected $_type;

		protected $_table;

		protected $_columns;

		protected $_flipColumns;

		protected $_values;

		protected $_useColumnName;

		protected $_conflict;

		/**
		 * \Dao\Mvc\Model\Query\Builder\Insert constructor
		 *
		 * @param array $params
		 * @param \Dao\Di $dependencyInjector
		 */
		public function __construct($params=null){ }


		/**
		 * Sets the table to insert into
		 *
		 * @param string table
		 * @return \Dao\Mvc\Model\Query\Builder\Insert
		 */
		public function table($table){ }


		/**
		 * Gets the table to insert into
		 *
		 * @return boolean
		 */
		public function getTable(){ }


		/**
		 * Set the columns that will be inserted
		 *
		 * @param array $columns
		 * @return \Dao\Mvc\Model\Query\Builder\Insert
		 */
		public function columns($columns){ }


		/**
		 * Gets the columns that will be inserted
		 *
		 * @return string|array
		 */
		public function getColumns(){ }


		/**
		 * Sets the values to insert
		 *
		 * @param array $values
		 * @return \Dao\Mvc\Model\Query\Builder\Insert
		 */
		public function values($values){ }


		/**
		 * Gets the values to insert
		 *
		 * @return array
		 */
		public function getValues(){ }


		/**
		 * Sets conflict
		 *
		 * @param array $conflict
		 * @return \Dao\Mvc\Model\Query\Builder\Insert
		 */
		public function setConflict($conflict){ }


		/**
		 * Returns a PHQL statement built based on the builder parameters
		 *
		 * @return string
		 */
		protected function _compile(){ }

	}
}
