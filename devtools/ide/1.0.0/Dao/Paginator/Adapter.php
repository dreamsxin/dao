<?php 

namespace Dao\Paginator {

	/**
	 * Dao\Paginator\Adapter
	 *
	 * Base class for Dao\Paginator adapters
	 */
	
	abstract class Adapter extends \Dao\Di\Injectable implements \Dao\Events\EventsAwareInterface, \Dao\Di\InjectionAwareInterface, \Dao\Paginator\AdapterInterface {

		protected $_limitRows;

		protected $_page;

		/**
		 * Set current page number
		 *
		 * @param int $page
		 */
		public function setCurrentPage($page){ }


		/**
		 * Get current page number
		 *
		 * @param int $page
		 */
		public function getCurrentPage(){ }


		/**
		 * Set current rows limit
		 *
		 * @param int $limit
		 *
		 * @return \Dao\Paginator\Adapter
		 */
		public function setLimit($limit){ }


		/**
		 * Get current rows limit
		 *
		 * @return int $limit
		 */
		public function getLimit(){ }

	}
}
