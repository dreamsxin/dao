<?php 

namespace Dao {

	/**
	 * Dao\Registry
	 *
	 * A registry is a container for storing objects and values in the application space.
	 * By storing the value in a registry, the same object is always available throughout
	 * your application.
	 *
	 * <code>
	 * 	$registry = new \Dao\Registry();
	 *
	 * 	// Set value
	 * 	$registry->something = 'something';
	 * 	// or
	 * 	$registry['something'] = 'something';
	 *
	 * 	// Get value
	 * 	$value = $registry->something;
	 * 	// or
	 * 	$value = $registry['something'];
	 *
	 * 	// Check if the key exists
	 * 	$exists = isset($registry->something);
	 * 	// or
	 * 	$exists = isset($registry['something']);
	 *
	 * 	// Unset
	 * 	unset($registry->something);
	 * 	// or
	 * 	unset($registry['something']);
	 * </code>
	 *
	 * In addition to ArrayAccess, Dao\Registry also implements Countable
	 * (count($registry) will return the number of elements in the registry),
	 * Serializable and Iterator (you can iterate over the registry
	 * using a foreach loop) interfaces. For PHP 5.4 and higher, JsonSerializable
	 * interface is implemented.
	 *
	 * Dao\Registry is very fast (it is typically faster than any userspace
	 * implementation of the registry); however, this comes at a price:
	 * Dao\Registry is a final class and cannot be inherited from.
	 *
	 * Though Dao\Registry exposes methods like __get(), offsetGet(), count() etc,
	 * it is not recommended to invoke them manually (these methods exist mainly to
	 * match the interfaces the registry implements): $registry->__get('property')
	 * is several times slower than $registry->property.
	 *
	 * Internally all the magic methods (and interfaces except JsonSerializable)
	 * are implemented using object handlers or similar techniques: this allows
	 * to bypass relatively slow method calls.
	 */
	
	class Registry implements \ArrayAccess, \Iterator, \Traversable, \Serializable, \Countable {

		protected $_data;

		/**
		 * \Dao\Di constructor
		 *
		 */
		public function __construct(){ }


		/**
		 * Returns an index in the registry
		 */
		public function __get($property){ }


		/**
		 * Sets an element in the registry
		 */
		public function __set($property, $value){ }


		public function __isset($property){ }


		public function __unset($property){ }


		/**
		 * @brief void \Dao\Registry::__call(string $name, array $arguments)
		 */
		public function __call($method, $arguments=null){ }


		/**
		 * @brief int \Dao\Registry::count()
		 */
		public function count(){ }


		/**
		 * @brief mixed& \Dao\Registry::offsetGet(mixed $offset)
		 */
		public function offsetGet($property){ }


		/**
		 * @brief void \Dao\Registry::offsetSet(mixed $offset, mixed $value)
		 */
		public function offsetSet($property, $value){ }


		/**
		 * @brief void \Dao\Registry::offsetUnset(mixed $offset)
		 */
		public function offsetUnset($property){ }


		/**
		 * @brief void \Dao\Registry::offsetExists(mixed $offset)
		 */
		public function offsetExists($property){ }


		/**
		 * @brief mixed& \Dao\Registry::current()
		 */
		public function current(){ }


		/**
		 * @brief string|int|null \Dao\Registry::key()
		 */
		public function key(){ }


		/**
		 * @brief void \Dao\Registry::next()
		 */
		public function next(){ }


		/**
		 * @brief void \Dao\Registry::rewind()
		 */
		public function rewind(){ }


		/**
		 * @brief bool \Dao\Registry::valid()
		 */
		public function valid(){ }


		/**
		 * @brief string|null \Dao\Registry::serialize()
		 */
		public function serialize(){ }


		/**
		 * @brief \Dao\Registry \Dao\Registry::unserialize(string $str)
		 */
		public function unserialize($serialized=null){ }


		/**
		 * @brief void \Dao\Registry::__wakeup()
		 */
		public function __wakeup(){ }

	}
}
