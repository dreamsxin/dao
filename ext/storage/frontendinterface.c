
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

#include "storage/frontendinterface.h"

#include "kernel/main.h"

zend_class_entry *dao_storage_frontendinterface_ce;

static const zend_function_entry dao_storage_frontendinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Storage_FrontendInterface, beforeStore, arginfo_dao_storage_frontendinterface_beforestore)
	PHP_ABSTRACT_ME(Dao_Storage_FrontendInterface, afterRetrieve, arginfo_dao_storage_frontendinterface_afterretrieve)
	PHP_FE_END
};


/**
 * Dao\Storage\FrontendInterface initializer
 */
DAO_INIT_CLASS(Dao_Storage_FrontendInterface){

	DAO_REGISTER_INTERFACE(Dao\\Storage, FrontendInterface, storage_frontendinterface, dao_storage_frontendinterface_method_entry);

	return SUCCESS;
}

/**
 * Serializes data before storing it
 *
 * @param mixed $data
 */
DAO_DOC_METHOD(Dao_Storage_FrontendInterface, beforeStore);

/**
 * Unserializes data after retrieving it
 *
 * @param mixed $data
 */
DAO_DOC_METHOD(Dao_Storage_FrontendInterface, afterRetrieve);
