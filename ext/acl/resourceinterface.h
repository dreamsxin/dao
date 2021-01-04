
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

#ifndef DAO_ACL_RESOURCEINTERFACE_H
#define DAO_ACL_RESOURCEINTERFACE_H

#include "php_dao.h"

extern zend_class_entry *dao_acl_resourceinterface_ce;

DAO_INIT_CLASS(Dao_Acl_ResourceInterface);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_acl_resourceinterface_getname, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_acl_resourceinterface_getdescription, 0, 0, 0)
ZEND_END_ARG_INFO()

#endif /* DAO_ACL_RESOURCEINTERFACE_H */
