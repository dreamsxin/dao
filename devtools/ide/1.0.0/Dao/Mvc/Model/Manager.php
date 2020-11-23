<?php 

namespace Dao\Mvc\Model {

	/**
	 * Dao\Mvc\Model\Manager
	 *
	 * This components controls the initialization of models, keeping record of relations
	 * between the different models of the application.
	 *
	 * A ModelsManager is injected to a model via a Dependency Injector/Services Container such as Dao\Di.
	 *
	 * <code>
	 * $di = new Dao\Di();
	 *
	 * $di->set('modelsManager', function() {
	 *      return new Dao\Mvc\Model\Manager();
	 * });
	 *
	 * $robot = new Robots($di);
	 * </code>
	 */
	
	class Manager extends \Dao\Di\Injectable implements \Dao\Events\EventsAwareInterface, \Dao\Di\InjectionAwareInterface, \Dao\Mvc\Model\ManagerInterface {

		protected $_customEventsManager;

		protected $_readConnectionServices;

		protected $_writeConnectionServices;

		protected $_defaultConnectionService;

		protected $_defaultReadConnectionService;

		protected $_defaultWriteConnectionService;

		protected $_aliases;

		protected $_hasMany;

		protected $_hasManySingle;

		protected $_hasOne;

		protected $_hasOneSingle;

		protected $_belongsTo;

		protected $_belongsToSingle;

		protected $_hasManyToMany;

		protected $_hasManyToManySingle;

		protected $_initialized;

		protected $_sources;

		protected $_schemas;

		protected $_behaviors;

		protected $_lastInitialized;

		protected $_lastQuery;

		protected $_reusable;

		protected $_dynamicUpdate;

		protected $_namespaceAliases;

		/**
		 * Sets a custom events manager for a specific model
		 *
		 * @param \Dao\Mvc\ModelInterface $model
		 * @param \Dao\Events\ManagerInterface $eventsManager
		 */
		public function setCustomEventsManager($model, $eventsManager){ }


		/**
		 * Returns a custom events manager related to a model
		 *
		 * @param \Dao\Mvc\ModelInterface $model
		 * @return \Dao\Events\ManagerInterface
		 */
		public function getCustomEventsManager($model){ }


		/**
		 * Initializes a model in the model manager
		 *
		 * @param \Dao\Mvc\ModelInterface $model
		 * @return boolean
		 */
		public function initialize($model){ }


		/**
		 * Check whether a model is already initialized
		 *
		 * @param string $modelName
		 * @return boolean
		 */
		public function isInitialized($modelName){ }


		/**
		 * Get last initialized model
		 *
		 * @return \Dao\Mvc\ModelInterface
		 */
		public function getLastInitialized(){ }


		/**
		 * Loads a model throwing an exception if it doesn't exist
		 *
		 * @param  string $modelName
		 * @param  boolean $newInstance
		 * @return \Dao\Mvc\ModelInterface
		 */
		public function load($modelName, $newInstance){ }


		/**
		 * Sets the mapped source for a model
		 *
		 * @param \Dao\Mvc\Model|string $model
		 * @param string $source
		 * @return string
		 */
		public function setModelSource($model, $source){ }


		/**
		 * Returns the mapped source for a model
		 *
		 * @param \Dao\Mvc\Model|string $model
		 * @return string
		 */
		public function getModelSource($model){ }


		/**
		 * Sets the mapped schema for a model
		 *
		 * @param \Dao\Mvc\Model $model
		 * @param string $schema
		 * @return string
		 */
		public function setModelSchema($model, $schema){ }


		/**
		 * Returns the mapped schema for a model
		 *
		 * @param \Dao\Mvc\Model $model
		 * @return string
		 */
		public function getModelSchema($model){ }


		/**
		 * Sets both write and read connection service for a model
		 *
		 * @param \Dao\Mvc\ModelInterface $model
		 * @param string $connectionService
		 */
		public function setConnectionService($model, $connectionService){ }


