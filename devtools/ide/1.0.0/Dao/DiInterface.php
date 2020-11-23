<?php 

namespace Dao {

	interface DiInterface extends \ArrayAccess {

		public function set($name, $definition, $shared=null);


		public function remove($name);


		public function get($name, $parameters=null, $noError=null, $noerror=null);


		public function getShared($name, $parameters=null, $noError=null);


		public function setService($name, \Dao\Di\ServiceInterface $rawDefinition);


		public function getService($name);


		public function has($name);


		public function wasFreshInstance();


		public function getServices();


		public static function setDefault(\Dao\DiInterface $dependencyInjector, $flag=null);


		public static function getDefault();


		public static function reset();

	}
}
