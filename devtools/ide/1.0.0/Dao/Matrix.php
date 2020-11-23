<?php 

namespace Dao {

	/**
	 * Dao\Matrix
	 *
	 */
	
	class Matrix {

		const TYPE_DOUBLE = 0;

		const TYPE_LONG = 1;

		/**
		 * 
		 *
		 * @param array $matrix
		 * @return boolean
		 */
		public static function valid($matrix){ }


		/**
		 * 
		 *
		 * @param array $matrix
		 */
		public static function dump($matrix){ }


		/**
		 * 
		 *
		 * @param array $matrix
		 * @return array
		 */
		public static function transpose($matrix){ }


		/**
		 * 
		 *
		 * @param array $matrix
		 * @param array $matrix2
		 * @return array
		 */
		public static function add($matrix, $matrix2, $elementwise=null, $type=null){ }


		/**
		 * 
		 *
		 * @param array $matrix
		 * @param mixed $matrix2
		 * @return array
		 */
		public static function mul($matrix, $matrix2, $elementwise=null, $type=null){ }

	}
}
