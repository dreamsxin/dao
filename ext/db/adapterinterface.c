
/*
  +------------------------------------------------------------------------+
  | Dao Framework                                                          |
  +------------------------------------------------------------------------+
  | Copyright (c) 2011-2014 Phalcon Team (http://www.phalconphp.com)       |
  +------------------------------------------------------------------------+
  | This source file is subject to the New BSD License that is bundled     |
  | with this package in the file docs/LICENSE.txt.                        |
  |                                                                        |
  | If you did not receive a copy of the license and are unable to         |
  | obtain it through the world-wide-web, please send an email             |
  | to license@phalconphp.com so we can send you a copy immediately.       |
  +------------------------------------------------------------------------+
  | Authors: Andres Gutierrez <andres@phalconphp.com>                      |
  |          Eduar Carvajal <eduar@phalconphp.com>                         |
  +------------------------------------------------------------------------+
*/

#include "db/adapterinterface.h"
#include "kernel/main.h"

zend_class_entry *dao_db_adapterinterface_ce;

static const zend_function_entry dao_db_adapterinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, fetchOne, arginfo_dao_db_adapterinterface_fetchone)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, fetchAll, arginfo_dao_db_adapterinterface_fetchall)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, insert, arginfo_dao_db_adapterinterface_insert)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, update, arginfo_dao_db_adapterinterface_update)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, delete, arginfo_dao_db_adapterinterface_delete)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, getColumnList, arginfo_dao_db_adapterinterface_getcolumnlist)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, limit, arginfo_dao_db_adapterinterface_limit)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, tableExists, arginfo_dao_db_adapterinterface_tableexists)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, viewExists, arginfo_dao_db_adapterinterface_viewexists)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, forUpdate, arginfo_dao_db_adapterinterface_forupdate)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, sharedLock, arginfo_dao_db_adapterinterface_sharedlock)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, createTable, arginfo_dao_db_adapterinterface_createtable)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, dropTable, arginfo_dao_db_adapterinterface_droptable)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, createView, arginfo_dao_db_adapterinterface_createview)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, dropView, arginfo_dao_db_adapterinterface_dropview)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, addColumn, arginfo_dao_db_adapterinterface_addcolumn)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, modifyColumn, arginfo_dao_db_adapterinterface_modifycolumn)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, dropColumn, arginfo_dao_db_adapterinterface_dropcolumn)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, addIndex, arginfo_dao_db_adapterinterface_addindex)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, dropIndex, arginfo_dao_db_adapterinterface_dropindex)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, addPrimaryKey, arginfo_dao_db_adapterinterface_addprimarykey)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, dropPrimaryKey, arginfo_dao_db_adapterinterface_dropprimarykey)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, addForeignKey, arginfo_dao_db_adapterinterface_addforeignkey)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, dropForeignKey, arginfo_dao_db_adapterinterface_dropforeignkey)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, getColumnDefinition, arginfo_dao_db_adapterinterface_getcolumndefinition)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, listTables, arginfo_dao_db_adapterinterface_listtables)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, listViews, arginfo_dao_db_adapterinterface_listviews)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, getDescriptor, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, getConnectionId, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, getSQLStatement, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, getExpectSQLStatement, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, getSQLVariables, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, getSQLBindTypes, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, getType, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, getDialectType, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, getDialect, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, connect, arginfo_dao_db_adapterinterface_connect)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, query, arginfo_dao_db_adapterinterface_query)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, execute, arginfo_dao_db_adapterinterface_execute)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, affectedRows, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, close, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, escapeIdentifier, arginfo_dao_db_adapterinterface_escapeidentifier)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, escapeString, arginfo_dao_db_adapterinterface_escapestring)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, convertBoundParams, arginfo_dao_db_adapterinterface_convertboundparams)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, lastInsertId, arginfo_dao_db_adapterinterface_lastinsertid)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, begin, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, rollback, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, commit, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, isUnderTransaction, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, getInternalHandler, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, describeIndexes, arginfo_dao_db_adapterinterface_describeindexes)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, describeReferences, arginfo_dao_db_adapterinterface_describereferences)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, tableOptions, arginfo_dao_db_adapterinterface_tableoptions)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, useExplicitIdValue, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, getDefaultIdValue, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, supportSequences, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, createSavepoint, arginfo_dao_db_adapterinterface_createsavepoint)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, releaseSavepoint, arginfo_dao_db_adapterinterface_releasesavepoint)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, rollbackSavepoint, arginfo_dao_db_adapterinterface_rollbacksavepoint)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, setNestedTransactionsWithSavepoints, arginfo_dao_db_adapterinterface_setnestedtransactionswithsavepoints)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, isNestedTransactionsWithSavepoints, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, getNestedTransactionSavepointName, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, describeColumns, arginfo_dao_db_adapterinterface_describecolumns)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, escapeBytea, arginfo_dao_db_adapterinterface_escapebytea)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, unescapeBytea, arginfo_dao_db_adapterinterface_unescapebytea)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, escapeArray, arginfo_dao_db_adapterinterface_escapearray)
	PHP_ABSTRACT_ME(Dao_Db_AdapterInterface, unescapeArray, arginfo_dao_db_adapterinterface_unescapearray)
	PHP_FE_END
};


