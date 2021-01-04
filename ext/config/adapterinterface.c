
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
  |          Vladimir Kolesnikov <vladimir@free-sevastopol.com>            |
  +------------------------------------------------------------------------+
*/

#include "config/adapterinterface.h"
#include "kernel/main.h"

zend_class_entry *dao_config_adapterinterface_ce;

static const zend_function_entry dao_config_adapterinterface_method_entry[] = {
	ZEND_FENTRY(setBasePath, NULL, arginfo_dao_config_adapterinterface_setbasepath, ZEND_ACC_STATIC|ZEND_ACC_ABSTRACT|ZEND_ACC_PUBLIC)
	ZEND_FENTRY(getBasePath, NULL, arginfo_empty, ZEND_ACC_STATIC|ZEND_ACC_ABSTRACT|ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Config\AdapterInterface initializer
 */
DAO_INIT_CLASS(Dao_Config_AdapterInterface){

	DAO_REGISTER_INTERFACE(Dao\\Config, AdapterInterface, config_adapterinterface, dao_config_adapterinterface_method_entry);

	return SUCCESS;
}

/**
 * Sets base path
 *
 * @param string $basePath
 * @return Dao\Config\Adapter
 */
DAO_DOC_METHOD(Dao_Config_AdapterInterface, setBasePath);

/**
 * Gets base path
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Config_AdapterInterface, getBasePath);
