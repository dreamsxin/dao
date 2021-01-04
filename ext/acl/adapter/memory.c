
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

#include "acl/adapter/memory.h"
#include "acl.h"
#include "acl/adapter.h"
#include "acl/adapterinterface.h"
#include "acl/exception.h"
#include "acl/resource.h"
#include "acl/resourceaware.h"
#include "acl/role.h"
#include "acl/roleaware.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/concat.h"
#include "kernel/exception.h"
#include "kernel/operators.h"
#include "kernel/hash.h"
#include "kernel/string.h"

/**
 * Dao\Acl\Adapter\Memory
 *
 * Manages ACL lists in memory
 *
 *<code>
 *
 *	$acl = new Dao\Acl\Adapter\Memory();
 *
 *	$acl->setDefaultAction(Dao\Acl::DENY);
 *
 *	//Register roles
 *	$roles = array(
 *		'users' => new Dao\Acl\Role('Users'),
 *		'guests' => new Dao\Acl\Role('Guests')
 *	);
 *	foreach ($roles as $role) {
 *		$acl->addRole($role);
 *	}
 *
 *	//Private area resources
 *  $privateResources = array(
 *		'companies' => array('index', 'search', 'new', 'edit', 'save', 'create', 'delete'),
 *		'products' => array('index', 'search', 'new', 'edit', 'save', 'create', 'delete'),
 *		'invoices' => array('index', 'profile')
 *	);
 *	foreach ($privateResources as $resource => $actions) {
 *		$acl->addResource(new Dao\Acl\Resource($resource), $actions);
 *	}
 *
 *	//Public area resources
 *	$publicResources = array(
 *		'index' => array('index'),
 *		'about' => array('index'),
 *		'session' => array('index', 'register', 'start', 'end'),
 *		'contact' => array('index', 'send')
 *	);
 *  foreach ($publicResources as $resource => $actions) {
 *		$acl->addResource(new Dao\Acl\Resource($resource), $actions);
 *	}
 *
 *  //Grant access to public areas to both users and guests
 *	foreach ($roles as $role){
 *		foreach ($publicResources as $resource => $actions) {
 *			$acl->allow($role->getName(), $resource, '*');
 *		}
 *	}
 *
 *	//Grant access to private area to role Users
 *  foreach ($privateResources as $resource => $actions) {
 * 		foreach ($actions as $action) {
 *			$acl->allow('Users', $resource, $action);
 *		}
 *	}
 *
 *</code>
 */
zend_class_entry *dao_acl_adapter_memory_ce;

