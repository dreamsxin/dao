
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

#include "paginator/adapterinterface.h"
#include "kernel/main.h"

zend_class_entry *dao_paginator_adapterinterface_ce;

static const zend_function_entry dao_paginator_adapterinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Paginator_AdapterInterface, setCurrentPage, arginfo_dao_paginator_adapterinterface_setcurrentpage)
	PHP_ABSTRACT_ME(Dao_Paginator_AdapterInterface, getCurrentPage, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Paginator_AdapterInterface, setLimit, arginfo_dao_paginator_adapterinterface_setlimit)
	PHP_ABSTRACT_ME(Dao_Paginator_AdapterInterface, getLimit, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Paginator_AdapterInterface, getPaginate, arginfo_dao_paginator_adapterinterface_getpaginate)
	PHP_FE_END
};


/**
 * Dao\Paginator\AdapterInterface initializer
 */
DAO_INIT_CLASS(Dao_Paginator_AdapterInterface){

	DAO_REGISTER_INTERFACE(Dao\\Paginator, AdapterInterface, paginator_adapterinterface, dao_paginator_adapterinterface_method_entry);

	return SUCCESS;
}

/**
 * Set the current page number
 *
 * @param int $page
 */
DAO_DOC_METHOD(Dao_Paginator_AdapterInterface, setCurrentPage);

/**
 * Get the current page number
 *
 * @return int
 */
DAO_DOC_METHOD(Dao_Paginator_AdapterInterface, getCurrentPage);

/**
 * Set current rows limit
 *
 * @param int $limit
 */
DAO_DOC_METHOD(Dao_Paginator_AdapterInterface, setLimit);

/**
 * Get the current rows limit
 *
 * @return int
 */
DAO_DOC_METHOD(Dao_Paginator_AdapterInterface, getLimit);

/**
 * Returns a slice of the resultset to show in the pagination
 *
 * @return \stdClass
 */
DAO_DOC_METHOD(Dao_Paginator_AdapterInterface, getPaginate);
