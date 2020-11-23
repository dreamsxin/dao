<?php 

namespace Dao\Mvc\User\Logic {

	abstract class Model extends \Dao\Mvc\User\Logic implements \Dao\Events\EventsAwareInterface, \Dao\Di\InjectionAwareInterface {

		abstract public function get($arguments=null);


		abstract public function getAll($arguments=null);


		abstract public function save($arguments=null);


		abstract public function create($arguments=null);


		abstract public function delete($arguments=null);


		abstract public function deleteAll($arguments=null);


		abstract public function update($arguments=null);


		abstract public function updateAll($arguments=null);


		abstract public function count($arguments=null);

	}
}
