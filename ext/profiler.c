
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

#include "profiler.h"
#include "profilerinterface.h"
#include "profiler/item.h"
#include "profiler/exception.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"
#include "kernel/object.h"
#include "kernel/array.h"
#include "kernel/operators.h"
#include "kernel/exception.h"

/**
 * Dao\Profiler
 *
 * Instances of Dao\Db can generate execution profiles
 * on SQL statements sent to the relational database. Profiled
 * information includes execution time in miliseconds.
 * This helps you to identify bottlenecks in your applications.
 *
 *<code>
 *
 *	$profiler = new Dao\Profiler();
 *
 *	//Set the connection profiler
 *	$connection->setProfiler($profiler);
 *
 *	//Get the last profile in the profiler
 *	$profile = $profiler->getLastProfile();
 *
 *	echo "Data: ", $profile->getData(), "\n";
 *	echo "Start Time: ", $profile->getInitialTime(), "\n";
 *	echo "Final Time: ", $profile->getFinalTime(), "\n";
 *	echo "Total Elapsed Time: ", $profile->getTotalElapsedSeconds(), "\n";
 *
 *</code>
 *
 */
zend_class_entry *dao_profiler_ce;

PHP_METHOD(Dao_Profiler, __construct);
PHP_METHOD(Dao_Profiler, startProfile);
PHP_METHOD(Dao_Profiler, stopProfile);
PHP_METHOD(Dao_Profiler, getTotalElapsedSeconds);
PHP_METHOD(Dao_Profiler, getTotalUsageMemory);
PHP_METHOD(Dao_Profiler, getProfiles);
PHP_METHOD(Dao_Profiler, getLastProfile);
PHP_METHOD(Dao_Profiler, getCurrentProfile);
PHP_METHOD(Dao_Profiler, reset);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_profiler___construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, unique, _IS_BOOL, 1)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_profiler_method_entry[] = {
	PHP_ME(Dao_Profiler, __construct, arginfo_dao_profiler___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Profiler, startProfile, arginfo_dao_profilerinterface_startprofile, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Profiler, stopProfile, arginfo_dao_profilerinterface_stopprofile, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Profiler, getTotalElapsedSeconds, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Profiler, getTotalUsageMemory, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Profiler, getProfiles, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Profiler, getLastProfile, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Profiler, getCurrentProfile, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Profiler, reset, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Profiler initializer
 */
DAO_INIT_CLASS(Dao_Profiler){

	DAO_REGISTER_CLASS(Dao, Profiler, profiler, dao_profiler_method_entry, 0);

	zend_declare_property_null(dao_profiler_ce, SL("_unique"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_profiler_ce, SL("_queue"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_profiler_ce, SL("_allProfiles"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_profiler_ce, SL("_currentProfile"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_profiler_ce, SL("_activeProfile"), ZEND_ACC_PROTECTED);
	zend_declare_property_double(dao_profiler_ce, SL("_totalSeconds"), 0, ZEND_ACC_PROTECTED);
	zend_declare_property_long(dao_profiler_ce, SL("_totalMemory"), 0, ZEND_ACC_PROTECTED);

	zend_class_implements(dao_profiler_ce, 1, dao_profilerinterface_ce);

	return SUCCESS;
}

/**
 * Constructor for Dao\Profiler\Item
 *
 * @param array $data
 */
PHP_METHOD(Dao_Profiler, __construct){

	zval *unique = NULL;

	dao_fetch_params(0, 0, 1, &unique);

	if (!unique) {
		unique = &DAO_GLOBAL(z_false);
	}

	dao_update_property(getThis(), SL("_unique"), unique);
}

/**
 * Starts the profile
 *
 * @param string $name
 * @param array $data
 * @param boolean $unique
 * @return Dao\Profiler\ItemInterface
 */
PHP_METHOD(Dao_Profiler, startProfile){

	zval *name, *data = NULL, unique = {}, active_profile = {}, memory = {}, time = {};

	dao_fetch_params(0, 1, 1, &name, &data);

	if (!data) {
		data = &DAO_GLOBAL(z_null);
	}

	dao_read_property(&unique, getThis(), SL("_unique"), PH_NOISY|PH_READONLY);

	if (zend_is_true(&unique) && dao_isset_property_array(getThis(), SL("_queue"), name)) {
		DAO_THROW_EXCEPTION_STR(dao_profiler_exception_ce, "The name must be unique");
		return;
	}

	object_init_ex(&active_profile, dao_profiler_item_ce);
	DAO_CALL_METHOD(NULL, &active_profile, "__construct", name, data);

	DAO_CALL_FUNCTION(&memory, "memory_get_usage");
	DAO_CALL_METHOD(NULL, &active_profile, "setstartmemory", &memory);

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
 * Stops the active profile
 *
 * @return Dao\Profiler
 */
PHP_METHOD(Dao_Profiler, stopProfile){

	zval *name = NULL, active_profile = {}, end_memory = {}, start_memory = {}, difference_memory = {}, total_memory = {}, new_total_memory = {};
	zval final_time = {}, initial_time = {}, difference_time = {}, total_seconds = {}, new_total_seconds = {};

	dao_fetch_params(0, 0, 1, &name);

	if (name && Z_TYPE_P(name) == IS_STRING) {
		if (dao_isset_property_array(getThis(), SL("_queue"), name)) {
			dao_read_property_array(&active_profile, getThis(), SL("_queue"), name, PH_COPY);
			if (Z_TYPE(active_profile) != IS_OBJECT) {
				zval_ptr_dtor(&active_profile);
				RETURN_THIS();
			}
		} else {
			while (1) {
				zval active_name = {};
				zval_ptr_dtor(&active_profile);
				dao_property_array_pop(&active_profile, getThis(), SL("_queue"));
				if (Z_TYPE(active_profile) != IS_OBJECT) {
					zval_ptr_dtor(&active_profile);
					RETURN_THIS();
				}
				DAO_CALL_METHOD(&active_name, &active_profile, "getname");
				if (DAO_IS_EQUAL(name, &active_name)) {
					zval_ptr_dtor(&active_name);
					break;
				}

				dao_update_property_array_append(getThis(), SL("_allProfiles"), &active_profile);
				zval_ptr_dtor(&active_name);
			}
		}
	} else {
		dao_property_array_pop(&active_profile, getThis(), SL("_queue"));
		if (Z_TYPE(active_profile) != IS_OBJECT) {
			zval_ptr_dtor(&active_profile);
			RETURN_THIS();
		}
	}

	// memory
	DAO_CALL_FUNCTION(&end_memory, "memory_get_usage");
	DAO_CALL_METHOD(NULL, &active_profile, "setendmemory", &end_memory);

	DAO_CALL_METHOD(&start_memory, &active_profile, "getstartmemory");

	if (DAO_GT(&end_memory, &start_memory)) {
		dao_sub_function(&difference_memory, &end_memory, &start_memory);

		dao_read_property(&total_memory, getThis(), SL("_totalMemory"), PH_NOISY|PH_READONLY);

		dao_add_function(&new_total_memory, &total_memory, &difference_memory);

		dao_update_property(getThis(), SL("_totalMemory"), &new_total_memory);
	}

	// time
	DAO_CALL_FUNCTION(&final_time, "microtime", &DAO_GLOBAL(z_true));
	DAO_CALL_METHOD(NULL, &active_profile, "setfinaltime", &final_time);

	DAO_CALL_METHOD(&initial_time, &active_profile, "getinitialtime");

	dao_sub_function(&difference_time, &final_time, &initial_time);

	dao_read_property(&total_seconds, getThis(), SL("_totalSeconds"), PH_NOISY|PH_READONLY);

	dao_add_function(&new_total_seconds, &total_seconds, &difference_time);

	dao_update_property(getThis(), SL("_totalSeconds"), &new_total_seconds);

	dao_update_property_array_append(getThis(), SL("_allProfiles"), &active_profile);

	if (dao_method_exists_ex(getThis(), SL("afterendprofile")) == SUCCESS) {
		DAO_CALL_METHOD(NULL, getThis(), "afterendprofile", &active_profile);
	}
	zval_ptr_dtor(&active_profile);

	dao_property_array_last(&active_profile, getThis(), SL("_queue"), PH_READONLY);
	dao_update_property(getThis(), SL("_currentProfile"), &active_profile);

	RETURN_THIS();
}

/**
 * Returns the total time in seconds spent by the profiles
 *
 * @return double
 */
PHP_METHOD(Dao_Profiler, getTotalElapsedSeconds){


	RETURN_MEMBER(getThis(), "_totalSeconds");
}

/**
 * Returns the total time in seconds spent by the profiles
 *
 * @return int
 */
PHP_METHOD(Dao_Profiler, getTotalUsageMemory){


	RETURN_MEMBER(getThis(), "_totalMemory");
}

/**
 * Returns all the processed profiles
 *
 * @return Dao\Profiler\Item[]
 */
PHP_METHOD(Dao_Profiler, getProfiles){


	RETURN_MEMBER(getThis(), "_allProfiles");
}

/**
 * Returns the last profile executed in the profiler
 *
 * @return Dao\Profiler\Item
 */
PHP_METHOD(Dao_Profiler, getLastProfile){


	RETURN_MEMBER(getThis(), "_activeProfile");
}

/**
 * Returns the current profile executed in the profiler
 *
 * @return Dao\Profiler\Item
 */
PHP_METHOD(Dao_Profiler, getCurrentProfile){


	RETURN_MEMBER(getThis(), "_currentProfile");
}

/**
 * Resets the profiler, cleaning up all the profiles
 *
 * @return Dao\Profiler
 */
PHP_METHOD(Dao_Profiler, reset){

	dao_update_property_empty_array(getThis(), SL("_allProfiles"));
	dao_update_property_empty_array(getThis(), SL("_queue"));
	RETURN_THIS();
}
