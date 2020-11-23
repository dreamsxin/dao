<?php 

namespace Dao\Paginator\Adapter {

	/**
	 * Dao\Paginator\Adapter\QueryBuilder
	 *
	 * Pagination using a PHQL query builder as source of data
	 *
	 *<code>
	 *  $builder = $this->modelsManager->createBuilder()
	 *                   ->columns('id, name')
	 *                   ->from('Robots')
	 *                   ->orderBy('name');
	 *
	 *  $paginator = new Dao\Paginator\Adapter\QueryBuilder(array(
	 *      "builder" => $builder,
	 *      "limit"=> 20,
	 *      "page" => 1
	 *  ));
	 *</code>
	 */
	
	class QueryBuilder extends \Dao\Paginator\Adapter implements \Dao\Paginator\AdapterInterface, \Dao\Di\InjectionAwareInterface, \Dao\Events\EventsAwareInterface {

		protected $_builder;

		protected $_totalItems;

		protected $_limitRows;

		protected $_cache;

		/**
		 * \Dao\Paginator\Adapter\QueryBuilder
		 *
		 * @param array $config
		 */
		public function __construct($config){ }


		/**
		 * Returns a slice of the resultset to show in the pagination
		 *
		 * @return \stdClass
		 */
		public function getPaginate(){ }


		/**
		 * Set query builder object
		 *
		 * @param \Dao\Mvc\Model\Query\BuilderInterface $builder
		 *
		 * @return \Dao\Paginator\Adapter\QueryBuilder $this Fluent interface
		 */
		public function setQueryBuilder($queryBuilder){ }


		/**
		 * Get query builder object
		 *
		 * @return \Dao\Mvc\Model\Query\BuilderInterface $builder
		 */
		public function getQueryBuilder(){ }

	}
}
