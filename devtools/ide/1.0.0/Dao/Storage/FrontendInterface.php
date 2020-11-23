<?php 

namespace Dao\Storage {

	interface FrontendInterface {

		public function beforeStore($data);


		public function afterRetrieve($data);

	}
}
