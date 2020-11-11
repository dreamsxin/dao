
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
  +------------------------------------------------------------------------+
*/

#include "db/adapter/pdo/sqlite.h"
#include "db/adapter/pdo.h"
#include "db/adapterinterface.h"
#include "db/column.h"
#include "db/exception.h"
#include "db/index.h"
#include "db/reference.h"

#include <ext/pdo/php_pdo_driver.h>

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/operators.h"
#include "kernel/concat.h"
#include "kernel/object.h"
#include "kernel/array.h"
#include "kernel/exception.h"
#include "kernel/fcall.h"
#include "kernel/string.h"
#include "kernel/hash.h"

/**
 * Dao\Db\Adapter\Pdo\Sqlite
 *
 * Specific functions for the Sqlite database system
 * <code>
 *
 * $config = array(
 *  "dbname" => "/tmp/test.sqlite"
 * );
 *
 * $connection = new Dao\Db\Adapter\Pdo\Sqlite($config);
 *
 * </code>
 */
zend_class_entry *dao_db_adapter_pdo_sqlite_ce;

PHP_METHOD(Dao_Db_Adapter_Pdo_Sqlite, connect);
PHP_METHOD(Dao_Db_Adapter_Pdo_Sqlite, describeColumns);
PHP_METHOD(Dao_Db_Adapter_Pdo_Sqlite, describeIndexes);
PHP_METHOD(Dao_Db_Adapter_Pdo_Sqlite, describeReferences);
PHP_METHOD(Dao_Db_Adapter_Pdo_Sqlite, useExplicitIdValue);
PHP_METHOD(Dao_Db_Adapter_Pdo_Sqlite, escapeBytea);
PHP_METHOD(Dao_Db_Adapter_Pdo_Sqlite, unescapeBytea);
PHP_METHOD(Dao_Db_Adapter_Pdo_Sqlite, escapeArray);
PHP_METHOD(Dao_Db_Adapter_Pdo_Sqlite, unescapeArray);

