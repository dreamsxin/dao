<?php 

namespace Dao\Mvc\Model {

	/**
	 * Dao\Mvc\Model\Query
	 *
	 * This class takes a PHQL intermediate representation and executes it.
	 *
	 *<code>
	 *
	 * $phql = "SELECT c.price*0.16 AS taxes, c.* FROM Cars AS c JOIN Brands AS b
	 *          WHERE b.name = :name: ORDER BY c.name";
	 *
	 * $result = $manager->executeQuery($phql, array(
	 *   'name' => 'Lamborghini'
	 * ));
	 *
	 * foreach ($result as $row) {
	 *   echo "Name: ", $row->cars->name, "\n";
	 *   echo "Price: ", $row->cars->price, "\n";
	 *   echo "Taxes: ", $row->taxes, "\n";
	 * }
	 *
	 *</code>
	 */
	
	class Query extends \Dao\Di\Injectable implements \Dao\Events\EventsAwareInterface, \Dao\Di\InjectionAwareInterface, \Dao\Mvc\Model\QueryInterface {

		const TYPE_SELECT = 309;

		const TYPE_INSERT = 306;

		const TYPE_UPDATE = 300;

		const TYPE_DELETE = 303;

		protected $_modelsManager;

		protected $_modelsMetaData;

		protected $_connection;

		protected $_type;

		protected $_phql;

		protected $_ast;

		protected $_intermediate;

		protected $_models;

		protected $_sqlAliases;

		protected $_sqlAliasesModels;

		protected $_sqlModelsAliases;

		protected $_sqlAliasesModelsInstances;

		protected $_sqlColumnAliases;

		protected $_modelsInstances;

		protected $_currentModelsInstances;

		protected $_cache;

		protected $_cacheOptions;

		protected $_uniqueRow;

		protected $_bindParams;

		protected $_bindTypes;

		protected $_mergeBindParams;

		protected $_mergeBindTypes;

		protected $_index;

		protected $_ignoreLastInsertId;

		/**
		 * \Dao\Mvc\Model\Query constructor
		 *
		 * @param string $phql
		 * @param \Dao\DiInterface $dependencyInjector
		 */
		public function __construct($phql){ }


		public function setPhql($phql=null){ }


		public function getPhql(){ }


		/**
		 * Returns the models manager related to the entity instance
		 *
		 * @return \Dao\Mvc\Model\ManagerInterface
		 */
		public function getModelsManager(){ }


		/**
		 * Returns the models meta-data service related to the entity instance
		 *
		 * @return \Dao\Mvc\Model\MetaDataInterface
		 */
		public function getModelsMetaData(){ }


		/**
		 * Tells to the query if only the first row in the resultset must be returned
		 *
		 * @param boolean $uniqueRow
		 * @return \Dao\Mvc\Model\Query
		 */
		public function setUniqueRow($uniqueRow){ }


		/**
		 * Check if the query is programmed to get only the first row in the resultset
		 *
		 * @return boolean
		 */
		public function getUniqueRow(){ }


		/**
		 * Replaces the model's name to its source name in a qualifed-name expression
		 *
		 * @param array $expr
		 * @return string
		 */
		protected function _getQualified(){ }


		/**
		 * Resolves a expression in a single call argument
		 *
		 * @param array $argument
		 * @return string
		 */
		protected function _getCallArgument(){ }


		/**
		 * Resolves a expression in a single call argument
		 *
		 * @param array $argument
		 * @return string
		 */
		protected function _getCaseExpression(){ }


		/**
		 * Resolves a expression in a single call argument
		 *
		 * @param array $expr
		 * @return string
		 */
		protected function _getFunctionCall(){ }


		/**
		 * Resolves an expression from its intermediate code into a string
		 *
		 * @param array $expr
		 * @param boolean $quoting
		 * @return string
		 */
		protected function _getExpression(){ }


		/**
		 * Resolves a column from its intermediate representation into an array used to determine
		 * if the resulset produced is simple or complex
		 *
		 * @param array $column
		 * @return array
		 */
		protected function _getSelectColumn(){ }


		/**
		 * Resolves a table in a SELECT statement checking if the model exists
		 *
		 * @param \Dao\Mvc\Model\ManagerInterface $manager
		 * @param array $qualifiedName
		 * @return string
		 */
		protected function _getTable(){ }


