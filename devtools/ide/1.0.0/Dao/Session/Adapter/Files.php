<?php 

namespace Dao\Session\Adapter {

	class Files extends \Dao\Session\Adapter implements \ArrayAccess, \Traversable, \IteratorAggregate, \Countable, \Dao\Session\AdapterInterface, \Dao\Di\InjectionAwareInterface, \Dao\Events\EventsAwareInterface {
	}
}
