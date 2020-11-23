<?php 

namespace Dao\Mvc\View {

	interface ModelInterface {

		public function setTemplate($template);


		public function getTemplate();


		public function setVars($params, $merge=null);


		public function getVars();


		public function setVar($key, $value);


		public function getVar($key, $default_value=null);


		public function addChild($viewmodel, $name=null, $append=null);


		public function appendChild($viewmodel, $name=null);


		public function getChild($name);


		public function hasChild();


		public function setCaptureTo($capture);


		public function getCaptureTo();


		public function setTerminal($terminate);


		public function getTerminal();


		public function setAppend($append);


		public function isAppend();


		public function setView($view);


		public function getView();


		public function render();

	}
}
