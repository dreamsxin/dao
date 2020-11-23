<?php 

namespace Dao\Mvc {

	interface ViewInterface {

		public function setViewsDir($viewsDir);


		public function getViewsDir();


		public function setLayoutsDir($layoutsDir);


		public function getLayoutsDir();


		public function setPartialsDir($partialsDir);


		public function getPartialsDir();


		public function setBasePath($basePath);


		public function getBasePath();


		public function getCurrentRenderLevel();


		public function getRenderLevel();


		public function setRenderLevel($level);


		public function disableLevel($level);


		public function getDisabledLevels();


		public function setMainView($viewPath);


		public function getMainView();


		public function setLayout($layout);


		public function getLayout();


		public function setTemplateBefore($templateBefore);


		public function cleanTemplateBefore();


		public function setTemplateAfter($templateAfter);


		public function cleanTemplateAfter();


		public function setParamToView($key, $value);


		public function getParamsToView();


		public function setVars($params, $merge=null);


		public function setVar($key, $value);


		public function getVar($key);


		public function setControllerName($controllerName);


		public function getControllerName();


		public function setActionName($actionName);


		public function getActionName();


		public function setParams($params);


		public function getParams();


		public function start();


		public function registerEngines($engines);


		public function getRegisteredEngines();


		public function getEngines();


		public function exists($view, $absolute_path=null);


		public function render($controllerName, $actionName, $params=null, $namespace=null, $viewModel=null);


		public function pick($renderView);


		public function partial($partialPath);


		public function getRender($controllerName, $actionName, $params=null, $configCallback=null);


		public function finish();


		public function isCaching();


		public function getCache();


		public function cache($options=null);


		public function setContent($content, $append=null);


		public function getContent();


		public function startSection($name);


		public function stopSection();


		public function section($name, $defaultValue=null);


		public function getActiveRenderPath();


		public function disable();


		public function enable();


		public function isDisabled();


		public function enableNamespaceView();


		public function disableNamespaceView();


		public function enableLowerCase();


		public function disableLowerCase();


		public function setConverter($name, $converter);


		public function getConverter($name);


		public function reset();

	}
}
