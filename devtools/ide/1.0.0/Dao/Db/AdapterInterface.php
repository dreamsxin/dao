<?php 

namespace Dao\Db {

	/**
	 * Dao\Db\AdapterInterface initializer
	 */
	
	interface AdapterInterface {

		public function fetchOne($sqlQuery, $fetchMode=null, $placeholders=null, $fetchArgument=null, $ctorArgs=null);


		public function fetchAll($sqlQuery, $fetchMode=null, $placeholders=null, $fetchArgument=null, $ctorArgs=null);


		public function insert($table, $values, $fields=null, $dataTypes=null);


		public function update($table, $fields, $values, $whereCondition=null, $dataTypes=null);


		public function delete($table, $whereCondition=null, $placeholders=null, $dataTypes=null);


		public function getColumnList($columnList);


		public function limit($sqlQuery, $number);


		public function tableExists($tableName, $schemaName=null);


		public function viewExists($viewName, $schemaName=null);


		public function forUpdate($sqlQuery);


		public function sharedLock($sqlQuery);


		public function createTable($tableName, $schemaName, $definition);


		public function dropTable($tableName, $schemaName=null, $ifExists=null);


		public function createView($viewName, $definition, $schemaName=null);


		public function dropView($viewName, $schemaName=null, $ifExists=null);


		public function addColumn($tableName, $schemaName, $column);


		public function modifyColumn($tableName, $schemaName, $column);


		public function dropColumn($tableName, $schemaName, $columnName);


		public function addIndex($tableName, $schemaName, $index);


		public function dropIndex($tableName, $schemaName, $indexName);


		public function addPrimaryKey($tableName, $schemaName, $index);


		public function dropPrimaryKey($tableName, $schemaName);


		public function addForeignKey($tableName, $schemaName, $reference);


		public function dropForeignKey($tableName, $schemaName, $referenceName);


		public function getColumnDefinition($column);


		public function listTables($schemaName=null);


		public function listViews($schemaName=null);


		public function getDescriptor();


		public function getConnectionId();


		public function getSQLStatement();


		public function getExpectSQLStatement();


		public function getSQLVariables();


		public function getSQLBindTypes();


		public function getType();


		public function getDialectType();


		public function getDialect();


		public function connect($descriptor=null);


		public function query($sqlStatement, $placeholders=null, $dataTypes=null);


		public function execute($sqlStatement, $placeholders=null, $dataTypes=null);


		public function affectedRows();


		public function close();


		public function escapeIdentifier($identifier);


		public function escapeString($str);


		public function convertBoundParams($sqlStatement, $params);


		public function lastInsertId($sequenceName=null);


		public function begin();


		public function rollback();


		public function commit();


		public function isUnderTransaction();


		public function getInternalHandler();


		public function describeIndexes($table, $schema=null);


		public function describeReferences($table, $schema=null);


		public function tableOptions($tableName, $schemaName=null);


		public function useExplicitIdValue();


		public function getDefaultIdValue();


		public function supportSequences();


		public function createSavepoint($name);


		public function releaseSavepoint($name);


		public function rollbackSavepoint($name);


		public function setNestedTransactionsWithSavepoints($nestedTransactionsWithSavepoints);


		public function isNestedTransactionsWithSavepoints();


		public function getNestedTransactionSavepointName();


		public function describeColumns($table, $schema=null);


		/**
		 * Convert php bytea to database bytea
		 *
		 * @param string $value
		 * @return string
		 */
		public function escapeBytea($value);


		/**
		 * Convert database bytea to php bytea
		 *
		 * @param string $value
		 * @return string
		 */
		public function unescapeBytea($value);


		/**
		 * Convert php array to database array
		 *
		 * @param array $value
		 * @param int $type
		 * @return string
		 */
		public function escapeArray($value, $type=null);


		/**
		 * Convert database array to php array
		 *
		 * @param string $value
		 * @param int $type
		 * @return array
		 */
		public function unescapeArray($value, $type=null);

	}
}
