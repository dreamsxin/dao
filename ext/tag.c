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
  |          Nikolaos Dimopoulos <nikos@phalconphp.com>                    |
  |          ZhuZongXin <dreamsxin@qq.com>                                 |
  +------------------------------------------------------------------------+
*/

#include "tag.h"
#include "tag/exception.h"
#include "tag/select.h"
#include "di.h"
#include "diinterface.h"
#include "escaperinterface.h"
#include "mvc/urlinterface.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/exception.h"
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/array.h"
#include "kernel/hash.h"
#include "kernel/concat.h"
#include "kernel/operators.h"
#include "kernel/string.h"

#include "interned-strings.h"

/**
 * Dao\Tag
 *
 * Dao\Tag is designed to simplify building of HTML tags.
 * It provides a set of helpers to generate HTML in a dynamic way.
 * This component is an abstract class that you can extend to add more helpers.
 */
zend_class_entry *dao_tag_ce;

PHP_METHOD(Dao_Tag, setDI);
PHP_METHOD(Dao_Tag, getDI);
PHP_METHOD(Dao_Tag, getUrlService);
PHP_METHOD(Dao_Tag, getEscaperService);
PHP_METHOD(Dao_Tag, getAutoescape);
PHP_METHOD(Dao_Tag, setAutoescape);
PHP_METHOD(Dao_Tag, setDefault);
PHP_METHOD(Dao_Tag, setDefaults);
PHP_METHOD(Dao_Tag, hasValue);
PHP_METHOD(Dao_Tag, getValue);
PHP_METHOD(Dao_Tag, resetInput);
PHP_METHOD(Dao_Tag, linkTo);
PHP_METHOD(Dao_Tag, _inputField);
PHP_METHOD(Dao_Tag, _inputFieldChecked);
PHP_METHOD(Dao_Tag, colorField);
PHP_METHOD(Dao_Tag, textField);
PHP_METHOD(Dao_Tag, numericField);
PHP_METHOD(Dao_Tag, rangeField);
PHP_METHOD(Dao_Tag, emailField);
PHP_METHOD(Dao_Tag, dateField);
PHP_METHOD(Dao_Tag, dateTimeField);
PHP_METHOD(Dao_Tag, dateTimeLocalField);
PHP_METHOD(Dao_Tag, monthField);
PHP_METHOD(Dao_Tag, timeField);
PHP_METHOD(Dao_Tag, weekField);
PHP_METHOD(Dao_Tag, passwordField);
PHP_METHOD(Dao_Tag, hiddenField);
PHP_METHOD(Dao_Tag, fileField);
PHP_METHOD(Dao_Tag, checkField);
PHP_METHOD(Dao_Tag, radioField);
PHP_METHOD(Dao_Tag, imageInput);
PHP_METHOD(Dao_Tag, searchField);
PHP_METHOD(Dao_Tag, telField);
PHP_METHOD(Dao_Tag, urlField);
PHP_METHOD(Dao_Tag, submitButton);
PHP_METHOD(Dao_Tag, selectStatic);
PHP_METHOD(Dao_Tag, select);
PHP_METHOD(Dao_Tag, textArea);
PHP_METHOD(Dao_Tag, form);
PHP_METHOD(Dao_Tag, endForm);
PHP_METHOD(Dao_Tag, setTitle);
PHP_METHOD(Dao_Tag, setTitleSeparator);
PHP_METHOD(Dao_Tag, appendTitle);
PHP_METHOD(Dao_Tag, prependTitle);
PHP_METHOD(Dao_Tag, getTitleSeparator);
PHP_METHOD(Dao_Tag, getTitle);
PHP_METHOD(Dao_Tag, stylesheetLink);
PHP_METHOD(Dao_Tag, javascriptInclude);
PHP_METHOD(Dao_Tag, friendlyTitle);
PHP_METHOD(Dao_Tag, setDocType);
PHP_METHOD(Dao_Tag, getDocType);
PHP_METHOD(Dao_Tag, tagHtml);
PHP_METHOD(Dao_Tag, tagHtmlClose);
PHP_METHOD(Dao_Tag, getDefault);
PHP_METHOD(Dao_Tag, getDefaults);
PHP_METHOD(Dao_Tag, setDefaultParams);
PHP_METHOD(Dao_Tag, getDefaultParams);
PHP_METHOD(Dao_Tag, setDefaultFormParams);
PHP_METHOD(Dao_Tag, getDefaultFormParams);
PHP_METHOD(Dao_Tag, choice);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_tag_setdi, 0, 0, 1)
	ZEND_ARG_INFO(0, dependencyInjector)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_tag_setautoescape, 0, 0, 1)
	ZEND_ARG_INFO(0, autoescape)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_tag_setdefault, 0, 0, 2)
	ZEND_ARG_INFO(0, id)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_tag_setdefaults, 0, 0, 1)
	ZEND_ARG_INFO(0, values)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_tag_hasvalue, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_tag_getvalue, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, params)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_tag_linkto, 0, 0, 1)
	ZEND_ARG_INFO(0, parameters)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_tag_generic_field, 0, 0, 1)
	ZEND_ARG_INFO(0, parameters)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_tag_selectstatic, 0, 0, 1)
	ZEND_ARG_INFO(0, parameters)
	ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_tag_select, 0, 0, 1)
	ZEND_ARG_INFO(0, parameters)
	ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_tag_textarea, 0, 0, 1)
	ZEND_ARG_INFO(0, parameters)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_tag_form, 0, 0, 0)
	ZEND_ARG_INFO(0, parameters)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_tag_settitle, 0, 0, 1)
	ZEND_ARG_INFO(0, title)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_tag_settitleseparator, 0, 0, 1)
	ZEND_ARG_INFO(0, separator)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_tag_appendtitle, 0, 0, 1)
	ZEND_ARG_INFO(0, title)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_tag_prependtitle, 0, 0, 1)
	ZEND_ARG_INFO(0, title)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_tag_gettitle, 0, 0, 0)
	ZEND_ARG_INFO(0, tags)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_tag_stylesheetlink, 0, 0, 0)
	ZEND_ARG_INFO(0, parameters)
	ZEND_ARG_INFO(0, local)
	ZEND_ARG_INFO(0, args)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_tag_javascriptinclude, 0, 0, 0)
	ZEND_ARG_INFO(0, parameters)
	ZEND_ARG_INFO(0, local)
	ZEND_ARG_INFO(0, args)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_tag_imageinput, 0, 0, 0)
	ZEND_ARG_INFO(0, parameters)
	ZEND_ARG_INFO(0, local)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_tag_friendlytitle, 0, 0, 1)
	ZEND_ARG_INFO(0, text)
	ZEND_ARG_INFO(0, separator)
	ZEND_ARG_INFO(0, lowercase)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_tag_setdoctype, 0, 0, 1)
	ZEND_ARG_INFO(0, doctype)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_tag_taghtml, 0, 0, 1)
	ZEND_ARG_INFO(0, tagName)
	ZEND_ARG_INFO(0, parameters)
	ZEND_ARG_INFO(0, selfClose)
	ZEND_ARG_INFO(0, onlyStart)
	ZEND_ARG_INFO(0, useEol)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_tag_taghtmlclose, 0, 0, 1)
	ZEND_ARG_INFO(0, tagName)
	ZEND_ARG_INFO(0, useEol)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_tag_getdefault, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_tag_setdefaultparams, 0, 0, 1)
	ZEND_ARG_INFO(0, params)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_tag_setdefaultformparams, 0, 0, 1)
	ZEND_ARG_INFO(0, params)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_tag_choice, 0, 0, 2)
	ZEND_ARG_INFO(0, expression)
	ZEND_ARG_INFO(0, value1)
	ZEND_ARG_INFO(0, value2)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_tag_method_entry[] = {
	PHP_ME(Dao_Tag, setDI, arginfo_dao_tag_setdi, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Tag, getDI, arginfo_empty, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Tag, getUrlService, arginfo_empty, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Tag, getEscaperService, arginfo_empty, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Tag, getAutoescape, arginfo_empty, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Tag, setAutoescape, arginfo_dao_tag_setautoescape, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Tag, setDefault, arginfo_dao_tag_setdefault, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Tag, setDefaults, arginfo_dao_tag_setdefaults, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_MALIAS(Dao_Tag, displayTo, setDefault, arginfo_dao_tag_setdefault, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Tag, hasValue, arginfo_dao_tag_hasvalue, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Tag, getValue, arginfo_dao_tag_getvalue, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Tag, resetInput, arginfo_empty, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Tag, linkTo, arginfo_dao_tag_linkto, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Tag, _inputField, arginfo_empty, ZEND_ACC_STATIC|ZEND_ACC_PROTECTED)
	PHP_ME(Dao_Tag, _inputFieldChecked, arginfo_empty, ZEND_ACC_STATIC|ZEND_ACC_PROTECTED)
	PHP_ME(Dao_Tag, colorField, arginfo_dao_tag_generic_field, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Tag, textField, arginfo_dao_tag_generic_field, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Tag, numericField, arginfo_dao_tag_generic_field, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Tag, rangeField, arginfo_dao_tag_generic_field, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Tag, emailField, arginfo_dao_tag_generic_field, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Tag, dateField, arginfo_dao_tag_generic_field, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Tag, dateTimeField, arginfo_dao_tag_generic_field, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Tag, dateTimeLocalField, arginfo_dao_tag_generic_field, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Tag, monthField, arginfo_dao_tag_generic_field, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Tag, timeField, arginfo_dao_tag_generic_field, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Tag, weekField, arginfo_dao_tag_generic_field, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Tag, passwordField, arginfo_dao_tag_generic_field, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Tag, hiddenField, arginfo_dao_tag_generic_field, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Tag, searchField, arginfo_dao_tag_generic_field, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Tag, telField, arginfo_dao_tag_generic_field, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Tag, urlField, arginfo_dao_tag_generic_field, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Tag, fileField, arginfo_dao_tag_generic_field, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Tag, checkField, arginfo_dao_tag_generic_field, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Tag, radioField, arginfo_dao_tag_generic_field, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Tag, imageInput, arginfo_dao_tag_imageinput, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Tag, submitButton, arginfo_dao_tag_generic_field, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Tag, selectStatic, arginfo_dao_tag_selectstatic, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Tag, select, arginfo_dao_tag_select, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Tag, textArea, arginfo_dao_tag_textarea, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Tag, form, arginfo_dao_tag_form, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Tag, endForm, arginfo_empty, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Tag, setTitle, arginfo_dao_tag_settitle, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Tag, setTitleSeparator, arginfo_dao_tag_settitleseparator, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Tag, appendTitle, arginfo_dao_tag_appendtitle, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Tag, prependTitle, arginfo_dao_tag_prependtitle, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Tag, getTitle, arginfo_dao_tag_gettitle, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Tag, getTitleSeparator, arginfo_empty, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Tag, stylesheetLink, arginfo_dao_tag_stylesheetlink, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Tag, javascriptInclude, arginfo_dao_tag_javascriptinclude, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Tag, friendlyTitle, arginfo_dao_tag_friendlytitle, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Tag, setDocType, arginfo_dao_tag_setdoctype, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Tag, getDocType, arginfo_empty, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Tag, tagHtml, arginfo_dao_tag_taghtml, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Tag, tagHtmlClose, arginfo_dao_tag_taghtmlclose, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Tag, getDefault, arginfo_dao_tag_getdefault, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Tag, getDefaults, arginfo_empty, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Tag, setDefaultParams, arginfo_dao_tag_setdefaultparams, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Tag, getDefaultParams, arginfo_empty, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Tag, setDefaultFormParams, arginfo_dao_tag_setdefaultformparams, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Tag, getDefaultFormParams, arginfo_empty, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Tag, choice, arginfo_dao_tag_choice, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Tag, color, colorField, arginfo_dao_tag_generic_field, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Tag, text, textField, arginfo_dao_tag_generic_field, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Tag, number, numericField, arginfo_dao_tag_generic_field, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Tag, range, rangeField, arginfo_dao_tag_generic_field, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Tag, email, emailField, arginfo_dao_tag_generic_field, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Tag, date, dateField, arginfo_dao_tag_generic_field, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Tag, dateTime, dateTimeField, arginfo_dao_tag_generic_field, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Tag, dateTimeLocal, dateTimeLocalField, arginfo_dao_tag_generic_field, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Tag, month, monthField, arginfo_dao_tag_generic_field, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Tag, time, timeField, arginfo_dao_tag_generic_field, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Tag, week, weekField, arginfo_dao_tag_generic_field, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Tag, password, passwordField, arginfo_dao_tag_generic_field, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Tag, hidden, hiddenField, arginfo_dao_tag_generic_field, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Tag, search, searchField, arginfo_dao_tag_generic_field, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Tag, tel, telField, arginfo_dao_tag_generic_field, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Tag, url, urlField, arginfo_dao_tag_generic_field, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Tag, file, fileField, arginfo_dao_tag_generic_field, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Tag, check, checkField, arginfo_dao_tag_generic_field, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Tag, radio, radioField, arginfo_dao_tag_generic_field, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Tag, image, imageInput, arginfo_dao_tag_generic_field, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Tag, submit, submitButton, arginfo_dao_tag_generic_field, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Tag initializer
 */
DAO_INIT_CLASS(Dao_Tag){

	DAO_REGISTER_CLASS(Dao, Tag, tag, dao_tag_method_entry, 0);

	zend_declare_property_null(dao_tag_ce, SL("_displayValues"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);
	zend_declare_property_null(dao_tag_ce, SL("_documentTitle"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);
	zend_declare_property_null(dao_tag_ce, SL("_documentTitleSeparator"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);
	zend_declare_property_long(dao_tag_ce, SL("_documentType"), 11, ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);
	zend_declare_property_null(dao_tag_ce, SL("_dependencyInjector"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);
	zend_declare_property_null(dao_tag_ce, SL("_dispatcherService"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);
	zend_declare_property_null(dao_tag_ce, SL("_escaperService"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);
	zend_declare_property_bool(dao_tag_ce, SL("_autoEscape"), 1, ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);
	zend_declare_property_null(dao_tag_ce, SL("_defaultParams"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);
	zend_declare_property_null(dao_tag_ce, SL("_defaultFormParams"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);

	zend_declare_class_constant_long(dao_tag_ce, SL("HTML32"), 1);
	zend_declare_class_constant_long(dao_tag_ce, SL("HTML401_STRICT"), 2);
	zend_declare_class_constant_long(dao_tag_ce, SL("HTML401_TRANSITIONAL"), 3);
	zend_declare_class_constant_long(dao_tag_ce, SL("HTML401_FRAMESET"), 4);
	zend_declare_class_constant_long(dao_tag_ce, SL("HTML5"), 5);
	zend_declare_class_constant_long(dao_tag_ce, SL("XHTML10_STRICT"), 6);
	zend_declare_class_constant_long(dao_tag_ce, SL("XHTML10_TRANSITIONAL"), 7);
	zend_declare_class_constant_long(dao_tag_ce, SL("XHTML10_FRAMESET"), 8);
	zend_declare_class_constant_long(dao_tag_ce, SL("XHTML11"), 9);
	zend_declare_class_constant_long(dao_tag_ce, SL("XHTML20"), 10);
	zend_declare_class_constant_long(dao_tag_ce, SL("XHTML5"), 11);

	return SUCCESS;
}

static void dao_tag_get_escaper(zval *escaper, zval *params)
{
	zval autoescape = {};

	if (!dao_array_isset_fetch_str(&autoescape, params, SL("escape"), PH_READONLY)) {
		dao_read_static_property_ce(&autoescape, dao_tag_ce, SL("_autoEscape"), PH_READONLY);
	}

	if (zend_is_true(&autoescape)) {
		if (FAILURE == dao_call_method_with_params(escaper, NULL, dao_tag_ce, dao_fcall_ce, SL("getescaperservice"), 0, NULL)) {
			return;
		}
	}
}

DAO_STATIC void dao_tag_render_attributes(zval *code, zval *attributes)
{
	zval escaper = {}, attrs = {}, v = {}, *value;
	zend_string *key;
	uint i;

	struct str_size_t {
		const char *str;
		uint size;
	};

	static const struct str_size_t order[14] = {
		{ SL("type") },
		{ SL("for") },
		{ SL("src") },
		{ SL("href") },
		{ SL("action") },
		{ SL("id") },
		{ SL("name") },
		{ SL("value") },
		{ SL("class") },
		{ SL("style") },
		{ SL("method") },
		{ SL("enctype") },
		{ SL("placeholder") },
		{ SL("multiple") },
	};

	assert(Z_TYPE_P(attributes) == IS_ARRAY);

	dao_tag_get_escaper(&escaper, attributes);
	if (Z_TYPE(escaper) <= IS_NULL) {
		Z_TRY_ADDREF_P(code);
		return;
	}
	DAO_MM_INIT();
	DAO_MM_ADD_ENTRY(&escaper);
	if (dao_array_isset_fetch_str(&v, attributes, SL("attrs"), PH_READONLY) && Z_TYPE(v) == IS_ARRAY) {
		ZVAL_COPY_VALUE(&attrs, &v);
		DAO_SEPARATE(&attrs);
	} else {
		array_init(&attrs);
	}
	DAO_MM_ADD_ENTRY(&attrs);
	for (i=0; i<sizeof(order)/sizeof(order[0]); ++i) {
		if (dao_array_isset_fetch_str(&v, attributes, order[i].str, order[i].size, PH_READONLY)) {
			dao_array_update_str(&attrs, order[i].str, order[i].size, &v, PH_COPY);
		}
	}

	ZEND_HASH_FOREACH_STR_KEY_VAL(Z_ARRVAL(attrs), key, value) {
		zval tmp = {}, escaped = {};
		if (key && Z_TYPE_P(value) > IS_NULL) {
			ZVAL_STR(&tmp, key);
			if (Z_TYPE(escaper) == IS_OBJECT) {
				DAO_MM_CALL_METHOD(&escaped, &escaper, "escapehtmlattr", value);
				DAO_SCONCAT_SVSVS(code, " ", &tmp, "=\"", &escaped, "\"");
				zval_ptr_dtor(&escaped);
			} else {
				DAO_SCONCAT_SVSVS(code, " ", &tmp, "=\"", value, "\"");
			}
			DAO_MM_ADD_ENTRY(code);
		}
	} ZEND_HASH_FOREACH_END();
	Z_TRY_ADDREF_P(code);
	RETURN_MM();
}

/**
 * Sets the dependency injector container.
 *
 * @param Dao\DiInterface $dependencyInjector
 */
PHP_METHOD(Dao_Tag, setDI){

	zval *dependency_injector;

	dao_fetch_params(0, 1, 0, &dependency_injector);
	DAO_VERIFY_INTERFACE_EX(dependency_injector, dao_diinterface_ce, dao_tag_exception_ce);
	dao_update_static_property_ce(dao_tag_ce, SL("_dependencyInjector"), dependency_injector);
}

/**
 * Internally gets the dependency injector
 *
 * @return Dao\DiInterface
 */
PHP_METHOD(Dao_Tag, getDI){


	dao_read_static_property_ce(return_value, dao_tag_ce, SL("_dependencyInjector"), PH_COPY);
}

/**
 * Return a URL service from the default DI
 *
 * @return Dao\Mvc\UrlInterface
 */
PHP_METHOD(Dao_Tag, getUrlService){

	zval url = {}, dependency_injector = {}, service = {};

	DAO_MM_INIT();

	dao_read_static_property_ce(&dependency_injector, dao_tag_ce, SL("_dependencyInjector"), PH_READONLY);
	if (Z_TYPE(dependency_injector) != IS_OBJECT) {
		DAO_MM_CALL_CE_STATIC(&dependency_injector, dao_di_ce, "getdefault");
		DAO_MM_ADD_ENTRY(&dependency_injector);
		DAO_MM_VERIFY_INTERFACE_EX(&dependency_injector, dao_diinterface_ce, dao_tag_exception_ce);
		dao_update_static_property_ce(dao_tag_ce, SL("_dependencyInjector"), &dependency_injector);
	} else {
		DAO_MM_VERIFY_INTERFACE_EX(&dependency_injector, dao_diinterface_ce, dao_tag_exception_ce);
	}

	ZVAL_STR(&service, IS(url));

	DAO_MM_CALL_METHOD(&url, &dependency_injector, "getshared", &service);
	DAO_MM_ADD_ENTRY(&url);
	DAO_MM_VERIFY_INTERFACE(&url, dao_mvc_urlinterface_ce);

	RETURN_MM_CTOR(&url);
}

/**
 * Returns an Escaper service from the default DI
 *
 * @return Dao\EscaperInterface
 */
PHP_METHOD(Dao_Tag, getEscaperService){

	zval escaper = {}, dependency_injector = {}, service = {};

	DAO_MM_INIT();

	dao_read_static_property_ce(&escaper, dao_tag_ce, SL("_escaperService"), PH_READONLY);
	if (Z_TYPE(escaper) != IS_OBJECT) {
		dao_read_static_property_ce(&dependency_injector, dao_tag_ce, SL("_dependencyInjector"), PH_READONLY);
		if (Z_TYPE(dependency_injector) != IS_OBJECT) {
			DAO_MM_CALL_CE_STATIC(&dependency_injector, dao_di_ce, "getdefault");
			DAO_MM_ADD_ENTRY(&dependency_injector);
		}

		if (Z_TYPE(dependency_injector) != IS_OBJECT) {
			DAO_MM_THROW_EXCEPTION_STR(dao_tag_exception_ce, "A dependency injector container is required to obtain the \"escaper\" service");
			return;
		}

		DAO_MM_VERIFY_INTERFACE(&dependency_injector, dao_diinterface_ce);

		ZVAL_STR(&service, IS(escaper));

		DAO_MM_CALL_METHOD(&escaper, &dependency_injector, "getshared", &service);
		DAO_MM_ADD_ENTRY(&escaper);

		DAO_MM_VERIFY_INTERFACE(&escaper, dao_escaperinterface_ce);
		dao_update_static_property_ce(dao_tag_ce, SL("_escaperService"), &escaper);
	}

	RETURN_MM_CTOR(&escaper);
}

/**
 * Get current autoescape mode
 *
 * @return boolean
 */
PHP_METHOD(Dao_Tag, getAutoescape) {


	dao_read_static_property_ce(return_value, dao_tag_ce, SL("_autoEscape"), PH_COPY);
}

/**
 * Set autoescape mode in generated html
 *
 * @param boolean $autoescape
 */
PHP_METHOD(Dao_Tag, setAutoescape){

	zval *autoescape;

	dao_fetch_params(0, 1, 0, &autoescape);

	dao_update_static_property_ce(dao_tag_ce, SL("_autoEscape"), autoescape);
}

/**
 * Assigns default values to generated tags by helpers
 *
 * <code>
 * //Assigning "peter" to "name" component
 * Dao\Tag::setDefault("name", "peter");
 *
 * //Later in the view
 * echo Dao\Tag::textField("name"); //Will have the value "peter" by default
 * </code>
 *
 * @param string $id
 * @param string $value
 */
PHP_METHOD(Dao_Tag, setDefault){

	zval *id, *value;

	dao_fetch_params(0, 2, 0, &id, &value);

	if (Z_TYPE_P(value) != IS_NULL) {
		if (Z_TYPE_P(value) == IS_ARRAY || Z_TYPE_P(value) == IS_OBJECT) {
			DAO_THROW_EXCEPTION_STR(dao_tag_exception_ce, "Only scalar values can be assigned to UI components");
			return;
		}
	}

	dao_update_static_property_array_multi_ce(dao_tag_ce, SL("_displayValues"), value, SL("z"), 1, id);
}

/**
 * Assigns default values to generated tags by helpers
 *
 * <code>
 * //Assigning "peter" to "name" component
 * Dao\Tag::setDefaults(array("name" => "peter"));
 *
 * //Later in the view
 * echo Dao\Tag::textField("name"); //Will have the value "peter" by default
 * </code>
 *
 * @param array $values
 * @param boolean $merge
 */
PHP_METHOD(Dao_Tag, setDefaults){

	zval *values, *merge = NULL, display_values = {}, merged_values = {};

	dao_fetch_params(0, 1, 1, &values, &merge);

	if (!merge) {
		merge = &DAO_GLOBAL(z_false);
	}

	if (Z_TYPE_P(values) != IS_ARRAY) {
		DAO_THROW_EXCEPTION_STR(dao_tag_exception_ce, "An array is required as default values");
		return;
	}

	if (zend_is_true(merge)) {
		dao_read_static_property_ce(&display_values, dao_tag_ce, SL("_displayValues"), PH_READONLY);
		if (Z_TYPE(display_values) == IS_ARRAY) {
			dao_fast_array_merge(&merged_values, &display_values, values);
			dao_update_static_property_ce(dao_tag_ce, SL("_displayValues"), &merged_values);
			zval_ptr_dtor(&merged_values);
		} else {
			dao_update_static_property_ce(dao_tag_ce, SL("_displayValues"), values);
		}
	} else {
		dao_update_static_property_ce(dao_tag_ce, SL("_displayValues"), values);
	}
}

/**
 * Alias of Dao\Tag::setDefault
 *
 * @param string $id
 * @param string $value
 */
DAO_DOC_METHOD(Dao_Tag, displayTo)

/**
 * Check if a helper has a default value set using Dao\Tag::setDefault or value from $_POST
 *
 * @param string $name
 * @return boolean
 */
PHP_METHOD(Dao_Tag, hasValue){

	zval *name, display_values = {}, *_POST;

	dao_fetch_params(0, 1, 0, &name);

	dao_read_static_property_ce(&display_values, dao_tag_ce, SL("_displayValues"), PH_READONLY);

	/**
	 * Check if there is a predefined value for it
	 */
	if (dao_array_isset(&display_values, name)) {
		RETURN_TRUE;
	}

	/**
	 * Check if there is a post value for the item
	 */
	_POST = dao_get_global_str(SL("_POST"));
	RETURN_BOOL(dao_array_isset(_POST, name));
}

/**
 * Every helper calls this function to check whether a component has a predefined
 * value using Dao\Tag::setDefault or value from $_POST
 *
 * @param string $name
 * @param array $params
 * @return mixed
 */
PHP_METHOD(Dao_Tag, getValue){

	zval *name, *params = NULL, display_values = {}, value = {}, *_POST;

	dao_fetch_params(0, 1, 1, &name, &params);

	if (!params || !dao_array_isset_fetch_str(&value, params, SL("value"), PH_READONLY) || Z_TYPE(value) == IS_NULL) {
		dao_read_static_property_ce(&display_values, dao_tag_ce, SL("_displayValues"), PH_READONLY);

		/**
		 * Check if there is a predefined value for it
		 */
		if (!dao_array_isset_fetch(&value, &display_values, name, PH_READONLY)) {

			/* Check if there is a post value for the item */
			_POST = dao_get_global_str(SL("_POST"));
			if (!dao_array_isset_fetch(&value, _POST, name, PH_READONLY)) {
				RETURN_NULL();
			}
		}
	}

	RETURN_CTOR(&value);
}

/**
 * Resets the request and internal values to avoid those fields will have any default value
 */
PHP_METHOD(Dao_Tag, resetInput){

	dao_update_static_property_empty_array_ce(dao_tag_ce, SL("_displayValues"));
}

/**
 * Builds a HTML A tag using framework conventions
 *
 *<code>
 *	echo Dao\Tag::linkTo('signup/register', 'Register Here!');
 *	echo Dao\Tag::linkTo(array('signup/register', 'Register Here!'));
 *	echo Dao\Tag::linkTo(array('signup/register', 'Register Here!', 'class' => 'btn-primary'));
 *	echo Dao\Tag::linkTo('http://phalconphp.com/', 'Google', FALSE);
 *	echo Dao\Tag::linkTo(array('http://phalconphp.com/', 'Dao Home', FALSE));
 *	echo Dao\Tag::linkTo(array('http://phalconphp.com/', 'Dao Home', 'local' =>FALSE));
 *</code>
 *
 * @param array|string $parameters
 * @param string $text
 * @param boolean $local
 * @return string
 */
PHP_METHOD(Dao_Tag, linkTo){

	zval *parameters, *text = NULL, *local = NULL,  params = {}, default_params = {}, action = {}, link_text = {}, z_local = {}, query = {}, url = {}, internal_url = {}, code = {};

	dao_fetch_params(1, 1, 2, &parameters, &text, &local);

	if (!text) {
		text = &DAO_GLOBAL(z_null);
	}

	if (!local) {
		local = &DAO_GLOBAL(z_true);
	}

	if (Z_TYPE_P(parameters) != IS_ARRAY) {
		array_init_size(&params, 3);
		DAO_MM_ADD_ENTRY(&params);
		dao_array_append(&params, parameters, PH_COPY);
		dao_array_append(&params, text, PH_COPY);
		dao_array_append(&params, local, PH_COPY);
	} else {
		DAO_MM_ZVAL_DUP(&params, parameters);
	}

	dao_read_static_property_ce(&default_params, dao_tag_ce, SL("_defaultParams"), PH_READONLY);
	if (Z_TYPE(default_params) == IS_ARRAY) {
		dao_array_merge_recursive_n2(&params, &default_params, PH_COPY);
	}

	if (!dao_array_isset_fetch_long(&action, &params, 0, PH_COPY)) {
		if (dao_array_isset_fetch_str(&action, &params, SL("action"), PH_COPY)) {
			DAO_MM_ADD_ENTRY(&action);
			dao_array_unset_str(&params, SL("action"), 0);
		} else {
			DAO_MM_ZVAL_EMPTY_STRING(&action);
		}
	} else {
		DAO_MM_ADD_ENTRY(&action);
		dao_array_unset_long(&params, 0, 0);
	}

	if (!dao_array_isset_fetch_long(&link_text, &params, 1, PH_COPY)) {
		if (dao_array_isset_fetch_str(&link_text, &params, SL("text"), PH_COPY)) {
			DAO_MM_ADD_ENTRY(&link_text);
			dao_array_unset_str(&params, SL("text"), 0);
		} else {
			DAO_MM_ZVAL_EMPTY_STRING(&link_text);
		}
	} else {
		DAO_MM_ADD_ENTRY(&link_text);
		dao_array_unset_long(&params, 1, 0);
	}

	if (!dao_array_isset_fetch_long(&z_local, &params, 2, PH_COPY)) {
		if (dao_array_isset_fetch_str(&z_local, &params, SL("local"), PH_COPY)) {
			DAO_MM_ADD_ENTRY(&z_local);
			dao_array_unset_str(&params, SL("local"), 0);
		} else {
			ZVAL_NULL(&z_local);
		}
	} else {
		DAO_MM_ADD_ENTRY(&z_local);
		dao_array_unset_long(&params, 2, 0);
	}

	if (dao_array_isset_fetch_str(&query, &params, SL("query"), PH_COPY)) {
		DAO_MM_ADD_ENTRY(&query);
		dao_array_unset_str(&params, SL("query"), 0);
	} else {
		ZVAL_NULL(&query);
	}

	DAO_MM_CALL_CE_STATIC(&url, dao_tag_ce, "geturlservice");
	DAO_MM_ADD_ENTRY(&url);
	DAO_MM_CALL_METHOD(&internal_url, &url, "get", &action, &query, &z_local);
	dao_array_update_str(&params, SL("href"), &internal_url, 0);

	ZVAL_STRING(&code, "<a");

	dao_tag_render_attributes(&code, &params);
	if (EG(exception)) {
		zval_ptr_dtor(&code);
		RETURN_MM();;
	}

	DAO_CONCAT_VSVS(return_value, &code, ">", &link_text, "</a>");
	zval_ptr_dtor(&code);
	RETURN_MM();
}

/**
 * Builds generic INPUT tags
 *
 * @param string $type
 * @param array $parameters
 * @param boolean $asValue
 * @return string
 */
PHP_METHOD(Dao_Tag, _inputField){

	zval *type, *parameters, *as_value = NULL, params = {}, default_params = {}, value = {}, id = {}, name = {}, code = {}, doctype = {};
	zval label = {};

	dao_fetch_params(1, 2, 1, &type, &parameters, &as_value);

	if (Z_TYPE_P(parameters) != IS_ARRAY) {
		array_init_size(&params, 1);
		DAO_MM_ADD_ENTRY(&params);
		dao_array_append(&params, parameters, PH_COPY);
	} else {
		DAO_MM_ZVAL_DUP(&params, parameters);
	}

	if (!as_value) {
		as_value = &DAO_GLOBAL(z_false);
	}

	dao_read_static_property_ce(&default_params, dao_tag_ce, SL("_defaultParams"), PH_READONLY);
	if (Z_TYPE(default_params) == IS_ARRAY) {
		dao_array_merge_recursive_n2(&params, &default_params, PH_COPY);
	}

	if (DAO_IS_FALSE(as_value)) {
		if (!dao_array_isset_fetch_str(&id, &params, SL("id"), PH_READONLY) || !zend_is_true(&id)) {
			if (!dao_array_isset_fetch_long(&id, &params, 0, PH_READONLY)) {
				ZVAL_NULL(&id);
			} else if (!dao_array_isset_str(&params, SL("id")) && Z_TYPE(id) == IS_STRING) {
				/**
				 * Automatically assign the id if the name is not an array
				 */
				if (!dao_memnstr_str(&id, SL("["))) {
					dao_array_update_str(&params, SL("id"), &id, PH_COPY);
				}
			}
		}

		if (!dao_array_isset_fetch_str(&name, &params, SL("name"), PH_READONLY)) {
			ZVAL_NULL(&name);
		}

		if (!zend_is_true(&name) && !zend_is_true(&id)) {
			DAO_MM_THROW_EXCEPTION_STR(dao_tag_exception_ce, "id or name must be specified");
			return;
		} else if (!zend_is_true(&name)) {
			ZVAL_COPY_VALUE(&name, &id);
			dao_array_update_string(&params, IS(name), &name, PH_COPY);
		}

		if (zend_is_true(&id)) {
			DAO_MM_CALL_CE_STATIC(&value, dao_tag_ce, "getvalue", &id, &params);
		} else {
			DAO_MM_CALL_CE_STATIC(&value, dao_tag_ce, "getvalue", &name, &params);
		}
		dao_array_update_string(&params, IS(value), &value, 0);

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
	} else {
		/**
		 * Use the 'id' as value if the user hadn't set it
		 */
		if (!dao_array_isset_str(&params, SL("value"))) {
			if (dao_array_isset_fetch_long(&value, &params, 0, PH_READONLY)) {
				dao_array_update_string(&params, IS(value), &value, PH_COPY);
			}
		}
	}

	dao_array_update_string(&params, IS(type), type, PH_COPY);

	DAO_SCONCAT_STR(&code, "<input");
	DAO_MM_ADD_ENTRY(&code);

	dao_tag_render_attributes(&code, &params);
	DAO_MM_ADD_ENTRY(&code);

	dao_read_static_property_ce(&doctype, dao_tag_ce, SL("_documentType"), PH_READONLY);

	/**
	 * Check if Doctype is XHTML
	 */
	if (DAO_GT_LONG(&doctype, 5)) {
		DAO_CONCAT_VS(return_value, &code, " />");
	} else {
		DAO_CONCAT_VS(return_value, &code, ">");
	}

	RETURN_MM();
}

/**
 * Builds INPUT tags that implements the checked attribute
 *
 * @param   string $type
 * @param array $parameters
 * @return string
 */
PHP_METHOD(Dao_Tag, _inputFieldChecked){

	zval *type, *parameters, params = {}, default_params = {}, value = {}, id = {}, name = {}, current_value = {}, code = {}, doctype = {};

	dao_fetch_params(0, 2, 0, &type, &parameters);

	if (Z_TYPE_P(parameters) != IS_ARRAY) {
		array_init_size(&params, 1);
		dao_array_append(&params, parameters, PH_COPY);
	} else {
		ZVAL_DUP(&params, parameters);
	}

	dao_read_static_property_ce(&default_params, dao_tag_ce, SL("_defaultParams"), PH_READONLY);
	if (Z_TYPE(default_params) == IS_ARRAY) {
		dao_array_merge_recursive_n2(&params, &default_params, PH_COPY);
	}

	if (!dao_array_isset_fetch_long(&id, &params, 0, PH_READONLY)) {
		dao_array_fetch_str(&id, &params, SL("id"), PH_NOISY|PH_READONLY);
		dao_array_update_long(&params, 0, &id, PH_COPY);
	}

	if (!dao_array_isset_fetch_str(&name, &params, SL("name"), PH_READONLY)) {
		dao_array_update_string(&params, IS(name), &id, PH_COPY);
	} else {
		if (!zend_is_true(&name)) {
			dao_array_update_string(&params, IS(name), &id, PH_COPY);
		}
	}

	/**
	 * Automatically assign the id if the name is not an array
	 */
	if (!dao_memnstr_str(&id, SL("["))) {
		if (!dao_array_isset_str(&params, SL("id"))) {
			dao_array_update_str(&params, SL("id"), &id, PH_COPY);
		}
	}

	/**
	 * Automatically check inputs
	 */
	if (dao_array_isset_fetch_str(&current_value, &params, SL("value"), PH_COPY)) {
		dao_array_unset_str(&params, SL("value"), 0);

		DAO_CALL_CE_STATIC(&value, dao_tag_ce, "getvalue", &id, &params);

		if (Z_TYPE(value) != IS_NULL && DAO_IS_EQUAL(&current_value, &value)) {
			dao_array_update_str_str(&params, SL("checked"), SL("checked"), 0);
		}
		zval_ptr_dtor(&value);

		dao_array_update_string(&params, IS(value), &current_value, 0);
	} else {
		DAO_CALL_CE_STATIC(&value, dao_tag_ce, "getvalue", &id, &params);

		/**
		 * Evaluate the value in POST
		 */
		if (zend_is_true(&value)) {
			dao_array_update_str_str(&params, SL("checked"), SL("checked"), 0);
		}

		dao_array_update_string(&params, IS(value), &value, 0);
	}

	dao_array_update_string(&params, IS(type), type, PH_COPY);

	ZVAL_STRING(&code, "<input");

	dao_tag_render_attributes(&code, &params);
	zval_ptr_dtor(&params);

	dao_read_static_property_ce(&doctype, dao_tag_ce, SL("_documentType"), PH_READONLY);

	/**
	 * Check if Doctype is XHTML
	 */
	if (DAO_GT_LONG(&doctype, 5)) {
		DAO_CONCAT_VS(return_value, &code, " />");
	} else {
		DAO_CONCAT_VS(return_value, &code, ">");
	}
	zval_ptr_dtor(&code);
}

static void dao_tag_generic_field(INTERNAL_FUNCTION_PARAMETERS, const char* type, int as_value)
{
	zval *parameters, field_type = {};

	dao_fetch_params(1, 1, 0, &parameters);

	DAO_MM_ZVAL_STRING(&field_type, type);
	if (as_value) {
		DAO_MM_CALL_CE_STATIC(return_value, dao_tag_ce, "_inputfield", &field_type, parameters, &DAO_GLOBAL(z_true));
	} else {
		DAO_MM_CALL_CE_STATIC(return_value, dao_tag_ce, "_inputfield", &field_type, parameters);
	}
	RETURN_MM();
}

static void dao_tag_generic_field_checked(INTERNAL_FUNCTION_PARAMETERS, const char* type)
{
	zval *parameters, field_type = {};

	dao_fetch_params(1, 1, 0, &parameters);

	DAO_MM_ZVAL_STRING(&field_type, type);
	DAO_MM_CALL_CE_STATIC(return_value, dao_tag_ce, "_inputfieldchecked", &field_type, parameters);
	RETURN_MM();
}

/**
 * Builds a HTML input[type="color"] tag
 *
 * @param array $parameters
 * @return string
 */
PHP_METHOD(Dao_Tag, colorField){

	dao_tag_generic_field(INTERNAL_FUNCTION_PARAM_PASSTHRU, "color", 0);
}

/**
 * Builds a HTML input[type="text"] tag
 *
 * <code>
 *	echo Dao\Tag::textField(array("name", "size" => 30));
 * </code>
 *
 * @param array $parameters
 * @return string
 */
PHP_METHOD(Dao_Tag, textField){

	dao_tag_generic_field(INTERNAL_FUNCTION_PARAM_PASSTHRU, "text", 0);
}

/**
 * Builds a HTML input[type="number"] tag
 *
 * <code>
 *	echo Dao\Tag::numericField(array("price", "min" => "1", "max" => "5"));
 * </code>
 *
 * @param array $parameters
 * @return string
 */
PHP_METHOD(Dao_Tag, numericField){

	dao_tag_generic_field(INTERNAL_FUNCTION_PARAM_PASSTHRU, "number", 0);
}

/**
 * Builds a HTML input[type="range"] tag
 *
 * @param array $parameters
 * @return string
 */
PHP_METHOD(Dao_Tag, rangeField){

	dao_tag_generic_field(INTERNAL_FUNCTION_PARAM_PASSTHRU, "range", 0);
}

/**
 * Builds a HTML input[type="email"] tag
 *
 * <code>
 *	echo Dao\Tag::emailField("email");
 * </code>
 *
 * @param array $parameters
 * @return string
 */
PHP_METHOD(Dao_Tag, emailField){

	dao_tag_generic_field(INTERNAL_FUNCTION_PARAM_PASSTHRU, "email", 0);
}

/**
 * Builds a HTML input[type="date"] tag
 *
 * <code>
 *	echo Dao\Tag::dateField(array("born", "value" => "14-12-1980"))
 * </code>
 *
 * @param array $parameters
 * @return string
 */
PHP_METHOD(Dao_Tag, dateField){

	dao_tag_generic_field(INTERNAL_FUNCTION_PARAM_PASSTHRU, "date", 0);
}

/**
 * Builds a HTML input[type="datetime"] tag
 *
 * @param array $parameters
 * @return string
 */
PHP_METHOD(Dao_Tag, dateTimeField){

	dao_tag_generic_field(INTERNAL_FUNCTION_PARAM_PASSTHRU, "datetime", 0);
}

/**
 * Builds a HTML input[type="datetime-local"] tag
 *
 * @param array $parameters
 * @return string
 */
PHP_METHOD(Dao_Tag, dateTimeLocalField){

	dao_tag_generic_field(INTERNAL_FUNCTION_PARAM_PASSTHRU, "datetime-local", 0);
}

/**
 * Builds a HTML input[type="month"] tag
 *
 * @param array $parameters
 * @return string
 */
PHP_METHOD(Dao_Tag, monthField){

	dao_tag_generic_field(INTERNAL_FUNCTION_PARAM_PASSTHRU, "month", 0);
}

/**
 * Builds a HTML input[type="time"] tag
 *
 * @param array $parameters
 * @return string
 */
PHP_METHOD(Dao_Tag, timeField){

	dao_tag_generic_field(INTERNAL_FUNCTION_PARAM_PASSTHRU, "time", 0);
}

/**
 * Builds a HTML input[type="week"] tag
 *
 * @param array $parameters
 * @return string
 */
PHP_METHOD(Dao_Tag, weekField){

	dao_tag_generic_field(INTERNAL_FUNCTION_PARAM_PASSTHRU, "week", 0);
}

/**
 * Builds a HTML input[type="password"] tag
 *
 *<code>
 * echo Dao\Tag::passwordField(array("name", "size" => 30));
 *</code>
 *
 * @param array $parameters
 * @return string
 */
PHP_METHOD(Dao_Tag, passwordField){

	dao_tag_generic_field(INTERNAL_FUNCTION_PARAM_PASSTHRU, "password", 0);
}

/**
 * Builds a HTML input[type="hidden"] tag
 *
 *<code>
 * echo Dao\Tag::hiddenField(array("name", "value" => "mike"));
 *</code>
 *
 * @param array $parameters
 * @return string
 */
PHP_METHOD(Dao_Tag, hiddenField){

	dao_tag_generic_field(INTERNAL_FUNCTION_PARAM_PASSTHRU, "hidden", 0);
}

/**
 * Builds a HTML input[type="file"] tag
 *
 *<code>
 * echo Dao\Tag::fileField("file");
 *</code>
 *
 * @param array $parameters
 * @return string
 */
PHP_METHOD(Dao_Tag, fileField){

	dao_tag_generic_field(INTERNAL_FUNCTION_PARAM_PASSTHRU, "file", 0);
}

/**
 * Builds a HTML input[type="search"] tag
 *
 * @param array $parameters
 * @return string
 */
PHP_METHOD(Dao_Tag, searchField){

	dao_tag_generic_field(INTERNAL_FUNCTION_PARAM_PASSTHRU, "search", 0);
}

/**
 * Builds a HTML input[type="tel"] tag
 *
 * @param array $parameters
 * @return string
 */
PHP_METHOD(Dao_Tag, telField){

	dao_tag_generic_field(INTERNAL_FUNCTION_PARAM_PASSTHRU, "tel", 0);
}

/**
 * Builds a HTML input[type="url"] tag
 *
 * @param array $parameters
 * @return string
 */
PHP_METHOD(Dao_Tag, urlField){

	dao_tag_generic_field(INTERNAL_FUNCTION_PARAM_PASSTHRU, "url", 0);
}

/**
 * Builds a HTML input[type="check"] tag
 *
 *<code>
 * echo Dao\Tag::checkField(array("terms", "value" => "Y"));
 *</code>
 *
 * @param array $parameters
 * @return string
 */
PHP_METHOD(Dao_Tag, checkField){

	dao_tag_generic_field_checked(INTERNAL_FUNCTION_PARAM_PASSTHRU, "checkbox");
}

/**
 * Builds a HTML input[type="radio"] tag
 *
 *<code>
 * echo Dao\Tag::radioField(array("wheather", "value" => "hot"))
 *</code>
 *
 * Volt syntax:
 *<code>
 * {{ radio_field('Save') }}
 *</code>
 *
 * @param array $parameters
 * @return string
 */
PHP_METHOD(Dao_Tag, radioField){

	dao_tag_generic_field_checked(INTERNAL_FUNCTION_PARAM_PASSTHRU, "radio");
}

/**
 * Builds a HTML input[type="submit"] tag
 *
 *<code>
 * echo Dao\Tag::submitButton("Save")
 *</code>
 *
 * Volt syntax:
 *<code>
 * {{ submit_button('Save') }}
 *</code>
 *
 * @param array $parameters
 * @return string
 */
PHP_METHOD(Dao_Tag, submitButton){

	dao_tag_generic_field(INTERNAL_FUNCTION_PARAM_PASSTHRU, "submit", 1);
}

/**
 * Builds a HTML SELECT tag using a PHP array for options
 *
 *<code>
 *	echo Dao\Tag::selectStatic("status", array("A" => "Active", "I" => "Inactive"))
 *</code>
 *
 * @param array $parameters
 * @param   array $data
 * @return string
 */
PHP_METHOD(Dao_Tag, selectStatic){

	zval *parameters, *data = NULL;

	dao_fetch_params(0, 1, 1, &parameters, &data);

	if (!data) {
		data = &DAO_GLOBAL(z_null);
	}

	DAO_RETURN_CALL_CE_STATIC(dao_tag_select_ce, "selectfield", parameters, data);
}

/**
 * Builds a HTML SELECT tag using a Dao\Mvc\Model resultset as options
 *
 *<code>
 *	echo Dao\Tag::select(array(
 *		"robotId",
 *		Robots::find("type = 'mechanical'"),
 *		"using" => array("id", "name")
 * 	));
 *</code>
 *
 * Volt syntax:
 *<code>
 * {{ select("robotId", robots, "using": ["id", "name"]) }}
 *</code>
 *
 * @param array $parameters
 * @param   array $data
 * @return string
 */
PHP_METHOD(Dao_Tag, select){

	zval *parameters, *data = NULL;

	dao_fetch_params(0, 1, 1, &parameters, &data);

	if (!data) {
		data = &DAO_GLOBAL(z_null);
	}

	DAO_RETURN_CALL_CE_STATIC(dao_tag_select_ce, "selectfield", parameters, data);
}

/**
 * Builds a HTML TEXTAREA tag
 *
 *<code>
 * echo Dao\Tag::textArea(array("comments", "cols" => 10, "rows" => 4))
 *</code>
 *
 * Volt syntax:
 *<code>
 * {{ text_area("comments", "cols": 10, "rows": 4) }}
 *</code>
 *
 * @param array $parameters
 * @return string
 */
PHP_METHOD(Dao_Tag, textArea){

	zval *parameters, params = {}, default_params = {}, id = {}, name = {}, content = {}, code = {}, escaper = {}, escaped = {};

	dao_fetch_params(0, 1, 0, &parameters);

	if (Z_TYPE_P(parameters) != IS_ARRAY) {
		array_init_size(&params, 1);
		dao_array_append(&params, parameters, PH_COPY);
	} else {
		ZVAL_DUP(&params, parameters);
	}

	dao_read_static_property_ce(&default_params, dao_tag_ce, SL("_defaultParams"), PH_READONLY);
	if (Z_TYPE(default_params) == IS_ARRAY) {
		dao_array_merge_recursive_n2(&params, &default_params, PH_COPY);
	}

	if (!dao_array_isset_fetch_long(&id, &params, 0, PH_READONLY)) {
		if (dao_array_isset_fetch_str(&id, &params, SL("id"), PH_READONLY)) {
			dao_array_update_long(&params, 0, &id, PH_COPY);
		}
	}

	if (!dao_array_isset_fetch_str(&name, &params, SL("name"), PH_READONLY)) {
		dao_array_update_string(&params, IS(name), &id, PH_COPY);
	} else {
		if (!zend_is_true(&name)) {
			dao_array_update_string(&params, IS(name), &id, PH_COPY);
		}
	}

	if (!dao_array_isset_str(&params, SL("id"))) {
		dao_array_update_str(&params, SL("id"), &id, PH_COPY);
	}

	DAO_CALL_CE_STATIC(&content, dao_tag_ce, "getvalue", &id, &params);

	dao_tag_get_escaper(&escaper, &params);

	if (Z_TYPE(escaper) == IS_OBJECT) {
		DAO_CALL_METHOD(&escaped, &escaper, "escapehtml", &content);
	} else {
		ZVAL_COPY(&escaped, &content);
	}
	zval_ptr_dtor(&escaper);
	zval_ptr_dtor(&content);

	ZVAL_STRING(&code, "<textarea");

	dao_tag_render_attributes(&code, &params);

	DAO_CONCAT_VSVS(return_value, &code, ">", &escaped, "</textarea>");
	zval_ptr_dtor(&escaped);
	zval_ptr_dtor(&code);
}

/**
 * Builds a HTML FORM tag
 *
 * <code>
 * echo Dao\Tag::form("posts/save");
 * echo Dao\Tag::form(array("posts/save", "method" => "post"));
 * </code>
 *
 * Volt syntax:
 * <code>
 * {{ form("posts/save") }}
 * {{ form("posts/save", "method": "post") }}
 * </code>
 *
 * @param array $parameters
 * @return string
 */
PHP_METHOD(Dao_Tag, form){

	zval *args = NULL, params = {}, default_params = {}, params_action = {}, action = {}, parameters = {}, url = {}, code = {};

	dao_fetch_params(0, 0, 1, &args);

	if (args) {
		if (Z_TYPE_P(args) != IS_ARRAY) {
			array_init_size(&params, 1);
			dao_array_append(&params, args, PH_COPY);
		} else {
			ZVAL_DUP(&params, args);
		}
	} else {
		array_init(&params);
	}

	dao_read_static_property_ce(&default_params, dao_tag_ce, SL("_defaultParams"), PH_READONLY);
	if (Z_TYPE(default_params) == IS_ARRAY) {
		dao_array_merge_recursive_n2(&params, &default_params, PH_COPY);
	}

	if (!dao_array_isset_fetch_long(&params_action, &params, 0, PH_READONLY)) {
		if (!dao_array_isset_fetch_str(&params_action, &params, SL("action"), PH_READONLY)) {
			ZVAL_NULL(&params_action);
		}
	}

	/**
	 * By default the method is POST
	 */
	if (!dao_array_isset_str(&params, SL("method"))) {
		dao_array_update_str_str(&params, SL("method"), SL("post"), 0);
	}

	if (Z_TYPE(params_action) != IS_NULL) {
		DAO_CALL_CE_STATIC(&url, dao_tag_ce, "geturlservice");
		DAO_CALL_METHOD(&action, &url, "get", &params_action);
		zval_ptr_dtor(&url);
	}

	/**
	 * Check for extra parameters
	 */
	if (dao_array_isset_fetch_str(&parameters, &params, SL("parameters"), PH_READONLY)) {
		DAO_SCONCAT_SV(&action, "?", &parameters);
	}

	if (Z_TYPE(action) != IS_NULL) {
		dao_array_update_str(&params, SL("action"), &action, PH_COPY);
		zval_ptr_dtor(&action);
	}

	ZVAL_STRING(&code, "<form");

	dao_tag_render_attributes(&code, &params);
	zval_ptr_dtor(&params);

	DAO_CONCAT_VS(return_value, &code, ">");
	zval_ptr_dtor(&code);
}

/**
 * Builds a HTML close FORM tag
 *
 * @return string
 */
PHP_METHOD(Dao_Tag, endForm){


	RETURN_STRING("</form>");
}

/**
 * Set the title of view content
 *
 *<code>
 * Dao\Tag::setTitle('Welcome to my Page');
 *</code>
 *
 * @param string $title
 */
PHP_METHOD(Dao_Tag, setTitle){

	zval *title;

	dao_fetch_params(0, 1, 0, &title);

	dao_update_static_property_ce(dao_tag_ce, SL("_documentTitle"), title);

}

/**
 * Set the title separator of view content
 *
 *<code>
 * Dao\Tag::setTitleSeparator('-');
 *</code>
 *
 * @param string $titleSeparator
 */
PHP_METHOD(Dao_Tag, setTitleSeparator){

	zval *title_separator;

	dao_fetch_params(0, 1, 0, &title_separator);

	dao_update_static_property_ce(dao_tag_ce, SL("_documentTitleSeparator"), title_separator);

}

/**
 * Appends a text to current document title
 *
 * @param string $title
 */
PHP_METHOD(Dao_Tag, appendTitle){

	zval *title, document_title = {}, document_title_separator = {}, s = {};

	dao_fetch_params(0, 1, 0, &title);

	dao_read_static_property_ce(&document_title, dao_tag_ce, SL("_documentTitle"), PH_READONLY);
	dao_read_static_property_ce(&document_title_separator, dao_tag_ce, SL("_documentTitleSeparator"), PH_READONLY);

	DAO_CONCAT_VVV(&s, &document_title, &document_title_separator, title);
	dao_update_static_property_ce(dao_tag_ce, SL("_documentTitle"), &s);
	zval_ptr_dtor(&s);
}

/**
 * Prepends a text to current document title
 *
 * @param string $title
 */
PHP_METHOD(Dao_Tag, prependTitle){

	zval *title, document_title = {}, document_title_separator = {}, s = {};

	dao_fetch_params(0, 1, 0, &title);

	dao_read_static_property_ce(&document_title, dao_tag_ce, SL("_documentTitle"), PH_READONLY);
	dao_read_static_property_ce(&document_title_separator, dao_tag_ce, SL("_documentTitleSeparator"), PH_READONLY);

	DAO_CONCAT_VVV(&s, title, &document_title_separator, &document_title);
	dao_update_static_property_ce(dao_tag_ce, SL("_documentTitle"), &s);
	zval_ptr_dtor(&s);
}

/**
 * Gets the current document title
 *
 * <code>
 * 	echo Dao\Tag::getTitle();
 * </code>
 *
 * <code>
 * 	{{ get_title() }}
 * </code>
 *
 * @return string
 */
PHP_METHOD(Dao_Tag, getTitle){

	zval *tags = NULL, document_title = {};

	dao_fetch_params(0, 0, 1, &tags);

	dao_read_static_property_ce(&document_title, dao_tag_ce, SL("_documentTitle"), PH_READONLY);
	if (!tags || zend_is_true(tags)) {
		DAO_CONCAT_SVS(return_value, "<title>", &document_title, "</title>" PHP_EOL);
	} else {
		RETURN_CTOR(&document_title);
	}
}

/**
 * Gets the current document title separator
 *
 * <code>
 * 	echo Dao\Tag::getTitleSeparator();
 * </code>
 *
 * <code>
 * 	{{ get_title_separator() }}
 * </code>
 *
 * @return string
 */
PHP_METHOD(Dao_Tag, getTitleSeparator)
{


	dao_read_static_property_ce(return_value, dao_tag_ce, SL("_documentTitleSeparator"), PH_COPY);
}

/**
 * Builds a LINK[rel="stylesheet"] tag
 *
 * <code>
 * 	echo Dao\Tag::stylesheetLink("http://fonts.googleapis.com/css?family=Rosario", false);
 * 	echo Dao\Tag::stylesheetLink("css/style.css");
 * </code>
 *
 * Volt Syntax:
 *<code>
 * 	{{ stylesheet_link("http://fonts.googleapis.com/css?family=Rosario", false) }}
 * 	{{ stylesheet_link("css/style.css") }}
 *</code>
 *
 * @param array $parameters
 * @param boolean $local
 * @param array $args
 * @return string
 */
PHP_METHOD(Dao_Tag, stylesheetLink){

	zval *parameters = NULL, *local = NULL, *args = NULL, params = {}, default_params = {};
	zval first_param = {}, z_local = {}, z_args = {}, url = {}, url_href = {}, href = {}, code = {}, doctype = {}, rel = {};

	dao_fetch_params(0, 0, 3, &parameters, &local, &args);

	if (!parameters) {
		parameters = &DAO_GLOBAL(z_null);
	}

	if (!local) {
		local = &DAO_GLOBAL(z_true);
	}

	if (!args) {
		args = &DAO_GLOBAL(z_null);
	}

	if (Z_TYPE_P(parameters) != IS_ARRAY) {
		array_init_size(&params, 3);
		dao_array_append(&params, parameters, PH_COPY);
		dao_array_append(&params, local, PH_COPY);
		dao_array_append(&params, args, PH_COPY);
	} else {
		ZVAL_DUP(&params, parameters);
	}

	dao_read_static_property_ce(&default_params, dao_tag_ce, SL("_defaultParams"), PH_READONLY);
	if (Z_TYPE(default_params) == IS_ARRAY) {
		dao_array_merge_recursive_n2(&params, &default_params, PH_COPY);
	}

	if (!dao_array_isset_str(&params, SL("href"))) {
		if (dao_array_isset_fetch_long(&first_param, &params, 0, PH_READONLY)) {
			dao_array_update_str(&params, SL("href"), &first_param, PH_COPY);
		} else {
			dao_array_update_str_str(&params, SL("href"), SL(""), 0);
		}
	}

	if (!dao_array_isset_fetch_long(&z_local, &params, 1, PH_COPY)) {
		if (dao_array_isset_fetch_str(&z_local, &params, SL("local"), PH_COPY)) {
			dao_array_unset_str(&params, SL("local"), 0);
		} else {
			ZVAL_TRUE(&z_local);
		}
	}

	if (!dao_array_isset_fetch_long(&z_args, &params, 2, PH_COPY)) {
		if (dao_array_isset_fetch_str(&z_args, &params, SL("args"), PH_COPY)) {
			dao_array_unset_str(&params, SL("args"), 0);
		} else {
			ZVAL_TRUE(&z_args);
		}
	}

	if (!dao_array_isset_str(&params, SL("type"))) {
		dao_array_update_string_str(&params, IS(type), SL("text/css"), 0);
	}

	/**
	 * URLs are generated through the 'url' service
	 */
	if (zend_is_true(&z_local)) {
		DAO_CALL_CE_STATIC(&url, dao_tag_ce, "geturlservice");

		dao_array_fetch_str(&url_href, &params, SL("href"), PH_NOISY|PH_READONLY);

		DAO_CALL_METHOD(&href, &url, "getstatic", &url_href, &z_args);
		zval_ptr_dtor(&url);
		dao_array_update_str(&params, SL("href"), &href, 0);
	}
	zval_ptr_dtor(&z_local);
	zval_ptr_dtor(&z_args);

	if (dao_array_isset_fetch_str(&rel, &params, SL("rel"), PH_READONLY)) {
		dao_array_unset_str(&params, SL("rel"), 0);
		DAO_CONCAT_SVS(&code, "<link rel=\"", &rel, "\"");
	} else {
		ZVAL_STRING(&code, "<link rel=\"stylesheet\"");
	}

	dao_tag_render_attributes(&code, &params);
	zval_ptr_dtor(&params);

	dao_read_static_property_ce(&doctype, dao_tag_ce, SL("_documentType"), PH_READONLY);

	/**
	 * Check if Doctype is XHTML
	 */
	if (DAO_GT_LONG(&doctype, 5)) {
		DAO_CONCAT_VS(return_value, &code, " />" PHP_EOL);
	} else {
		DAO_CONCAT_VS(return_value, &code, ">" PHP_EOL);
	}
	zval_ptr_dtor(&code);
}

/**
 * Builds a SCRIPT[type="javascript"] tag
 *
 * <code>
 * 	echo Dao\Tag::javascriptInclude("http://ajax.googleapis.com/ajax/libs/jquery/1.7.1/jquery.min.js", false);
 * 	echo Dao\Tag::javascriptInclude("javascript/jquery.js");
 * </code>
 *
 * Volt syntax:
 * <code>
 * {{ javascript_include("http://ajax.googleapis.com/ajax/libs/jquery/1.7.1/jquery.min.js", false) }}
 * {{ javascript_include("javascript/jquery.js") }}
 * </code>
 *
 * @param array $parameters
 * @param boolean $local
 * @param array $args
 * @return string
 */
PHP_METHOD(Dao_Tag, javascriptInclude){

	zval *parameters = NULL, *local = NULL, *args = NULL, params = {}, default_params = {};
	zval first_param = {}, z_local = {}, z_args = {}, params_src = {}, url = {}, src = {}, code = {};

	dao_fetch_params(1, 0, 3, &parameters, &local, &args);

	if (!parameters) {
		parameters = &DAO_GLOBAL(z_null);
	}

	if (!local) {
		local = &DAO_GLOBAL(z_true);
	}

	if (!args) {
		args = &DAO_GLOBAL(z_null);
	}

	if (Z_TYPE_P(parameters) != IS_ARRAY) {
		array_init_size(&params, 3);
		dao_array_append(&params, parameters, PH_COPY);
		dao_array_append(&params, local, PH_COPY);
		dao_array_append(&params, args, PH_COPY);
		DAO_MM_ADD_ENTRY(&params);
	} else {
		DAO_MM_ZVAL_DUP(&params, parameters);
	}

	dao_read_static_property_ce(&default_params, dao_tag_ce, SL("_defaultParams"), PH_READONLY);
	if (Z_TYPE(default_params) == IS_ARRAY) {
		dao_array_merge_recursive_n2(&params, &default_params, PH_COPY);
	}

	if (!dao_array_isset_str(&params, SL("src"))) {
		if (dao_array_isset_fetch_long(&first_param, &params, 0, PH_READONLY)) {
			dao_array_update_str(&params, SL("src"), &first_param, PH_COPY);
		} else {
			dao_array_update_str_str(&params, SL("src"), SL(""), 0);
		}
	}

	if (!dao_array_isset_fetch_long(&z_local, &params, 1, PH_READONLY)) {
		if (dao_array_isset_fetch_str(&z_local, &params, SL("local"), PH_COPY)) {
			DAO_MM_ADD_ENTRY(&z_local);
			dao_array_unset_str(&params, SL("local"), 0);
		} else {
			ZVAL_TRUE(&z_local);
		}
	}

	if (!dao_array_isset_fetch_long(&z_args, &params, 2, PH_READONLY)) {
		if (dao_array_isset_fetch_str(&z_args, &params, SL("args"), PH_COPY)) {
			DAO_MM_ADD_ENTRY(&z_args);
			dao_array_unset_str(&params, SL("args"), 0);
		} else {
			ZVAL_TRUE(&z_args);
		}
	}

	if (!dao_array_isset_str(&params, SL("type"))) {
		dao_array_update_string_str(&params, IS(type), SL("text/javascript"), 0);
	}

	/**
	 * URLs are generated through the 'url' service
	 */
	if (zend_is_true(&z_local)) {
		DAO_MM_CALL_CE_STATIC(&url, dao_tag_ce, "geturlservice");
		DAO_MM_ADD_ENTRY(&url);

		dao_array_fetch_str(&params_src, &params, SL("src"), PH_NOISY|PH_READONLY);

		DAO_MM_CALL_METHOD(&src, &url, "getstatic", &params_src, &z_args);

		dao_array_update_str(&params, SL("src"), &src, 0);
	}

	ZVAL_STRING(&code, "<script");

	dao_tag_render_attributes(&code, &params);

	DAO_CONCAT_VS(return_value, &code, "></script>" PHP_EOL);
	DAO_MM_ADD_ENTRY(&code);
	RETURN_MM();
}

/**
 * Builds HTML IMG tags
 *
 * <code>
 * 	echo Dao\Tag::image("img/bg.png");
 * 	echo Dao\Tag::image(array("img/photo.jpg", "alt" => "Some Photo"));
 * </code>
 *
 * Volt Syntax:
 * <code>
 * 	{{ image("img/bg.png") }}
 * 	{{ image("img/photo.jpg", "alt": "Some Photo") }}
 * 	{{ image("http://static.mywebsite.com/img/bg.png", false) }}
 * </code>
 *
 * @param  array $parameters
 * @param  boolean $local
 * @return string
 */
PHP_METHOD(Dao_Tag, imageInput){

	zval *parameters = NULL, *local = NULL, params = {}, default_params = {}, first_param = {}, second_param = {};
	zval url = {}, url_src = {}, src = {}, code = {}, doctype = {};

	dao_fetch_params(0, 0, 2, &parameters, &local);

	if (!parameters) {
		parameters = &DAO_GLOBAL(z_null);
	}

	if (!local) {
		local = &DAO_GLOBAL(z_true);
		if (Z_TYPE_P(parameters) == IS_ARRAY && dao_array_isset_fetch_long(&second_param, parameters, 1, PH_READONLY)) {
			if (!zend_is_true(&second_param)) {
				local = &DAO_GLOBAL(z_false);
			}
		}
	}

	if (Z_TYPE_P(parameters) != IS_ARRAY) {
		array_init_size(&params, 1);
		dao_array_append(&params, parameters, PH_COPY);
	} else {
		ZVAL_DUP(&params, parameters);
	}

	dao_read_static_property_ce(&default_params, dao_tag_ce, SL("_defaultParams"), PH_READONLY);
	if (Z_TYPE(default_params) == IS_ARRAY) {
		dao_array_merge_recursive_n2(&params, &default_params, PH_COPY);
	}

	if (!dao_array_isset_str(&params, SL("src"))) {
		if (dao_array_isset_fetch_long(&first_param, &params, 0, PH_READONLY)) {
			dao_array_update_str(&params, SL("src"), &first_param, PH_COPY);
		} else {
			dao_array_update_str_str(&params, SL("src"), SL(""), 0);
		}
	}

	/**
	 * Use the 'url' service if the URI is local
	 */
	if (zend_is_true(local)) {
		DAO_CALL_CE_STATIC(&url, dao_tag_ce, "geturlservice");

		dao_array_fetch_str(&url_src, &params, SL("src"), PH_NOISY|PH_READONLY);

		DAO_CALL_METHOD(&src, &url, "getstatic", &url_src);
		zval_ptr_dtor(&url);
		dao_array_update_str(&params, SL("src"), &src, 0);
	}

	ZVAL_STRING(&code, "<img");

	dao_tag_render_attributes(&code, &params);
	zval_ptr_dtor(&params);

	dao_read_static_property_ce(&doctype, dao_tag_ce, SL("_documentType"), PH_READONLY);

	/**
	 * Check if Doctype is XHTML
	 */
	if (DAO_GT_LONG(&doctype, 5)) {
		DAO_CONCAT_VS(return_value, &code, " />");
	} else {
		DAO_CONCAT_VS(return_value, &code, ">");
	}
	zval_ptr_dtor(&code);
}

/**
 * Converts texts into URL-friendly titles
 *
 *<code>
 * echo Dao\Tag::friendlyTitle('These are big important news', '-')
 *</code>
 *
 * @param string $text
 * @param string $separator
 * @param boolean $lowercase
 * @return text
 */
PHP_METHOD(Dao_Tag, friendlyTitle){

	zval *text, *separator = NULL, *lowercase = NULL, sep = {}, pattern = {}, friendly = {};

	dao_fetch_params(1, 1, 2, &text, &separator, &lowercase);

	if (!separator) {
		DAO_MM_ZVAL_STRING(&sep, "-");
	} else {
		ZVAL_COPY_VALUE(&sep, separator);
	}

	if (!lowercase) {
		lowercase = &DAO_GLOBAL(z_true);
	}

	DAO_MM_ZVAL_STRING(&pattern, "~[^a-z0-9A-Z]+~");

	dao_fast_preg_replace(&friendly, &pattern, &sep, text);
	DAO_MM_ADD_ENTRY(&friendly);
	if (zend_is_true(lowercase)) {
		dao_fast_strtolower(return_value, &friendly);
		RETURN_MM();
	}
	RETURN_MM_CTOR(&friendly);
}

/**
 * Set the document type of content
 *
 * @param string $doctype
 */
PHP_METHOD(Dao_Tag, setDocType){

	zval *doctype;

	dao_fetch_params(0, 1, 0, &doctype);

	dao_update_static_property_ce(dao_tag_ce, SL("_documentType"), doctype);

}

/**
 * Get the document type declaration of content
 *
 * @return string
 */
PHP_METHOD(Dao_Tag, getDocType){

	zval doctype = {};

	dao_read_static_property_ce(&doctype, dao_tag_ce, SL("_documentType"), PH_READONLY);

	switch (dao_get_intval(&doctype)) {
		case 1:  RETURN_STRING("<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 3.2 Final//EN\">" PHP_EOL);
		/* no break */
		case 2:  RETURN_STRING("<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01//EN\"" PHP_EOL "\t\"http://www.w3.org/TR/html4/strict.dtd\">" PHP_EOL);
		/* no break */
		case 3:  RETURN_STRING("<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"" PHP_EOL "\t\"http://www.w3.org/TR/html4/loose.dtd\">" PHP_EOL);
		/* no break */
		case 4:  RETURN_STRING("<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Frameset//EN\"" PHP_EOL"\t\"http://www.w3.org/TR/html4/frameset.dtd\">" PHP_EOL);
		/* no break */
		case 5:  RETURN_STRING("<!DOCTYPE html>" PHP_EOL);
		/* no break */
		case 6:  RETURN_STRING("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\"" PHP_EOL "\t\"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">" PHP_EOL);
		/* no break */
		case 7:  RETURN_STRING("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\"" PHP_EOL "\t\"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">" PHP_EOL);
		/* no break */
		case 8:  RETURN_STRING("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Frameset//EN\"" PHP_EOL "\t\"http://www.w3.org/TR/xhtml1/DTD/xhtml1-frameset.dtd\">" PHP_EOL);
		/* no break */
		case 9:  RETURN_STRING("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.1//EN\"" PHP_EOL"\t\"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">" PHP_EOL);
		/* no break */
		case 10: RETURN_STRING("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 2.0//EN\"" PHP_EOL "\t\"http://www.w3.org/MarkUp/DTD/xhtml2.dtd\">" PHP_EOL);
		/* no break */
		case 11: RETURN_STRING("<!DOCTYPE html>" PHP_EOL);
		/* no break */
		default: RETURN_EMPTY_STRING();
	}
}

/**
 * Builds a HTML tag
 *
 *<code>
 *	echo Dao\Tag::tagHtml($name, $parameters, $selfClose, $onlyStart, $eol);
 *</code>
 *
 * @param string $tagName
 * @param array $parameters
 * @param boolean $selfClose
 * @param boolean $onlyStart
 * @param boolean $useEol
 * @return string
 */
PHP_METHOD(Dao_Tag, tagHtml){

	zval *tag_name, *parameters = NULL, *self_close = NULL, *only_start = NULL;
	zval *use_eol = NULL, params = {}, default_params = {}, local_code = {}, doctype = {};

	dao_fetch_params(1, 1, 4, &tag_name, &parameters, &self_close, &only_start, &use_eol);

	if (!parameters) {
		parameters = &DAO_GLOBAL(z_null);
	}

	if (!self_close) {
		self_close = &DAO_GLOBAL(z_false);
	}

	if (!only_start) {
		only_start = &DAO_GLOBAL(z_false);
	}

	if (!use_eol) {
		use_eol = &DAO_GLOBAL(z_false);
	}

	if (Z_TYPE_P(parameters) != IS_ARRAY) {
		array_init(&params);
		DAO_MM_ADD_ENTRY(&params);
		dao_array_append(&params, parameters, PH_COPY);
	} else {
		DAO_MM_ZVAL_DUP(&params, parameters);
	}

	dao_read_static_property_ce(&default_params, dao_tag_ce, SL("_defaultParams"), PH_READONLY);
	if (Z_TYPE(default_params) == IS_ARRAY) {
		dao_array_merge_recursive_n2(&params, &default_params, PH_COPY);
	}

	DAO_CONCAT_SV(&local_code, "<", tag_name);
	DAO_MM_ADD_ENTRY(&local_code);
	dao_tag_render_attributes(&local_code, &params);
	DAO_MM_ADD_ENTRY(&local_code);
	dao_read_static_property_ce(&doctype, dao_tag_ce, SL("_documentType"), PH_READONLY);

	/**
	 * Check if Doctype is XHTML
	 */
	if (DAO_GT_LONG(&doctype, 5)) {
		if (zend_is_true(self_close)) {
			dao_concat_self_str(&local_code, SL(" />"));
		} else {
			dao_concat_self_str(&local_code, SL(">"));
		}
		DAO_MM_ADD_ENTRY(&local_code);
	} else {
		if (zend_is_true(only_start)) {
			dao_concat_self_str(&local_code, SL(">"));
		} else {
			DAO_SCONCAT_SVS(&local_code, "></", tag_name, ">");
		}
		DAO_MM_ADD_ENTRY(&local_code);
	}

	if (zend_is_true(use_eol)) {
		dao_concat_self_str(&local_code, SL(PHP_EOL));
		DAO_MM_ADD_ENTRY(&local_code);
	}

	RETURN_MM_CTOR(&local_code);
}

/**
 * Builds a HTML tag closing tag
 *
 *<code>
 *	echo Dao\Tag::tagHtmlClose('script', true)
 *</code>
 *
 * @param string $tagName
 * @param boolean $useEol
 * @return string
 */
PHP_METHOD(Dao_Tag, tagHtmlClose){

	zval *tag_name, *use_eol = NULL, local_code = {};

	dao_fetch_params(1, 1, 1, &tag_name, &use_eol);

	if (!use_eol) {
		use_eol = &DAO_GLOBAL(z_false);
	}

	DAO_CONCAT_SVS(&local_code, "</", tag_name, ">");
	DAO_MM_ADD_ENTRY(&local_code);
	if (zend_is_true(use_eol)) {
		dao_concat_self_str(&local_code, SL(PHP_EOL));
		DAO_MM_ADD_ENTRY(&local_code);
	}

	RETURN_MM_CTOR(&local_code);
}
/**
 * Return default value
 *
 * @param string $name
 * @return mixed
 */
PHP_METHOD(Dao_Tag, getDefault){

	zval *name, display_values = {}, value = {};

	dao_fetch_params(0, 1, 0, &name);

	dao_read_static_property_ce(&display_values, dao_tag_ce, SL("_displayValues"), PH_READONLY);

	if (!dao_array_isset_fetch(&value, &display_values, name, PH_READONLY)) {
		RETURN_NULL();
	}

	RETURN_CTOR(&value);
}

/**
 * Return default values
 *
 * @return mixed
 */
PHP_METHOD(Dao_Tag, getDefaults){


	dao_read_static_property_ce(return_value, dao_tag_ce, SL("_displayValues"), PH_COPY);
}

/**
 * Set default parameters
 *
 * @param array $params
 */
PHP_METHOD(Dao_Tag, setDefaultParams){

	zval *params;

	dao_fetch_params(0, 1, 0, &params);

	if (Z_TYPE_P(params) != IS_ARRAY) {
		DAO_THROW_EXCEPTION_STR(dao_tag_exception_ce, "Default parameters must be an array");
		return;
	}

	dao_update_static_property_ce(dao_tag_ce, SL("_defaultParams"), params);
}

/**
 * Returns default params
 *
 * @return array
 */
PHP_METHOD(Dao_Tag, getDefaultParams)
{

	dao_read_static_property_ce(return_value, dao_tag_ce, SL("_defaultParams"), PH_COPY);
}

/**
 * Set default form parameters
 *
 * @param array $params
 */
PHP_METHOD(Dao_Tag, setDefaultFormParams){

	zval *params;

	dao_fetch_params(0, 1, 0, &params);

	if (Z_TYPE_P(params) != IS_ARRAY) {
		DAO_THROW_EXCEPTION_STR(dao_tag_exception_ce, "Default parameters must be an array");
		return;
	}

	dao_update_static_property_ce(dao_tag_ce, SL("_defaultFormParams"), params);
}

/**
 * Returns default params
 *
 * @return array
 */
PHP_METHOD(Dao_Tag, getDefaultFormParams)
{

	dao_read_static_property_ce(return_value, dao_tag_ce, SL("_defaultFormParams"), PH_COPY);
}

PHP_METHOD(Dao_Tag, choice){

	zval *expression, *value1, *value2 = NULL;

	dao_fetch_params(0, 2, 1, &expression, &value1, &value2);

	if (!value2) {
		value2 = &DAO_GLOBAL(z_null);
	}

	if (zend_is_true(expression)) {
		RETURN_CTOR(value1);
	}

	RETURN_CTOR(value2);
}
