<?php 

namespace Dao\Db\Builder {

	/**
	 * Dao\Db\Builder\Select
	 */
	
	class Select extends \Dao\Db\Builder\Join implements \Dao\Db\BuilderInterface, \Dao\Di\InjectionAwareInterface, \Dao\Events\EventsAwareInterface {

		const LEFT = LEFT;

		const RIGHT = RIGHT;

		const INNER = INNER;

		/**
		 * \Dao\Db\Builder\Select constructor
		 *
		 * @param string|array $tables
		 * @param string $db
		 */
		public function __construct($tables, $db=null){ }


		/**
		 * Sets SELECT DISTINCT / SELECT ALL flag
		 *
		 * @param bool|null distinct
		 * @return \Dao\Db\Builder\Select
		 */
		public function distinct($distinct){ }


		/**
		 * Sets the columns to be queried
		 *
		 * @param string|array $columns
		 * @return \Dao\Db\Builder\Select
		 */
		public function columns($columns){ }


		/**
		 * Sets a ORDER BY condition clause
		 *
		 * @param string|array $orderBy
		 * @return \Dao\Db\Builder\Select
		 */
		public function orderBy($orderBy){ }


		/**
		 * Sets a HAVING condition clause. You need to escape PHQL reserved words using [ and ] delimiters
		 *
		 *<code>
		 *	$builder->having('SUM(Robots.price) > 0');
		 *</code>
		 *
		 * @param string $having
		 * @return \Dao\Db\Builder\Select
		 */
		public function having($having){ }


		/**
		 * Sets a LIMIT clause, optionally a offset clause
		 *
		 * @param int $limit
		 * @param int $offset
		 * @return \Dao\Db\Builder\Select
		 */
		public function limit($limit, $offset=null){ }


		/**
		 * Sets an OFFSET clause
		 *
		 * @param int $offset
		 * @return \Dao\Db\Builder\Select
		 */
		public function offset($offset){ }


		/**
		 * Sets a GROUP BY clause
		 *
		 * @param string|array $group
		 * @return \Dao\Db\Builder\Select
		 */
		public function groupBy($group){ }


		/**
		 * Returns a PHQL statement built based on the builder parameters
		 *
		 * @return \Dao\Db\ResultInterface
		 */
		protected function _execute(){ }


		/**
		 * Returns a PHQL statement built based on the builder parameters
		 *
		 * @return \Dao\Db\ResultInterface
		 */
		public function count(){ }

	}
}
