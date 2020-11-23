<?php 

namespace Dao\Mvc\Router {

	interface RouteInterface {

		public function compilePattern($pattern, $regex=null);


		public function via($httpMethods);


		public function reConfigure($pattern, $paths=null, $regex=null);


		public function getName();


		public function setName($name);


		public function setHttpMethods($httpMethods);


		public function getRouteId();


		public function getPattern();


		public function getCompiledPattern();


		public function getPaths();


		public function getHttpMethods();


		public static function reset();


		public function setDefaults($defaults);


		public function getDefaultModule();


		public function getDefaultNamespace();


		public function getDefaultController();


		public function getDefaultAction();


		public function getDefaultParams();


		public function setMode($mode);


		public function getMode();

	}
}
