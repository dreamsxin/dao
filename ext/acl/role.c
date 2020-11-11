
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

#include "acl/role.h"
#include "acl/roleinterface.h"
#include "acl/exception.h"

#include "kernel/main.h"
#include "kernel/exception.h"
#include "kernel/object.h"
#include "kernel/operators.h"

/**
 * Dao\Acl\Role
 *
 * This class defines role entity and its description
 *
 */
zend_class_entry *dao_acl_role_ce;

PHP_METHOD(Dao_Acl_Role, __construct);
PHP_METHOD(Dao_Acl_Role, getName);
PHP_METHOD(Dao_Acl_Role, getDescription);
PHP_METHOD(Dao_Acl_Role, __toString);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_acl_role___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, description)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_acl_role_method_entry[] = {
	PHP_ME(Dao_Acl_Role, __construct, arginfo_dao_acl_role___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Acl_Role, getName, arginfo_dao_acl_roleinterface_getname, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Acl_Role, getDescription, arginfo_dao_acl_roleinterface_getdescription, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Acl_Role, __toString, arginfo___tostring, ZEND_ACC_PUBLIC)
	PHP_FE_END
};


/**
 * Dao\Acl\Role initializer
 */
DAO_INIT_CLASS(Dao_Acl_Role){

	DAO_REGISTER_CLASS(Dao\\Acl, Role, acl_role, dao_acl_role_method_entry, 0);

	zend_declare_property_null(dao_acl_role_ce, SL("_name"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_acl_role_ce, SL("_description"), ZEND_ACC_PROTECTED);

	zend_class_implements(dao_acl_role_ce, 1, dao_acl_roleinterface_ce);

	return SUCCESS;
}

/**
 * Dao\Acl\Role description
 *
 * @param string $name
 * @param string $description
 */
PHP_METHOD(Dao_Acl_Role, __construct){

	zval *name, *description = NULL;

	dao_fetch_params(0, 1, 1, &name, &description);
	
	if (DAO_IS_STRING(name, "*")) {
		DAO_THROW_EXCEPTION_STR(dao_acl_exception_ce, "Role name cannot be \"*\"");
		return;
	}

	if (Z_TYPE_P(name) != IS_STRING) {
		convert_to_string(name);
	}

	dao_update_property(getThis(), SL("_name"), name);
	if (description && Z_TYPE_P(description) != IS_NULL) {
		dao_update_property(getThis(), SL("_description"), description);
	}
}

/**
 * Returns the role name
 *
 * @return string
 */
PHP_METHOD(Dao_Acl_Role, getName){


	RETURN_MEMBER(getThis(), "_name");
}

/**
 * Returns role description
 *
 * @return string
 */
PHP_METHOD(Dao_Acl_Role, getDescription){


	RETURN_MEMBER(getThis(), "_description");
}

/**
 * Magic method __toString
 *
 * @return string
 */
PHP_METHOD(Dao_Acl_Role, __toString){


	RETURN_MEMBER(getThis(), "_name");
}

