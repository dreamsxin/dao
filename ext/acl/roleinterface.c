
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

#include "acl/roleinterface.h"
#include "kernel/main.h"

zend_class_entry *dao_acl_roleinterface_ce;

static const zend_function_entry dao_acl_roleinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Acl_RoleInterface, getName, arginfo_dao_acl_roleinterface_getname)
	PHP_ABSTRACT_ME(Dao_Acl_RoleInterface, getDescription, arginfo_dao_acl_roleinterface_getdescription)
	PHP_FE_END
};

/**
 * Dao\Acl\RoleInterface initializer
 */
DAO_INIT_CLASS(Dao_Acl_RoleInterface){

	DAO_REGISTER_INTERFACE(Dao\\Acl, RoleInterface, acl_roleinterface, dao_acl_roleinterface_method_entry);

	return SUCCESS;
}

/**
 * Returns the role name
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Acl_RoleInterface, getName);

/**
 * Returns role description
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Acl_RoleInterface, getDescription);
