
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

#include "acl/resourceaware.h"
#include "kernel/main.h"

zend_class_entry *dao_acl_resourceaware_ce;

static const zend_function_entry dao_acl_resourceaware_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Acl_ResourceAware, getResourceName, arginfo_empty)
	PHP_FE_END
};

/**
 * Dao\Acl\ResourceAware initializer
 */
DAO_INIT_CLASS(Dao_Acl_ResourceAware){

	DAO_REGISTER_INTERFACE(Dao\\Acl, ResourceAware, acl_resourceaware, dao_acl_resourceaware_method_entry);

	return SUCCESS;
}

/**
 * Returns the resource name
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Acl_ResourceAware, getResourceName);