		/**
		 * Resolves a JOIN clause checking if the associated models exist
		 *
		 * @param \Dao\Mvc\Model\ManagerInterface $manager
		 * @param array $join
		 * @return array
		 */
		protected function _getJoin(){ }


		/**
		 * Resolves a JOIN type
		 *
		 * @param array $join
		 * @return string
		 */
		protected function _getJoinType(){ }


		/**
		 * Resolves joins involving has-one/belongs-to/has-many relations
		 *
		 * @param string $joinType
		 * @param string $joinSource
		 * @param string $modelAlias
		 * @param string $joinAlias
		 * @param \Dao\Mvc\Model\RelationInterface $relation
		 * @return array
		 */
		protected function _getSingleJoin(){ }


		/**
		 * Resolves joins involving many-to-many relations
		 *
		 * @param string $joinType
		 * @param string $joinSource
		 * @param string $modelAlias
		 * @param string $joinAlias
		 * @param \Dao\Mvc\Model\RelationInterface $relation
		 * @return array
		 */
		protected function _getMultiJoin(){ }


		/**
		 * Processes the JOINs in the query returning an internal representation for the database dialect
		 *
		 * @param array $select
		 * @return array
		 */
		protected function _getJoins(){ }


		/**
		 * Returns a processed order clause for a SELECT statement
		 *
		 * @param array $order
		 * @return string
		 */
		protected function _getOrderClause(){ }


		/**
		 * Returns a processed group clause for a SELECT statement
		 *
		 * @param array $group
		 * @return string
		 */
		protected function _getGroupClause(){ }


		protected function _getLimitClause(){ }


		/**
		 * Analyzes a SELECT intermediate code and produces an array to be executed later
		 *
		 * @return array
		 */
		protected function _prepareSelect(){ }


		/**
		 * Analyzes an INSERT intermediate code and produces an array to be executed later
		 *
		 * @return array
		 */
		protected function _prepareInsert(){ }


		/**
		 * Analyzes an UPDATE intermediate code and produces an array to be executed later
		 *
		 * @return array
		 */
		protected function _prepareUpdate(){ }


		/**
		 * Analyzes a DELETE intermediate code and produces an array to be executed later
		 *
		 * @return array
		 */
		protected function _prepareDelete(){ }


		/**
		 * Parses the intermediate code produced by \Dao\Mvc\Model\Query\Lang generating another
		 * intermediate representation that could be executed by \Dao\Mvc\Model\Query
		 *
		 * @return array
		 */
		public function parse(){ }


		/**
		 * Sets the cache parameters of the query
		 *
		 * @param array $cacheOptions
		 * @return \Dao\Mvc\Model\Query
		 */
		public function cache($cacheOptions){ }


		/**
		 * Returns the current cache options
		 *
		 * @param array
		 */
		public function getCacheOptions(){ }


		/**
		 * Returns the current cache backend instance
		 *
		 * @return \Dao\Cache\BackendInterface
		 */
		public function getCache(){ }


		/**
		 * Ignore last insert id
		 *
		 * @return \Dao\Mvc\Model\Query
		 */
		public function ignoreLastInsertId(){ }


		/**
		 * Executes the SELECT intermediate representation producing a \Dao\Mvc\Model\Resultset
		 *
		 * @param array $intermediate
		 * @param array $bindParams
		 * @param array $bindTypes
		 * @return \Dao\Mvc\Model\ResultsetInterface
		 */
		protected function _executeSelect(){ }


		/**
		 * Executes the INSERT intermediate representation producing a \Dao\Mvc\Model\Query\Status
		 *
		 * @param array $intermediate
		 * @param array $bindParams
		 * @param array $bindTypes
		 * @return \Dao\Mvc\Model\Query\StatusInterface
		 */
		protected function _executeInsert(){ }


		/**
		 * Executes the UPDATE intermediate representation producing a \Dao\Mvc\Model\Query\Status
		 *
		 * @param array $intermediate
		 * @param array $bindParams
		 * @param array $bindTypes
		 * @return \Dao\Mvc\Model\Query\StatusInterface
		 */
		protected function _executeUpdate(){ }


