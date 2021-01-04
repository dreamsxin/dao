
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

#include "db/dialect/mysql.h"
#include "db/dialect.h"
#include "db/dialectinterface.h"
#include "db/column.h"
#include "db/columninterface.h"
#include "db/indexinterface.h"
#include "db/referenceinterface.h"
#include "db/exception.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/exception.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "kernel/concat.h"
#include "kernel/array.h"
#include "kernel/string.h"
#include "kernel/file.h"

/**
 * Dao\Db\Dialect\Mysql
 *
 * Generates database specific SQL for the MySQL RBDMS
 */
zend_class_entry *dao_db_dialect_mysql_ce;

PHP_METHOD(Dao_Db_Dialect_Mysql, getColumnDefinition);
PHP_METHOD(Dao_Db_Dialect_Mysql, addColumn);
PHP_METHOD(Dao_Db_Dialect_Mysql, modifyColumn);
PHP_METHOD(Dao_Db_Dialect_Mysql, dropColumn);
PHP_METHOD(Dao_Db_Dialect_Mysql, addIndex);
PHP_METHOD(Dao_Db_Dialect_Mysql, dropIndex);
PHP_METHOD(Dao_Db_Dialect_Mysql, addPrimaryKey);
PHP_METHOD(Dao_Db_Dialect_Mysql, dropPrimaryKey);
PHP_METHOD(Dao_Db_Dialect_Mysql, addForeignKey);
PHP_METHOD(Dao_Db_Dialect_Mysql, dropForeignKey);
PHP_METHOD(Dao_Db_Dialect_Mysql, _getTableOptions);
PHP_METHOD(Dao_Db_Dialect_Mysql, createTable);
PHP_METHOD(Dao_Db_Dialect_Mysql, dropTable);
PHP_METHOD(Dao_Db_Dialect_Mysql, createView);
PHP_METHOD(Dao_Db_Dialect_Mysql, dropView);
PHP_METHOD(Dao_Db_Dialect_Mysql, tableExists);
PHP_METHOD(Dao_Db_Dialect_Mysql, viewExists);
PHP_METHOD(Dao_Db_Dialect_Mysql, describeColumns);
PHP_METHOD(Dao_Db_Dialect_Mysql, listTables);
PHP_METHOD(Dao_Db_Dialect_Mysql, listViews);
PHP_METHOD(Dao_Db_Dialect_Mysql, describeIndexes);
PHP_METHOD(Dao_Db_Dialect_Mysql, describeReferences);
PHP_METHOD(Dao_Db_Dialect_Mysql, tableOptions);
PHP_METHOD(Dao_Db_Dialect_Mysql, getDefaultValue);

