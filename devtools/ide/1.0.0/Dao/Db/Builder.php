<?php 

namespace Dao\Db {

	/**
	 * Dao\Db\Builder
	 *
	 * Helps to create queries using an OO interface
	 */
	
	abstract class Builder extends \Dao\Di\Injectable implements \Dao\Events\EventsAwareInterface, \Dao\Di\InjectionAwareInterface, \Dao\Db\BuilderInterface {

		protected $_defaultConnectionService;

		protected $_definition;

		protected $_bindParams;

		protected $_bindTypes;

		/**
		 * Create a select builder
		 *
		 *<code>
		 * $resultset = \Dao\Db\Builder::select('robots')
		 * 	  ->join('robots_parts', 'robots.id = robots_parts.robots_id')
		 * 	  ->where('robots.id = 1')
		 * 	  ->limit(20)
		 * 	  ->orderBy('robots.name')
		 *    ->execute();
		 *</code>
		 *
		 * @return \Dao\Db\Builder\Select
		 */
		public static function select($tables, $db=null){ }


		/**
		 * Create a update builder
		 *
		 *<code>
		 * $ret = \Dao\Db\Builder::update('robots')
		 * 	  ->set(['name' => 'test'])
		 * 	  ->where('id = 1')
		 *    ->execute();
		 *</code>
		 *
		 * @return \Dao\Db\Builder\Update
		 */
		public static function update($table, $db=null){ }


		/**
		 * Create a insert builder
		 *
		 *<code>
		 * $ret = \Dao\Db\Builder::insert('robots')
		 * 	  ->values(['name' => 'test'])
		 *    ->execute();
		 *</code>
		 *
		 * @return \Dao\Db\Builder\Insert
		 */
		public static function insert($table, $db=null){ }


		/**
		 * Create a delete builder
		 *
		 *<code>
		 * $ret = \Dao\Db\Builder::delete('robots')
		 * 	  ->where('id = 1')
		 *    ->execute();
		 *</code>
		 *
		 * @return \Dao\Db\Builder\Delete
		 */
		public static function delete($table, $db=null){ }


		/**
		 * Sets the bind parameters
		 *
		 * @param array $bindParams
		 * @return \Dao\Db\Builder
		 */
		public function setBindParams($bindparams, $merge=null){ }


		/**
		 * Gets the bind parameters
		 *
		 * @return \Dao\Db\Builder
		 */
		public function getBindParams(){ }


		/**
		 * Sets the bind types
		 *
		 * @param array $bindTypes
		 * @return \Dao\Db\Builder
		 */
		public function setBindTypes($bindtypes, $merge=null){ }


		/**
		 * Gets the bind types
		 *
		 * @return \Dao\Db\Builder
		 */
		public function getBindTypes(){ }


		/**
		 * Execute query
		 *
		 * @return \Dao\Db\ResultInterface|boolean|array
		 */
		public function execute($pretreatment=null){ }

	}
}
