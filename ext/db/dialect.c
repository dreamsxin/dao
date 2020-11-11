
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

#include "db/dialect.h"
#include "db/dialectinterface.h"
#include "db/exception.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/operators.h"
#include "kernel/fcall.h"
#include "kernel/concat.h"
#include "kernel/object.h"
#include "kernel/array.h"
#include "kernel/string.h"
#include "kernel/exception.h"
#include "kernel/hash.h"
#include "kernel/debug.h"

#include "interned-strings.h"

/**
 * Dao\Db\Dialect
 *
 * This is the base class to each database dialect. This implements
 * common methods to transform intermediate code into its RDBM related syntax
 */
zend_class_entry *dao_db_dialect_ce;

PHP_METHOD(Dao_Db_Dialect, limit);
PHP_METHOD(Dao_Db_Dialect, forUpdate);
PHP_METHOD(Dao_Db_Dialect, sharedLock);
PHP_METHOD(Dao_Db_Dialect, getColumnList);
PHP_METHOD(Dao_Db_Dialect, getSqlExpression);
PHP_METHOD(Dao_Db_Dialect, getSqlExpressionCase);
PHP_METHOD(Dao_Db_Dialect, getSqlExpressionFunctionCall);
PHP_METHOD(Dao_Db_Dialect, getSqlTable);
PHP_METHOD(Dao_Db_Dialect, select);
PHP_METHOD(Dao_Db_Dialect, insert);
PHP_METHOD(Dao_Db_Dialect, update);
PHP_METHOD(Dao_Db_Dialect, delete);
PHP_METHOD(Dao_Db_Dialect, supportsSavepoints);
PHP_METHOD(Dao_Db_Dialect, supportsReleaseSavepoints);
PHP_METHOD(Dao_Db_Dialect, createSavepoint);
PHP_METHOD(Dao_Db_Dialect, releaseSavepoint);
PHP_METHOD(Dao_Db_Dialect, rollbackSavepoint);
PHP_METHOD(Dao_Db_Dialect, getEscapeChar);;
PHP_METHOD(Dao_Db_Dialect, registerCustomFunction);
PHP_METHOD(Dao_Db_Dialect, getCustomFunctions);
PHP_METHOD(Dao_Db_Dialect, escape);
PHP_METHOD(Dao_Db_Dialect, escapeSchema);
PHP_METHOD(Dao_Db_Dialect, prepareTable);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_db_dialect_getsqlexpression, 0, 0, 1)
	ZEND_ARG_INFO(0, expression)
	ZEND_ARG_INFO(0, escapeChar)
	ZEND_ARG_INFO(0, quoting)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_db_dialect_getsqlexpressioncase, 0, 0, 1)
	ZEND_ARG_INFO(0, expression)
	ZEND_ARG_INFO(0, escapeChar)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_db_dialect_getsqlexpressionfunctioncall, 0, 0, 1)
	ZEND_ARG_INFO(0, expression)
	ZEND_ARG_INFO(0, escapeChar)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_db_dialect_getsqltable, 0, 0, 1)
	ZEND_ARG_INFO(0, table)
	ZEND_ARG_INFO(0, escapeChar)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_db_dialect_registercustomfunction, 0, 0, 2)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, customFunction)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_db_dialect_escape, 0, 0, 1)
	ZEND_ARG_INFO(0, str)
	ZEND_ARG_INFO(0, escapeChar)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_db_dialect_escapeschema, 0, 0, 1)
	ZEND_ARG_INFO(0, schema)
	ZEND_ARG_INFO(0, escapeChar)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_db_dialect_preparetable, 0, 0, 1)
	ZEND_ARG_INFO(0, table)
	ZEND_ARG_INFO(0, schema)
	ZEND_ARG_INFO(0, alias)
	ZEND_ARG_INFO(0, escapeChar)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_db_dialect_method_entry[] = {
	PHP_ME(Dao_Db_Dialect, limit, arginfo_dao_db_dialectinterface_limit, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect, forUpdate, arginfo_dao_db_dialectinterface_forupdate, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect, sharedLock, arginfo_dao_db_dialectinterface_sharedlock, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect, getColumnList, arginfo_dao_db_dialectinterface_getcolumnlist, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect, getSqlExpression, arginfo_dao_db_dialect_getsqlexpression, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect, getSqlExpressionCase, arginfo_dao_db_dialect_getsqlexpressioncase, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect, getSqlExpressionFunctionCall, arginfo_dao_db_dialect_getsqlexpressionfunctioncall, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect, getSqlTable, arginfo_dao_db_dialect_getsqltable, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect, select, arginfo_dao_db_dialectinterface_select, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect, insert, arginfo_dao_db_dialectinterface_insert, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect, update, arginfo_dao_db_dialectinterface_update, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect, delete, arginfo_dao_db_dialectinterface_delete, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect, supportsSavepoints, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect, supportsReleaseSavepoints, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect, createSavepoint, arginfo_dao_db_dialectinterface_createsavepoint, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect, releaseSavepoint, arginfo_dao_db_dialectinterface_releasesavepoint, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect, rollbackSavepoint, arginfo_dao_db_dialectinterface_rollbacksavepoint, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect, getEscapeChar, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect, registerCustomFunction, arginfo_dao_db_dialect_registercustomfunction, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect, getCustomFunctions, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect, escape, arginfo_dao_db_dialect_escape, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect, escapeSchema, arginfo_dao_db_dialect_escapeschema, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Dialect, prepareTable, arginfo_dao_db_dialect_preparetable, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Db\Dialect initializer
 */
DAO_INIT_CLASS(Dao_Db_Dialect){

	DAO_REGISTER_CLASS(Dao\\Db, Dialect, db_dialect, dao_db_dialect_method_entry, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	zend_declare_property_null(dao_db_dialect_ce, SL("_escapeChar"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_db_dialect_ce, SL("_customFunctions"), ZEND_ACC_PROTECTED);

	zend_class_implements(dao_db_dialect_ce, 1, dao_db_dialectinterface_ce);

	return SUCCESS;
}

/**
 * Generates the SQL for LIMIT clause
 *
 *<code>
 * $sql = $dialect->limit('SELECT * FROM robots', 10);
 * echo $sql; // SELECT * FROM robots LIMIT 10
 *</code>
 *
 * @param string $sqlQuery
 * @param int $number
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect, limit){

	zval *sql_query, *number, limit = {};

	dao_fetch_params(0, 2, 0, &sql_query, &number);

	if (dao_is_numeric(number)) {
		ZVAL_LONG(&limit, dao_get_intval(number));
		DAO_CONCAT_VSV(return_value, sql_query, " LIMIT ", &limit);
		return;
	}

	RETURN_CTOR(sql_query);
}

/**
 * Returns a SQL modified with a FOR UPDATE clause
 *
 *<code>
 * $sql = $dialect->forUpdate('SELECT * FROM robots');
 * echo $sql; // SELECT * FROM robots FOR UPDATE
 *</code>
 *
 * @param string $sqlQuery
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect, forUpdate){

	zval *sql_query;

	dao_fetch_params(0, 1, 0, &sql_query);

	DAO_CONCAT_VS(return_value, sql_query, " FOR UPDATE");
}

/**
 * Returns a SQL modified with a LOCK IN SHARE MODE clause
 *
 *<code>
 * $sql = $dialect->sharedLock('SELECT * FROM robots');
 * echo $sql; // SELECT * FROM robots LOCK IN SHARE MODE
 *</code>
 *
 * @param string $sqlQuery
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect, sharedLock){

	zval *sql_query;

	dao_fetch_params(0, 1, 0, &sql_query);

	DAO_CONCAT_VS(return_value, sql_query, " LOCK IN SHARE MODE");
}

/**
 * Gets a list of columns with escaped identifiers
 *
 *<code>
 * echo $dialect->getColumnList(array('column1', 'column'));
 *</code>
 *
 * @param array $columnList
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect, getColumnList){

	zval *column_list, str_list = {}, escape_char = {}, *column;

	dao_fetch_params(0, 1, 0, &column_list);

	array_init(&str_list);

	dao_read_property(&escape_char, getThis(), SL("_escapeChar"), PH_NOISY|PH_READONLY);

	ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(column_list), column) {
		zval column_quoted = {};
		DAO_CONCAT_VVV(&column_quoted, &escape_char, column, &escape_char);
		dao_array_append(&str_list, &column_quoted, 0);
	} ZEND_HASH_FOREACH_END();

	dao_fast_join_str(return_value, SL(", "), &str_list);
	zval_ptr_dtor(&str_list);
}

/**
 * Return the escape char
 *
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect, getEscapeChar){


	RETURN_MEMBER(getThis(), "_escapeChar");
}

/**
 * Transforms an intermediate representation for a expression into a database system valid expression
 *
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect, getSqlExpression){

	zval *expression, *_escape_char = NULL, *quoting = NULL, escape_char = {}, type = {}, name = {}, escaped_name = {},  domain = {}, value = {}, expression_value = {}, operator = {};
	zval left = {}, right = {}, expression_left = {}, expression_right = {}, list_expression = {}, exception_message = {};

	dao_fetch_params(0, 1, 2, &expression, &_escape_char, &quoting);

	if (Z_TYPE_P(expression) != IS_ARRAY) {
		DAO_THROW_EXCEPTION_STR(dao_db_exception_ce, "Invalid SQL expression");
		return;
	}

	if (DAO_GLOBAL(db).escape_identifiers) {
		if (!_escape_char) {
			 dao_read_property(&escape_char, getThis(), SL("_escapeChar"), PH_READONLY);
		} else {
			ZVAL_COPY_VALUE(&escape_char, _escape_char);
		}
	} else {
		ZVAL_NULL(&escape_char);
	}

	if (!quoting) {
		quoting = &DAO_GLOBAL(z_false);
	}

	if (!dao_array_isset_fetch_str(&type, expression, SL("type"), PH_READONLY)) {
		DAO_THROW_EXCEPTION_STR(dao_db_exception_ce, "Invalid SQL expression");
		return;
	}

	/**
	 * Resolve qualified expressions
	 */
	if (DAO_IS_STRING(&type, "qualified")) {
		dao_array_fetch_str(&name, expression, SL("name"), PH_NOISY|PH_READONLY);

		DAO_CONCAT_VVV(&escaped_name, &escape_char, &name, &escape_char);

		/**
		 * A domain could be a table/schema
		 */
		if (dao_array_isset_fetch_str(&domain, expression, SL("domain"), PH_READONLY)) {
			DAO_CONCAT_VVVSV(return_value, &escape_char, &domain, &escape_char, ".", &escaped_name);
			zval_ptr_dtor(&escaped_name);
			return;
		}

		RETVAL_ZVAL(&escaped_name, 0, 0);
		return;
	}

	/**
	 * Resolve literal expressions
	 */
	if (DAO_IS_STRING(&type, "literal")) {
		dao_array_fetch_str(&value, expression, SL("value"), PH_NOISY|PH_READONLY);

		if (zend_is_true(quoting)) {
			DAO_CONCAT_SVS(return_value, "'", &value, "'");
			return;
		}

		RETURN_CTOR(&value);
	}

	/**
	 * Resolve binary operations expressions
	 */
	if (DAO_IS_STRING(&type, "binary-op")) {
		dao_array_fetch_str(&operator, expression, SL("op"), PH_NOISY|PH_READONLY);
		dao_array_fetch_str(&left, expression, SL("left"), PH_NOISY|PH_READONLY);
		dao_array_fetch_str(&right, expression, SL("right"), PH_NOISY|PH_READONLY);

		DAO_CALL_METHOD(&expression_left, getThis(), "getsqlexpression", &left, &escape_char);
		DAO_CALL_METHOD(&expression_right, getThis(), "getsqlexpression", &right, &escape_char);

		DAO_CONCAT_VSVSV(return_value, &expression_left, " ", &operator, " ", &expression_right);
		zval_ptr_dtor(&expression_left);
		zval_ptr_dtor(&expression_right);
		return;
	}

	/**
	 * Resolve unary operations expressions
	 */
	if (DAO_IS_STRING(&type, "unary-op")) {
		dao_array_fetch_str(&operator, expression, SL("op"), PH_NOISY|PH_READONLY);

		/**
		 * Some unary operators uses the left operand...
		 */
		if (dao_array_isset_fetch_str(&left, expression, SL("left"), PH_READONLY)) {
			DAO_CALL_METHOD(&expression_left, getThis(), "getsqlexpression", &left, &escape_char);
			DAO_CONCAT_VV(return_value, &expression_left, &operator);
			zval_ptr_dtor(&expression_left);
			return;
		}

		/**
		 * ...Others uses the right operand
		 */
		if (dao_array_isset_fetch_str(&right, expression, SL("right"), PH_READONLY)) {
			DAO_CALL_METHOD(&expression_right, getThis(), "getsqlexpression", &right, &escape_char);
			DAO_CONCAT_VV(return_value, &operator, &expression_right);
			zval_ptr_dtor(&expression_right);
			return;
		}
	}

	/**
	 * Resolve placeholder
	 */
	if (DAO_IS_STRING(&type, "placeholder")) {
		dao_array_fetch_str(&value, expression, SL("value"), PH_NOISY|PH_COPY);
/*
		// array array-int
		if (dao_array_isset_fetch_str(&times, expression, SL("times"), PH_READONLY)) {
			array_init(&placeholders);

			t = dao_get_intval(&times);
			i = 0;
			while (i++ < t) {
				zval index = {}, placeholder = {};
				ZVAL_LONG(&index, i);

				DAO_CONCAT_VV(&placeholder, &value, &index);

				dao_array_append(&placeholders, &placeholder, 0);
			}

			dao_fast_join_str(&value, SL(", "), &placeholders);
			zval_ptr_dtor(&placeholders);
		}
*/
		RETVAL_ZVAL(&value, 0, 0);
		return;
	}

	/**
	 * Resolve parentheses
	 */
	if (DAO_IS_STRING(&type, "parentheses")) {
		dao_array_fetch_str(&left, expression, SL("left"), PH_NOISY|PH_READONLY);

		DAO_CALL_METHOD(&expression_left, getThis(), "getsqlexpression", &left, &escape_char);

		DAO_CONCAT_SVS(return_value, "(", &expression_left, ")");
		zval_ptr_dtor(&expression_left);
		return;
	}

	/**
	 * Resolve function calls
	 */
	if (DAO_IS_STRING(&type, "functionCall")) {
		DAO_RETURN_CALL_METHOD(getThis(), "getsqlexpressionfunctioncall", expression, &escape_char);
		return;
	}

	/**
	 * Resolve lists
	 */
	if (DAO_IS_STRING(&type, "list")) {
		zval sql_items = {}, items = {}, *item;
		array_init(&sql_items);

		dao_array_fetch_long(&items, expression, 0, PH_NOISY|PH_READONLY);

		ZEND_HASH_FOREACH_VAL(Z_ARRVAL(items), item) {
			zval item_expression = {};
			DAO_CALL_METHOD(&item_expression, getThis(), "getsqlexpression", item, &escape_char);
			dao_array_append(&sql_items, &item_expression, 0);
		} ZEND_HASH_FOREACH_END();

		dao_fast_join_str(&list_expression, SL(", "), &sql_items);
		zval_ptr_dtor(&sql_items);
		DAO_CONCAT_SVS(return_value, "(", &list_expression, ")");
		zval_ptr_dtor(&list_expression);
		return;
	}

	/**
	 * Resolve *
	 */
	if (DAO_IS_STRING(&type, "all")) {
		RETURN_STRING("*");
	}

	/**
	 * Resolve CAST of values
	 */
	if (DAO_IS_STRING(&type, "cast")) {
		dao_array_fetch_str(&left, expression, SL("left"), PH_NOISY|PH_READONLY);
		dao_array_fetch_str(&right, expression, SL("right"), PH_NOISY|PH_READONLY);

		DAO_CALL_METHOD(&expression_left, getThis(), "getsqlexpression", &left, &escape_char);
		DAO_CALL_METHOD(&expression_right, getThis(), "getsqlexpression", &right, &escape_char);

		DAO_CONCAT_SVSVS(return_value, "CAST(", &expression_left, " AS ", &expression_right, ")");
		zval_ptr_dtor(&expression_left);
		zval_ptr_dtor(&expression_right);
		return;
	}

	/**
	 * Resolve CASE of values
	 */
	if (DAO_IS_STRING(&type, "case")) {
		DAO_RETURN_CALL_METHOD(getThis(), "getsqlexpressioncase", expression, &escape_char);
		return;
	}

	/**
	 * Resolve CONVERT of values encodings
	 */
	if (DAO_IS_STRING(&type, "convert")) {
		dao_array_fetch_str(&left, expression, SL("left"), PH_NOISY|PH_READONLY);
		dao_array_fetch_str(&right, expression, SL("right"), PH_NOISY|PH_READONLY);
		DAO_CALL_METHOD(&expression_left, getThis(), "getsqlexpression", &left, &escape_char);
		DAO_CALL_METHOD(&expression_right, getThis(), "getsqlexpression", &right, &escape_char);
		DAO_CONCAT_SVSVS(return_value, "CONVERT(", &expression_left, " USING ", &expression_right, ")");
		zval_ptr_dtor(&expression_left);
		zval_ptr_dtor(&expression_right);
		return;
	}

	/**
	 * Resolve SELECT
	 */
	if (DAO_IS_STRING(&type, "select")) {
		dao_array_fetch_str(&value, expression, SL("value"), PH_NOISY|PH_READONLY);
		DAO_CALL_METHOD(&expression_value, getThis(), "select", &value);
		DAO_CONCAT_SVS(return_value, "(", &expression_value, ")");
		zval_ptr_dtor(&expression_value);
		return;
	}

	/**
	 * Expression type wasn't found
	 */
	DAO_CONCAT_SVS(&exception_message, "Invalid SQL expression type '", &type, "'");
	DAO_THROW_EXCEPTION_ZVAL(dao_db_exception_ce, &exception_message);
}

/**
 * Resolve CASE expressions
 *
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect, getSqlExpressionCase){

	zval *expression, *escape_char = NULL, expr = {}, expr_tmp = {}, clauses = {},  *clause;

	dao_fetch_params(0, 1, 1, &expression, &escape_char);

	if (!escape_char) {
		escape_char = &DAO_GLOBAL(z_null);
	}

	ZVAL_STRING(return_value, "CASE ");

	dao_array_fetch_str(&expr, expression, SL("expr"), PH_NOISY|PH_READONLY);

	DAO_CALL_METHOD(&expr_tmp, getThis(), "getsqlexpression", &expr, escape_char);

	DAO_CONCAT_SV(return_value, "CASE ", &expr_tmp);
	zval_ptr_dtor(&expr_tmp);

	dao_array_fetch_str(&clauses, expression, SL("when-clauses"), PH_NOISY|PH_READONLY);

	ZEND_HASH_FOREACH_VAL(Z_ARRVAL(clauses), clause) {
		zval type = {}, clause_when = {}, clause_then = {}, clause_expr = {}, tmp = {}, tmp1 = {};

		dao_array_fetch_str(&type, clause, ISL(type), PH_NOISY|PH_READONLY);
		if (DAO_IS_STRING(&type, "when")) {
			dao_array_fetch_str(&clause_when, clause, SL("when"), PH_NOISY|PH_READONLY);
			dao_array_fetch_str(&clause_then, clause, SL("then"), PH_NOISY|PH_READONLY);

			DAO_CALL_METHOD(&tmp, getThis(), "getsqlexpression", &clause_when, escape_char);
			DAO_CALL_METHOD(&tmp1, getThis(), "getsqlexpression", &clause_then, escape_char);

			DAO_SCONCAT_SVSV(return_value, " WHEN ", &tmp, " THEN ", &tmp1);
			zval_ptr_dtor(&tmp);
			zval_ptr_dtor(&tmp1);
		} else {
			dao_array_fetch_str(&clause_expr, clause, ISL(expr), PH_NOISY|PH_READONLY);

			DAO_CALL_METHOD(&tmp, getThis(), "getsqlexpression", &clause_expr, escape_char);

			DAO_SCONCAT_SV(return_value, " ELSE ", &tmp);
			zval_ptr_dtor(&tmp);
		}
	} ZEND_HASH_FOREACH_END();

	DAO_SCONCAT_STR(return_value, " END");
}

/**
 * Resolve function calls
 *
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect, getSqlExpressionFunctionCall){

	zval *expression, *escape_char = NULL, name = {}, custom_functions = {}, custom_function = {}, arguments = {}, *argument, arguments_joined = {};

	dao_fetch_params(0, 1, 1, &expression, &escape_char);

	if (!escape_char) {
		escape_char = &DAO_GLOBAL(z_null);
	}

	dao_array_fetch_str(&name, expression, SL("name"), PH_NOISY|PH_READONLY);

	DAO_CALL_METHOD(&custom_functions, getThis(), "getcustomfunctions");

	if (dao_array_isset_fetch(&custom_function, &custom_functions, &name, 0)) {
		DAO_CALL_ZVAL_FUNCTION(return_value, &custom_function, getThis(), expression, escape_char);
		zval_ptr_dtor(&custom_functions);
		return;
	}
	zval_ptr_dtor(&custom_functions);

	if (dao_array_isset_fetch_str(&arguments, expression, SL("arguments"), PH_READONLY)) {
		zval sql_arguments = {};
		array_init(&sql_arguments);
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL(arguments), argument) {
			zval argument_expression = {};
			DAO_CALL_METHOD(&argument_expression, getThis(), "getsqlexpression", argument, escape_char);
			dao_array_append(&sql_arguments, &argument_expression, 0);
		} ZEND_HASH_FOREACH_END();

		dao_fast_join_str(&arguments_joined, SL(", "), &sql_arguments);
		zval_ptr_dtor(&sql_arguments);
		if (dao_array_isset_str(expression, SL("distinct"))) {
			DAO_CONCAT_VSVS(return_value, &name, "(DISTINCT ", &arguments_joined, ")");
		} else {
			DAO_CONCAT_VSVS(return_value, &name, "(", &arguments_joined, ")");
		}
		zval_ptr_dtor(&arguments_joined);
	} else {
		DAO_CONCAT_VS(return_value, &name, "()");
	}
}

/**
 * Transform an intermediate representation for a schema/table into a database system valid expression
 *
 * @param array $table
 * @param string $escapeChar
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect, getSqlTable){

	zval *table, *escape = NULL, escape_char = {};

	dao_fetch_params(0, 1, 1, &table, &escape);

	if (!escape || Z_TYPE_P(escape) == IS_NULL) {
		dao_read_property(&escape_char, getThis(), SL("_escapeChar"), PH_NOISY|PH_READONLY);
	} else {
		ZVAL_COPY_VALUE(&escape_char, escape);
	}

	if (Z_TYPE_P(table) == IS_ARRAY) {
		zval table_name = {}, sql_table = {}, schema_name = {}, sql_schema = {}, alias_name = {}, sql_table_alias = {}, indexs = {};
		/**
		 * The index '0' is the table name
		 */
		dao_array_fetch_long(&table_name, table, 0, PH_NOISY|PH_READONLY);
		if (DAO_GLOBAL(db).escape_identifiers) {
			DAO_CONCAT_VVV(&sql_table, &escape_char, &table_name, &escape_char);
		} else {
			ZVAL_COPY(&sql_table, &table_name);
		}

		/**
		 * The index '1' is the schema name
		 */
		dao_array_fetch_long(&schema_name, table, 1, PH_NOISY|PH_READONLY);
		if (DAO_IS_NOT_EMPTY(&schema_name)) {
			if (DAO_GLOBAL(db).escape_identifiers) {
				DAO_CONCAT_VVVSV(&sql_schema, &escape_char, &schema_name, &escape_char, ".", &sql_table);
			} else {
				DAO_CONCAT_VSV(&sql_schema, &schema_name, ".", &sql_table);
			}
		} else {
			ZVAL_COPY(&sql_schema, &sql_table);
		}
		zval_ptr_dtor(&sql_table);

		/**
		 * The index '2' is the table alias
		 */
		if (dao_array_isset_fetch_long(&alias_name, table, 2, PH_READONLY)) {
			if (DAO_GLOBAL(db).escape_identifiers) {
				DAO_CONCAT_VSVVV(&sql_table_alias, &sql_schema, " AS ", &escape_char, &alias_name, &escape_char);
			} else {
				DAO_CONCAT_VSV(&sql_table_alias, &sql_schema, " AS ", &alias_name);
			}
		} else {
			ZVAL_COPY(&sql_table_alias, &sql_schema);
		}
		zval_ptr_dtor(&sql_schema);

		/**
		 * The index '3' is the table index
		 */
		if (dao_array_isset_fetch_long(&indexs, table, 3, PH_READONLY)) {

		}

		RETVAL_ZVAL(&sql_table_alias, 0, 0);
		return;
	}

	if (DAO_GLOBAL(db).escape_identifiers) {
		DAO_CONCAT_VVV(return_value, &escape_char, table, &escape_char);
		return;
	}

	RETURN_CTOR(table);
}

/**
 * Builds a SELECT statement
 *
 * @param array $definition
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect, select){

	zval *definition, *count = NULL, escape_char = {}, columns = {}, distinct = {}, index = {}, *column, columns_sql = {}, tables = {}, *table;
	zval tables_sql = {}, sql = {}, joins = {}, *join = NULL, where_conditions = {}, where_expression = {}, group_fields = {}, order_fields = {};
	zval tmp1 = {}, tmp2 = {}, limit_value = {}, number = {}, offset = {};

	dao_fetch_params(1, 1, 1, &definition, &count);

	if (!count) {
		count = &DAO_GLOBAL(z_false);
	}

	if (Z_TYPE_P(definition) != IS_ARRAY) {
		DAO_MM_THROW_EXCEPTION_STR(dao_db_exception_ce, "Invalid SELECT definition");
		return;
	}
	if (!dao_array_isset_fetch_str(&tables, definition, SL("tables"), PH_READONLY)) {
		DAO_MM_THROW_EXCEPTION_STR(dao_db_exception_ce, "The index 'tables' is required in the definition array");
		return;
	}

	if (!dao_array_isset_fetch_str(&columns, definition, SL("columns"), PH_READONLY)) {
		DAO_MM_THROW_EXCEPTION_STR(dao_db_exception_ce, "The index 'columns' is required in the definition array");
		return;
	}

	DAO_MM_CALL_METHOD(&escape_char, getThis(), "getescapechar");
	DAO_MM_ADD_ENTRY(&escape_char);
	if (!dao_array_isset_fetch_str(&distinct, definition, SL("distinct"), PH_READONLY)) {
		ZVAL_NULL(&distinct);
	}

	if (!zend_is_true(count) || dao_array_isset_str(definition, SL("group")) 
		|| (Z_TYPE(distinct) == IS_LONG && (Z_LVAL(distinct) == 1 || Z_LVAL(distinct) == 0))) {
		if (Z_TYPE(columns) == IS_ARRAY) {
			zval selected_columns = {};
			array_init(&selected_columns);
			DAO_MM_ADD_ENTRY(&selected_columns);
			ZEND_HASH_FOREACH_VAL(Z_ARRVAL(columns), column) {
				zval column_item = {}, column_sql = {}, column_domain = {}, column_domain_sql = {}, column_alias = {}, column_alias_sql = {};
				if (Z_TYPE_P(column) == IS_STRING) {
					dao_array_append(&selected_columns, column, PH_COPY);
					continue;
				}

				/**
				 * Escape column name
				 */
				if (
						dao_array_isset_fetch_long(&column_item, column, 0, PH_READONLY)
					 || dao_array_isset_fetch_str(&column_item, column, SL("column"), PH_READONLY)
				) {
					if (Z_TYPE(column_item) == IS_ARRAY) {
						DAO_MM_CALL_METHOD(&column_sql, getThis(), "getsqlexpression", &column_item, &escape_char);
						DAO_MM_ADD_ENTRY(&column_sql);
					} else if (DAO_IS_STRING(&column_item, "*")) {
						ZVAL_COPY_VALUE(&column_sql, &column_item);
					} else if (DAO_GLOBAL(db).escape_identifiers) {
						DAO_CONCAT_VVV(&column_sql, &escape_char, &column_item, &escape_char);
						DAO_MM_ADD_ENTRY(&column_sql);
					} else {
						ZVAL_COPY_VALUE(&column_sql, &column_item);
					}
				} else {
					DAO_MM_THROW_EXCEPTION_STR(dao_db_exception_ce, "Invalid columns");
					return;
				}

				/**
				 * Escape column domain
				 */
				if (dao_array_isset_fetch_long(&column_domain, column, 1, PH_READONLY)) {
					if (zend_is_true(&column_domain)) {
						if (DAO_GLOBAL(db).escape_identifiers) {
							DAO_CONCAT_VVVSV(&column_domain_sql, &escape_char, &column_domain, &escape_char, ".", &column_sql);
						} else {
							DAO_CONCAT_VSV(&column_domain_sql, &column_domain, ".", &column_sql);
						}
						DAO_MM_ADD_ENTRY(&column_domain_sql);
					} else {
						ZVAL_COPY_VALUE(&column_domain_sql, &column_sql);
					}
				} else {
					ZVAL_COPY_VALUE(&column_domain_sql, &column_sql);
				}

				/**
				 * Escape column alias
				 */
				if (dao_array_isset_fetch_long(&column_alias, column, 2, PH_READONLY)) {
					if (zend_is_true(&column_alias)) {
						if (DAO_GLOBAL(db).escape_identifiers) {
							DAO_CONCAT_VSVVV(&column_alias_sql, &column_domain_sql, " AS ", &escape_char, &column_alias, &escape_char);
						} else {
							DAO_CONCAT_VSV(&column_alias_sql, &column_domain_sql, " AS ", &column_alias);
						}
						DAO_MM_ADD_ENTRY(&column_alias_sql);
					} else {
						ZVAL_COPY_VALUE(&column_alias_sql, &column_domain_sql);
					}
				} else {
					ZVAL_COPY_VALUE(&column_alias_sql, &column_domain_sql);
				}

				dao_array_append(&selected_columns, &column_alias_sql, PH_COPY);
			} ZEND_HASH_FOREACH_END();

			dao_fast_join_str(&columns_sql, SL(", "), &selected_columns);
			DAO_MM_ADD_ENTRY(&columns_sql);
		} else {
			ZVAL_COPY_VALUE(&columns_sql, &columns);
		}
	} else {
		DAO_MM_ZVAL_STRING(&columns_sql, "COUNT(*) AS rowcount");
	}

	/**
	 * Check and escape tables
	 */
	if (Z_TYPE(tables) == IS_ARRAY) {
		zval selected_tables = {};
		array_init(&selected_tables);
		DAO_MM_ADD_ENTRY(&selected_tables);
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(&tables), table) {
			zval sql_table = {};
			DAO_MM_CALL_METHOD(&sql_table, getThis(), "getsqltable", table, &escape_char);
			dao_array_append(&selected_tables, &sql_table, 0);
		} ZEND_HASH_FOREACH_END();

		dao_fast_join_str(&tables_sql, SL(", "), &selected_tables);
		DAO_MM_ADD_ENTRY(&tables_sql);
	} else {
		DAO_MM_CALL_METHOD(&tables_sql, getThis(), "getsqltable", &tables, &escape_char);
		DAO_MM_ADD_ENTRY(&tables_sql);
	}

	if (Z_TYPE(distinct) == IS_LONG) {
		if (Z_LVAL(distinct) == 0) {
			DAO_MM_ZVAL_STRING(&sql, "SELECT ALL ");
		} else if (Z_LVAL(distinct) == 1) {
			DAO_MM_ZVAL_STRING(&sql, "SELECT DISTINCT ");
		} else {
			DAO_MM_ZVAL_STRING(&sql, "SELECT ");
		}
	} else {
		DAO_MM_ZVAL_STRING(&sql, "SELECT ");
	}

	DAO_SCONCAT_VSV(&sql, &columns_sql, " FROM ", &tables_sql);
	DAO_MM_ADD_ENTRY(&sql);

	if (dao_array_isset_fetch_str(&index, definition, SL("index"), PH_READONLY)) {
		if (Z_TYPE(index) == IS_STRING) {
			DAO_SCONCAT_SV(&sql, " ", &index);
			DAO_MM_ADD_ENTRY(&sql);
		}
	}

	/**
	 * Check for joins
	 */
	if (dao_array_isset_fetch_str(&joins, definition, SL("joins"), PH_READONLY) && Z_TYPE(joins) == IS_ARRAY) {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL(joins), join) {
			zval type = {}, source = {}, sql_table = {}, sql_join = {}, join_conditions_array = {};
			zval *join_condition, join_expressions = {}, join_conditions = {};

			dao_array_fetch_str(&type, join, SL("type"), PH_NOISY|PH_READONLY);
			dao_array_fetch_str(&source, join, SL("source"), PH_NOISY|PH_READONLY);

			DAO_MM_CALL_METHOD(&sql_table, getThis(), "getsqltable", &source, &escape_char);
			DAO_CONCAT_SVSV(&sql_join, " ", &type, " JOIN ", &sql_table);
			zval_ptr_dtor(&sql_table);
			DAO_MM_ADD_ENTRY(&sql_join);

			/**
			 * Check if the join has conditions
			 */
			if (dao_array_isset_fetch_str(&join_conditions_array, join, SL("conditions"), PH_READONLY)) {
				if (Z_TYPE(join_conditions_array) == IS_ARRAY && dao_fast_count_ev(&join_conditions_array)) {
					array_init(&join_expressions);
					DAO_MM_ADD_ENTRY(&join_expressions);
					ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(&join_conditions_array), join_condition) {
						zval join_expression = {};
						DAO_MM_CALL_METHOD(&join_expression, getThis(), "getsqlexpression", join_condition, &escape_char);
						dao_array_append(&join_expressions, &join_expression, 0);
					} ZEND_HASH_FOREACH_END();

					dao_fast_join_str(&join_conditions, SL(" AND "), &join_expressions);
					DAO_SCONCAT_SV(&sql_join, " ON ", &join_conditions);
					zval_ptr_dtor(&join_conditions);
					DAO_MM_ADD_ENTRY(&sql_join);
				} else if (Z_TYPE(join_conditions_array) == IS_STRING) {
					DAO_SCONCAT_SV(&sql_join, " ON ", &join_conditions_array);
					DAO_MM_ADD_ENTRY(&sql_join);
				}
			}
			dao_concat_self(&sql, &sql_join);
			DAO_MM_ADD_ENTRY(&sql);
		} ZEND_HASH_FOREACH_END();
	}

	/* Check for a WHERE clause */
	if (dao_array_isset_fetch_str(&where_conditions, definition, SL("where"), PH_READONLY)) {
		if (Z_TYPE(where_conditions) == IS_ARRAY) {
			DAO_CALL_METHOD(&where_expression, getThis(), "getsqlexpression", &where_conditions, &escape_char);
			DAO_SCONCAT_SV(&sql, " WHERE ", &where_expression);
			zval_ptr_dtor(&where_expression);
			DAO_MM_ADD_ENTRY(&sql);
		} else if (DAO_IS_NOT_EMPTY(&where_conditions)) {
			DAO_SCONCAT_SV(&sql, " WHERE ", &where_conditions);
			DAO_MM_ADD_ENTRY(&sql);
		}
	}

	/* Check for a GROUP clause */
	if (dao_array_isset_fetch_str(&group_fields, definition, SL("group"), PH_READONLY)) {
		zval having_conditions = {}, having_expression = {};
		if (Z_TYPE(group_fields) == IS_ARRAY) {
			zval group_items = {}, *group_field, group_clause = {}, group_sql = {};
			array_init(&group_items);
			DAO_MM_ADD_ENTRY(&group_items);
			ZEND_HASH_FOREACH_VAL(Z_ARRVAL(group_fields), group_field) {
				zval group_expression = {};
				DAO_MM_CALL_METHOD(&group_expression, getThis(), "getsqlexpression", group_field, &escape_char);
				dao_array_append(&group_items, &group_expression, 0);
			} ZEND_HASH_FOREACH_END();

			dao_fast_join_str(&group_sql, SL(", "), &group_items);
			DAO_CONCAT_SV(&group_clause, " GROUP BY ", &group_sql);
			zval_ptr_dtor(&group_sql);
			dao_concat_self(&sql, &group_clause);
			zval_ptr_dtor(&group_clause);
			DAO_MM_ADD_ENTRY(&sql);

			/* Check for a HAVING clause */
			if (dao_array_isset_fetch_str(&having_conditions, definition, SL("having"), PH_READONLY)) {
				DAO_MM_CALL_METHOD(&having_expression, getThis(), "getsqlexpression", &having_conditions, &escape_char);
				DAO_SCONCAT_SV(&sql, " HAVING ", &having_expression);
				zval_ptr_dtor(&having_expression);
				DAO_MM_ADD_ENTRY(&sql);
			}
		} else if (DAO_IS_NOT_EMPTY(&group_fields)) {
			DAO_SCONCAT_SV(&sql, " GROUP BY ", &group_fields);
			DAO_MM_ADD_ENTRY(&sql);

			/* Check for a HAVING clause */
			if (dao_array_isset_fetch_str(&having_conditions, definition, SL("having"), PH_READONLY)) {
				DAO_MM_CALL_METHOD(&having_expression, getThis(), "getsqlexpression", &having_conditions, &escape_char);
				DAO_SCONCAT_SV(&sql, " HAVING ", &having_expression);
				zval_ptr_dtor(&having_expression);
				DAO_MM_ADD_ENTRY(&sql);
			}
		}
	}

	if (!zend_is_true(count)) {
		/* Check for a ORDER clause */
		if (dao_array_isset_fetch_str(&order_fields, definition, SL("order"), PH_READONLY)) {
			if (Z_TYPE(order_fields) == IS_ARRAY) {
				zval order_items = {}, *order_item, order_sql = {};
				array_init(&order_items);
				DAO_MM_ADD_ENTRY(&order_items);
				ZEND_HASH_FOREACH_VAL(Z_ARRVAL(order_fields), order_item) {
					zval order_expression = {}, order_sql_item = {}, sql_order_type = {}, order_sql_item_type = {};

					dao_array_fetch_long(&order_expression, order_item, 0, PH_NOISY|PH_READONLY);
					DAO_MM_CALL_METHOD(&order_sql_item, getThis(), "getsqlexpression", &order_expression, &escape_char);

					/**
					 * In the numeric 1 position could be a ASC/DESC clause
					 */
					if (dao_array_isset_fetch_long(&sql_order_type, order_item, 1, PH_READONLY)) {
						DAO_CONCAT_VSV(&order_sql_item_type, &order_sql_item, " ", &sql_order_type);
						zval_ptr_dtor(&order_sql_item);
						dao_array_append(&order_items, &order_sql_item_type, 0);
					} else {
						dao_array_append(&order_items, &order_sql_item, 0);
					}

				} ZEND_HASH_FOREACH_END();

				dao_fast_join_str(&order_sql, SL(", "), &order_items);
				DAO_SCONCAT_SV(&sql, " ORDER BY ", &order_sql);
				zval_ptr_dtor(&order_sql);
				DAO_MM_ADD_ENTRY(&sql);
			} else {
				DAO_SCONCAT_SV(&sql, " ORDER BY ", &order_fields);
				DAO_MM_ADD_ENTRY(&sql);
			}
		}

		/**
		 * Check for a LIMIT condition
		 */
		if (dao_array_isset_fetch_str(&limit_value, definition, SL("limit"), PH_READONLY)) {
			if (likely(Z_TYPE(limit_value) == IS_ARRAY)) {
				if (likely(dao_array_isset_fetch_str(&number, &limit_value, SL("number"), PH_READONLY))) {
					dao_array_fetch_str(&tmp1, &number, SL("value"), PH_NOISY|PH_READONLY);

					/**
					 * Check for a OFFSET condition
					 */
					if (dao_array_isset_fetch_str(&offset, &limit_value, SL("offset"), PH_READONLY)) {
						dao_array_fetch_str(&tmp2, &offset, SL("value"), PH_NOISY|PH_READONLY);
						DAO_SCONCAT_SVSV(&sql, " LIMIT ", &tmp1, " OFFSET ", &tmp2);
					} else {
						DAO_SCONCAT_SV(&sql, " LIMIT ", &tmp1);
					}
					DAO_MM_ADD_ENTRY(&sql);
				}
			} else {
				if (dao_array_isset_fetch_str(&offset, definition, SL("offset"), PH_READONLY)) {
					DAO_SCONCAT_SVSV(&sql, " LIMIT ", &limit_value, " OFFSET ", &offset);
				} else {
					DAO_SCONCAT_SV(&sql, " LIMIT ", &limit_value);
				}
				DAO_MM_ADD_ENTRY(&sql);
			}
		}
	}

	if (!zend_is_true(count)) {
		/**
		 * Check for a FOR UPDATE clause
		 */
		if (dao_array_isset_str(definition, SL("forupdate"))) {
			DAO_MM_CALL_METHOD(return_value, getThis(), "forupdate", &sql);
			RETURN_MM();
		}
	}

	if (zend_is_true(count) && (dao_array_isset_str(definition, SL("group")) || (Z_TYPE(distinct) == IS_LONG && (Z_LVAL(distinct) == 1 || Z_LVAL(distinct) == 0)))) {
		DAO_CONCAT_SVS(return_value, "SELECT COUNT(*) \"rowcount\" FROM (", &sql, ") AS T");
		RETURN_MM();
	}
	RETURN_MM_CTOR(&sql);
}

