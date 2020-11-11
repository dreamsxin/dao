
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

#include "filterinterface.h"
#include "kernel/main.h"

zend_class_entry *dao_filterinterface_ce;

static const zend_function_entry dao_filterinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_FilterInterface, add, arginfo_dao_filterinterface_add)
	PHP_ABSTRACT_ME(Dao_FilterInterface, sanitize, arginfo_dao_filterinterface_sanitize)
	PHP_ABSTRACT_ME(Dao_FilterInterface, getFilters, arginfo_empty)
	PHP_FE_END
};

/**
 * Dao\FilterInterface initializer
 */
DAO_INIT_CLASS(Dao_FilterInterface){

	DAO_REGISTER_INTERFACE(Dao, FilterInterface, filterinterface, dao_filterinterface_method_entry);

	return SUCCESS;
}

/**
 * Adds a user-defined filter
 *
 * @param string $name
 * @param callable|closure $handler
 * @return Dao\FilterInterface
 */
DAO_DOC_METHOD(Dao_FilterInterface, add);

/**
 * Sanizites a value with a specified single or set of filters
 *
 * @param  mixed $value
 * @param  mixed $filters
 * @return mixed
 */
DAO_DOC_METHOD(Dao_FilterInterface, sanitize);

/**
 * Return the user-defined filters in the instance
 *
 * @return object[]
 */
DAO_DOC_METHOD(Dao_FilterInterface, getFilters);
