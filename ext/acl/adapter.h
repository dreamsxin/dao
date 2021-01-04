
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
  +------------------------------------------------------------------------+
*/

#ifndef DAO_ACL_ADAPTER_H
#define DAO_ACL_ADAPTER_H

#include "php_dao.h"

extern zend_class_entry *dao_acl_adapter_ce;

DAO_INIT_CLASS(Dao_Acl_Adapter);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_acl_adapter_setdefaultaction, 0, 0, 1)
	ZEND_ARG_INFO(0, defaultAccess)
ZEND_END_ARG_INFO()

#endif /* DAO_ACL_ADAPTER_H */
