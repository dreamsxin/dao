
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
  |          Rack Lin <racklin@gmail.com>                                  |
  |          Vladimir Kolesnikov <vladimir@extrememember.com>              |
  +------------------------------------------------------------------------+
*/

#include "db/dialect/sqlite.h"
#include "db/dialect.h"
#include "db/dialectinterface.h"
#include "db/column.h"
#include "db/columninterface.h"
#include "db/indexinterface.h"
#include "db/exception.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/operators.h"
#include "kernel/exception.h"
#include "kernel/fcall.h"
#include "kernel/concat.h"
#include "kernel/array.h"
#include "kernel/string.h"

/**
 * Dao\Db\Dialect\Sqlite
 *
 * Generates database specific SQL for the SQLite RDBMS
 */
zend_class_entry *dao_db_dialect_sqlite_ce;

PHP_METHOD(Dao_Db_Dialect_Sqlite, getColumnDefinition);
PHP_METHOD(Dao_Db_Dialect_Sqlite, addColumn);
PHP_METHOD(Dao_Db_Dialect_Sqlite, modifyColumn);
PHP_METHOD(Dao_Db_Dialect_Sqlite, dropColumn);
PHP_METHOD(Dao_Db_Dialect_Sqlite, addIndex);
PHP_METHOD(Dao_Db_Dialect_Sqlite, dropIndex);
PHP_METHOD(Dao_Db_Dialect_Sqlite, addPrimaryKey);
PHP_METHOD(Dao_Db_Dialect_Sqlite, dropPrimaryKey);
PHP_METHOD(Dao_Db_Dialect_Sqlite, addForeignKey);
PHP_METHOD(Dao_Db_Dialect_Sqlite, dropForeignKey);
PHP_METHOD(Dao_Db_Dialect_Sqlite, _getTableOptions);
PHP_METHOD(Dao_Db_Dialect_Sqlite, createTable);
PHP_METHOD(Dao_Db_Dialect_Sqlite, dropTable);
PHP_METHOD(Dao_Db_Dialect_Sqlite, createView);
PHP_METHOD(Dao_Db_Dialect_Sqlite, dropView);
PHP_METHOD(Dao_Db_Dialect_Sqlite, tableExists);
PHP_METHOD(Dao_Db_Dialect_Sqlite, viewExists);
PHP_METHOD(Dao_Db_Dialect_Sqlite, describeColumns);
PHP_METHOD(Dao_Db_Dialect_Sqlite, listTables);
PHP_METHOD(Dao_Db_Dialect_Sqlite, listViews);
PHP_METHOD(Dao_Db_Dialect_Sqlite, describeIndexes);
PHP_METHOD(Dao_Db_Dialect_Sqlite, describeIndex);
PHP_METHOD(Dao_Db_Dialect_Sqlite, describeReferences);
PHP_METHOD(Dao_Db_Dialect_Sqlite, tableOptions);
PHP_METHOD(Dao_Db_Dialect_Sqlite, getDefaultValue);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_db_dialect_sqlite_describeindex, 0, 0, 1)
	ZEND_ARG_INFO(0, indexName)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_db_dialect_sqlite_method_entry[] = {
	PHP_ME(Dao_Db_Dialect_Sqlite, getColumnDefinition, arginfo_dao_db_dialectinterface_getcolumndefinition, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect_Sqlite, addColumn, arginfo_dao_db_dialectinterface_addcolumn, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect_Sqlite, modifyColumn, arginfo_dao_db_dialectinterface_modifycolumn, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect_Sqlite, dropColumn, arginfo_dao_db_dialectinterface_dropcolumn, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect_Sqlite, addIndex, arginfo_dao_db_dialectinterface_addindex, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect_Sqlite, dropIndex, arginfo_dao_db_dialectinterface_dropindex, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect_Sqlite, addPrimaryKey, arginfo_dao_db_dialectinterface_addprimarykey, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect_Sqlite, dropPrimaryKey, arginfo_dao_db_dialectinterface_dropprimarykey, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect_Sqlite, addForeignKey, arginfo_dao_db_dialectinterface_addforeignkey, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect_Sqlite, dropForeignKey, arginfo_dao_db_dialectinterface_dropforeignkey, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect_Sqlite, _getTableOptions, arginfo_empty, ZEND_ACC_PROTECTED)
	PHP_ME(Dao_Db_Dialect_Sqlite, createTable, arginfo_dao_db_dialectinterface_createtable, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect_Sqlite, dropTable, arginfo_dao_db_dialectinterface_droptable, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect_Sqlite, createView, arginfo_dao_db_dialectinterface_createview, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect_Sqlite, dropView, arginfo_dao_db_dialectinterface_dropview, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect_Sqlite, tableExists, arginfo_dao_db_dialectinterface_tableexists, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect_Sqlite, viewExists, arginfo_dao_db_dialectinterface_viewexists, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect_Sqlite, describeColumns, arginfo_dao_db_dialectinterface_describecolumns, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect_Sqlite, listTables, arginfo_dao_db_dialectinterface_listtables, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect_Sqlite, listViews, arginfo_dao_db_dialectinterface_listtables, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect_Sqlite, describeIndexes, arginfo_dao_db_dialectinterface_describeindexes, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect_Sqlite, describeIndex, arginfo_dao_db_dialect_sqlite_describeindex, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect_Sqlite, describeReferences, arginfo_dao_db_dialectinterface_describereferences, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect_Sqlite, tableOptions, arginfo_dao_db_dialectinterface_tableoptions, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect_Sqlite, getDefaultValue, arginfo_dao_db_dialectinterface_getdefaultvalue, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Db\Dialect\Sqlite initializer
 */
DAO_INIT_CLASS(Dao_Db_Dialect_Sqlite){

	DAO_REGISTER_CLASS_EX(Dao\\Db\\Dialect, Sqlite, db_dialect_sqlite, dao_db_dialect_ce, dao_db_dialect_sqlite_method_entry, 0);

	zend_declare_property_string(dao_db_dialect_sqlite_ce, SL("_escapeChar"), "\"", ZEND_ACC_PROTECTED);

	zend_class_implements(dao_db_dialect_sqlite_ce, 1, dao_db_dialectinterface_ce);

	return SUCCESS;
}

/**
 * Gets the column name in Sqlite
 *
 * @param Dao\Db\ColumnInterface $column
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect_Sqlite, getColumnDefinition){

	zval *column, size = {}, column_type = {}, column_sql = {}, type_values = {}, slash = {}, *value, value_cslashes = {}, is_unsigned = {}, scale = {}, name = {};
	int c, i = 0;

	dao_fetch_params(0, 1, 0, &column);

	if (Z_TYPE_P(column) != IS_OBJECT) {
		DAO_THROW_EXCEPTION_STR(dao_db_exception_ce, "Column definition must be an instance of Dao\\Db\\Column");
		return;
	}

	DAO_CALL_METHOD(&size, column, "getsize");
	DAO_CALL_METHOD(&column_type, column, "gettype");

	if (Z_TYPE(column_type) == IS_STRING) {
		DAO_ZVAL_DUP(&column_sql, &column_type);
		DAO_CALL_METHOD(&type_values, column, "gettypevalues");
		if (DAO_IS_NOT_EMPTY(&type_values)) {
			ZVAL_STRING(&slash, "\"");
			if (Z_TYPE(type_values) == IS_ARRAY) {
				c = dao_fast_count_int(&type_values);
				dao_concat_self_str(&column_sql, SL("("));
				ZEND_HASH_FOREACH_VAL(Z_ARRVAL(type_values), value) {
					i++;
					DAO_CALL_FUNCTION(&value_cslashes, "addcslashes", value, &slash);
					if (i < c) {
						DAO_SCONCAT_SVS(&column_sql, "\"", &value_cslashes, "\", ");
					} else {
						DAO_SCONCAT_SVS(&column_sql, "\"", &value_cslashes, "\"");
					}
				} ZEND_HASH_FOREACH_END();
				dao_concat_self_str(&column_sql, SL(")"));
			} else {
				DAO_CALL_FUNCTION(&value_cslashes, "addcslashes", &type_values, &slash);
				DAO_SCONCAT_SVS(&column_sql, "(\"", &value_cslashes, "\")");
			}
			RETURN_CTOR(&column_sql);
		}

		DAO_CALL_METHOD(&column_type, column, "gettypereference");
		switch (dao_get_intval(&column_type)) {

			case DAO_DB_COLUMN_TYPE_INTEGER:
				break;

			case DAO_DB_COLUMN_TYPE_BIGINTEGER:
				DAO_CALL_METHOD(&is_unsigned, column, "isunsigned");
				if (zend_is_true(&is_unsigned)) {
					dao_concat_self_str(&column_sql, SL(" UNSIGNED"));
				}

				break;

			case DAO_DB_COLUMN_TYPE_DECIMAL:
				DAO_CALL_METHOD(&scale, column, "getscale");
				DAO_SCONCAT_SVSVS(&column_sql, "(", &size, ",", &scale, ")");
				break;

			case DAO_DB_COLUMN_TYPE_FLOAT:
				break;

			case DAO_DB_COLUMN_TYPE_DOUBLE:

				DAO_CALL_METHOD(&is_unsigned, column, "isunsigned");
				if (zend_is_true(&is_unsigned)) {
					dao_concat_self_str(&column_sql, SL(" UNSIGNED"));
				}

				break;

			case DAO_DB_COLUMN_TYPE_DATE:
			case DAO_DB_COLUMN_TYPE_DATETIME:
			case DAO_DB_COLUMN_TYPE_TIMESTAMP:
				break;

			case DAO_DB_COLUMN_TYPE_CHAR:
				DAO_SCONCAT_SVS(&column_sql, "(", &size, ")");
				break;

			case DAO_DB_COLUMN_TYPE_VARCHAR:
				DAO_SCONCAT_SVS(&column_sql, "(", &size, ")");
				break;

			case DAO_DB_COLUMN_TYPE_TEXT:
			case DAO_DB_COLUMN_TYPE_TINYBLOB:
			case DAO_DB_COLUMN_TYPE_BLOB:
			case DAO_DB_COLUMN_TYPE_MEDIUMBLOB:
			case DAO_DB_COLUMN_TYPE_LONGBLOB:
			case DAO_DB_COLUMN_TYPE_BOOLEAN:
				break;

			default:
				DAO_CALL_METHOD(&name, column, "getname");
				DAO_THROW_EXCEPTION_FORMAT(dao_db_exception_ce, "Unrecognized SQLite data type at column %s", Z_STRVAL(name));
		}
		RETURN_CTOR(&column_sql);
	}

	switch (dao_get_intval(&column_type)) {

		case DAO_DB_COLUMN_TYPE_INTEGER:
			ZVAL_STRING(&column_sql, "INTEGER");
			break;

		case DAO_DB_COLUMN_TYPE_BIGINTEGER:
			ZVAL_STRING(&column_sql, "BIGINT");

			DAO_CALL_METHOD(&is_unsigned, column, "isunsigned");
			if (zend_is_true(&is_unsigned)) {
				dao_concat_self_str(&column_sql, SL(" UNSIGNED"));
			}

			break;

		case DAO_DB_COLUMN_TYPE_DECIMAL:
			DAO_CALL_METHOD(&scale, column, "getscale");
			DAO_CONCAT_SVSVS(&column_sql, "NUMERIC(", &size, ",", &scale, ")");
			break;

		case DAO_DB_COLUMN_TYPE_FLOAT:
			ZVAL_STRING(&column_sql, "FLOAT");
			break;

		case DAO_DB_COLUMN_TYPE_DOUBLE:
			ZVAL_STRING(&column_sql, "DOUBLE");

			DAO_CALL_METHOD(&is_unsigned, column, "isunsigned");
			if (zend_is_true(&is_unsigned)) {
				dao_concat_self_str(&column_sql, SL(" UNSIGNED"));
			}

			break;

		case DAO_DB_COLUMN_TYPE_DATE:
			ZVAL_STRING(&column_sql, "DATE");
			break;

		case DAO_DB_COLUMN_TYPE_DATETIME:
			ZVAL_STRING(&column_sql, "DATETIME");
			break;

		case DAO_DB_COLUMN_TYPE_TIMESTAMP:
			ZVAL_STRING(&column_sql, "TIMESTAMP");
			break;

		case DAO_DB_COLUMN_TYPE_CHAR:
			DAO_CONCAT_SVS(&column_sql, "CHARACTER(", &size, ")");
			break;

		case DAO_DB_COLUMN_TYPE_VARCHAR:
			DAO_CONCAT_SVS(&column_sql, "VARCHAR(", &size, ")");
			break;

		case DAO_DB_COLUMN_TYPE_TEXT:
			ZVAL_STRING(&column_sql, "TEXT");
			break;

		case DAO_DB_COLUMN_TYPE_TINYBLOB:
			ZVAL_STRING(&column_sql, "TINYBLOB");
			break;

		case DAO_DB_COLUMN_TYPE_BLOB:
			ZVAL_STRING(&column_sql, "BLOB");
			break;

		case DAO_DB_COLUMN_TYPE_MEDIUMBLOB:
			ZVAL_STRING(&column_sql, "MEDIUMBLOB");
			break;

		case DAO_DB_COLUMN_TYPE_LONGBLOB:
			ZVAL_STRING(&column_sql, "LONGBLOB");
			break;

		case DAO_DB_COLUMN_TYPE_BOOLEAN:
			ZVAL_STRING(&column_sql, "TINYINT");
			break;

		default:
			DAO_CALL_METHOD(&name, column, "getname");
			DAO_THROW_EXCEPTION_FORMAT(dao_db_exception_ce, "Unrecognized SQLite data type at column %s", Z_STRVAL(name));
			return;
	}

	RETURN_CTOR(&column_sql);
}

/**
 * Generates SQL to add a column to a table
 *
 * @param string $tableName
 * @param string $schemaName
 * @param Dao\Db\ColumnInterface $column
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect_Sqlite, addColumn){

	zval *table_name, *schema_name, *column, sql = {}, name = {}, column_definition = {}, column_type = {}, default_value = {}, is_not_null = {}, is_autoincrement = {};

	dao_fetch_params(0, 3, 0, &table_name, &schema_name, &column);

	DAO_VERIFY_INTERFACE_EX(column, dao_db_columninterface_ce, dao_db_exception_ce);

	if (zend_is_true(schema_name)) {
		DAO_CONCAT_SVSVS(&sql, "ALTER TABLE \"", schema_name, "\".\"", table_name, "\" ADD COLUMN ");
	}
	else {
		DAO_CONCAT_SVS(&sql, "ALTER TABLE \"", table_name, "\" ADD COLUMN ");
	}

	DAO_CALL_METHOD(&name, column, "getname");
	DAO_CALL_METHOD(&column_definition, getThis(), "getcolumndefinition", column);
	DAO_SCONCAT_SVSV(&sql, "\"", &name, "\" ", &column_definition);

	DAO_CALL_METHOD(&default_value, column, "getdefaultvalue");
	if (Z_TYPE(default_value) != IS_NULL) {
		DAO_CALL_METHOD(&column_type, column, "gettype");
		DAO_CALL_METHOD(&default_value, getThis(), "getdefaultvalue", &default_value, &column_type);
		DAO_SCONCAT_SV(&sql, " DEFAULT ", &default_value);
	}

	DAO_CALL_METHOD(&is_not_null, column, "isnotnull");
	if (zend_is_true(&is_not_null)) {
		dao_concat_self_str(&sql, SL(" NOT NULL"));
	}

	DAO_CALL_METHOD(&is_autoincrement, column, "isautoincrement");
	/*
	 * See http://www.sqlite.org/syntaxdiagrams.html#column-constraint
	 */
	if (zend_is_true(&is_autoincrement)) {
		dao_concat_self_str(&sql, SL(" PRIMARY KEY AUTOINCREMENT"));
	}

	RETURN_CTOR(&sql);
}

/**
 * Generates SQL to modify a column in a table
 *
 * @param string $tableName
 * @param string $schemaName
 * @param Dao\Db\ColumnInterface $column
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect_Sqlite, modifyColumn){

	DAO_THROW_EXCEPTION_STR(dao_db_exception_ce, "Altering a DB column is not supported by SQLite");
}

/**
 * Generates SQL to delete a column from a table
 *
 * @param string $tableName
 * @param string $schemaName
 * @param string $columnName
 * @return 	string
 */
PHP_METHOD(Dao_Db_Dialect_Sqlite, dropColumn){

	DAO_THROW_EXCEPTION_STR(dao_db_exception_ce, "Dropping DB column is not supported by SQLite");
}

/**
 * Generates SQL to add an index to a table
 *
 * @param string $tableName
 * @param string $schemaName
 * @param Dao\Db\IndexInterface $index
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect_Sqlite, addIndex){

	zval *table_name, *schema_name, *index, name = {}, index_type = {}, sql = {}, columns = {}, quoted_column_list = {};

	dao_fetch_params(0, 3, 0, &table_name, &schema_name, &index);
	DAO_VERIFY_INTERFACE_EX(index, dao_db_indexinterface_ce, dao_db_exception_ce);


	DAO_CALL_METHOD(&name, index, "getname");
	DAO_CALL_METHOD(&index_type, index, "gettype");

	if (zend_is_true(schema_name)) {
		if (Z_TYPE(index_type) == IS_STRING && Z_STRLEN(index_type) > 0) {
			DAO_CONCAT_SVSVSVSVS(&sql, "CREATE ", &index_type, " INDEX \"", schema_name, "\".\"", &name, "\" ON \"", table_name, "\" (");
		} else {
			DAO_CONCAT_SVSVSVS(&sql, "CREATE INDEX \"", schema_name, "\".\"", &name, "\" ON \"", table_name, "\" (");
		}
	} else if (Z_TYPE(index_type) == IS_STRING && Z_STRLEN(index_type) > 0) {
		DAO_CONCAT_SVSVSVS(&sql, "CREATE ", &index_type, " INDEX \"", &name, "\" ON \"", table_name, "\" (");
	} else {
		DAO_CONCAT_SVSVS(&sql, "CREATE INDEX \"", &name, "\" ON \"", table_name, "\" (");
	}

	DAO_CALL_METHOD(&columns, index, "getcolumns");
	DAO_CALL_METHOD(&quoted_column_list, getThis(), "getcolumnlist", &columns);

	DAO_SCONCAT_VS(&sql, &quoted_column_list, ")");
	RETURN_CTOR(&sql);
}

/**
 * Generates SQL to delete an index from a table
 *
 * @param string $tableName
 * @param string $schemaName
 * @param string $indexName
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect_Sqlite, dropIndex){

	zval *table_name, *schema_name, *index_name, sql = {};

	dao_fetch_params(0, 3, 0, &table_name, &schema_name, &index_name);

	if (zend_is_true(schema_name)) {
		DAO_CONCAT_SVSVS(&sql, "DROP INDEX \"", schema_name, "\".\"", index_name, "\"");
	}
	else {
		DAO_CONCAT_SVS(&sql, "DROP INDEX \"", index_name, "\"");
	}

	RETURN_CTOR(&sql);
}

/**
 * Generates SQL to add the primary key to a table
 *
 * @param string $tableName
 * @param string $schemaName
 * @param Dao\Db\IndexInterface $index
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect_Sqlite, addPrimaryKey){

	DAO_THROW_EXCEPTION_STR(dao_db_exception_ce, "Adding a primary key after table has been created is not supported by SQLite");
}

/**
 * Generates SQL to delete primary key from a table
 *
 * @param string $tableName
 * @param string $schemaName
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect_Sqlite, dropPrimaryKey){

	DAO_THROW_EXCEPTION_STR(dao_db_exception_ce, "Removing a primary key after table has been created is not supported by SQLite");
}

/**
 * Generates SQL to add an index to a table
 *
 * @param string $tableName
 * @param string $schemaName
 * @param Dao\Db\Reference $reference
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect_Sqlite, addForeignKey){

	DAO_THROW_EXCEPTION_STR(dao_db_exception_ce, "Adding a foreign key constraint to an existing table is not supported by SQLite");
}

/**
 * Generates SQL to delete a foreign key from a table
 *
 * @param string $tableName
 * @param string $schemaName
 * @param string $referenceName
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect_Sqlite, dropForeignKey){

	DAO_THROW_EXCEPTION_STR(dao_db_exception_ce, "Dropping a foreign key constraint is not supported by SQLite");
}

/**
 * Generates SQL to add the table creation options
 *
 * @param array $definition
 * @return array
 */
PHP_METHOD(Dao_Db_Dialect_Sqlite, _getTableOptions){

	zval *definition;

	dao_fetch_params(0, 1, 0, &definition);

	array_init(return_value);
}

/**
 * Generates SQL to create a table in Sqlite
 *
 * @param 	string $tableName
 * @param string $schemaName
 * @param array $definition
 * @return 	string
 */
PHP_METHOD(Dao_Db_Dialect_Sqlite, createTable){

	zval *table_name, *schema_name, *definition, columns = {}, table = {}, options = {}, temporary = {}, sql = {}, create_lines = {}, slash = {};
	zval has_primary = {}, *column, indexes = {}, *index, references = {}, *reference, joined_lines = {};

	dao_fetch_params(0, 3, 0, &table_name, &schema_name, &definition);

	if (!dao_array_isset_fetch_str(&columns, definition, SL("columns"), PH_READONLY)) {
		DAO_THROW_EXCEPTION_STR(dao_db_exception_ce, "The index 'columns' is required in the definition array");
		return;
	}

	DAO_CALL_METHOD(&table, getThis(), "preparetable", table_name, schema_name);

	if (dao_array_isset_fetch_str(&options, definition, SL("options"), PH_READONLY)) {
		if (!dao_array_isset_fetch_str(&temporary, &options, SL("temporary"), PH_READONLY)) {
			ZVAL_FALSE(&temporary);
		}
	} else {
		ZVAL_FALSE(&temporary);
	}

	/**
	 * Create a temporary o normal table
	 */
	if (zend_is_true(&temporary)) {
		DAO_CONCAT_SVS(&sql, "CREATE TEMPORARY TABLE ", &table, " (\n\t");
	} else {
		DAO_CONCAT_SVS(&sql, "CREATE TABLE ", &table, " (\n\t");
	}

	array_init(&create_lines);

	ZVAL_STRING(&slash, "\"");
	ZVAL_FALSE(&has_primary);
	ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(&columns), column) {
		zval column_name = {}, column_definition = {}, column_type = {}, column_line = {}, default_value = {}, attribute = {};

		DAO_CALL_METHOD(&column_name, column, "getname");
		DAO_CALL_METHOD(&column_definition, getThis(), "getcolumndefinition", column);

		DAO_CONCAT_SVSV(&column_line, "`", &column_name, "` ", &column_definition);

		/**
		 * Mark the column as primary key
		 */
		DAO_CALL_METHOD(&attribute, column, "isprimary");
		if (zend_is_true(&attribute) && !zend_is_true(&has_primary)) {
			dao_concat_self_str(&column_line, SL(" PRIMARY KEY"));
			ZVAL_TRUE(&has_primary);
		}

		/**
		 * Add an AUTO_INCREMENT clause
		 */
		DAO_CALL_METHOD(&attribute, column, "isautoincrement");
		if (zend_is_true(&attribute)) {
			dao_concat_self_str(&column_line, SL(" AUTOINCREMENT"));
		}

		DAO_CALL_METHOD(&default_value, column, "getdefaultvalue");
		if (Z_TYPE(default_value) != IS_NULL) {
			DAO_CALL_METHOD(&column_type, column, "gettype");
			DAO_CALL_METHOD(&default_value, getThis(), "getdefaultvalue", &default_value, &column_type);
			DAO_SCONCAT_SV(&column_line, " DEFAULT ", &default_value);
		}

		/**
		 * Add a NOT NULL clause
		 */
		DAO_CALL_METHOD(&attribute, column, "isnotnull");
		if (zend_is_true(&attribute)) {
			dao_concat_self_str(&column_line, SL(" NOT NULL"));
		}

		dao_array_append(&create_lines, &column_line, PH_COPY);
	} ZEND_HASH_FOREACH_END();

	/**
	 * Create related indexes
	 */
	if (dao_array_isset_fetch_str(&indexes, definition, SL("indexes"), PH_READONLY)) {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(&indexes), index) {
			zval index_name = {}, columns = {}, column_list = {}, index_type = {}, index_sql = {};

			DAO_CALL_METHOD(&index_name, index, "getname");
			DAO_CALL_METHOD(&columns, index, "getcolumns");
			DAO_CALL_METHOD(&column_list, getThis(), "getcolumnlist", &columns);
			DAO_CALL_METHOD(&index_type, index, "gettype");

			/**
			 * If the index name is primary we add a primary key
			 */
			if (DAO_IS_STRING(&index_name, "PRIMARY")) {
				DAO_CONCAT_SVS(&index_sql, "PRIMARY KEY (", &column_list, ")");
			} else if (DAO_IS_NOT_EMPTY(&index_type) && dao_comparestr_str(&index_type, SL("UNIQUE"), &DAO_GLOBAL(z_false))) {
				DAO_CONCAT_SVS(&index_sql, "UNIQUE (", &column_list, ")");
			}

			dao_array_append(&create_lines, &index_sql, PH_COPY);
		} ZEND_HASH_FOREACH_END();
	}

	/**
	 * Create related references
	 */
	if (dao_array_isset_fetch_str(&references, definition, SL("references"), PH_READONLY)) {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(&references), reference) {
			zval name = {}, columns = {}, column_list = {}, referenced_table = {}, referenced_columns = {}, referenced_column_list = {}, constaint_sql = {}, reference_sql = {}, on_delete = {}, on_update = {};

			DAO_CALL_METHOD(&name, reference, "getname");
			DAO_CALL_METHOD(&columns, reference, "getcolumns");
			DAO_CALL_METHOD(&column_list, getThis(), "getcolumnlist", &columns);
			DAO_CALL_METHOD(&referenced_table, reference, "getreferencedtable");
			DAO_CALL_METHOD(&referenced_columns, reference, "getreferencedcolumns");
			DAO_CALL_METHOD(&referenced_column_list, getThis(), "getcolumnlist", &referenced_columns);

			DAO_CONCAT_SVSVS(&constaint_sql, "CONSTRAINT `", &name, "` FOREIGN KEY (", &column_list, ")");
			DAO_CONCAT_VSVSVS(&reference_sql, &constaint_sql, " REFERENCES `", &referenced_table, "`(", &referenced_column_list, ")");

			DAO_CALL_METHOD(&on_delete, reference, "getondelete");
			if (DAO_IS_NOT_EMPTY(&on_delete)) {
				DAO_SCONCAT_SV(&reference_sql, " ON DELETE ", &on_delete);
			}

			DAO_CALL_METHOD(&on_update, reference, "getonupdate");
			if (DAO_IS_NOT_EMPTY(&on_update)) {
				DAO_SCONCAT_SV(&reference_sql, " ON UPDATE ", &on_update);
			}

			dao_array_append(&create_lines, &reference_sql, PH_COPY);
		} ZEND_HASH_FOREACH_END();
	}

	dao_fast_join_str(&joined_lines, SL(",\n\t"), &create_lines);

	DAO_SCONCAT_VS(&sql, &joined_lines, "\n)");

	RETURN_CTOR(&sql);
}

