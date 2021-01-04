
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

#include "storage/exception.h"
#include "storage/../exception.h"
#include "kernel/main.h"

/**
 * Dao\Storage\Exception
 *
 * Exceptions thrown in Dao\Storage will use this class
 *
 */
zend_class_entry *dao_storage_exception_ce;

/**
 * Dao\Storage\Exception initializer
 */
DAO_INIT_CLASS(Dao_Storage_Exception){

	DAO_REGISTER_CLASS_EX(Dao\\Storage, Exception, storage_exception, dao_exception_ce, NULL, 0);

	return SUCCESS;
}
