<?php 

namespace Dao\Db\Builder {

	/**
	 * Dao\Db\Builder\Join
	 */
	
	abstract class Join extends \Dao\Db\Builder\Where implements \Dao\Events\EventsAwareInterface, \Dao\Di\InjectionAwareInterface, \Dao\Db\BuilderInterface {

		const LEFT = LEFT;

		const RIGHT = RIGHT;

		const INNER = INNER;

		protected $_joins;

		/**
		 * Adds a join to the query
		 *
		 * @param string $table
		 * @param string $conditions
		 * @param string $type
		 * @return \Dao\Db\Builder\Join
		 */
		public function join($table, $conditions=null, $type=null){ }


		/**
		 * Adds a INNER join to the query
		 *
		 * @param string $table
		 * @param string $conditions
		 * @return \Dao\Db\Builder\Join
		 */
		public function innerJoin($table, $conditions=null){ }


		/**
		 * Adds a LEFT join to the query
		 *
		 * @param string $table
		 * @param string $conditions
		 * @return \Dao\Db\Builder\Join
		 */
		public function leftJoin($table, $conditions=null){ }


		/**
		 * Adds a RIGHT join to the query
		 *
		 * @param string $table
		 * @param string $conditions
		 * @return \Dao\Db\Builder\Join
		 */
		public function rightJoin($table, $conditions=null){ }

	}
}
