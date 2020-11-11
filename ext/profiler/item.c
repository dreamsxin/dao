
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

#include "profiler/item.h"
#include "profiler/iteminterface.h"

#include "kernel/main.h"
#include "kernel/object.h"
#include "kernel/operators.h"
#include "kernel/fcall.h"

/**
 * Dao\Profiler\Item
 *
 * This class identifies each profile in a Dao\Profiler
 *
 */
zend_class_entry *dao_profiler_item_ce;

PHP_METHOD(Dao_Profiler_Item, __construct);
PHP_METHOD(Dao_Profiler_Item, getName);
PHP_METHOD(Dao_Profiler_Item, setData);
PHP_METHOD(Dao_Profiler_Item, getData);
PHP_METHOD(Dao_Profiler_Item, setInitialTime);
PHP_METHOD(Dao_Profiler_Item, getInitialTime);
PHP_METHOD(Dao_Profiler_Item, setFinalTime);
PHP_METHOD(Dao_Profiler_Item, getFinalTime);
PHP_METHOD(Dao_Profiler_Item, getTotalElapsedSeconds);
PHP_METHOD(Dao_Profiler_Item, setStartMemory);
PHP_METHOD(Dao_Profiler_Item, getStartMemory);
PHP_METHOD(Dao_Profiler_Item, setEndMemory);
PHP_METHOD(Dao_Profiler_Item, getEndMemory);
PHP_METHOD(Dao_Profiler_Item, getTotalUsageMemory);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_profiler_item___construct, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_profiler_item_method_entry[] = {
	PHP_ME(Dao_Profiler_Item, __construct, arginfo_dao_profiler_item___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Profiler_Item, getName, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Profiler_Item, setData, arginfo_dao_profiler_iteminterface_setdata, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Profiler_Item, getData, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Profiler_Item, setInitialTime, arginfo_dao_profiler_iteminterface_setinitialtime, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Profiler_Item, getInitialTime, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Profiler_Item, setFinalTime, arginfo_dao_profiler_iteminterface_setfinaltime, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Profiler_Item, getFinalTime, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Profiler_Item, getTotalElapsedSeconds, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Profiler_Item, setStartMemory, arginfo_dao_profiler_iteminterface_setstartmemory, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Profiler_Item, getStartMemory, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Profiler_Item, setEndMemory, arginfo_dao_profiler_iteminterface_setendmemory, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Profiler_Item, getEndMemory, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Profiler_Item, getTotalUsageMemory, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Profiler\Item initializer
 */
DAO_INIT_CLASS(Dao_Profiler_Item){

	DAO_REGISTER_CLASS(Dao\\Profiler, Item, profiler_item, dao_profiler_item_method_entry, 0);

	zend_declare_property_null(dao_profiler_item_ce, SL("_name"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_profiler_item_ce, SL("_data"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_profiler_item_ce, SL("_initialTime"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_profiler_item_ce, SL("_finalTime"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_profiler_item_ce, SL("_startMemory"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_profiler_item_ce, SL("_endMemory"), ZEND_ACC_PROTECTED);

	zend_class_implements(dao_profiler_item_ce, 1, dao_profiler_iteminterface_ce);

	return SUCCESS;
}

/**
 * Constructor for Dao\Profiler\Item
 *
 * @param array $data
 */
PHP_METHOD(Dao_Profiler_Item, __construct){

	zval *name, *data;

	dao_fetch_params(0, 2, 0, &name, &data);

	dao_update_property(getThis(), SL("_name"), name);
	dao_update_property(getThis(), SL("_data"), data);
}

/**
 * Returns the name
 *
 * @return string
 */
PHP_METHOD(Dao_Profiler_Item, getName){


	RETURN_MEMBER(getThis(), "_name");
}

/**
 * Sets the data related to the profile
 *
 * @param array $data
 */
PHP_METHOD(Dao_Profiler_Item, setData){

	zval *data;

	dao_fetch_params(0, 1, 0, &data);

	dao_update_property(getThis(), SL("_data"), data);

}

/**
 * Returns the data related to the profile
 *
 * @return array
 */
PHP_METHOD(Dao_Profiler_Item, getData){


	RETURN_MEMBER(getThis(), "_data");
}

/**
 * Sets the timestamp on when the profile started
 *
 * @param double $initialTime
 */
PHP_METHOD(Dao_Profiler_Item, setInitialTime){

	zval *initial_time;

	dao_fetch_params(0, 1, 0, &initial_time);

	dao_update_property(getThis(), SL("_initialTime"), initial_time);

}

/**
 * Returns the initial time in milseconds on when the profile started
 *
 * @return double
 */
PHP_METHOD(Dao_Profiler_Item, getInitialTime){


	RETURN_MEMBER(getThis(), "_initialTime");
}

/**
 * Sets the timestamp on when the profile ended
 *
 * @param double $finalTime
 */
PHP_METHOD(Dao_Profiler_Item, setFinalTime){

	zval *final_time;

	dao_fetch_params(0, 1, 0, &final_time);

	dao_update_property(getThis(), SL("_finalTime"), final_time);

}

/**
 * Returns the initial time in milseconds on when the profile ended
 *
 * @return double
 */
PHP_METHOD(Dao_Profiler_Item, getFinalTime){


	RETURN_MEMBER(getThis(), "_finalTime");
}

/**
 * Returns the total time in seconds spent by the profile
 *
 * @return double|null
 */
PHP_METHOD(Dao_Profiler_Item, getTotalElapsedSeconds){

	zval final_time = {}, initial_time = {};

	dao_read_property(&final_time, getThis(), SL("_finalTime"), PH_NOISY|PH_READONLY);
	dao_read_property(&initial_time, getThis(), SL("_initialTime"), PH_NOISY|PH_READONLY);
	if (Z_TYPE(final_time) == IS_NULL || Z_TYPE(initial_time) == IS_NULL) {
		RETURN_DOUBLE(0);
	}
	dao_sub_function(return_value, &final_time, &initial_time);
}

/**
 * Sets the amount of memory allocated on when the profile started
 *
 * @param double $startMemory
 */
PHP_METHOD(Dao_Profiler_Item, setStartMemory){

	zval *usage;

	dao_fetch_params(0, 1, 0, &usage);

	dao_update_property(getThis(), SL("_startMemory"), usage);

}

/**
 * Returns the amount of memory allocated on when the profile started
 *
 * @return double
 */
PHP_METHOD(Dao_Profiler_Item, getStartMemory){


	RETURN_MEMBER(getThis(), "_startMemory");
}

/**
 * Sets the amount of memory allocated on when the profile ended
 *
 * @param double $endMemory
 */
PHP_METHOD(Dao_Profiler_Item, setEndMemory){

	zval *usage;

	dao_fetch_params(0, 1, 0, &usage);

	dao_update_property(getThis(), SL("_endMemory"), usage);

}

/**
 * Returns the amount of memory allocated on when the profile ended
 *
 * @return int
 */
PHP_METHOD(Dao_Profiler_Item, getEndMemory){


	RETURN_MEMBER(getThis(), "_endMemory");
}

/**
 * Returns the total time in seconds spent by the profile
 *
 * @return int
 */
PHP_METHOD(Dao_Profiler_Item, getTotalUsageMemory){

	zval start_memory = {}, end_memory = {};

	dao_read_property(&start_memory, getThis(), SL("_startMemory"), PH_NOISY|PH_READONLY);
	dao_read_property(&end_memory, getThis(), SL("_endMemory"), PH_NOISY|PH_READONLY);
	if (Z_TYPE(start_memory) == IS_NULL || Z_TYPE(end_memory) == IS_NULL) {
		RETURN_LONG(0);
	}

	if (DAO_GT(&end_memory, &start_memory)) {
		dao_sub_function(return_value, &end_memory, &start_memory);
	} else {
		RETURN_LONG(0);
	}
}
