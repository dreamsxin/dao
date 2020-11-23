<?php 

namespace Dao\Mvc\Model\Query\Builder {

	/**
	 * Dao\Mvc\Model\Query\Builder\Delete
	 *
	 *<code>
	 *$resultset = Dao\Mvc\Model\Query\Builder::createDeleteBuilder()
	 *   ->table('Robots')
	 *   ->where('name = "Peter"')
	 *   ->orderBy('Robots.id')
	 *   ->limit(20)
	 *   ->execute();
	 *</code>
	 */
	
	class Delete extends \Dao\Mvc\Model\Query\Builder\Where implements \Dao\Events\EventsAwareInterface, \Dao\Di\InjectionAwareInterface, \Dao\Mvc\Model\Query\BuilderInterface {

		protected $_type;

		protected $_table;

		protected $_columns;

		protected $_values;

		/**
		 * \Dao\Mvc\Model\Query\Builder\Delete constructor
		 *
		 * @param array $params
		 * @param \Dao\Di $dependencyInjector
		 */
		public function __construct($params=null){ }


		/**
		 * Sets the table to delete from
		 *
		 * @param string table
		 * @return \Dao\Mvc\Model\Query\Builder\Delete
		 */
		public function table($table){ }


		/**
		 * Gets the table to delete from
		 *
		 * @return boolean
		 */
		public function getTable(){ }


		/**
		 * Returns a PHQL statement built based on the builder parameters
		 *
		 * @return string
		 */
		protected function _compile(){ }

	}
}