static const zend_function_entry dao_db_dialect_mysql_method_entry[] = {
	PHP_ME(Dao_Db_Dialect_Mysql, getColumnDefinition, arginfo_dao_db_dialectinterface_getcolumndefinition, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect_Mysql, addColumn, arginfo_dao_db_dialectinterface_addcolumn, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect_Mysql, modifyColumn, arginfo_dao_db_dialectinterface_modifycolumn, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect_Mysql, dropColumn, arginfo_dao_db_dialectinterface_dropcolumn, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect_Mysql, addIndex, arginfo_dao_db_dialectinterface_addindex, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect_Mysql, dropIndex, arginfo_dao_db_dialectinterface_dropindex, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect_Mysql, addPrimaryKey, arginfo_dao_db_dialectinterface_addprimarykey, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect_Mysql, dropPrimaryKey, arginfo_dao_db_dialectinterface_dropprimarykey, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect_Mysql, addForeignKey, arginfo_dao_db_dialectinterface_addforeignkey, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect_Mysql, dropForeignKey, arginfo_dao_db_dialectinterface_dropforeignkey, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect_Mysql, _getTableOptions, arginfo_empty, ZEND_ACC_PROTECTED)
	PHP_ME(Dao_Db_Dialect_Mysql, createTable, arginfo_dao_db_dialectinterface_createtable, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect_Mysql, dropTable, arginfo_dao_db_dialectinterface_droptable, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect_Mysql, createView, arginfo_dao_db_dialectinterface_createview, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect_Mysql, dropView, arginfo_dao_db_dialectinterface_dropview, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect_Mysql, tableExists, arginfo_dao_db_dialectinterface_tableexists, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect_Mysql, viewExists, arginfo_dao_db_dialectinterface_viewexists, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect_Mysql, describeColumns, arginfo_dao_db_dialectinterface_describecolumns, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect_Mysql, listTables, arginfo_dao_db_dialectinterface_listtables, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect_Mysql, listViews, arginfo_dao_db_dialectinterface_listtables, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect_Mysql, describeIndexes, arginfo_dao_db_dialectinterface_describeindexes, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect_Mysql, describeReferences, arginfo_dao_db_dialectinterface_describereferences, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect_Mysql, tableOptions, arginfo_dao_db_dialectinterface_tableoptions, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect_Mysql, getDefaultValue, arginfo_dao_db_dialectinterface_getdefaultvalue, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Db\Dialect\Mysql initializer
 */
DAO_INIT_CLASS(Dao_Db_Dialect_Mysql){

	DAO_REGISTER_CLASS_EX(Dao\\Db\\Dialect, Mysql, db_dialect_mysql, dao_db_dialect_ce, dao_db_dialect_mysql_method_entry, 0);

	zend_declare_property_string(dao_db_dialect_mysql_ce, SL("_escapeChar"), "`", ZEND_ACC_PROTECTED);

	zend_class_implements(dao_db_dialect_mysql_ce, 1, dao_db_dialectinterface_ce);

	return SUCCESS;
}

/**
 * Gets the column name in MySQL
 *
 * @param Dao\Db\ColumnInterface $column
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect_Mysql, getColumnDefinition){

	zval *column, size = {}, column_type = {}, column_sql = {}, type_values = {}, slash = {}, *value, value_cslashes = {}, is_unsigned = {}, scale = {}, name = {};
	int c, i = 0;

	dao_fetch_params(0, 1, 0, &column);
	DAO_VERIFY_INTERFACE_EX(column, dao_db_columninterface_ce, dao_db_exception_ce);

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
				if (Z_LVAL(size) > 0) {
					DAO_SCONCAT_SVS(&column_sql, "(", &size, ")");
				}

				DAO_CALL_METHOD(&is_unsigned, column, "isunsigned");
				if (zend_is_true(&is_unsigned)) {
					dao_concat_self_str(&column_sql, SL(" UNSIGNED"));
				}

				break;

			case DAO_DB_COLUMN_TYPE_BIGINTEGER:
				if (Z_LVAL(size) > 0) {
					DAO_SCONCAT_SVS(&column_sql, "(", &size, ")");
				}

				DAO_CALL_METHOD(&is_unsigned, column, "isunsigned");
				if (zend_is_true(&is_unsigned)) {
					dao_concat_self_str(&column_sql, SL(" UNSIGNED"));
				}

				break;

			case DAO_DB_COLUMN_TYPE_DECIMAL:
				DAO_CALL_METHOD(&scale, column, "getscale");
				DAO_SCONCAT_SVSVS(&column_sql, "(", &size, ",", &scale, ")");

				DAO_CALL_METHOD(&is_unsigned, column, "isunsigned");
				if (zend_is_true(&is_unsigned)) {
					dao_concat_self_str(&column_sql, SL(" UNSIGNED"));
				}

				break;

			case DAO_DB_COLUMN_TYPE_FLOAT:
				DAO_CALL_METHOD(&scale, column, "getscale");
				if (zend_is_true(&size)) {
					DAO_SCONCAT_SV(&column_sql, "(", &size);
					if (zend_is_true(&scale)) {
						DAO_SCONCAT_SVS(&column_sql, ",", &scale, ")");
					} else {
						dao_concat_self_str(&column_sql, SL(")"));
					}
				}

				DAO_CALL_METHOD(&is_unsigned, column, "isunsigned");
				if (zend_is_true(&is_unsigned)) {
					dao_concat_self_str(&column_sql, SL(" UNSIGNED"));
				}

				break;

			case DAO_DB_COLUMN_TYPE_DOUBLE:
				DAO_CALL_METHOD(&scale, column, "getscale");
				if (zend_is_true(&size)) {
					DAO_SCONCAT_SV(&column_sql, "(", &size);
					if (zend_is_true(&scale)) {
						DAO_SCONCAT_SVS(&column_sql, ",", &scale, ")");
					} else {
						dao_concat_self_str(&column_sql, SL(")"));
					}
				}

				DAO_CALL_METHOD(&is_unsigned, column, "isunsigned");
				if (zend_is_true(&is_unsigned)) {
					dao_concat_self_str(&column_sql, SL(" UNSIGNED"));
				}

				break;

			case DAO_DB_COLUMN_TYPE_TINYBLOB:
			case DAO_DB_COLUMN_TYPE_BLOB:
			case DAO_DB_COLUMN_TYPE_MEDIUMBLOB:
			case DAO_DB_COLUMN_TYPE_LONGBLOB:
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
				break;

			case DAO_DB_COLUMN_TYPE_BOOLEAN:
				dao_concat_self_str(&column_sql, SL("(1)"));
				break;

			default:
				DAO_CALL_METHOD(&name, column, "getname");
				DAO_THROW_EXCEPTION_FORMAT(dao_db_exception_ce, "Unrecognized MySQL data type at column %s", Z_STRVAL(name));
		}

		RETURN_CTOR(&column_sql);
	}

	switch (dao_get_intval(&column_type)) {
		case DAO_DB_COLUMN_TYPE_INTEGER:
			if (Z_LVAL(size) > 0) {
				DAO_CONCAT_SVS(&column_sql, "INT(", &size, ")");
			}
			else {
				ZVAL_STRING(&column_sql, "INT");
			}

			DAO_CALL_METHOD(&is_unsigned, column, "isunsigned");
			if (zend_is_true(&is_unsigned)) {
				dao_concat_self_str(&column_sql, SL(" UNSIGNED"));
			}

			break;

		case DAO_DB_COLUMN_TYPE_BIGINTEGER:
			if (Z_LVAL(size) > 0) {
				DAO_CONCAT_SVS(&column_sql, "BIGINT(", &size, ")");
			}
			else {
				ZVAL_STRING(&column_sql, "BIGINT");
			}

			DAO_CALL_METHOD(&is_unsigned, column, "isunsigned");
			if (zend_is_true(&is_unsigned)) {
				dao_concat_self_str(&column_sql, SL(" UNSIGNED"));
			}

			break;

		case DAO_DB_COLUMN_TYPE_DECIMAL:
			DAO_CALL_METHOD(&scale, column, "getscale");
			DAO_CONCAT_SVSVS(&column_sql, "DECIMAL(", &size, ",", &scale, ")");

			DAO_CALL_METHOD(&is_unsigned, column, "isunsigned");
			if (zend_is_true(&is_unsigned)) {
				dao_concat_self_str(&column_sql, SL(" UNSIGNED"));
			}

			break;

		case DAO_DB_COLUMN_TYPE_FLOAT:
			ZVAL_STRING(&column_sql, "FLOAT");

			DAO_CALL_METHOD(&scale, column, "getscale");
			if (Z_TYPE(size) != IS_NULL) {
				DAO_SCONCAT_SV(&column_sql, "(", &size);
				if (Z_TYPE(scale) != IS_NULL) {
					DAO_SCONCAT_SVS(&column_sql, ",", &scale, ")");
				} else {
					dao_concat_self_str(&column_sql, SL(")"));
				}
			}

			DAO_CALL_METHOD(&is_unsigned, column, "isunsigned");
			if (zend_is_true(&is_unsigned)) {
				dao_concat_self_str(&column_sql, SL(" UNSIGNED"));
			}

			break;

		case DAO_DB_COLUMN_TYPE_DOUBLE:
			ZVAL_STRING(&column_sql, "DOUBLE");

			DAO_CALL_METHOD(&scale, column, "getscale");
			if (Z_TYPE(size) != IS_NULL) {
				DAO_SCONCAT_SV(&column_sql, "(", &size);
				if (Z_TYPE(scale) != IS_NULL) {
					DAO_SCONCAT_SVS(&column_sql, ",", &scale, ")");
				} else {
					dao_concat_self_str(&column_sql, SL(")"));
				}
			}

			DAO_CALL_METHOD(&is_unsigned, column, "isunsigned");
			if (zend_is_true(&is_unsigned)) {
				dao_concat_self_str(&column_sql, SL(" UNSIGNED"));
			}

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
			DAO_CONCAT_SVS(&column_sql, "CHAR(", &size, ")");
			break;

		case DAO_DB_COLUMN_TYPE_VARCHAR:
			DAO_CONCAT_SVS(&column_sql, "VARCHAR(", &size, ")");
			break;

		case DAO_DB_COLUMN_TYPE_TEXT:
			ZVAL_STRING(&column_sql, "TEXT");
			break;

		case DAO_DB_COLUMN_TYPE_BOOLEAN:
			ZVAL_STRING(&column_sql, "TINYINT(1)");
			break;

		default:
			DAO_CALL_METHOD(&name, column, "getname");
			DAO_THROW_EXCEPTION_FORMAT(dao_db_exception_ce, "Unrecognized MySQL data type at column %s", Z_STRVAL(name));
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
 * @see http://dev.mysql.com/doc/refman/5.5/en/example-auto-increment.html
 */
PHP_METHOD(Dao_Db_Dialect_Mysql, addColumn){

	zval *table_name, *schema_name, *column, table = {}, sql = {}, name = {}, column_definition = {}, column_type = {}, default_value = {};
	zval is_not_null = {}, is_autoincrement = {}, is_first = {}, after_position = {};

	dao_fetch_params(0, 3, 0, &table_name, &schema_name, &column);
	DAO_VERIFY_INTERFACE_EX(column, dao_db_columninterface_ce, dao_db_exception_ce);

	DAO_CALL_METHOD(&table, getThis(), "preparetable", table_name, schema_name);
	DAO_CONCAT_SVS(&sql, "ALTER TABLE ", &table, " ADD ");

	DAO_CALL_METHOD(&name, column, "getname");
	DAO_CALL_METHOD(&column_definition, getThis(), "getcolumndefinition", column);

	DAO_SCONCAT_SVSV(&sql, "`", &name, "` ", &column_definition);

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
	 * In MySQL the AUTO_INCREMENT column has to be a part of the PRIMARY KEY
	 * This is why we explicitly create PRIMARY KEY here, otherwise ALTER TABLE will fail.
	 */
	if (zend_is_true(&is_autoincrement)) {
		dao_concat_self_str(&sql, SL(" PRIMARY KEY AUTO_INCREMENT"));
	}

	DAO_CALL_METHOD(&is_first, column, "isfirst");
	if (zend_is_true(&is_first)) {
		dao_concat_self_str(&sql, SL(" FIRST"));
	} else {
		DAO_CALL_METHOD(&after_position, column, "getafterposition");
		if (zend_is_true(&after_position)) {
			DAO_SCONCAT_SVS(&sql, " AFTER `", &after_position, "`");
		}
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
PHP_METHOD(Dao_Db_Dialect_Mysql, modifyColumn){

	zval *table_name, *schema_name, *column, sql = {}, name = {}, column_definition = {}, column_type = {}, is_not_null = {};
	zval default_value = {}, is_autoincrement = {}, is_first = {}, after_position = {};

	dao_fetch_params(0, 3, 0, &table_name, &schema_name, &column);
	DAO_VERIFY_INTERFACE_EX(column, dao_db_columninterface_ce, dao_db_exception_ce);

	if (zend_is_true(schema_name)) {
		DAO_CONCAT_SVSVS(&sql, "ALTER TABLE `", schema_name, "`.`", table_name, "` MODIFY ");
	} else {
		DAO_CONCAT_SVS(&sql, "ALTER TABLE `", table_name, "` MODIFY ");
	}

	DAO_CALL_METHOD(&name, column, "getname");

	DAO_CALL_METHOD(&column_definition, getThis(), "getcolumndefinition", column);
	DAO_SCONCAT_SVSV(&sql, "`", &name, "` ", &column_definition);

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
	if (zend_is_true(&is_autoincrement)) {
		dao_concat_self_str(&sql, SL(" AUTO_INCREMENT"));
	}

	DAO_CALL_METHOD(&is_first, column, "isfirst");
	if (zend_is_true(&is_first)) {
		dao_concat_self_str(&sql, SL(" FIRST"));
	} else {
		DAO_CALL_METHOD(&after_position, column, "getafterposition");
		if (zend_is_true(&after_position)) {
			DAO_SCONCAT_SVS(&sql, " AFTER `", &after_position, "`");
		}
	}

	RETURN_CTOR(&sql);
}

/**
 * Generates SQL to delete a column from a table
 *
 * @param string $tableName
 * @param string $schemaName
 * @param string $columnName
 * @return 	string
 */
PHP_METHOD(Dao_Db_Dialect_Mysql, dropColumn){

	zval *table_name, *schema_name, *column_name, sql = {};

	dao_fetch_params(0, 3, 0, &table_name, &schema_name, &column_name);

	if (zend_is_true(schema_name)) {
		DAO_CONCAT_SVSVS(&sql, "ALTER TABLE `", schema_name, "`.`", table_name, "` DROP COLUMN ");
	} else {
		DAO_CONCAT_SVS(&sql, "ALTER TABLE `", table_name, "` DROP COLUMN ");
	}
	DAO_SCONCAT_SVS(&sql, "`", column_name, "`");

	RETURN_CTOR(&sql);
}

/**
 * Generates SQL to add an index to a table
 *
 * @param string $tableName
 * @param string $schemaName
 * @param Dao\Db\IndexInterface $index
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect_Mysql, addIndex){

	zval *table_name, *schema_name, *index, index_type = {}, sql = {}, columns = {}, quoted_column_list = {}, name = {};

	dao_fetch_params(0, 3, 0, &table_name, &schema_name, &index);
	DAO_VERIFY_INTERFACE_EX(index, dao_db_indexinterface_ce, dao_db_exception_ce);

	DAO_CALL_METHOD(&index_type, index, "gettype");

	if (zend_is_true(schema_name)) {
		if (Z_TYPE(index_type) == IS_STRING && Z_STRLEN(index_type) > 0) {
			DAO_CONCAT_SVSVSVS(&sql, "ALTER TABLE `", schema_name, "`.`", table_name, "` ADD ", &index_type, " INDEX ");
		} else {
			DAO_CONCAT_SVSVS(&sql, "ALTER TABLE `", schema_name, "`.`", table_name, "` ADD INDEX ");
		}
	} else if (Z_TYPE(index_type) == IS_STRING && Z_STRLEN(index_type) > 0) {
		DAO_CONCAT_SVSVS(&sql, "ALTER TABLE `", table_name, "` ADD ", &index_type, " INDEX ");
	} else {
		DAO_CONCAT_SVS(&sql, "ALTER TABLE `", table_name, "` ADD INDEX ");
	}

	DAO_CALL_METHOD(&columns, index, "getcolumns");
	DAO_CALL_METHOD(&quoted_column_list, getThis(), "getcolumnlist", &columns);
	DAO_CALL_METHOD(&name, index, "getname");
	DAO_SCONCAT_SVSVS(&sql, "`", &name, "` (", &quoted_column_list, ")");

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
PHP_METHOD(Dao_Db_Dialect_Mysql, dropIndex){

	zval *table_name, *schema_name, *index_name, sql;

	dao_fetch_params(0, 3, 0, &table_name, &schema_name, &index_name);

	if (zend_is_true(schema_name)) {
		DAO_CONCAT_SVSVS(&sql, "ALTER TABLE `", schema_name, "`.`", table_name, "` DROP INDEX ");
	} else {
		DAO_CONCAT_SVS(&sql, "ALTER TABLE `", table_name, "` DROP INDEX ");
	}

	DAO_SCONCAT_SVS(&sql, "`", index_name, "`");

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
PHP_METHOD(Dao_Db_Dialect_Mysql, addPrimaryKey){

	zval *table_name, *schema_name, *index, sql = {}, columns = {}, column_list = {};

	dao_fetch_params(0, 3, 0, &table_name, &schema_name, &index);
	DAO_VERIFY_INTERFACE_EX(index, dao_db_indexinterface_ce, dao_db_exception_ce);

	if (zend_is_true(schema_name)) {
		DAO_CONCAT_SVSVS(&sql, "ALTER TABLE `", schema_name, "`.`", table_name, "` ADD PRIMARY KEY ");
	} else {
		DAO_CONCAT_SVS(&sql, "ALTER TABLE `", table_name, "` ADD PRIMARY KEY ");
	}

	DAO_CALL_METHOD(&columns, index, "getcolumns");
	DAO_CALL_METHOD(&column_list, getThis(), "getcolumnlist", &columns);
	DAO_SCONCAT_SVS(&sql, "(", &column_list, ")");

	RETURN_CTOR(&sql);
}

/**
 * Generates SQL to delete primary key from a table
 *
 * @param string $tableName
 * @param string $schemaName
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect_Mysql, dropPrimaryKey){

	zval *table_name, *schema_name, sql = {};

	dao_fetch_params(0, 2, 0, &table_name, &schema_name);

	if (zend_is_true(schema_name)) {
		DAO_CONCAT_SVSVS(&sql, "ALTER TABLE `", schema_name, "`.`", table_name, "` DROP PRIMARY KEY");
	} else {
		DAO_CONCAT_SVS(&sql, "ALTER TABLE `", table_name, "` DROP PRIMARY KEY");
	}

	RETURN_CTOR(&sql);
}

/**
 * Generates SQL to add an index to a table
 *
 * @param string $tableName
 * @param string $schemaName
 * @param Dao\Db\ReferenceInterface $reference
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect_Mysql, addForeignKey){

	zval *table_name, *schema_name, *reference, sql = {}, columns = {}, quoted_column_list = {}, reference_name = {};
	zval referenced_schema = {}, referenced_columns = {}, quoted_columns = {}, referenced_table = {}, on_delete = {}, on_update = {};

	dao_fetch_params(0, 3, 0, &table_name, &schema_name, &reference);

	DAO_VERIFY_INTERFACE_EX(reference, dao_db_referenceinterface_ce, dao_db_exception_ce);

	if (zend_is_true(schema_name)) {
		DAO_CONCAT_SVSVS(&sql, "ALTER TABLE `", schema_name, "`.`", table_name, "` ");
	} else {
		DAO_CONCAT_SVS(&sql, "ALTER TABLE `", table_name, "` ");
	}

    DAO_CALL_METHOD(&reference_name, reference, "getname");

	DAO_SCONCAT_SVS(&sql, "ADD CONSTRAINT `", &reference_name, "` FOREIGN KEY ");

	DAO_CALL_METHOD(&columns, reference, "getcolumns");

	DAO_CALL_METHOD(&quoted_column_list, getThis(), "getcolumnlist", &columns);

	DAO_SCONCAT_SVS(&sql, "(", &quoted_column_list, ") REFERENCES ");

	/**
	 * Add the schema
	 */
	DAO_CALL_METHOD(&referenced_schema, reference, "getreferencedschema");

	if (zend_is_true(&referenced_schema)) {
		DAO_SCONCAT_SVS(&sql, "`", &referenced_schema, "`.");
	}

	DAO_CALL_METHOD(&referenced_columns, reference, "getreferencedcolumns");
	DAO_CALL_METHOD(&quoted_columns, getThis(), "getcolumnlist", &referenced_columns);
	DAO_CALL_METHOD(&referenced_table, reference, "getreferencedtable");

	DAO_SCONCAT_SVSVS(&sql, "`", &referenced_table, "`(", &quoted_columns, ")");

	DAO_CALL_METHOD(&on_delete, reference, "getondelete");
	if (DAO_IS_NOT_EMPTY(&on_delete)) {
		DAO_SCONCAT_SV(&sql, " ON DELETE ", &on_delete);
	}

	DAO_CALL_METHOD(&on_update, reference, "getonupdate");
	if (DAO_IS_NOT_EMPTY(&on_update)) {
		DAO_SCONCAT_SV(&sql, " ON UPDATE ", &on_update);
	}

	RETURN_CTOR(&sql);
}

/**
 * Generates SQL to delete a foreign key from a table
 *
 * @param string $tableName
 * @param string $schemaName
 * @param string $referenceName
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect_Mysql, dropForeignKey){

	zval *table_name, *schema_name, *reference_name, sql;

	dao_fetch_params(0, 3, 0, &table_name, &schema_name, &reference_name);

	if (zend_is_true(schema_name)) {
		DAO_CONCAT_SVSVS(&sql, "ALTER TABLE `", schema_name, "`.`", table_name, "` DROP FOREIGN KEY ");
	} else {
		DAO_CONCAT_SVS(&sql, "ALTER TABLE `", table_name, "` DROP FOREIGN KEY ");
	}

	DAO_SCONCAT_SVS(&sql, "`", reference_name, "`");

	RETURN_CTOR(&sql);
}

/**
 * Generates SQL to add the table creation options
 *
 * @param array $definition
 * @return array
 */
PHP_METHOD(Dao_Db_Dialect_Mysql, _getTableOptions){

	zval *definition, options = {}, table_options = {}, engine = {}, sql_engine = {}, auto_increment = {}, sql_autoincrement = {};
	zval table_collation = {}, collation_parts = {}, first_part = {}, sql_charset = {}, sql_collate = {}, sql_table_options = {};

	dao_fetch_params(0, 1, 0, &definition);

	if (dao_array_isset_fetch_str(&options, definition, SL("options"), PH_READONLY)) {
		array_init(&table_options);

		/**
		 * Check if there is an ENGINE option
		 */
		if (dao_array_isset_fetch_str(&engine, &options, SL("ENGINE"), PH_READONLY)) {
			if (zend_is_true(&engine)) {
				DAO_CONCAT_SV(&sql_engine, "ENGINE=", &engine);
				dao_array_append(&table_options, &sql_engine, PH_COPY);
			}
		}

		/**
		 * Check if there is an AUTO_INCREMENT option
		 */
		if (dao_array_isset_fetch_str(&auto_increment, &options, SL("AUTO_INCREMENT"), PH_READONLY)) {
			if (zend_is_true(&auto_increment)) {
				DAO_CONCAT_SV(&sql_autoincrement, "AUTO_INCREMENT=", &auto_increment);
				dao_array_append(&table_options, &sql_autoincrement, PH_COPY);
			}
		}

		/**
		 * Check if there is a TABLE_COLLATION option
		 */
		if (dao_array_isset_fetch_str(&table_collation, &options, SL("TABLE_COLLATION"), PH_READONLY)) {
			if (zend_is_true(&table_collation)) {
				dao_fast_explode_str(&collation_parts, SL("_"), &table_collation);
				dao_array_fetch_long(&first_part, &collation_parts, 0, PH_NOISY|PH_READONLY);

				DAO_CONCAT_SV(&sql_charset, "DEFAULT CHARSET=", &first_part);
				dao_array_append(&table_options, &sql_charset, PH_COPY);

				DAO_CONCAT_SV(&sql_collate, "COLLATE=", &table_collation);
				dao_array_append(&table_options, &sql_collate, PH_COPY);
			}
		}

		if (dao_fast_count_ev(&table_options)) {
			dao_fast_join_str(&sql_table_options, SL(" "), &table_options);
			RETURN_CTOR(&sql_table_options);
		}
	}
	RETURN_NULL();
}

/**
 * Generates SQL to create a table in MySQL
 *
 * @param 	string $tableName
 * @param string $schemaName
 * @param array $definition
 * @return 	string
 */
PHP_METHOD(Dao_Db_Dialect_Mysql, createTable){

	zval *table_name, *schema_name, *definition, columns = {}, table = {}, options = {}, temporary = {}, sql = {}, create_lines = {}, slash = {};
	zval *column, indexes = {}, *index, references = {}, *reference, joined_lines = {};

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

	ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(&columns), column) {
		zval column_name = {}, column_definition = {}, column_type = {}, column_line = {}, default_value = {}, attribute = {};

		DAO_CALL_METHOD(&column_name, column, "getname");
		DAO_CALL_METHOD(&column_definition, getThis(), "getcolumndefinition", column);

		DAO_CONCAT_SVSV(&column_line, "`", &column_name, "` ", &column_definition);

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

		/**
		 * Add an AUTO_INCREMENT clause
		 */
		DAO_CALL_METHOD(&attribute, column, "isautoincrement");
		if (zend_is_true(&attribute)) {
			dao_concat_self_str(&column_line, SL(" AUTO_INCREMENT"));
		}

		/**
		 * Mark the column as primary key
		 */
		DAO_CALL_METHOD(&attribute, column, "isprimary");
		if (zend_is_true(&attribute)) {
			dao_concat_self_str(&column_line, SL(" PRIMARY KEY"));
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
			} else if (DAO_IS_NOT_EMPTY(&index_type)) {
				DAO_CONCAT_VSVSVS(&index_sql, &index_type, " KEY `", &index_name, "` (", &column_list, ")");
			} else {
				DAO_CONCAT_SVSVS(&index_sql, "KEY `", &index_name, "` (", &column_list, ")");
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

	if (dao_array_isset_str(definition, SL("options"))) {
		DAO_CALL_METHOD(&options, getThis(), "_gettableoptions", definition);
		DAO_SCONCAT_SV(&sql, " ", &options);
	}

	RETURN_CTOR(&sql);
}

/**
 * Generates SQL to drop a table
 *
 * @param  string $tableName
 * @param  string $schemaName
 * @param  boolean $ifExists
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect_Mysql, dropTable){

	zval *table_name, *schema_name, *if_exists = NULL, table = {};

	dao_fetch_params(0, 2, 1, &table_name, &schema_name, &if_exists);

	if (!if_exists) {
		if_exists = &DAO_GLOBAL(z_true);
	}

	DAO_CALL_METHOD(&table, getThis(), "preparetable", table_name, schema_name);

	if (zend_is_true(if_exists)) {
		DAO_CONCAT_SV(return_value, "DROP TABLE IF EXISTS ", &table);
	} else {
		DAO_CONCAT_SV(return_value, "DROP TABLE ", &table);
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
PHP_METHOD(Dao_Db_Dialect_Mysql, createView){

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
PHP_METHOD(Dao_Db_Dialect_Mysql, dropView){

	zval *view_name, *schema_name, *if_exists = NULL, view = {};

	dao_fetch_params(0, 2, 1, &view_name, &schema_name, &if_exists);

	if (!if_exists) {
		if_exists = &DAO_GLOBAL(z_true);
	}

	DAO_CALL_METHOD(&view, getThis(), "preparetable", view_name, schema_name);

	if (zend_is_true(if_exists)) {
		DAO_CONCAT_SV(return_value, "DROP VIEW IF EXISTS ", &view);
	} else {
		DAO_CONCAT_SV(return_value, "DROP VIEW ", &view);
	}
	zval_ptr_dtor(&view);
}

/**
 * Generates SQL checking for the existence of a schema.table
 *
 * <code>
 * echo $dialect->tableExists("posts", "blog");
 * echo $dialect->tableExists("posts");
 * </code>
 *
 * @param string $tableName
 * @param string $schemaName
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect_Mysql, tableExists){

	zval *table_name, *schema_name = NULL;

	dao_fetch_params(0, 1, 1, &table_name, &schema_name);

	if (schema_name && zend_is_true(schema_name)) {
		DAO_CONCAT_SVSVS(return_value, "SELECT IF(COUNT(*)>0, 1 , 0) FROM `INFORMATION_SCHEMA`.`TABLES` WHERE `TABLE_NAME`= '", table_name, "' AND `TABLE_SCHEMA`='", schema_name, "'");
	}
	else {
		DAO_CONCAT_SVS(return_value, "SELECT IF(COUNT(*)>0, 1 , 0) FROM `INFORMATION_SCHEMA`.`TABLES` WHERE `TABLE_NAME`='", table_name, "'");
	}
}

/**
 * Generates SQL checking for the existence of a schema.view
 *
 * @param string $viewName
 * @param string $schemaName
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect_Mysql, viewExists){

	zval *view_name, *schema_name = NULL;

	dao_fetch_params(0, 1, 1, &view_name, &schema_name);

	if (schema_name && zend_is_true(schema_name)) {
		DAO_CONCAT_SVSVS(return_value, "SELECT IF(COUNT(*)>0, 1 , 0) FROM `INFORMATION_SCHEMA`.`VIEWS` WHERE `TABLE_NAME`= '", view_name, "' AND `TABLE_SCHEMA`='", schema_name, "'");
	}
	else {
		DAO_CONCAT_SVS(return_value, "SELECT IF(COUNT(*)>0, 1 , 0) FROM `INFORMATION_SCHEMA`.`VIEWS` WHERE `TABLE_NAME`='", view_name, "'");
	}
}

/**
 * Generates SQL describing a table
 *
 *<code>
 *	print_r($dialect->describeColumns("posts")) ?>
 *</code>
 *
 * @param string $table
 * @param string $schema
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect_Mysql, describeColumns){

	zval *table_name, *schema_name = NULL, table = {};

	dao_fetch_params(0, 1, 1, &table_name, &schema_name);

	if (!schema_name) {
		DAO_CALL_METHOD(&table, getThis(), "preparetable", table_name);
	} else {
		DAO_CALL_METHOD(&table, getThis(), "preparetable", table_name, schema_name);
	}

	DAO_CONCAT_SV(return_value, "DESCRIBE ", &table);
	zval_ptr_dtor(&table);

}

/**
 * Generates SQL list all tables on database
 *
 *<code>
 *	print_r($dialect->listTables("blog")) ?>
 *</code>
 *
 * @param string $schemaName
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect_Mysql, listTables){

	zval *schema_name = NULL;

	dao_fetch_params(0, 0, 1, &schema_name);

	if (schema_name && zend_is_true(schema_name)) {
		DAO_CONCAT_SVS(return_value, "SHOW TABLES FROM `", schema_name, "`");
	}
	else {
		RETURN_STRING("SHOW TABLES");
	}
}

/**
 * Generates the SQL to list all views of a schema or user
 *
 * @param string $schemaName
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect_Mysql, listViews){

	zval *schema_name = NULL;

	dao_fetch_params(0, 0, 1, &schema_name);

	if (schema_name && zend_is_true(schema_name)) {
		DAO_CONCAT_SVS(return_value, "SELECT `TABLE_NAME` AS view_name FROM `INFORMATION_SCHEMA`.`VIEWS` WHERE `TABLE_SCHEMA` = '", schema_name, "' ORDER BY view_name");
	}
	else {
		RETURN_STRING("SELECT `TABLE_NAME` AS view_name FROM `INFORMATION_SCHEMA`.`VIEWS` ORDER BY view_name");
	}
}

/**
 * Generates SQL to query indexes on a table
 *
 * @param string $table
 * @param string $schema
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect_Mysql, describeIndexes){

	zval *table, *schema = NULL;

	dao_fetch_params(0, 1, 1, &table, &schema);

	if (schema && zend_is_true(schema)) {
		DAO_CONCAT_SVSVS(return_value, "SHOW INDEXES FROM `", schema, "`.`", table, "`");
	}
	else {
		DAO_CONCAT_SVS(return_value, "SHOW INDEXES FROM `", table, "`");
	}
}

/**
 * Generates SQL to query foreign keys on a table
 *
 * @param string $table
 * @param string $schema
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect_Mysql, describeReferences){

	zval *table, *schema = NULL, sql = {};

	dao_fetch_params(0, 1, 1, &table, &schema);

	ZVAL_STRING(&sql, "SELECT TABLE_NAME,COLUMN_NAME,CONSTRAINT_NAME,REFERENCED_TABLE_SCHEMA,REFERENCED_TABLE_NAME,REFERENCED_COLUMN_NAME FROM INFORMATION_SCHEMA.KEY_COLUMN_USAGE WHERE REFERENCED_TABLE_NAME IS NOT NULL AND ");
	if (schema && zend_is_true(schema)) {
		DAO_SCONCAT_SVSVS(&sql, "CONSTRAINT_SCHEMA = \"", schema, "\" AND TABLE_NAME = \"", table, "\"");
	} else {
		DAO_SCONCAT_SVS(&sql, "TABLE_NAME = \"", table, "\"");
	}

	RETURN_ZVAL(&sql, 0, 0);
}

/**
 * Generates the SQL to describe the table creation options
 *
 * @param string $table
 * @param string $schema
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect_Mysql, tableOptions){

	zval *table, *schema = NULL, sql = {};

	dao_fetch_params(0, 1, 1, &table, &schema);

	ZVAL_STRING(&sql, "SELECT TABLES.TABLE_TYPE AS table_type,TABLES.AUTO_INCREMENT AS auto_increment,TABLES.ENGINE AS engine,TABLES.TABLE_COLLATION AS table_collation FROM INFORMATION_SCHEMA.TABLES WHERE ");
	if (schema && zend_is_true(schema)) {
		DAO_SCONCAT_SVSVS(&sql, "TABLES.TABLE_SCHEMA = \"", schema, "\" AND TABLES.TABLE_NAME = \"", table, "\"");
	} else {
		DAO_SCONCAT_SVS(&sql, "TABLES.TABLE_NAME = \"", table, "\"");
	}

	RETURN_ZVAL(&sql, 0, 0);
}

/**
 * Return the default value
 *
 * @param string $defaultValue
 * @param string $columnDefinition
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect_Mysql, getDefaultValue){

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
