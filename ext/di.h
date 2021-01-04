
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
  |          Nikolaos Dimopoulos <nikos@niden.net>                         |
  |          ZhuZongXin <dreamsxin@qq.com>                                 |
  +------------------------------------------------------------------------+
*/

#ifndef DAO_DI_H
#define DAO_DI_H

#include "php_dao.h"

extern zend_class_entry *dao_di_ce;

DAO_INIT_CLASS(Dao_Di);

DAO_STATIC void dao_di_set_service(zval *this_ptr, zval *name, zval *services, int flags);
DAO_STATIC void dao_di_set_services(zval *this_ptr, zval *services);

#endif /* DAO_DI_H */
