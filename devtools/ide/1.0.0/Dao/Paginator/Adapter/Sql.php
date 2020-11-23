<?php 

namespace Dao\Paginator\Adapter {

	/**
	 * Dao\Paginator\Adapter\Sql
	 *
	 * Pagination using a SQL as source of data
	 *
	 * <code>
	 * $sql = "SELECT * FROM robots WHERE type = :type LIMIT :limit OFFSET :offset";
	 * $sql2 = "SELECT COUNT(*) rowcount FROM robots WHERE type = :type FROM robots";
	 *
	 * $bind = ['type' => 'google'];
	 *
	 * $paginator = new \Dao\Paginator\Adapter\Sql(array(
	 *                 "db" => $this->db,
	 *                 "sql" => $sql,
	 *                 "total_sql" => $sql2,
	 *                 "bind" => $bind,
	 *                 //"total_bind" => $total_bind,
	 *                 "limit" => 20,
	 *                 "page" => $page
	 * ));
	 * </code>
	 */
	
	class Sql extends \Dao\Paginator\Adapter implements \Dao\Paginator\AdapterInterface, \Dao\Di\InjectionAwareInterface, \Dao\Events\EventsAwareInterface {

		protected $_db;

		protected $_sql;

		protected $_total_sql;

		protected $_bind;

		protected $_total_bind;

		protected $_fetchMode;

		/**
		 * \Dao\Paginator\Adapter\Sql
		 *
		 * @param array $config
		 */
		public function __construct($config){ }


		/**
		 * Returns a slice of the resultset to show in the pagination
		 *
		 * @return stdClass
		 */
		public function getPaginate(){ }


		/**
		 * Set query builder object
		 *
		 * @param \Dao\Db\AdapterInterface $db
		 *
		 * @return \Dao\Paginator\Adapter\Sql $this Fluent interface
		 */
		public function setDb($db){ }


		/**
		 * Get query builder object
		 *
		 * @return \Dao\Db\AdapterInterface $db
		 */
		public function getDb(){ }

	}
}
