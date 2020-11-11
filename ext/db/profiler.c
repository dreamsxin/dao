
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

#include "db/profiler.h"
#include "db/../profiler.h"
#include "db/../profilerinterface.h"
#include "db/profiler/item.h"
#include "db/exception.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"
#include "kernel/object.h"
#include "kernel/array.h"
#include "kernel/operators.h"
#include "kernel/exception.h"

/**
 * Dao\Db\Profiler
 *
 * Instances of Dao\Db can generate execution profiles
 * on SQL statements sent to the relational database. Profiled
 * information includes execution time in miliseconds.
 * This helps you to identify bottlenecks in your applications.
 *
 *<code>
 *
 *	$profiler = new Dao\Db\Profiler();
 *
 *	//Set the connection profiler
 *	$connection->setProfiler($profiler);
 *
 *	$sql = "SELECT buyer_name, quantity, product_name
 *	FROM buyers LEFT JOIN products ON
 *	buyers.pid=products.id";
 *
 *	//Execute a SQL statement
 *	$connection->query($sql);
 *
 *	//Get the last profile in the profiler
 *	$profile = $profiler->getLastProfile();
 *
 *	echo "SQL Statement: ", $profile->getSQLStatement(), "\n";
 *	echo "Start Time: ", $profile->getInitialTime(), "\n";
 *	echo "Final Time: ", $profile->getFinalTime(), "\n";
 *	echo "Total Elapsed Time: ", $profile->getTotalElapsedSeconds(), "\n";
 *
 *</code>
 *
 */
zend_class_entry *dao_db_profiler_ce;

PHP_METHOD(Dao_Db_Profiler, startProfile);
PHP_METHOD(Dao_Db_Profiler, getNumberTotalStatements);

static const zend_function_entry dao_db_profiler_method_entry[] = {
	PHP_ME(Dao_Db_Profiler, startProfile, arginfo_dao_profilerinterface_startprofile, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_Profiler, getNumberTotalStatements, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Db\Profiler initializer
 */
DAO_INIT_CLASS(Dao_Db_Profiler){

	DAO_REGISTER_CLASS_EX(Dao\\Db, Profiler, db_profiler, dao_profiler_ce, dao_db_profiler_method_entry, 0);

	return SUCCESS;
}

/**
 * Starts the profile of a SQL sentence
 *
 * @param string $name
 * @param array $data
 * @param boolean $unique
 * @return Dao\Db\Profiler\Item
 */
PHP_METHOD(Dao_Db_Profiler, startProfile){

	zval *name, *data = NULL, unique = {}, sql_statement = {}, sql_variables = {}, sql_bindtypes = {}, active_profile = {}, time = {};

	dao_fetch_params(0, 1, 1, &name, &data);

	if (!data || Z_TYPE_P(data) != IS_ARRAY) {
		DAO_THROW_EXCEPTION_STR(dao_db_exception_ce, "The data is required");
		return;
	}

	if (!dao_array_isset_fetch_str(&sql_statement, data, SL("sqlStatement"), PH_READONLY)) {
		DAO_THROW_EXCEPTION_STR(dao_db_exception_ce, "The sqlStatement is required");
		return;
	}

	dao_read_property(&unique, getThis(), SL("_unique"), PH_NOISY|PH_READONLY);

	if (zend_is_true(&unique) && dao_isset_property_array(getThis(), SL("_queue"), name)) {
		DAO_THROW_EXCEPTION_STR(dao_db_exception_ce, "The name must be unique");
		return;
	}

	object_init_ex(&active_profile, dao_db_profiler_item_ce);
	DAO_CALL_METHOD(NULL, &active_profile, "__construct", name, data);
	DAO_CALL_METHOD(NULL, &active_profile, "setsqlstatement", &sql_statement);

	if (dao_array_isset_fetch_str(&sql_variables, data, SL("sqlVariables"), PH_READONLY)) {
		DAO_CALL_METHOD(NULL, &active_profile, "setsqlvariables", &sql_variables);
	}

	if (dao_array_isset_fetch_str(&sql_bindtypes, data, SL("sqlBindtypes"), PH_READONLY)) {
		DAO_CALL_METHOD(NULL, &active_profile, "setsqlvariables", &sql_bindtypes);
	}

	DAO_CALL_FUNCTION(&time, "microtime", &DAO_GLOBAL(z_true));
	DAO_CALL_METHOD(NULL, &active_profile, "setinitialtime", &time);

	if (dao_method_exists_ex(getThis(), SL("beforestartprofile")) == SUCCESS) {
		DAO_CALL_METHOD(NULL, getThis(), "beforestartprofile", &active_profile);
	}

	dao_update_property(getThis(), SL("_activeProfile"), &active_profile);
	dao_update_property(getThis(), SL("_currentProfile"), &active_profile);
	if (zend_is_true(&unique)) {
		dao_update_property_array(getThis(), SL("_queue"), name, &active_profile);
	} else {
		dao_update_property_array_append(getThis(), SL("_queue"), &active_profile);
	}
	zval_ptr_dtor(&active_profile);

	RETURN_THIS();
}

/**
 * Returns the total number of SQL statements processed
 *
 * @return integer
 */
PHP_METHOD(Dao_Db_Profiler, getNumberTotalStatements){

	zval all_profiles = {};
	dao_read_property(&all_profiles, getThis(), SL("_allProfiles"), PH_NOISY|PH_READONLY);
	dao_fast_count(return_value, &all_profiles);
}
