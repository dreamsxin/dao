<?php 

namespace Dao {

	/**
	 * Dao\Arr
	 *
	 * Provides utilities to work with arrs
	 */
	
	class Arr {

		const TYPE_BOOLEAN = 1;

		const TYPE_LONG = 2;

		const TYPE_DOUBLE = 3;

		const TYPE_STRING = 4;

		const AGGR_SUM = 5;

		const AGGR_AVG = 4;

		const AGGR_COUNT = 3;

		const AGGR_FIRST = 1;

		const AGGR_LAST = 2;

		const AGGR_MIN = 7;

		const AGGR_MAX = 8;

		const AGGR_GROUP = 6;

		public static $delimiter;

		/**
		 * Tests if an array is associative or not.
		 *
		 *<code>
		 *	// Returns TRUE
		 *	\Dao\Arr::is_assoc(array('username' => 'john.doe'))
		 *</code>
		 *
		 * @param array $array
		 * @return boolean
		 */
		public static function is_assoc($array){ }


		/**
		 * Test if a value is an array with an additional check for array-like objects.
		 *
		 *<code>
		 *	// Returns TRUE
		 *	\Dao\Arr::is_array(array());
		 *</code>
		 *
		 * @param mixed $value
		 * @return boolean
		 */
		public static function is_array($array){ }


		/**
		 * Gets a value from an array using a dot separated path.
		 *
		 *<code>
		 *	// Get the value of $array['foo']['bar']
		 *	$value = \Dao\Arr::path($array, 'foo.bar');
		 *</code>
		 *
		 * Using a wildcard "*" will search intermediate arrays and return an array.
		 *
		 *<code>
		 *	// Get the values of "color" in theme
		 *	$colors = \Dao\Arr::path($array, 'theme.*.color');
		 *
		 *	// Using an array of keys
		 *	$colors = \Dao\Arr::path($array, array('theme', '*', 'color'));
		 *</code>
		 *
		 * @param array $array
		 * @param mixed $path
		 * @param mixed $default
		 * @param string $delimiter
		 * @return mixed
		 */
		public static function path($array, $path, $default_value=null, $delimiter=null){ }


		/**
		 * Set a value on an array by path.
		 *
		 * Using a wildcard "*" will search intermediate arrays and return an array.
		 *
		 *<code>
		 *	// Set the values of "color" in theme
		 *	$array = array('theme' => array('one' => array('color' => 'green'), 'two' => array('size' => 11));
		 *	\Dao\Arr::set_path($array, 'theme.*.color', 'red');
		 *	// Result: array('theme' => array('one' => array('color' => 'red'), 'two' => array('size' => 11, 'color' => 'red'));
		 *</code>
		 *
		 * @param array $array
		 * @param string $path
		 * @param mixed $value
		 * @param string $delimiter
		 */
		public static function set_path($array, $path, $value, $delimiter=null, $flag=null){ }


		/**
		 * Fill an array with a range of numbers.
		 *
		 *<code>
		 *	// Fill an array with values 5, 10, 15, 20
		 *	$values = \Dao\Arr::range(5, 20);
		 *</code>
		 *
		 * @param integer $step
		 * @param integer $max
		 * @return array
		 */
		public static function range($step=null, $max=null){ }


		/**
		 * Retrieve a single key from an array. If the key does not exist in the
		 * array, the default value will be returned instead.
		 *
		 *<code>
		 *	// Get the value "username" from $_POST, if it exists
		 *	$username = \Dao\Arr::get($_POST, 'username');
		 *</code>
		 *
		 * @param array|object $array
		 * @param string|array|\Closure $key
		 * @param mixed $default_value
		 * @return mixed
		 */
		public static function get($array, $key, $default_value=null){ }


		/**
		 * Retrieve a single key from an array, if value an array return the first element.
		 *
		 * @param array $array
		 * @param string|array|\Closure $key
		 * @param mixed $default_value
		 * @return mixed
		 */
		public static function first($array, $key, $default_value=null){ }


