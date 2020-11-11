
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

#include "db/result/pdo.h"
#include "db/resultinterface.h"
#include "db/exception.h"

#include <ext/pdo/php_pdo_driver.h>

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/exception.h"
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "kernel/string.h"
#include "kernel/array.h"
#include "kernel/concat.h"
#include "kernel/debug.h"

/**
 * Dao\Db\Result\Pdo
 *
 * Encapsulates the resultset internals
 *
 * <code>
 *	$result = $connection->query("SELECT * FROM robots ORDER BY name");
 *	$result->setFetchMode(Dao\Db::FETCH_NUM);
 *	while ($robot = $result->fetchArray()) {
 *		print_r($robot);
 *	}
 * </code>
 */
zend_class_entry *dao_db_result_pdo_ce;

PHP_METHOD(Dao_Db_Result_Pdo, __construct);
PHP_METHOD(Dao_Db_Result_Pdo, execute);
PHP_METHOD(Dao_Db_Result_Pdo, fetch);
PHP_METHOD(Dao_Db_Result_Pdo, fetchArray);
PHP_METHOD(Dao_Db_Result_Pdo, fetchAll);
PHP_METHOD(Dao_Db_Result_Pdo, numRows);
PHP_METHOD(Dao_Db_Result_Pdo, dataSeek);
PHP_METHOD(Dao_Db_Result_Pdo, setFetchMode);
PHP_METHOD(Dao_Db_Result_Pdo, getInternalResult);
PHP_METHOD(Dao_Db_Result_Pdo, nextRowset);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_db_result___construct, 0, 0, 2)
	ZEND_ARG_INFO(0, connection)
	ZEND_ARG_INFO(0, result)
	ZEND_ARG_INFO(0, sqlStatement)
	ZEND_ARG_INFO(0, bindParams)
	ZEND_ARG_INFO(0, bindTypes)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_db_result_pdo_method_entry[] = {
	PHP_ME(Dao_Db_Result_Pdo, __construct, arginfo_dao_db_result___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Db_Result_Pdo, execute, arginfo_dao_db_resultinterface_execute, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Result_Pdo, fetch, arginfo_dao_db_resultinterface_fetch, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Result_Pdo, fetchArray, arginfo_dao_db_resultinterface_fetcharray, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Result_Pdo, fetchAll, arginfo_dao_db_resultinterface_fetchall, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Result_Pdo, numRows, arginfo_dao_db_resultinterface_numrows, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Result_Pdo, dataSeek, arginfo_dao_db_resultinterface_dataseek, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Result_Pdo, setFetchMode, arginfo_dao_db_resultinterface_setfetchmode, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Result_Pdo, getInternalResult, arginfo_dao_db_resultinterface_getinternalresult, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Result_Pdo, nextRowset, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Db\Result\Pdo initializer
 */
DAO_INIT_CLASS(Dao_Db_Result_Pdo){

	DAO_REGISTER_CLASS(Dao\\Db\\Result, Pdo, db_result_pdo, dao_db_result_pdo_method_entry, 0);

	zend_declare_property_null(dao_db_result_pdo_ce, SL("_connection"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_db_result_pdo_ce, SL("_result"), ZEND_ACC_PROTECTED);
	zend_declare_property_long(dao_db_result_pdo_ce, SL("_fetchMode"), PDO_FETCH_OBJ, ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_db_result_pdo_ce, SL("_pdoStatement"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_db_result_pdo_ce, SL("_sqlStatement"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_db_result_pdo_ce, SL("_bindParams"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_db_result_pdo_ce, SL("_bindTypes"), ZEND_ACC_PROTECTED);
	zend_declare_property_bool(dao_db_result_pdo_ce, SL("_rowCount"), 0, ZEND_ACC_PROTECTED);

	return SUCCESS;
}

/**
 * Dao\Db\Result\Pdo constructor
 *
 * @param Dao\Db\AdapterInterface $connection
 * @param string $sqlStatement
 * @param array $bindParams
 * @param array $bindTypes
 * @param \PDOStatement $result
 */
PHP_METHOD(Dao_Db_Result_Pdo, __construct){

	zval *connection, *result, *sql_statement = NULL, *bind_params = NULL, *bind_types = NULL;

	dao_fetch_params(0, 2, 3, &connection, &result, &sql_statement, &bind_params, &bind_types);

	if (!sql_statement) {
		sql_statement = &DAO_GLOBAL(z_null);
	}

	if (!bind_params) {
		bind_params = &DAO_GLOBAL(z_null);
	}

	if (!bind_types) {
		bind_types = &DAO_GLOBAL(z_null);
	}

	if (Z_TYPE_P(result) != IS_OBJECT) {
		DAO_THROW_EXCEPTION_STR(dao_db_exception_ce, "Invalid PDOStatement supplied to Dao\\Db\\Result\\Pdo");
		return;
	}
	dao_update_property(getThis(), SL("_connection"), connection);
	dao_update_property(getThis(), SL("_pdoStatement"), result);
	if (Z_TYPE_P(sql_statement) != IS_NULL) {
		dao_update_property(getThis(), SL("_sqlStatement"), sql_statement);
	}

	if (Z_TYPE_P(bind_params) != IS_NULL) {
		dao_update_property(getThis(), SL("_bindParams"), bind_params);
	}

	if (Z_TYPE_P(bind_types) != IS_NULL) {
		dao_update_property(getThis(), SL("_bindTypes"), bind_types);
	}
}

/**
 * Allows to executes the statement again. Some database systems don't support scrollable cursors,
 * So, as cursors are forward only, we need to execute the cursor again to fetch rows from the begining
 *
 * @return boolean
 */
PHP_METHOD(Dao_Db_Result_Pdo, execute){

	zval pdo_statement = {};
	dao_read_property(&pdo_statement, getThis(), SL("_pdoStatement"), PH_NOISY|PH_READONLY);
	DAO_RETURN_CALL_METHOD(&pdo_statement, "execute");
}

/**
 * Fetches an array/object of strings that corresponds to the fetched row, or FALSE if there are no more rows.
 * This method is affected by the active fetch flag set using Dao\Db\Result\Pdo::setFetchMode
 *
 *<code>
 *	$result = $connection->query("SELECT * FROM robots ORDER BY name");
 *	$result->setFetchMode(Dao\Db::FETCH_OBJ);
 *	while ($robot = $result->fetch()) {
 *		echo $robot->name;
 *	}
 *</code>
 *
 * @return mixed
 */
PHP_METHOD(Dao_Db_Result_Pdo, fetch){

	zval *fetch_style = NULL, *cursor_orientation = NULL, *cursor_offset = NULL, pdo_statement = {};

	dao_fetch_params(0, 0, 3, &fetch_style, &cursor_orientation, &cursor_offset);

	if (!fetch_style) {
		fetch_style = &DAO_GLOBAL(z_null);
	}

	if (!cursor_orientation) {
		cursor_orientation = &DAO_GLOBAL(z_null);
	}

	if (!cursor_offset) {
		cursor_offset = &DAO_GLOBAL(z_null);
	}

	dao_read_property(&pdo_statement, getThis(), SL("_pdoStatement"), PH_NOISY|PH_READONLY);
	if (Z_TYPE_P(fetch_style) != IS_NULL) {
		if (Z_TYPE_P(cursor_orientation) != IS_NULL) {
			if (Z_TYPE_P(cursor_offset) != IS_NULL) {
				DAO_RETURN_CALL_METHOD(&pdo_statement, "fetch", fetch_style, cursor_orientation, cursor_offset);
			} else {
				DAO_RETURN_CALL_METHOD(&pdo_statement, "fetch", fetch_style, cursor_orientation);
			}
		} else {
			DAO_RETURN_CALL_METHOD(&pdo_statement, "fetch", fetch_style);
		}
	} else {
		DAO_RETURN_CALL_METHOD(&pdo_statement, "fetch");
	}
}

/**
 * Returns an array of strings that corresponds to the fetched row, or FALSE if there are no more rows.
 * This method is affected by the active fetch flag set using Dao\Db\Result\Pdo::setFetchMode
 *
 *<code>
 *	$result = $connection->query("SELECT * FROM robots ORDER BY name");
 *	$result->setFetchMode(Dao\Db::FETCH_NUM);
 *	while ($robot = $result->fetchArray()) {
 *		print_r($robot);
 *	}
 *</code>
 *
 * @return mixed
 */
PHP_METHOD(Dao_Db_Result_Pdo, fetchArray){

	zval pdo_statement = {};
	dao_read_property(&pdo_statement, getThis(), SL("_pdoStatement"), PH_NOISY|PH_READONLY);
	DAO_RETURN_CALL_METHOD(&pdo_statement, "fetch");
}

/**
 * Returns an array of arrays containing all the records in the result
 * This method is affected by the active fetch flag set using Dao\Db\Result\Pdo::setFetchMode
 *
 *<code>
 *	$result = $connection->query("SELECT * FROM robots ORDER BY name");
 *	$robots = $result->fetchAll();
 *</code>
 *
 * @param int $fetchMode
 * @param mixed $fetchArgument
 * @param array $ctorArgs
 * @return array
 */
PHP_METHOD(Dao_Db_Result_Pdo, fetchAll){

	zval *fetch_mode = NULL, *fetch_argument = NULL, *ctor_args = NULL, pdo_statement = {};

	dao_fetch_params(0, 0, 3, &fetch_mode, &fetch_argument, &ctor_args);

	if (!fetch_mode) {
		fetch_mode = &DAO_GLOBAL(z_null);
	}

	if (!fetch_argument) {
		fetch_argument = &DAO_GLOBAL(z_null);
	}

	if (!ctor_args) {
		ctor_args = &DAO_GLOBAL(z_null);
	}

	dao_read_property(&pdo_statement, getThis(), SL("_pdoStatement"), PH_NOISY|PH_READONLY);
	if (DAO_IS_NOT_TYPE(fetch_mode, IS_NULL)) {
		if (DAO_IS_NOT_TYPE(fetch_argument, IS_NULL)) {
			if (DAO_IS_NOT_TYPE(ctor_args, IS_NULL)) {
				DAO_RETURN_CALL_METHOD(&pdo_statement, "fetchall", fetch_mode, fetch_argument, ctor_args);
			} else {
				DAO_RETURN_CALL_METHOD(&pdo_statement, "fetchall", fetch_mode, fetch_argument);
			}
		} else {
			DAO_RETURN_CALL_METHOD(&pdo_statement, "fetchall", fetch_mode);
		}
	} else {
		DAO_RETURN_CALL_METHOD(&pdo_statement, "fetchall");
	}
}

/**
 * Gets number of rows returned by a resulset
 *
 *<code>
 *	$result = $connection->query("SELECT * FROM robots ORDER BY name");
 *	echo 'There are ', $result->numRows(), ' rows in the resulset';
 *</code>
 *
 * @return int
 */
PHP_METHOD(Dao_Db_Result_Pdo, numRows){

	zval row_count = {}, connection = {}, type = {}, pdo_statement = {}, sql_statement = {}, bind_params = {}, bind_types = {};
	zval matches = {}, pattern = {}, match = {}, else_clauses = {}, result = {}, row = {};

	dao_read_property(&row_count, getThis(), SL("_rowCount"), PH_READONLY);

	DAO_MM_INIT();

	if (DAO_IS_FALSE(&row_count)) {
		dao_read_property(&connection, getThis(), SL("_connection"), PH_NOISY|PH_READONLY);

		DAO_MM_CALL_METHOD(&type, &connection, "gettype");
		DAO_MM_ADD_ENTRY(&type);

		/**
		 * MySQL/PostgreSQL library property returns the number of records
		 */
		if (DAO_IS_STRING(&type, "mysql") || DAO_IS_STRING(&type, "pgsql")) {
			dao_read_property(&pdo_statement, getThis(), SL("_pdoStatement"), PH_NOISY|PH_READONLY);
			DAO_MM_CALL_METHOD(&row_count, &pdo_statement, "rowcount");
			DAO_MM_ADD_ENTRY(&row_count);
		}

		/**
		 * We should get the count using a new statement :(
		 */
		if (DAO_IS_FALSE(&row_count)) {
			/**
			 * SQLite/Oracle/SQLServer returns resultsets that to the client eyes (PDO) has an
			 * arbitrary number of rows, so we need to perform an extra count to know that
			 */
			dao_read_property(&sql_statement, getThis(), SL("_sqlStatement"), PH_NOISY|PH_READONLY);

			/**
			 * If the sql_statement starts with SELECT COUNT(*) we don't make the count
			 */
			if (!dao_start_with_str(&sql_statement, SL("SELECT COUNT(*) "))) {
				dao_read_property(&bind_params, getThis(), SL("_bindParams"), PH_NOISY|PH_READONLY);
				dao_read_property(&bind_types, getThis(), SL("_bindTypes"), PH_NOISY|PH_READONLY);

				DAO_MM_ZVAL_STRING(&pattern, "/^SELECT\\s+(.*)$/i");

				ZVAL_NULL(&matches);
				RETURN_ON_FAILURE(dao_preg_match(&match, &pattern, &sql_statement, &matches, 0, 0));
				DAO_MM_ADD_ENTRY(&matches);

				if (zend_is_true(&match)) {
					zval sql = {};
					dao_array_fetch_long(&else_clauses, &matches, 1, PH_NOISY|PH_READONLY);

					DAO_CONCAT_SVS(&sql, "SELECT COUNT(*) \"numrows\" FROM (SELECT ", &else_clauses, ")");
					DAO_MM_ADD_ENTRY(&sql);

					DAO_MM_CALL_METHOD(&result, &connection, "query", &sql, &bind_params, &bind_types);
					DAO_MM_ADD_ENTRY(&result);

					DAO_MM_CALL_METHOD(&row, &result, "fetch");
					DAO_MM_ADD_ENTRY(&row);

					dao_array_fetch_str(&row_count, &row, SL("numrows"), PH_NOISY|PH_COPY);
					DAO_MM_ADD_ENTRY(&row_count);
				}
			} else {
				ZVAL_LONG(&row_count, 1);
			}
		}

		/**
		 * Update the value to avoid further calculations
		 */
		dao_update_property(getThis(), SL("_rowCount"), &row_count);
	}

	RETURN_MM_CTOR(&row_count);
}

/**
 * Moves internal resulset cursor to another position letting us to fetch a certain row
 *
 *<code>
 *	$result = $connection->query("SELECT * FROM robots ORDER BY name");
 *	$result->dataSeek(2); // Move to third row on result
 *	$row = $result->fetch(); // Fetch third row
 *</code>
 *
 * @param int $number
 */
PHP_METHOD(Dao_Db_Result_Pdo, dataSeek){

	zval *num, connection = {}, pdo = {}, sql_statement = {}, bind_params = {}, bind_types = {}, statement = {}, temp_statement = {};
	pdo_stmt_t *stmt;
	long number = 0, n;

	dao_fetch_params(0, 1, 0, &num);

	number = dao_get_intval(num);
	dao_read_property(&connection, getThis(), SL("_connection"), PH_NOISY|PH_READONLY);

	DAO_CALL_METHOD(&pdo, &connection, "getinternalhandler");

	dao_read_property(&sql_statement, getThis(), SL("_sqlStatement"), PH_NOISY|PH_READONLY);
	dao_read_property(&bind_params, getThis(), SL("_bindParams"), PH_NOISY|PH_READONLY);

	/**
	 * PDO doesn't support scrollable cursors, so we need to re-execute the statement again
	 */
	if (Z_TYPE(bind_params) == IS_ARRAY) {
		dao_read_property(&bind_types, getThis(), SL("_bindTypes"), PH_NOISY|PH_READONLY);

		DAO_CALL_METHOD(&statement, &pdo, "prepare", &sql_statement);
		if (Z_TYPE(statement) == IS_OBJECT) {
			DAO_CALL_METHOD(&temp_statement, &connection, "executeprepared", &statement, &bind_params, &bind_types);
			zval_ptr_dtor(&statement);
			ZVAL_COPY_VALUE(&statement, &temp_statement);
		}

	} else {
		DAO_CALL_METHOD(&statement, &pdo, "query", &sql_statement);
	}
	zval_ptr_dtor(&pdo);

	dao_update_property(getThis(), SL("_pdoStatement"), &statement);
	zval_ptr_dtor(&statement);

	/**
	 * This a fetch scroll to reach the desired position, however with a big number of records
	 * maybe it may be very slow
	 */

	stmt = Z_PDO_STMT_P(&statement);
	if (!stmt->dbh) {
		RETURN_FALSE;
	}

	n = -1;
	number--;
	while (n != number) {

		if(!stmt->methods->fetcher(stmt, PDO_FETCH_ORI_NEXT, 0)) {
			RETURN_NULL();
		}

		n++;
	}
}

/**
 * Changes the fetching mode affecting Dao\Db\Result\Pdo::fetch()
 *
 *<code>
 *	//Return array with integer indexes
 *	$result->setFetchMode(Dao\Db::FETCH_NUM);
 *
 *	//Return associative array without integer indexes
 *	$result->setFetchMode(Dao\Db::FETCH_ASSOC);
 *
 *	//Return associative array together with integer indexes
 *	$result->setFetchMode(Dao\Db::FETCH_BOTH);
 *
 *	//Return an object
 *	$result->setFetchMode(Dao\Db::FETCH_OBJ);
 *</code>
 *
 * @param int $fetchMode
 */
PHP_METHOD(Dao_Db_Result_Pdo, setFetchMode){

	zval *fetch_mode, *fetch_arg1 = NULL, *fetch_arg2 = NULL, pdo_statement = {};

	dao_fetch_params(0, 1, 2, &fetch_mode, &fetch_arg1, &fetch_arg2);
	DAO_SEPARATE_PARAM(fetch_mode);

	if (Z_TYPE_P(fetch_mode) != IS_LONG) {
		convert_to_long(fetch_mode);
	}

	dao_read_property(&pdo_statement, getThis(), SL("_pdoStatement"), PH_NOISY|PH_READONLY);

	if (Z_LVAL_P(fetch_mode) != 0) {
		if (fetch_arg1 && fetch_arg2) {
			DAO_CALL_METHOD(NULL, &pdo_statement, "setfetchmode", fetch_mode, fetch_arg1, fetch_arg2);
		} else if (fetch_arg1) {
			DAO_CALL_METHOD(NULL, &pdo_statement, "setfetchmode", fetch_mode, fetch_arg1);
		} else if (fetch_arg2) {
			DAO_CALL_METHOD(NULL, &pdo_statement, "setfetchmode", fetch_mode, fetch_arg2);
		} else {
			DAO_CALL_METHOD(NULL, &pdo_statement, "setfetchmode", fetch_mode);
		}

		dao_update_property_long(getThis(), SL("_fetchMode"), Z_LVAL_P(fetch_mode));
	}

	RETURN_NULL();
}

/**
 * Gets the internal PDO result object
 *
 * @return \PDOStatement
 */
PHP_METHOD(Dao_Db_Result_Pdo, getInternalResult){


	RETURN_MEMBER(getThis(), "_pdoStatement");
}

/**
 * Advances to the next rowset in a multi-rowset statement handle
 *
 * @return boolean
 */
PHP_METHOD(Dao_Db_Result_Pdo, nextRowset){

	zval pdo_statement = {};

	dao_read_property(&pdo_statement, getThis(), SL("_pdoStatement"), PH_NOISY|PH_READONLY);
	DAO_RETURN_CALL_METHOD(&pdo_statement, "nextrowset");
}
