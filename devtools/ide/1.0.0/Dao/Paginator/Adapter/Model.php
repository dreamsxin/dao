<?php 

namespace Dao\Paginator\Adapter {

	/**
	 * Dao\Paginator\Adapter\Model
	 *
	 * This adapter allows to paginate data using a Dao\Mvc\Model resultset as base
	 */
	
	class Model extends \Dao\Paginator\Adapter implements \Dao\Paginator\AdapterInterface, \Dao\Di\InjectionAwareInterface, \Dao\Events\EventsAwareInterface {

		protected $_config;

		/**
		 * \Dao\Paginator\Adapter\Model constructor
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
