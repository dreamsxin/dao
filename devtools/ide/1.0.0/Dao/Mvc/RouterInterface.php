<?php 

namespace Dao\Mvc {

	interface RouterInterface {

		public function setDefaultModule($moduleName);


		public function setDefaultController($controllerName);


		public function setDefaultAction($actionName);


		public function setDefaults($defaults);


		public function handle($uri=null);


		public function add($pattern, $paths=null, $regex=null, $httpMethods=null);


		public function addGet($pattern, $paths=null, $regex=null);


		public function addPost($pattern, $paths=null, $regex=null);


		public function addPut($pattern, $paths=null, $regex=null);


		public function addDelete($pattern, $paths=null, $regex=null);


		public function addOptions($pattern, $paths=null, $regex=null);


		public function addPatch($pattern, $paths=null, $regex=null);


		public function addHead($pattern, $paths=null, $regex=null);


		public function clear();


		public function getModuleName();


		public function getControllerName();


		public function getActionName();


		public function getParams();


		public function getMatchedRoute();


		public function getMatches();


		public function wasMatched();


		public function getRoutes();


		public function getRouteById($id);


		public function getRouteByName($name);


		public function isExactControllerName();

	}
}