/**
 * Builds a INSERT statement
 *
 * @param array $definition
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect, insert)
{
	zval *definition, table = {}, fields = {}, number_fields = {}, values = {}, exception_message = {}, escaped_table = {}, escape_char = {};
	zval *row_values = NULL, joined_rows = {}, joined_values = {}, escaped_fields = {}, *field;

	dao_fetch_params(1, 1, 0, &definition);

	if (Z_TYPE_P(definition) != IS_ARRAY) {
		DAO_MM_THROW_EXCEPTION_STR(dao_db_exception_ce, "Invalid INSERT definition");
		return;
	}

	if (!dao_array_isset_fetch_str(&table, definition, SL("table"), PH_READONLY)) {
		DAO_MM_THROW_EXCEPTION_STR(dao_db_exception_ce, "The index 'table' is required in the definition array");
		return;
	}

	if (!dao_array_isset_fetch_str(&values, definition, SL("values"), PH_READONLY)) {
		DAO_THROW_EXCEPTION_STR(dao_db_exception_ce, "The index 'values' is required in the definition array");
		return;
	}

	if (unlikely(Z_TYPE(values) != IS_ARRAY)) {
		DAO_MM_THROW_EXCEPTION_STR(dao_db_exception_ce, "The second parameter for insert isn't an Array");
		return;
	}


	if (DAO_GLOBAL(db).escape_identifiers) {
		dao_read_property(&escape_char, getThis(), SL("_escapeChar"), PH_READONLY);
	}

	/**
	 * A valid array with more than one element is required
	 */
	if (!dao_fast_count_ev(&values)) {
		DAO_CONCAT_SVS(&exception_message, "Unable to insert into ", &table, " without data");
		DAO_MM_ADD_ENTRY(&exception_message);
		DAO_MM_THROW_EXCEPTION_ZVAL(dao_db_exception_ce, &exception_message);
		return;
	}

	DAO_MM_CALL_METHOD(&escaped_table, getThis(), "getsqltable", &table, &escape_char);
	DAO_MM_ADD_ENTRY(&escaped_table);

	if (!dao_array_isset_fetch_str(&fields, definition, SL("fields"), PH_READONLY)) {
		zval joined_fields = {}, escaped_fields ={}, field_values = {}, *value;
		zend_string *str_key;
		array_init(&escaped_fields);
		DAO_MM_ADD_ENTRY(&escaped_fields);
		array_init(&field_values);
		DAO_MM_ADD_ENTRY(&field_values);

		ZEND_HASH_FOREACH_STR_KEY_VAL(Z_ARRVAL(values), str_key, value) {
			zval key = {}, escaped_field = {};

			if (!str_key) {
				continue;
			}
			ZVAL_STR(&key, str_key);
			if (DAO_GLOBAL(db).escape_identifiers) {
				DAO_CONCAT_VVV(&escaped_field, &escape_char, &key, &escape_char);
			} else {
				ZVAL_COPY(&escaped_field, &key);
			}
			dao_array_append(&escaped_fields, &escaped_field, 0);
			dao_array_append(&field_values, value, PH_COPY);
		} ZEND_HASH_FOREACH_END();

		dao_fast_join_str(&joined_fields, SL(", "), &escaped_fields);
		dao_fast_join_str(&joined_values, SL(", "), &field_values);

		DAO_CONCAT_SVSVSVS(return_value, "INSERT INTO ", &escaped_table, " (", &joined_fields, ") VALUES (", &joined_values, ")");
		zval_ptr_dtor(&joined_fields);
		zval_ptr_dtor(&joined_values);
		RETURN_MM();
	}

	dao_fast_count(&number_fields, &fields);

	/**
	 * Build the final SQL INSERT statement
	 */
	array_init(&joined_rows);
	DAO_MM_ADD_ENTRY(&joined_rows);
	ZEND_HASH_FOREACH_VAL(Z_ARRVAL(values), row_values) {
		zval number_values = {}, insert_row_values = {}, *value, joined_row = {};

		dao_fast_count(&number_values, row_values);

		/**
		 * The number of calculated values must be equal to the number of fields in the
		 * model
		 */
		if (!DAO_IS_EQUAL(&number_fields, &number_values)) {
			DAO_CONCAT_SVSVS(&exception_message, "The fields count(", &number_fields, ") does not match the values count(", &number_values, ")");
			DAO_MM_ADD_ENTRY(&exception_message);
			DAO_MM_THROW_EXCEPTION_ZVAL(dao_db_exception_ce, &exception_message);
			return;
		}

		array_init(&insert_row_values);
		DAO_MM_ADD_ENTRY(&insert_row_values);

		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(row_values), value) {
			zval insert_value = {};

			DAO_MM_CALL_METHOD(&insert_value, getThis(), "getsqlexpression", value, &escape_char);

			dao_array_append(&insert_row_values, &insert_value, 0);
		} ZEND_HASH_FOREACH_END();

		dao_fast_join_str(&joined_row, SL(", "), &insert_row_values);
		dao_array_append(&joined_rows, &joined_row, 0);
	} ZEND_HASH_FOREACH_END();

	dao_fast_join_str(&joined_values, SL("), ("), &joined_rows);
	DAO_MM_ADD_ENTRY(&joined_values);

	if (Z_TYPE(fields) == IS_ARRAY) {
		zval joined_fields = {};
		if (DAO_GLOBAL(db).escape_identifiers) {
			array_init(&escaped_fields);

			ZEND_HASH_FOREACH_VAL(Z_ARRVAL(fields), field) {
				zval escaped_field = {};
				DAO_CONCAT_VVV(&escaped_field, &escape_char, field, &escape_char);
				dao_array_append(&escaped_fields, &escaped_field, 0);
			} ZEND_HASH_FOREACH_END();

		} else {
			ZVAL_COPY(&escaped_fields, &fields);
		}

		dao_fast_join_str(&joined_fields, SL(", "), &escaped_fields);
		zval_ptr_dtor(&escaped_fields);

		DAO_CONCAT_SVSVSVS(return_value, "INSERT INTO ", &escaped_table, " (", &joined_fields, ") VALUES (", &joined_values, ")");
		zval_ptr_dtor(&joined_fields);
	} else {
		DAO_CONCAT_SVSVS(return_value, "INSERT INTO ", &escaped_table, " VALUES (", &joined_values, ")");
	}
	RETURN_MM();
}