static const zend_function_entry dao_db_adapter_pdo_sqlite_method_entry[] = {
	PHP_ME(Dao_Db_Adapter_Pdo_Sqlite, connect, arginfo_dao_db_adapterinterface_connect, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Adapter_Pdo_Sqlite, describeColumns, arginfo_dao_db_adapterinterface_describecolumns, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Adapter_Pdo_Sqlite, describeIndexes, arginfo_dao_db_adapterinterface_describeindexes, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Adapter_Pdo_Sqlite, describeReferences, arginfo_dao_db_adapterinterface_describereferences, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Adapter_Pdo_Sqlite, useExplicitIdValue, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Adapter_Pdo_Sqlite, escapeBytea, arginfo_dao_db_adapterinterface_escapebytea, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Adapter_Pdo_Sqlite, unescapeBytea, arginfo_dao_db_adapterinterface_unescapebytea, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Adapter_Pdo_Sqlite, escapeArray, arginfo_dao_db_adapterinterface_escapearray, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Adapter_Pdo_Sqlite, unescapeArray, arginfo_dao_db_adapterinterface_unescapearray, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Db\Adapter\Pdo\Sqlite initializer
 */
DAO_INIT_CLASS(Dao_Db_Adapter_Pdo_Sqlite){

	DAO_REGISTER_CLASS_EX(Dao\\Db\\Adapter\\Pdo, Sqlite, db_adapter_pdo_sqlite, dao_db_adapter_pdo_ce, dao_db_adapter_pdo_sqlite_method_entry, 0);

	zend_declare_property_string(dao_db_adapter_pdo_sqlite_ce, SL("_type"), "sqlite", ZEND_ACC_PROTECTED);
	zend_declare_property_string(dao_db_adapter_pdo_sqlite_ce, SL("_dialectType"), "sqlite", ZEND_ACC_PROTECTED);

	zend_class_implements(dao_db_adapter_pdo_sqlite_ce, 1, dao_db_adapterinterface_ce);

	return SUCCESS;
}

/**
 * This method is automatically called in Dao\Db\Adapter\Pdo constructor.
 * Call it when you need to restore a database connection.
 *
 * @param array $descriptor
 * @return boolean
 */
PHP_METHOD(Dao_Db_Adapter_Pdo_Sqlite, connect){

	zval *desc = NULL, descriptor = {}, dbname = {};

	dao_fetch_params(0, 0, 1, &desc);

	if (!desc || !zend_is_true(desc)) {
		dao_read_property(&descriptor, getThis(), SL("_descriptor"), PH_CTOR);
	} else {
		ZVAL_DUP(&descriptor, desc);
	}

	if (!dao_array_isset_fetch_str(&dbname, &descriptor, SL("dbname"), PH_READONLY)) {
		DAO_THROW_EXCEPTION_STR(dao_db_exception_ce, "dbname must be specified");
		return;
	} else {
		dao_array_update_str(&descriptor, SL("dsn"), &dbname, PH_COPY);
	}

	DAO_CALL_PARENT(NULL, dao_db_adapter_pdo_sqlite_ce, getThis(), "connect", &descriptor);
	zval_ptr_dtor(&descriptor);
}

/**
 * Returns an array of Dao\Db\Column objects describing a table
 *
 * <code>
 * print_r($connection->describeColumns("posts")); ?>
 * </code>
 *
 * @param string $table
 * @param string $schema
 * @return Dao\Db\Column[]
 */
PHP_METHOD(Dao_Db_Adapter_Pdo_Sqlite, describeColumns){

	zval *table, *schema = NULL, dialect = {}, size_pattern = {}, sql = {}, fetch_num = {}, describe = {}, old_column = {}, *field;

	dao_fetch_params(1, 1, 1, &table, &schema);

	if (!schema) {
		schema = &DAO_GLOBAL(z_null);
	}

	array_init(return_value);

	dao_read_property(&dialect, getThis(), SL("_dialect"), PH_NOISY|PH_READONLY);

	DAO_MM_ZVAL_STRING(&size_pattern, "#\\(([0-9]++)(?:,\\s*([0-9]++))?\\)#");

	DAO_MM_CALL_METHOD(&sql, &dialect, "describecolumns", table, schema);
	DAO_MM_ADD_ENTRY(&sql);

	/**
	 * We're using FETCH_NUM to fetch the columns
	 */
	ZVAL_LONG(&fetch_num, PDO_FETCH_NUM);

	DAO_MM_CALL_METHOD(&describe, getThis(), "fetchall", &sql, &fetch_num);
	DAO_MM_ADD_ENTRY(&describe);

	ZEND_HASH_FOREACH_VAL(Z_ARRVAL(describe), field) {
		zval definition = {}, column_type = {}, matches = {}, pos = {}, match_one = {}, match_two = {}, attribute = {}, column_name = {}, column = {};

		array_init(&definition);
		DAO_MM_ADD_ENTRY(&definition);
		add_assoc_long_ex(&definition, SL("bindType"), DAO_DB_COLUMN_BIND_PARAM_STR);

		dao_array_fetch_long(&column_type, field, 2, PH_NOISY|PH_READONLY);

		if (dao_memnstr_str(&column_type, SL("("))) {
			ZVAL_NULL(&matches);
			RETURN_MM_ON_FAILURE(dao_preg_match(&pos, &size_pattern, &column_type, &matches, 0, 0));
			DAO_MM_ADD_ENTRY(&matches);
			if (zend_is_true(&pos)) {
				if (dao_array_isset_fetch_long(&match_one, &matches, 1, PH_READONLY)) {
					dao_array_update_str_long(&definition, SL("size"), dao_get_intval(&match_one), 0);
					dao_array_update_str_long(&definition, SL("bytes"), dao_get_intval(&match_one), 0);
				}
				if (dao_array_isset_fetch_long(&match_two, &matches, 2, PH_READONLY)) {
					dao_array_update_str_long(&definition, SL("scale"), dao_get_intval(&match_two), 0);
				}
			}
		}

		/**
		 * Check the column type to get the correct Dao type
		 */
		while (1) {
			/**
			 * Tinyint(1) is boolean
			 */
			if (dao_memnstr_str(&column_type, SL("tinyint(1)"))) {
				dao_array_update_str_long(&definition, SL("type"), DAO_DB_COLUMN_TYPE_BOOLEAN, 0);
				dao_array_update_str_long(&definition, SL("bindType"), 5, 0);
				DAO_MM_ZVAL_STRING(&column_type, "boolean"); // Change column type to skip size check.
				break;
			}

			/**
			 * Smallint
			 */
			if (dao_memnstr_str(&column_type, SL("smallint"))) {
				dao_array_update_str_long(&definition, SL("type"), DAO_DB_COLUMN_TYPE_INTEGER, 0);
				dao_array_update_str(&definition, SL("isNumeric"), &DAO_GLOBAL(z_true), 0);
				dao_array_update_str_long(&definition, SL("bindType"), DAO_DB_COLUMN_BIND_PARAM_INT, 0);
				dao_array_update_str_long(&definition, SL("bytes"), 2, 0);

				dao_array_fetch_long(&attribute, field, 5, PH_NOISY|PH_READONLY);

				/**
				 * Check if the column is auto increment
				 */
				if (zend_is_true(&attribute)) {
					dao_array_update_str_bool(&definition, SL("autoIncrement"), 1, 0);
				}
				break;
			}

			/**
			 * Mediumint
			 */
			if (dao_memnstr_str(&column_type, SL("mediumint"))) {
				dao_array_update_str_long(&definition, SL("type"), DAO_DB_COLUMN_TYPE_INTEGER, 0);
				dao_array_update_str(&definition, SL("isNumeric"), &DAO_GLOBAL(z_true), 0);
				dao_array_update_str_long(&definition, SL("bindType"), DAO_DB_COLUMN_BIND_PARAM_INT, 0);
				dao_array_update_str_long(&definition, SL("bytes"), 3, 0);

				dao_array_fetch_long(&attribute, field, 5, PH_NOISY|PH_READONLY);

				/**
				 * Check if the column is auto increment
				 */
				if (zend_is_true(&attribute)) {
					dao_array_update_str_bool(&definition, SL("autoIncrement"), 1, 0);
				}
				break;
			}

			/**
			 * BIGINT
			 */
			if (dao_memnstr_str(&column_type, SL("bigint"))) {
				dao_array_update_str_long(&definition, SL("type"), DAO_DB_COLUMN_TYPE_INTEGER, 0);
				dao_array_update_str(&definition, SL("isNumeric"), &DAO_GLOBAL(z_true), 0);
				dao_array_update_str_long(&definition, SL("bindType"), DAO_DB_COLUMN_BIND_PARAM_INT, 0);
				dao_array_update_str_long(&definition, SL("bytes"), 8, 0);

				dao_array_fetch_long(&attribute, field, 5, PH_NOISY|PH_READONLY);

				/**
				 * Check if the column is auto increment
				 */
				if (zend_is_true(&attribute)) {
					dao_array_update_str_bool(&definition, SL("autoIncrement"), 1, 0);
				}
				break;
			}

			/**
			 * Integers/Int are int
			 */
			dao_fast_stripos_str(&pos, &column_type, SL("int"));
			if (DAO_IS_NOT_FALSE(&pos)) {
				dao_array_update_str_long(&definition, SL("type"), DAO_DB_COLUMN_TYPE_INTEGER, 0);
				dao_array_update_str_bool(&definition, SL("isNumeric"), 1, 0);
				dao_array_update_str_long(&definition, SL("bindType"), DAO_DB_COLUMN_BIND_PARAM_INT, 0);
				dao_array_update_str_long(&definition, SL("bytes"), 4, 0);

				dao_array_fetch_long(&attribute, field, 5, PH_NOISY|PH_READONLY);

				/**
				 * Check if the column is auto increment
				 */
				if (zend_is_true(&attribute)) {
					dao_array_update_str_bool(&definition, SL("autoIncrement"), 1, 0);
				}
				break;
			}

			/**
			 * Float/Smallfloats/Decimals are float
			 */
			if (dao_memnstr_str(&column_type, SL("float"))) {
				dao_array_update_str_long(&definition, SL("type"), DAO_DB_COLUMN_TYPE_FLOAT, 0);
				dao_array_update_str_bool(&definition, SL("isNumeric"), 1, 0);
				dao_array_update_str_long(&definition, SL("bindType"), DAO_DB_COLUMN_BIND_PARAM_DECIMAL, 0);
				dao_array_update_str_long(&definition, SL("bytes"), 4, 0);
				break;
			}

			/**
			 * Double are floats
			 */
			if (dao_memnstr_str(&column_type, SL("double"))) {
				dao_array_update_str_long(&definition, SL("type"), DAO_DB_COLUMN_TYPE_DOUBLE, 0);
				dao_array_update_str(&definition, SL("isNumeric"), &DAO_GLOBAL(z_true), 0);
				dao_array_update_str_long(&definition, SL("bindType"), DAO_DB_COLUMN_BIND_PARAM_DECIMAL, 0);
				dao_array_update_str_long(&definition, SL("bytes"), 8, 0);
				break;
			}

			/**
			 * Decimals are floats
			 */
			if (dao_memnstr_str(&column_type, SL("decimal"))) {
				dao_array_update_str_long(&definition, SL("type"), DAO_DB_COLUMN_TYPE_DECIMAL, 0);
				dao_array_update_str_bool(&definition, SL("isNumeric"), 1, 0);
				dao_array_update_str_long(&definition, SL("bindType"), DAO_DB_COLUMN_BIND_PARAM_DECIMAL, 0);
				if (dao_is_numeric(&match_one)) {
					dao_array_update_str_long(&definition, SL("bytes"), dao_get_intval(&match_one) * 8, 0);
				} else {
					dao_array_update_str_long(&definition, SL("size"), 5, 0);
					dao_array_update_str_long(&definition, SL("bytes"), 5, 0);
				}
				if (dao_is_numeric(&match_two)) {
					dao_array_update_str_long(&definition, SL("scale"), dao_get_intval(&match_two), 0);
				} else {
					dao_array_update_str_long(&definition, SL("scale"), 2, 0);
				}
				break;
			}

			/**
			 * Varchar are varchars
			 */
			if (dao_memnstr_str(&column_type, SL("varchar"))) {
				dao_array_update_str_long(&definition, SL("type"), DAO_DB_COLUMN_TYPE_VARCHAR, 0);
				break;
			}

			/**
			 * Date/Datetime are varchars
			 */
			if (dao_memnstr_str(&column_type, SL("date"))) {
				dao_array_update_str_long(&definition, SL("type"), DAO_DB_COLUMN_TYPE_DATE, 0);
				break;
			}

			/**
			 * Timestamp as date
			 */
			if (dao_memnstr_str(&column_type, SL("timestamp"))) {
				dao_array_update_str_long(&definition, SL("type"), DAO_DB_COLUMN_TYPE_TIMESTAMP, 0);
				break;
			}

			/**
			 * Chars are chars
			 */
			if (dao_memnstr_str(&column_type, SL("char"))) {
				dao_array_update_str_long(&definition, SL("type"), DAO_DB_COLUMN_TYPE_CHAR, 0);
				break;
			}

			/**
			 * Special type for datetime
			 */
			if (dao_memnstr_str(&column_type, SL("datetime"))) {
				dao_array_update_str_long(&definition, SL("type"), DAO_DB_COLUMN_TYPE_DATETIME, 0);
				break;
			}

			/**
			 * Text are varchars
			 */
			if (dao_memnstr_str(&column_type, SL("text"))) {
				dao_array_update_str_long(&definition, SL("type"), DAO_DB_COLUMN_TYPE_TEXT, 0);
				break;
			}

			/**
			 * Enum are treated as char
			 */
			if (dao_memnstr_str(&column_type, SL("enum"))) {
				dao_array_update_str_long(&definition, SL("type"), DAO_DB_COLUMN_TYPE_CHAR, 0);
				break;
			}

			/**
			 * By default is string
			 */
			dao_array_update_str_long(&definition, SL("type"), DAO_DB_COLUMN_TYPE_VARCHAR, 0);
			break;
		}

		if (dao_memnstr_str(&column_type, SL("unsigned"))) {
			dao_array_update_str_bool(&definition, SL("unsigned"), 1, 0);
		}

		if (Z_TYPE(old_column) <= IS_NULL) {
			dao_array_update_str_bool(&definition, SL("first"), 1, 0);
		} else {
			dao_array_update_str(&definition, SL("after"), &old_column, PH_COPY);
		}

		/**
		 * Check if the field is primary key
		 */
		dao_array_fetch_long(&attribute, field, 5, PH_NOISY|PH_READONLY);
		if (zend_is_true(&attribute)) {
			dao_array_update_str_bool(&definition, SL("primary"), 1, 0);
		}

		/**
		 * Check if the column allows null values
		 */
		dao_array_fetch_long(&attribute, field, 3, PH_NOISY|PH_READONLY);
		if (zend_is_true(&attribute)) {
			dao_array_update_str_bool(&definition, SL("notNull"), 1, 0);
		}

		dao_array_fetch_long(&column_name, field, 1, PH_NOISY|PH_READONLY);

		/**
		 * If the column set the default values, get it
		 */
		dao_array_fetch_long(&attribute, field, 4, PH_NOISY|PH_READONLY);
		if (!DAO_IS_EMPTY(&attribute)) {
			dao_array_update_str(&definition, SL("default"), &attribute, PH_COPY);
		}

		/**
		 * Every column is stored as a Dao\Db\Column
		 */
		object_init_ex(&column, dao_db_column_ce);
		DAO_MM_CALL_METHOD(NULL, &column, "__construct", &column_name, &definition);
		dao_array_append(return_value, &column, 0);
		ZVAL_COPY_VALUE(&old_column, &column_name);
	} ZEND_HASH_FOREACH_END();
	RETURN_MM();
}

/**
 * Lists table indexes
 *
 * @param string $table
 * @param string $schema
 * @return Dao\Db\Index[]
 */
PHP_METHOD(Dao_Db_Adapter_Pdo_Sqlite, describeIndexes){

	zval *table, *_schema = NULL, schema = {}, dialect = {}, fetch_num = {}, sql = {}, describe = {}, indexes = {}, *index, index_objects = {}, *index_columns;
	zend_string *str_key;
	ulong idx;

	dao_fetch_params(1, 1, 1, &table, &_schema);

	if (!_schema || !zend_is_true(_schema)) {
		dao_read_property(&schema, getThis(), SL("_schema"), PH_NOISY|PH_READONLY);
	} else {
		ZVAL_COPY_VALUE(&schema, _schema);
	}

	dao_read_property(&dialect, getThis(), SL("_dialect"), PH_NOISY|PH_READONLY);

	/**
	 * We're using FETCH_NUM to fetch the columns
	 */
	ZVAL_LONG(&fetch_num, PDO_FETCH_NUM);

	DAO_MM_CALL_METHOD(&sql, &dialect, "describeindexes", table, &schema);
	DAO_MM_ADD_ENTRY(&sql);
	DAO_MM_CALL_METHOD(&describe, getThis(), "fetchall", &sql, &fetch_num);
	DAO_MM_ADD_ENTRY(&describe);

	/**
	 * Cryptic Guide: 0: position, 1: name
	 */
	array_init(&indexes);
	DAO_MM_ADD_ENTRY(&indexes);

	ZEND_HASH_FOREACH_VAL(Z_ARRVAL(describe), index) {
		zval key_name = {}, sql_index_describe = {}, describe_index = {}, *index_column;

		dao_array_fetch_long(&key_name, index, 1, PH_NOISY|PH_READONLY);

		DAO_MM_CALL_METHOD(&sql_index_describe, &dialect, "describeindex", &key_name);
		DAO_MM_ADD_ENTRY(&sql_index_describe);
		DAO_MM_CALL_METHOD(&describe_index, getThis(), "fetchall", &sql_index_describe, &fetch_num);
		DAO_MM_ADD_ENTRY(&describe_index);

		ZEND_HASH_FOREACH_VAL(Z_ARRVAL(describe_index), index_column) {
			zval column_name = {};
			dao_array_fetch_long(&column_name, index_column, 2, PH_NOISY|PH_READONLY);
			dao_array_append_multi_2(&indexes, &key_name, &column_name, PH_COPY);
		} ZEND_HASH_FOREACH_END();
	} ZEND_HASH_FOREACH_END();

	array_init(&index_objects);
	DAO_MM_ADD_ENTRY(&index_objects);

	ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL(indexes), idx, str_key, index_columns) {
		zval name = {}, index = {};
		if (str_key) {
			ZVAL_STR(&name, str_key);
		} else {
			ZVAL_LONG(&name, idx);
		}

		object_init_ex(&index, dao_db_index_ce);
		DAO_MM_CALL_METHOD(NULL, &index, "__construct", &name, index_columns);

		dao_array_update(&index_objects, &name, &index, 0);
	} ZEND_HASH_FOREACH_END();

	RETURN_MM_CTOR(&index_objects);
}

/**
 * Lists table references
 *
 * @param string $table
 * @param string $schema
 * @return Dao\Db\Reference[]
 */
PHP_METHOD(Dao_Db_Adapter_Pdo_Sqlite, describeReferences){

	zval *table, *schema = NULL, dialect = {}, sql, fetch_num, describe, *reference_describe;
	zend_string *str_key;
	ulong idx;

	dao_fetch_params(1, 1, 1, &table, &schema);

	if (!schema) {
		schema = &DAO_GLOBAL(z_null);
	}

	dao_read_property(&dialect, getThis(), SL("_dialect"), PH_NOISY|PH_READONLY);

	/**
	 * Get the SQL to describe the references
	 */
	DAO_MM_CALL_METHOD(&sql, &dialect, "describereferences", table, schema);
	DAO_MM_ADD_ENTRY(&sql);

	/**
	 * We're using FETCH_NUM to fetch the columns
	 */
	ZVAL_LONG(&fetch_num, PDO_FETCH_NUM);

	/**
	 * Execute the SQL describing the references
	 */
	DAO_MM_CALL_METHOD(&describe, getThis(), "fetchall", &sql, &fetch_num);
	DAO_MM_ADD_ENTRY(&describe);

	/**
	 * Cryptic Guide: 2: table, 3: from, 4: to
	 */
	array_init(return_value);

	ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL(describe), idx, str_key, reference_describe) {
		zval number = {}, constraint_name = {}, referenced_table = {}, from = {}, to = {}, columns = {}, referenced_columns = {}, reference_array = {}, reference = {};
		if (str_key) {
			ZVAL_STR(&number, str_key);
		} else {
			ZVAL_LONG(&number, idx);
		}

		DAO_CONCAT_SV(&constraint_name, "foreign_key_", &number);
		DAO_MM_ADD_ENTRY(&constraint_name);
		dao_array_fetch_long(&referenced_table, reference_describe, 2, PH_NOISY|PH_READONLY);
		dao_array_fetch_long(&from, reference_describe, 3, PH_NOISY|PH_READONLY);
		dao_array_fetch_long(&to, reference_describe, 4, PH_NOISY|PH_READONLY);

		array_init_size(&columns, 1);
		dao_array_append(&columns, &from, PH_COPY);

		array_init_size(&referenced_columns, 1);
		dao_array_append(&referenced_columns, &to, PH_COPY);

		array_init_size(&reference_array, 4);
		add_assoc_null_ex(&reference_array, SL("referencedSchema"));
		dao_array_update_str(&reference_array, SL("referencedTable"), &referenced_table, PH_COPY);
		dao_array_update_str(&reference_array, SL("columns"), &columns, 0);
		dao_array_update_str(&reference_array, SL("referencedColumns"), &referenced_columns, 0);
		DAO_MM_ADD_ENTRY(&reference_array);

		/**
		 * Every route is abstracted as a Dao\Db\Reference instance
		 */
		object_init_ex(&reference, dao_db_reference_ce);
		DAO_CALL_METHOD(NULL, &reference, "__construct", &constraint_name, &reference_array);
		dao_array_update(return_value, &constraint_name, &reference, 0);
	} ZEND_HASH_FOREACH_END();

	RETURN_MM();
}

