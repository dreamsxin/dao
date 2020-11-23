<?php 

namespace Dao\Aop {

	/**
	 * Dao\Aop\Joinpoint
	 */
	
	class Joinpoint {

		/**
		 * Return caught method call arguments
		 *
		 * @return array
		 */
		public function getArguments(){ }


		/**
		 * Sets caught method call arguments
		 *
		 * @param array $params
		 */
		public function setArguments($params){ }


		/**
		 */
		public function getException(){ }


		/**
		 */
		public function getPointcut(){ }


		/**
		 */
		public function process(){ }


		/**
		 */
		public function getKindOfAdvice(){ }


		/**
		 */
		public function getObject(){ }


		/**
		 */
		public function getReturnedValue(){ }


		/**
		 */
		public function setReturnedValue($value){ }


		/**
		 */
		public function getClassName(){ }


		/**
		 */
		public function getMethodName(){ }


		/**
		 */
		public function getFunctionName(){ }


		/**
		 * Gets the assigned value
		 */
		public function getAssignedValue(){ }


		/**
		 * Sets the assigned value
		 */
		public function setAssignedValue($value){ }


		/**
		 * Gets the property name
		 */
		public function getPropertyName(){ }


		/**
		 */
		public function getPropertyValue(){ }


		/**
		 * Sets the object property
		 */
		public function setProperty($key, $value){ }


		/**
		 * Gets the object property
		 */
		public function getProperty($key){ }

	}
}