/**
 * Builds a UPDATE statement
 *
 * @param array $definition
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect, update){

	zval *definition, tables = {}, sets = {}, fields = {}, values = {}, escape_char = {}, updated_tables = {}, *table, sql = {};
	zval updated_fields = {}, *column, columns_sql = {}, where_conditions = {}, where_expression = {};
	zval order_fields = {}, limit_value = {}, number = {}, offset = {}, tmp1 = {}, tmp2 = {};
	zend_string *str_key;
	ulong idx;

	dao_fetch_params(1, 1, 0, &definition);

	if (Z_TYPE_P(definition) != IS_ARRAY) {
		DAO_MM_THROW_EXCEPTION_STR(dao_db_exception_ce, "Invalid Update definition");
		return;
	}

	if (!dao_array_isset_fetch_str(&tables, definition, SL("tables"), PH_READONLY)) {
		DAO_MM_THROW_EXCEPTION_STR(dao_db_exception_ce, "The index 'tables' is required in the definition array");
		return;
	}

	if (!dao_array_isset_fetch_str(&sets, definition, SL("sets"), PH_READONLY)) {
		if (!dao_array_isset_fetch_str(&fields, definition, SL("fields"), PH_READONLY) || Z_TYPE(fields) != IS_ARRAY) {
			DAO_MM_THROW_EXCEPTION_STR(dao_db_exception_ce, "The index 'fields' must be array in the definition array");
			return;
		}

		if (!dao_array_isset_fetch_str(&values, definition, SL("values"), PH_READONLY) || Z_TYPE(values) != IS_ARRAY) {
			DAO_MM_THROW_EXCEPTION_STR(dao_db_exception_ce, "The index 'values' must be array in the definition array");
			return;
		}
	} else if (Z_TYPE(sets) != IS_ARRAY) {
		DAO_MM_THROW_EXCEPTION_STR(dao_db_exception_ce, "The index 'sets' must be array in the definition array");
		return;
	}

	if (DAO_GLOBAL(db).escape_identifiers) {
		dao_read_property(&escape_char, getThis(), SL("_escapeChar"), PH_READONLY);
	}

	/**
	 * Check and escape tables
	 */
	array_init(&updated_tables);
	DAO_MM_ADD_ENTRY(&updated_tables);

	if (Z_TYPE(tables) == IS_ARRAY) {
		zval tables_sql = {};
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL(tables), table) {
			zval table_expression = {};
			DAO_MM_CALL_METHOD(&table_expression, getThis(), "getsqltable", table, &escape_char);
			dao_array_append(&updated_tables, &table_expression, 0);
		} ZEND_HASH_FOREACH_END();
		dao_fast_join_str(&tables_sql, SL(", "), &updated_tables);
		DAO_CONCAT_SV(&sql, "UPDATE ", &tables_sql);
		zval_ptr_dtor(&tables_sql);
	} else {
		zval tables_sql = {};
		DAO_MM_CALL_METHOD(&tables_sql, getThis(), "getsqltable", &tables, &escape_char);
		DAO_CONCAT_SV(&sql, "UPDATE ", &tables_sql);
		zval_ptr_dtor(&tables_sql);
	}

	DAO_MM_ADD_ENTRY(&sql);

	if (Z_TYPE(fields) == IS_ARRAY) {
		array_init(&updated_fields);
		DAO_MM_ADD_ENTRY(&updated_fields);
		ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL(fields), idx, str_key, column) {
			zval position = {}, column_name = {}, column_quoted = {}, value_expr = {}, value = {}, value_expression = {}, column_expression = {};

			if (str_key) {
				ZVAL_STR(&position, str_key);
			} else {
				ZVAL_LONG(&position, idx);
			}
			dao_array_fetch_str(&column_name, column, SL("name"), PH_NOISY|PH_READONLY);
			dao_array_fetch(&value_expr, &values, &position, PH_NOISY|PH_READONLY);
			dao_array_fetch_str(&value, &value_expr, SL("value"), PH_NOISY|PH_READONLY);

			DAO_MM_CALL_METHOD(&value_expression, getThis(), "getsqlexpression", &value, &escape_char);

			if (DAO_GLOBAL(db).escape_identifiers) {
				DAO_CONCAT_VVV(&column_quoted, &escape_char, &column_name, &escape_char);
			} else {
				ZVAL_COPY(&column_quoted, &column_name);
			}

			DAO_CONCAT_VSV(&column_expression, &column_quoted, " = ", &value_expression);

			zval_ptr_dtor(&value_expression);
			zval_ptr_dtor(&column_quoted);

			dao_array_append(&updated_fields, &column_expression, 0);
		} ZEND_HASH_FOREACH_END();

		dao_fast_join_str(&columns_sql, SL(", "), &updated_fields);
	} else {
		array_init(&updated_fields);
		DAO_MM_ADD_ENTRY(&updated_fields);
		ZEND_HASH_FOREACH_STR_KEY_VAL(Z_ARRVAL(sets), str_key, column) {
			zval column_name = {}, column_quoted = {}, column_expression = {};

			if (!str_key) {
				continue;
			}
			ZVAL_STR(&column_name, str_key);
			if (DAO_GLOBAL(db).escape_identifiers) {
				DAO_CONCAT_VVV(&column_quoted, &escape_char, &column_name, &escape_char);
			} else {
				ZVAL_COPY(&column_quoted, &column_name);
			}
			DAO_CONCAT_VSV(&column_expression, &column_quoted, " = ", column);
			zval_ptr_dtor(&column_quoted);

			dao_array_append(&updated_fields, &column_expression, 0);
		} ZEND_HASH_FOREACH_END();

		dao_fast_join_str(&columns_sql, SL(", "), &updated_fields);
	}

	DAO_SCONCAT_SV(&sql, " SET ", &columns_sql);
	zval_ptr_dtor(&columns_sql);
	DAO_MM_ADD_ENTRY(&sql);

	/* Check for a WHERE clause */
	if (dao_array_isset_fetch_str(&where_conditions, definition, SL("where"), PH_READONLY)) {
		if (Z_TYPE(where_conditions) == IS_ARRAY) {
			DAO_MM_CALL_METHOD(&where_expression, getThis(), "getsqlexpression", &where_conditions, &escape_char);
			DAO_SCONCAT_SV(&sql, " WHERE ", &where_expression);
			zval_ptr_dtor(&where_expression);
		} else {
			DAO_SCONCAT_SV(&sql, " WHERE ", &where_conditions);
		}
		DAO_MM_ADD_ENTRY(&sql);
	}

	/* Check for a ORDER clause */
	if (dao_array_isset_fetch_str(&order_fields, definition, SL("order"), PH_READONLY)) {
		zval order_sql = {}, order_items = {}, *order_item;
		array_init(&order_items);
		DAO_MM_ADD_ENTRY(&order_items);
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL(order_fields), order_item) {
			zval order_expression = {}, order_sql_item = {}, sql_order_type = {}, order_sql_item_type = {};

			dao_array_fetch_long(&order_expression, order_item, 0, PH_NOISY|PH_READONLY);

			DAO_CALL_METHOD(&order_sql_item, getThis(), "getsqlexpression", &order_expression, &escape_char);

			/**
			 * In the numeric 1 position could be a ASC/DESC clause
			 */
			if (dao_array_isset_fetch_long(&sql_order_type, order_item, 1, PH_READONLY)) {
				DAO_CONCAT_VSV(&order_sql_item_type, &order_sql_item, " ", &sql_order_type);
			} else {
				ZVAL_COPY(&order_sql_item_type, &order_sql_item);
			}
			zval_ptr_dtor(&order_sql_item);
			dao_array_append(&order_items, &order_sql_item_type, 0);
		} ZEND_HASH_FOREACH_END();

		dao_fast_join_str(&order_sql, SL(", "), &order_items);
		DAO_SCONCAT_SV(&sql, " ORDER BY ", &order_sql);
		zval_ptr_dtor(&order_sql);
		DAO_MM_ADD_ENTRY(&sql);
	}

	/**
	 * Check for a LIMIT condition
	 */
	if (dao_array_isset_fetch_str(&limit_value, definition, SL("limit"), PH_READONLY)) {
		if (likely(Z_TYPE(limit_value) == IS_ARRAY)) {
			if (likely(dao_array_isset_fetch_str(&number, &limit_value, SL("number"), PH_READONLY))) {
				dao_array_fetch_str(&tmp1, &number, SL("value"), PH_NOISY|PH_READONLY);

				/**
				 * Check for a OFFSET condition
				 */
				if (dao_array_isset_fetch_str(&offset, &limit_value, SL("offset"), PH_READONLY)) {
					dao_array_fetch_str(&tmp2, &offset, SL("value"), PH_NOISY|PH_READONLY);
					DAO_SCONCAT_SVSV(&sql, " LIMIT ", &tmp1, " OFFSET ", &tmp2);
				} else {
					DAO_SCONCAT_SV(&sql, " LIMIT ", &tmp1);
				}
			}
		} else {
			DAO_SCONCAT_SV(&sql, " LIMIT ", &limit_value);
		}
		DAO_MM_ADD_ENTRY(&sql);
	}

	RETURN_MM_CTOR(&sql);
}