/**
 * Dao\Db\AdapterInterface initializer
 */
DAO_INIT_CLASS(Dao_Db_AdapterInterface){

	DAO_REGISTER_INTERFACE(Dao\\Db, AdapterInterface, db_adapterinterface, dao_db_adapterinterface_method_entry);

	return SUCCESS;
}

/**
 * Returns the first row in a SQL query result
 *
 * @param string $sqlQuery
 * @param int $fetchMode
 * @param int $placeholders
 * @return array
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, fetchOne);

/**
 * Dumps the complete result of a query into an array
 *
 * @param string $sqlQuery
 * @param int $fetchMode
 * @param int $placeholders
 * @return array
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, fetchAll);

/**
 * Inserts data into a table using custom RBDM SQL syntax
 *
 * @param 	string $table
 * @param 	array $values
 * @param 	array $fields
 * @param 	array $dataTypes
 * @return 	boolean
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, insert);

/**
 * Updates data on a table using custom RBDM SQL syntax
 *
 * @param 	string $table
 * @param 	array $fields
 * @param 	array $values
 * @param 	string $whereCondition
 * @param 	array $dataTypes
 * @return 	boolean
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, update);

/**
 * Deletes data from a table using custom RBDM SQL syntax
 *
 * @param  string $table
 * @param  string $whereCondition
 * @param  array $placeholders
 * @param  array $dataTypes
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, delete);

/**
 * Gets a list of columns
 *
 * @param array $columnList
 * @return string
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, getColumnList);

/**
 * Appends a LIMIT clause to $sqlQuery argument
 *
 * @param  	string $sqlQuery
 * @param 	int $number
 * @return 	string
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, limit);

/**
 * Generates SQL checking for the existence of a schema.table
 *
 * @param string $tableName
 * @param string $schemaName
 * @return string
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, tableExists);

/**
 * Generates SQL checking for the existence of a schema.view
 *
 * @param string $viewName
 * @param string $schemaName
 * @return string
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, viewExists);

/**
 * Returns a SQL modified with a FOR UPDATE clause
 *
 * @param string $sqlQuery
 * @return string
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, forUpdate);

/**
 * Returns a SQL modified with a LOCK IN SHARE MODE clause
 *
 * @param string $sqlQuery
 * @return string
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, sharedLock);

/**
 * Creates a table
 *
 * @param string $tableName
 * @param string $schemaName
 * @param array $definition
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, createTable);

/**
 * Drops a table from a schema/database
 *
 * @param string $tableName
 * @param   string $schemaName
 * @param boolean $ifExists
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, dropTable);

/**
 * Creates a view
 *
 * @param string $tableName
 * @param array $definition
 * @param string $schemaName
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, createView);

/**
 * Drops a view
 *
 * @param string $viewName
 * @param   string $schemaName
 * @param boolean $ifExists
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, dropView);

/**
 * Adds a column to a table
 *
 * @param string $tableName
 * @param 	string $schemaName
 * @param Dao\Db\ColumnInterface $column
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, addColumn);

/**
 * Modifies a table column based on a definition
 *
 * @param string $tableName
 * @param string $schemaName
 * @param Dao\Db\ColumnInterface $column
 * @return 	boolean
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, modifyColumn);

/**
 * Drops a column from a table
 *
 * @param string $tableName
 * @param string $schemaName
 * @param string $columnName
 * @return 	boolean
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, dropColumn);

/**
 * Adds an index to a table
 *
 * @param string $tableName
 * @param string $schemaName
 * @param Dao\Db\IndexInterface $index
 * @return 	boolean
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, addIndex);

/**
 * Drop an index from a table
 *
 * @param string $tableName
 * @param string $schemaName
 * @param string $indexName
 * @return 	boolean
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, dropIndex);

/**
 * Adds a primary key to a table
 *
 * @param string $tableName
 * @param string $schemaName
 * @param Dao\Db\IndexInterface $index
 * @return 	boolean
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, addPrimaryKey);

/**
 * Drops primary key from a table
 *
 * @param string $tableName
 * @param string $schemaName
 * @return 	boolean
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, dropPrimaryKey);

/**
 * Adds a foreign key to a table
 *
 * @param string $tableName
 * @param string $schemaName
 * @param Dao\Db\ReferenceInterface $reference
 * @return boolean true
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, addForeignKey);

/**
 * Drops a foreign key from a table
 *
 * @param string $tableName
 * @param string $schemaName
 * @param string $referenceName
 * @return boolean true
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, dropForeignKey);

/**
 * Returns the SQL column definition from a column
 *
 * @param Dao\Db\ColumnInterface $column
 * @return string
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, getColumnDefinition);

/**
 * List all tables on a database
 *
 * @param string $schemaName
 * @return array
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, listTables);

/**
 * List all views on a database
 *
 * @param string $schemaName
 * @return array
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, listViews);

/**
 * Return descriptor used to connect to the active database
 *
 * @return array
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, getDescriptor);

/**
 * Gets the active connection unique identifier
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, getConnectionId);

/**
 * Active SQL statement in the object
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, getSQLStatement);

/**
 * Active SQL statement in the object without replace bound paramters
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, getRealSQLStatement);

/**
 * Active SQL statement in the object
 *
 * @return array
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, getSQLVariables);

/**
 * Active SQL statement in the object
 *
 * @return array
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, getSQLBindTypes);

/**
 * Returns type of database system the adapter is used for
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, getType);

/**
 * Returns the name of the dialect used
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, getDialectType);

/**
 * Returns internal dialect instance
 *
 * @return Dao\Db\DialectInterface
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, getDialect);

/**
 * This method is automatically called in Dao\Db\Adapter\Pdo constructor.
 * Call it when you need to restore a database connection
 *
 * @param 	array $descriptor
 * @return 	boolean
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, connect);

/**
 * Sends SQL statements to the database server returning the success state.
 * Use this method only when the SQL statement sent to the server return rows
 *
 * @param  string $sqlStatement
 * @param  array $placeholders
 * @param  array $dataTypes
 * @return Dao\Db\ResultInterface
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, query);

/**
 * Sends SQL statements to the database server returning the success state.
 * Use this method only when the SQL statement sent to the server don't return any row
 *
 * @param  string $sqlStatement
 * @param  array $placeholders
 * @param  array $dataTypes
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, execute);

/**
 * Returns the number of affected rows by the last INSERT/UPDATE/DELETE reported by the database system
 *
 * @return int
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, affectedRows);

/**
 * Closes active connection returning success. Dao automatically closes and destroys active connections within Dao\Db\Pool
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, close);

/**
 * Escapes a column/table/schema name
 *
 * @param string $identifier
 * @return string
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, escapeIdentifier);

/**
 * Escapes a value to avoid SQL injections
 *
 * @param string $str
 * @return string
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, escapeString);

/**
 * Converts bound params like :name: or ?1 into ? bind params
 *
 * @param string $sqlStatement
 * @param array $params
 * @return array
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, convertBoundParams);

/**
 * Returns insert id for the auto_increment column inserted in the last SQL statement
 *
 * @param string $sequenceName
 * @return int
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, lastInsertId);

/**
 * Starts a transaction in the connection
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, begin);

/**
 * Rollbacks the active transaction in the connection
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, rollback);

/**
 * Commits the active transaction in the connection
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, commit);

/**
 * Checks whether connection is under database transaction
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, isUnderTransaction);

/**
 * Return internal PDO handler
 *
 * @return \PDO
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, getInternalHandler);

/**
 * Lists table indexes
 *
 * @param string $table
 * @param string $schema
 * @return Dao\Db\IndexInterface[]
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, describeIndexes);

/**
 * Lists table references
 *
 * @param string $table
 * @param string $schema
 * @return Dao\Db\ReferenceInterface[]
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, describeReferences);

/**
 * Gets creation options from a table
 *
 * @param string $tableName
 * @param string $schemaName
 * @return array
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, tableOptions);

/**
 * Check whether the database system requires an explicit value for identity columns
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, useExplicitIdValue);

/**
 * Return the default identity value to insert in an identity column
 *
 * @return Dao\Db\RawValue
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, getDefaultIdValue);

/**
 * Check whether the database system requires a sequence to produce auto-numeric values
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, supportSequences);

/**
 * Creates a new savepoint
 *
 * @param string $name
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, createSavepoint);

/**
 * Releases given savepoint
 *
 * @param string $name
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, releaseSavepoint);

/**
 * Rollbacks given savepoint
 *
 * @param string $name
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, rollbackSavepoint);

/**
 * Set if nested transactions should use savepoints
 *
 * @param boolean $nestedTransactionsWithSavepoints
 * @return Dao\Db\AdapterInterface
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, setNestedTransactionsWithSavepoints);

/**
 * Returns if nested transactions should use savepoints
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, isNestedTransactionsWithSavepoints);

/**
 * Returns the savepoint name to use for nested transactions
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, getNestedTransactionSavepointName);

/**
 * Returns an array of Dao\Db\Column objects describing a table
 *
 * @param string $table
 * @param string $schema
 * @return Dao\Db\ColumnInterface[]
 */
DAO_DOC_METHOD(Dao_Db_AdapterInterface, describeColumns);


/**
 * Convert php bytea to database bytea
 *
 * @param string $value
 * @return string
 */
PHP_METHOD(Dao_Db_AdapterInterface, escapeBytea);

/**
 * Convert database bytea to php bytea
 *
 * @param string $value
 * @return string
 */
PHP_METHOD(Dao_Db_AdapterInterface, unescapeBytea);

/**
 * Convert php array to database array
 *
 * @param array $value
 * @param int $type
 * @return string
 */
PHP_METHOD(Dao_Db_AdapterInterface, escapeArray);

/**
 * Convert database array to php array
 *
 * @param string $value
 * @param int $type
 * @return array
 */
PHP_METHOD(Dao_Db_AdapterInterface, unescapeArray);