/**
 * Check whether the database system requires an explicit value for identity columns
 *
 * @return boolean
 */
PHP_METHOD(Dao_Db_Adapter_Pdo_Sqlite, useExplicitIdValue){


	RETURN_TRUE;
}

/**
 * Convert php bytea to database bytea
 *
 * @param string $value
 * @return string
 * @return string
 */
PHP_METHOD(Dao_Db_Adapter_Pdo_Sqlite, escapeBytea){

	zval *value;

	dao_fetch_params(0, 1, 0, &value);

	RETURN_CTOR(value);
}

/**
 * Convert database bytea to php bytea
 *
 * @param string $value
 * @return string
 */
PHP_METHOD(Dao_Db_Adapter_Pdo_Sqlite, unescapeBytea){

	zval *value;

	dao_fetch_params(0, 1, 0, &value);

	RETURN_CTOR(value);
}

/**
 * Convert php array to database array
 *
 * @param array $value
 * @return string
 */
PHP_METHOD(Dao_Db_Adapter_Pdo_Sqlite, escapeArray){

	zval *value, *type = NULL;

	dao_fetch_params(0, 1, 1, &value, &type);

	RETURN_CTOR(value);
}

/**
 * Convert database array to php array
 *
 * @param string $value
 * @return array
 */
PHP_METHOD(Dao_Db_Adapter_Pdo_Sqlite, unescapeArray){

	zval *value, *type = NULL;

	dao_fetch_params(0, 1, 1, &value, &type);

	RETURN_CTOR(value);
}
