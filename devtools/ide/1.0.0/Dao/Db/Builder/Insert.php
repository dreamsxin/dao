<?php 

namespace Dao\Db\Builder {

	/**
	 * Dao\Db\Builder\Insert
	 */
	
	class Insert extends \Dao\Db\Builder implements \Dao\Db\BuilderInterface, \Dao\Di\InjectionAwareInterface, \Dao\Events\EventsAwareInterface {

		protected $_values;

		/**
		 * \Dao\Db\Builder\Insert constructor
		 *
		 * @param string $table
		 * @param string $db
		 */
		public function __construct($table=null, $db=null){ }


		/**
		 * Sets the values to insert
		 *
		 * @param array $values
		 * @return \Dao\Db\Builder\Insert
		 */
		public function values($values){ }


		/**
		 * Returns a PHQL statement built based on the builder parameters
		 *
		 * @return string
		 */
		protected function _execute(){ }

	}
}
