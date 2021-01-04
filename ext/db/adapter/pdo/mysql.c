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
  |          ZhuZongXin <dreamsxin@qq.com>                                 |
  +------------------------------------------------------------------------+
*/

#include "db/adapter/pdo/mysql.h"
#include "db/adapter/pdo.h"
#include "db/adapterinterface.h"
#include "db/column.h"

#include <ext/pdo/php_pdo_driver.h>

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/concat.h"
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/string.h"
#include "kernel/operators.h"

/**
 * Dao\Db\Adapter\Pdo\Mysql
 *
 * Specific functions for the Mysql database system
 *
 *<code>
 *
 *	$config = array(
 *		"host" => "192.168.0.11",
 *		"dbname" => "blog",
 *		"port" => 3306,
 *		"username" => "sigma",
 *		"password" => "secret"
 *	);
 *
 *	$connection = new Dao\Db\Adapter\Pdo\Mysql($config);
 *
 *</code>
 */
zend_class_entry *dao_db_adapter_pdo_mysql_ce;

PHP_METHOD(Dao_Db_Adapter_Pdo_Mysql, escapeIdentifier);
PHP_METHOD(Dao_Db_Adapter_Pdo_Mysql, describeColumns);
PHP_METHOD(Dao_Db_Adapter_Pdo_Mysql, escapeBytea);
PHP_METHOD(Dao_Db_Adapter_Pdo_Mysql, unescapeBytea);
PHP_METHOD(Dao_Db_Adapter_Pdo_Mysql, escapeArray);
PHP_METHOD(Dao_Db_Adapter_Pdo_Mysql, unescapeArray);

