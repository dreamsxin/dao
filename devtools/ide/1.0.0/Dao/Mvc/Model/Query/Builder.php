<?php 

namespace Dao\Mvc\Model\Query {

	/**
	 * Dao\Mvc\Model\Query\Builder
	 *
	 * Helps to create PHQL queries using an OO interface
	 *
	 *<code>
	 *$resultset = Dao\Mvc\Model\Query\Builder::create(Dao\Mvc\Model\Query::TYPE_SELECT)
	 *   ->from('Robots')
	 *   ->join('RobotsParts')
	 *   ->limit(20)
	 *   ->orderBy('Robots.name')
	 *   ->getQuery()
	 *   ->execute();
	 *</code>
	 */
	
	abstract class Builder extends \Dao\Di\Injectable implements \Dao\Events\EventsAwareInterface, \Dao\Di\InjectionAwareInterface, \Dao\Mvc\Model\Query\BuilderInterface {

		protected $_type;

		protected $_bindParams;

		protected $_bindTypes;

		protected $_mergeBindParams;

		protected $_mergeBindTypes;

		protected $_phql;

		protected $_hiddenParamNumber;

		protected $_index;

		/**
		 * Create a new Query Builder of the given type.
		 *
		 *<code>
		 *	Dao\Mvc\Model\Query\Builder::create(Dao\Mvc\Model\Query::TYPE_SELECT);
		 *</code>
		 *
		 * @return \Dao\Mvc\Model\Query\Builder
		 */
		public static function create($type){ }


		/**
		 * Create a new Query Builder for Select
		 *
		 *
		 * @return \Dao\Mvc\Model\Query\Builder\Select
		 */
		public static function createSelectBuilder($params=null, \Dao\DiInterface $dependencyInjector=null){ }


		/**
		 * Create a new Query Builder for Insert
		 *
		 *
		 * @return \Dao\Mvc\Model\Query\Builder\Insert
		 */
		public static function createInsertBuilder($params=null, \Dao\DiInterface $dependencyInjector=null){ }


		/**
		 * Create a new Query Builder for Update
		 *
		 *
		 * @return \Dao\Mvc\Model\Query\Builder\Update
		 */
		public static function createUpdateBuilder($params=null, \Dao\DiInterface $dependencyInjector=null){ }


		/**
		 * Create a new Query Builder for Delete
		 *
		 *
		 * @return \Dao\Mvc\Model\Query\Builder\Delete
		 */
		public static function createDeleteBuilder($params=null, \Dao\DiInterface $dependencyInjector=null){ }


		/**
		 * Gets the type of PHQL queries
		 *
		 *
		 * @return int
		 */
		public function getType(){ }


		/**
		 * Sets the bind parameters
		 *
		 * @param array $bindParams
		 * @return \Dao\Mvc\Model\Query\Builder
		 */
		public function setBindParams($bindparams, $merge=null){ }


		/**
		 * Gets the bind parameters
		 *
		 * @return \Dao\Mvc\Model\Query\Builder
		 */
		public function getBindParams(){ }


		/**
		 * Gets the merge bind parameters
		 *
		 * @return \Dao\Mvc\Model\Query\Builder
		 */
		public function getMergeBindParams(){ }


		/**
		 * Sets the bind types
		 *
		 * @param array $bindTypes
		 * @return \Dao\Mvc\Model\Query\Builder
		 */
		public function setBindTypes($bindtypes, $merge=null){ }


		/**
		 * Gets the bind types
		 *
		 * @return \Dao\Mvc\Model\Query\Builder
		 */
		public function getBindTypes(){ }


		/**
		 * Gets the merge bind types
		 *
		 * @return \Dao\Mvc\Model\Query\Builder
		 */
		public function getMergeBindTypes(){ }


		/**
		 * Adds the index
		 *
		 * @param string $index
		 * @return \Dao\Mvc\Model\Query\Builder
		 */
		public function setIndex($index){ }


		/**
		 * Gets the index
		 *
		 * @return string
		 */
		public function getIndex(){ }


		/**
		 * Compile the PHQL query
		 *
		 * @return \Dao\Mvc\Model\Query\Builder
		 */
		public function compile(){ }


		/**
		 * Returns a PHQL statement built based on the builder parameters
		 *
		 * @return string
		 */
		public function getPhql(){ }


		/**
		 * Returns the query built
		 *
		 * @return \Dao\Mvc\Model\Query
		 */
		public function getQuery(){ }

	}
}
