<?php 

namespace Dao\Mvc\View {

	interface EngineInterface {

		public function getContent();


		public function startSection($name);


		public function stopSection();


		public function section($name, $defaultValue=null);


		public function partial($partialPath);


		public function render($path, $params, $mustClean=null);


		public function addMethod($name, \Closure $handler);


		public function __call($method, $arguments=null);

	}
}
