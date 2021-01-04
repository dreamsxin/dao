
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

#include "profiler/iteminterface.h"
#include "kernel/main.h"

zend_class_entry *dao_profiler_iteminterface_ce;

static const zend_function_entry dao_profiler_iteminterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_ProfilerInterface, getName, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_ProfilerInterface, setData, arginfo_dao_profiler_iteminterface_setdata)
	PHP_ABSTRACT_ME(Dao_ProfilerInterface, getData, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_ProfilerInterface, setInitialTime, arginfo_dao_profiler_iteminterface_setinitialtime)
	PHP_ABSTRACT_ME(Dao_ProfilerInterface, getInitialTime, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_ProfilerInterface, setFinalTime, arginfo_dao_profiler_iteminterface_setfinaltime)
	PHP_ABSTRACT_ME(Dao_ProfilerInterface, getFinalTime, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_ProfilerInterface, getTotalElapsedSeconds, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_ProfilerInterface, setStartMemory, arginfo_dao_profiler_iteminterface_setstartmemory)
	PHP_ABSTRACT_ME(Dao_ProfilerInterface, getStartMemory, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_ProfilerInterface, setEndMemory, arginfo_dao_profiler_iteminterface_setendmemory)
	PHP_ABSTRACT_ME(Dao_ProfilerInterface, getEndMemory, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_ProfilerInterface, getTotalUsageMemory, arginfo_empty)
	PHP_FE_END
};

/**
 * Dao\Profiler\ItemInterface initializer
 */
DAO_INIT_CLASS(Dao_Profiler_ItemInterface){

	DAO_REGISTER_INTERFACE(Dao\\Profile, ItemInterface, profiler_iteminterface, dao_profiler_iteminterface_method_entry);

	return SUCCESS;
}

/**
 * Returns the name
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Profiler_ItemInterface, getName);

/**
 * Sets the data related to the profile
 *
 * @param array $data
 */
DAO_DOC_METHOD(Dao_Profiler_ItemInterface, setData);

/**
 * Returns the data related to the profile
 *
 * @return array
 */
DAO_DOC_METHOD(Dao_Profiler_ItemInterface, getData);

/**
 * Sets the timestamp on when the profile started
 *
 * @param double $initialTime
 */
DAO_DOC_METHOD(Dao_Profiler_ItemInterface, setInitialTime);

/**
 * Returns the initial time in milseconds on when the profile started
 *
 * @return double
 */
DAO_DOC_METHOD(Dao_Profiler_ItemInterface, getInitialTime);

/**
 * Sets the timestamp on when the profile ended
 *
 * @param double $finalTime
 */
DAO_DOC_METHOD(Dao_Profiler_ItemInterface, setFinalTime);

/**
 * Returns the initial time in milseconds on when the profile ended
 *
 * @return double
 */
DAO_DOC_METHOD(Dao_Profiler_ItemInterface, getFinalTime);

/**
 * Returns the total time in seconds spent by the profile
 *
 * @return double
 */
DAO_DOC_METHOD(Dao_Profiler_ItemInterface, getTotalElapsedSeconds);

/**
 * Sets the amount of memory allocated on when the profile started
 *
 * @param int $startMemory
 */
DAO_DOC_METHOD(Dao_Profiler_ItemInterface, setStartMemory);

/**
 * Returns the amount of memory allocated on when the profile started
 *
 * @return int
 */
DAO_DOC_METHOD(Dao_Profiler_ItemInterface, getStartMemory);

/**
 * Sets the amount of memory allocated on when the profile ended
 *
 * @param int $endMemory
 */
DAO_DOC_METHOD(Dao_Profiler_ItemInterface, setEndMemory);

/**
 * Returns the the amount of memory allocated on when the profile ended
 *
 * @return int
 */
DAO_DOC_METHOD(Dao_Profiler_ItemInterface, getEndMemory);

/**
 * Returns the amount of memory allocated spent by the profile
 *
 * @return int
 */
DAO_DOC_METHOD(Dao_Profiler_ItemInterface, getTotalUsageMemory);
