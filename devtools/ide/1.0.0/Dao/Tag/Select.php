<?php 

namespace Dao\Tag {

	/**
	 * Dao\Tag\Select
	 *
	 * Generates a SELECT html tag using a static array of values or a Dao\Mvc\Model resultset
	 */
	
	abstract class Select {

		/**
		 * Generates a SELECT tag
		 *
		 * @param array $parameters
		 * @param array $data
		 */
		public static function selectField($parameters, $data=null){ }


		/**
		 * Generate the OPTION tags based on a resulset
		 *
		 * @param \Dao\Mvc\Model $resultset
		 * @param array $using
		 * @param mixed value
		 * @param string $closeOption
		 */
		protected static function _optionsFromResultset(){ }


		/**
		 * Generate the OPTION tags based on an array
		 *
		 * @param \Dao\Mvc\ModelInterface $resultset
		 * @param array $using
		 * @param mixed value
		 * @param string $closeOption
		 */
		protected static function _optionsFromArray(){ }

	}
}
