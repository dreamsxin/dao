<?php 

namespace Dao\Db\Builder {

	/**
	 * Dao\Db\Builder\Delete
	 */
	
	class Delete extends \Dao\Db\Builder\Where implements \Dao\Events\EventsAwareInterface, \Dao\Di\InjectionAwareInterface, \Dao\Db\BuilderInterface {

		/**
		 * \Dao\Db\Builder\Delete constructor
		 *
		 * @param array $params
		 * @param \Dao\Di $dependencyInjector
		 */
		public function __construct($table=null, $db=null){ }


		/**
		 * Returns a PHQL statement built based on the builder parameters
		 *
		 * @return string
		 */
		protected function _execute(){ }

	}
}
