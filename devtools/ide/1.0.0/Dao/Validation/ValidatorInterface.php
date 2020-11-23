<?php 

namespace Dao\Validation {

	interface ValidatorInterface {

		public function isSetOption($key);


		public function getOption($key);


		public function setOption($key, $value);


		public function validate(\Dao\ValidationInterface $validator, $attribute, $allowEmpty=null);


		public function valid($value);

	}
}
