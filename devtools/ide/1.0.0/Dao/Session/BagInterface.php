<?php 

namespace Dao\Session {

	interface BagInterface {

		public function initialize();


		public function destroy();


		public function set($property, $value);


		public function get($property, $defaultValue=null);


		public function has($property);


		public function remove($property);

	}
}
