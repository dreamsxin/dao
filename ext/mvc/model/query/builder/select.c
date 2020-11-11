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

#include "mvc/model/query/builder/select.h"
#include "mvc/model/query/builder/join.h"
#include "mvc/model/query/builderinterface.h"
#include "mvc/model/query/exception.h"
#include "mvc/model/metadatainterface.h"
#include "mvc/model/metadata/memory.h"
#include "mvc/model/query.h"
#include "mvc/model/query/scanner.h"
#include "di.h"
#include "diinterface.h"
#include "di/injectable.h"
#include "db/rawvalue.h"

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
#include "kernel/hash.h"
#include "kernel/framework/orm.h"
#include "kernel/debug.h"

#include "interned-strings.h"

/**
 * Dao\Mvc\Model\Query\Builder\Select
 *
 *<code>
 *$resultset = $this->modelsManager->createBuilder()
 *   ->from('Robots')
 *   ->join('RobotsParts')
 *   ->limit(20)
 *   ->orderBy('Robots.name')
 *   ->getQuery()
 *   ->execute();
 *</code>
 */
zend_class_entry *dao_mvc_model_query_builder_select_ce;

PHP_METHOD(Dao_Mvc_Model_Query_Builder_Select, __construct);
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Select, distinct);
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Select, getDistinct);
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Select, columns);
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Select, getColumns);
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Select, from);
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Select, addFrom);
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Select, getFrom);
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Select, orderBy);
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Select, getOrderBy);
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Select, having);
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Select, getHaving);
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Select, limit);
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Select, getLimit);
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Select, offset);
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Select, getOffset);
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Select, groupBy);
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Select, getGroupBy);
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Select, cache);
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Select, _compile);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_query_builder_select___construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, params, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_query_builder_select_distinct, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, distinct, _IS_BOOL, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_query_builder_select_columns, 0, 0, 1)
	ZEND_ARG_INFO(0, columns)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_query_builder_select_from, 0, 0, 1)
	ZEND_ARG_INFO(0, model)
	ZEND_ARG_TYPE_INFO(0, alias, IS_STRING, 1)
	ZEND_ARG_TYPE_INFO(0, merge, _IS_BOOL, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_query_builder_select_addfrom, 0, 0, 1)
	ZEND_ARG_INFO(0, model)
	ZEND_ARG_TYPE_INFO(0, alias, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_query_builder_select_orderby, 0, 0, 1)
	ZEND_ARG_INFO(0, orderBy)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_query_builder_select_having, 0, 0, 1)
	ZEND_ARG_INFO(0, having)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_query_builder_select_limit, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, limit, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, offset, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_query_builder_select_offset, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, offset, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_query_builder_select_groupby, 0, 0, 1)
	ZEND_ARG_INFO(0, group)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_query_builder_select_cache, 0, 0, 1)
	ZEND_ARG_INFO(0, cacheOptions)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_mvc_model_query_builder_select_method_entry[] = {
	PHP_ME(Dao_Mvc_Model_Query_Builder_Select, __construct, arginfo_dao_mvc_model_query_builder_select___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Mvc_Model_Query_Builder_Select, distinct, arginfo_dao_mvc_model_query_builder_select_distinct, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Query_Builder_Select, getDistinct, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Query_Builder_Select, columns, arginfo_dao_mvc_model_query_builder_select_columns, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Query_Builder_Select, getColumns, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Query_Builder_Select, from, arginfo_dao_mvc_model_query_builder_select_from, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Query_Builder_Select, addFrom, arginfo_dao_mvc_model_query_builder_select_addfrom, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Query_Builder_Select, getFrom, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Query_Builder_Select, orderBy, arginfo_dao_mvc_model_query_builder_select_orderby, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Query_Builder_Select, getOrderBy, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Query_Builder_Select, having, arginfo_dao_mvc_model_query_builder_select_having, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Query_Builder_Select, getHaving, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Query_Builder_Select, limit, arginfo_dao_mvc_model_query_builder_select_limit, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Query_Builder_Select, getLimit, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Query_Builder_Select, offset, arginfo_dao_mvc_model_query_builder_select_offset, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Query_Builder_Select, getOffset, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Query_Builder_Select, groupBy, arginfo_dao_mvc_model_query_builder_select_groupby, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Query_Builder_Select, getGroupBy, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Query_Builder_Select, cache, arginfo_dao_mvc_model_query_builder_select_cache, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Query_Builder_Select, _compile, arginfo_empty, ZEND_ACC_PROTECTED)
	PHP_FE_END
};

