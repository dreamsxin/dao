
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

#include "assets/filterinterface.h"
#include "kernel/main.h"

zend_class_entry *dao_assets_filterinterface_ce;

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_assets_filterinterface_filter, 0, 0, 1)
	ZEND_ARG_INFO(0, content)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_assets_filterinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Assets_FilterInterface, filter, arginfo_dao_assets_filterinterface_filter)
	PHP_FE_END
};

/**
 * Dao\Assets\FilterInterface initializer
 */
DAO_INIT_CLASS(Dao_Assets_FilterInterface){

	DAO_REGISTER_INTERFACE(Dao\\Assets, FilterInterface, assets_filterinterface, dao_assets_filterinterface_method_entry);

	return SUCCESS;
}

/**
 * Filters the content returning a string with the filtered content
 *
 * @param string $content
 * @return $content
 */
DAO_DOC_METHOD(Dao_Assets_FilterInterface, filter);
