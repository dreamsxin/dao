<?php 

namespace Dao\Db\Profiler {

	/**
	 * Dao\Db\Profiler\Item
	 *
	 * This class identifies each profile in a Dao\Db\Profiler
	 *
	 */
	
	class Item extends \Dao\Profiler\Item implements \Dao\Profile\ItemInterface {

		protected $_sqlStatement;

		protected $_sqlVariables;

		protected $_sqlBindTypes;

		/**
		 * Sets the SQL statement related to the profile
		 *
		 * @param string $sqlStatement
		 */
		public function setSQLStatement($sqlStatement){ }


		/**
		 * Returns the SQL statement related to the profile
		 *
		 * @return string
		 */
		public function getSQLStatement(){ }


		/**
		 * Sets the SQL variables related to the profile
		 *
		 * @param array $sqlParams
		 */
		public function setSQLVariables($sqlVariables){ }


		/**
		 * Returns the SQL variables related to the profile
		 *
		 * @return array
		 */
		public function getSQLVariables(){ }


		/**
		 * Sets the SQL bind types related to the profile
		 *
		 * @param array $sqlParams
		 */
		public function setSQLBindTypes($sqlBindTypes){ }


		/**
		 * Returns the SQL bind types related to the profile
		 *
		 * @return array
		 */
		public function getSQLBindTypes(){ }

	}
}
