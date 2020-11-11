
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

#include "tag/select.h"
#include "tag.h"
#include "tag/exception.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/string.h"
#include "kernel/fcall.h"
#include "kernel/hash.h"
#include "kernel/operators.h"
#include "kernel/concat.h"
#include "kernel/exception.h"
#include "kernel/object.h"

/**
 * Dao\Tag\Select
 *
 * Generates a SELECT html tag using a static array of values or a Dao\Mvc\Model resultset
 */
zend_class_entry *dao_tag_select_ce;

PHP_METHOD(Dao_Tag_Select, selectField);
PHP_METHOD(Dao_Tag_Select, _optionsFromResultset);
PHP_METHOD(Dao_Tag_Select, _optionsFromArray);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_tag_select_selectfield, 0, 0, 1)
	ZEND_ARG_INFO(0, parameters)
	ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_tag_select_method_entry[] = {
	PHP_ME(Dao_Tag_Select, selectField, arginfo_dao_tag_select_selectfield, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Tag_Select, _optionsFromResultset, arginfo_empty, ZEND_ACC_PROTECTED|ZEND_ACC_STATIC)
	PHP_ME(Dao_Tag_Select, _optionsFromArray, arginfo_empty, ZEND_ACC_PROTECTED|ZEND_ACC_STATIC)
	PHP_FE_END
};

/**
 * Dao\Tag\Select initializer
 */
