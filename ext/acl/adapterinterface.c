
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

#include "acl/adapterinterface.h"
#include "kernel/main.h"

zend_class_entry *dao_acl_adapterinterface_ce;

static const zend_function_entry dao_acl_adapterinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Acl_AdapterInterface, setDefaultAction, arginfo_dao_acl_adapterinterface_setdefaultaction)
	PHP_ABSTRACT_ME(Dao_Acl_AdapterInterface, getDefaultAction, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Acl_AdapterInterface, addRole, arginfo_dao_acl_adapterinterface_addrole)
	PHP_ABSTRACT_ME(Dao_Acl_AdapterInterface, addInherit, arginfo_dao_acl_adapterinterface_addinherit)
	PHP_ABSTRACT_ME(Dao_Acl_AdapterInterface, isRole, arginfo_dao_acl_adapterinterface_isrole)
	PHP_ABSTRACT_ME(Dao_Acl_AdapterInterface, isResource, arginfo_dao_acl_adapterinterface_isresource)
	PHP_ABSTRACT_ME(Dao_Acl_AdapterInterface, addResource, arginfo_dao_acl_adapterinterface_addresource)
	PHP_ABSTRACT_ME(Dao_Acl_AdapterInterface, addResourceAccess, arginfo_dao_acl_adapterinterface_addresourceaccess)
	PHP_ABSTRACT_ME(Dao_Acl_AdapterInterface, dropResourceAccess, arginfo_dao_acl_adapterinterface_dropresourceaccess)
	PHP_ABSTRACT_ME(Dao_Acl_AdapterInterface, allow, arginfo_dao_acl_adapterinterface_allow)
	PHP_ABSTRACT_ME(Dao_Acl_AdapterInterface, deny, arginfo_dao_acl_adapterinterface_deny)
	PHP_ABSTRACT_ME(Dao_Acl_AdapterInterface, isAllowed, arginfo_dao_acl_adapterinterface_isallowed)
	PHP_ABSTRACT_ME(Dao_Acl_AdapterInterface, getActiveRole, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Acl_AdapterInterface, getActiveResource, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Acl_AdapterInterface, getActiveAccess, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Acl_AdapterInterface, getRoles, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Acl_AdapterInterface, getResources, arginfo_empty)
	PHP_FE_END
};

/**
 * Dao\Acl\AdapterInterface initializer
 */
DAO_INIT_CLASS(Dao_Acl_AdapterInterface){

	DAO_REGISTER_INTERFACE(Dao\\Acl, AdapterInterface, acl_adapterinterface, dao_acl_adapterinterface_method_entry);

	return SUCCESS;
}

/**
 * Sets the default access level (Dao\Acl::ALLOW or Dao\Acl::DENY)
 *
 * @param int $defaultAccess
 */
DAO_DOC_METHOD(Dao_Acl_AdapterInterface, setDefaultAction);

/**
 * Returns the default ACL access level
 *
 * @return int
 */
DAO_DOC_METHOD(Dao_Acl_AdapterInterface, getDefaultAction);

/**
 * Adds a role to the ACL list. Second parameter lets to inherit access data from other existing role
 *
 * @param  Dao\Acl\RoleInterface $role
 * @param  string $accessInherits
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Acl_AdapterInterface, addRole);

/**
 * Do a role inherit from another existing role
 *
 * @param string $roleName
 * @param string $roleToInherit
 */
DAO_DOC_METHOD(Dao_Acl_AdapterInterface, addInherit);

/**
 * Check whether role exist in the roles list
 *
 * @param  string $roleName
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Acl_AdapterInterface, isRole);

/**
 * Check whether resource exist in the resources list
 *
 * @param  string $resourceName
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Acl_AdapterInterface, isResource);

/**
 * Adds a resource to the ACL list
 *
 * Access names can be a particular action, by example
 * search, update, delete, etc or a list of them
 *
 * @param   Dao\Acl\ResourceInterface $resource
 * @param   array $accessList
 * @return  boolean
 */
DAO_DOC_METHOD(Dao_Acl_AdapterInterface, addResource);

/**
 * Adds access to resources
 *
 * @param string $resourceName
 * @param mixed $accessList
 */
DAO_DOC_METHOD(Dao_Acl_AdapterInterface, addResourceAccess);

/**
 * Removes an access from a resource
 *
 * @param string $resourceName
 * @param mixed $accessList
 */
DAO_DOC_METHOD(Dao_Acl_AdapterInterface, dropResourceAccess);

/**
 * Allow access to a role on a resource
 *
 * @param string $roleName
 * @param string $resourceName
 * @param mixed $access
 */
DAO_DOC_METHOD(Dao_Acl_AdapterInterface, allow);

/**
 * Deny access to a role on a resource
 *
 * @param string $roleName
 * @param string $resourceName
 * @param mixed $access
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Acl_AdapterInterface, deny);

/**
 * Check whether a role is allowed to access an action from a resource
 *
 * @param  string $role
 * @param  string $resource
 * @param  string $access
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Acl_AdapterInterface, isAllowed);

/**
 * Returns the role which the list is checking if it's allowed to certain resource/access
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Acl_AdapterInterface, getActiveRole);

/**
 * Returns the resource which the list is checking if some role can access it
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Acl_AdapterInterface, getActiveResource);

/**
 * Returns the access which the list is checking if some role can access it
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Acl_AdapterInterface, getActiveAccess);

/**
 * Return an array with every role registered in the list
 *
 * @return Dao\Acl\RoleInterface[]
 */
DAO_DOC_METHOD(Dao_Acl_AdapterInterface, getRoles);

/**
 * Return an array with every resource registered in the list
 *
 * @return Dao\Acl\ResourceInterface[]
 */
DAO_DOC_METHOD(Dao_Acl_AdapterInterface, getResources);
