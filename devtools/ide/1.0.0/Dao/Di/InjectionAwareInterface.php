<?php 

namespace Dao\Di {

	interface InjectionAwareInterface {

		public function setDI(\Dao\DiInterface $dependencyInjector);


		public function getDI($error=null, $notUseDefault=null);

	}
}