		/**
		 * Choice one value, If the key does not exist in the array, the value2 will be returned instead.
		 *
		 *	// Choice the "value1", if exists the value "email" of $_POST
		 *	$username = \Dao\Arr::choice($_POST, 'email', 'value1', 'value2');
		 *
		 * @param array $array
		 * @param string $key
		 * @param string $value1
		 * @param string $value2
		 * @return mixed
		 */
		public static function choice($array, $key, $value1, $value2=null){ }


		/**
		 * Retrieves multiple paths from an array. If the path does not exist in the
		 * array, the default value will be added instead.
		 *
		 *	// Get the values "username", "password" from $_POST
		 *	$auth = \Dao\Arr::extract($_POST, array('username', 'password'));
		 *
		 *	// Get the value "level1.level2a" from $data
		 *	$data = array('level1' => array('level2a' => 'value 1', 'level2b' => 'value 2'));
		 *	\Dao\Arr::extract($data, array('level1.level2a', 'password'));
		 *
		 * @param array $array
		 * @param array $paths
		 * @param mixed $default_value
		 * @return array
		 */
		public static function extract($array, $paths, $default_value=null){ }


		/**
		 * Retrieves muliple single-key values from a list of arrays.
		 *
		 *	// Get all of the "id" values from a result
		 *	$ids = \Dao\Arr::pluck($result, 'id');
		 *
		 * @param array $array
		 * @param string $key
		 * @return array
		 */
		public static function pluck($array, $key){ }


		/**
		 * Adds a value to the beginning of an associative array.
		 *
		 *	// Add an empty value to the start of a select list
		 *	\Dao\Arr::unshift($array, 'none', 'Select a value');
		 *
		 * @param array $array
		 * @param string $key
		 * @param mixed $val
		 * @return array
		 */
		public static function unshift($array, $key, $val){ }


		/**
		 * Recursive version of [array_map](http://php.net/array_map), applies one or more
		 * callbacks to all elements in an array, including sub-arrays.
		 *
		 *	// Apply "strip_tags" to every element in the array
		 *	$array = \Dao\Arr::map($array, 'strip_tags');
		 *
		 *	// Apply $this->filter to every element in the array
		 *	$array = \Dao\Arr::map($array, array(array($this,'filter')));
		 *
		 * @param array $array
		 * @param mixed $callbacks
		 * @return array
		 */
		public static function map($array, $callbacks, $keys=null){ }


		/**
		 * Recursively merge two or more arrays. Values in an associative array
		 * overwrite previous values with the same key. Values in an indexed array
		 * are appended, but only when they do not already exist in the result.
		 *
		 * Note that this does not work the same as [array_merge_recursive](http://php.net/array_merge_recursive)!
		 *
		 *	$john = array('name' => 'john', 'children' => array('fred', 'paul', 'sally', 'jane'));
		 *	$mary = array('name' => 'mary', 'children' => array('jane'));
		 *
		 *	// John and Mary are married, merge them together
		 *	$john = \Dao\Arr::merge($john, $mary);
		 *
		 *	// The output of $john will now be:
		 *	array('name' => 'mary', 'children' => array('fred', 'paul', 'sally', 'jane'))
		 *
		 * @param array $array1
		 * @param array $array2,...
		 * @return array
		 */
		public static function merge($array1, $array2, $arrays=null){ }


		/**
		 * Overwrites an array with values from input arrays.
		 * Keys that do not exist in the first array will not be added!
		 *
		 *	$a1 = array('name' => 'john', 'mood' => 'happy', 'food' => 'bacon');
		 *	$a2 = array('name' => 'jack', 'food' => 'tacos', 'drink' => 'beer');
		 *
		 *	// Overwrite the values of $a1 with $a2
		 *	$array = \Dao\Arr::overwrite($a1, $a2);
		 *
		 *	// The output of $array will now be:
		 *	array('name' => 'jack', 'mood' => 'happy', 'food' => 'tacos')
		 *
		 * @param array $array1
		 * @param array $array2
		 * @return array
		 */
		public static function overwrite($array1, $array2, $arrays=null){ }


