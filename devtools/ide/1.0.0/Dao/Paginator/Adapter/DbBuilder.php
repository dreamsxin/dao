<?php 

namespace Dao\Paginator\Adapter {

	/**
	 * Dao\Paginator\Adapter\DbBuilder
	 *
	 * Pagination using a PHQL query builder as source of data
	 *
	 *<code>
	 *  $builder = Dao\Db\Builder::select('robots')
	 *                   ->columns('id, name')
	 *                   ->orderBy('name');
	 *
	 *  $paginator = new Dao\Paginator\Adapter\DbBuilder(array(
	 *      "builder" => $builder,
	 *      "limit"=> 20,
	 *      "page" => 1
	 *  ));
	 *</code>
	 */
	
	class DbBuilder extends \Dao\Paginator\Adapter implements \Dao\Paginator\AdapterInterface, \Dao\Di\InjectionAwareInterface, \Dao\Events\EventsAwareInterface {

		protected $_builder;

		protected $_totalItems;

		protected $_limitRows;

		/**
		 * \Dao\Paginator\Adapter\DbBuilder
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

	}
}
