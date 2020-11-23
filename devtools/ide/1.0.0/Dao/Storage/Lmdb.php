<?php 

namespace Dao\Storage {

	/**
	 * Dao\Storage\Lmdb
	 *
	 */
	
	class Lmdb {

		const FIXEDMAP = 1;

		const NOSUBDIR = 16384;

		const NOSYNC = 65536;

		const RDONLY = 131072;

		const NOMETASYNC = 262144;

		const WRITEMAP = 524288;

		const MAPASYNC = 1048576;

		const NOTLS = 2097152;

		const NOLOCK = 4194304;

		const NORDAHEAD = 8388608;

		const NOMEMINIT = 16777216;

		const MDB_PREVSNAPSHOT = 33554432;

		const REVERSEKEY = 2;

		const DUPSORT = 4;

		const INTEGERKEY = 8;

		const DUPFIXED = 16;

		const INTEGERDUP = 32;

		const REVERSEDUP = 64;

		const CREATE = 262144;

		const NOOVERWRITE = 16;

		const NODUPDATA = 32;

		const RESERVE = 65536;

		const APPEND = 131072;

		const APPENDDUP = 262144;

		const CP_COMPACT = 1;

		const SUCCESS = 0;

		const KEYEXIST = -30799;

		const NOTFOUND = -30798;

		const PAGE_NOTFOUND = -30797;

		const CORRUPTED = -30796;

		const PANIC = -30795;

		const VERSION_MISMATCH = -30794;

		const INVALID = -30793;

		const MAP_FULL = -30792;

		const DBS_FULL = -30791;

		const READERS_FULL = -30790;

		const TLS_FULL = -30789;

		const TXN_FULL = -30788;

		const CURSOR_FULL = -30787;

		const PAGE_FULL = -30786;

		const MAP_RESIZED = -30785;

		const INCOMPATIBLE = -30784;

		const BAD_RSLOT = -30783;

		const BAD_TXN = -30782;

		const BAD_VALSIZE = -30781;

		const BAD_DBI = -30780;

		const PROBLEM = -30779;

		const LAST_ERRCODE = -30779;

		protected $_frontend;

		protected $_path;

		protected $_name;

		protected $_flag;

		/**
		 * \Dao\Storage\Lmdb constructor
		 *
		 * @param string $path
		 * @param string $name
		 * @param int $readers
		 * @param int $mapsize
		 * @param int $envflags
		 * @param int $flags
		 * @param \Dao\Storage\FrontendInterface $frontend
		 */
		public function __construct($path, $name=null, $readers=null, $mapsize=null, $envflags=null, $flags=null, $frontend=null){ }


		/**
		 * Create a transaction for use with the environment
		 *
		 * @param int $flags
		 */
		public function begin($flags=null){ }


		/**
		 * Commit all the operations of a transaction into the database
		 *
		 */
		public function commit(){ }


		/**
		 * Renew a read-only transaction
		 *
		 */
		public function renew(){ }


		/**
		 * Reset a read-only transaction
		 *
		 */
		public function reset(){ }


		/**
		 * Get all items from a database
		 *
		 * @return array
		 */
		public function getAll($dup=null){ }


		/**
		 * Get items from a database
		 *
		 * @param string $key
		 * @return string
		 */
		public function get($key){ }


		/**
		 * Store items into a database
		 *
		 * @param string $key
		 * @param mixed $value
		 * @param int $flags
		 * @return mixed
		 */
		public function put($key, $value, $flags=null){ }


		/**
		 * Delete items from a database
		 *
		 * @param string $keys
		 * @return boolean
		 */
		public function del($key){ }


		/**
		 * Create a cursor handle
		 *
		 * @return \Dao\Storage\Lmdb\Cursor
		 */
		public function cursor(){ }


		/**
		 * Copy an LMDB environment to the specified path
		 *
		 * @param string $path
		 * @param int $flags
		 * @return boolean
		 */
		public function copy($path, $flags=null){ }


		/**
		 * Empty or delete+close a database
		 *
		 * @return boolean
		 */
		public function drop($delete=null){ }


		public function set($key, $value, $flags=null){ }


		public function delete($key){ }

	}
}
