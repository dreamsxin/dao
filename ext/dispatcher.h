
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
  |          Rack Lin <racklin@gmail.com>                                  |
  |          ZhuZongXin <dreamsxin@qq.com>                                 |
  +------------------------------------------------------------------------+
*/

#ifndef DAO_DISPATCHER_H
#define DAO_DISPATCHER_H

#include "php_dao.h"

#define DAO_EXCEPTION_NO_DI				1
#define DAO_EXCEPTION_CYCLIC_ROUTING	2
#define DAO_EXCEPTION_HANDLER_NOT_FOUND	4
#define DAO_EXCEPTION_INVALID_HANDLER	8
#define DAO_EXCEPTION_INVALID_PARAMS	16
#define DAO_EXCEPTION_ACTION_NOT_FOUND	32

extern zend_class_entry *dao_dispatcher_ce;

DAO_INIT_CLASS(Dao_Dispatcher);

#endif /* DAO_DISPATCHER_H */