/**
 * Builds a DELETE statement
 *
 * @param array $definition
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect, delete){

	zval *definition, tables = {}, escape_char = {}, updated_tables = {}, *table, tables_sql = {}, sql = {}, where_conditions = {};
	zval order_fields = {}, order_items = {}, *order_item, limit_value = {}, offset = {};

	dao_fetch_params(1, 1, 0, &definition);

	if (Z_TYPE_P(definition) != IS_ARRAY) {
		DAO_MM_THROW_EXCEPTION_STR(dao_db_exception_ce, "Invalid Update definition");
		return;
	}
	if (!dao_array_isset_fetch_str(&tables, definition, SL("tables"), PH_READONLY)) {
		DAO_MM_THROW_EXCEPTION_STR(dao_db_exception_ce, "The index 'tables' is required in the definition array");
		return;
	}

	if (DAO_GLOBAL(db).escape_identifiers) {
		dao_read_property(&escape_char, getThis(), SL("_escapeChar"), PH_READONLY);
	}

	/**
	 * Check and escape tables
	 */
	if (Z_TYPE(tables) == IS_ARRAY) {
		array_init(&updated_tables);
		DAO_MM_ADD_ENTRY(&updated_tables);

		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(&tables), table) {
			zval sql_table = {};
			DAO_MM_CALL_METHOD(&sql_table, getThis(), "getsqltable", table, &escape_char);
			dao_array_append(&updated_tables, &sql_table, 0);
		} ZEND_HASH_FOREACH_END();

		dao_fast_join_str(&tables_sql, SL(", "), &updated_tables);
		DAO_CONCAT_SV(&sql, "DELETE FROM ", &tables_sql);
		zval_ptr_dtor(&tables_sql);
	} else {
		zval tables_sql = {};
		DAO_MM_CALL_METHOD(&tables_sql, getThis(), "getsqltable", &tables, &escape_char);
		DAO_CONCAT_SV(&sql, "DELETE FROM ", &tables_sql);
		zval_ptr_dtor(&tables_sql);
		DAO_MM_ADD_ENTRY(&sql);
	}

	/* Check for a WHERE clause */
	if (dao_array_isset_fetch_str(&where_conditions, definition, SL("where"), PH_READONLY)) {
		if (Z_TYPE(where_conditions) == IS_ARRAY) {
			zval where_expression = {};
			DAO_MM_CALL_METHOD(&where_expression, getThis(), "getsqlexpression", &where_conditions, &escape_char);
			DAO_SCONCAT_SV(&sql, " WHERE ", &where_expression);
			zval_ptr_dtor(&where_expression);
		} else {
			DAO_SCONCAT_SV(&sql, " WHERE ", &where_conditions);
		}
		DAO_MM_ADD_ENTRY(&sql);
	}

	/* Check for a ORDER clause */
	if (dao_array_isset_fetch_str(&order_fields, definition, SL("order"), PH_READONLY)) {
		zval order_sql = {};
		array_init(&order_items);

		ZEND_HASH_FOREACH_VAL(Z_ARRVAL(order_fields), order_item) {
			zval order_expression = {}, order_sql_item = {}, sql_order_type = {}, order_sql_item_type = {};

			dao_array_fetch_long(&order_expression, order_item, 0, PH_NOISY|PH_READONLY);

			DAO_MM_CALL_METHOD(&order_sql_item, getThis(), "getsqlexpression", &order_expression, &escape_char);

			/**
			 * In the numeric 1 position could be a ASC/DESC clause
			 */
			if (dao_array_isset_fetch_long(&sql_order_type, order_item, 1, PH_READONLY)) {
				DAO_CONCAT_VSV(&order_sql_item_type, &order_sql_item, " ", &sql_order_type);
			} else {
				ZVAL_COPY(&order_sql_item_type, &order_sql_item);
			}
			zval_ptr_dtor(&order_sql_item);
			dao_array_append(&order_items, &order_sql_item_type, 0);
		} ZEND_HASH_FOREACH_END();

		dao_fast_join_str(&order_sql, SL(", "), &order_items);
		zval_ptr_dtor(&order_items);
		DAO_SCONCAT_SV(&sql, " ORDER BY ", &order_sql);
		zval_ptr_dtor(&order_sql);
		DAO_MM_ADD_ENTRY(&sql);
	}

	/**
	 * Check for a LIMIT condition
	 */
	if (dao_array_isset_fetch_str(&limit_value, definition, SL("limit"), PH_READONLY)) {
		if (likely(Z_TYPE(limit_value) == IS_ARRAY)) {
			zval number = {};
			if (likely(dao_array_isset_fetch_str(&number, &limit_value, SL("number"), PH_READONLY))) {
				zval tmp1 = {};
				dao_array_fetch_str(&tmp1, &number, SL("value"), PH_NOISY|PH_READONLY);

				/**
				 * Check for a OFFSET condition
				 */
				if (dao_array_isset_fetch_str(&offset, &limit_value, SL("offset"), PH_READONLY)) {
					zval tmp2 = {};
					dao_array_fetch_str(&tmp2, &offset, SL("value"), PH_NOISY|PH_READONLY);
					DAO_SCONCAT_SVSV(&sql, " LIMIT ", &tmp1, " OFFSET ", &tmp2);
				} else {
					DAO_SCONCAT_SV(&sql, " LIMIT ", &tmp1);
				}
			}
		} else {
			DAO_SCONCAT_SV(&sql, " LIMIT ", &limit_value);
		}
		DAO_MM_ADD_ENTRY(&sql);
	}

	RETURN_MM_CTOR(&sql);
}

