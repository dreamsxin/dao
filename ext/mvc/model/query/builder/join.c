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

#include "mvc/model/query/builder/join.h"
#include "mvc/model/query/builder/where.h"
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

#include "interned-strings.h"

/**
 * Dao\Mvc\Model\Query\Builder\Join
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
zend_class_entry *dao_mvc_model_query_builder_join_ce;

PHP_METHOD(Dao_Mvc_Model_Query_Builder_Join, join);
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Join, innerJoin);
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Join, leftJoin);
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Join, rightJoin);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_query_builder_join_join, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, model, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, conditions, IS_STRING, 1)
	ZEND_ARG_TYPE_INFO(0, alias, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_query_builder_join_innerjoin, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, model, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, conditions, IS_STRING, 1)
	ZEND_ARG_TYPE_INFO(0, alias, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_query_builder_join_leftjoin, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, model, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, conditions, IS_STRING, 1)
	ZEND_ARG_TYPE_INFO(0, alias, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_query_builder_join_rightjoin, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, model, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, conditions, IS_STRING, 1)
	ZEND_ARG_TYPE_INFO(0, alias, IS_STRING, 1)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_mvc_model_query_builder_join_method_entry[] = {
	PHP_ME(Dao_Mvc_Model_Query_Builder_Join, join, arginfo_dao_mvc_model_query_builder_join_join, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Query_Builder_Join, innerJoin, arginfo_dao_mvc_model_query_builder_join_innerjoin, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Query_Builder_Join, leftJoin, arginfo_dao_mvc_model_query_builder_join_leftjoin, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Query_Builder_Join, rightJoin, arginfo_dao_mvc_model_query_builder_join_rightjoin, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Mvc\Model\Query\Builder\Join initializer
 */
DAO_INIT_CLASS(Dao_Mvc_Model_Query_Builder_Join){

	DAO_REGISTER_CLASS_EX(Dao\\Mvc\\Model\\Query\\Builder, Join, mvc_model_query_builder_join, dao_mvc_model_query_builder_where_ce, dao_mvc_model_query_builder_join_method_entry, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	zend_declare_property_null(dao_mvc_model_query_builder_join_ce, SL("_joins"), ZEND_ACC_PROTECTED);

	zend_class_implements(dao_mvc_model_query_builder_join_ce, 1, dao_mvc_model_query_builderinterface_ce);

	return SUCCESS;
}

/**
 * Adds a join to the query
 *
 *<code>
 *	$builder->join('Robots');
 *	$builder->join('Robots', 'r.id = RobotsParts.robots_id');
 *	$builder->join('Robots', 'r.id = RobotsParts.robots_id', 'r');
 *	$builder->join('Robots', 'r.id = RobotsParts.robots_id', 'r', 'LEFT');
 *</code>
 *
 * @param string $model
 * @param string $conditions
 * @param string $alias
 * @param string $type
 * @return Dao\Mvc\Model\Query\Builder\Join
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Join, join){

	zval *model, *conditions = NULL, *alias = NULL, *type = NULL, join = {};

	dao_fetch_params(0, 1, 3, &model, &conditions, &alias, &type);

	if (!conditions) {
		conditions = &DAO_GLOBAL(z_null);
	}

	if (!alias) {
		alias = &DAO_GLOBAL(z_null);
	}

	if (!type) {
		type = &DAO_GLOBAL(z_null);
	}

	array_init_size(&join, 4);
	dao_array_append(&join, model, PH_COPY);
	dao_array_append(&join, conditions, PH_COPY);
	dao_array_append(&join, alias, PH_COPY);
	dao_array_append(&join, type, PH_COPY);
	dao_update_property_array_append(getThis(), SL("_joins"), &join);
	zval_ptr_dtor(&join);
	RETURN_THIS();
}

/**
 * Adds a INNER join to the query
 *
 *<code>
 *	$builder->innerJoin('Robots');
 *	$builder->innerJoin('Robots', 'r.id = RobotsParts.robots_id');
 *	$builder->innerJoin('Robots', 'r.id = RobotsParts.robots_id', 'r');
 *</code>
 *
 * @param string $model
 * @param string $conditions
 * @param string $alias
 * @return Dao\Mvc\Model\Query\Builder\Join
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Join, innerJoin){

	zval *model, *conditions = NULL, *alias = NULL, type = {}, join = {};

	dao_fetch_params(0, 1, 2, &model, &conditions, &alias);

	if (!conditions) {
		conditions = &DAO_GLOBAL(z_null);
	}

	if (!alias) {
		alias = &DAO_GLOBAL(z_null);
	}

	ZVAL_STRING(&type, "INNER");

	array_init_size(&join, 4);
	dao_array_append(&join, model, PH_COPY);
	dao_array_append(&join, conditions, PH_COPY);
	dao_array_append(&join, alias, PH_COPY);
	dao_array_append(&join, &type, 0);
	dao_update_property_array_append(getThis(), SL("_joins"), &join);
	zval_ptr_dtor(&join);
	RETURN_THIS();
}

/**
 * Adds a LEFT join to the query
 *
 *<code>
 *	$builder->leftJoin('Robots', 'r.id = RobotsParts.robots_id', 'r');
 *</code>
 *
 * @param string $model
 * @param string $conditions
 * @param string $alias
 * @return Dao\Mvc\Model\Query\Builder\Join
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Join, leftJoin){

	zval *model, *conditions = NULL, *alias = NULL, type = {}, join = {};

	dao_fetch_params(0, 1, 2, &model, &conditions, &alias);

	if (!conditions) {
		conditions = &DAO_GLOBAL(z_null);
	}

	if (!alias) {
		alias = &DAO_GLOBAL(z_null);
	}

	ZVAL_STRING(&type, "LEFT");

	array_init_size(&join, 4);
	dao_array_append(&join, model, PH_COPY);
	dao_array_append(&join, conditions, PH_COPY);
	dao_array_append(&join, alias, PH_COPY);
	dao_array_append(&join, &type, 0);
	dao_update_property_array_append(getThis(), SL("_joins"), &join);
	zval_ptr_dtor(&join);
	RETURN_THIS();
}

/**
 * Adds a RIGHT join to the query
 *
 *<code>
 *	$builder->rightJoin('Robots', 'r.id = RobotsParts.robots_id', 'r');
 *</code>
 *
 * @param string $model
 * @param string $conditions
 * @param string $alias
 * @return Dao\Mvc\Model\Query\Builder\Join
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder_Join, rightJoin){

	zval *model, *conditions = NULL, *alias = NULL, type = {}, join = {};

	dao_fetch_params(0, 1, 2, &model, &conditions, &alias);

	if (!conditions) {
		conditions = &DAO_GLOBAL(z_null);
	}

	if (!alias) {
		alias = &DAO_GLOBAL(z_null);
	}

	ZVAL_STRING(&type, "RIGHT");

	array_init_size(&join, 4);
	dao_array_append(&join, model, PH_COPY);
	dao_array_append(&join, conditions, PH_COPY);
	dao_array_append(&join, alias, PH_COPY);
	dao_array_append(&join, &type, 0);
	dao_update_property(getThis(), SL("_joins"), &join);
	zval_ptr_dtor(&join);
	RETURN_THIS();
}
