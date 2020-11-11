
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

#include "mvc/view/engine/php.h"
#include "mvc/view/engine.h"
#include "mvc/view/engineinterface.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/operators.h"
#include "kernel/fcall.h"
#include "kernel/output.h"
#include "kernel/hash.h"
#include "kernel/require.h"
#include "kernel/object.h"
#include "kernel/debug.h"

/**
 * Dao\Mvc\View\Engine\Php
 *
 * Adapter to use PHP itself as templating engine
 */
zend_class_entry *dao_mvc_view_engine_php_ce;

PHP_METHOD(Dao_Mvc_View_Engine_Php, render);

static const zend_function_entry dao_mvc_view_engine_php_method_entry[] = {
	PHP_ME(Dao_Mvc_View_Engine_Php, render, arginfo_dao_mvc_view_engineinterface_render, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Mvc\View\Engine\Php initializer
 */
DAO_INIT_CLASS(Dao_Mvc_View_Engine_Php){

	DAO_REGISTER_CLASS_EX(Dao\\Mvc\\View\\Engine, Php, mvc_view_engine_php, dao_mvc_view_engine_ce, dao_mvc_view_engine_php_method_entry, 0);

	zend_class_implements(dao_mvc_view_engine_php_ce, 1, dao_mvc_view_engineinterface_ce);

	return SUCCESS;
}

/**
 * Renders a view using the template engine
 *
 * @param string $path
 * @param array $params
 * @param boolean $mustClean
 */
PHP_METHOD(Dao_Mvc_View_Engine_Php, render){

	zval *path, *params, *must_clean = NULL, contents = {}, view = {};
	int clean = 0;

	dao_fetch_params(0, 2, 2, &path, &params, &must_clean);

	DAO_ENSURE_IS_STRING(path);

	if (must_clean) {
		clean = DAO_IS_TRUE(must_clean);
	}

	if (clean && dao_ob_get_level() >= 1) {
		dao_ob_clean();
	}

	/**
	 * Require the file
	 */
	if (likely(DAO_GLOBAL(mvc).enable_view_strict)) {
		if (dao_exec_file(&contents, NULL, path, params, NULL) == FAILURE) {
			RETURN_FALSE;
		}
	} else {
		zend_array *symbol_table = zend_rebuild_symbol_table();

		if (dao_exec_file(&contents, NULL, path, params, symbol_table) == FAILURE) {
			RETURN_FALSE;
		}
	}

	if (clean) {
		int flag = 0;

		dao_read_property(&view, getThis(), SL("_view"), PH_NOISY|PH_READONLY);
		DAO_CALL_METHOD_FLAG(flag, NULL, &view, "setcontent", &contents);
		zval_ptr_dtor(&contents);
		if (flag == FAILURE) {
			RETURN_FALSE;
		}
	} else {
		zend_print_zval_r(&contents, 0);
		zval_ptr_dtor(&contents);
	}

	RETURN_TRUE;
}
