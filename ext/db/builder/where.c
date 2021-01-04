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

#include "db/builder/where.h"
#include "db/builder.h"
#include "db/builder/exception.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/object.h"
#include "kernel/exception.h"
#include "kernel/fcall.h"
#include "kernel/concat.h"
#include "kernel/operators.h"
#include "kernel/string.h"

/**
 * Dao\Db\Builder\Where
 *
 * Helps to create PHQL queries for WHERE statements
 */
zend_class_entry *dao_db_builder_where_ce;

PHP_METHOD(Dao_Db_Builder_Where, setConditions);
PHP_METHOD(Dao_Db_Builder_Where, getConditions);
PHP_METHOD(Dao_Db_Builder_Where, where);
PHP_METHOD(Dao_Db_Builder_Where, andWhere);
PHP_METHOD(Dao_Db_Builder_Where, orWhere);
PHP_METHOD(Dao_Db_Builder_Where, inWhere);
PHP_METHOD(Dao_Db_Builder_Where, notInWhere);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_db_builder_where_setconditions, 0, 0, 1)
	ZEND_ARG_INFO(0, conditions)
	ZEND_ARG_TYPE_INFO(0, bindParams, IS_ARRAY, 1)
	ZEND_ARG_TYPE_INFO(0, bindTypes, IS_ARRAY, 1)
	ZEND_ARG_TYPE_INFO(0, type, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, grouping, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_db_builder_where_where, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, conditions, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, bindParams, IS_ARRAY, 1)
	ZEND_ARG_TYPE_INFO(0, bindTypes, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_db_builder_where_andwhere, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, conditions, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, bindParams, IS_ARRAY, 1)
	ZEND_ARG_TYPE_INFO(0, bindTypes, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_db_builder_where_orwhere, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, conditions, IS_STRING, 0)
	ZEND_ARG_INFO(0, bindParams)
	ZEND_ARG_INFO(0, bindTypes)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_db_builder_where_inwhere, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, expr, IS_STRING, 0)
	ZEND_ARG_INFO(0, values)
	ZEND_ARG_INFO(0, useOrWhere)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_db_builder_where_notinwhere, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, expr, IS_STRING, 0)
	ZEND_ARG_INFO(0, values)
	ZEND_ARG_INFO(0, useOrWhere)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_db_builder_where_method_entry[] = {
	PHP_ME(Dao_Db_Builder_Where, setConditions, arginfo_dao_db_builder_where_setconditions, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Builder_Where, getConditions, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Builder_Where, where, arginfo_dao_db_builder_where_where, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Builder_Where, andWhere, arginfo_dao_db_builder_where_andwhere, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Builder_Where, orWhere, arginfo_dao_db_builder_where_orwhere, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Builder_Where, inWhere, arginfo_dao_db_builder_where_inwhere, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Builder_Where, notInWhere, arginfo_dao_db_builder_where_notinwhere, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Db\Builder initializer
 */
DAO_INIT_CLASS(Dao_Db_Builder_Where){

	DAO_REGISTER_CLASS_EX(Dao\\Db\\Builder, Where, db_builder_where, dao_db_builder_ce, dao_db_builder_where_method_entry, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	zend_declare_property_null(dao_db_builder_where_ce, SL("_conditions"), ZEND_ACC_PROTECTED);

	return SUCCESS;
}

/**
 * Return the conditions
 *
 * @return string
 */
PHP_METHOD(Dao_Db_Builder_Where, getConditions){


	RETURN_MEMBER(getThis(), "_conditions");
}

/**
 * Gets the type of PHQL queries
 *
 * @param string|array $conditions
 * @param array $bindParams
 * @param array $bindTypes
 * @param boolean $type
 * @param boolean $grouping
 * @return int
 */
PHP_METHOD(Dao_Db_Builder_Where, setConditions){

	zval *conditions, *bind_params = NULL, *bind_types = NULL, *type = NULL, *grouping = NULL, merge = {};

	dao_fetch_params(0, 1, 4, &conditions, &bind_params, &bind_types, &type, &grouping);

	if (!bind_params) {
		bind_params = &DAO_GLOBAL(z_null);
	}

	if (!bind_types) {
		bind_types = &DAO_GLOBAL(z_null);
	}

	if (!type) {
		type = &DAO_GLOBAL(z_null);
	}

	if (!grouping) {
		grouping = &DAO_GLOBAL(z_null);
	}

	ZVAL_BOOL(&merge, Z_TYPE_P(type) != IS_NULL ? 1 : 0);

	DAO_CALL_METHOD(NULL, getThis(), "setbindparams", bind_params, &merge);
	DAO_CALL_METHOD(NULL, getThis(), "setbindtypes", bind_types, &merge);

	if (Z_TYPE_P(type) != IS_NULL) {
		zval new_conditions = {}, current_conditions = {};
		DAO_CALL_SELF(&current_conditions, "getConditions");

		if (zend_is_true(type)) {
			/**
			 * Nest the condition to current ones or set as unique
			 */
			if (zend_is_true(&current_conditions)) {
				if (zend_is_true(grouping)) {
					DAO_CONCAT_SVSVS(&new_conditions, "(", &current_conditions, ") AND (", conditions, ")");
				} else {
					DAO_CONCAT_VSV(&new_conditions, &current_conditions, " AND ", conditions);
				}
			} else {
				ZVAL_COPY(&new_conditions, conditions);
			}
		} else {
			if (zend_is_true(&current_conditions)) {
				if (zend_is_true(grouping)) {
					DAO_CONCAT_SVSVS(&new_conditions, "(", &current_conditions, ") OR (", conditions, ")");
				} else {
					DAO_CONCAT_VSV(&new_conditions, &current_conditions, " OR ", conditions);
				}
			} else {
				ZVAL_COPY(&new_conditions, conditions);
			}
		}
		zval_ptr_dtor(&current_conditions);

		dao_update_property(getThis(), SL("_conditions"), &new_conditions);
		zval_ptr_dtor(&new_conditions);
	} else {
		dao_update_property(getThis(), SL("_conditions"), conditions);
	}
}

/**
 * Sets the query conditions
 *
 *<code>
 *	$builder->where('name = "Peter"');
 *	$builder->where('name = :name AND id > :id', array('name' => 'Peter', 'id' => 100));
 *</code>
 *
 * @param string $conditions
 * @param array $bindParams
 * @param array $bindTypes
 * @param boolean $grouping
 * @return Dao\Db\Builder
 */
PHP_METHOD(Dao_Db_Builder_Where, where){

	zval *conditions, *bind_params = NULL, *bind_types = NULL;

	dao_fetch_params(0, 1, 2, &conditions, &bind_params, &bind_types);

	if (!bind_params) {
		bind_params = &DAO_GLOBAL(z_null);
	}

	if (!bind_types) {
		bind_types = &DAO_GLOBAL(z_null);
	}

	DAO_CALL_SELF(NULL, "setconditions", conditions, bind_params, bind_types);

	RETURN_THIS();
}

/**
 * Appends a condition to the current conditions using a AND operator
 *
 *<code>
 *	$builder->andWhere('name = "Peter"');
 *	$builder->andWhere('name = :name AND id > :id', array('name' => 'Peter', 'id' => 100));
 *</code>
 *
 * @param string $conditions
 * @param array $bindParams
 * @param array $bindTypes
 * @param boolean $grouping
 * @return Dao\Db\Builder
 */
PHP_METHOD(Dao_Db_Builder_Where, andWhere){

	zval *conditions, *bind_params = NULL, *bind_types = NULL, *grouping = NULL;

	dao_fetch_params(0, 1, 3, &conditions, &bind_params, &bind_types, &grouping);

	if (!bind_params) {
		bind_params = &DAO_GLOBAL(z_null);
	}

	if (!bind_types) {
		bind_types = &DAO_GLOBAL(z_null);
	}

	if (!grouping) {
		grouping = &DAO_GLOBAL(z_true);
	}

	DAO_CALL_SELF(NULL, "setconditions", conditions, bind_params, bind_types, &DAO_GLOBAL(z_true), grouping);

	RETURN_THIS();
}

/**
 * Appends a condition to the current conditions using a OR operator
 *
 * @param string $conditions
 * @param array $bindParams
 * @param array $bindTypes
 * @return Dao\Db\Builder
 */
PHP_METHOD(Dao_Db_Builder_Where, orWhere){

	zval *conditions, *bind_params = NULL, *bind_types = NULL, *grouping = NULL;

	dao_fetch_params(0, 1, 3, &conditions, &bind_params, &bind_types, &grouping);

	if (!bind_params) {
		bind_params = &DAO_GLOBAL(z_null);
	}

	if (!bind_types) {
		bind_types = &DAO_GLOBAL(z_null);
	}

	if (!grouping) {
		grouping = &DAO_GLOBAL(z_true);
	}

	DAO_CALL_SELF(NULL, "setconditions", conditions, bind_params, bind_types, &DAO_GLOBAL(z_false), grouping);

	RETURN_THIS();
}

/**
 * Appends an IN condition to the current conditions
 *
 * @param string $expr
 * @param array $values
 * @param boolean $useOrWhere
 * @return Dao\Db\Builder
 */
PHP_METHOD(Dao_Db_Builder_Where, inWhere){

	zval *expr, *values, *use_orwhere = NULL, hidden_param = {}, bind_params = {}, bind_keys = {}, *value, joined_keys = {}, conditions = {};

	dao_fetch_params(0, 2, 1, &expr, &values, &use_orwhere);

	if (!use_orwhere) {
		use_orwhere = &DAO_GLOBAL(z_false);
	}

	dao_read_property(&hidden_param, getThis(), SL("_hiddenParamNumber"), PH_READONLY);

	array_init(&bind_params);
	array_init(&bind_keys);

	ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(values), value) {
		zval key = {}, query_key = {};
		/**
		 * Key with auto bind-params
		 */
		DAO_CONCAT_SV(&key, "phi", &hidden_param);

		DAO_CONCAT_SV(&query_key, ":", &key);
		dao_array_append(&bind_keys, &query_key, 0);
		dao_array_update(&bind_params, &key, value, PH_COPY);
		zval_ptr_dtor(&key);
		dao_increment(&hidden_param);
	} ZEND_HASH_FOREACH_END();

	dao_fast_join_str(&joined_keys, SL(", "), &bind_keys);
	zval_ptr_dtor(&bind_keys);

	/**
	 * Create a standard IN condition with bind params
	 */
	DAO_CONCAT_VSVS(&conditions, expr, " IN (", &joined_keys, ")");
	zval_ptr_dtor(&joined_keys);

	/**
	 * Append the IN to the current conditions using and 'and'
	 */
	if (zend_is_true(use_orwhere)) {
		DAO_CALL_METHOD(NULL, getThis(), "orwhere", &conditions, &bind_params);
	} else {
		DAO_CALL_METHOD(NULL, getThis(), "andwhere", &conditions, &bind_params);
	}
	zval_ptr_dtor(&conditions);
	zval_ptr_dtor(&bind_params);
	dao_update_property(getThis(), SL("_hiddenParamNumber"), &hidden_param);

	RETURN_THIS();
}

/**
 * Appends a NOT IN condition to the current conditions
 *
 * @param string $expr
 * @param array $values
 * @param boolean $useOrWhere
 * @return Dao\Db\Builder
 */
PHP_METHOD(Dao_Db_Builder_Where, notInWhere){

	zval *expr, *values, *use_orwhere = NULL, hidden_param = {}, bind_params = {}, bind_keys = {}, *value, joined_keys = {}, conditions = {};

	dao_fetch_params(0, 2, 1, &expr, &values, &use_orwhere);

	if (!use_orwhere) {
		use_orwhere = &DAO_GLOBAL(z_false);
	}

	if (Z_TYPE_P(values) != IS_ARRAY) {
		DAO_THROW_EXCEPTION_STR(dao_db_builder_exception_ce, "Values must be an array");
		return;
	}

	dao_read_property(&hidden_param, getThis(), SL("_hiddenParamNumber"), PH_NOISY|PH_READONLY);

	array_init(&bind_params);
	array_init(&bind_keys);

	ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(values), value) {
		zval key = {}, query_key = {};
		/**
		 * Key with auto bind-params
		 */
		DAO_CONCAT_SV(&key, "phi", &hidden_param);

		DAO_CONCAT_SV(&query_key, ":", &key);
		dao_array_append(&bind_keys, &query_key, 0);
		dao_array_update(&bind_params, &key, value, PH_COPY);
		zval_ptr_dtor(&key);
		dao_increment(&hidden_param);
	} ZEND_HASH_FOREACH_END();

	dao_fast_join_str(&joined_keys, SL(", "), &bind_keys);
	zval_ptr_dtor(&bind_keys);

	/**
	 * Create a standard IN condition with bind params
	 */
	DAO_CONCAT_VSVS(&conditions, expr, " NOT IN (", &joined_keys, ")");
	zval_ptr_dtor(&joined_keys);

	/**
	 * Append the IN to the current conditions using and 'and'
	 */
	if (zend_is_true(use_orwhere)) {
		DAO_CALL_METHOD(NULL, getThis(), "orwhere", &conditions, &bind_params);
	} else {
		DAO_CALL_METHOD(NULL, getThis(), "andwhere", &conditions, &bind_params);
	}
	zval_ptr_dtor(&conditions);
	zval_ptr_dtor(&bind_params);
	dao_update_property(getThis(), SL("_hiddenParamNumber"), &hidden_param);

	RETURN_THIS();
}
