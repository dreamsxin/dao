
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

#include "filter/userfilterinterface.h"
#include "kernel/main.h"

zend_class_entry *dao_filter_userfilterinterface_ce;

static const zend_function_entry dao_filter_userfilterinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Filter_UserFilterInterface, filter, arginfo_dao_filter_userfilterinterface_filter)
	PHP_FE_END
};

/**
 * Dao\Filter\UserFilterInterface initializer
 */
DAO_INIT_CLASS(Dao_Filter_UserFilterInterface){

	DAO_REGISTER_INTERFACE(Dao\\Filter, UserFilterInterface, filter_userfilterinterface, dao_filter_userfilterinterface_method_entry);

	return SUCCESS;
}

/**
 * Filters a value
 *
 * @param mixed $value
 * @return mixed
 */
DAO_DOC_METHOD(Dao_Filter_UserFilterInterface, filter);
