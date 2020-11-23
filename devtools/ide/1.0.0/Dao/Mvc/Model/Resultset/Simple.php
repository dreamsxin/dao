<?php 

namespace Dao\Mvc\Model\Resultset {

	/**
	 * Dao\Mvc\Model\Resultset\Simple
	 *
	 * Simple resultsets only contains complete objects.
	 * This class builds every complete object as it is required
	 */
	
	class Simple extends \Dao\Mvc\Model\Resultset implements \JsonSerializable, \Serializable, \ArrayAccess, \Countable, \SeekableIterator, \Traversable, \Iterator, \Dao\Mvc\Model\ResultsetInterface, \Dao\Di\InjectionAwareInterface, \Dao\Events\EventsAwareInterface {

		const TYPE_RESULT_FULL = 0;

		const TYPE_RESULT_PARTIAL = 1;

		const HYDRATE_RECORDS = 0;

		const HYDRATE_OBJECTS = 2;

		const HYDRATE_ARRAYS = 1;

		protected $_sourceModel;

		protected $_model;

		protected $_columnMap;

		protected $_rowsModels;

		protected $_rowsObjects;

		/**
		 * \Dao\Mvc\Model\Resultset\Simple constructor
		 *
		 * @param array $columnMap
		 * @param \Dao\Mvc\ModelInterface $model
		 * @param \Dao\Db\Result\Pdo $result
		 * @param \Dao\Cache\BackendInterface $cache
		 */
		public function __construct($columnMap, $model, $result, $cache=null, $sourceModel=null){ }


		/**
		 * Check whether the internal resource has rows to fetch
		 *
		 * @return boolean
		 */
		public function valid(){ }


		/**
		 * Returns a complete resultset as an array, if the resultset has a big number of rows
		 * it could consume more memory than it currently does. Exporting the resultset to an array
		 * couldn't be faster with a large number of records
		 *
		 * @param array $columns
		 * @param boolean $mustColumn
		 * @return array
		 */
		public function toArray($columns=null, $mustColumn=null){ }


		/**
		 * Serializing a resultset will dump all related rows into a big array
		 *
		 * @return string
		 */
		public function serialize(){ }


		/**
		 * Unserializing a resultset only works on the rows present in the saved state
		 *
		 * @param string $data
		 */
		public function unserialize($serialized=null){ }

	}
}
