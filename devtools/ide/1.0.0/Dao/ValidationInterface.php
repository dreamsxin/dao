<?php 

namespace Dao {

	interface ValidationInterface {

		public function validate($data, $entity=null);


		public function add($attribute, $validator);


		public function getLabel($field);


		public function getValue($attribute, $entity=null);


		public function appendMessage(\Dao\Validation\MessageInterface $message);


		public function getMessages();

	}
}