		/**
		 * Sets write connection service for a model
		 *
		 * @param \Dao\Mvc\ModelInterface $model
		 * @param string $connectionService
		 */
		public function setWriteConnectionService($model, $connectionService){ }


		/**
		 * Sets read connection service for a model
		 *
		 * @param \Dao\Mvc\ModelInterface $model
		 * @param string $connectionService
		 */
		public function setReadConnectionService($model, $connectionService){ }


		/**
		 * Sets default connection service for a model
		 *
		 * @param string $connectionService
		 */
		public function setDefaultConnectionService($connectionService){ }


		/**
		 * Gets default connection service for a model
		 *
		 * @return string
		 */
		public function getDefaultConnectionService(){ }


		/**
		 * Sets default write connection service for a model
		 *
		 * @param string $connectionService
		 */
		public function setDefaultWriteConnectionService($connectionService){ }


		/**
		 * Gets default write connection service for a model
		 *
		 * @return string
		 */
		public function getDefaultWriteConnectionService(){ }


		/**
		 * Sets default read connection service for a model
		 *
		 * @param string $connectionService
		 */
		public function setDefaultReadConnectionService($connectionService){ }


		/**
		 * Gets default read connection service for a model
		 *
		 * @return string
		 */
		public function getDefaultReadConnectionService(){ }


		/**
		 * Returns the connection to write data related to a model
		 *
		 * @param \Dao\Mvc\ModelInterface $model
		 * @return \Dao\Db\AdapterInterface
		 */
		public function getWriteConnection($model){ }


		/**
		 * Returns the connection to read data related to a model
		 *
		 * @param \Dao\Mvc\ModelInterface $model
		 * @return \Dao\Db\AdapterInterface
		 */
		public function getReadConnection($model){ }


		/**
		 * Returns the connection service name used to read data related to a model
		 *
		 * @param \Dao\Mvc\ModelInterface $model
		 * @param string
		 */
		public function getReadConnectionService($model){ }


		/**
		 * Returns the connection service name used to write data related to a model
		 *
		 * @param \Dao\Mvc\ModelInterface $model
		 * @param string
		 */
		public function getWriteConnectionService($model){ }


		/**
		 * Receives events generated in the models and dispatches them to a events-manager if available
		 * Notify the behaviors that are listening in the model
		 *
		 * @param string $eventName
		 * @param \Dao\Mvc\ModelInterface $model
		 */
		public function notifyEvent($eventName, $model){ }


		/**
		 * Dispatch a event to the listeners and behaviors
		 * This method expects that the endpoint listeners/behaviors returns true
		 * meaning that a least one is implemented
		 *
		 * @param \Dao\Mvc\ModelInterface $model
		 * @param string $eventName
		 * @param array $data
		 * @return boolean
		 */
		public function missingMethod($model, $eventName, $data){ }


		/**
		 * Binds a behavior to a model
		 *
		 * @param \Dao\Mvc\ModelInterface $model
		 * @param \Dao\Mvc\Model\BehaviorInterface $behavior
		 */
		public function addBehavior($model, $behavior){ }


		/**
		 * Sets if a model must use dynamic update instead of the all-field update
		 *
		 * @param \Dao\Mvc\Model $model
		 * @param boolean $dynamicUpdate
		 */
		public function useDynamicUpdate($model, $dynamicUpdate){ }


		/**
		 * Checks if a model is using dynamic update instead of all-field update
		 *
		 * @return boolean
		 */
		public function isUsingDynamicUpdate($model){ }


		/**
		 * Setup a 1-1 relation between two models
		 *
		 * @param   \Dao\Mvc\Model $model
		 * @param mixed $fields
		 * @param string $referencedModel
		 * @param mixed $referencedFields
		 * @param array $options
		 * @return  \Dao\Mvc\Model\Relation
		 */
		public function addHasOne($model, $fields, $referencedModel, $referencedFields, $options=null){ }


