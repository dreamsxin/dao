<?php 

namespace Dao {

	interface RouterInterface {

		public function setDefaultModule($moduleName);


		public function getDefaultModule();


		public function setDefaultNamespace($namespaceName);


		public function getDefaultNamespace();


		public function setDefaultHandler($handlerName);


		public function getDefaultHandler();


		public function setDefaultAction($actionName);


		public function getDefaultAction();


		public function setDefaultParams($params);


		public function getDefaultParams();


		public function setCaseSensitive($caseSensitive);


		public function getCaseSensitive();


		public function setMode($mode);


		public function getMode();


		public function setModuleName($moduleName);


		public function getModuleName();


		public function setNamespaceName($namespaceName);


		public function getNamespaceName();


		public function setHandlerName($handlerName);


		public function getHandlerName();


		public function setActionName($actionName);


		public function getActionName();


		public function setParams($params);


		public function getParams();


		public function handle($uri=null);

	}
}