		/**
		 * Creates a callable function and parameter list from a string representation.
		 * Note that this function does not validate the callback string.
		 *
		 *	// Get the callback function and parameters
		 *	list($func, $params) = \Dao\Arr::callback('Foo::bar(apple,orange)');
		 *
		 *	// Get the result of the callback
		 *	$result = call_user_func_array($func, $params);
		 *
		 * @param string $str
		 * @return array function, params
		 */
		public static function callback($str){ }


		/**
		 * Convert a multi-dimensional array into a single-dimensional array.
		 *
		 *	$array = array('set' => array('one' => 'something'), 'two' => 'other');
		 *
		 *	// Flatten the array
		 *	$array = \Dao\Arr::flatten($array);
		 *
		 *	// The array will now be
		 *	array('one' => 'something', 'two' => 'other');
		 *
		 * @param array $array
		 * @return array
		 */
		public static function flatten($array){ }


		/**
		 * Convert a array to a array object.
		 *
		 *	$array = array('name' => 'Dao7', 'version' => '1.0.x');
		 *
		 *	$arrayobject = \Dao\Arr::arrayobject($array);
		 *
		 * @param array $array
		 * @return ArrayObject
		 */
		public static function arrayobject($array){ }


		/**
		 * Gets array key of the postion
		 *
		 *	$array = array('name' => 'Dao7', 'version' => '1.0.x');
		 *
		 *	$key = \Dao\Arr::key($array, 1);
		 *
		 * @param array $array
		 * @param int $postion
		 * @return mixed
		 */
		public static function key($array, $postion=null){ }


		/**
		 * Filters elements of an array using a the filter
		 *
		 *	$array = array('name' => 'Dao7', 'version' => '1.0.x');
		 *
		 *	$key = \Dao\Arr::filter($array, 'int');
		 *
		 * @param array $array
		 * @param mixed $filters
		 * @return array
		 */
		public static function filter($array, $callback=null, $strict=null, $allowEmpty=null){ }


		/**
		 * Return the sum of all the values in the array using a dot separated path
		 *
		 * @param array $array
		 * @param mixed $path
		 * @param mixed $default
		 * @param string $delimiter
		 * @return number
		 */
		public static function sum($array, $path=null){ }


		/**
		 * Converts an object or an array of objects into an array
		 *
		 *<code>
		 *	print_r(Dao\Arr::toArray($user);
		 *</code>
		 *
		 * @param object|array|string $object
		 * @param array $properties
		 * @param bool $recursive
		 * @return array
		 */
		public static function toArray($object, $properties=null, $recursive=null, $negate=null){ }


		/**
		 * Aggregating
		 *
		 * @param array $rows
		 * @param array $aggregators
		 * @return array
		 */
		public static function aggr($rows, $aggregators){ }


		/**
		 * Aggregating
		 *
		 * @param array $rows
		 * @param array $fields
		 * @param array $aggregators
		 * @return array
		 */
		public static function group($rows, $fields, $aggregators){ }


		/**
		 * Exchanges all keys with their associated values in an array
		 *
		 * @param array $array
		 * @param string $key
		 * @param string $value
		 * @return array
		 */
		public static function flip($array, $key, $value=null){ }


		/**
		 * Gets the postion key of an array
		 *
		 * @param array $array
		 * @param int $pos
		 * @return mixed
		 */
		public static function getHashKey($array, $pos=null){ }


		/**
		 * Gets the postion value of an array
		 *
		 * @param array $array
		 * @param int $pos
		 * @return mixed
		 */
		public static function getHashValue($array, $pos=null){ }


		/**
		 * Sort an array
		 * 
		 *<code>
		 * $arr = [
		 * 	'2x' => ['sort' => 3],
		 * 	'1x' =>['sort' => 4],
		 * 	'3x' =>['sort' => 1],
		 * 	'4x' =>['sort' => 2],
		 * ];
		 * \Dao\Arr::sort($arr, '*.sort'));
		 *</code>
		 * 
		 * @param array $array
		 * @param string|array $field
		 * @param int $order
		 * @param int $flag
		 * @return bool
		 */
		public static function sort($array, $sort_field, $sort_order=null, $sort_flags=null){ }

	}
}
