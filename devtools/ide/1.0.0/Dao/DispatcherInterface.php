<?php 

namespace Dao {

	interface DispatcherInterface {

		public function setActionSuffix($actionSuffix);


		public function setDefaultModule($module);


		public function getDefaultModule();


		public function setDefaultNamespace($namespace);


		public function getDefaultNamespace();


		public function setDefaultHandler($handlerName);


		public function getDefaultHandler();


		public function setDefaultAction($actionName);


		public function getDefaultAction();


		public function setModuleName($module);


		public function getModuleName();


		public function setNamespaceName($namespace);


		public function getNamespaceName();


		public function setHandlerName($handlerName);


		public function getHandlerName();


		public function setActionName($actionName);


		public function getActionName();


		public function setParams($params);


		public function getParams();


		public function hasParam($param);


		public function setParam($param, $value);


		public function getParam($param, $filters=null);


		public function isFinished();


		public function getReturnedValue();


		public function dispatch();


		public function forward($forward);


		public function camelizeNamespace($camelize);


		public function setErrorHandler($callback, $exception_code=null);


		public function getErrorHandler($exception_code);

	}
}
