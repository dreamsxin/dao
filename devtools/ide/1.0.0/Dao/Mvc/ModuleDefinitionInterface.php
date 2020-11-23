<?php 

namespace Dao\Mvc {

	interface ModuleDefinitionInterface {

		public function registerAutoloaders(\Dao\DiInterface $dependencyInjector);


		public function registerServices(\Dao\DiInterface $dependencyInjector);

	}
}
