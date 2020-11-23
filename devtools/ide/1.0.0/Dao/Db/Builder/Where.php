<?php 

namespace Dao\Db\Builder {

	/**
	 * Dao\Db\Builder\Where
	 *
	 * Helps to create PHQL queries for WHERE statements
	 */
	
	abstract class Where extends \Dao\Db\Builder implements \Dao\Db\BuilderInterface, \Dao\Di\InjectionAwareInterface, \Dao\Events\EventsAwareInterface {

		protected $_conditions;

		/**
		 * Gets the type of PHQL queries
		 *
		 * @param string|array $conditions
		 * @param array $bindParams
		 * @param array $bindTypes
		 * @param boolean $type
		 * @param boolean $grouping
		 * @return int
		 */
		public function setConditions($conditions, $bindParams=null, $bindTypes=null, $type=null, $grouping=null){ }


		/**
		 * Return the conditions
		 *
		 * @return string
		 */
		public function getConditions(){ }


		/**
		 * Sets the query conditions
		 *
		 *<code>
		 *	$builder->where('name = "Peter"');
		 *	$builder->where('name = :name AND id > :id', array('name' => 'Peter', 'id' => 100));
		 *</code>
		 *
		 * @param string $conditions
		 * @param array $bindParams
		 * @param array $bindTypes
		 * @param boolean $grouping
		 * @return \Dao\Db\Builder
		 */
		public function where($conditions, $bindParams=null, $bindTypes=null){ }


		/**
		 * Appends a condition to the current conditions using a AND operator
		 *
		 *<code>
		 *	$builder->andWhere('name = "Peter"');
		 *	$builder->andWhere('name = :name AND id > :id', array('name' => 'Peter', 'id' => 100));
		 *</code>
		 *
		 * @param string $conditions
		 * @param array $bindParams
		 * @param array $bindTypes
		 * @param boolean $grouping
		 * @return \Dao\Db\Builder
		 */
		public function andWhere($conditions, $bindParams=null, $bindTypes=null){ }


		/**
		 * Appends a condition to the current conditions using a OR operator
		 *
		 * @param string $conditions
		 * @param array $bindParams
		 * @param array $bindTypes
		 * @return \Dao\Db\Builder
		 */
		public function orWhere($conditions, $bindParams=null, $bindTypes=null){ }


		/**
		 * Appends an IN condition to the current conditions
		 *
		 * @param string $expr
		 * @param array $values
		 * @param boolean $useOrWhere
		 * @return \Dao\Db\Builder
		 */
		public function inWhere($expr, $values, $useOrWhere=null){ }


		/**
		 * Appends a NOT IN condition to the current conditions
		 *
		 * @param string $expr
		 * @param array $values
		 * @param boolean $useOrWhere
		 * @return \Dao\Db\Builder
		 */
		public function notInWhere($expr, $values, $useOrWhere=null){ }

	}
}