/**
 * Dao\Mvc\Model\Query\Builder\Select initializer
 */
DAO_INIT_CLASS(Dao_Mvc_Model_Query_Builder_Select){

	DAO_REGISTER_CLASS_EX(Dao\\Mvc\\Model\\Query\\Builder, Select, mvc_model_query_builder_select, dao_mvc_model_query_builder_join_ce, dao_mvc_model_query_builder_select_method_entry, 0);

	zend_declare_property_long(dao_mvc_model_query_builder_select_ce, SL("_type"), PHQL_T_SELECT, ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_model_query_builder_select_ce, SL("_columns"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_model_query_builder_select_ce, SL("_models"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_model_query_builder_select_ce, SL("_group"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_model_query_builder_select_ce, SL("_having"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_model_query_builder_select_ce, SL("_order"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_model_query_builder_select_ce, SL("_limit"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_model_query_builder_select_ce, SL("_offset"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_model_query_builder_select_ce, SL("_forUpdate"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_model_query_builder_select_ce, SL("_sharedLock"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_model_query_builder_select_ce, SL("_distinct"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_model_query_builder_select_ce, SL("_cache"), ZEND_ACC_PROTECTED);

	zend_class_implements(dao_mvc_model_query_builder_select_ce, 1, dao_mvc_model_query_builderinterface_ce);

	return SUCCESS;
}

/**
 * Dao\Mvc\Model\Query\Builder\Select constructor
 *
 *<code>
 * $params = array(
 *    'models'     => array('Users'),
 *    'columns'    => array('id', 'name', 'status'),
 *    'conditions' => array(
 *        array(
 *            "created > :min: AND created < :max:",
 *            array("min" => '2013-01-01',   'max' => '2014-01-01'),
 *            array("min" => PDO::PARAM_STR, 'max' => PDO::PARAM_STR),
 *        ),
 *    ),
 *    // or 'conditions' => "created > '2013-01-01' AND created < '2014-01-01'",
 *    'group'      => array('id', 'name'),
 *    'having'     => "name = 'Kamil'",
 *    'order'      => array('name', 'id'),
 *    'limit'      => 20,
 *    'offset'     => 20,
 *    // or 'limit' => array(20, 20),
 * );
 * $queryBuilder = new Dao\Mvc\Model\Query\Builder\Select($params);
 *</code>
 *
 * @param array $params
 * @param Dao\Di $dependencyInjector
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Select, __construct){

	zval *params = NULL, *dependency_injector = NULL;

	dao_fetch_params(0, 0, 2, &params, &dependency_injector);

	/**
	 * Update the dependency injector if any
	 */
	if (dependency_injector && Z_TYPE_P(dependency_injector) != IS_NULL) {
		DAO_CALL_METHOD(NULL, getThis(), "setdi", dependency_injector);
	}

	if (params && Z_TYPE_P(params) == IS_ARRAY) {
		zval cache = {}, conditions = {}, bind_params = {}, bind_types = {}, models = {}, index = {}, columns = {}, group_clause = {}, joins = {};
		zval having_clause = {}, order_clause = {}, limit_clause = {}, offset_clause = {}, limit = {}, offset = {}, for_update = {}, shared_lock = {};

		if (dao_array_isset_fetch_str(&cache, params, SL("cache"), PH_READONLY)) {
			dao_update_property(getThis(), SL("_cache"), &cache);
		}

		/**
		 * Process conditions
		 */
		if (dao_array_isset_fetch_str(&conditions, params, SL("conditions"), PH_READONLY)
			|| dao_array_isset_fetch_long(&conditions, params, 0, PH_READONLY)) {
			DAO_CALL_METHOD(NULL, getThis(), "setconditions", &conditions);
		}

		if (dao_array_isset_fetch_str(&bind_params, params, SL("bind"), PH_READONLY)) {
			DAO_CALL_METHOD(NULL, getThis(), "setbindparams", &bind_params);
		} else if (dao_array_isset_fetch_str(&bind_params, params, SL("bindParams"), PH_READONLY)) {
			DAO_CALL_METHOD(NULL, getThis(), "setbindparams", &bind_params);
		}

		if (dao_array_isset_fetch_str(&bind_types, params, SL("bindTypes"), PH_READONLY)) {
			DAO_CALL_METHOD(NULL, getThis(), "setbindtypes", &bind_types);
		}

		/**
		 * Assign 'FROM' clause
		 */
		if (dao_array_isset_fetch_str(&models, params, SL("models"), PH_READONLY)) {
			DAO_CALL_METHOD(NULL, getThis(), "from", &models);
		}

		if (dao_array_isset_fetch_str(&index, params, SL("index"), PH_READONLY)) {
			DAO_CALL_METHOD(NULL, getThis(), "setindex", &index);
		}

		/**
		 * Assign COLUMNS clause
		 */
		if (dao_array_isset_fetch_str(&columns, params, SL("columns"), PH_READONLY)) {
			dao_update_property(getThis(), SL("_columns"), &columns);
		}

		/**
		 * Assign JOIN clause
		 */
		if (dao_array_isset_fetch_str(&joins, params, SL("joins"), PH_READONLY)) {
			dao_update_property(getThis(), SL("_joins"), &joins);
		}

		/**
		 * Assign GROUP clause
		 */
		if (dao_array_isset_fetch_str(&group_clause, params, SL("group"), PH_READONLY)) {
			dao_update_property(getThis(), SL("_group"), &group_clause);
		}

		/**
		 * Assign HAVING clause
		 */
		if (dao_array_isset_fetch_str(&having_clause, params, SL("having"), PH_READONLY)) {
			dao_update_property(getThis(), SL("_having"), &having_clause);
		}

		/**
		 * Assign ORDER clause
		 */
		if (dao_array_isset_fetch_str(&order_clause, params, SL("order"), PH_READONLY)) {
			dao_update_property(getThis(), SL("_order"), &order_clause);
		}

		/**
		 * Assign LIMIT clause
		 */
		if (dao_array_isset_fetch_str(&limit_clause, params, SL("limit"), PH_READONLY)) {
			if (Z_TYPE(limit_clause) == IS_ARRAY
				&& dao_array_isset_fetch_long(&limit, &limit_clause, 0, PH_READONLY)
				&& dao_array_isset_fetch_long(&offset, &limit_clause, 1, PH_READONLY)
			) {
				dao_update_property(getThis(), SL("_limit"), &limit);
				dao_update_property(getThis(), SL("_offset"), &offset);
			} else {
				dao_update_property(getThis(), SL("_limit"), &limit_clause);
			}
		}

		/**
		 * Assign OFFSET clause
		 */
		if (dao_array_isset_fetch_str(&offset_clause, params, SL("offset"), PH_READONLY)) {
			dao_update_property(getThis(), SL("_offset"), &offset_clause);
		}

		/**
		 * Assign FOR UPDATE clause
		 */
		if (dao_array_isset_fetch_str(&for_update, params, SL("for_update"), PH_READONLY)) {
			dao_update_property(getThis(), SL("_forUpdate"), &for_update);
		} else if (dao_array_isset_fetch_str(&for_update, params, SL("forUpdate"), PH_READONLY)) {
			dao_update_property(getThis(), SL("_forUpdate"), &for_update);
		}

		/**
		 * Assign SHARED LOCK clause
		 */
		if (dao_array_isset_fetch_str(&shared_lock, params, SL("shared_lock"), PH_READONLY)) {
			dao_update_property(getThis(), SL("_sharedLock"), &shared_lock);
		} else if (dao_array_isset_fetch_str(&shared_lock, params, SL("sharedLock"), PH_READONLY)) {
			dao_update_property(getThis(), SL("_sharedLock"), &shared_lock);
		}
	}
}

/**
 * Sets SELECT DISTINCT / SELECT ALL flag
 *
 * @param bool|null distinct
 * @return Dao\Mvc\Model\Query\Builder\Select
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Select, distinct){

	zval *distinct;

	dao_fetch_params(0, 1, 0, &distinct);

	dao_update_property(getThis(), SL("_distinct"), distinct);
	RETURN_THIS();
}

/**
 * Returns SELECT DISTINCT / SELECT ALL flag
 *
 * @return boolean
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Select, getDistinct){


	RETURN_MEMBER(getThis(), "_distinct");
}

/**
 * Sets the columns to be queried
 *
 *<code>
 *	$builder->columns(array('id', 'name'));
 *</code>
 *
 * @param string|array $columns
 * @return Dao\Mvc\Model\Query\Builder\Select
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Select, columns){

	zval *columns;

	dao_fetch_params(0, 1, 0, &columns);

	dao_update_property(getThis(), SL("_columns"), columns);
	RETURN_THIS();
}

/**
 * Return the columns to be queried
 *
 * @return string|array
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Select, getColumns){


	RETURN_MEMBER(getThis(), "_columns");
}

/**
 * Sets the models who makes part of the query
 *
 *<code>
 *	$builder->from('Robots');
 *	$builder->from(array('Robots', 'RobotsParts'));
 *</code>
 *
 * @param string|array $model
 * @return Dao\Mvc\Model\Query\Builder\Select
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Select, from){

	zval *model, *alias = NULL, *merge = NULL, models = {};

	dao_fetch_params(0, 1, 2, &model, &alias, &merge);

	if (!alias) {
		alias = &DAO_GLOBAL(z_null);
	}

	if (!merge) {
		merge = &DAO_GLOBAL(z_false);
	}

	if (zend_is_true(merge)) {
		dao_read_property(&models, getThis(), SL("_models"), PH_COPY);
		if (Z_TYPE(models) != IS_ARRAY) {
			array_init(&models);
		}
	} else {
		array_init(&models);
	}

	if (Z_TYPE_P(model) == IS_ARRAY) {
		dao_array_merge_recursive_n(&models, model);
	} else if (Z_TYPE_P(model) != IS_NULL) {
		if (Z_TYPE_P(alias) == IS_STRING) {
			dao_array_update(&models, alias, model, PH_COPY);
		} else {
			dao_array_append(&models, model, PH_COPY);
		}
	}

	dao_update_property(getThis(), SL("_models"), &models);
	zval_ptr_dtor(&models);

	RETURN_THIS();
}

/**
 * Add a model to take part of the query
 *
 *<code>
 *	$builder->addFrom('Robots', 'r');
 *</code>
 *
 * @param string|array $model
 * @param string $alias
 * @return Dao\Mvc\Model\Query\Builder\Select
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Select, addFrom){

	zval *model, *alias = NULL;

	dao_fetch_params(0, 1, 1, &model, &alias);

	if (!alias) {
		alias = &DAO_GLOBAL(z_null);
	}

	DAO_CALL_METHOD(NULL, getThis(), "from", model, alias, &DAO_GLOBAL(z_true));

	RETURN_THIS();
}

/**
 * Return the models who makes part of the query
 *
 * @return array
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Select, getFrom){


	RETURN_MEMBER(getThis(), "_models");
}

/**
 * Sets a ORDER BY condition clause
 *
 *<code>
 *	$builder->orderBy('Robots.name');
 *	$builder->orderBy(array('1', 'Robots.name'));
 *</code>
 *
 * @param string|array $orderBy
 * @return Dao\Mvc\Model\Query\Builder\Select
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Select, orderBy){

	zval *order_by;

	dao_fetch_params(0, 1, 0, &order_by);

	dao_update_property(getThis(), SL("_order"), order_by);
	RETURN_THIS();
}

/**
 * Returns the set ORDER BY clause
 *
 * @return string|array
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Select, getOrderBy){


	RETURN_MEMBER(getThis(), "_order");
}

/**
 * Sets a HAVING condition clause. You need to escape PHQL reserved words using [ and ] delimiters
 *
 *<code>
 *	$builder->having('SUM(Robots.price) > 0');
 *</code>
 *
 * @param string $having
 * @return Dao\Mvc\Model\Query\Builder\Select
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Select, having){

	zval *having;

	dao_fetch_params(0, 1, 0, &having);

	dao_update_property(getThis(), SL("_having"), having);
	RETURN_THIS();
}

/**
 * Return the current having clause
 *
 * @return string|array
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Select, getHaving){


	RETURN_MEMBER(getThis(), "_having");
}

/**
 * Sets a LIMIT clause, optionally a offset clause
 *
 *<code>
 *	$builder->limit(100);
 *	$builder->limit(100, 20);
 *</code>
 *
 * @param int $limit
 * @param int $offset
 * @return Dao\Mvc\Model\Query\Builder\Select
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Select, limit){

	zval *limit, *offset = NULL;

	dao_fetch_params(0, 1, 1, &limit, &offset);

	dao_update_property(getThis(), SL("_limit"), limit);

	if (offset) {
		dao_update_property(getThis(), SL("_offset"), offset);
	}

	RETURN_THIS();
}

/**
 * Returns the current LIMIT clause
 *
 * @return string|array
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Select, getLimit){


	RETURN_MEMBER(getThis(), "_limit");
}

/**
 * Sets an OFFSET clause
 *
 *<code>
 *	$builder->offset(30);
 *</code>
 *
 * @param int $offset
 * @return Dao\Mvc\Model\Query\Builder\Select
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Select, offset){

	zval *offset;

	dao_fetch_params(0, 1, 0, &offset);

	dao_update_property(getThis(), SL("_offset"), offset);
	RETURN_THIS();
}

/**
 * Returns the current OFFSET clause
 *
 * @return string|array
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Select, getOffset){


	RETURN_MEMBER(getThis(), "_offset");
}

/**
 * Sets a GROUP BY clause
 *
 *<code>
 *	$builder->groupBy(array('Robots.name'));
 *</code>
 *
 * @param string|array $group
 * @return Dao\Mvc\Model\Query\Builder\Select
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Select, groupBy){

	zval *group;

	dao_fetch_params(0, 1, 0, &group);

	dao_update_property(getThis(), SL("_group"), group);
	RETURN_THIS();
}

/**
 * Returns the GROUP BY clause
 *
 * @return string|array
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Select, getGroupBy){


	RETURN_MEMBER(getThis(), "_group");
}

/**
 * Sets the cache parameters
 *
 * @param array $cacheOptions
 * @return Dao\Mvc\Model\Query\Builder\Select
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Select, cache){

	zval *cache_options;

	dao_fetch_params(0, 1, 0, &cache_options);

	dao_update_property(getThis(), SL("_cache"), cache_options);
	RETURN_THIS();
}

/**
 * Returns a PHQL statement built based on the builder parameters
 *
 * @return string
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Select, _compile){

	zval models = {}, *model, conditions = {}, distinct = {}, phql = {}, columns = {}, selected_columns = {};
	zval *column, joined_columns = {}, selected_models = {}, joined_models = {}, joins = {}, *join, group = {};
	zval having = {}, order = {}, limit = {}, offset = {}, for_update = {}, bind_params = {}, bind_types = {};
	zend_string *str_key;
	ulong idx;

	DAO_MM_INIT();
	dao_read_property(&models, getThis(), SL("_models"), PH_NOISY|PH_READONLY);
	if (Z_TYPE(models) == IS_ARRAY) {
		if (!dao_fast_count_ev(&models)) {
			DAO_MM_THROW_EXCEPTION_STR(dao_mvc_model_query_exception_ce, "At least one model is required to build the query");
			return;
		}
	} else {
		DAO_MM_THROW_EXCEPTION_STR(dao_mvc_model_query_exception_ce, "At least one model is required to build the query");
		return;
	}
	DAO_MM_CALL_SELF(&conditions, "getConditions");
	DAO_MM_ADD_ENTRY(&conditions);
	dao_read_property(&distinct, getThis(), SL("_distinct"), PH_NOISY|PH_READONLY);
	if (DAO_IS_BOOL(&distinct)) {
		if (Z_TYPE(distinct) == IS_TRUE) {
			DAO_MM_ZVAL_STRING(&phql, "SELECT DISTINCT ");
		} else {
			DAO_MM_ZVAL_STRING(&phql, "SELECT ALL ");
		}
	} else {
		DAO_MM_ZVAL_STRING(&phql, "SELECT ");
	}

	dao_read_property(&columns, getThis(), SL("_columns"), PH_NOISY|PH_READONLY);
	if (Z_TYPE(columns) != IS_NULL) {
		/**
		 * Generate PHQL for columns
		 */
		if (Z_TYPE(columns) == IS_ARRAY) {

			array_init(&selected_columns);
			DAO_MM_ADD_ENTRY(&selected_columns);
			ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL(columns), idx, str_key, column) {
				zval column_alias = {}, aliased_column = {};
				if (str_key) {
					ZVAL_STR(&column_alias, str_key);
				} else {
					ZVAL_LONG(&column_alias, idx);
				}

				if (Z_TYPE(column_alias) == IS_LONG) {
					dao_array_append(&selected_columns, column, PH_COPY);
				} else {
					DAO_CONCAT_VSV(&aliased_column, column, " AS ", &column_alias);
					dao_array_append(&selected_columns, &aliased_column, 0);
				}
			} ZEND_HASH_FOREACH_END();

			dao_fast_join_str(&joined_columns, SL(", "), &selected_columns);
			dao_concat_self(&phql, &joined_columns);
			zval_ptr_dtor(&joined_columns);
			DAO_MM_ADD_ENTRY(&phql);
		} else {
			dao_concat_self(&phql, &columns);
			DAO_MM_ADD_ENTRY(&phql);
		}
	} else {
		/**
		 * Automatically generate an array of models
		 */
		if (Z_TYPE(models) == IS_ARRAY) {
			array_init(&selected_columns);
			DAO_MM_ADD_ENTRY(&selected_columns);
			ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL(models), idx, str_key, model) {
				zval model_column_alias = {}, selected_column = {};
				if (str_key) {
					ZVAL_STR(&model_column_alias, str_key);
				} else {
					ZVAL_LONG(&model_column_alias, idx);
				}

				if (Z_TYPE(model_column_alias) == IS_LONG) {
					DAO_CONCAT_SVS(&selected_column, "[", model, "].*");
				} else {
					DAO_CONCAT_SVS(&selected_column, "[", &model_column_alias, "].*");
				}
				dao_array_append(&selected_columns, &selected_column, 0);
			} ZEND_HASH_FOREACH_END();

			dao_fast_join_str(&joined_columns, SL(", "), &selected_columns);
			dao_concat_self(&phql, &joined_columns);
			zval_ptr_dtor(&joined_columns);
			DAO_MM_ADD_ENTRY(&phql);
		} else {
			DAO_SCONCAT_SVS(&phql, "[", &models, "].*");
			DAO_MM_ADD_ENTRY(&phql);
		}
	}

	/**
	 * Join multiple models or use a single one if it is a string
	 */
	if (Z_TYPE(models) == IS_ARRAY) {
		array_init(&selected_models);
		DAO_MM_ADD_ENTRY(&selected_models);
		ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL(models), idx, str_key, model) {
			zval model_alias = {}, selected_model = {};
			if (str_key) {
				ZVAL_STR(&model_alias, str_key);
			} else {
				ZVAL_LONG(&model_alias, idx);
			}

			if (Z_TYPE(model_alias) == IS_STRING) {
				DAO_CONCAT_SVSVS(&selected_model, "[", model, "] AS [", &model_alias, "]");
			} else {
				DAO_CONCAT_SVS(&selected_model, "[", model, "]");
			}
			dao_array_append(&selected_models, &selected_model, 0);
		} ZEND_HASH_FOREACH_END();

		dao_fast_join_str(&joined_models, SL(", "), &selected_models);
		DAO_SCONCAT_SV(&phql, " FROM ", &joined_models);
		zval_ptr_dtor(&joined_models);
		DAO_MM_ADD_ENTRY(&phql);
	} else {
		DAO_SCONCAT_SVS(&phql, " FROM [", &models, "]");
		DAO_MM_ADD_ENTRY(&phql);
	}

	/**
	 * Check if joins were passed to the builders
	 */
	dao_read_property(&joins, getThis(), SL("_joins"), PH_NOISY|PH_READONLY);
	if (Z_TYPE(joins) == IS_ARRAY) {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL(joins), join) {
			zval join_model = {}, join_conditions = {}, join_alias = {}, join_type = {};
			/**
			 * The joined table is in the first place of the array
			 */
			dao_array_fetch_long(&join_model, join, 0, PH_NOISY|PH_READONLY);

			/**
			 * The join conditions are in the second place of the array
			 */
			dao_array_fetch_long(&join_conditions, join, 1, PH_NOISY|PH_READONLY);

			/**
			 * The join alias is in the second place of the array
			 */
			dao_array_fetch_long(&join_alias, join, 2, PH_NOISY|PH_READONLY);

			/**
			 * Join type
			 */
			dao_array_fetch_long(&join_type, join, 3, PH_NOISY|PH_READONLY);

			/**
			 * Create the join according to the type
			 */
			if (zend_is_true(&join_type)) {
				DAO_SCONCAT_SVSVS(&phql, " ", &join_type, " JOIN [", &join_model, "]");
			} else {
				DAO_SCONCAT_SVS(&phql, " JOIN [", &join_model, "]");
			}
			DAO_MM_ADD_ENTRY(&phql);

			/**
			 * Alias comes first
			 */
			if (zend_is_true(&join_alias)) {
				DAO_SCONCAT_SVS(&phql, " AS [", &join_alias, "]");
				DAO_MM_ADD_ENTRY(&phql);
			}

			/**
			 * Conditions then
			 */
			if (zend_is_true(&join_conditions)) {
				DAO_SCONCAT_SV(&phql, " ON ", &join_conditions);
				DAO_MM_ADD_ENTRY(&phql);
			}
		} ZEND_HASH_FOREACH_END();
	}

	/**
	 * Only append conditions if it's string
	 */
	if (Z_TYPE(conditions) == IS_STRING && DAO_IS_NOT_EMPTY(&conditions)) {
		DAO_SCONCAT_SV(&phql, " WHERE ", &conditions);
		DAO_MM_ADD_ENTRY(&phql);
	}

	/**
	 * Process group parameters
	 */
	dao_read_property(&group, getThis(), SL("_group"), PH_NOISY|PH_READONLY);
	if (DAO_IS_NOT_EMPTY(&group)) {
		dao_orm_phql_build_group(&phql, &group);
		DAO_MM_ADD_ENTRY(&phql);
	}

	/* Process HAVING clause */
	dao_read_property(&having, getThis(), SL("_having"), PH_NOISY|PH_READONLY);
	if (Z_TYPE(having) != IS_NULL) {
		if (DAO_IS_NOT_EMPTY(&having)) {
			DAO_SCONCAT_SV(&phql, " HAVING ", &having);
			DAO_MM_ADD_ENTRY(&phql);
		}
	}

	/**
	 * Process order clause
	 */
	dao_read_property(&order, getThis(), SL("_order"), PH_NOISY|PH_READONLY);
	if (DAO_IS_NOT_EMPTY(&order)) {
		dao_orm_phql_build_order(&phql, &order);
		DAO_MM_ADD_ENTRY(&phql);
	}

	/**
	 * Process limit parameters
	 */
	dao_read_property(&limit, getThis(), SL("_limit"), PH_NOISY|PH_READONLY);
	if (DAO_IS_NOT_EMPTY(&limit)) {
		if (Z_TYPE(limit) != IS_ARRAY) {
			zval tmp = {};
			dao_read_property(&offset, getThis(), SL("_offset"), PH_READONLY);
			if (DAO_IS_NOT_EMPTY(&offset)) {
				DAO_CONCAT_VSV(&tmp, &limit, " OFFSET ", &offset);
				DAO_MM_ADD_ENTRY(&tmp);
			} else {
				ZVAL_COPY_VALUE(&tmp, &limit);
			}
			if (DAO_IS_NOT_EMPTY(&tmp)) {
				dao_orm_phql_build_limit(&phql, &tmp);
				DAO_MM_ADD_ENTRY(&phql);
			}
		} else if (DAO_IS_NOT_EMPTY(&limit)) {
			dao_orm_phql_build_limit(&phql, &limit);
			DAO_MM_ADD_ENTRY(&phql);
		}
	}

	/**
	 * Process FOR UPDATE clause
	 */
	dao_read_property(&for_update, getThis(), SL("_forUpdate"), PH_NOISY|PH_READONLY);
	if (zend_is_true(&for_update)) {
		dao_concat_self_str(&phql, SL(" FOR UPDATE"));
		DAO_MM_ADD_ENTRY(&phql);
	}

	DAO_MM_CALL_SELF(&bind_params, "getbindparams");
	dao_update_property(getThis(), SL("_mergeBindParams"), &bind_params);
	zval_ptr_dtor(&bind_params);

	DAO_MM_CALL_SELF(&bind_types, "getbindtypes");
	dao_update_property(getThis(), SL("_mergeBindTypes"), &bind_types);
	zval_ptr_dtor(&bind_types);

	dao_update_property(getThis(), SL("_phql"), &phql);
	RETURN_MM();
}