PHP_METHOD(Dao_Acl_Adapter_Memory, __construct);
PHP_METHOD(Dao_Acl_Adapter_Memory, addRole);
PHP_METHOD(Dao_Acl_Adapter_Memory, addInherit);
PHP_METHOD(Dao_Acl_Adapter_Memory, isRole);
PHP_METHOD(Dao_Acl_Adapter_Memory, isResource);
PHP_METHOD(Dao_Acl_Adapter_Memory, addResource);
PHP_METHOD(Dao_Acl_Adapter_Memory, addResourceAccess);
PHP_METHOD(Dao_Acl_Adapter_Memory, dropResourceAccess);
PHP_METHOD(Dao_Acl_Adapter_Memory, _allowOrDeny);
PHP_METHOD(Dao_Acl_Adapter_Memory, allow);
PHP_METHOD(Dao_Acl_Adapter_Memory, deny);
PHP_METHOD(Dao_Acl_Adapter_Memory, isAllowed);
PHP_METHOD(Dao_Acl_Adapter_Memory, getRoles);
PHP_METHOD(Dao_Acl_Adapter_Memory, getResources);
PHP_METHOD(Dao_Acl_Adapter_Memory, getAccess);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_acl_adapterinterface__allowordeny, 0, 0, 4)
	ZEND_ARG_INFO(0, roleName)
	ZEND_ARG_INFO(0, resourceName)
	ZEND_ARG_INFO(0, access)
	ZEND_ARG_TYPE_INFO(0, action, IS_LONG, 0)
	ZEND_ARG_CALLABLE_INFO(0, callback, 1)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_acl_adapter_memory_method_entry[] = {
	PHP_ME(Dao_Acl_Adapter_Memory, __construct, arginfo_empty, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Acl_Adapter_Memory, addRole, arginfo_dao_acl_adapterinterface_addrole, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Acl_Adapter_Memory, addInherit, arginfo_dao_acl_adapterinterface_addinherit, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Acl_Adapter_Memory, isRole, arginfo_dao_acl_adapterinterface_isrole, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Acl_Adapter_Memory, isResource, arginfo_dao_acl_adapterinterface_isresource, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Acl_Adapter_Memory, addResource, arginfo_dao_acl_adapterinterface_addresource, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Acl_Adapter_Memory, addResourceAccess, arginfo_dao_acl_adapterinterface_addresourceaccess, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Acl_Adapter_Memory, dropResourceAccess, arginfo_dao_acl_adapterinterface_dropresourceaccess, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Acl_Adapter_Memory, _allowOrDeny, arginfo_dao_acl_adapterinterface__allowordeny, ZEND_ACC_PROTECTED)
	PHP_ME(Dao_Acl_Adapter_Memory, allow, arginfo_dao_acl_adapterinterface_allow, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Acl_Adapter_Memory, deny, arginfo_dao_acl_adapterinterface_deny, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Acl_Adapter_Memory, isAllowed, arginfo_dao_acl_adapterinterface_isallowed, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Acl_Adapter_Memory, getRoles, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Acl_Adapter_Memory, getResources, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Acl_Adapter_Memory, getAccess, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Acl\Adapter\Memory initializer
 */
DAO_INIT_CLASS(Dao_Acl_Adapter_Memory){

	DAO_REGISTER_CLASS_EX(Dao\\Acl\\Adapter, Memory, acl_adapter_memory, dao_acl_adapter_ce, dao_acl_adapter_memory_method_entry, 0);

	zend_declare_property_null(dao_acl_adapter_memory_ce, SL("_rolesNames"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_acl_adapter_memory_ce, SL("_roles"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_acl_adapter_memory_ce, SL("_resourcesNames"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_acl_adapter_memory_ce, SL("_resources"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_acl_adapter_memory_ce, SL("_access"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_acl_adapter_memory_ce, SL("_funcs"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_acl_adapter_memory_ce, SL("_roleInherits"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_acl_adapter_memory_ce, SL("_accessList"), ZEND_ACC_PROTECTED);

	zend_class_implements(dao_acl_adapter_memory_ce, 1, dao_acl_adapterinterface_ce);

	return SUCCESS;
}

/**
 * Dao\Acl\Adapter\Memory constructor
 *
 */
PHP_METHOD(Dao_Acl_Adapter_Memory, __construct){

	zval resources_names = {}, access_list = {};

	array_init_size(&resources_names, 1);
	dao_array_update_str(&resources_names, SL("*"), &DAO_GLOBAL(z_true), PH_COPY);
	dao_update_property(getThis(), SL("_resourcesNames"), &resources_names);
	zval_ptr_dtor(&resources_names);

	array_init_size(&access_list, 1);
	dao_array_update_str(&access_list, SL("*!*"), &DAO_GLOBAL(z_true), PH_COPY);
	dao_update_property(getThis(), SL("_accessList"), &access_list);
	zval_ptr_dtor(&access_list);

	dao_update_property_empty_array(getThis(), SL("_access"));
}

/**
 * Adds a role to the ACL list. Second parameter allows inheriting access data from other existing role
 *
 * Example:
 * <code>
 * 	$acl->addRole(new Dao\Acl\Role('administrator'), 'consultant');
 * 	$acl->addRole('administrator', 'consultant');
 * </code>
 *
 * @param  Dao\Acl\RoleInterface|string $role
 * @param  array|string $accessInherits
 * @return boolean
 */
PHP_METHOD(Dao_Acl_Adapter_Memory, addRole){

	zval *role, *access_inherits = NULL, role_name = {}, object = {}, roles_names = {};

	dao_fetch_params(0, 1, 1, &role, &access_inherits);

	if (!access_inherits) {
		access_inherits = &DAO_GLOBAL(z_null);
	}

	if (Z_TYPE_P(role) == IS_OBJECT) {
		DAO_CALL_METHOD(&role_name, role, "getname");
		ZVAL_COPY(&object, role);
	} else {
		ZVAL_COPY(&role_name, role);

		object_init_ex(&object, dao_acl_role_ce);
		DAO_CALL_METHOD(NULL, &object, "__construct", &role_name);
	}

	dao_read_property(&roles_names, getThis(), SL("_rolesNames"), PH_READONLY);
	if (dao_array_isset(&roles_names, &role_name)) {
		zval_ptr_dtor(&role_name);
		zval_ptr_dtor(&object);
		RETURN_FALSE;
	}

	dao_update_property_array_append(getThis(), SL("_roles"), &object);
	zval_ptr_dtor(&object);
	dao_update_property_array(getThis(), SL("_rolesNames"), &role_name, &DAO_GLOBAL(z_true));

	if (Z_TYPE_P(access_inherits) != IS_NULL) {
		DAO_RETURN_CALL_METHOD(getThis(), "addinherit", &role_name, access_inherits);
	} else {
		RETVAL_TRUE;
	}
	zval_ptr_dtor(&role_name);
}

/**
 * Do a role inherit from another existing role
 *
 * @param string $roleName
 * @param string $roleToInherit
 */
PHP_METHOD(Dao_Acl_Adapter_Memory, addInherit){

	zval *role_name, *role_to_inherit, roles_names = {}, exception_message = {}, role_inherit_name = {}, roles_inherits = {};

	dao_fetch_params(0, 2, 0, &role_name, &role_to_inherit);

	dao_read_property(&roles_names, getThis(), SL("_rolesNames"), PH_NOISY|PH_READONLY);
	if (!dao_array_isset(&roles_names, role_name)) {
		DAO_CONCAT_SVS(&exception_message, "Role '", role_name, "' does not exist in the role list");
		DAO_THROW_EXCEPTION_ZVAL(dao_acl_exception_ce, &exception_message);
		return;
	}

	if (Z_TYPE_P(role_to_inherit) == IS_OBJECT) {
		DAO_CALL_METHOD(&role_inherit_name, role_to_inherit, "getname");
	} else {
		ZVAL_COPY_VALUE(&role_inherit_name, role_to_inherit);
	}

	/**
	 * Check if the role to inherit is valid
	 */
	if (!dao_array_isset(&roles_names, &role_inherit_name)) {
		DAO_CONCAT_SVS(&exception_message, "Role '", &role_inherit_name, "' (to inherit) does not exist in the role list");
		DAO_THROW_EXCEPTION_ZVAL(dao_acl_exception_ce, &exception_message);
		return;
	}

	if (DAO_IS_EQUAL(&role_inherit_name, role_name)) {
		RETURN_FALSE;
	}

	dao_read_property(&roles_inherits, getThis(), SL("_roleInherits"), PH_NOISY|PH_READONLY);
	dao_array_append_multi_2(&roles_inherits, role_name, &role_inherit_name, PH_COPY);
	dao_update_property(getThis(), SL("_roleInherits"), &roles_inherits);
	RETURN_TRUE;
}

/**
 * Check whether role exist in the roles list
 *
 * @param  string $roleName
 * @return boolean
 */
PHP_METHOD(Dao_Acl_Adapter_Memory, isRole){

	zval *role_name, roles_names = {};

	dao_fetch_params(0, 1, 0, &role_name);

	dao_read_property(&roles_names, getThis(), SL("_rolesNames"), PH_NOISY|PH_READONLY);
	if (dao_array_isset(&roles_names, role_name)) {
		RETURN_TRUE;
	}

	RETURN_FALSE;
}

/**
 * Check whether resource exist in the resources list
 *
 * @param  string $resourceName
 * @return boolean
 */
PHP_METHOD(Dao_Acl_Adapter_Memory, isResource){

	zval *resource_name, resources_names = {};

	dao_fetch_params(0, 1, 0, &resource_name);

	dao_read_property(&resources_names, getThis(), SL("_resourcesNames"), PH_NOISY|PH_READONLY);
	if (dao_array_isset(&resources_names, resource_name)) {
		RETURN_TRUE;
	}

	RETURN_FALSE;
}

/**
 * Adds a resource to the ACL list
 *
 * Access names can be a particular action, by example
 * search, update, delete, etc or a list of them
 *
 * Example:
 * <code>
 * //Add a resource to the the list allowing access to an action
 * $acl->addResource(new Dao\Acl\Resource('customers'), 'search');
 * $acl->addResource('customers', 'search');
 *
 * //Add a resource  with an access list
 * $acl->addResource(new Dao\Acl\Resource('customers'), array('create', 'search'));
 * $acl->addResource('customers', array('create', 'search'));
 * </code>
 *
 * @param Dao\Acl\Resource|string $resource
 * @param array $accessList
 * @return boolean
 */
PHP_METHOD(Dao_Acl_Adapter_Memory, addResource){

	zval *resource, *access_list = NULL, resource_name = {}, object = {}, resources_names = {};

	dao_fetch_params(0, 1, 1, &resource, &access_list);

	if (!access_list) {
		access_list = &DAO_GLOBAL(z_null);
	}

	if (Z_TYPE_P(resource) == IS_OBJECT) {
		DAO_CALL_METHOD(&resource_name, resource, "getname");
		ZVAL_COPY(&object, resource);
	} else {
		ZVAL_COPY(&resource_name, resource);

		object_init_ex(&object, dao_acl_resource_ce);
		DAO_CALL_METHOD(NULL, &object, "__construct", &resource_name);
	}

	dao_read_property(&resources_names, getThis(), SL("_resourcesNames"), PH_NOISY|PH_READONLY);
	if (!dao_array_isset(&resources_names, &resource_name)) {
		dao_update_property_array_append(getThis(), SL("_resources"), &object);
		dao_update_property_array(getThis(), SL("_resourcesNames"), &resource_name, &DAO_GLOBAL(z_true));
	}
	zval_ptr_dtor(&object);

	DAO_RETURN_CALL_METHOD(getThis(), "addresourceaccess", &resource_name, access_list);
	zval_ptr_dtor(&resource_name);
}

/**
 * Adds access to resources
 *
 * @param string $resourceName
 * @param mixed $accessList
 */
PHP_METHOD(Dao_Acl_Adapter_Memory, addResourceAccess){

	zval *resource_name, *access_list, resources_names = {}, exception_message = {}, internal_access_list = {}, *access_name, access_key = {};

	dao_fetch_params(0, 2, 0, &resource_name, &access_list);

	dao_read_property(&resources_names, getThis(), SL("_resourcesNames"), PH_NOISY|PH_READONLY);
	if (!dao_array_isset(&resources_names, resource_name)) {
		DAO_CONCAT_SVS(&exception_message, "Resource '", resource_name, "' does not exist in ACL");
		DAO_THROW_EXCEPTION_ZVAL(dao_acl_exception_ce, &exception_message);
		return;
	}

	dao_read_property(&internal_access_list, getThis(), SL("_accessList"), PH_NOISY|PH_READONLY);
	if (Z_TYPE_P(access_list) == IS_ARRAY) {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(access_list), access_name) {
			DAO_CONCAT_VSV(&access_key, resource_name, "!", access_name);
			if (!dao_array_isset(&internal_access_list, &access_key)) {
				dao_update_property_array(getThis(), SL("_accessList"), &access_key, &DAO_GLOBAL(z_true));
			}
			zval_ptr_dtor(&access_key);
		} ZEND_HASH_FOREACH_END();
	} else {
		if (Z_TYPE_P(access_list) == IS_STRING) {
			DAO_CONCAT_VSV(&access_key, resource_name, "!", access_list);
			if (!dao_array_isset(&internal_access_list, &access_key)) {
				dao_update_property_array(getThis(), SL("_accessList"), &access_key, &DAO_GLOBAL(z_true));
			}
			zval_ptr_dtor(&access_key);
		}
	}

	RETURN_TRUE;
}

/**
 * Removes an access from a resource
 *
 * @param string $resourceName
 * @param mixed $accessList
 */
PHP_METHOD(Dao_Acl_Adapter_Memory, dropResourceAccess){

	zval *resource_name, *access_list, *access_name = NULL, access_key = {};

	dao_fetch_params(0, 2, 0, &resource_name, &access_list);

	if (Z_TYPE_P(access_list) == IS_ARRAY) {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(access_list), access_name) {
			DAO_CONCAT_VSV(&access_key, resource_name, "!", access_name);
			dao_unset_property_array(getThis(), SL("_accessList"), &access_key);
		} ZEND_HASH_FOREACH_END();
	} else {
		DAO_CONCAT_VSV(&access_key, resource_name, "!", access_list);
		dao_unset_property_array(getThis(), SL("_accessList"), &access_key);
	}
}

/**
 * Checks if a role has access to a resource
 *
 * @param string $roleName
 * @param string $resourceName
 * @param string $access
 * @param string $action
 * @param callable $callback
 */
PHP_METHOD(Dao_Acl_Adapter_Memory, _allowOrDeny){

	zval *role_name, *resource_name, *access, *action, *callback = NULL, roles_names = {}, exception_message = {}, resources_names = {};
	zval default_access = {}, access_list = {}, internal_access = {}, *access_name, access_key = {};
	zend_string *str_key;

	dao_fetch_params(0, 4, 1, &role_name, &resource_name, &access, &action, &callback);

	if (!callback) {
		callback = &DAO_GLOBAL(z_null);
	}

	dao_read_property(&roles_names, getThis(), SL("_rolesNames"), PH_NOISY|PH_READONLY);
	if (!dao_array_isset(&roles_names, role_name)) {
		DAO_CONCAT_SVS(&exception_message, "Role \"", role_name, "\" does not exist in ACL");
		DAO_THROW_EXCEPTION_ZVAL(dao_acl_exception_ce, &exception_message);
		return;
	}

	dao_read_property(&resources_names, getThis(), SL("_resourcesNames"), PH_NOISY|PH_READONLY);
	if (!dao_array_isset(&resources_names, resource_name)) {
		DAO_CONCAT_SVS(&exception_message, "Resource \"", resource_name, "\" does not exist in ACL");
		DAO_THROW_EXCEPTION_ZVAL(dao_acl_exception_ce, &exception_message);
		return;
	}

	dao_read_property(&default_access, getThis(), SL("_defaultAccess"), PH_NOISY|PH_READONLY);
	dao_read_property(&access_list, getThis(), SL("_accessList"), PH_NOISY|PH_READONLY);
	dao_read_property(&internal_access, getThis(), SL("_access"), PH_NOISY|PH_READONLY);
	if (Z_TYPE_P(access) == IS_ARRAY) {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(access), access_name) {
			DAO_CONCAT_VSV(&access_key, resource_name, "!", access_name);
			if (!dao_array_isset(&access_list, &access_key)) {
				DAO_CONCAT_SVSVS(&exception_message, "Acccess '", access_name, "' does not exist in resource '", resource_name, "'");
				DAO_THROW_EXCEPTION_ZVAL(dao_acl_exception_ce, &exception_message);
				zval_ptr_dtor(&access_key);
				return;
			}
			zval_ptr_dtor(&access_key);
		} ZEND_HASH_FOREACH_END();

		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(access), access_name) {
			DAO_CONCAT_VSVSV(&access_key, role_name, "!", resource_name, "!", access_name);
			dao_update_property_array(getThis(), SL("_access"), &access_key, action);
			if (Z_TYPE_P(callback) != IS_NULL) {
				dao_update_property_array(getThis(), SL("_funcs"), &access_key, callback);
			}
			zval_ptr_dtor(&access_key);
		} ZEND_HASH_FOREACH_END();
	} else {
		if (!DAO_IS_STRING(access, "*") && !DAO_IS_STRING(resource_name, "*")) {
			DAO_CONCAT_VSV(&access_key, resource_name, "!", access);
			if (!dao_array_isset(&access_list, &access_key)) {
				DAO_CONCAT_SVSVS(&exception_message, "Acccess '", access, "' does not exist in resource '", resource_name, "'");
				DAO_THROW_EXCEPTION_ZVAL(dao_acl_exception_ce, &exception_message);
				zval_ptr_dtor(&access_key);
				return;
			}
			zval_ptr_dtor(&access_key);
		}

		DAO_CONCAT_VSVSV(&access_key, role_name, "!", resource_name, "!", access);

		/**
		 * Define the access action for the specified accessKey
		 */
		dao_update_property_array(getThis(), SL("_access"), &access_key, action);
		if (Z_TYPE_P(callback) != IS_NULL) {
			dao_update_property_array(getThis(), SL("_funcs"), &access_key, callback);
		}
		zval_ptr_dtor(&access_key);

		if (Z_TYPE(internal_access) == IS_ARRAY && !DAO_IS_STRING(resource_name, "*") && DAO_IS_STRING(access, "*")) {
			ZEND_HASH_FOREACH_STR_KEY(Z_ARRVAL(internal_access), str_key) {
				zval key = {}, arr = {}, arr_role_name = {}, arr_resource_name = {}, arr_access = {};
				if (likely(str_key != NULL)) {
					ZVAL_STR(&key, str_key);

					dao_fast_explode_str(&arr, SL("!"), &key);

					if (dao_fast_count_int(&arr) >= 3) {
						dao_array_fetch_long(&arr_role_name, &arr, 0, PH_NOISY|PH_READONLY);
						dao_array_fetch_long(&arr_resource_name, &arr, 1, PH_NOISY|PH_READONLY);

						if (DAO_IS_IDENTICAL(&arr_role_name, role_name) && DAO_IS_IDENTICAL(&arr_resource_name, resource_name)) {
							dao_array_fetch_long(&arr_access, &arr, 2, PH_NOISY|PH_READONLY);

							DAO_CONCAT_VSVSV(&access_key, &arr_role_name, "!", &arr_resource_name, "!", &arr_access);

							dao_update_property_array(getThis(), SL("_access"), &access_key, action);
							if (Z_TYPE_P(callback) != IS_NULL) {
								dao_update_property_array(getThis(), SL("_funcs"), &access_key, callback);
							}
							zval_ptr_dtor(&access_key);
						}
					}
					zval_ptr_dtor(&arr);
				}
			} ZEND_HASH_FOREACH_END();
		}
	}
}

/**
 * Allow access to a role on a resource
 *
 * You can use '*' as wildcard
 *
 * Example:
 * <code>
 * //Allow access to guests to search on customers
 * $acl->allow('guests', 'customers', 'search');
 *
 * //Allow access to guests to search or create on customers
 * $acl->allow('guests', 'customers', array('search', 'create'));
 *
 * //Allow access to any role to browse on products
 * $acl->allow('*', 'products', 'browse');
 *
 * //Allow access to any role to browse on any resource
 * $acl->allow('*', '*', 'browse');
 * </code>
 *
 * @param string $roleName
 * @param string $resourceName
 * @param mixed $access
 * @param callable $callback
 */
PHP_METHOD(Dao_Acl_Adapter_Memory, allow){

	zval *role_name, *resource_name, *access, *callback = NULL, action = {}, roles_names = {}, resources = {}, *resource = NULL;
	zend_string *str_key;

	dao_fetch_params(0, 3, 1, &role_name, &resource_name, &access, &callback);

	if (!callback) {
		callback = &DAO_GLOBAL(z_null);
	}

	ZVAL_LONG(&action, 1);

	if (!DAO_IS_STRING(role_name, "*")) {
		DAO_RETURN_CALL_METHOD(getThis(), "_allowordeny", role_name, resource_name, access, &action, callback);
		if (DAO_IS_STRING(resource_name, "*")) {
			dao_read_property(&resources, getThis(), SL("_resources"), PH_NOISY|PH_READONLY);
			ZEND_HASH_FOREACH_VAL(Z_ARRVAL(resources), resource) {
				zval name = {};
				DAO_CALL_METHOD(&name, resource, "getname");
				DAO_CALL_METHOD(NULL, getThis(), "_allowordeny", role_name, &name, access, &action, callback);
				zval_ptr_dtor(&name);
			} ZEND_HASH_FOREACH_END();
		}
	} else {
		dao_read_property(&roles_names, getThis(), SL("_rolesNames"), PH_NOISY|PH_READONLY);

		ZEND_HASH_FOREACH_STR_KEY(Z_ARRVAL(roles_names), str_key) {
			zval roles_name = {};
			if (str_key) {
				ZVAL_STR(&roles_name, str_key);
				DAO_CALL_METHOD(NULL, getThis(), "_allowordeny", &roles_name, resource_name, access, &action, callback);
				if (DAO_IS_STRING(resource_name, "*")) {
					dao_read_property(&resources, getThis(), SL("_resources"), PH_NOISY|PH_READONLY);
					ZEND_HASH_FOREACH_VAL(Z_ARRVAL(resources), resource) {
						zval name = {};
						DAO_CALL_METHOD(&name, resource, "getname");
						DAO_CALL_METHOD(NULL, getThis(), "_allowordeny", &roles_name, &name, access, &action, callback);
						zval_ptr_dtor(&name);
					} ZEND_HASH_FOREACH_END();
				}
			}
		} ZEND_HASH_FOREACH_END();
	}
}

/**
 * Deny access to a role on a resource
 *
 * You can use '*' as wildcard
 *
 * Example:
 * <code>
 * //Deny access to guests to search on customers
 * $acl->deny('guests', 'customers', 'search');
 *
 * //Deny access to guests to search or create on customers
 * $acl->deny('guests', 'customers', array('search', 'create'));
 *
 * //Deny access to any role to browse on products
 * $acl->deny('*', 'products', 'browse');
 *
 * //Deny access to any role to browse on any resource
 * $acl->deny('*', '*', 'browse');
 * </code>
 *
 * @param string $roleName
 * @param string $resourceName
 * @param mixed $access
 * @param callable $callback
 * @return boolean
 */
PHP_METHOD(Dao_Acl_Adapter_Memory, deny){

	zval *role_name, *resource_name, *access, *callback = NULL, action = {}, roles_names = {}, resources = {}, *resource = NULL;
	zend_string *str_key;

	dao_fetch_params(0, 3, 1, &role_name, &resource_name, &access, &callback);

	if (!callback) {
		callback = &DAO_GLOBAL(z_null);
	}

	ZVAL_LONG(&action, 0);

	if (!DAO_IS_STRING(role_name, "*")) {
		DAO_RETURN_CALL_METHOD(getThis(), "_allowordeny", role_name, resource_name, access, &action, callback);
		if (DAO_IS_STRING(resource_name, "*")) {
			dao_read_property(&resources, getThis(), SL("_resources"), PH_NOISY|PH_READONLY);
			ZEND_HASH_FOREACH_VAL(Z_ARRVAL(resources), resource) {
				zval name = {};
				DAO_CALL_METHOD(&name, resource, "getname");
				DAO_CALL_METHOD(NULL, getThis(), "_allowordeny", role_name, &name, access, &action, callback);
				zval_ptr_dtor(&name);
			} ZEND_HASH_FOREACH_END();
		}
	} else {
		dao_read_property(&roles_names, getThis(), SL("_rolesNames"), PH_NOISY|PH_READONLY);

		ZEND_HASH_FOREACH_STR_KEY(Z_ARRVAL(roles_names), str_key) {
			zval roles_name = {};
			if (str_key) {
				ZVAL_STR(&roles_name, str_key);
				DAO_CALL_METHOD(NULL, getThis(), "_allowordeny", &roles_name, resource_name, access, &action, callback);
				if (DAO_IS_STRING(resource_name, "*")) {
					dao_read_property(&resources, getThis(), SL("_resources"), PH_NOISY|PH_READONLY);
					ZEND_HASH_FOREACH_VAL(Z_ARRVAL(resources), resource) {
						zval name = {};
						DAO_CALL_METHOD(&name, resource, "getname");
						DAO_CALL_METHOD(NULL, getThis(), "_allowordeny", &roles_name, &name, access, &action, callback);
						zval_ptr_dtor(&name);
					} ZEND_HASH_FOREACH_END();
				}
			}
		} ZEND_HASH_FOREACH_END();
	}
}

static int dao_role_adapter_memory_check_inheritance(zval *role, zval *resource, zval *access, zval *access_list, zval* role_inherits)
{
	zval inherited_roles = {}, access_key = {};
	zval *parent_role;
	int result = DAO_ACL_DUNNO;

	assert(Z_TYPE_P(role) == IS_STRING);
	assert(Z_TYPE_P(resource) == IS_STRING);
	assert(Z_TYPE_P(access) == IS_STRING);
	assert(Z_TYPE_P(access_list) == IS_ARRAY);

	if (!dao_array_isset_fetch(&inherited_roles, role_inherits, role, PH_READONLY) || Z_TYPE(inherited_roles) != IS_ARRAY) {
		return DAO_ACL_DUNNO;
	}

	ZEND_HASH_FOREACH_VAL(Z_ARRVAL(inherited_roles), parent_role) {
		zval have_access = {};
		int found;

		dao_concat_vsvsv(&access_key, parent_role, SL("!"), resource, SL("!"), access, 0);
		found = dao_array_isset_fetch(&have_access, access_list, &access_key, PH_READONLY);

		if (found) {
			result = zend_is_true(&have_access) ? DAO_ACL_ALLOW : DAO_ACL_DENY;
		} else {
			result = dao_role_adapter_memory_check_inheritance(parent_role, resource, access, access_list, role_inherits);
		}

		if (DAO_ACL_DUNNO != result) {
			break;
		}
	} ZEND_HASH_FOREACH_END();

	return result;
}

/**
 * Check whether a role is allowed to access an action from a resource
 *
 * <code>
 * //Does andres have access to the customers resource to create?
 * $acl->isAllowed('andres', 'Products', 'create');
 *
 * //Do guests have access to any resource to edit?
 * $acl->isAllowed('guests', '*', 'edit');
 * </code>
 *
 * @param mixed $role
 * @param mixed $resource
 * @param string $access
 * @param mixed $data
 * @return boolean
 */
PHP_METHOD(Dao_Acl_Adapter_Memory, isAllowed){

	zval *role, *resource, *access, *data = NULL, role_name = {}, resource_name ={}, star = {}, event_name = {}, status = {}, default_access = {}, roles_names = {}, access_list = {};
	zval access_key = {}, have_access = {}, role_inherits = {}, funcs = {}, func = {}, arguments = {}, ret = {};
	int allow_access;

	dao_fetch_params(0, 3, 1, &role, &resource, &access, &data);

	if (!data) {
		data = &DAO_GLOBAL(z_null);
	}

	if (Z_TYPE_P(role) == IS_OBJECT) {
		if (!instanceof_function(Z_OBJCE_P(role), dao_acl_roleaware_ce)) {
			DAO_THROW_EXCEPTION_STR(dao_acl_exception_ce, "Object passed as role must implement Dao\\Acl\\RoleAware");
			return;
		} else {
			DAO_CALL_METHOD(&role_name, role, "getrolename");
		}
	} else {
		ZVAL_COPY_VALUE(&role_name, role);
	}

	if (Z_TYPE_P(resource) == IS_OBJECT) {
		if (!instanceof_function(Z_OBJCE_P(resource), dao_acl_resourceaware_ce)) {
			DAO_THROW_EXCEPTION_STR(dao_acl_exception_ce, "Object passed as resource must implement Dao\\Acl\\ResourceAware");
			return;
		} else {
			DAO_CALL_METHOD(&resource_name, resource, "getresourcename");
		}
	} else {
		ZVAL_COPY_VALUE(&resource_name, resource);
	}

	ZVAL_STRING(&star, "*");

	dao_update_property(getThis(), SL("_activeRole"), &role_name);
	dao_update_property(getThis(), SL("_activeResource"), &resource_name);
	dao_update_property(getThis(), SL("_activeAccess"), access);

	ZVAL_STRING(&event_name, "acl:beforeCheckAccess");
	DAO_CALL_METHOD(&status, getThis(), "fireeventcancel", &event_name);
	zval_ptr_dtor(&event_name);
	if (DAO_IS_FALSE(&status)) {
		RETURN_ZVAL(&status, 0, 0);
	}
	zval_ptr_dtor(&status);

	dao_read_property(&default_access, getThis(), SL("_defaultAccess"), PH_NOISY|PH_READONLY);

	/**
	 * Check if the role exists
	 */
	dao_read_property(&roles_names, getThis(), SL("_rolesNames"), PH_NOISY|PH_READONLY);
	if (!dao_array_isset(&roles_names, &role_name)) {
		RETURN_CTOR(&default_access);
	}

	dao_read_property(&access_list, getThis(), SL("_access"), PH_NOISY|PH_READONLY);

	DAO_CONCAT_VSVSV(&access_key, &role_name, "!", &resource_name, "!", access);

	/**
	 * Check if there is a direct combination for role-resource-access
	 */
	if (dao_array_isset_fetch(&have_access, &access_list, &access_key, PH_READONLY)) {
		allow_access = zend_is_true(&have_access) ? DAO_ACL_ALLOW : DAO_ACL_DENY;
	} else {
		allow_access = DAO_ACL_DUNNO;
	}

	/**
	 * Check in the inherits roles
	 */
	if (DAO_ACL_DUNNO == allow_access) {
		dao_read_property(&role_inherits, getThis(), SL("_roleInherits"), PH_NOISY|PH_READONLY);
		allow_access  = dao_role_adapter_memory_check_inheritance(role, resource, access, &access_list, &role_inherits);
	}

	/**
	 * If access wasn't found yet, try role-resource-*
	 */
	if (DAO_ACL_DUNNO == allow_access) {
		DAO_CONCAT_VSVS(&access_key, &role_name, "!", &resource_name, "!*");

		/**
		 * In the direct role
		 */
		if (dao_array_isset_fetch(&have_access, &access_list, &access_key, PH_READONLY)) {
			allow_access = zend_is_true(&have_access) ? DAO_ACL_ALLOW : DAO_ACL_DENY;
		} else {
			allow_access = dao_role_adapter_memory_check_inheritance(role, resource, &star, &access_list, &role_inherits);
		}
	}

	/**
	 * If access wasn't found yet, try role-*-*
	 */
	if (DAO_ACL_DUNNO == allow_access) {
		DAO_CONCAT_VS(&access_key, &role_name, "!*!*");

		/**
		 * Try in the direct role
		 */
		if (dao_array_isset_fetch(&have_access, &access_list, &access_key, PH_READONLY)) {
			allow_access = zend_is_true(&have_access) ? DAO_ACL_ALLOW : DAO_ACL_DENY;
		} else {
			allow_access = dao_role_adapter_memory_check_inheritance(role, &star, &star, &access_list, &role_inherits);
		}
	}

	ZVAL_BOOL(return_value, DAO_ACL_ALLOW == allow_access);

	if (DAO_ACL_DUNNO != allow_access) {
		dao_read_property(&funcs, getThis(), SL("_funcs"), PH_NOISY|PH_READONLY);
		if (dao_array_isset_fetch(&func, &funcs, &access_key, PH_READONLY)) {
			array_init_size(&arguments, 5);
			dao_array_append(&arguments, role, PH_COPY);
			dao_array_append(&arguments, resource, PH_COPY);
			dao_array_append(&arguments, access, PH_COPY);
			dao_array_append(&arguments, data, PH_COPY);
			dao_array_append_long(&arguments, allow_access, PH_COPY);
			DAO_CALL_USER_FUNC_ARRAY(&ret, &func, &arguments);
			if (!zend_is_true(&ret)) {
				ZVAL_BOOL(return_value, DAO_ACL_DENY == allow_access);
			}
			zval_ptr_dtor(&arguments);
		}
	}

	dao_update_property(getThis(), SL("_accessGranted"), return_value);

	ZVAL_STRING(&event_name, "acl:afterCheckAccess");
	DAO_CALL_METHOD(NULL, getThis(), "fireevent", &event_name, return_value);
	zval_ptr_dtor(&event_name);
}

/**
 * Return an array with every role registered in the list
 *
 * @return Dao\Acl\Role[]
 */
PHP_METHOD(Dao_Acl_Adapter_Memory, getRoles){


	RETURN_MEMBER(getThis(), "_roles");
}

/**
 * Return an array with every resource registered in the list
 *
 * @return Dao\Acl\Resource[]
 */
PHP_METHOD(Dao_Acl_Adapter_Memory, getResources){


	RETURN_MEMBER(getThis(), "_resources");
}

/**
 * Return the access action
 *
 * @return string[]
 */
PHP_METHOD(Dao_Acl_Adapter_Memory, getAccess){


	RETURN_MEMBER(getThis(), "_access");
}
