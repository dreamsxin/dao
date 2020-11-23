<?php 

namespace Dao\Annotations {

	/**
	 * Dao\Annotations\Adapter
	 *
	 * This is the base class for Dao\Annotations adapters
	 */
	
	abstract class Adapter implements \Dao\Annotations\AdapterInterface {

		protected $_reader;

		protected $_annotations;

		/**
		 * Sets the annotations parser
		 *
		 * @param \Dao\Annotations\ReaderInterface $reader
		 */
		public function setReader($reader){ }


		/**
		 * Returns the annotation reader
		 *
		 * @return \Dao\Annotations\ReaderInterface
		 */
		public function getReader(){ }


		/**
		 * Parses or retrieves all the annotations found in a class
		 *
		 * @param string|object $className
		 * @return \Dao\Annotations\Reflection
		 */
		public function get($className){ }


		/**
		 * Returns the annotations found in all the class' methods
		 *
		 * @param string $className
		 * @return array
		 */
		public function getMethods($className){ }


		/**
		 * Returns the annotations found in a specific method
		 *
		 * @param string $className
		 * @param string $methodName
		 * @return \Dao\Annotations\Collection
		 */
		public function getMethod($className, $methodName){ }


		/**
		 * Returns the annotations found in all the class' methods
		 *
		 * @param string $className
		 * @return array
		 */
		public function getProperties($className){ }


		/**
		 * Returns the annotations found in a specific property
		 *
		 * @param string $className
		 * @param string $propertyName
		 * @return \Dao\Annotations\Collection
		 */
		public function getProperty($className, $propertyName){ }

	}
}