/**
 * Checks whether the platform supports savepoints
 *
 * @return boolean
 */
PHP_METHOD(Dao_Db_Dialect, supportsSavepoints){


	RETURN_TRUE;
}

/**
 * Checks whether the platform supports releasing savepoints.
 *
 * @return boolean
 */
PHP_METHOD(Dao_Db_Dialect, supportsReleaseSavepoints)
{
	DAO_RETURN_CALL_METHOD(getThis(), "supportssavepoints");
}

/**
 * Generate SQL to create a new savepoint
 *
 * @param string $name
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect, createSavepoint){

	zval *name;

	dao_fetch_params(0, 1, 0, &name);

	DAO_CONCAT_SV(return_value, "SAVEPOINT ", name);
}

/**
 * Generate SQL to release a savepoint
 *
 * @param string $name
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect, releaseSavepoint){

	zval *name;

	dao_fetch_params(0, 1, 0, &name);

	DAO_CONCAT_SV(return_value, "RELEASE SAVEPOINT ", name);
}

/**
 * Generate SQL to rollback a savepoint
 *
 * @param string $name
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect, rollbackSavepoint){

	zval *name;

	dao_fetch_params(0, 1, 0, &name);

	DAO_CONCAT_SV(return_value, "ROLLBACK TO SAVEPOINT ", name);
}

/**
 * Registers custom SQL functions
 *
 * @return array
 */
