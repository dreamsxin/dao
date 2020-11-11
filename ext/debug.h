
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

#ifndef DAO_DEBUG_H
#define DAO_DEBUG_H

#include "php_dao.h"

extern zend_class_entry *dao_debug_ce;

DAO_INIT_CLASS(Dao_Debug);

#define DAO_DEBUG_LOG(message) DAO_CALL_CE_STATIC(NULL, dao_debug_ce, "log", message);
#define DAO_DEBUG_TYPE_LOG(message, type) DAO_CALL_CE_STATIC(NULL, dao_debug_ce, "log", message, type);

#endif /* DAO_DEBUG_H */
