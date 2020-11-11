
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

#include "acl.h"
#include "kernel/main.h"

/**
 * Dao\Acl
 *
 * This component allows to manage ACL lists. An access control list (ACL) is a list
 * of permissions attached to an object. An ACL specifies which users or system processes
 * are granted access to objects, as well as what operations are allowed on given objects.
 *
 *<code>
 *
 *	$acl = new Dao\Acl\Adapter\Memory();
 *
 *	//Default action is deny access
 *	$acl->setDefaultAction(Dao\Acl::DENY);
 *
 *	//Create some roles
 *	$roleAdmins = new Dao\Acl\Role('Administrators', 'Super-User role');
 *	$roleGuests = new Dao\Acl\Role('Guests');
 *
 *	//Add "Guests" role to acl
 *	$acl->addRole($roleGuests);
 *
 *	//Add "Designers" role to acl
 *	$acl->addRole('Designers');
 *
 *	//Define the "Customers" resource
 *	$customersResource = new Dao\Acl\Resource('Customers', 'Customers management');
 *
 *	//Add "customers" resource with a couple of operations
 *	$acl->addResource($customersResource, 'search');
 *	$acl->addResource($customersResource, array('create', 'update'));
 *
 *	//Set access level for roles into resources
 *	$acl->allow('Guests', 'Customers', 'search');
 *	$acl->allow('Guests', 'Customers', 'create');
 *	$acl->deny('Guests', 'Customers', 'update');
 *
 *	//Check whether role has access to the operations
 *	$acl->isAllowed('Guests', 'Customers', 'edit'); //Returns 0
 *	$acl->isAllowed('Guests', 'Customers', 'search'); //Returns 1
 *	$acl->isAllowed('Guests', 'Customers', 'create'); //Returns 1
 *
 *</code>
 */
zend_class_entry *dao_acl_ce;

/**
 * Dao\Acl initializer
 */
DAO_INIT_CLASS(Dao_Acl){

	DAO_REGISTER_CLASS(Dao, Acl, acl, NULL, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	zend_declare_class_constant_long(dao_acl_ce, SL("ALLOW"), DAO_ACL_ALLOW);
	zend_declare_class_constant_long(dao_acl_ce, SL("DENY"), DAO_ACL_DENY);

	return SUCCESS;
}
