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

#include "db/builder/select.h"
#include "db/builder/join.h"
#include "db/builder/exception.h"
#include "db/adapterinterface.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/object.h"
#include "kernel/exception.h"
#include "kernel/fcall.h"
#include "kernel/concat.h"
#include "kernel/operators.h"
#include "kernel/string.h"
#include "kernel/file.h"
#include "kernel/debug.h"

/**
 * Dao\Db\Builder\Select
 */
zend_class_entry *dao_db_builder_select_ce;

PHP_METHOD(Dao_Db_Builder_Select, __construct);
PHP_METHOD(Dao_Db_Builder_Select, distinct);
PHP_METHOD(Dao_Db_Builder_Select, columns);
PHP_METHOD(Dao_Db_Builder_Select, orderBy);
PHP_METHOD(Dao_Db_Builder_Select, having);
PHP_METHOD(Dao_Db_Builder_Select, limit);
PHP_METHOD(Dao_Db_Builder_Select, offset);
PHP_METHOD(Dao_Db_Builder_Select, groupBy);
PHP_METHOD(Dao_Db_Builder_Select, _execute);
PHP_METHOD(Dao_Db_Builder_Select, count);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_db_builder_select___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, tables)
	ZEND_ARG_TYPE_INFO(0, db, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_db_builder_select_distinct, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, distinct, _IS_BOOL, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_db_builder_select_columns, 0, 0, 1)
	ZEND_ARG_INFO(0, columns)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_db_builder_select_orderby, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, orderBy, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_db_builder_select_having, 0, 0, 1)
	ZEND_ARG_INFO(0, having)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_db_builder_select_limit, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, limit, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, offset, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_db_builder_select_offset, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, offset, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_db_builder_select_groupby, 0, 0, 1)
	ZEND_ARG_INFO(0, group)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_db_builder_select_method_entry[] = {
	PHP_ME(Dao_Db_Builder_Select, __construct, arginfo_dao_db_builder_select___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Db_Builder_Select, distinct, arginfo_dao_db_builder_select_distinct, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Builder_Select, columns, arginfo_dao_db_builder_select_columns, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Builder_Select, orderBy, arginfo_dao_db_builder_select_orderby, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Builder_Select, having, arginfo_dao_db_builder_select_having, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Builder_Select, limit, arginfo_dao_db_builder_select_limit, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Builder_Select, offset, arginfo_dao_db_builder_select_offset, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Builder_Select, groupBy, arginfo_dao_db_builder_select_groupby, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Builder_Select, _execute, arginfo_empty, ZEND_ACC_PROTECTED)
	PHP_ME(Dao_Db_Builder_Select, count, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Db\Builder\Select initializer
 */
DAO_INIT_CLASS(Dao_Db_Builder_Select){

	DAO_REGISTER_CLASS_EX(Dao\\Db\\Builder, Select, db_builder_select, dao_db_builder_join_ce, dao_db_builder_select_method_entry, 0);

	return SUCCESS;
}

/**
 * Dao\Db\Builder\Select constructor
 *
 * @param string|array $tables
 * @param string $db
 */
PHP_METHOD(Dao_Db_Builder_Select, __construct){

	zval *tables, *db = NULL, columns = {};

	dao_fetch_params(0, 1, 1, &tables, &db);

	if (Z_TYPE_P(tables) != IS_STRING && Z_TYPE_P(tables) != IS_ARRAY) {
		DAO_THROW_EXCEPTION_STR(dao_db_builder_exception_ce, "The tables must be string or array");
		return;
	}

	dao_update_property_array_str(getThis(), SL("_definition"), SL("tables"), tables);

	if (db && DAO_IS_NOT_EMPTY(db)) {
		dao_update_property(getThis(), SL("_defaultConnectionService"), db);
	}

	ZVAL_STRINGL(&columns, "*", 1);
	dao_update_property_array_str(getThis(), SL("_definition"), SL("columns"), &columns);
	zval_ptr_dtor(&columns);
}

/**
 * Sets SELECT DISTINCT / SELECT ALL flag
 *
 * @param bool|null distinct
 * @return Dao\Db\Builder\Select
 */
PHP_METHOD(Dao_Db_Builder_Select, distinct){

	zval *distinct;

	dao_fetch_params(0, 1, 0, &distinct);

	dao_update_property_array_str(getThis(), SL("_definition"), SL("distinct"), distinct);
	RETURN_THIS();
}

/**
 * Sets the columns to be queried
 *
 * @param string|array $columns
 * @return Dao\Db\Builder\Select
 */
PHP_METHOD(Dao_Db_Builder_Select, columns){

	zval *columns;

	dao_fetch_params(0, 1, 0, &columns);

	dao_update_property_array_str(getThis(), SL("_definition"), SL("columns"), columns);
	RETURN_THIS();
}

/**
 * Sets a ORDER BY condition clause
 *
 * @param string|array $orderBy
 * @return Dao\Db\Builder\Select
 */
PHP_METHOD(Dao_Db_Builder_Select, orderBy){

	zval *order_by;

	dao_fetch_params(0, 1, 0, &order_by);

	dao_update_property_array_str(getThis(), SL("_definition"), SL("order"), order_by);
	RETURN_THIS();
}

/**
 * Sets a HAVING condition clause. You need to escape PHQL reserved words using [ and ] delimiters
 *
 *<code>
 *	$builder->having('SUM(Robots.price) > 0');
 *</code>
 *
 * @param string $having
 * @return Dao\Db\Builder\Select
 */
PHP_METHOD(Dao_Db_Builder_Select, having){

	zval *having;

	dao_fetch_params(0, 1, 0, &having);

	dao_update_property_array_str(getThis(), SL("_definition"), SL("having"), having);
	RETURN_THIS();
}

/**
 * Sets a LIMIT clause, optionally a offset clause
 *
 * @param int $limit
 * @param int $offset
 * @return Dao\Db\Builder\Select
 */
PHP_METHOD(Dao_Db_Builder_Select, limit){

	zval *limit, *offset = NULL;

	dao_fetch_params(0, 1, 1, &limit, &offset);

	dao_update_property_array_str(getThis(), SL("_definition"), SL("limit"), limit);

	if (offset) {
		dao_update_property_array_str(getThis(), SL("_definition"), SL("offset"), offset);
	}

	RETURN_THIS();
}

/**
 * Sets an OFFSET clause
 *
 * @param int $offset
 * @return Dao\Db\Builder\Select
 */
PHP_METHOD(Dao_Db_Builder_Select, offset){

	zval *offset;

	dao_fetch_params(0, 1, 0, &offset);

	dao_update_property_array_str(getThis(), SL("_definition"), SL("offset"), offset);
	RETURN_THIS();
}

/**
 * Sets a GROUP BY clause
 *
 * @param string|array $group
 * @return Dao\Db\Builder\Select
 */
PHP_METHOD(Dao_Db_Builder_Select, groupBy){

	zval *group;

	dao_fetch_params(0, 1, 0, &group);

	dao_update_property_array_str(getThis(), SL("_definition"), SL("group"), group);
	RETURN_THIS();
}

/**
 * Returns a PHQL statement built based on the builder parameters
 *
 * @return Dao\Db\ResultInterface
 */
PHP_METHOD(Dao_Db_Builder_Select, _execute){

	zval *pretreatment = NULL, *count = NULL, definition = {}, conditions = {}, joins = {}, bind_params = {}, bind_types = {}, service = {}, dependency_injector = {}, connection = {}, dialect = {}, sql_select = {};

	dao_fetch_params(1, 0, 2, &pretreatment, &count);

	if (!pretreatment) {
		pretreatment = &DAO_GLOBAL(z_false);
	}

	if (!count) {
		count = &DAO_GLOBAL(z_false);
	}

	dao_read_property(&definition, getThis(), SL("_definition"), PH_CTOR);
	DAO_MM_ADD_ENTRY(&definition);

	dao_read_property(&conditions, getThis(), SL("_conditions"), PH_READONLY);
	if (DAO_IS_NOT_EMPTY(&conditions)) {
		dao_array_update_str(&definition, SL("where"), &conditions, PH_COPY);
	}

	dao_read_property(&joins, getThis(), SL("_joins"), PH_READONLY);
	if (DAO_IS_NOT_EMPTY(&joins)) {
		dao_array_update_str(&definition, SL("joins"), &joins, PH_COPY);
	}

	DAO_MM_CALL_SELF(&bind_params, "getbindparams");
	DAO_MM_ADD_ENTRY(&bind_params);

	DAO_MM_CALL_SELF(&bind_types, "getbindtypes");
	DAO_MM_ADD_ENTRY(&bind_types);

	dao_read_property(&service, getThis(), SL("_defaultConnectionService"), PH_READONLY);
	if (Z_TYPE(service) != IS_OBJECT) {
		if (DAO_IS_EMPTY(&service)) {
			DAO_MM_THROW_EXCEPTION_STR(dao_db_builder_exception_ce, "Invalid injected connection service");
			return;
		}

		DAO_MM_CALL_METHOD(&dependency_injector, getThis(), "getdi");
		DAO_MM_ADD_ENTRY(&dependency_injector);
		if (Z_TYPE(dependency_injector) != IS_OBJECT) {
			DAO_MM_THROW_EXCEPTION_STR(dao_db_builder_exception_ce, "A dependency injector container is required to obtain the services related to the ORM");
			return;
		}

		/**
		 * Request the connection service from the DI
		 */
		DAO_MM_CALL_METHOD(&connection, &dependency_injector, "getshared", &service);
		DAO_MM_ADD_ENTRY(&connection);
		if (Z_TYPE(connection) != IS_OBJECT) {
			DAO_THROW_EXCEPTION_STR(dao_db_builder_exception_ce, "Invalid injected connection service");
			return;
		}
	} else {
		ZVAL_COPY_VALUE(&connection, &service);
		DAO_MM_VERIFY_INTERFACE(&connection, dao_db_adapterinterface_ce);
	}

	DAO_MM_CALL_METHOD(&dialect, &connection, "getdialect");
	DAO_MM_ADD_ENTRY(&dialect);

	DAO_MM_CALL_METHOD(&sql_select, &dialect, "select", &definition, count);
	DAO_MM_ADD_ENTRY(&sql_select);

	if (zend_is_true(pretreatment)) {
		array_init(return_value);
		dao_array_update_str(return_value, SL("sql"), &sql_select, PH_COPY);
		dao_array_update_str(return_value, SL("variables"), &bind_params, PH_COPY);
		dao_array_update_str(return_value, SL("types"), &bind_types, PH_COPY);
	} else {
		/**
		 * Execute the query
		 */
		DAO_MM_CALL_METHOD(return_value, &connection, "query", &sql_select, &bind_params, &bind_types);
	}

	RETURN_MM();
}

/**
 * Returns a PHQL statement built based on the builder parameters
 *
 * @return Dao\Db\ResultInterface
 */
PHP_METHOD(Dao_Db_Builder_Select, count){

	DAO_CALL_METHOD(return_value, getThis(), "_execute", &DAO_GLOBAL(z_false), &DAO_GLOBAL(z_true));
}