/**
 * Generates SQL to drop a table
 *
 * @param  string $tableName
 * @param  string $schemaName
 * @param  boolean $ifExists
 * @return boolean
 */
PHP_METHOD(Dao_Db_Dialect_Sqlite, dropTable){

	zval *table_name, *schema_name, *if_exists = NULL, table = {};

	dao_fetch_params(0, 2, 1, &table_name, &schema_name, &if_exists);

	if (!if_exists) {
		if_exists = &DAO_GLOBAL(z_true);
	}

	DAO_CALL_METHOD(&table, getThis(), "preparetable", table_name, schema_name);

	if (zend_is_true(if_exists)) {
		DAO_CONCAT_SVS(return_value, "DROP TABLE IF EXISTS \"", &table, "\"");
	} else {
		DAO_CONCAT_SVS(return_value, "DROP TABLE \"", &table, "\"");
	}
}

/**
 * Generates SQL to create a view
 *
 * @param string $viewName
 * @param array $definition
 * @param string $schemaName
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect_Sqlite, createView){

	zval *view_name, *definition, *schema_name, view_sql = {}, view = {};

	dao_fetch_params(0, 3, 0, &view_name, &definition, &schema_name);

	if (!dao_array_isset_fetch_str(&view_sql, definition, SL("sql"), PH_READONLY)) {
		DAO_THROW_EXCEPTION_STR(dao_db_exception_ce, "The index 'sql' is required in the definition array");
		return;
	}

	DAO_CALL_METHOD(&view, getThis(), "preparetable", view_name, schema_name);

	DAO_CONCAT_SVSV(return_value, "CREATE VIEW ", &view, " AS ", &view_sql);

	zval_ptr_dtor(&view);
}

/**
 * Generates SQL to drop a view
 *
 * @param string $viewName
 * @param string $schemaName
 * @param boolean $ifExists
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect_Sqlite, dropView){

	zval *view_name, *schema_name, *if_exists = NULL, view = {};

	dao_fetch_params(0, 2, 1, &view_name, &schema_name, &if_exists);

	if (!if_exists) {
		if_exists = &DAO_GLOBAL(z_true);
	}

	DAO_CALL_METHOD(&view, getThis(), "preparetable", view_name, schema_name);

	if (zend_is_true(if_exists)) {
		DAO_CONCAT_SV(return_value, "DROP VIEW IF EXISTS ", &view);
	} else {
		DAO_CONCAT_SVS(return_value, "DROP VIEW ", &view, "");
	}
	zval_ptr_dtor(&view);
}

/**
 * Generates SQL checking for the existence of a schema.table
 *
 * <code>echo $dialect->tableExists("posts", "blog")</code>
 * <code>echo $dialect->tableExists("posts")</code>
 *
 * @param string $tableName
 * @param string $schemaName
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect_Sqlite, tableExists){

	zval *table_name, *schema_name = NULL;

	dao_fetch_params(0, 1, 1, &table_name, &schema_name);

	DAO_CONCAT_SVS(return_value, "SELECT CASE WHEN COUNT(*) > 0 THEN 1 ELSE 0 END FROM sqlite_master WHERE type='table' AND tbl_name='", table_name, "'");
}

/**
 * Generates SQL checking for the existence of a schema.view
 *
 * @param string $viewName
 * @param string $schemaName
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect_Sqlite, viewExists){

	zval *view_name, *schema_name = NULL;

	dao_fetch_params(0, 1, 1, &view_name, &schema_name);

	DAO_CONCAT_SVS(return_value, "SELECT CASE WHEN COUNT(*) > 0 THEN 1 ELSE 0 END FROM sqlite_master WHERE type='view' AND tbl_name='", view_name, "'");
}

/**
 * Generates a SQL describing a table
 *
 * <code>print_r($dialect->describeColumns("posts") ?></code>
 *
 * @param string $table
 * @param string $schema
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect_Sqlite, describeColumns){

	zval *table, *schema = NULL;

	dao_fetch_params(0, 1, 1, &table, &schema);

	DAO_CONCAT_SVS(return_value, "PRAGMA table_info('", table, "')");
}

/**
 * Generates SQL list all tables on database
 *
 * <code>print_r($dialect->listTables("blog")) ?></code>
 *
 * @param string $schemaName
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect_Sqlite, listTables){

	zval *schema_name = NULL;

	dao_fetch_params(0, 0, 1, &schema_name);

	RETURN_STRING("SELECT tbl_name FROM sqlite_master WHERE type = 'table' ORDER BY tbl_name");
}

/**
 * Generates the SQL to list all views of a schema or user
 *
 * @param string $schemaName
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect_Sqlite, listViews){

	zval *schema_name = NULL;

	dao_fetch_params(0, 0, 1, &schema_name);

	RETURN_STRING("SELECT tbl_name FROM sqlite_master WHERE type = 'view' ORDER BY tbl_name");
}

/**
 * Generates SQL to query indexes on a table
 *
 * @param string $table
 * @param string $schema
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect_Sqlite, describeIndexes){

	zval *table, *schema = NULL;

	dao_fetch_params(0, 1, 1, &table, &schema);

	DAO_CONCAT_SVS(return_value, "PRAGMA index_list('", table, "')");
}

/**
 * Generates SQL to query indexes detail on a table
 *
 * @param string $indexName
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect_Sqlite, describeIndex){

	zval *index_name;

	dao_fetch_params(0, 1, 0, &index_name);

	DAO_CONCAT_SVS(return_value, "PRAGMA index_info('", index_name, "')");
}

/**
 * Generates SQL to query foreign keys on a table
 *
 * @param string $table
 * @param string $schema
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect_Sqlite, describeReferences){

	zval *table, *schema = NULL;

	dao_fetch_params(0, 1, 1, &table, &schema);

	DAO_CONCAT_SVS(return_value, "PRAGMA foreign_key_list('", table, "')");
}

/**
 * Generates the SQL to describe the table creation options
 *
 * @param string $table
 * @param string $schema
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect_Sqlite, tableOptions){

	zval *table, *schema = NULL;

	dao_fetch_params(0, 1, 1, &table, &schema);

	RETURN_EMPTY_STRING();
}

/**
 * Return the default value
 *
 * @param string $defaultValue
 * @param string $columnDefinition
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect_Sqlite, getDefaultValue){

	zval *default_value, *column_type, slash = {}, value_cslashes = {};
	int type;

	dao_fetch_params(0, 2, 0, &default_value, &column_type);

	type = Z_LVAL_P(column_type);

	if (Z_TYPE_P(column_type) == IS_LONG) {
		type = Z_LVAL_P(column_type);

		if (type == DAO_DB_COLUMN_TYPE_BOOLEAN) {
			if (zend_is_true(default_value)) {
				ZVAL_STRING(return_value, "true");
			} else {
				ZVAL_STRING(return_value, "false");
			}
			return;
		} else if (dao_comparestr_str(default_value, SL("CURRENT_TIMESTAMP"), &DAO_GLOBAL(z_false))) {
			ZVAL_STRING(return_value, "CURRENT_TIMESTAMP");
			return;
		}  else if (
			type == DAO_DB_COLUMN_TYPE_INTEGER
			|| type == DAO_DB_COLUMN_TYPE_BIGINTEGER
			|| type == DAO_DB_COLUMN_TYPE_FLOAT
			|| type == DAO_DB_COLUMN_TYPE_DOUBLE
			|| type == DAO_DB_COLUMN_TYPE_DECIMAL
			|| type == DAO_DB_COLUMN_TYPE_MONEY
		) {
			RETURN_CTOR(default_value);
		}
	}
	ZVAL_STRING(&slash, "\"");
	DAO_CALL_FUNCTION(&value_cslashes, "addcslashes", default_value, &slash);
	zval_ptr_dtor(&slash);
	DAO_CONCAT_SVS(return_value, "\"", &value_cslashes, "\"");
	zval_ptr_dtor(&value_cslashes);
}
