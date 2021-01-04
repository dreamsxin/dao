
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

#include "storage/frontend/igbinary.h"
#include "storage/frontendinterface.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"

/**
 * Dao\Storage\Frontend\Igbinary
 *
 * Allows to cache native PHP data in a serialized form using igbinary extension
 */
zend_class_entry *dao_storage_frontend_igbinary_ce;

PHP_METHOD(Dao_Storage_Frontend_Igbinary, beforeStore);
PHP_METHOD(Dao_Storage_Frontend_Igbinary, afterRetrieve);

static const zend_function_entry dao_storage_frontend_igbinary_method_entry[] = {
	PHP_ME(Dao_Storage_Frontend_Igbinary, beforeStore, arginfo_dao_storage_frontendinterface_beforestore, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Frontend_Igbinary, afterRetrieve, arginfo_dao_storage_frontendinterface_afterretrieve, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Storage\Frontend\Igbinary initializer
 */
DAO_INIT_CLASS(Dao_Storage_Frontend_Igbinary){

	DAO_REGISTER_CLASS(Dao\\Storage\\Frontend, Igbinary, storage_frontend_igbinary, dao_storage_frontend_igbinary_method_entry, 0);

	zend_class_implements(dao_storage_frontend_igbinary_ce, 1, dao_storage_frontendinterface_ce);

	return SUCCESS;
}

/**
 * Serializes data before storing them
 *
 * @param mixed $data
 * @return string
 */
PHP_METHOD(Dao_Storage_Frontend_Igbinary, beforeStore)
{
	zval *data;

	dao_fetch_params(0, 1, 0, &data);
	
	DAO_RETURN_CALL_FUNCTION("igbinary_serialize", data);
}

/**
 * Unserializes data after retrieval
 *
 * @param mixed $data
 * @return mixed
 */
PHP_METHOD(Dao_Storage_Frontend_Igbinary, afterRetrieve)
{
	zval *data;

	dao_fetch_params(0, 1, 0, &data);
	
	DAO_RETURN_CALL_FUNCTION("igbinary_unserialize", data);
}