		/**
		 * Setup a relation reverse many to one between two models
		 *
		 * @param   \Dao\Mvc\Model $model
		 * @param mixed $fields
		 * @param string $referencedModel
		 * @param mixed $referencedFields
		 * @param array $options
		 * @return  \Dao\Mvc\Model\Relation
		 */
		public function addBelongsTo($model, $fields, $referencedModel, $referencedFields, $options=null){ }


		/**
		 * Setup a relation 1-n between two models
		 *
		 * @param 	Dao\Mvc\ModelInterface $model
		 * @param mixed $fields
		 * @param string $referencedModel
		 * @param mixed $referencedFields
		 * @param array $options
		 */
		public function addHasMany($model, $fields, $referencedModel, $referencedFields, $options=null){ }


		/**
		 * Setups a relation n-m between two models
		 *
		 * @param string $fields
		 * @param string $intermediateModel
		 * @param string $intermediateFields
		 * @param string $intermediateReferencedFields
		 * @param string $referencedModel
		 * @param string $referencedFields
		 * @param   array $options
		 * @return  \Dao\Mvc\Model\Relation
		 */
		public function addHasManyToMany($model, $fields, $intermediateModel, $intermediateFields, $intermediateReferencedFields, $referencedModel, $referencedFields, $options=null){ }


		/**
		 * Checks whether a model has a belongsTo relation with another model
		 *
		 * @param 	string $modelName
		 * @param 	string $modelRelation
		 * @return 	boolean
		 */
		public function existsBelongsTo($modelName, $modelRelation){ }


		/**
		 * Checks whether a model has a hasMany relation with another model
		 *
		 * @param 	string $modelName
		 * @param 	string $modelRelation
		 * @return 	boolean
		 */
		public function existsHasMany($modelName, $modelRelation){ }


		/**
		 * Checks whether a model has a hasOne relation with another model
		 *
		 * @param 	string $modelName
		 * @param 	string $modelRelation
		 * @return 	boolean
		 */
		public function existsHasOne($modelName, $modelRelation){ }


		/**
		 * Checks whether a model has a hasManyToMany relation with another model
		 *
		 * @param 	string $modelName
		 * @param 	string $modelRelation
		 * @return 	boolean
		 */
		public function existsHasManyToMany($modelName, $modelRelation){ }


		/**
		 * Returns a relation by its alias
		 *
		 * @param string $modelName
		 * @param string $alias
		 * @return \Dao\Mvc\Model\Relation
		 */
		public function getRelationByAlias($modelName, $alias){ }


		/**
		 * Helper method to query records based on a relation definition
		 *
		 * @param \Dao\Mvc\Model\Relation $relation
		 * @param string $method
		 * @param \Dao\Mvc\ModelInterface $record
		 * @param array $parameters
		 * @return \Dao\Mvc\Model\Resultset\Simple
		 */
		public function getRelationRecords($relation, $method, $record, $parameters=null){ }


		/**
		 * Returns a reusable object from the internal list
		 *
		 * @param string $modelName
		 * @param string $key
		 * @return object
		 */
		public function getReusableRecords($modelName, $key){ }


		/**
		 * Stores a reusable record in the internal list
		 *
		 * @param string $modelName
		 * @param string $key
		 * @param mixed $records
		 */
		public function setReusableRecords($modelName, $key, $records){ }


		/**
		 * Clears the internal reusable list
		 *
		 * @param
		 */
		public function clearReusableObjects(){ }


		/**
		 * Gets belongsTo related records from a model
		 *
		 * @param string $method
		 * @param string $modelName
		 * @param string $modelRelation
		 * @param \Dao\Mvc\Model $record
		 * @param array $parameters
		 * @return \Dao\Mvc\Model\ResultsetInterface
		 */
		public function getBelongsToRecords($method, $modelName, $modelRelation, $record, $parameters=null){ }