PHP_METHOD(Dao_Db_Dialect, registerCustomFunction){

	zval *name, *custom_function;

	dao_fetch_params(0, 2, 0, &name, &custom_function);

	dao_update_property_array(getThis(), SL("_customFunctions"), name, custom_function);

	RETURN_THIS();
}

/**
 * Returns registered functions
 *
 * @return array
 */
PHP_METHOD(Dao_Db_Dialect, getCustomFunctions){


	RETURN_MEMBER(getThis(), "_customFunctions");
}

/**
 * Escape identifiers
 *
 * @param string $str
 * @param string $escapeChar
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect, escape){

	zval *_str, *escape = NULL, str = {}, escape_char = {}, parts = {}, *part;

	dao_fetch_params(0, 1, 1, &_str, &escape);

	if (!escape || Z_TYPE_P(escape) == IS_NULL) {
		dao_read_property(&escape_char, getThis(), SL("_escapeChar"), PH_NOISY|PH_READONLY);
	} else {
		ZVAL_COPY_VALUE(&escape_char, escape);
	}

	if (DAO_IS_EMPTY(&escape_char)) {
		RETURN_CTOR(_str);
	}

	ZVAL_STR(&str, dao_trim(_str, &escape_char, DAO_TRIM_BOTH));

	if (!DAO_GLOBAL(db).escape_identifiers) {
		RETVAL_ZVAL(&str, 0, 0);
		return;
	}

	if (dao_start_with_str(&str, SL("*"))) {
		RETVAL_ZVAL(&str, 0, 0);
		return;
	}

	if (dao_memnstr_str(&str, SL("."))) {
		dao_fast_explode_str(&parts, SL("."), &str);
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL(parts), part) {

			if (DAO_IS_EMPTY(part) || dao_start_with_str(part, SL("*"))) {
				if (DAO_IS_EMPTY(return_value)) {
					ZVAL_COPY(return_value, part);
				} else {
					DAO_SCONCAT_SV(return_value, ".", part);
				}
			} else {
				if (DAO_IS_EMPTY(return_value)) {
					DAO_CONCAT_VVV(return_value, &escape_char, part, &escape_char);
				} else {
					DAO_SCONCAT_SVVV(return_value, ".", &escape_char, part, &escape_char);
				}
			}
		} ZEND_HASH_FOREACH_END();
		zval_ptr_dtor(&parts);
	} else {
		DAO_CONCAT_VVV(return_value, &escape_char, &str, &escape_char);
	}
	zval_ptr_dtor(&str);
}

/**
 * Escape Schema
 *
 * @param string $schema
 * @param string $escapeChar
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect, escapeSchema){

	zval *_schema, *escape = NULL, escape_char = {}, schema = {};

	dao_fetch_params(0, 1, 1, &_schema, &escape);

	if (!escape || Z_TYPE_P(escape) == IS_NULL) {
		dao_read_property(&escape_char, getThis(), SL("_escapeChar"), PH_NOISY|PH_READONLY);
	} else {
		ZVAL_COPY_VALUE(&escape_char, escape);
	}

	if (DAO_IS_EMPTY(&escape_char)) {
		RETURN_CTOR(_schema);
	}

	ZVAL_STR(&schema, dao_trim(_schema, &escape_char, DAO_TRIM_BOTH));

	if (!DAO_GLOBAL(db).escape_identifiers) {
		RETVAL_ZVAL(&schema, 0, 0);
		return;
	}

	DAO_CONCAT_VVV(return_value, &escape_char, &schema, &escape_char);
	zval_ptr_dtor(&schema);
}

/**
 * Prepares table for this RDBMS
 *
 * @param string $table
 * @param string $schema
 * @param string $alias
 * @param string $escapeChar
 * @return string
 */
