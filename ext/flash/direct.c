
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

#include "flash/direct.h"
#include "flash.h"
#include "flashinterface.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"

/**
 * Dao\Flash\Direct
 *
 * This is a variant of the Dao\Flash that inmediately outputs any message passed to it
 */
zend_class_entry *dao_flash_direct_ce;

PHP_METHOD(Dao_Flash_Direct, message);

static const zend_function_entry dao_flash_direct_method_entry[] = {
	PHP_ME(Dao_Flash_Direct, message, arginfo_dao_flashinterface_message, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Flash\Direct initializer
 */
DAO_INIT_CLASS(Dao_Flash_Direct){

	DAO_REGISTER_CLASS_EX(Dao\\Flash, Direct, flash_direct, dao_flash_ce, dao_flash_direct_method_entry, 0);

	zend_class_implements(dao_flash_direct_ce, 1, dao_flashinterface_ce);

	return SUCCESS;
}

/**
 * Outputs a message
 *
 * @param  string $type
 * @param  string $message
 * @return string
 */
PHP_METHOD(Dao_Flash_Direct, message)
{
	zval *type, *message;

	dao_fetch_params(0, 2, 0, &type, &message);

	DAO_RETURN_CALL_METHOD(getThis(), "outputmessage", type, message);
}