		/**
		 * Executes the DELETE intermediate representation producing a \Dao\Mvc\Model\Query\Status
		 *
		 * @param array $intermediate
		 * @param array $bindParams
		 * @param array $bindTypes
		 * @return \Dao\Mvc\Model\Query\StatusInterface
		 */
		protected function _executeDelete(){ }


		/**
		 * Executes a parsed PHQL statement
		 *
		 * @param array $bindParams
		 * @param array $bindTypes
		 * @return mixed
		 */
		public function execute($bindParams=null, $bindTypes=null, $useRawsql=null){ }


		/**
		 * Executes the query returning the first result
		 *
		 * @param array $bindParams
		 * @param array $bindTypes
		 * @return \Dao\Mvc\ModelInterface
		 */
		public function getSingleResult($bindParams=null, $bindTypes=null){ }


		/**
		 * Sets the type of PHQL statement to be executed
		 *
		 * @param int $type
		 * @return \Dao\Mvc\Model\Query
		 */
		public function setType($type){ }


		/**
		 * Gets the type of PHQL statement executed
		 *
		 * @return int
		 */
		public function getType(){ }


		/**
		 * Get bind parameter
		 *
		 * @param string $name
		 * @return mixed
		 */
		public function getBindParam($name){ }


		/**
		 * Set default bind parameters
		 *
		 * @param array $bindParams
		 * @return \Dao\Mvc\Model\Query
		 */
		public function setBindParams($bindParams){ }


		/**
		 * Returns default bind params
		 *
		 * @return array
		 */
		public function getBindParams(){ }


		/**
		 * Set merge bind parameters
		 *
		 * @param array $bindParams
		 * @return \Dao\Mvc\Model\Query
		 */
		public function setMergeBindParams($bindParams){ }


		/**
		 * Returns merge bind params
		 *
		 * @return array
		 */
		public function getMergeBindParams(){ }


		/**
		 * Set bind type
		 *
		 * @param string $name
		 * @param int $type
		 * @return \Dao\Mvc\Model\Query
		 */
		public function setBindType($name, $type){ }


		/**
		 * Set default bind types
		 *
		 * @param array $bindTypes
		 * @return \Dao\Mvc\Model\Query
		 */
		public function setBindTypes($bindTypes){ }


		/**
		 * Returns default bind types
		 *
		 * @return array
		 */
		public function getBindTypes(){ }


		/**
		 * Set merge bind types
		 *
		 * @param array $bindTypes
		 * @return \Dao\Mvc\Model\Query
		 */
		public function setMergeBindTypes($bindTypes){ }


		/**
		 * Returns default bind types
		 *
		 * @return array
		 */
		public function getMergeBindTypes(){ }


		/**
		 * Adds the index
		 *
		 * @param string $index
		 * @return \Dao\Mvc\Model\Query
		 */
		public function setIndex($index){ }


		/**
		 * Gets the index
		 *
		 * @return string
		 */
		public function getIndex(){ }


		/**
		 * Allows to set the IR to be executed
		 *
		 * @param array $intermediate
		 * @return \Dao\Mvc\Model\Query
		 */
		public function setIntermediate($intermediate){ }


		/**
		 * Returns the intermediate representation of the PHQL statement
		 *
		 * @return array
		 */
		public function getIntermediate(){ }


		/**
		 * Returns the models of the PHQL statement
		 *
		 * @return array
		 */
		public function getModels(){ }


		/**
		 * Sets the connection
		 *
		 * @param \Dao\Db\AdapterInterface $connection
		 * @return \Dao\Mvc\Model\Query
		 */
		public function setConnection($connection){ }


		/**
		 * Gets the connection
		 *
		 * @return mixed
		 */
		public function getConnection(){ }


		/**
		 * Gets the read connection
		 *
		 * @param array $intermediate
		 * @param array $bindParams
		 * @param array $bindTypes
		 * @return mixed
		 */
		public function getReadConnection($intermediate=null, $bindParams=null, $bindTypes=null){ }


		/**
		 * Gets the write connection
		 *
		 * @param array $intermediate
		 * @param array $bindParams
		 * @param array $bindTypes
		 * @return mixed
		 */
		public function getWriteConnection($intermediate=null, $bindParams=null, $bindTypes=null){ }


		/**
		 * Sets conflict
		 *
		 * @param array $conflict
		 * @return \Dao\Mvc\Model\Query
		 */
		public function setConflict($conflict){ }

	}
}
