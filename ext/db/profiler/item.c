
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

#include "db/profiler/item.h"
#include "db/../profiler/item.h"

#include "kernel/main.h"
#include "kernel/object.h"
#include "kernel/operators.h"

/**
 * Dao\Db\Profiler\Item
 *
 * This class identifies each profile in a Dao\Db\Profiler
 *
 */
zend_class_entry *dao_db_profiler_item_ce;

PHP_METHOD(Dao_Db_Profiler_Item, setSQLStatement);
PHP_METHOD(Dao_Db_Profiler_Item, getSQLStatement);
PHP_METHOD(Dao_Db_Profiler_Item, setSQLVariables);
PHP_METHOD(Dao_Db_Profiler_Item, getSQLVariables);
PHP_METHOD(Dao_Db_Profiler_Item, setSQLBindTypes);
PHP_METHOD(Dao_Db_Profiler_Item, getSQLBindTypes);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_db_profiler_item_setsqlstatement, 0, 0, 1)
	ZEND_ARG_INFO(0, sqlStatement)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_db_profiler_item_setsqlvariables, 0, 0, 1)
	ZEND_ARG_INFO(0, sqlVariables)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_db_profiler_item_setsqlbindtypes, 0, 0, 1)
	ZEND_ARG_INFO(0, sqlBindTypes)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_db_profiler_item_method_entry[] = {
	PHP_ME(Dao_Db_Profiler_Item, setSQLStatement, arginfo_dao_db_profiler_item_setsqlstatement, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Profiler_Item, getSQLStatement, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Profiler_Item, setSQLVariables, arginfo_dao_db_profiler_item_setsqlvariables, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Profiler_Item, getSQLVariables, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Profiler_Item, setSQLBindTypes, arginfo_dao_db_profiler_item_setsqlbindtypes, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Profiler_Item, getSQLBindTypes, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Db\Profiler\Item initializer
 */
DAO_INIT_CLASS(Dao_Db_Profiler_Item){

	DAO_REGISTER_CLASS_EX(Dao\\Db\\Profiler, Item, db_profiler_item, dao_profiler_item_ce, dao_db_profiler_item_method_entry, 0);

	zend_declare_property_null(dao_db_profiler_item_ce, SL("_sqlStatement"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_db_profiler_item_ce, SL("_sqlVariables"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_db_profiler_item_ce, SL("_sqlBindTypes"), ZEND_ACC_PROTECTED);

	return SUCCESS;
}

/**
 * Sets the SQL statement related to the profile
 *
 * @param string $sqlStatement
 */
PHP_METHOD(Dao_Db_Profiler_Item, setSQLStatement){

	zval *sql_statement;

	dao_fetch_params(0, 1, 0, &sql_statement);

	dao_update_property(getThis(), SL("_sqlStatement"), sql_statement);

}

/**
 * Returns the SQL statement related to the profile
 *
 * @return string
 */
PHP_METHOD(Dao_Db_Profiler_Item, getSQLStatement){


	RETURN_MEMBER(getThis(), "_sqlStatement");
}

/**
 * Sets the SQL variables related to the profile
 *
 * @param array $sqlParams
 */
PHP_METHOD(Dao_Db_Profiler_Item, setSQLVariables){

	zval *sql_variables;

	dao_fetch_params(0, 1, 0, &sql_variables);

	dao_update_property(getThis(), SL("_sqlVariables"), sql_variables);

}

/**
 * Returns the SQL variables related to the profile
 *
 * @return array
 */
PHP_METHOD(Dao_Db_Profiler_Item, getSQLVariables){


	RETURN_MEMBER(getThis(), "_sqlVariables");
}

/**
 * Sets the SQL bind types related to the profile
 *
 * @param array $sqlParams
 */
PHP_METHOD(Dao_Db_Profiler_Item, setSQLBindTypes){

	zval *sql_bindtypes;

	dao_fetch_params(0, 1, 0, &sql_bindtypes);

	dao_update_property(getThis(), SL("_sqlBindTypes"), sql_bindtypes);

}

/**
 * Returns the SQL bind types related to the profile
 *
 * @return array
 */
PHP_METHOD(Dao_Db_Profiler_Item, getSQLBindTypes){


	RETURN_MEMBER(getThis(), "_sqlBindTypes");
}