static const zend_function_entry dao_db_adapter_pdo_mysql_method_entry[] = {
	PHP_ME(Dao_Db_Adapter_Pdo_Mysql, escapeIdentifier, arginfo_dao_db_adapterinterface_escapeidentifier, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Adapter_Pdo_Mysql, describeColumns, arginfo_dao_db_adapterinterface_describecolumns, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Adapter_Pdo_Mysql, escapeBytea, arginfo_dao_db_adapterinterface_escapebytea, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Adapter_Pdo_Mysql, unescapeBytea, arginfo_dao_db_adapterinterface_unescapebytea, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Adapter_Pdo_Mysql, escapeArray, arginfo_dao_db_adapterinterface_escapearray, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Adapter_Pdo_Mysql, unescapeArray, arginfo_dao_db_adapterinterface_unescapearray, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Db\Adapter\Pdo\Mysql initializer
 */
DAO_INIT_CLASS(Dao_Db_Adapter_Pdo_Mysql){

	DAO_REGISTER_CLASS_EX(Dao\\Db\\Adapter\\Pdo, Mysql, db_adapter_pdo_mysql, dao_db_adapter_pdo_ce, dao_db_adapter_pdo_mysql_method_entry, 0);

	zend_declare_property_string(dao_db_adapter_pdo_mysql_ce, SL("_type"), "mysql", ZEND_ACC_PROTECTED);
	zend_declare_property_string(dao_db_adapter_pdo_mysql_ce, SL("_dialectType"), "mysql", ZEND_ACC_PROTECTED);

	zend_class_implements(dao_db_adapter_pdo_mysql_ce, 1, dao_db_adapterinterface_ce);

	return SUCCESS;
}

/**
 * Escapes a column/table/schema name
 *
 * @param string $identifier
 * @return string
 */
PHP_METHOD(Dao_Db_Adapter_Pdo_Mysql, escapeIdentifier){

	zval *identifier, domain = {}, name = {};

	dao_fetch_params(0, 1, 0, &identifier);

	if (Z_TYPE_P(identifier) == IS_ARRAY) {
		dao_array_fetch_long(&domain, identifier, 0, PH_NOISY|PH_READONLY);
		dao_array_fetch_long(&name, identifier, 1, PH_NOISY|PH_READONLY);
		if (DAO_GLOBAL(db).escape_identifiers) {
			DAO_CONCAT_SVSVS(return_value, "`", &domain, "`.`", &name, "`");
			return;
		}

		DAO_CONCAT_VSV(return_value, &domain, ".", &name);

		return;
	}
	if (DAO_GLOBAL(db).escape_identifiers) {
		DAO_CONCAT_SVS(return_value, "`", identifier, "`");
		return;
	}

	RETURN_CTOR(identifier);
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
PHP_METHOD(Dao_Db_Adapter_Pdo_Mysql, describeColumns){

	zval *table, *_schema = NULL, schema = {}, dialect = {}, sql = {}, fetch_num = {}, describe = {}, size_pattern = {}, *field, old_column = {};

	dao_fetch_params(1, 1, 1, &table, &_schema);

	if (!_schema || !zend_is_true(_schema)) {
		dao_read_property(&schema, getThis(), SL("_schema"), PH_NOISY|PH_READONLY);
	} else {
		ZVAL_COPY_VALUE(&schema, _schema);
	}

	dao_read_property(&dialect, getThis(), SL("_dialect"), PH_NOISY|PH_READONLY);

	/**
	 * Get the SQL to describe a table
	 */
	DAO_MM_CALL_METHOD(&sql, &dialect, "describecolumns", table, &schema);
	DAO_MM_ADD_ENTRY(&sql);

	/**
	 * We're using FETCH_NUM to fetch the columns
	 */
	ZVAL_LONG(&fetch_num, PDO_FETCH_NUM);

	/**
	 * Get the describe
	 */
	DAO_MM_CALL_METHOD(&describe, getThis(), "fetchall", &sql, &fetch_num);
	DAO_MM_ADD_ENTRY(&describe);

	array_init(return_value);

	DAO_MM_ZVAL_STRING(&size_pattern, "#\\(([0-9]++)(?:,\\s*([0-9]++))?\\)#");

	/**
	 * Field Indexes: 0:name, 1:type, 2:not null, 3:key, 4:default, 5:extra
	 */
	ZEND_HASH_FOREACH_VAL(Z_ARRVAL(describe), field) {
		zval definition = {}, column_type = {}, pos = {}, matches = {}, match_one = {}, match_two = {}, attribute = {}, column_name = {}, column = {};

		/**
		 * By default the bind types is two
		 */
		array_init(&definition);
		DAO_MM_ADD_ENTRY(&definition);
		add_assoc_long_ex(&definition, SL("bindType"), DAO_DB_COLUMN_BIND_PARAM_STR);

		/**
		 * By checking every column type we convert it to a Dao\Db\Column
		 */
		dao_array_fetch_long(&column_type, field, 1, PH_NOISY|PH_READONLY);

		/**
		 * If the column type has a parentheses we try to get the column size from it
		 */
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
			 * Point are varchars
			 */
			if (dao_memnstr_str(&column_type, SL("point"))) {
				dao_array_update_str_long(&definition, SL("type"), DAO_DB_COLUMN_TYPE_VARCHAR, 0);
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
			 * Smallint
			 */
			if (dao_memnstr_str(&column_type, SL("smallint"))) {
				dao_array_update_str_long(&definition, SL("type"), DAO_DB_COLUMN_TYPE_INTEGER, 0);
				dao_array_update_str(&definition, SL("isNumeric"), &DAO_GLOBAL(z_true), PH_COPY);
				dao_array_update_str_long(&definition, SL("bindType"), DAO_DB_COLUMN_BIND_PARAM_INT, 0);
				dao_array_update_str_long(&definition, SL("bytes"), 2, 0);
				break;
			}

			/**
			 * Mediumint
			 */
			if (dao_memnstr_str(&column_type, SL("mediumint"))) {
				dao_array_update_str_long(&definition, SL("type"), DAO_DB_COLUMN_TYPE_INTEGER, 0);
				dao_array_update_str(&definition, SL("isNumeric"), &DAO_GLOBAL(z_true), PH_COPY);
				dao_array_update_str_long(&definition, SL("bindType"), DAO_DB_COLUMN_BIND_PARAM_INT, 0);
				dao_array_update_str_long(&definition, SL("bytes"), 3, 0);
				break;
			}

			/**
			 * BIGINT
			 */
			if (dao_memnstr_str(&column_type, SL("bigint"))) {
				dao_array_update_str_long(&definition, SL("type"), DAO_DB_COLUMN_TYPE_BIGINTEGER, 0);
				dao_array_update_str(&definition, SL("isNumeric"), &DAO_GLOBAL(z_true), PH_COPY);
				dao_array_update_str_long(&definition, SL("bindType"), DAO_DB_COLUMN_BIND_PARAM_INT, 0);
				dao_array_update_str_long(&definition, SL("bytes"), 64, 0);
				break;
			}

			/**
			 * Integers/Int are int
			 */
			if (dao_memnstr_str(&column_type, SL("int"))) {
				dao_array_update_str_long(&definition, SL("type"), DAO_DB_COLUMN_TYPE_INTEGER, 0);
				dao_array_update_str(&definition, SL("isNumeric"), &DAO_GLOBAL(z_true), PH_COPY);
				dao_array_update_str_long(&definition, SL("bindType"), 1, 0);
				dao_array_update_str_long(&definition, SL("bytes"), 4, 0);
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
			 * Special type for datetime
			 */
			if (dao_memnstr_str(&column_type, SL("datetime"))) {
				dao_array_update_str_long(&definition, SL("type"), DAO_DB_COLUMN_TYPE_DATETIME, 0);
				break;
			}

			/**
			 * Decimals are floats
			 */
			if (dao_memnstr_str(&column_type, SL("decimal"))) {
				dao_array_update_str_long(&definition, SL("type"), DAO_DB_COLUMN_TYPE_DECIMAL, 0);
				dao_array_update_str(&definition, SL("isNumeric"), &DAO_GLOBAL(z_true), PH_COPY);
				dao_array_update_str_long(&definition, SL("bindType"), DAO_DB_COLUMN_BIND_PARAM_DECIMAL, 0);
				if (dao_is_numeric(&match_one)) {
					if (dao_is_numeric(&match_two) && DAO_GT(&match_two, &match_one)) {
						dao_array_update_str_long(&definition, SL("bytes"), (dao_get_intval(&match_two) + 2), 0);
					} else {
						dao_array_update_str_long(&definition, SL("bytes"), dao_get_intval(&match_one), 0);
					}
				} else {
					dao_array_update_str_long(&definition, SL("size"), 30, 0);
					dao_array_update_str_long(&definition, SL("bytes"), 10, 0);
				}
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
				dao_array_update_str_long(&definition, SL("type"), DAO_DB_COLUMN_TYPE_DATE, 0);
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
			 * Float/Smallfloats/Decimals are float
			 */
			if (dao_memnstr_str(&column_type, SL("float"))) {
				dao_array_update_str_long(&definition, SL("type"), DAO_DB_COLUMN_TYPE_FLOAT, 0);
				dao_array_update_str(&definition, SL("isNumeric"), &DAO_GLOBAL(z_true), PH_COPY);
				dao_array_update_str_long(&definition, SL("bindType"), DAO_DB_COLUMN_BIND_PARAM_DECIMAL, 0);
				if (dao_is_numeric(&match_one) && DAO_GE_LONG(&match_one, 25)) {
					dao_array_update_str_long(&definition, SL("bytes"), 8, 0);
				} else {
					dao_array_update_str_long(&definition, SL("bytes"), 4, 0);
				}
				break;
			}

			/**
			 * Double are floats
			 */
			if (dao_memnstr_str(&column_type, SL("double"))) {
				dao_array_update_str_long(&definition, SL("type"), DAO_DB_COLUMN_TYPE_DOUBLE, 0);
				dao_array_update_str(&definition, SL("isNumeric"), &DAO_GLOBAL(z_true), PH_COPY);
				dao_array_update_str_long(&definition, SL("bindType"), DAO_DB_COLUMN_BIND_PARAM_DECIMAL, 0);
				dao_array_update_str_long(&definition, SL("bytes"), 8, 0);
				break;
			}

			/**
			 * Boolean
			 */
			if (dao_memnstr_str(&column_type, SL("bit"))) {
				dao_array_update_str_long(&definition, SL("type"), DAO_DB_COLUMN_TYPE_BOOLEAN, 0);
				dao_array_update_str_long(&definition, SL("bindType"), DAO_DB_COLUMN_BIND_PARAM_BOOL, 0);
				break;
			}

			/**
			 * TINYBLOB
			 */
			if (dao_memnstr_str(&column_type, SL("tinyblob"))) {
				dao_array_update_str_long(&definition, SL("type"), DAO_DB_COLUMN_TYPE_TINYBLOB, 0);
				break;
			}

			/**
			 * MEDIUMBLOB
			 */
			if (dao_memnstr_str(&column_type, SL("mediumblob"))) {
				dao_array_update_str_long(&definition, SL("type"), DAO_DB_COLUMN_TYPE_MEDIUMBLOB, 0);
				break;
			}

			/**
			 * LONGBLOB
			 */
			if (dao_memnstr_str(&column_type, SL("longblob"))) {
				dao_array_update_str_long(&definition, SL("type"), DAO_DB_COLUMN_TYPE_LONGBLOB, 0);
				break;
			}

			/**
			 * BLOB
			 */
			if (dao_memnstr_str(&column_type, SL("blob"))) {
				dao_array_update_str_long(&definition, SL("type"), DAO_DB_COLUMN_TYPE_BLOB, 0);
				break;
			}

			/**
			 * JSON
			 */
			if (dao_memnstr_str(&column_type, SL("json"))) {
				dao_array_update_str_long(&definition, SL("type"), DAO_DB_COLUMN_TYPE_JSON, 0);
				break;
			}

			/**
			 * By default is string
			 */
			dao_array_update_str_long(&definition, SL("type"), DAO_DB_COLUMN_TYPE_VARCHAR, 0);
			break;
		}

		/**
		 * Check if the column is unsigned, only MySQL support this
		 */
		if (dao_memnstr_str(&column_type, SL("unsigned"))) {
			dao_array_update_str(&definition, SL("unsigned"), &DAO_GLOBAL(z_true), PH_COPY);
		}

		/**
		 * Positions
		 */
		if (Z_TYPE(old_column) <= IS_NULL) {
			dao_array_update_str(&definition, SL("first"), &DAO_GLOBAL(z_true), PH_COPY);
		} else {
			dao_array_update_str(&definition, SL("after"), &old_column, PH_COPY);
		}

		/**
		 * Check if the field is primary key
		 */
		dao_array_fetch_long(&attribute, field, 3, PH_NOISY|PH_READONLY);
		if (DAO_IS_STRING(&attribute, "PRI")) {
			dao_array_update_str(&definition, SL("primary"), &DAO_GLOBAL(z_true), PH_COPY);
		}

		/**
		 * Check if the column allows null values
		 */
		dao_array_fetch_long(&attribute, field, 2, PH_NOISY|PH_READONLY);
		if (DAO_IS_STRING(&attribute, "NO")) {
			dao_array_update_str(&definition, SL("notNull"), &DAO_GLOBAL(z_true), PH_COPY);
		}

		/**
		 * Check if the column is auto increment
		 */
		dao_array_fetch_long(&attribute, field, 5, PH_NOISY|PH_READONLY);
		if (DAO_IS_STRING(&attribute, "auto_increment")) {
			dao_array_update_str(&definition, SL("autoIncrement"), &DAO_GLOBAL(z_true), PH_COPY);
		}

		dao_array_fetch_long(&column_name, field, 0, PH_NOISY|PH_READONLY);

		/**
		 * If the column set the default values, get it
		 */
		dao_array_fetch_long(&attribute, field, 4, PH_NOISY|PH_READONLY);
		if (Z_TYPE(attribute) == IS_STRING) {
			dao_array_update_str(&definition, SL("default"), &attribute, PH_COPY);
		}

		/**
		 * Every route is stored as a Dao\Db\Column
		 */
		object_init_ex(&column, dao_db_column_ce);
		DAO_MM_CALL_METHOD(NULL, &column, "__construct", &column_name, &definition);

		dao_array_append(return_value, &column, 0);

		ZVAL_COPY_VALUE(&old_column, &column_name);
	} ZEND_HASH_FOREACH_END();
	RETURN_MM();
}

/**
 * Convert php bytea to database bytea
 *
 * @param string $value
 * @return string
 * @return string
 */
PHP_METHOD(Dao_Db_Adapter_Pdo_Mysql, escapeBytea){

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
PHP_METHOD(Dao_Db_Adapter_Pdo_Mysql, unescapeBytea){

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
PHP_METHOD(Dao_Db_Adapter_Pdo_Mysql, escapeArray){

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
PHP_METHOD(Dao_Db_Adapter_Pdo_Mysql, unescapeArray){

	zval *value, *type = NULL;

	dao_fetch_params(0, 1, 1, &value, &type);

	RETURN_CTOR(value);
}
