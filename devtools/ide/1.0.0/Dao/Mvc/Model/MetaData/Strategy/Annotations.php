<?php 

namespace Dao\Mvc\Model\MetaData\Strategy {

	/**
	 * Dao\Mvc\Model\MetaData\Strategy\Annotations
	 *
	 * Queries the table meta-data in order to instrospect the model's metadata
	 */
	
	class Annotations {

		protected $_columnMap;

		/**
		 * The meta-data is obtained by reading the column descriptions from the database information schema
		 *
		 * @param \Dao\Mvc\ModelInterface $model
		 * @param \Dao\DiInterface $dependencyInjector
		 * @return array
		 */
		public function getMetaData($model, $dependencyInjector){ }


		/**
		 * Read the model's column map, this can't be inferred
		 *
		 * @param \Dao\Mvc\ModelInterface $model
		 * @param \Dao\DiInterface $dependencyInjector
		 * @return array
		 */
		public function getColumnMaps(){ }

	}
}
