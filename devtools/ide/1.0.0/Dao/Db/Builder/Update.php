<?php 

namespace Dao\Db\Builder {

	/**
	 * Dao\Db\Builder\Update
	 */
	
	class Update extends \Dao\Db\Builder\Where implements \Dao\Events\EventsAwareInterface, \Dao\Di\InjectionAwareInterface, \Dao\Db\BuilderInterface {

		protected $_set;

		/**
		 * \Dao\Db\Builder\Update constructor
		 *
		 * @param string|array $tables
		 * @param string $db
		 */
		public function __construct($tables=null, $db=null){ }


		/**
		 * Sets the values to update with an associative array
		 *
		 *<code>
		 *	$builder->set(array('id' => 1, 'name' => 'Google'));
		 *</code>
		 *
		 * @param array $set
		 * @return \Dao\Db\Builder\Update
		 */
		public function set($set){ }


		/**
		 * Returns a PHQL statement built based on the builder parameters
		 *
		 * @return string
		 */
		protected function _execute(){ }

	}
}
