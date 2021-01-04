
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

#include "acl/resourceinterface.h"
#include "kernel/main.h"

zend_class_entry *dao_acl_resourceinterface_ce;

static const zend_function_entry dao_acl_resourceinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Acl_ResourceInterface, getName, arginfo_dao_acl_resourceinterface_getname)
	PHP_ABSTRACT_ME(Dao_Acl_ResourceInterface, getDescription, arginfo_dao_acl_resourceinterface_getdescription)
	PHP_FE_END
};

/**
 * Dao\Acl\ResourceInterface initializer
 */
DAO_INIT_CLASS(Dao_Acl_ResourceInterface){

	DAO_REGISTER_INTERFACE(Dao\\Acl, ResourceInterface, acl_resourceinterface, dao_acl_resourceinterface_method_entry);

	return SUCCESS;
}

/**
 * Returns the resource name
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Acl_ResourceInterface, getName);

/**
 * Returns resource description
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Acl_ResourceInterface, getDescription);