DAO_INIT_CLASS(Dao_Tag_Select){

	DAO_REGISTER_CLASS(Dao\\Tag, Select, tag_select, dao_tag_select_method_entry, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	return SUCCESS;
}

/**
 * Generates a SELECT tag
 *
 * @param array $parameters
 * @param array $data
 */
PHP_METHOD(Dao_Tag_Select, selectField)
{
	zval *parameters, *data = NULL, params = {}, default_params = {}, id = {}, name = {}, value = {}, use_empty = {}, empty_value = {};
	zval empty_text = {}, code = {}, close_option = {}, options = {}, using = {}, callback = {}, label = {};

	dao_fetch_params(1, 1, 1, &parameters, &data);

	if (!data) {
		data = &DAO_GLOBAL(z_null);
	}

	if (Z_TYPE_P(parameters) != IS_ARRAY) {
		array_init_size(&params, 2);
		dao_array_append(&params, parameters, PH_COPY);
		dao_array_append(&params, data, PH_COPY);
	} else {
		ZVAL_DUP(&params, parameters);
	}
	DAO_MM_ADD_ENTRY(&params);

	dao_read_static_property_ce(&default_params, dao_tag_ce, SL("_defaultParams"), PH_READONLY);
	if (Z_TYPE(default_params) == IS_ARRAY) {
		dao_array_merge_recursive_n2(&params, &default_params, PH_COPY);
	}

	dao_read_static_property_ce(&default_params, dao_tag_ce, SL("_defaultFormParams"), PH_READONLY);
	if (Z_TYPE(default_params) == IS_ARRAY) {
		dao_array_merge_recursive_n2(&params, &default_params, PH_COPY);
	}

	if (!dao_array_isset_fetch_str(&id, &params, SL("id"), PH_READONLY)) {
		if (!dao_array_isset_fetch_long(&id, &params, 0, PH_READONLY)) {
			ZVAL_NULL(&id);
		} else {
			dao_array_update_str(&params, SL("id"), &id, PH_COPY);
		}
	}

	if (!dao_array_isset_fetch_str(&name, &params, SL("name"), PH_READONLY)) {
		DAO_MM_ZVAL_COPY(&name, &id);
		dao_array_update_str(&params, SL("name"), &name, PH_COPY);
	}

	if (!dao_array_isset_fetch_str(&value, &params, SL("value"), PH_COPY)) {
		DAO_MM_CALL_CE_STATIC(&value, dao_tag_ce, "getvalue", &id, &params);
	} else {
		dao_array_unset_str(&params, SL("value"), 0);
		if (Z_TYPE(value) == IS_NULL) {
			DAO_MM_CALL_CE_STATIC(&value, dao_tag_ce, "getvalue", &id, &params);
		}
	}
	DAO_MM_ADD_ENTRY(&value);

	if (dao_array_isset_fetch_str(&use_empty, &params, SL("useEmpty"), PH_READONLY) && zend_is_true(&use_empty)) {
		if (!dao_array_isset_fetch_str(&empty_value, &params, SL("emptyValue"), PH_COPY)) {
			ZVAL_EMPTY_STRING(&empty_value);
		} else {
			dao_array_unset_str(&params, SL("emptyValue"), 0);
		}
		DAO_MM_ADD_ENTRY(&empty_value);

		if (!dao_array_isset_fetch_str(&empty_text, &params, SL("emptyText"), PH_COPY)) {
			ZVAL_STRING(&empty_text, "Choose...");
		} else {
			dao_array_unset_str(&params, SL("emptyText"), 0);
		}
		DAO_MM_ADD_ENTRY(&empty_text);

		dao_array_unset_str(&params, SL("useEmpty"), 0);
	}

	if (dao_array_isset_fetch_str(&using, &params, SL("using"), PH_COPY)) {
		dao_array_unset_str(&params, SL("using"), 0);
		DAO_MM_ADD_ENTRY(&using);
	} else {
		ZVAL_NULL(&using);
	}

	if (dao_array_isset_fetch_str(&callback, &params, SL("callback"), PH_COPY)) {
		dao_array_unset_str(&params, SL("callback"), 0);
		DAO_MM_ADD_ENTRY(&callback);
		if (Z_TYPE(callback) != IS_NULL && !dao_is_callable(&callback)) {
			DAO_MM_THROW_EXCEPTION_STR(dao_tag_exception_ce, "The 'callback' parameter must be callable");
			return;
		}
	} else {
		ZVAL_NULL(&callback);
	}

	if (dao_array_isset_fetch_str(&label, &params, SL("label"), PH_READONLY) && zend_is_true(&label)) {
		zval label_text = {};
		if (DAO_IS_NOT_EMPTY_STRING(&label)) {
			ZVAL_COPY_VALUE(&label_text, &label);
		} else {
			ZVAL_COPY_VALUE(&label_text, &name);
		}

		if (Z_TYPE(label) == IS_ARRAY) {
			DAO_SCONCAT_STR(&code, "<label");
			DAO_MM_ADD_ENTRY(&code);
			dao_tag_render_attributes(&code, &label);
			DAO_MM_ADD_ENTRY(&code);
			DAO_SCONCAT_SVS(&code, ">", &label_text, "</label>");
			DAO_MM_ADD_ENTRY(&code);
		} else {
			if (zend_is_true(&id)) {
				DAO_CONCAT_SVSVS(&code, "<label for=\"", &id, "\">", &label_text, "</label>");
			} else {
				DAO_CONCAT_SVS(&code, "<label>", &label_text, "</label>");
			}
			DAO_MM_ADD_ENTRY(&code);
		}
	}

	DAO_SCONCAT_STR(&code, "<select");
	DAO_MM_ADD_ENTRY(&code);

	dao_tag_render_attributes(&code, &params);
	DAO_MM_ADD_ENTRY(&code);

	dao_concat_self_str(&code, SL(">" PHP_EOL));
	DAO_MM_ADD_ENTRY(&code);

	ZVAL_STRING(&close_option, "</option>" PHP_EOL);
	DAO_MM_ADD_ENTRY(&close_option);

	if (zend_is_true(&use_empty)) {
		/**
		 * Create an empty value
		 */
		DAO_SCONCAT_SVSVV(&code, "\t<option value=\"", &empty_value, "\">", &empty_text, &close_option);
		DAO_MM_ADD_ENTRY(&code);
	}

	if (!dao_array_isset_fetch_str(&options, &params, SL("options"), PH_READONLY)) {
		if (!dao_array_isset_fetch_long(&options, &params, 1, PH_READONLY)) {
			ZVAL_COPY_VALUE(&options, data);
		}
	}

	if (Z_TYPE(options) == IS_OBJECT) {
		zval resultset_options = {};
		/**
		 * The options is a resultset
		 */
		if (Z_TYPE(using) < IS_NULL) {
			DAO_MM_THROW_EXCEPTION_STR(dao_tag_exception_ce, "The 'using' parameter is required");
			return;
		}

		if (Z_TYPE(using) != IS_ARRAY && Z_TYPE(using) != IS_OBJECT) {
			DAO_MM_THROW_EXCEPTION_STR(dao_tag_exception_ce, "The 'using' parameter should be an Array");
			return;
		}

		/**
		 * Create the SELECT's option from a resultset
		 */
		DAO_MM_CALL_CE_STATIC(&resultset_options, dao_tag_select_ce, "_optionsfromresultset", &options, &using, &value, &close_option, &callback);
		dao_concat_self(&code, &resultset_options);
		DAO_MM_ADD_ENTRY(&code);
		zval_ptr_dtor(&resultset_options);
	} else if (Z_TYPE(options) == IS_ARRAY) {
		zval array_options = {};
		/**
		 * Create the SELECT's option from an array
		 */
		DAO_MM_CALL_CE_STATIC(&array_options, dao_tag_select_ce, "_optionsfromarray",  &options, &using, &value, &close_option, &callback);
		dao_concat_self(&code, &array_options);
		DAO_MM_ADD_ENTRY(&code);
		zval_ptr_dtor(&array_options);
	} else if (Z_TYPE(options) != IS_NULL) {
		DAO_MM_THROW_EXCEPTION_STR(dao_tag_exception_ce, "Invalid data provided to SELECT helper");
		return;
	}

	dao_concat_self_str(&code, SL("</select>"));
	DAO_MM_ADD_ENTRY(&code);
	RETURN_MM_CTOR(&code);
}

/**
 * Generate the OPTION tags based on a resulset
 *
 * @param Dao\Mvc\Model $resultset
 * @param array $using
 * @param mixed value
 * @param string $closeOption
 */
PHP_METHOD(Dao_Tag_Select, _optionsFromResultset)
{
	zval *resultset, *using, *value, *close_option, *callback, using_zero = {}, using_one = {}, code = {};

	dao_fetch_params(1, 5, 0, &resultset, &using, &value, &close_option, &callback);

	if (Z_TYPE_P(value) != IS_ARRAY) {
		DAO_SEPARATE_PARAM(value);
		convert_to_string(value);
	}

	if (Z_TYPE_P(using) == IS_ARRAY) {
		if (!dao_array_isset_fetch_long(&using_zero, using, 0, PH_COPY)) {
			ZVAL_STRING(&using_zero, "id");
		}
		DAO_MM_ADD_ENTRY(&using_zero);
		if (!dao_array_isset_fetch_long(&using_one, using, 1, PH_READONLY)) {
			ZVAL_COPY_VALUE(&using_one, &using_zero);
		}
	}

	DAO_MM_CALL_METHOD(NULL, resultset, "rewind");

	while (1) {
		zval r0 = {}, option = {}, option_value = {}, option_text = {}, escaped = {}, params = {}, code_option = {};

		DAO_MM_CALL_METHOD(&r0, resultset, "valid");
		DAO_MM_ADD_ENTRY(&r0);
		if (!DAO_IS_NOT_FALSE(&r0)) {
			break;
		}

		/**
		 * Get the current option
		 */
		DAO_MM_CALL_METHOD(&option, resultset, "current");
		DAO_MM_ADD_ENTRY(&option);
		if (Z_TYPE_P(using) == IS_ARRAY) {
			if (Z_TYPE(option) == IS_OBJECT) {
				if (dao_method_exists_ex(&option, SL("readattribute")) == SUCCESS) {
					if (Z_TYPE(using_zero) == IS_OBJECT) {
						array_init(&params);
						dao_array_update_long(&params, 0, &option, PH_COPY);
						DAO_MM_ADD_ENTRY(&params);
						DAO_MM_CALL_USER_FUNC_ARRAY(&option_value, &using_zero, &params);
					} else {
						/**
						 * Read the value attribute from the model
						 */
						DAO_MM_CALL_METHOD(&option_value, &option, "readattribute", &using_zero);
					}
					DAO_MM_ADD_ENTRY(&option_value);

					if (dao_is_callable(&using_one)) {
						array_init(&params);
						dao_array_update_long(&params, 0, &option, PH_COPY);
						DAO_MM_ADD_ENTRY(&params);
						DAO_MM_CALL_USER_FUNC_ARRAY(&option_text, &using_one, &params);
					} else {
						/**
						 * Read the text attribute from the model
						 */
						DAO_MM_CALL_METHOD(&option_text, &option, "readattribute", &using_one);
					}
					DAO_MM_ADD_ENTRY(&option_text);
				} else {
					if (Z_TYPE(using_zero) == IS_OBJECT) {
						array_init(&params);
						dao_array_update_long(&params, 0, &option, PH_COPY);
						DAO_MM_ADD_ENTRY(&params);
						DAO_MM_CALL_USER_FUNC_ARRAY(&option_value, &using_zero, &params);

						DAO_MM_ADD_ENTRY(&option_value);
					} else {
						/**
						 * Read the variable directly from the model/object
						 */
						dao_read_property_zval(&option_value, &option, &using_zero, PH_READONLY);
					}

					if (dao_is_callable(&using_one)) {
						array_init(&params);
						dao_array_update_long(&params, 0, &option, PH_COPY);
						DAO_MM_ADD_ENTRY(&params);
						DAO_MM_CALL_USER_FUNC_ARRAY(&option_text, &using_one, &params);
						DAO_MM_ADD_ENTRY(&option_value);
					} else {
						/**
						 * Read the text directly from the model/object
						 */
						dao_read_property_zval(&option_text, &option, &using_one, PH_READONLY);
					}
				}
			} else if (Z_TYPE(option) == IS_ARRAY) {
				/**
				 * Read the variable directly from the model/object
				 */
				dao_array_fetch(&option_value, &option, &using_zero, PH_NOISY|PH_READONLY);

				/**
				 * Read the text directly from the model/object
				 */
				dao_array_fetch(&option_text, &option, &using_one, PH_NOISY|PH_READONLY);
			} else {
				DAO_MM_THROW_EXCEPTION_STR(dao_tag_exception_ce, "Resultset returned an invalid value");
				return;
			}

			/**
			 * If the value is equal to the option's value we mark it as selected
			 */
			dao_htmlspecialchars(&escaped, &option_value, NULL, NULL);
			DAO_MM_ADD_ENTRY(&escaped);
			if (dao_is_callable(callback)) {
				zval tmp = {};
				array_init(&params);
				dao_array_update_long(&params, 0, &option_value, PH_COPY);
				dao_array_update_long(&params, 1, value, PH_COPY);
				DAO_MM_ADD_ENTRY(&params);
				DAO_MM_CALL_USER_FUNC_ARRAY(&tmp, callback, &params);
				DAO_MM_ADD_ENTRY(&tmp);
				if (zend_is_true(&tmp)) {
					DAO_SCONCAT_SVSVV(&code, "\t<option selected=\"selected\" value=\"", &escaped, "\">", &option_text, close_option);
				} else {
					DAO_SCONCAT_SVSVV(&code, "\t<option value=\"", &escaped, "\">", &option_text, close_option);
				}
			} else if (Z_TYPE_P(value) == IS_ARRAY) {
				if (dao_fast_in_array(&option_value, value)) {
					DAO_SCONCAT_SVSVV(&code, "\t<option selected=\"selected\" value=\"", &escaped, "\">", &option_text, close_option);
				} else {
					DAO_SCONCAT_SVSVV(&code, "\t<option value=\"", &escaped, "\">", &option_text, close_option);
				}
			} else {
				convert_to_string(&option_value);
				if (DAO_IS_IDENTICAL(&option_value, value)) {
					DAO_SCONCAT_SVSVV(&code, "\t<option selected=\"selected\" value=\"", &escaped, "\">", &option_text, close_option);
				} else {
					DAO_SCONCAT_SVSVV(&code, "\t<option value=\"", &escaped, "\">", &option_text, close_option);
				}
			}
			DAO_MM_ADD_ENTRY(&code);
		} else {
			/**
			 * Check if using is a closure
			 */
			if (Z_TYPE_P(using) == IS_OBJECT) {
				array_init(&params);
				dao_array_update_long(&params, 0, &option, PH_COPY);
				DAO_MM_ADD_ENTRY(&params);
				DAO_MM_CALL_USER_FUNC_ARRAY(&code_option, using, &params);

				dao_concat_self(&code, &code_option);
				DAO_MM_ADD_ENTRY(&code);
				zval_ptr_dtor(&code_option);
			}
		}

		DAO_MM_CALL_METHOD(NULL, resultset, "next");
	}

	RETURN_MM_CTOR(&code);
}

/**
 * Generate the OPTION tags based on an array
 *
 * @param Dao\Mvc\ModelInterface $resultset
 * @param array $using
 * @param mixed value
 * @param string $closeOption
 */
PHP_METHOD(Dao_Tag_Select, _optionsFromArray){

	zval *data, *using, *value, *close_option, *callback, using_zero = {}, using_one = {}, code = {}, *option_text;
	zend_string *str_key;
	ulong idx;

	dao_fetch_params(1, 5, 0, &data, &using, &value, &close_option, &callback);

	if (Z_TYPE_P(using) == IS_ARRAY) {
		if (!dao_array_isset_fetch_long(&using_zero, using, 0, PH_READONLY)) {
			DAO_MM_ZVAL_STRING(&using_zero, "id");
		}
		if (!dao_array_isset_fetch_long(&using_one, using, 1, PH_READONLY)) {
			ZVAL_COPY_VALUE(&using_one, &using_zero);
		}
	}

	if (Z_TYPE_P(value) != IS_ARRAY) {
		DAO_SEPARATE_PARAM(value);
		convert_to_string(value);
	}

	ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(data), idx, str_key, option_text) {
		zval option_value = {};
		if (str_key) {
			ZVAL_STR(&option_value, str_key);
		} else {
			ZVAL_LONG(&option_value, idx);
		}

		if (Z_TYPE_P(option_text) == IS_ARRAY) {			
			if (Z_TYPE_P(using) == IS_ARRAY) {
				zval op_text = {}, op_value = {}, escaped = {};

				dao_array_fetch(&op_value, option_text, &using_zero, PH_NOISY|PH_READONLY);
				dao_array_fetch(&op_text, option_text, &using_one, PH_NOISY|PH_READONLY);

				dao_htmlspecialchars(&escaped, &op_value, NULL, NULL);
				DAO_MM_ADD_ENTRY(&escaped);
				if (dao_is_callable(callback)) {
					zval tmp = {}, params = {};
					array_init(&params);
					dao_array_update_long(&params, 0, &op_value, PH_COPY);
					dao_array_update_long(&params, 1, value, PH_COPY);
					DAO_MM_ADD_ENTRY(&params);
					DAO_MM_CALL_USER_FUNC_ARRAY(&tmp, callback, &params);
					DAO_MM_ADD_ENTRY(&tmp);
					if (zend_is_true(&tmp)) {
						DAO_SCONCAT_SVSVV(&code, "\t<option selected=\"selected\" value=\"", &escaped, "\">", &op_text, close_option);
					} else {
						DAO_SCONCAT_SVSVV(&code, "\t<option value=\"", &escaped, "\">", &op_text, close_option);
					}
				} else if (Z_TYPE_P(value) == IS_ARRAY) {
					if (dao_fast_in_array(&op_value, value)) {
						DAO_SCONCAT_SVSVV(&code, "\t<option selected=\"selected\" value=\"", &escaped, "\">", &op_text, close_option);
					} else {
						DAO_SCONCAT_SVSVV(&code, "\t<option value=\"", &escaped, "\">", &op_text, close_option);
					}
					DAO_MM_ADD_ENTRY(&code);
				} else {
					zval v = {};
					ZVAL_DUP(&v, &op_value);
					if (Z_TYPE(v) != IS_STRING) {
						convert_to_string(&v);
					}
					if (DAO_IS_IDENTICAL(&op_value, value)) {
						DAO_SCONCAT_SVSVV(&code, "\t<option selected=\"selected\" value=\"", &escaped, "\">", &op_text, close_option);
					} else {
						DAO_SCONCAT_SVSVV(&code, "\t<option value=\"", &escaped, "\">", &op_text, close_option);
					}
					DAO_MM_ADD_ENTRY(&code);
					zval_ptr_dtor(&v);
				}
			} else {
				zval array_options = {}, escaped = {};
				dao_htmlspecialchars(&escaped, &option_value, NULL, NULL);
				DAO_MM_ADD_ENTRY(&escaped);
				DAO_MM_CALL_CE_STATIC(&array_options, dao_tag_select_ce, "_optionsfromarray", option_text, using, value, close_option, callback);
				DAO_SCONCAT_SVSVS(&code, "\t<optgroup label=\"", &escaped, "\">" PHP_EOL, &array_options, "\t</optgroup>" PHP_EOL);
				DAO_MM_ADD_ENTRY(&code);
				zval_ptr_dtor(&array_options);
			}
		} else {
			zval escaped = {};
			dao_htmlspecialchars(&escaped, &option_value, NULL, NULL);
			DAO_MM_ADD_ENTRY(&escaped);
			if (dao_is_callable(callback)) {
				zval tmp = {}, params = {};
				array_init(&params);
				dao_array_update_long(&params, 0, &option_value, PH_COPY);
				dao_array_update_long(&params, 1, value, PH_COPY);
				DAO_MM_ADD_ENTRY(&params);
				DAO_MM_CALL_USER_FUNC_ARRAY(&tmp, callback, &params);
				DAO_MM_ADD_ENTRY(&tmp);
				if (zend_is_true(&tmp)) {
					DAO_SCONCAT_SVSVV(&code, "\t<option selected=\"selected\" value=\"", &escaped, "\">", option_text, close_option);
				} else {
					DAO_SCONCAT_SVSVV(&code, "\t<option value=\"", &escaped, "\">", option_text, close_option);
				}
			} else if (Z_TYPE_P(value) == IS_ARRAY) {
				if (dao_fast_in_array(&option_value, value)) {
					DAO_SCONCAT_SVSVV(&code, "\t<option selected=\"selected\" value=\"", &escaped, "\">", option_text, close_option);
				} else {
					DAO_SCONCAT_SVSVV(&code, "\t<option value=\"", &escaped, "\">", option_text, close_option);
				}
				DAO_MM_ADD_ENTRY(&code);
			} else {
				zval v = {};
				ZVAL_DUP(&v, &option_value);
				if (Z_TYPE(v) != IS_STRING) {
					convert_to_string(&v);
				}
				if (DAO_IS_IDENTICAL(&v, value)) {
					DAO_SCONCAT_SVSVV(&code, "\t<option selected=\"selected\" value=\"", &escaped, "\">", option_text, close_option);
				} else {
					DAO_SCONCAT_SVSVV(&code, "\t<option value=\"", &escaped, "\">", option_text, close_option);
				}
				DAO_MM_ADD_ENTRY(&code);
				zval_ptr_dtor(&v);
			}
		}
	} ZEND_HASH_FOREACH_END();

	RETURN_MM_CTOR(&code);
}
