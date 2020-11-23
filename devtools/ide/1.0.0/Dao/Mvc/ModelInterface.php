<?php 

namespace Dao\Mvc {

	interface ModelInterface {

		public function setTransaction($transaction);


		public function getSource();


		public function getSchema();


		public function getIdentityField();


		public function getColumnMap();


		public function getReverseColumnMap();


		public function getAttributes();


		public function getPrimaryKeyAttributes();


		public function getNonPrimaryKeyAttributes();


		public function getNotNullAttributes();


		public function getDataTypesNumeric();


		public function isNotNull($attribute);


		public function getDataTypes();


		public function getDataSize($attribute);


		public function getDataByte($attribute);


		public function getDataScale($attribute);


		public function getBindTypes();


		public function getDefaultValues();


		public function getAutomaticCreateAttributes();


		public function getAutomaticUpdateAttributes();


		public function hasRealAttribute($column);


		public function getRealAttribute($column);


		public function hasAttribute($attribute);


		public function getAttribute($attribute);


		public function setConnectionService($connectionService);


		public function setWriteConnectionService($connectionService);


		public function setReadConnectionService($connectionService);


		public function getReadConnectionService();


		public function getWriteConnectionService();


		public function getReadConnection();


		public function getWriteConnection();


		public function assign($data, $columnMap=null, $whiteList=null, $negate=null);


		public static function cloneResultMap($base, $data, $columnMap, $dirtyState, $keepSnapshots=null, $sourceModel=null);


		public static function cloneResult($base, $data, $dirtyState=null, $sourceModel=null);


		public static function cloneResultMapHydrate($data, $columnMap, $hydrationMode, $sourceModel=null);


		public static function find($conditions=null, $bindParams=null, $options=null);


		public static function findFirst($conditions=null, $bindParams=null, $options=null, $autoCreate=null);


		public static function query(\Dao\DiInterface $dependencyInjector=null);


		public static function count($parameters=null);


		public static function sum($parameters=null);


		public static function maximum($parameters=null);


		public static function minimum($parameters=null);


		public static function average($parameters=null);


		public function appendMessage($message, $field=null, $type=null, $code=null);


		public function validationHasFailed();


		public function getMessages($filter=null);


		public function save($data=null, $whiteList=null, $exists=null);


		public function create($data=null, $whiteList=null, $existsCheck=null);


		public function update($data=null, $whiteList=null, $existsCheck=null);


		public function delete();


		public function getOperationMade();


		public function refresh();


		public function readAttribute($attribute);


		public function writeAttribute($attribute, $value);


		public function getRelated($alias, $arguments=null);


		public static function remove($parameters);


		public function reset();

	}
}
