<?php 

namespace Dao\Config {

	interface AdapterInterface {

		public static function setBasePath($basePath);


		public static function getBasePath();

	}
}
