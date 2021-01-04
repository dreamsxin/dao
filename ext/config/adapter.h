
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
  |          Vladimir Kolesnikov <vladimir@free-sevastopol.com>            |
  +------------------------------------------------------------------------+
*/

#ifndef DAO_CONFIG_ADAPTER_H
#define DAO_CONFIG_ADAPTER_H

#include "php_dao.h"

extern zend_class_entry *dao_config_adapter_ce;

DAO_INIT_CLASS(Dao_Config_Adapter);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_config_adapter___construct, 0, 0, 0)
	ZEND_ARG_INFO(0, filePath)
	ZEND_ARG_INFO(0, absolutePath)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_config_adapter_factory, 0, 0, 0)
	ZEND_ARG_INFO(0, filePath)
	ZEND_ARG_INFO(0, absolutePath)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_config_adapter_read, 0, 0, 1)
	ZEND_ARG_INFO(0, filePath)
	ZEND_ARG_INFO(0, absolutePath)
ZEND_END_ARG_INFO()

#endif /* DAO_CONFIG_ADAPTER_H */
