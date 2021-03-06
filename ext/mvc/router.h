
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

#ifndef DAO_MVC_ROUTER_H
#define DAO_MVC_ROUTER_H

#include "php_dao.h"

#if DAO_TREEROUTER
#include "kernel/r3/r3.h"

typedef struct {
	R3Node *tree;
	zend_object std;
} dao_mvc_router_object;

static inline dao_mvc_router_object *dao_mvc_router_object_from_obj(zend_object *obj) {
	return (dao_mvc_router_object*)((char*)(obj) - XtOffsetOf(dao_mvc_router_object, std));
}
#endif
extern zend_class_entry *dao_mvc_router_ce;

DAO_INIT_CLASS(Dao_Mvc_Router);

#endif /* DAO_MVC_ROUTER_H */
