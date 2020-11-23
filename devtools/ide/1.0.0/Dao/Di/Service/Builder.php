<?php 

namespace Dao\Di\Service {

	/**
	 * Dao\Di\Service\Builder
	 *
	 * This class builds instances based on complex definitions
	 */
	
	class Builder {

		/**
		 * Resolves a constructor/call parameter
		 *
		 * @param \Dao\DiInterface $dependencyInjector
		 * @param int $position
		 * @param array $argument
		 * @return mixed
		 */
		protected function _buildParameter(){ }


		/**
		 * Resolves an array of parameters
		 *
		 * @param \Dao\DiInterface $dependencyInjector
		 * @param array $arguments
		 * @return array
		 */
		protected function _buildParameters(){ }


		/**
		 * Builds a service using a complex service definition
		 *
		 * @param \Dao\DiInterface $dependencyInjector
		 * @param array $definition
		 * @param array $parameters
		 * @return mixed
		 */
		public function build(\Dao\DiInterface $dependencyInjector, $definition, $parameters=null){ }

	}
}
