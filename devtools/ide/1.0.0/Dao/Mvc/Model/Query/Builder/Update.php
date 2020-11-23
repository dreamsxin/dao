<?php 

namespace Dao\Mvc\Model\Query\Builder {

	/**
	 * Dao\Mvc\Model\Query\Builder\Update
	 *
	 *<code>
	 *$resultset = Dao\Mvc\Model\Query\Builder::createUpdateBuilder()
	 *   ->table('Robots')
	 *   ->set(array('name' => 'Google'))
	 *   ->getQuery()
	 *   ->execute();
	 *</code>
	 */
	
	class Update extends \Dao\Mvc\Model\Query\Builder\Where implements \Dao\Events\EventsAwareInterface, \Dao\Di\InjectionAwareInterface, \Dao\Mvc\Model\Query\BuilderInterface {

		protected $_type;

		protected $_table;

		protected $_set;

		protected $_conflict;

		/**
		 * \Dao\Mvc\Model\Query\Builder\Update constructor
		 *
		 * @param array $params
		 * @param \Dao\Di $dependencyInjector
		 */
		public function __construct($params=null){ }


		/**
		 * Sets the table to update
		 *
		 * @param string table
		 * @return \Dao\Mvc\Model\Query\Builder\Update
		 */
		public function table($table){ }


		/**
		 * Gets the table to update
		 *
		 * @return string
		 */
		public function getTable(){ }


		/**
		 * Sets the values to update with an associative array
		 *
		 *<code>
		 *	$builder->set(array('id' => 1, 'name' => 'Google'));
		 *</code>
		 *
		 * @param array $set
		 * @return \Dao\Mvc\Model\Query\Builder\Update
		 */
		public function set($set){ }


		/**
		 * Return the values to update with an associative array
		 *
		 * @return string|array
		 */
		public function getSet(){ }


		/**
		 * Sets conflict
		 *
		 * @param array $conflict
		 * @return \Dao\Mvc\Model\Query\Builder\Update
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
