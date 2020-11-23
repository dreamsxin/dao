<?php 

namespace Dao\Storage\Lmdb {

	/**
	 * Dao\Storage\Lmdb\Cursor
	 *
	 */
	
	class Cursor implements \Iterator, \Traversable {

		const GET_BOTH = 2;

		const GET_BOTH_RANGE = 3;

		const GET_CURRENT = 4;

		const GET_MULTIPLE = 5;

		const CURRENT = 64;

		const NODUPDATA = 32;

		const NOOVERWRITE = 16;

		const RESERVE = 65536;

		const APPEND = 131072;

		const APPENDDUP = 262144;

		const MULTIPLE = 524288;

		const NEXT = 8;

		const NEXT_DUP = 9;

		const NEXT_MULTIPLE = 10;

		const NEXT_NODUP = 11;

		const PREV = 12;

		const PREV_DUP = 13;

		const PREV_MULTIPLE = 18;

		const PREV_NODUP = 14;

		protected $_frontend;

		/**
		 * \Dao\Storage\Lmdb\Cursor constructor
		 *
		 */
		final private function __construct(){ }


		/**
		 * Retrieve by cursor
		 *
		 * @param string $key
		 * @return string
		 */
		public function retrieve($key){ }


		/**
		 * Return key/data at current cursor position
		 *
		 * @param string $key
		 * @param int $flags
		 * @return mixed
		 */
		public function get($key, $flags=null){ }


		/**
		 * Store by cursor
		 *
		 * @param string $key
		 * @param mixed $value
		 * @param int $flags
		 * @return mixed
		 */
		public function put($key, $value, $flags=null){ }


		/**
		 * Delete current key/data pair
		 *
		 * @param int $flags
		 * @return boolean
		 */
		public function del($flags=null){ }


		/**
		 * Return count of duplicates for current key
		 *
		 * @return int
		 */
		public function count(){ }


		/**
		 * Gets current value
		 *
		 * @return string
		 */
		public function current(){ }


		/**
		 * Gets current key
		 *
		 * @return string
		 */
		public function key(){ }


		/**
		 * Moves cursor to next row
		 *
		 * @param int $flag
		 * @return boolean
		 */
		public function next($flag=null){ }


		/**
		 * Moves cursor to prev row
		 *
		 * @param int $flag
		 * @return boolean
		 */
		public function prev($flag=null){ }


		/**
		 * Rewinds cursor to it's beginning
		 *
		 * @param boolean $dup
		 * @return boolean
		 */
		public function rewind($dup=null){ }


		public function first($dup=null){ }


		/**
		 * Rewinds cursor to it's last
		 *
		 * @param boolean $dup
		 * @return boolean
		 */
		public function last($dup=null){ }


		/**
		 * Checks if current position is valid
		 *
		 * @return boolean
		 */
		public function valid(){ }

	}
}
