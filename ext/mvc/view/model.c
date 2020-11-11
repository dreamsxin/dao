
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

#include "mvc/view/model.h"
#include "mvc/view/exception.h"
#include "mvc/viewinterface.h"
#include "mvc/view/modelinterface.h"
#include "di.h"
#include "di/injectable.h"
#include "debug.h"

#include <Zend/zend_closures.h>

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/exception.h"
#include "kernel/operators.h"
#include "kernel/fcall.h"
#include "kernel/array.h"
#include "kernel/hash.h"
#include "kernel/concat.h"
#include "kernel/file.h"
#include "kernel/string.h"
#include "kernel/output.h"
#include "kernel/debug.h"

#include "interned-strings.h"

/**
 * Dao\Mvc\View\Model
 *
 * This component allows to render views without hicherquical levels
 *
 *<code>
 * $view = new Dao\Mvc\View\Model();
 * echo $view->render('templates/my-view', array('content' => $html));
 *</code>
 *
 */
zend_class_entry *dao_mvc_view_model_ce;

PHP_METHOD(Dao_Mvc_View_Model, __construct);
PHP_METHOD(Dao_Mvc_View_Model, setTemplate);
PHP_METHOD(Dao_Mvc_View_Model, getTemplate);
PHP_METHOD(Dao_Mvc_View_Model, setVars);
PHP_METHOD(Dao_Mvc_View_Model, getVars);
PHP_METHOD(Dao_Mvc_View_Model, setVar);
PHP_METHOD(Dao_Mvc_View_Model, getVar);
PHP_METHOD(Dao_Mvc_View_Model, addChild);
PHP_METHOD(Dao_Mvc_View_Model, appendChild);
PHP_METHOD(Dao_Mvc_View_Model, getChild);
PHP_METHOD(Dao_Mvc_View_Model, hasChild);
PHP_METHOD(Dao_Mvc_View_Model, setCaptureTo);
PHP_METHOD(Dao_Mvc_View_Model, getCaptureTo);
PHP_METHOD(Dao_Mvc_View_Model, setTerminal);
PHP_METHOD(Dao_Mvc_View_Model, getTerminal);
PHP_METHOD(Dao_Mvc_View_Model, setAppend);
PHP_METHOD(Dao_Mvc_View_Model, isAppend);
PHP_METHOD(Dao_Mvc_View_Model, setView);
PHP_METHOD(Dao_Mvc_View_Model, getView);
PHP_METHOD(Dao_Mvc_View_Model, render);
PHP_METHOD(Dao_Mvc_View_Model, __set);
PHP_METHOD(Dao_Mvc_View_Model, __get);
PHP_METHOD(Dao_Mvc_View_Model, __isset);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_view_model___construct, 0, 0, 0)
	ZEND_ARG_INFO(0, vars)
	ZEND_ARG_INFO(0, template)
	ZEND_ARG_INFO(0, capture)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_mvc_view_model_method_entry[] = {
	PHP_ME(Dao_Mvc_View_Model, __construct, arginfo_dao_mvc_view_model___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Mvc_View_Model, setTemplate, arginfo_dao_mvc_view_modelinterface_settemplate, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View_Model, getTemplate, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View_Model, setVars, arginfo_dao_mvc_view_modelinterface_setvars, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View_Model, getVars, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View_Model, setVar, arginfo_dao_mvc_view_modelinterface_setvar, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View_Model, getVar, arginfo_dao_mvc_view_modelinterface_getvar, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View_Model, addChild, arginfo_dao_mvc_view_modelinterface_addchild, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View_Model, appendChild, arginfo_dao_mvc_view_modelinterface_appendchild, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View_Model, getChild, arginfo_dao_mvc_view_modelinterface_getchild, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View_Model, hasChild, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View_Model, setCaptureTo, arginfo_dao_mvc_view_modelinterface_setcaptureto, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View_Model, getCaptureTo, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View_Model, setTerminal, arginfo_dao_mvc_view_modelinterface_setterminal, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View_Model, getTerminal, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View_Model, setAppend, arginfo_dao_mvc_view_modelinterface_setappend, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View_Model, isAppend, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View_Model, setView, arginfo_dao_mvc_view_modelinterface_setview, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View_Model, getView, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View_Model, render, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View_Model, __set, arginfo___set, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View_Model, __get, arginfo___get, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View_Model, __isset, arginfo___isset, ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Mvc_View_Model, __toString, render, arginfo___tostring, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Mvc\View\Model initializer
 */
DAO_INIT_CLASS(Dao_Mvc_View_Model){

	DAO_REGISTER_CLASS_EX(Dao\\Mvc\\View, Model, mvc_view_model, dao_di_injectable_ce, dao_mvc_view_model_method_entry, 0);

	zend_declare_property_null(dao_mvc_view_model_ce, SL("_viewParams"), ZEND_ACC_PROTECTED);
	zend_declare_property_string(dao_mvc_view_model_ce, SL("_captureTo"), "content", ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_view_model_ce, SL("_template"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_view_model_ce, SL("_childs"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_view_model_ce, SL("_terminal"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_view_model_ce, SL("_append"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_view_model_ce, SL("_view"), ZEND_ACC_PROTECTED);

	zend_class_implements(dao_mvc_view_model_ce, 1, dao_mvc_view_modelinterface_ce);

	return SUCCESS;
}

/**
 * Dao\Mvc\View\Model constructor
 *
 * @param array $options
 */
PHP_METHOD(Dao_Mvc_View_Model, __construct){

	zval *variables = NULL, *template = NULL, *capture = NULL;

	dao_fetch_params(0, 0, 3, &variables, &template, &capture);

	if (variables && Z_TYPE_P(variables) != IS_NULL) {
		DAO_CALL_SELF(NULL, "setvars", variables);
	}

	if (template && Z_TYPE_P(template) != IS_NULL) {
		DAO_CALL_SELF(NULL, "settemplate", template);
	}

	if (capture && Z_TYPE_P(capture) != IS_NULL) {
		DAO_CALL_SELF(NULL, "setcaptureto", capture);
	}
}

/**
 * Set the template to be used by this model
 *
 * @param  string $template
 * @return Dao\Mvc\View\ModelInterface
 */
PHP_METHOD(Dao_Mvc_View_Model, setTemplate){

	zval *template;

	dao_fetch_params(0, 1, 0, &template);

	dao_update_property(getThis(), SL("_template"), template);

	RETURN_THIS();
}

/**
 * Get the template to be used by this model
 *
 * @return string
 */
PHP_METHOD(Dao_Mvc_View_Model, getTemplate){

	RETURN_MEMBER(getThis(), "_template");
}

/**
 * Set all the render params
 *
 * @param array $params
 * @param boolean $merge
 * @return Dao\Mvc\View\ModelInterface
 */
PHP_METHOD(Dao_Mvc_View_Model, setVars){

	zval *params, *merge = NULL, view_params = {}, merged_params = {};

	dao_fetch_params(1, 1, 1, &params, &merge);

	if (!merge) {
		merge = &DAO_GLOBAL(z_true);
	}

	if (Z_TYPE_P(params) != IS_ARRAY) {
		DAO_MM_THROW_EXCEPTION_STR(dao_mvc_view_exception_ce, "The render parameters must be an array");
		return;
	}

	if (zend_is_true(merge)) {
		dao_read_property(&view_params, getThis(), SL("_viewParams"), PH_NOISY|PH_READONLY);
		if (Z_TYPE(view_params) == IS_ARRAY) {
			dao_fast_array_merge(&merged_params, &view_params, params);
			DAO_MM_ADD_ENTRY(&merged_params);
		} else {
			ZVAL_COPY_VALUE(&merged_params, params);
		}

		dao_update_property(getThis(), SL("_viewParams"), &merged_params);
	} else {
		dao_update_property(getThis(), SL("_viewParams"), params);
	}

	RETURN_MM_THIS();
}

/**
 * Get the vars
 *
 * @return string
 */
PHP_METHOD(Dao_Mvc_View_Model, getVars){

	RETURN_MEMBER(getThis(), "_viewParams");
}

/**
 * Set a single view parameter
 *
 * @param string $key
 * @param mixed $value
 * @return Dao\Mvc\View\ModelInterface
 */
PHP_METHOD(Dao_Mvc_View_Model, setVar){

	zval *key, *value, *isappend = NULL, view_params = {}, var = {}, var_append = {};

	dao_fetch_params(0, 2, 1, &key, &value, &isappend);

	if (isappend && zend_is_true(isappend)) {
		dao_read_property(&view_params, getThis(), SL("_viewParams"), PH_NOISY|PH_READONLY);

		if (Z_TYPE(view_params) != IS_ARRAY || !dao_array_isset_fetch(&var, &view_params, key, PH_READONLY)) {
			ZVAL_COPY_VALUE(&var_append, value);
		}
	} else {
		ZVAL_COPY_VALUE(&var_append, value);
	}

	dao_update_property_array(getThis(), SL("_viewParams"), key, &var_append);

	RETURN_THIS();
}

/**
 * Get the vars
 *
 * @param string $key
 * @return mixed
 */
PHP_METHOD(Dao_Mvc_View_Model, getVar){

	zval *key, *default_value = NULL, view_params = {}, var = {};

	dao_fetch_params(0, 1, 1, &key, &default_value);

	if (!default_value) {
		default_value = &DAO_GLOBAL(z_null);
	}

	dao_read_property(&view_params, getThis(), SL("_viewParams"), PH_NOISY|PH_READONLY);

	if (Z_TYPE(view_params) != IS_ARRAY || !dao_array_isset_fetch(&var, &view_params, key, PH_READONLY)) {
		ZVAL_COPY_VALUE(&var, default_value);
	}

	RETURN_CTOR(&var);
}

/**
 * Add a child model
 *
 * @param  Dao\Mvc\View\ModelInterface $child
 * @param  null|string $captureTo Optional; if specified, the "capture to" value to set on the child
 * @param  null|bool $append Optional; if specified, append to child  with the same capture
 * @return Dao\Mvc\View\ModelInterface
 */
PHP_METHOD(Dao_Mvc_View_Model, addChild){

	zval *child, *capture_to = NULL, *append = NULL;

	dao_fetch_params(0, 1, 2, &child, &capture_to, &append);

	if (capture_to && !DAO_IS_EMPTY(capture_to)) {
		DAO_CALL_METHOD(NULL, child, "setcaptureto", capture_to);
	}

	if (append) {
		DAO_CALL_METHOD(NULL, child, "setappend", append);
	}

	dao_update_property_array_append(getThis(), SL("_childs"), child);

	RETURN_THIS();
}

/**
 * Append a child model
 *
 * @param  Dao\Mvc\View\ModelInterface $child
 * @param  null|string $captureTo Optional; if specified, the "capture to" value to set on the child
 * @return Dao\Mvc\View\ModelInterface
 */
PHP_METHOD(Dao_Mvc_View_Model, appendChild){

	zval *child, *capture_to = NULL;

	dao_fetch_params(0, 1, 1, &child, &capture_to);

	if (!capture_to) {
		capture_to = &DAO_GLOBAL(z_null);
	}

	DAO_CALL_SELF(NULL, "addchild", child, capture_to, &DAO_GLOBAL(z_true));

	RETURN_THIS();
}

/**
 * Return a child model or all child model
 *
 * @param null|string $captureTo
 * @return array
 */
PHP_METHOD(Dao_Mvc_View_Model, getChild){

	zval *capture_to = NULL, childs = {}, *child;

	dao_fetch_params(1, 0, 1, &capture_to);

	if (capture_to) {
		array_init(return_value);

		dao_read_property(&childs, getThis(), SL("_childs"), PH_NOISY|PH_READONLY);

		if (Z_TYPE(childs) != IS_ARRAY) {
			RETURN_MM_EMPTY_ARRAY();
		}

		ZEND_HASH_FOREACH_VAL(Z_ARRVAL(childs), child) {
			zval child_capture_to = {};

			DAO_MM_CALL_METHOD(&child_capture_to, child, "getcaptureto");
			DAO_MM_ADD_ENTRY(&child_capture_to);
			if (dao_memnstr(capture_to, &child_capture_to)) {
				dao_array_append(return_value, child, PH_COPY);
			}
		} ZEND_HASH_FOREACH_END();
		RETURN_MM();
	}
	RETURN_MM_MEMBER(getThis(), "_childs");
}

/**
 * Does the model have any children?
 *
 * @param null|string $captureTo
 * @return boolean
 */
PHP_METHOD(Dao_Mvc_View_Model, hasChild){

	zval *capture_to = NULL, childs = {}, *child;

	dao_fetch_params(1, 0, 1, &capture_to);

	dao_read_property(&childs, getThis(), SL("_childs"), PH_NOISY|PH_READONLY);

	if (Z_TYPE(childs) != IS_ARRAY) {
		RETURN_MM_FALSE;
	}

	if (!dao_fast_count_ev(&childs)) {
		RETURN_MM_FALSE;
	}

	if (capture_to) {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL(childs), child) {
			zval child_capture_to = {};

			DAO_MM_CALL_METHOD(&child_capture_to, child, "getcaptureto");
			DAO_MM_ADD_ENTRY(&child_capture_to);
			if (dao_memnstr(capture_to, &child_capture_to)) {
				RETURN_TRUE;
			}
		} ZEND_HASH_FOREACH_END();

		RETURN_MM_FALSE;
	}

	RETURN_MM_TRUE;
}

/**
 * Set the name of the variable to capture this model to, if it is a child model
 *
 * @param string $capture
 * @return Dao\Mvc\View\ModelInterface
 */
PHP_METHOD(Dao_Mvc_View_Model, setCaptureTo){

	zval *capture;

	dao_fetch_params(0, 1, 0, &capture);

	dao_update_property(getThis(), SL("_captureTo"), capture);

	RETURN_THIS();
}

/**
 * Get the name of the variable to which to capture this model
 *
 * @return string
 */
PHP_METHOD(Dao_Mvc_View_Model, getCaptureTo){

	RETURN_MEMBER(getThis(), "_captureTo");
}

/**
 * Set flag indicating whether or not this is considered a terminal or standalone model
 *
 * @param boolean $terminate
 * @return Dao\Mvc\View\ModelInterface
 */
PHP_METHOD(Dao_Mvc_View_Model, setTerminal){

	zval *terminate;

	dao_fetch_params(0, 1, 0, &terminate);

	dao_update_property(getThis(), SL("_terminate"), terminate);

	RETURN_THIS();
}

/**
 * Is this considered a terminal or standalone model?
 *
 * @return boolean
 */
PHP_METHOD(Dao_Mvc_View_Model, getTerminal){

	RETURN_MEMBER(getThis(), "_terminate");
}

/**
 * Set flag indicating whether or not append to child  with the same capture
 *
 * @param boolean $append
 * @return Dao\Mvc\View\ModelInterface
 */
PHP_METHOD(Dao_Mvc_View_Model, setAppend){

	zval *append;

	dao_fetch_params(0, 1, 0, &append);

	dao_update_property(getThis(), SL("_append"), append);

	RETURN_THIS();
}

/**
 * Is this append to child  with the same capture?
 *
 * @return boolean
 */
PHP_METHOD(Dao_Mvc_View_Model, isAppend){

	zval append = {};

	dao_read_property(&append, getThis(), SL("_append"), PH_NOISY|PH_READONLY);

	if (zend_is_true(&append)) {
		RETURN_TRUE;
	}

	RETURN_FALSE;
}

/**
 * Set the view
 *
 * @param Dao\Mvc\ViewInterface $view
 * @return Dao\Mvc\View\ModelInterface
 */
PHP_METHOD(Dao_Mvc_View_Model, setView){

	zval *view;

	dao_fetch_params(0, 1, 0, &view);

	dao_update_property(getThis(), SL("_view"), view);

	RETURN_THIS();
}

/**
 * Get the view
 *
 * @return Dao\Mvc\ViewInterface
 */
PHP_METHOD(Dao_Mvc_View_Model, getView){


	RETURN_MEMBER(getThis(), "_view");
}

/**
 * Renders the view
 *
 * @return string
 */
PHP_METHOD(Dao_Mvc_View_Model, render){

	zval child_contents = {}, debug_message = {}, childs = {}, *child, view = {}, dependency_injector = {}, service = {}, event_name = {};
	zval status = {}, not_exists = {}, base_path = {}, paths = {}, views_dir = {}, vars = {}, new_vars = {}, tpl = {};
	zval views_dir_path = {}, engines = {}, *engine, *path, exception_message = {}, contents = {};
	zend_string *str_key;
	ulong idx;

	DAO_MM_INIT();
	array_init(&child_contents);
	DAO_MM_ADD_ENTRY(&child_contents);

	DAO_MM_CALL_METHOD(&childs, getThis(), "getchild");

	if (Z_TYPE(childs) == IS_ARRAY && dao_fast_count_ev(&childs)) {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL(childs), child) {
			zval isappend = {}, capture = {}, content = {}, child_content = {};

			DAO_MM_CALL_METHOD(&isappend, child, "isappend");
			DAO_MM_ADD_ENTRY(&isappend);
			DAO_MM_CALL_METHOD(&capture, child, "getcaptureto");
			DAO_MM_ADD_ENTRY(&capture);
			DAO_MM_CALL_METHOD(&content, child, "render");
			DAO_MM_ADD_ENTRY(&content);

			if (zend_is_true(&isappend)) {
				if (Z_TYPE(child_contents) == IS_ARRAY && dao_array_isset_fetch(&child_content, &child_contents, &capture, PH_READONLY)) {
					zval content_append = {};
					DAO_CONCAT_VV(&content_append, &child_content, &content);
					dao_array_update(&child_contents, &capture, &content_append, 0);
				} else {
					dao_array_update(&child_contents, &capture, &content, PH_COPY);
				}
			} else {
				dao_array_update(&child_contents, &capture, &content, PH_COPY);
			}
		} ZEND_HASH_FOREACH_END();
	}

	dao_ob_start();

	dao_read_property(&view, getThis(), SL("_view"), PH_COPY);

	if (Z_TYPE(view) != IS_OBJECT) {
		DAO_MM_CALL_CE_STATIC(&dependency_injector, dao_di_ce, "getdefault");
		DAO_MM_ADD_ENTRY(&dependency_injector);

		ZVAL_STR(&service, IS(view));
		DAO_MM_CALL_METHOD(&view, &dependency_injector, "getshared", &service);
		DAO_MM_ADD_ENTRY(&view);
	}

	DAO_MM_VERIFY_INTERFACE(&view, dao_mvc_viewinterface_ce);

	/**
	 * Call beforeRender if there is an events manager
	 */
	DAO_MM_ZVAL_STRING(&event_name, "view:beforeRender");
	DAO_MM_CALL_METHOD(&status, getThis(), "fireeventcancel", &event_name);
	if (DAO_IS_FALSE(&status)) {
		RETURN_NULL();
	}
	zval_ptr_dtor(&status);

	ZVAL_TRUE(&not_exists);

	DAO_MM_CALL_METHOD(&base_path, &view, "getbasepath");
	DAO_MM_ADD_ENTRY(&base_path);
	if (Z_TYPE(base_path) != IS_ARRAY) {
		array_init(&paths);
		DAO_MM_ADD_ENTRY(&paths);
		dao_array_append(&paths, &base_path, PH_COPY);
	} else {
		ZVAL_COPY_VALUE(&paths, &base_path);
	}

	DAO_MM_CALL_METHOD(&views_dir, &view, "getviewsdir");
	DAO_MM_ADD_ENTRY(&views_dir);
	DAO_MM_CALL_METHOD(&vars, getThis(), "getvars");
	DAO_MM_ADD_ENTRY(&vars);
	DAO_MM_CALL_METHOD(&tpl, getThis(), "gettemplate");
	DAO_MM_ADD_ENTRY(&tpl);

	if (unlikely(DAO_GLOBAL(debug).enable_debug)) {
		DAO_CONCAT_SV(&debug_message, "Render Model View: ", &tpl);
		DAO_DEBUG_LOG(&debug_message);
		zval_ptr_dtor(&debug_message);
	}

	if (Z_TYPE(vars) == IS_ARRAY) {
		dao_fast_array_merge(&new_vars, &vars, &child_contents);
		DAO_MM_ADD_ENTRY(&new_vars);
	} else {
		ZVAL_COPY_VALUE(&new_vars, &child_contents);
	}

	DAO_CONCAT_VV(&views_dir_path, &views_dir, &tpl);
	DAO_MM_ADD_ENTRY(&views_dir_path);

	DAO_MM_CALL_METHOD(&engines, &view, "getEngines");
	DAO_MM_ADD_ENTRY(&engines);

	/**
	 * Views are rendered in each engine
	 */
	ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL(engines), idx, str_key, engine) {
		zval extension = {};
		if (str_key) {
			ZVAL_STR(&extension, str_key);
		} else {
			ZVAL_LONG(&extension, idx);
		}

		ZEND_HASH_FOREACH_VAL(Z_ARRVAL(paths), path) {
			zval view_engine_path = {};

			DAO_CONCAT_VVV(&view_engine_path, path, &views_dir_path, &extension);
			DAO_MM_ADD_ENTRY(&view_engine_path);
			if (dao_file_exists(&view_engine_path) == SUCCESS) {

				if (unlikely(DAO_GLOBAL(debug).enable_debug)) {
					DAO_CONCAT_SV(&debug_message, "--Found: ", &view_engine_path);
					DAO_DEBUG_LOG(&debug_message);
					zval_ptr_dtor(&debug_message);
				}

				/**
				 * Call beforeRenderView if there is a events manager available
				 */
				DAO_MM_ZVAL_STRING(&event_name, "view:beforeRenderView");
				DAO_MM_CALL_METHOD(&status, getThis(), "fireeventcancel", &event_name, &view_engine_path);
				if (DAO_IS_FALSE(&status)) {
					continue;
				}
				zval_ptr_dtor(&status);

				DAO_MM_CALL_METHOD(NULL, engine, "render", &view_engine_path, &new_vars, &DAO_GLOBAL(z_false));

				/**
				 * Call afterRenderView if there is a events manager available
				 */
				ZVAL_FALSE(&not_exists);

				DAO_MM_ZVAL_STRING(&event_name, "view:afterRenderView");
				DAO_MM_CALL_METHOD(NULL, getThis(), "fireeventcancel", &event_name);
				break;
			} else if (unlikely(DAO_GLOBAL(debug).enable_debug)) {
				DAO_CONCAT_SV(&debug_message, "--Not Found: ", &view_engine_path);
				DAO_DEBUG_LOG(&debug_message);
				zval_ptr_dtor(&debug_message);
			}
		} ZEND_HASH_FOREACH_END();
	} ZEND_HASH_FOREACH_END();

	/**
	 * Always throw an exception if the view does not exist
	 */
	if (DAO_IS_TRUE(&not_exists)) {
		DAO_CONCAT_SVS(&exception_message, "View '", &views_dir_path, "' was not found in the views directory");
		DAO_MM_ADD_ENTRY(&exception_message);
		DAO_MM_THROW_EXCEPTION_ZVAL(dao_mvc_view_exception_ce, &exception_message);
		return;
	}

	/**
	 * Call afterRender event
	 */
	DAO_MM_ZVAL_STRING(&event_name, "view:afterRender");
	DAO_MM_CALL_METHOD(NULL, getThis(), "fireevent", &event_name);

	dao_ob_get_contents(&contents);
	dao_ob_end_clean();

	RETURN_MM_NCTOR(&contents);
}

/**
 * Magic method to pass variables to the views
 *
 * @param string $key
 * @param mixed $value
 */
PHP_METHOD(Dao_Mvc_View_Model, __set){

	zval *key, *value;

	dao_fetch_params(0, 2, 0, &key, &value);

	dao_update_property_array(getThis(), SL("_viewParams"), key, value);

}

/**
 * Magic method to retrieve a variable passed to the view
 *
 * @param string $key
 * @return mixed
 */
PHP_METHOD(Dao_Mvc_View_Model, __get){

	zval *key, params = {}, value = {};

	dao_fetch_params(0, 1, 0, &key);

	dao_read_property(&params, getThis(), SL("_viewParams"), PH_NOISY|PH_READONLY);
	if (dao_array_isset_fetch(&value, &params, key, PH_READONLY)) {
		RETURN_CTOR(&value);
	}

	RETURN_NULL();
}

/**
 * Magic method to inaccessible a variable passed to the view
 *
 * @param string $key
 * @return boolean
 */
PHP_METHOD(Dao_Mvc_View_Model, __isset){

	zval *key, params = {};

	dao_fetch_params(0, 1, 0, &key);

	dao_read_property(&params, getThis(), SL("_viewParams"), PH_NOISY|PH_READONLY);
	if (dao_array_isset(&params, key)) {
		RETURN_TRUE;
	}

	RETURN_FALSE;
}
