
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

#include "assets/filters/none.h"
#include "assets/filterinterface.h"

#include "kernel/main.h"

/**
 * Dao\Assets\Filters\None
 *
 * Returns the content without make any modification to the original source
 */
zend_class_entry *dao_assets_filters_none_ce;

PHP_METHOD(Dao_Assets_Filters_None, filter);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_assets_filters_none_filter, 0, 0, 1)
	ZEND_ARG_INFO(0, content)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_assets_filters_none_method_entry[] = {
	PHP_ME(Dao_Assets_Filters_None, filter, arginfo_dao_assets_filters_none_filter, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Assets\Filters\None initializer
 */
DAO_INIT_CLASS(Dao_Assets_Filters_None){

	DAO_REGISTER_CLASS(Dao\\Assets\\Filters, None, assets_filters_none, dao_assets_filters_none_method_entry, 0);

	zend_class_implements(dao_assets_filters_none_ce, 1, dao_assets_filterinterface_ce);
	return SUCCESS;
}

/**
 * Returns the content without touching
 *
 * @param string $content
 * @return $content
 */
PHP_METHOD(Dao_Assets_Filters_None, filter){

	zval *content;

	dao_fetch_params(0, 1, 0, &content);
	
	RETURN_CTOR(content);
}