PHP_METHOD(Dao_Db_Dialect, prepareTable){

	zval *_table, *_schema = NULL, *_alias = NULL, *escape = NULL, table = {};

	dao_fetch_params(1, 1, 3, &_table, &_schema, &_alias, &escape);

	if (!escape) {
		escape = &DAO_GLOBAL(z_null);
	}

	if (DAO_GLOBAL(db).escape_identifiers) {
		zval tmp = {};
		DAO_MM_CALL_METHOD(&table, getThis(), "escape", _table, escape);
		DAO_MM_ADD_ENTRY(&table);
		if (_schema && DAO_IS_NOT_EMPTY(_schema)) {
			zval schema = {};
			DAO_MM_CALL_METHOD(&schema, getThis(), "escapeschema", _schema, escape);
			DAO_MM_ADD_ENTRY(&schema);
			DAO_CONCAT_VSV(&tmp, &schema, ".", &table);
			DAO_MM_ADD_ENTRY(&tmp);
		} else {
			ZVAL_COPY_VALUE(&tmp, &table);
		}

		if (_alias && DAO_IS_NOT_EMPTY(_alias)) {
			zval alias = {};
			DAO_MM_CALL_METHOD(&alias, getThis(), "escape", _alias, escape);
			DAO_MM_ADD_ENTRY(&alias);
			DAO_CONCAT_VSV(return_value, &tmp, " AS ", &alias);
			RETURN_MM();
		} 
		RETURN_MM_CTOR(&tmp);
	} else {
		zval tmp = {};
		if (_schema && DAO_IS_NOT_EMPTY(_schema)) {
			DAO_CONCAT_VSV(&tmp, _schema, ".", _table);
			DAO_MM_ADD_ENTRY(&tmp);
		} else {
			ZVAL_COPY_VALUE(&tmp, _table);
		}

		if (_alias && DAO_IS_NOT_EMPTY(_alias)) {
			DAO_CONCAT_VSV(return_value, &tmp, " AS ", _alias);
			RETURN_MM();
		} 
		RETURN_MM_CTOR(&tmp);
	}
}
