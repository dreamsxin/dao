<?php 

namespace Dao\Mvc\Model {

	interface CriteriaInterface {

		public function setModelName($modelName);


		public function getModelName();


		public function bind($bindParams, $merge=null);


		public function bindTypes($bindTypes, $merge=null);


		public function columns($columns);


		public function join($model, $conditions=null, $alias=null, $type=null);


		public function where($conditions, $bindParams=null, $bindTypes=null);


		public function andWhere($conditions, $bindParams=null, $bindTypes=null);


		public function orWhere($conditions, $bindParams=null, $bindTypes=null);


		public function betweenWhere($expr, $minimum, $maximum, $useOrWhere=null);


		public function notBetweenWhere($expr, $minimum, $maximum, $useOrWhere=null);


		public function inWhere($expr, $values, $useOrWhere=null);


		public function notInWhere($expr, $values, $useOrWhere=null);


		public function getWhere();


		public function conditions($conditions);


		public function getColumns();


		public function getConditions();


		public function orderBy($orderColumns);


		public function getOrder();


		public function limit($limit, $offset=null);


		public function getLimit();


		public function setUniqueRow($uniqueRow);


		public function getUniqueRow();


		public function forUpdate($forUpdate=null);


		public function sharedLock($sharedLock=null);


		public function getParams();


		public static function fromInput(\Dao\DiInterface $dependencyInjector, $modelName, $data);


		public function execute();


		public function insert();


		public function update();


		public function delete();


		public function getPhql();

	}
}
