
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

#ifndef DAO_PROFILER_ITEMINTERFACE_H
#define DAO_PROFILER_ITEMINTERFACE_H

#include "php_dao.h"

extern zend_class_entry *dao_profiler_iteminterface_ce;

DAO_INIT_CLASS(Dao_Profiler_ItemInterface);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_profiler_iteminterface_setdata, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, data, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_profiler_iteminterface_setinitialtime, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, initialTime, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_profiler_iteminterface_setfinaltime, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, finalTime, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_profiler_iteminterface_setstartmemory, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, startMemory, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_profiler_iteminterface_setendmemory, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, endMemory, IS_LONG, 0)
ZEND_END_ARG_INFO()

#endif /* DAO_PROFILER_ITEMINTERFACE_H */
