
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

#include "debug.h"
#include "debug/exception.h"
#include "logger/adapterinterface.h"
#include "logger.h"
#include "exception.h"
#include "version.h"
#include "di.h"
#include "loader.h"

#include <ext/standard/php_string.h>
#include <ext/standard/php_var.h>
#include <Zend/zend_builtin_functions.h>
#include <Zend/zend_execute.h>

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/array.h"
#include "kernel/string.h"
#include "kernel/operators.h"
#include "kernel/hash.h"
#include "kernel/concat.h"
#include "kernel/file.h"
#include "kernel/output.h"
#include "kernel/debug.h"

#include "interned-strings.h"

/**
 * Dao\Debug
 *
 * Provides debug capabilities to Dao applications
 */
zend_class_entry *dao_debug_ce;

PHP_METHOD(Dao_Debug, setShowBackTrace);
PHP_METHOD(Dao_Debug, setShowFiles);
PHP_METHOD(Dao_Debug, setShowFileFragment);
PHP_METHOD(Dao_Debug, listen);
PHP_METHOD(Dao_Debug, listenExceptions);
PHP_METHOD(Dao_Debug, listenLowSeverity);
PHP_METHOD(Dao_Debug, halt);
PHP_METHOD(Dao_Debug, debugVar);
PHP_METHOD(Dao_Debug, clearVars);
PHP_METHOD(Dao_Debug, _escapeString);
PHP_METHOD(Dao_Debug, _getArrayDump);
PHP_METHOD(Dao_Debug, _getVarDump);
PHP_METHOD(Dao_Debug, getMajorVersion);
PHP_METHOD(Dao_Debug, getVersion);
PHP_METHOD(Dao_Debug, getCssSources);
PHP_METHOD(Dao_Debug, getJsSources);
PHP_METHOD(Dao_Debug, showTraceItem);
PHP_METHOD(Dao_Debug, onUncaughtException);
PHP_METHOD(Dao_Debug, onUserDefinedError);
PHP_METHOD(Dao_Debug, onShutdown);
PHP_METHOD(Dao_Debug, getLinesBeforeContext);
PHP_METHOD(Dao_Debug, setLinesBeforeContext);
PHP_METHOD(Dao_Debug, getLinesAfterContext);
PHP_METHOD(Dao_Debug, setLinesAfterContext);
PHP_METHOD(Dao_Debug, getFileLink);
PHP_METHOD(Dao_Debug, setLogger);
PHP_METHOD(Dao_Debug, enable);
PHP_METHOD(Dao_Debug, disable);
PHP_METHOD(Dao_Debug, isEnable);
PHP_METHOD(Dao_Debug, log);
PHP_METHOD(Dao_Debug, dumpVar);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_debug_setshowbacktrace, 0, 0, 1)
	ZEND_ARG_INFO(0, showBackTrace)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_debug_setshowfiles, 0, 0, 1)
	ZEND_ARG_INFO(0, showFiles)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_debug_setshowfilefragment, 0, 0, 1)
	ZEND_ARG_INFO(0, showFileFragment)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_debug_listen, 0, 0, 0)
	ZEND_ARG_INFO(0, exceptions)
	ZEND_ARG_INFO(0, lowSeverity)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_debug_debugvar, 0, 0, 1)
	ZEND_ARG_INFO(0, var)
	ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_debug_onuncaughtexception, 0, 0, 1)
	ZEND_ARG_INFO(0, exception)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_debug_onuserdefinederror, 0, 0, 2)
	ZEND_ARG_INFO(0, severity)
	ZEND_ARG_INFO(0, message)
	ZEND_ARG_INFO(0, file)
	ZEND_ARG_INFO(0, line)
	ZEND_ARG_INFO(0, context)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_debug_setcharset, 0, 0, 1)
	ZEND_ARG_INFO(0, charset)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_debug_setlines, 0, 0, 1)
	ZEND_ARG_INFO(0, lines)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_debug_getfilelink, 0, 0, 3)
	ZEND_ARG_INFO(0, file)
	ZEND_ARG_INFO(0, line)
	ZEND_ARG_INFO(0, format)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_debug_setlogger, 0, 0, 1)
	ZEND_ARG_INFO(0, logger)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_debug_enable, 0, 0, 0)
	ZEND_ARG_INFO(0, logger)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_debug_log, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, message, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, type, IS_LONG, 1)
	ZEND_ARG_INFO(0, context)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_debug_dumpvar, 0, 0, 1)
	ZEND_ARG_INFO(0, var)
	ZEND_ARG_TYPE_INFO(0, level, IS_LONG, 1)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_debug_method_entry[] = {
	PHP_ME(Dao_Debug, setShowBackTrace, arginfo_dao_debug_setshowbacktrace, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Debug, setShowFiles, arginfo_dao_debug_setshowfiles, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Debug, setShowFileFragment, arginfo_dao_debug_setshowfilefragment, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Debug, listen, arginfo_dao_debug_listen, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Debug, listenExceptions, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Debug, listenLowSeverity, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Debug, halt, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Debug, debugVar, arginfo_dao_debug_debugvar, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Debug, clearVars, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Debug, _escapeString, arginfo_empty, ZEND_ACC_PROTECTED)
	PHP_ME(Dao_Debug, _getArrayDump, arginfo_empty, ZEND_ACC_PROTECTED)
	PHP_ME(Dao_Debug, _getVarDump, arginfo_empty, ZEND_ACC_PROTECTED)
	PHP_ME(Dao_Debug, getMajorVersion, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Debug, getVersion, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Debug, getCssSources, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Debug, getJsSources, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Debug, showTraceItem, arginfo_empty, ZEND_ACC_PROTECTED)
	PHP_ME(Dao_Debug, onUncaughtException, arginfo_dao_debug_onuncaughtexception, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Debug, onUserDefinedError, arginfo_dao_debug_onuserdefinederror, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Debug, onShutdown, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Debug, getLinesBeforeContext, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Debug, setLinesBeforeContext, arginfo_dao_debug_setlines, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Debug, getLinesAfterContext, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Debug, setLinesAfterContext, arginfo_dao_debug_setlines, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Debug, getFileLink, arginfo_dao_debug_getfilelink, ZEND_ACC_PROTECTED)
	PHP_ME(Dao_Debug, setLogger, arginfo_dao_debug_setlogger, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(Dao_Debug, enable, arginfo_dao_debug_enable, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(Dao_Debug, disable, arginfo_empty, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(Dao_Debug, isEnable, arginfo_empty, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(Dao_Debug, log, arginfo_dao_debug_log, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(Dao_Debug, dumpVar, arginfo_dao_debug_dumpvar, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_FE_END
};

/**
 * Dao\Debug initializer
 */
DAO_INIT_CLASS(Dao_Debug){

	DAO_REGISTER_CLASS(Dao, Debug, debug, dao_debug_method_entry, 0);

	zend_declare_property_bool(dao_debug_ce, SL("_hideDocumentRoot"), 0, ZEND_ACC_PROTECTED);
	zend_declare_property_bool(dao_debug_ce, SL("_showBackTrace"), 1, ZEND_ACC_PROTECTED);
	zend_declare_property_bool(dao_debug_ce, SL("_showFiles"), 0, ZEND_ACC_PROTECTED);
	zend_declare_property_bool(dao_debug_ce, SL("_showFileFragment"), 0, ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_debug_ce, SL("_data"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_debug_ce, SL("_isActive"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);
	zend_declare_property_string(dao_debug_ce, SL("_charset"), "utf-8", ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);
	zend_declare_property_long(dao_debug_ce, SL("_beforeContext"), 7, ZEND_ACC_PROTECTED);
	zend_declare_property_long(dao_debug_ce, SL("_afterContext"), 5, ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_debug_ce, SL("_logger"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);
	zend_declare_property_bool(dao_debug_ce, SL("_listen"), 0, ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);
	zend_declare_property_null(dao_debug_ce, SL("_logs"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);

	return SUCCESS;
}

/**
 * Sets if files the exception's backtrace must be showed
 *
 * @param boolean $showBackTrace
 * @return Dao\Debug
 */
PHP_METHOD(Dao_Debug, setShowBackTrace){

	zval *show_back_trace;

	dao_fetch_params(0, 1, 0, &show_back_trace);

	dao_update_property(getThis(), SL("_showBackTrace"), show_back_trace);
	RETURN_THIS();
}

/**
 * Set if files part of the backtrace must be shown in the output
 *
 * @param boolean $showFiles
 * @return Dao\Debug
 */
PHP_METHOD(Dao_Debug, setShowFiles){

	zval *show_files;

	dao_fetch_params(0, 1, 0, &show_files);

	dao_update_property(getThis(), SL("_showFiles"), show_files);
	RETURN_THIS();
}

/**
 * Sets if files must be completely opened and showed in the output
 * or just the fragment related to the exception
 *
 * @param boolean $showFileFragment
 * @return Dao\Debug
 */
PHP_METHOD(Dao_Debug, setShowFileFragment){

	zval *show_file_fragment;

	dao_fetch_params(0, 1, 0, &show_file_fragment);

	dao_update_property(getThis(), SL("_showFileFragment"), show_file_fragment);
	RETURN_THIS();
}

/**
 * Listen for uncaught exceptions and unsilent notices or warnings
 *
 * @param boolean $exceptions
 * @param boolean $lowSeverity
 * @return Dao\Debug
 */
PHP_METHOD(Dao_Debug, listen){

	zval *exceptions = NULL, *low_severity = NULL;

	dao_fetch_params(0, 0, 2, &exceptions, &low_severity);

	if (!exceptions || zend_is_true(exceptions)) {
		DAO_CALL_METHOD(NULL, getThis(), "listenexceptions");
	}

	if (low_severity && zend_is_true(low_severity)) {
		DAO_CALL_METHOD(NULL, getThis(), "listenlowseverity");
	}

	dao_update_static_property_bool_ce(dao_debug_ce, SL("_listen"), 1);
	RETURN_THIS();
}

/**
 * Listen for uncaught exceptions
 *
 * @return Dao\Debug
 */
PHP_METHOD(Dao_Debug, listenExceptions){

	zval handler = {};

	array_init_size(&handler, 2);
	dao_array_append(&handler, getThis(), PH_COPY);
	add_next_index_stringl(&handler, SL("onUncaughtException"));
	DAO_CALL_FUNCTION(NULL, "set_exception_handler", &handler);
	zval_ptr_dtor(&handler);
	RETURN_THIS();
}

/**
 * Listen for unsilent notices or warnings or user-defined error
 *
 * @return Dao\Debug
 */
PHP_METHOD(Dao_Debug, listenLowSeverity)
{
	zval handler = {};

	array_init_size(&handler, 2);
	dao_array_append(&handler, getThis(), PH_COPY);
	add_next_index_stringl(&handler, SL("onUserDefinedError"));
	DAO_CALL_FUNCTION(NULL, "set_error_handler", &handler);
	zval_ptr_dtor(&handler);

	array_init_size(&handler, 2);
	dao_array_append(&handler, getThis(), PH_COPY);
	add_next_index_stringl(&handler, SL("onShutdown"));
	DAO_CALL_FUNCTION(NULL, "register_shutdown_function", &handler);
	zval_ptr_dtor(&handler);
	RETURN_THIS();
}

/**
 * Halts the request showing a backtrace
 */
PHP_METHOD(Dao_Debug, halt){

	zend_throw_exception(NULL, "Halted request", 0);
}

/**
 * Adds a variable to the debug output
 *
 * @param mixed $var
 * @param string $key
 * @return Dao\Debug
 */
PHP_METHOD(Dao_Debug, debugVar){

	zval *var, *key = NULL, ztime = {}, backtrace = {}, data = {};

	dao_fetch_params(0, 1, 1, &var, &key);

	if (!key) {
		key = &DAO_GLOBAL(z_null);
	}

	ZVAL_LONG(&ztime, (long) time(NULL));

	zend_fetch_debug_backtrace(&backtrace, 0, DEBUG_BACKTRACE_PROVIDE_OBJECT, 0);

	array_init_size(&data, 3);
	dao_array_append(&data, var, PH_COPY);
	dao_array_append(&data, &backtrace, PH_COPY);
	dao_array_append(&data, &ztime, PH_COPY);
	dao_update_property_array_append(getThis(), SL("_data"), &data);
	zval_ptr_dtor(&data);
	RETURN_THIS();
}

/**
 * Clears are variables added previously
 *
 * @return Dao\Debug
 */
PHP_METHOD(Dao_Debug, clearVars){


	dao_update_property_null(getThis(), SL("_data"));
	RETURN_THIS();
}

/**
 * Escapes a string with htmlentities
 *
 * @param string $value
 * @return string
 */
PHP_METHOD(Dao_Debug, _escapeString){

	zval *value, charset = {}, replaced_value = {};

	dao_fetch_params(1, 1, 0, &value);

	if (Z_TYPE_P(value) == IS_STRING) {
		zval line_break = {}, escaped_line_break = {};

		dao_read_static_property_ce(&charset, dao_debug_ce, SL("_charset"), PH_READONLY);

		DAO_MM_ZVAL_STRING(&line_break, "\n");
		DAO_MM_ZVAL_STRING(&escaped_line_break, "\\n");

		DAO_STR_REPLACE(&replaced_value, &line_break, &escaped_line_break, value);
		dao_htmlentities(return_value, &replaced_value, NULL, &charset);
		zval_ptr_dtor(&replaced_value);
		RETURN_MM();
	}

	RETURN_MM_CTOR(value);
}

/**
 * Produces a recursive representation of an array
 *
 * @param array $argument
 * @return string
 */
PHP_METHOD(Dao_Debug, _getArrayDump){

	zval *argument, *n = NULL, number_arguments = {}, dump = {}, *v, joined_dump = {};
	zend_string *str_key;
	ulong idx;

	dao_fetch_params(1, 1, 1, &argument, &n);

	if (!n) {
		n = &DAO_GLOBAL(z_zero);
	}

	if (DAO_LT_LONG(n, 3)) {
		dao_fast_count(&number_arguments, argument);
		if (DAO_GT_LONG(&number_arguments, 0)) {
			if (DAO_LT_LONG(&number_arguments, 10)) {
				array_init(&dump);
				DAO_MM_ADD_ENTRY(&dump);
				ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(argument), idx, str_key, v) {
					zval tmp = {}, var_dump = {}, escaped_string = {}, next = {}, array_dump = {}, class_name = {};
					zend_class_entry *ce;
					if (str_key) {
						ZVAL_STR(&tmp, str_key);
					} else {
						ZVAL_LONG(&tmp, idx);
					}
					if (DAO_IS_SCALAR(v)) {
						if (DAO_IS_STRING(v, "")) {
							DAO_CONCAT_SVS(&var_dump, "[", &tmp, "] =&gt; (empty string)");
						} else {
							DAO_MM_CALL_METHOD(&escaped_string, getThis(), "_escapestring", v);
							DAO_CONCAT_SVSV(&var_dump, "[", &tmp, "] =&gt; ", &escaped_string);
							zval_ptr_dtor(&escaped_string);
						}
						dao_array_append(&dump, &var_dump, 0);
					} else {
						if (Z_TYPE_P(v) == IS_ARRAY) {
							dao_add_function(&next, n, &DAO_GLOBAL(z_one));

							DAO_MM_CALL_METHOD(&array_dump, getThis(), "_getarraydump", v, &next);
							DAO_CONCAT_SVSVS(&var_dump, "[", &tmp, "] =&gt; Array(", &array_dump, ")");
							zval_ptr_dtor(&array_dump);
							dao_array_append(&dump, &var_dump, 0);
							continue;
						}
						if (Z_TYPE_P(v) == IS_OBJECT) {
							ce = Z_OBJCE_P(v);
							ZVAL_NEW_STR(&class_name, ce->name);

							DAO_CONCAT_SVSVS(&var_dump, "[", &tmp, "] =&gt; Object(", &class_name, ")");
							dao_array_append(&dump, &var_dump, 0);
							continue;
						}

						if (Z_TYPE_P(v) == IS_NULL) {
							DAO_CONCAT_SVS(&var_dump, "[", &tmp, "] =&gt; null");
							dao_array_append(&dump, &var_dump, 0);
							continue;
						}

						DAO_CONCAT_SVSV(&var_dump, "[", &tmp, "] =&gt; ", v);
						dao_array_append(&dump, &var_dump, 0);
					}
				} ZEND_HASH_FOREACH_END();

				dao_fast_join_str(&joined_dump, SL(", "), &dump);
				RETURN_MM_NCTOR(&joined_dump);
			}

			RETURN_MM_CTOR(&number_arguments);
		}
	}

	RETURN_NULL();
}

/**
 * Produces an string representation of a variable
 *
 * @param mixed $variable
 * @return string
 */
PHP_METHOD(Dao_Debug, _getVarDump){

	zval *variable, class_name = {}, dumped_object = {}, array_dump = {}, dump = {};

	dao_fetch_params(1, 1, 0, &variable);

	if (DAO_IS_SCALAR(variable)) {
		/**
		 * Boolean variables are represented as 'true'/'false'
		 */
		if (DAO_IS_BOOL(variable)) {
			if (zend_is_true(variable)) {
				RETURN_MM_STRING("true");
			} else {
				RETURN_MM_STRING("false");
			}
		}

		/**
		 * String variables are escaped to avoid XSS injections
		 */
		if (Z_TYPE_P(variable) == IS_STRING) {
			DAO_MM_RETURN_CALL_METHOD(getThis(), "_escapestring", variable);
			RETURN_MM();
		}

		/**
		 * Other scalar variables are just converted to strings
		 */
		RETURN_MM_CTOR(variable);
	}

	/**
	 * If the variable is an object print its class name
	 */
	if (Z_TYPE_P(variable) == IS_OBJECT) {
		const zend_class_entry *ce = Z_OBJCE_P(variable);

		ZVAL_STR(&class_name, ce->name);

		/**
		 * Try to check for a 'dump' method, this surely produces a better printable
		 * representation
		 */
		if (dao_method_exists_ex(variable, SL("dump")) == SUCCESS) {
			DAO_MM_CALL_METHOD(&dumped_object, variable, "dump");
			DAO_MM_ADD_ENTRY(&dumped_object);
			/**
			 * dump() must return an array, generate a recursive representation using
			 * getArrayDump
			 */
			DAO_MM_CALL_METHOD(&array_dump, getThis(), "_getarraydump", &dumped_object);
			DAO_CONCAT_SVSVS(&dump, "Object(", &class_name, ": ", &array_dump, ")");
			zval_ptr_dtor(&array_dump);
			DAO_MM_ADD_ENTRY(&dump);
		} else {
			/**
			 * If dump() is not available just print the class name
			 */
			DAO_CONCAT_SVS(&dump, "Object(", &class_name, ")</span>");
			DAO_MM_ADD_ENTRY(&dump);
		}

		RETURN_MM_CTOR(&dump);
	}

	/**
	 * Recursively process the array and enclose it in Array()
	 */
	if (Z_TYPE_P(variable) == IS_ARRAY) {
		DAO_MM_CALL_METHOD(&array_dump, getThis(), "_getarraydump", variable);
		DAO_CONCAT_SVS(return_value, "Array(", &array_dump, ")");
		zval_ptr_dtor(&array_dump);
		RETURN_MM();
	}

	/**
	 * Null variables are represented as 'null'
	 * Other types are represented by its type
	 */
	RETURN_MM_STRING(zend_zval_type_name(variable));
}

/**
 * Returns the major framework's version
 *
 * @return string
 */
PHP_METHOD(Dao_Debug, getMajorVersion){

	zval version = {}, parts = {};

	DAO_CALL_CE_STATIC(&version, dao_version_ce, "get");

	dao_fast_explode_str(&parts, SL(" "), &version);
	zval_ptr_dtor(&version);

	dao_array_fetch_long(return_value, &parts, 0, PH_NOISY|PH_COPY);
	zval_ptr_dtor(&parts);
}

/**
 * Generates a link to the current version documentation
 *
 * @return string
 */
PHP_METHOD(Dao_Debug, getVersion){

	zval version = {};

	DAO_CALL_METHOD(&version, getThis(), "getmajorversion");
	DAO_CONCAT_SVSVS(return_value, "<div class=\"version\">Dao7 Framework <a target=\"_new\" href=\"http://docs.myleftstudio.com/", &version, "/\">", &version, "</a></div>");
	zval_ptr_dtor(&version);
}

/**
 * Returns the css sources
 *
 * @return string
 */
PHP_METHOD(Dao_Debug, getCssSources){

	RETURN_STRING("<link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.1.3/css/bootstrap.min.css\" integrity=\"sha384-MCw98/SFnGE8fJT3GXwEOngsV7Zt27NXFoaoApmYm81iuXoPkFOJwJ8ERdknLPMO\" crossorigin=\"anonymous\">");
}

/**
 * Returns the javascript sources
 *
 * @return string
 */
PHP_METHOD(Dao_Debug, getJsSources){

}

PHP_METHOD(Dao_Debug, getFileLink) {

	zval *file, *line, *format;

	dao_fetch_params(0, 3, 0, &file, &line, &format);
	DAO_ENSURE_IS_STRING(file);
	DAO_ENSURE_IS_STRING(line);

	if (Z_TYPE_P(format) == IS_STRING) {
		zend_string *tmp, *link;
		zval z_link = {};

		tmp  = php_str_to_str(Z_STRVAL_P(format), Z_STRLEN_P(format), SL("%f"), Z_STRVAL_P(file), Z_STRLEN_P(file));
		link = php_str_to_str(ZSTR_VAL(tmp), ZSTR_LEN(tmp), SL("%l"), Z_STRVAL_P(line), Z_STRLEN_P(line));

		ZVAL_STR(&z_link, link);

		DAO_CONCAT_SVSVS(return_value, "<a href=\"", &z_link, "\">", file, "</a>");

		zend_string_release(tmp);
		zend_string_release(link);
	} else {
		RETVAL_ZVAL(file, 1, 0);
	}
}

/**
 * Shows a backtrace item
 *
 * @param int $n
 * @param array $trace
 */
PHP_METHOD(Dao_Debug, showTraceItem){

	zval *n, *trace, *link_format, space = {}, two_spaces = {}, underscore = {}, minus = {}, html = {}, class_name = {}, prepare_uri_class = {};
	zval type = {}, function_name = {}, trace_args = {}, *argument, file = {}, line = {}, show_files = {}, lines = {}, number_lines = {};
	zval show_file_fragment = {}, before_context = {}, before_line = {}, first_line = {}, after_context = {}, after_line = {}, last_line = {}, comment_pattern = {}, charset = {}, tab = {}, comment = {}, i = {}, formatted_file = {};
	zend_class_entry *class_ce;

	dao_fetch_params(1, 3, 0, &n, &trace, &link_format);

	DAO_MM_ZVAL_STRING(&space, " ");
	DAO_MM_ZVAL_STRING(&two_spaces, "  ");
	DAO_MM_ZVAL_STRING(&underscore, "_");
	DAO_MM_ZVAL_STRING(&minus, "-");

	/**
	 * Every trace in the backtrace have a unique number
	 */
	DAO_CONCAT_SVS(&html, "<tr><td align=\"right\" valign=\"top\" class=\"error-number\">#", n, "</td><td>");
	DAO_MM_ADD_ENTRY(&html);
	if (dao_array_isset_fetch_str(&class_name, trace, SL("class"), PH_READONLY)) {
		class_ce = dao_fetch_class(&class_name, ZEND_FETCH_CLASS_AUTO | ZEND_FETCH_CLASS_SILENT);

		if (!class_ce) {
			/* Unable to load the class, should never happen */
		} else if (is_dao_class(class_ce)) {
			zval namespace_separator = {};
			ZVAL_STRING(&namespace_separator, "\\");

			/* Prepare the class name according to the Dao's conventions */
			DAO_STR_REPLACE(&prepare_uri_class, &namespace_separator, &underscore, &class_name);
			zval_ptr_dtor(&namespace_separator);
			DAO_MM_ADD_ENTRY(&prepare_uri_class);

			/* Generate a link to the official docs */
			DAO_SCONCAT_SVSVS(&html, "<span class=\"error-class\"><a target=\"_new\" href=\"http://www.myleftstudio.com/api/", &prepare_uri_class, ".html\">", &class_name, "</a></span>");
			DAO_MM_ADD_ENTRY(&html);
		} else if (class_ce->type == ZEND_INTERNAL_CLASS) {
			zval lower_class_name = {}, prepare_internal_class = {};
			dao_fast_strtolower(&lower_class_name, &class_name);
			DAO_STR_REPLACE(&prepare_internal_class, &underscore, &minus, &lower_class_name);
			zval_ptr_dtor(&lower_class_name);

			/* Generate a link to the official docs */
			DAO_SCONCAT_SVSVS(&html, "<span class=\"error-class\"><a target=\"_new\" href=\"http://php.net/manual/en/class.", &prepare_internal_class, ".php\">", &class_name, "</a></span>");
			zval_ptr_dtor(&prepare_internal_class);
			DAO_MM_ADD_ENTRY(&html);
		} else {
			DAO_SCONCAT_SVS(&html, "<span class=\"error-class\">", &class_name, "</span>");
			DAO_MM_ADD_ENTRY(&html);
		}

		/**
		 * Object access operator: static/instance
		 */
		dao_array_fetch_str(&type, trace, SL("type"), PH_NOISY|PH_READONLY);
		dao_concat_self(&html, &type);
		DAO_MM_ADD_ENTRY(&html);
	}

	/**
	 * Normally the backtrace contains only classes
	 */

	if (dao_array_isset_fetch_str(&function_name, trace, SL("function"), PH_READONLY)) {
		if (dao_array_isset_str(trace, SL("class"))) {
			DAO_SCONCAT_SVS(&html, "<span class=\"error-function\">", &function_name, "</span>");
			DAO_MM_ADD_ENTRY(&html);
		} else {
			zend_function *func;

			/**
			 * Check if the function exists
			 */
			if ((func = dao_fetch_function(Z_STR(function_name))) != NULL) {

				/**
				 * Internal functions links to the PHP documentation
				 */
				if (func->type == ZEND_INTERNAL_FUNCTION) {
					zval prepared_function_name = {};
					/**
					 * Prepare function's name according to the conventions in the docs
					 */
					DAO_STR_REPLACE(&prepared_function_name, &underscore, &minus, &function_name);
					DAO_SCONCAT_SVSVS(&html, "<span class=\"error-function\"><a target=\"_new\" href=\"http://php.net/manual/en/function.", &prepared_function_name, ".php\">", &function_name, "</a></span>");
					zval_ptr_dtor(&prepared_function_name);
					DAO_MM_ADD_ENTRY(&html);
				} else {
					DAO_SCONCAT_SVS(&html, "<span class=\"error-function\">", &function_name, "</span>");
					DAO_MM_ADD_ENTRY(&html);
				}
			} else {
				DAO_SCONCAT_SVS(&html, "<span class=\"error-function\">", &function_name, "</span>");
				DAO_MM_ADD_ENTRY(&html);
			}
		}
	}

	/**
	 * Check for arguments in the function
	 */
	if (dao_array_isset_fetch_str(&trace_args, trace, SL("args"), PH_READONLY)) {
		if (dao_fast_count_ev(&trace_args)) {
			zval arguments = {}, joined_arguments = {};
			array_init(&arguments);
			DAO_MM_ADD_ENTRY(&html);
			ZEND_HASH_FOREACH_VAL(Z_ARRVAL(trace_args), argument) {
				zval dumped_argument = {}, span_argument = {};
				/**
				 * Every argument is generated using _getVarDump
				 */
				DAO_MM_CALL_METHOD(&dumped_argument, getThis(), "_getvardump", argument);
				DAO_CONCAT_SVS(&span_argument, "<span class=\"error-parameter\">", &dumped_argument, "</span>");
				zval_ptr_dtor(&dumped_argument);

				/**
				 * Append the HTML generated to the argument's list
				 */
				dao_array_append(&arguments, &span_argument, 0);
			} ZEND_HASH_FOREACH_END();

			/**
			 * Join all the arguments
			 */
			dao_fast_join_str(&joined_arguments, SL(", "), &arguments);
			DAO_SCONCAT_SVS(&html, "(", &joined_arguments, ")");
			zval_ptr_dtor(&joined_arguments);
			DAO_MM_ADD_ENTRY(&html);
		} else {
			dao_concat_self_str(&html, SL("()"));
			DAO_MM_ADD_ENTRY(&html);
		}
	}

	/**
	 * When 'file' is present, it usually means the function is provided by the user
	 */
	if (dao_array_isset_fetch_str(&file, trace, SL("file"), PH_READONLY)) {
		dao_array_fetch_str(&line, trace, SL("line"), PH_NOISY|PH_READONLY);

		DAO_MM_CALL_METHOD(&formatted_file, getThis(), "getfilelink", &file, &line, link_format);

		/**
		 * Realpath to the file and its line using a special header
		 */
		DAO_SCONCAT_SVSVS(&html, "<br/><div class=\"error-file\">", &formatted_file, " (", &line, ")</div>");
		zval_ptr_dtor(&formatted_file);
		DAO_MM_ADD_ENTRY(&html);

		dao_read_property(&show_files, getThis(), SL("_showFiles"), PH_READONLY);

		/**
		 * The developer can change if the files must be opened or not
		 */
		if (zend_is_true(&show_files)) {
			/**
			 * Open the file to an array using 'file', this respects the openbase-dir directive
			 */
			DAO_MM_CALL_FUNCTION(&lines, "file", &file);
			DAO_MM_ADD_ENTRY(&lines);
			dao_fast_count(&number_lines, &lines);

			dao_read_property(&show_file_fragment, getThis(), SL("_showFileFragment"), PH_READONLY);

			/**
			 * File fragments just show a piece of the file where the exception is located
			 */
			if (zend_is_true(&show_file_fragment)) {

				/**
				 * Take lines back to the current exception's line
				 */
				dao_read_property(&before_context, getThis(), SL("_beforeContext"), PH_READONLY);

				dao_sub_function(&before_line, &line, &before_context);

				/**
				 * Check for overflows
				 */
				if (DAO_LT_LONG(&before_line, 1)) {
					ZVAL_COPY_VALUE(&first_line, &DAO_GLOBAL(z_one));
				} else {
					ZVAL_COPY_VALUE(&first_line, &before_line);
				}

				/**
				 * Take lines after the current exception's line
				 */
				dao_read_property(&after_context, getThis(), SL("_afterContext"), PH_READONLY);

				dao_add_function(&after_line, &line, &after_context);

				/**
				 * Check for overflows
				 */
				if (DAO_GT(&after_line, &number_lines)) {
					ZVAL_COPY_VALUE(&last_line, &number_lines);
				} else {
					ZVAL_COPY_VALUE(&last_line, &after_line);
				}

				DAO_SCONCAT_SVSVSVS(&html, "<pre class='prettyprint highlight:", &first_line, ":", &line, " linenums:", &first_line, "'>");
				DAO_MM_ADD_ENTRY(&html);
			} else {
				ZVAL_COPY_VALUE(&first_line, &DAO_GLOBAL(z_one));
				ZVAL_COPY_VALUE(&last_line, &number_lines);
				DAO_SCONCAT_SVSVS(&html, "<pre class='prettyprint highlight:", &first_line, ":", &line, " linenums error-scroll'>");
				DAO_MM_ADD_ENTRY(&html);
			}

			DAO_MM_ZVAL_STRING(&comment_pattern, "#\\*\\/$#");

			dao_read_static_property_ce(&charset, dao_debug_ce, SL("_charset"), PH_READONLY);

			DAO_MM_ZVAL_STRING(&tab, "\t");
			DAO_MM_ZVAL_STRING(&comment, "* /");

			ZVAL_COPY_VALUE(&i, &first_line);

			while (DAO_LE(&i, &last_line)) {
				zval line_position = {}, current_line = {}, trimmed = {}, is_comment = {}, spaced_current_line = {}, escaped_line = {};
				/**
				 * Current line in the file
				 */
				dao_sub_function(&line_position, &i, &DAO_GLOBAL(z_one));

				/**
				 * Current line content in the piece of file
				 */
				dao_array_fetch(&current_line, &lines, &line_position, PH_NOISY|PH_READONLY);

				/**
				 * File fragments are cleaned, removing tabs and comments
				 */
				if (zend_is_true(&show_file_fragment)) {
					if (DAO_IS_EQUAL(&i, &first_line)) {
						ZVAL_STR(&trimmed, dao_trim(&current_line, NULL, DAO_TRIM_RIGHT));
						DAO_MM_ADD_ENTRY(&trimmed);
						dao_preg_match(&is_comment, &comment_pattern, &current_line, NULL, 0, 0);

						if (zend_is_true(&is_comment)) {
							DAO_STR_REPLACE(&spaced_current_line, &comment, &space, &current_line);
							DAO_MM_ADD_ENTRY(&spaced_current_line);
							ZVAL_COPY_VALUE(&current_line, &spaced_current_line);
						}
					}
				}

				/**
				 * Print a non break space if the current line is a line break, this allows to show
				 * the html zebra properly
				 */
				if (DAO_IS_STRING(&current_line, "\n")) {
					dao_concat_self_str(&html, SL("&nbsp;\n"));
					DAO_MM_ADD_ENTRY(&html);
				} else {
					if (DAO_IS_STRING(&current_line, "\r\n")) {
						dao_concat_self_str(&html, SL("&nbsp;\n"));
						DAO_MM_ADD_ENTRY(&html);
					} else {
						DAO_STR_REPLACE(&spaced_current_line, &tab, &two_spaces, &current_line);
						DAO_MM_ADD_ENTRY(&spaced_current_line);
						dao_htmlentities(&escaped_line, &spaced_current_line, NULL, &charset);
						DAO_MM_ADD_ENTRY(&escaped_line);
						dao_concat_self(&html, &escaped_line);
						DAO_MM_ADD_ENTRY(&html);
					}
				}

				dao_increment(&i);
			}
			dao_concat_self_str(&html, SL("</pre>"));
			DAO_MM_ADD_ENTRY(&html);
		}
	}

	dao_concat_self_str(&html, SL("</td></tr>"));
	DAO_MM_ADD_ENTRY(&html);
	RETURN_MM_CTOR(&html);
}

/**
 * Handles uncaught exceptions
 *
 * @param \Exception $exception
 * @return boolean
 */
PHP_METHOD(Dao_Debug, onUncaughtException){

	zval *exception, is_active = {}, message = {}, class_name = {}, css_sources = {}, escaped_message = {}, html = {}, version = {}, file = {}, line = {}, show_back_trace = {};
	zval data_vars = {}, trace = {}, *trace_item, *_REQUEST, *value = NULL, *_SERVER, *_SESSION, *_COOKIE, files = {}, di = {}, service_name = {}, router = {}, routes = {}, *route;
	zval loader = {}, loader_value = {}, dumped_loader_value = {}, memory = {}, *data_var, logs = {}, *log, js_sources = {}, formatted_file = {}, z_link_format = {};
	zend_bool ini_exists = 1;
	zend_class_entry *ce;
	zend_string *str_key;
	ulong idx;
	char* link_format;

	dao_fetch_params(1, 1, 0, &exception);

	/**
	 * Cancel the output buffer if active
	 */
	if (dao_ob_get_level() > 0) {
		dao_ob_end_clean();
	}

	dao_read_static_property_ce(&is_active, dao_debug_ce, SL("_isActive"), PH_READONLY);

	/**
	 * Avoid that multiple exceptions being showed
	 */
	if (zend_is_true(&is_active)) {
		DAO_MM_CALL_METHOD(&message, exception, "getmessage");
		zend_print_zval(&message, 0);
		zval_ptr_dtor(&message);
	}

	/**
	 * Globally block the debug component to avoid other exceptions must be shown
	 */
	zend_update_static_property_bool(dao_debug_ce, SL("_isActive"), 1);

	ce = Z_OBJCE_P(exception);
	ZVAL_NEW_STR(&class_name, ce->name);
	DAO_MM_ADD_ENTRY(&class_name);
	DAO_MM_CALL_METHOD(&message, exception, "getmessage");
	DAO_MM_ADD_ENTRY(&message);

	/**
	 * CSS static sources to style the error presentation
	 */
	DAO_MM_CALL_METHOD(&css_sources, getThis(), "getcsssources");
	DAO_MM_ADD_ENTRY(&css_sources);

	/**
	 * Escape the exception's message avoiding possible XSS injections?
	 */
	ZVAL_COPY_VALUE(&escaped_message, &message);

	/**
	 * Use the exception info as document's title
	 */
	DAO_CONCAT_SVSVS(&html, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"/><title>", &class_name, ": ", &escaped_message, "</title>");
	DAO_MM_ADD_ENTRY(&html);
	DAO_SCONCAT_VS(&html, &css_sources, "</head><body><div class=\"container-fluid\" style=\"margin-top:15px\"><div class=\"card-group\">");
	DAO_MM_ADD_ENTRY(&html);

	/**
	 * Get the version link
	 */
	DAO_MM_CALL_METHOD(&version, getThis(), "getversion");
	DAO_MM_ADD_ENTRY(&version);

	DAO_SCONCAT_SVS(&html, "<div class=\"card\"><div class=\"card-header\">Version</div><div class=\"card-body\"><p class=\"card-text\">", &version, __DATE__ " " __TIME__ "</p></div></div>");
	DAO_MM_ADD_ENTRY(&html);

	/**
	 * Memory usage
	 */
	ZVAL_LONG(&memory, zend_memory_usage(1));
	DAO_SCONCAT_SVS(&html, "<div class=\"card\"><div class=\"card-header\">Memory Usage</div><div class=\"card-body\"><p class=\"card-text\">", &memory, "</p></div></div></div>");
	DAO_MM_ADD_ENTRY(&html);

	DAO_MM_CALL_METHOD(&file, exception, "getfile");
	DAO_MM_ADD_ENTRY(&file);
	DAO_MM_CALL_METHOD(&line, exception, "getline");
	DAO_MM_ADD_ENTRY(&line);

	link_format = zend_ini_string_ex(SL("xdebug.file_link_format"), 0, &ini_exists);
	if (!link_format || !ini_exists || !strlen(link_format)) {
		link_format = "file://%f#%l";
	}

	DAO_MM_ZVAL_STRING(&z_link_format, link_format);

	DAO_MM_CALL_METHOD(&formatted_file, getThis(), "getfilelink", &file, &line, &z_link_format);
	DAO_MM_ADD_ENTRY(&formatted_file);

	/**
	 * Main exception info
	 */

	DAO_SCONCAT_SVSVS(&html, "<div class=\"card error-info\" style=\"margin-top:15px\"><div class=\"card-header\">Exception info</div><div class=\"card-body\"><p class=\"card-text\">", &class_name, ": ", &escaped_message, "</p>");
	DAO_MM_ADD_ENTRY(&html);

	DAO_SCONCAT_SVSVS(&html, "<p class=\"card-text\">", &formatted_file, " (", &line, ")</p></div>");
	DAO_MM_ADD_ENTRY(&html);

	/**
	 * Check if the developer wants to show the backtrace or not
	 */
	dao_read_property(&show_back_trace, getThis(), SL("_showBackTrace"), PH_READONLY);
	if (!zend_is_true(&show_back_trace)) {
		DAO_SCONCAT_STR(&html, "</div>");
		DAO_MM_ADD_ENTRY(&html);
	} else {
		dao_concat_self_str(&html, SL("<table class=\"table table-striped\">"));
		DAO_MM_ADD_ENTRY(&html);

		DAO_MM_CALL_METHOD(&trace, exception, "gettrace");
		DAO_MM_ADD_ENTRY(&trace);

		ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL(trace), idx, str_key, trace_item) {
			zval tmp = {}, html_item = {};
			if (str_key) {
				ZVAL_STR(&tmp, str_key);
			} else {
				ZVAL_LONG(&tmp, idx);
			}

			/**
			 * Every line in the trace is rendered using 'showTraceItem'
			 */
			DAO_MM_CALL_METHOD(&html_item, getThis(), "showtraceitem", &tmp, trace_item, &z_link_format);
			dao_concat_self(&html, &html_item);
			zval_ptr_dtor(&html_item);
			DAO_MM_ADD_ENTRY(&html);
		} ZEND_HASH_FOREACH_END();

		dao_concat_self_str(&html, SL("</table></div>"));
		DAO_MM_ADD_ENTRY(&html);

		/**
		 * Print _REQUEST superglobal
		 */
		dao_concat_self_str(&html, SL("<div class=\"card\" style=\"margin-top:15px\"><div class=\"card-header\">_REQUEST</div><table class=\"table table-striped\">"));
		DAO_MM_ADD_ENTRY(&html);
		dao_concat_self_str(&html, SL("<tr><th class=\"col-md-4\">Key</th><th class=\"col-md-8\">Value</th></tr>"));
		DAO_MM_ADD_ENTRY(&html);

		_REQUEST = dao_get_global_str(SL("_REQUEST"));

		if (Z_TYPE_P(_REQUEST) == IS_ARRAY) {
			ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(_REQUEST), idx, str_key, value) {
				zval tmp = {}, joined_value = {};
				if (str_key) {
					ZVAL_STR(&tmp, str_key);
				} else {
					ZVAL_LONG(&tmp, idx);
				}
				if (Z_TYPE_P(value) == IS_ARRAY) {
					DAO_MM_CALL_METHOD(&joined_value, getThis(), "_getvardump", value);
					DAO_SCONCAT_SVSVS(&html, "<tr><td class=\"key\">", &tmp, "</td><td>", &joined_value, "</td></tr>");
					zval_ptr_dtor(&joined_value);
					DAO_MM_ADD_ENTRY(&html);
				} else {
					DAO_SCONCAT_SVSVS(&html, "<tr><td class=\"key\">", &tmp, "</td><td>", value, "</td></tr>");
					DAO_MM_ADD_ENTRY(&html);
				}
			} ZEND_HASH_FOREACH_END();
		}

		dao_concat_self_str(&html, SL("</table></div>"));
		DAO_MM_ADD_ENTRY(&html);

		/**
		 * Print _SERVER superglobal
		 */
		dao_concat_self_str(&html, SL("<div class=\"card\" style=\"margin-top:15px\"><div class=\"card-header\">_SERVER</div><table class=\"table table-striped\">"));
		DAO_MM_ADD_ENTRY(&html);
		dao_concat_self_str(&html, SL("<tr><th class=\"col-md-4\">Key</th><th class=\"col-md-8\">Value</th></tr>"));
		DAO_MM_ADD_ENTRY(&html);

		_SERVER = dao_get_global_str(SL("_SERVER"));

		if (Z_TYPE_P(_SERVER) == IS_ARRAY) {
			ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(_SERVER), idx, str_key, value) {
				zval tmp = {}, dumped_argument = {};
				if (str_key) {
					ZVAL_STR(&tmp, str_key);
				} else {
					ZVAL_LONG(&tmp, idx);
				}
				DAO_MM_CALL_METHOD(&dumped_argument, getThis(), "_getvardump", value);
				DAO_SCONCAT_SVSVS(&html, "<tr><td class=\"key\">", &tmp, "</td><td>", &dumped_argument, "</td></tr>");
				zval_ptr_dtor(&dumped_argument);
				DAO_MM_ADD_ENTRY(&html);
			} ZEND_HASH_FOREACH_END();
		}

		dao_concat_self_str(&html, SL("</table></div>"));
		DAO_MM_ADD_ENTRY(&html);

		/**
		 * Print _SESSION superglobal
		 */
		dao_concat_self_str(&html, SL("<div class=\"card\" style=\"margin-top:15px\"><div class=\"card-header\">_SESSION</div><table class=\"table table-striped\">"));
		DAO_MM_ADD_ENTRY(&html);
		dao_concat_self_str(&html, SL("<tr><th class=\"col-md-4\">Key</th><th class=\"col-md-8\">Value</th></tr>"));
		DAO_MM_ADD_ENTRY(&html);

		_SESSION = dao_get_global_str(SL("_SESSION"));

		if (Z_TYPE_P(_SESSION) == IS_ARRAY) {
			ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(_SESSION), idx, str_key, value) {
				zval tmp = {}, dumped_argument = {};
				if (str_key) {
					ZVAL_STR(&tmp, str_key);
				} else {
					ZVAL_LONG(&tmp, idx);
				}
				DAO_MM_CALL_METHOD(&dumped_argument, getThis(), "_getvardump", value);
				DAO_SCONCAT_SVSVS(&html, "<tr><td class=\"key\">", &tmp, "</td><td>", &dumped_argument, "</td></tr>");
				zval_ptr_dtor(&dumped_argument);
				DAO_MM_ADD_ENTRY(&html);
			} ZEND_HASH_FOREACH_END();
		}

		dao_concat_self_str(&html, SL("</table></div>"));
		DAO_MM_ADD_ENTRY(&html);

		/**
		 * Print _COOKIE superglobal
		 */
		dao_concat_self_str(&html, SL("<div class=\"card\" style=\"margin-top:15px\"><div class=\"card-header\">_COOKIE</div><table class=\"table table-striped\">"));
		DAO_MM_ADD_ENTRY(&html);
		dao_concat_self_str(&html, SL("<tr><th class=\"col-md-4\">Key</th><th class=\"col-md-8\">Value</th></tr>"));
		DAO_MM_ADD_ENTRY(&html);

		_COOKIE = dao_get_global_str(SL("_COOKIE"));

		if (Z_TYPE_P(_COOKIE) == IS_ARRAY) {
			ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(_COOKIE), idx, str_key, value) {
				zval tmp = {}, dumped_argument = {};
				if (str_key) {
					ZVAL_STR(&tmp, str_key);
				} else {
					ZVAL_LONG(&tmp, idx);
				}
				DAO_MM_CALL_METHOD(&dumped_argument, getThis(), "_getvardump", value);
				DAO_SCONCAT_SVSVS(&html, "<tr><td class=\"key\">", &tmp, "</td><td>", &dumped_argument, "</td></tr>");
				zval_ptr_dtor(&dumped_argument);
				DAO_MM_ADD_ENTRY(&html);
			} ZEND_HASH_FOREACH_END();
		}

		dao_concat_self_str(&html, SL("</table></div>"));
		DAO_MM_ADD_ENTRY(&html);

		/**
		 * Show included files
		 */
		DAO_MM_CALL_FUNCTION(&files, "get_included_files");
		DAO_MM_ADD_ENTRY(&files);
		dao_concat_self_str(&html, SL("<div class=\"card\" style=\"margin-top:15px\"><div class=\"card-header\">INCLUDE</div><table class=\"table table-striped\">"));
		DAO_MM_ADD_ENTRY(&html);
		dao_concat_self_str(&html, SL("<tr><th class=\"col-md-4\">#</th><th class=\"col-md-8\">Path</th></tr>"));
		DAO_MM_ADD_ENTRY(&html);

		ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL(files), idx, str_key, value) {
			zval tmp = {};
			if (str_key) {
				ZVAL_STR(&tmp, str_key);
			} else {
				ZVAL_LONG(&tmp, idx);
			}

			DAO_SCONCAT_SVSVS(&html, "<tr><td>", &tmp, "</th><td>", value, "</td></tr>");
			DAO_MM_ADD_ENTRY(&html);
		} ZEND_HASH_FOREACH_END();

		dao_concat_self_str(&html, SL("</table></div>"));
		DAO_MM_ADD_ENTRY(&html);

		DAO_MM_CALL_CE_STATIC(&di, dao_di_ce, "getdefault");
		DAO_MM_ADD_ENTRY(&di);

		/**
		 * Router
		 */
		dao_concat_self_str(&html, SL("<div class=\"card\" style=\"margin-top:15px\"><div class=\"card-header\">Router</div><table class=\"table table-striped\">"));
		DAO_MM_ADD_ENTRY(&html);
		dao_concat_self_str(&html, SL("<tr><th class=\"col-md-4\">Pattern</th><th class=\"col-md-8\">Paths</th><th>Methods</th></tr>"));
		DAO_MM_ADD_ENTRY(&html);

		if (Z_TYPE(di) == IS_OBJECT) {
			ZVAL_STR(&service_name, IS(router));
			DAO_MM_CALL_METHOD(&router, &di, "getshared", &service_name);
			DAO_MM_ADD_ENTRY(&router);
			if (Z_TYPE(router) == IS_OBJECT) {
				DAO_MM_CALL_METHOD(&routes, &router, "getroutes");
				DAO_MM_ADD_ENTRY(&routes);

				ZEND_HASH_FOREACH_VAL(Z_ARRVAL(routes), route) {
					zval pattern = {}, paths = {}, methods = {}, dumped_paths = {}, dumped_methods = {};
					DAO_MM_CALL_METHOD(&pattern, route, "getpattern");
					DAO_MM_ADD_ENTRY(&pattern);
					DAO_MM_CALL_METHOD(&paths, route, "getpaths");
					DAO_MM_ADD_ENTRY(&paths);
					DAO_MM_CALL_METHOD(&methods, route, "gethttpmethods");
					DAO_MM_ADD_ENTRY(&methods);

					DAO_MM_CALL_METHOD(&dumped_paths, getThis(), "_getvardump", &paths);
					DAO_MM_ADD_ENTRY(&dumped_paths);
					DAO_MM_CALL_METHOD(&dumped_methods, getThis(), "_getvardump", &methods);

					DAO_SCONCAT_SVSVSVS(&html, "<tr><td>", &pattern, "</th><td>", &dumped_paths, "</td><td>", &dumped_methods, "</td></tr>");
					DAO_MM_ADD_ENTRY(&html);
					zval_ptr_dtor(&dumped_methods);
				} ZEND_HASH_FOREACH_END();
			}
		}

		dao_concat_self_str(&html, SL("</table></div>"));
		DAO_MM_ADD_ENTRY(&html);

		/**
		 * Loader
		 */
		dao_concat_self_str(&html, SL("<div class=\"card\" style=\"margin-top:15px\"><div class=\"card-header\">Loader</div><table class=\"table table-striped\">"));
		DAO_MM_ADD_ENTRY(&html);
		dao_concat_self_str(&html, SL("<tr><th class=\"col-md-4\">#</th><th class=\"col-md-8\">Value</th></tr>"));
		DAO_MM_ADD_ENTRY(&html);

		dao_read_static_property_ce(&loader, dao_loader_ce, SL("_default"), PH_READONLY);
		if (Z_TYPE(loader) == IS_OBJECT) {
			DAO_MM_CALL_METHOD(&loader_value, &loader, "getdirs");
			DAO_MM_ADD_ENTRY(&loader_value);
			DAO_MM_CALL_METHOD(&dumped_loader_value, getThis(), "_getvardump", &loader_value);
			DAO_SCONCAT_SVS(&html, "<tr><td>Directories Registered</th><td>", &dumped_loader_value, "</td></tr>");
			zval_ptr_dtor(&dumped_loader_value);
			DAO_MM_ADD_ENTRY(&html);

			DAO_MM_CALL_METHOD(&loader_value, &loader, "getclasses");
			DAO_MM_ADD_ENTRY(&loader_value);
			DAO_MM_CALL_METHOD(&dumped_loader_value, getThis(), "_getvardump", &loader_value);
			DAO_SCONCAT_SVS(&html, "<tr><td>Class-map Registered</th><td>", &dumped_loader_value, "</td></tr>");
			zval_ptr_dtor(&dumped_loader_value);
			DAO_MM_ADD_ENTRY(&html);

			DAO_MM_CALL_METHOD(&loader_value, &loader, "getprefixes");
			DAO_MM_ADD_ENTRY(&loader_value);
			DAO_MM_CALL_METHOD(&dumped_loader_value, getThis(), "_getvardump", &loader_value);
			DAO_SCONCAT_SVS(&html, "<tr><td>Prefixes Registered</th><td>", &dumped_loader_value, "</td></tr>");
			zval_ptr_dtor(&dumped_loader_value);
			DAO_MM_ADD_ENTRY(&html);

			DAO_MM_CALL_METHOD(&loader_value, &loader, "getnamespaces");
			DAO_MM_ADD_ENTRY(&loader_value);
			DAO_MM_CALL_METHOD(&dumped_loader_value, getThis(), "_getvardump", &loader_value);
			DAO_SCONCAT_SVS(&html, "<tr><td>Namespaces Registered</th><td>", &dumped_loader_value, "</td></tr>");
			zval_ptr_dtor(&dumped_loader_value);
			DAO_MM_ADD_ENTRY(&html);
		}

		dao_concat_self_str(&html, SL("</table></div>"));
		DAO_MM_ADD_ENTRY(&html);

		/**
		 * Print extra variables passed to the component
		 */
		dao_read_property(&data_vars, getThis(), SL("_data"), PH_NOISY|PH_READONLY);
		dao_concat_self_str(&html, SL("<div class=\"card\" style=\"margin-top:15px\"><div class=\"card-header\">Variables</div><table class=\"table table-striped\">"));
		DAO_MM_ADD_ENTRY(&html);
		dao_concat_self_str(&html, SL("<tr><th class=\"col-md-4\">Key</th><th class=\"col-md-8\">Value</th></tr>"));
		DAO_MM_ADD_ENTRY(&html);

		if (Z_TYPE(data_vars) == IS_ARRAY) {
			ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL(data_vars), idx, str_key, data_var) {
				zval tmp = {}, variable = {}, dumped_argument = {};
				if (str_key) {
					ZVAL_STR(&tmp, str_key);
				} else {
					ZVAL_LONG(&tmp, idx);
				}

				dao_array_fetch_long(&variable, data_var, 0, PH_NOISY|PH_READONLY);

				DAO_MM_CALL_METHOD(&dumped_argument, getThis(), "_getvardump", &variable);
				DAO_SCONCAT_SVSVS(&html, "<tr><td class=\"key\">", &tmp, "</td><td>", &dumped_argument, "</td></tr>");
				zval_ptr_dtor(&dumped_argument);
				DAO_MM_ADD_ENTRY(&html);
			} ZEND_HASH_FOREACH_END();
		}

		dao_concat_self_str(&html, SL("</table></div>"));
		DAO_MM_ADD_ENTRY(&html);

		// Logs
		dao_concat_self_str(&html, SL("<div class=\"card\" style=\"margin-top:15px\"><div class=\"card-header\">Logs</div><table class=\"table table-striped\">"));
		DAO_MM_ADD_ENTRY(&html);
		dao_concat_self_str(&html, SL("<tr><th>Message</th></tr>"));
		DAO_MM_ADD_ENTRY(&html);

		dao_read_static_property_ce(&logs, dao_debug_ce, SL("_logs"), PH_READONLY);
		if (Z_TYPE(logs) == IS_ARRAY) {
			ZEND_HASH_FOREACH_VAL(Z_ARRVAL(logs), log) {
				zval dumped_argument = {};

				DAO_MM_CALL_METHOD(&dumped_argument, getThis(), "_getvardump", log);
				DAO_SCONCAT_SVS(&html, "<tr><td>", &dumped_argument, "</td></tr>");
				zval_ptr_dtor(&dumped_argument);
				DAO_MM_ADD_ENTRY(&html);
			} ZEND_HASH_FOREACH_END();
		}

		dao_concat_self_str(&html, SL("</table></div>"));
		DAO_MM_ADD_ENTRY(&html);
	}

	/**
	 * Get Javascript sources
	 */
	DAO_MM_CALL_METHOD(&js_sources, getThis(), "getjssources");
	DAO_SCONCAT_SVS(&html, "</div>", &js_sources, "</body></html>");
	zval_ptr_dtor(&js_sources);
	DAO_MM_ADD_ENTRY(&html);

	/**
	 * Print the HTML, @TODO, add an option to store the html
	 */
	zend_print_zval(&html, 0);

	/**
	 * Unlock the exception renderer
	 */
	zend_update_static_property_bool(dao_debug_ce, SL("_isActive"), 0);
	RETURN_MM_TRUE;
}

/**
 * Handles user-defined error
 *
 * @param int $severity
 * @param string $message
 * @param string $file
 * @param string $line
 * @param array $context
 * @return boolean
 */
PHP_METHOD(Dao_Debug, onUserDefinedError){

	zval *severity, *message, *file = NULL, *line = NULL, *context = NULL, previous = {}, exception = {};
	zend_class_entry *default_exception_ce;

	dao_fetch_params(0, 2, 3, &severity, &message, &file, &line, &context);

	if (!file) {
		file = &DAO_GLOBAL(z_null);
	}

	if (!line) {
		line = &DAO_GLOBAL(z_null);
	}

	if (context && Z_TYPE_P(context) == IS_ARRAY) {
		if (
			!dao_array_isset_fetch_str(&previous, context, SL("e"), PH_READONLY) ||
			Z_TYPE(previous) != IS_OBJECT ||
			!instanceof_function(Z_OBJCE_P(&previous), zend_exception_get_default())
		) {
			ZVAL_NULL(&previous);
		}
	} else {
		ZVAL_NULL(&previous);
	}

	default_exception_ce = zend_get_error_exception();

	object_init_ex(&exception, default_exception_ce);

	DAO_CALL_METHOD(NULL, &exception, "__construct", message, &DAO_GLOBAL(z_zero), severity, file, line, &previous);

	zend_throw_exception_object(&exception);

	RETURN_TRUE;
}

/**
 * Handles user-defined error
 *
 * @return boolean
 */
PHP_METHOD(Dao_Debug, onShutdown){

	zval error = {}, message = {}, type = {}, file = {}, line = {}, exception = {};
	zend_class_entry *default_exception_ce;

	DAO_CALL_FUNCTION(&error, "error_get_last");

	if (
		!dao_array_isset_fetch_str(&message, &error, SL("message"), PH_READONLY) ||
		!dao_array_isset_fetch_str(&type, &error, SL("type"), PH_READONLY) ||
		!dao_array_isset_fetch_str(&file, &error, SL("file"), PH_READONLY) ||
		!dao_array_isset_fetch_str(&line, &error, SL("line"), PH_READONLY)

	) {
		default_exception_ce = zend_get_error_exception();

		object_init_ex(&exception, default_exception_ce);

		DAO_CALL_METHOD(NULL, &exception, "__construct", &message, &DAO_GLOBAL(z_zero), &type, &file, &line);

		zend_throw_exception_object(&exception);
	}
	zval_ptr_dtor(&error);
}

/**
 * Returns the number of lines deplayed before the error line
 *
 * @brief int \Dao\Debug::getLinesBeforeContext(void)
 * @return int
 */
PHP_METHOD(Dao_Debug, getLinesBeforeContext) {
	RETURN_MEMBER(getThis(), "_beforeContext");
}

/**
 * Sets the number of lines deplayed before the error line
 *
 * @brief \Dao\Debug \Dao\Debug::setLinesBeforeContext(int $lines)
 * @param int $lines
 * @return \Dao\Debug
 */
PHP_METHOD(Dao_Debug, setLinesBeforeContext) {

	zval *lines;

	dao_fetch_params(0, 1, 0, &lines);
	DAO_ENSURE_IS_LONG(lines);

	dao_update_property(getThis(), SL("_beforeContext"), lines);
	RETURN_THIS();
}

/**
 * Returns the number of lines deplayed after the error line
 *
 * @brief int \Dao\Debug::getLinesAfterContext(void)
 * @return int
 */
PHP_METHOD(Dao_Debug, getLinesAfterContext) {
	RETURN_MEMBER(getThis(), "_afterContext");
}

/**
 * Sets the number of lines deplayed after the error line
 *
 * @brief \Dao\Debug \Dao\Debug::setLinesAfterContext(int $lines)
 * @param int $lines
 * @return \Dao\Debug
 */
PHP_METHOD(Dao_Debug, setLinesAfterContext) {

	zval *lines;

	dao_fetch_params(0, 1, 0, &lines);
	DAO_ENSURE_IS_LONG(lines);

	dao_update_property(getThis(), SL("_afterContext"), lines);
	RETURN_THIS();
}

/**
 * Sets logger
 *
 * @param Dao\Logger\AdapterInterface $logger
 */
PHP_METHOD(Dao_Debug, setLogger){

	zval *logger;

	dao_fetch_params(0, 1, 0, &logger);
	if (logger) {
		DAO_VERIFY_INTERFACE_EX(logger, dao_logger_adapterinterface_ce, dao_debug_exception_ce);
	} else {
		logger = &DAO_GLOBAL(z_null);
	}

	dao_update_static_property_ce(dao_debug_ce, SL("_logger"), logger);
}

/**
 * Enable simple debug mode
 *
 * @param Dao\Logger\AdapterInterface $logger
 */
PHP_METHOD(Dao_Debug, enable){

	zval *logger = NULL;

	dao_fetch_params(0, 0, 1, &logger);
	if (logger) {
		DAO_VERIFY_INTERFACE_EX(logger, dao_logger_adapterinterface_ce, dao_debug_exception_ce);
		dao_update_static_property_ce(dao_debug_ce, SL("_logger"), logger);
	}

	DAO_GLOBAL(debug).enable_debug = 1;
}

/**
 * Disable simple debug mode
 *
 */
PHP_METHOD(Dao_Debug, disable){

	if (unlikely(DAO_GLOBAL(debug).enable_debug)) {
		zval listen = {};
		dao_read_static_property_ce(&listen, dao_debug_ce, SL("_listen"), PH_READONLY);
		if (!zend_is_true(&listen)) {
			zval logs = {};
			dao_read_static_property_ce(&logs, dao_debug_ce, SL("_logs"), PH_READONLY);
			dao_debug_print_r(&logs);
		}
	}
	DAO_GLOBAL(debug).enable_debug = 0;
	RETURN_STATIC_MEMBER(dao_debug_ce, "_logs");
}

/**
 * Check if debug mode
 *
 * @return boolean
 */
PHP_METHOD(Dao_Debug, isEnable){

	RETURN_BOOL(DAO_GLOBAL(debug).enable_debug);
}

/**
 * Logs messages
 *
 * @param string $message
 * @param mixed $type
 * @param array $context
 */
PHP_METHOD(Dao_Debug, log){

	zval *message, *_type = NULL, *context = NULL, type = {},  log_type = {}, log = {}, logger = {};

	dao_fetch_params(1, 1, 2, &message, &_type, &context);

	if (!_type || Z_TYPE_P(_type) == IS_NULL) {
		ZVAL_LONG(&type, DAO_LOGGER_DEBUG);
	} else {
		ZVAL_COPY_VALUE(&type, _type);
	}

	if (!context) {
		context = &DAO_GLOBAL(z_null);
	}


	if (Z_TYPE_P(message) == IS_STRING) {
		DAO_MM_CALL_CE_STATIC(&log_type, dao_logger_ce, "gettypestring", &type);
		DAO_MM_ADD_ENTRY(&log_type);
		DAO_CONCAT_SVSV(&log, "[", &log_type, "] ", message);
		DAO_MM_ADD_ENTRY(&log);
		dao_update_static_property_array_append_ce(dao_debug_ce, SL("_logs"), &log);
	} else {
		dao_update_static_property_array_append_ce(dao_debug_ce, SL("_logs"), message);
	}

	dao_read_static_property_ce(&logger, dao_debug_ce, SL("_logger"), PH_READONLY);
	if (Z_TYPE(logger) != IS_NULL) {
		DAO_MM_VERIFY_INTERFACE_EX(&logger, dao_logger_adapterinterface_ce, dao_debug_exception_ce);
		DAO_MM_CALL_METHOD(NULL, &logger, "log", &type, message, context);
	}

	RETURN_MM();
}

/**
 * Dumps a string representation of variable to output
 *
 * @param mixed $var
 */
PHP_METHOD(Dao_Debug, dumpVar){

	zval *var, *level = NULL;
	int lvl = 0;

	dao_fetch_params(0, 1, 1, &var, &level);

	if (level && Z_LVAL_P(level) > 0) {
		lvl = Z_LVAL_P(level);
	}

	zend_printf("File: %s, Line: %d\n", zend_get_executed_filename(), zend_get_executed_lineno());
	php_debug_zval_dump(var, lvl);
}