		/**
		 * Gets hasMany related records from a model
		 *
		 * @param string $method
		 * @param string $modelName
		 * @param string $modelRelation
		 * @param \Dao\Mvc\Model $record
		 * @param array $parameters
		 * @return \Dao\Mvc\Model\ResultsetInterface
		 */
		public function getHasManyRecords($method, $modelName, $modelRelation, $record, $parameters=null){ }


		/**
		 * Gets belongsTo related records from a model
		 *
		 * @param string $method
		 * @param string $modelName
		 * @param string $modelRelation
		 * @param \Dao\Mvc\Model $record
		 * @param array $parameters
		 * @return \Dao\Mvc\Model\ResultsetInterface
		 */
		public function getHasOneRecords($method, $modelName, $modelRelation, $record, $parameters=null){ }


		/**
		 * Gets all the belongsTo relations defined in a model
		 *
		 *<code>
		 *	$relations = $modelsManager->getBelongsTo(new Robots());
		 *</code>
		 *
		 * @param  \Dao\Mvc\ModelInterface $model
		 * @return \Dao\Mvc\Model\RelationInterface[]
		 */
		public function getBelongsTo($model){ }


		/**
		 * Gets hasMany relations defined on a model
		 *
		 * @param  \Dao\Mvc\ModelInterface $model
		 * @return \Dao\Mvc\Model\RelationInterface[]
		 */
		public function getHasMany($model){ }


		/**
		 * Gets hasOne relations defined on a model
		 *
		 * @param  \Dao\Mvc\ModelInterface $model
		 * @return array
		 */
		public function getHasOne($model){ }


		/**
		 * Gets hasManyToMany relations defined on a model
		 *
		 * @param  \Dao\Mvc\ModelInterface $model
		 * @return \Dao\Mvc\Model\RelationInterface[]
		 */
		public function getHasManyToMany($model){ }


		/**
		 * Gets hasOne relations defined on a model
		 *
		 * @param  \Dao\Mvc\ModelInterface $model
		 * @return array
		 */
		public function getHasOneAndHasMany($model){ }


		/**
		 * Query all the relationships defined on a model
		 *
		 * @param string $modelName
		 * @return \Dao\Mvc\Model\RelationInterface[]
		 */
		public function getRelations($modelName){ }


		/**
		 * Query the first relationship defined between two models
		 *
		 * @param string $first
		 * @param string $second
		 * @return \Dao\Mvc\Model\RelationInterface
		 */
		public function getRelationsBetween($first, $second){ }


		/**
		 * Creates a \Dao\Mvc\Model\Query without execute it
		 *
		 * @param string $phql
		 * @return \Dao\Mvc\Model\QueryInterface
		 */
		public function createQuery($phql){ }


		/**
		 * Creates a \Dao\Mvc\Model\Query and execute it
		 *
		 * @param string $phql
		 * @param array $placeholders
		 * @return \Dao\Mvc\Model\QueryInterface
		 */
		public function executeQuery($phql, $placeholders=null){ }


		/**
		 * Creates a \Dao\Mvc\Model\Query\Builder
		 *
		 * @param string $params
		 * @return \Dao\Mvc\Model\Query\BuilderInterface
		 */
		public function createBuilder($params=null, $type=null){ }


		/**
		 * Returns the lastest query created or executed in the models manager
		 *
		 * @return \Dao\Mvc\Model\QueryInterface
		 */
		public function getLastQuery(){ }


		/**
		 * Registers shorter aliases for namespaces in PHQL statements
		 *
		 * @param string $alias
		 * @param string $namespace
		 */
		public function registerNamespaceAlias($alias, $namespace){ }


		/**
		 * Returns a real namespace from its alias
		 *
		 * @param string $alias
		 * @return string
		 */
		public function getNamespaceAlias($alias){ }


		/**
		 * Returns all the registered namespace aliases
		 *
		 * @return array
		 */
		public function getNamespaceAliases(){ }


		/**
		 * Destroys the PHQL cache
		 */
		public function __destruct(){ }

	}
}
