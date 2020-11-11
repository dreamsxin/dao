
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

#include "profilerinterface.h"
#include "kernel/main.h"

zend_class_entry *dao_profilerinterface_ce;

static const zend_function_entry dao_profilerinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_ProfilerInterface, startProfile, arginfo_dao_profilerinterface_startprofile)
	PHP_ABSTRACT_ME(Dao_ProfilerInterface, stopProfile, arginfo_dao_profilerinterface_stopprofile)
	PHP_ABSTRACT_ME(Dao_ProfilerInterface, getTotalElapsedSeconds, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_ProfilerInterface, getTotalUsageMemory, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_ProfilerInterface, getProfiles, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_ProfilerInterface, getLastProfile, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_ProfilerInterface, reset, arginfo_empty)
	PHP_FE_END
};

/**
 * Dao\ProfilerInterface initializer
 */
DAO_INIT_CLASS(Dao_ProfilerInterface){

	DAO_REGISTER_INTERFACE(Dao, ProfilerInterface, profilerinterface, dao_profilerinterface_method_entry);

	return SUCCESS;
}

/**
 * Starts the profile
 *
 * @param string $name
 * @param array $data
 * @return Dao\ProfilerInterface
 */
DAO_DOC_METHOD(Dao_ProfilerInterface, startProfile);

/**
 * Stops the active profile
 *
 * @param string $name
 * @return Dao\ProfilerInterface
 */
DAO_DOC_METHOD(Dao_ProfilerInterface, stopProfile);

/**
 * Returns the total time in seconds spent by the profiles
 *
 * @return double
 */
DAO_DOC_METHOD(Dao_ProfilerInterface, getTotalElapsedSeconds);

/**
 * Returns the amount of memory allocated spent by the profiles
 *
 * @return int
 */
DAO_DOC_METHOD(Dao_ProfilerInterface, getTotalUsageMemory);

/**
 * Returns all the processed profiles
 *
 * @return Dao\Profiler\ItemInterface[]
 */
DAO_DOC_METHOD(Dao_ProfilerInterface, getProfiles);

/**
 * Returns the last profile executed in the profiler
 *
 * @return Dao\Profiler\ItemInterface
 */
DAO_DOC_METHOD(Dao_ProfilerInterface, getLastProfile);

/**
 * Resets the profiler, cleaning up all the profiles
 *
 * @return Dao\ProfilerInterface
 */
DAO_DOC_METHOD(Dao_ProfilerInterface, reset);
