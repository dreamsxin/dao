
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

#include "text.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"
#include "kernel/string.h"
#include "kernel/array.h"
#include "kernel/operators.h"
#include "kernel/concat.h"

/**
 * Dao\Text
 *
 * Provides utilities to work with texts
 */
zend_class_entry *dao_text_ce;

PHP_METHOD(Dao_Text, camelize);
PHP_METHOD(Dao_Text, uncamelize);
PHP_METHOD(Dao_Text, increment);
PHP_METHOD(Dao_Text, decrement);
PHP_METHOD(Dao_Text, random);
PHP_METHOD(Dao_Text, startsWith);
PHP_METHOD(Dao_Text, endsWith);
PHP_METHOD(Dao_Text, lower);
PHP_METHOD(Dao_Text, upper);
PHP_METHOD(Dao_Text, bytes);
PHP_METHOD(Dao_Text, reduceSlashes);
PHP_METHOD(Dao_Text, concat);
PHP_METHOD(Dao_Text, underscore);
PHP_METHOD(Dao_Text, humanize);
PHP_METHOD(Dao_Text, limitChars);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_text_camelize, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_text_uncamelize, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_text_increment, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
	ZEND_ARG_INFO(0, separator)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_text_decrement, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
	ZEND_ARG_INFO(0, separator)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_text_random, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, type, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, length, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_text_startswith, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, start, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, ignoreCase, _IS_BOOL, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_text_endswith, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, end, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, ignoreCase, _IS_BOOL, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_text_lower, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_text_upper, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_text_bytes, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, size, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, forceUnit, IS_STRING, 1)
	ZEND_ARG_TYPE_INFO(0, format, IS_STRING, 1)
	ZEND_ARG_TYPE_INFO(0, si, _IS_BOOL, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_text_reduceslashes, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_text_concat, 0, 0, 3)
	ZEND_ARG_TYPE_INFO(0, separator, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, strA, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, strB, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_text_underscore, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_text_humanize, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_text_limitchars, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, limit, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, end, IS_STRING, 1)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_text_method_entry[] = {
	PHP_ME(Dao_Text, camelize, arginfo_dao_text_camelize, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Text, uncamelize, arginfo_dao_text_uncamelize, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Text, increment, arginfo_dao_text_increment, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Text, decrement, arginfo_dao_text_decrement, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Text, random, arginfo_dao_text_random, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Text, startsWith, arginfo_dao_text_startswith, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Text, endsWith, arginfo_dao_text_endswith, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Text, lower, arginfo_dao_text_lower, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Text, upper, arginfo_dao_text_upper, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Text, bytes, arginfo_dao_text_bytes, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Text, reduceSlashes, arginfo_dao_text_reduceslashes, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Text, concat, arginfo_dao_text_concat, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Text, underscore, arginfo_dao_text_underscore, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Text, humanize, arginfo_dao_text_humanize, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Text, limitChars, arginfo_dao_text_limitchars, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Text initializer
 */
DAO_INIT_CLASS(Dao_Text){

	DAO_REGISTER_CLASS(Dao, Text, text, dao_text_method_entry, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	zend_declare_class_constant_long(dao_text_ce, SL("RANDOM_ALNUM"),   DAO_TEXT_RANDOM_ALNUM  );
	zend_declare_class_constant_long(dao_text_ce, SL("RANDOM_ALPHA"),   DAO_TEXT_RANDOM_ALPHA  );
	zend_declare_class_constant_long(dao_text_ce, SL("RANDOM_HEXDEC"),  DAO_TEXT_RANDOM_HEXDEC );
	zend_declare_class_constant_long(dao_text_ce, SL("RANDOM_NUMERIC"), DAO_TEXT_RANDOM_NUMERIC);
	zend_declare_class_constant_long(dao_text_ce, SL("RANDOM_NOZERO"),  DAO_TEXT_RANDOM_NOZERO );

	return SUCCESS;
}

/**
 * Converts strings to camelize style
 *
 *<code>
 *	echo Dao\Text::camelize('coco_bongo'); //CocoBongo
 *</code>
 *
 * @param string $str
 * @return string
 */
PHP_METHOD(Dao_Text, camelize){

	zval *str;

	dao_fetch_params(0, 1, 0, &str);

	dao_camelize(return_value, str);
}

/**
 * Uncamelize strings which are camelized
 *
 *<code>
 *	echo Dao\Text::uncamelize('CocoBongo'); //coco_bongo
 *</code>
 *
 * @param string $str
 * @return string
 */
PHP_METHOD(Dao_Text, uncamelize){

	zval *str;

	dao_fetch_params(0, 1, 0, &str);

	dao_uncamelize(return_value, str);
}

/**
 * Adds a number to a string or increment that number if it already is defined
 *
 *<code>
 *	echo Dao\Text::increment("a"); // "a_1"
 *	echo Dao\Text::increment("a_1"); // "a_2"
 *</code>
 *
 * @param string $str
 * @param string|int $separator
 * @return string
 */
PHP_METHOD(Dao_Text, increment){

	zval *str, *separator = NULL, sep = {}, parts = {}, number = {}, first_part = {};

	dao_fetch_params(0, 1, 1, &str, &separator);

	if (!separator || DAO_IS_EMPTY(separator)) {
		ZVAL_STRING(&sep, "_");
	} else {
		if (Z_TYPE_P(separator) != IS_LONG && Z_TYPE_P(separator) != IS_STRING) {
			DAO_SEPARATE_PARAM(separator);
			convert_to_string(separator);
		}
		ZVAL_COPY_VALUE(&sep, separator);
	}

	if (Z_TYPE(sep) == IS_LONG) {
		dao_substr(&first_part, str, 0, Z_STRLEN_P(str) - Z_LVAL(sep));
		dao_substr(&number, str, Z_STRLEN(first_part), 0);

		dao_increment(&number);
		DAO_CONCAT_VV(return_value, &first_part, &number);
	} else {
		dao_fast_explode(&parts, &sep, str);
		if (dao_array_isset_fetch_long(&number, &parts, 1, PH_READONLY)) {
			dao_increment(&number);
		} else {
			ZVAL_COPY_VALUE(&number, &DAO_GLOBAL(z_one));
		}

		dao_array_fetch_long(&first_part, &parts, 0, PH_NOISY|PH_READONLY);
		DAO_CONCAT_VVV(return_value, &first_part, &sep, &number);
	}
}

/**
 * Adds a number to a string or decrement that number if it already is defined
 *
 *<code>
 *	echo Dao\Text::decrement("a"); // "a_1"
 *	echo Dao\Text::decrement("a_1"); // "a_0"
 *</code>
 *
 * @param string $str
 * @param string|int $separator
 * @return string
 */
PHP_METHOD(Dao_Text, decrement){

	zval *str, *separator = NULL, sep = {}, parts = {}, number = {}, first_part = {};

	dao_fetch_params(0, 1, 1, &str, &separator);

	if (!separator || DAO_IS_EMPTY(separator)) {
		ZVAL_STRING(&sep, "_");
	} else {
		ZVAL_COPY_VALUE(&sep, separator);
	}

	if (Z_TYPE(sep) == IS_LONG) {
		dao_substr(&first_part, str, 0, Z_STRLEN_P(str) - Z_LVAL(sep));
		dao_substr(&number, str, Z_STRLEN(first_part), 0);

		dao_decrement(&number);
		DAO_CONCAT_VV(return_value, &first_part, &number);
	} else {
		dao_fast_explode(&parts, &sep, str);
		if (dao_array_isset_fetch_long(&number, &parts, 1, PH_READONLY)) {
			dao_decrement(&number);
		} else {
			ZVAL_LONG(&number, Z_LVAL(DAO_GLOBAL(z_one)));
		}

		dao_array_fetch_long(&first_part, &parts, 0, PH_NOISY|PH_READONLY);
		DAO_CONCAT_VVV(return_value, &first_part, &sep, &number);
	}
}

/**
 * Generates a random string based on the given type. Type is one of the RANDOM_* constants
 *
 *<code>
 *	echo Dao\Text::random(Dao\Text::RANDOM_ALNUM); //"aloiwkqz"
 *</code>
 *
 * @param int $type
 * @param int $length
 * @return string
 */
PHP_METHOD(Dao_Text, random){

	zval *type, *length = NULL, len = {};

	dao_fetch_params(0, 1, 1, &type, &length);

	if (!length) {
		ZVAL_LONG(&len, 8);
		dao_random_string(return_value, type, &len);
	} else {
		dao_random_string(return_value, type, length);
	}
}

/**
 * Check if a string starts with a given string
 *
 *<code>
 *	echo Dao\Text::startsWith("Hello", "He"); // true
 *	echo Dao\Text::startsWith("Hello", "he"); // false
 *	echo Dao\Text::startsWith("Hello", "he", false); // true
 *</code>
 *
 * @param string $str
 * @param string $start
 * @param boolean $ignoreCase
 * @return boolean
 */
PHP_METHOD(Dao_Text, startsWith){

	zval *str, *start, *ignore_case = NULL, *case_sensitive;

	dao_fetch_params(0, 2, 1, &str, &start, &ignore_case);

	if (!ignore_case) {
		case_sensitive = &DAO_GLOBAL(z_false);
	}
	else {
		case_sensitive = zend_is_true(ignore_case) ? &DAO_GLOBAL(z_false) : &DAO_GLOBAL(z_true);
	}

	RETURN_BOOL(dao_start_with(str, start, case_sensitive));
}

/**
 * Check if a string ends with a given string
 *
 *<code>
 *	echo Dao\Text::endsWith("Hello", "llo"); // true
 *	echo Dao\Text::endsWith("Hello", "LLO"); // false
 *	echo Dao\Text::endsWith("Hello", "LLO", false); // true
 *</code>
 *
 * @param string $str
 * @param string $end
 * @param boolean $ignoreCase
 * @return boolean
 */
PHP_METHOD(Dao_Text, endsWith){

	zval *str, *end, *ignore_case = NULL, *case_sensitive;

	dao_fetch_params(0, 2, 1, &str, &end, &ignore_case);

	if (!ignore_case) {
		case_sensitive = &DAO_GLOBAL(z_false);
	}
	else {
		case_sensitive = zend_is_true(ignore_case) ? &DAO_GLOBAL(z_false) : &DAO_GLOBAL(z_true);
	}

	RETURN_BOOL(dao_end_with(str, end, case_sensitive));
}

/**
 * Lowercases a string, this function makes use of the mbstring extension if available
 *
 * @param string $str
 * @return string
 */
PHP_METHOD(Dao_Text, lower){

	zval *str;

	dao_fetch_params(0, 1, 0, &str);

	/**
	 * 'lower' checks for the mbstring extension to make a correct lowercase
	 * transformation
	 */
#ifdef DAO_USE_PHP_MBSTRING
	dao_strtolower(return_value, str);
#else
	if (dao_function_exists_ex(SL("mb_strtolower")) == SUCCESS) {
		DAO_RETURN_CALL_FUNCTION("mb_strtolower", str);
		return;
	}

	dao_fast_strtolower(return_value, str);
#endif
}

/**
 * Uppercases a string, this function makes use of the mbstring extension if available
 *
 * @param string $str
 * @return string
 */
PHP_METHOD(Dao_Text, upper){

	zval *str;

	dao_fetch_params(0, 1, 0, &str);

#ifdef DAO_USE_PHP_MBSTRING
	dao_strtoupper(return_value, str);
#else
	/**
	 * 'upper' checks for the mbstring extension to make a correct lowercase
	 * transformation
	 */
	if (dao_function_exists_ex(SL("mb_strtoupper")) == SUCCESS) {
		DAO_RETURN_CALL_FUNCTION("mb_strtoupper", str);
		return;
	}

	dao_fast_strtoupper(return_value, str);
#endif
}

/**
 * Returns human readable sizes
 *
 * @param int $size
 * @param string $forceUnit
 * @param string $format
 * @param boolean $si
 * @return string
 */
PHP_METHOD(Dao_Text, bytes){

	zval *_z_size, *_z_force_unit = NULL, *_format = NULL, z_size = {}, z_force_unit = {}, *si = NULL;
	char *format, *output;
	char *force_unit;
	const char **units;
	const char *units1[] = {"B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};
	const char *units2[] = {"B", "KiB", "MiB", "GiB", "TiB", "PiB", "EiB", "ZiB", "YiB"};
	double size;
	int mod, power = 0, found = 0, i, j = 0;

	dao_fetch_params(0, 1, 3, &_z_size, &_z_force_unit, &_format, &si);

	ZVAL_COPY_VALUE(&z_size, _z_size);

	size = Z_LVAL(z_size);

	if (_z_force_unit && DAO_IS_NOT_EMPTY(_z_force_unit)) {
		ZVAL_COPY_VALUE(&z_force_unit, _z_force_unit);
	} else {
		ZVAL_NULL(&z_force_unit);
	}

	if (_format && DAO_IS_NOT_EMPTY(_format)) {
		format = Z_STRVAL_P(_format);
	} else {
		format = "%01.2f %s";
	}

	if (!si) {
		si = &DAO_GLOBAL(z_true);
	}

	if (!zend_is_true(si) || (DAO_IS_NOT_EMPTY(&z_force_unit) && dao_memnstr_str(&z_force_unit, SL("i")))) {
		units = units2;
		mod = 1024;
	} else {
		units = units1;
		mod = 1000;
	}

	if (DAO_IS_NOT_EMPTY(&z_force_unit)) {
		force_unit = Z_STRVAL(z_force_unit);
		for (i = 0; i < sizeof(units); i++)
		{
			if (strcasecmp(force_unit, units[i]) == 0) {
				found = 1;
				power = i;
				break;
			}
		}
	}

	if (found) {
		while(j < power) {
			size /= mod;
			j++;
		}
	} else {
		while(size > mod) {
			size /= mod;
			power++;
		}
	}

	dao_spprintf(&output, 0, format, size, units[power]);
	RETVAL_STRING(output);
	efree(output);
}

/**
 * Reduces multiple slashes in a string to single slashes
 *
 * <code>
 *    echo Dao\Text::reduceSlashes("foo//bar/baz"); // foo/bar/baz
 *    echo Dao\Text::reduceSlashes("http://foo.bar///baz/buz"); // http://foo.bar/baz/buz
 * </code>
 */
PHP_METHOD(Dao_Text, reduceSlashes){

	zval *str, pattern = {}, replacement = {};

	dao_fetch_params(0, 1, 0, &str);

	ZVAL_STRING(&pattern, "#(?<!:)//+#");
	ZVAL_STRING(&replacement, "/");

	dao_fast_preg_replace(return_value, &pattern, &replacement, str);
	zval_ptr_dtor(&pattern);
	zval_ptr_dtor(&replacement);
}

/**
 * Concatenates strings using the separator only once without duplication in places concatenation
 *
 * <code>
 *    $str = Dao\Text::concat("/", "/tmp/", "/folder_1/", "/folder_2", "folder_3/");
 *    echo $str; // /tmp/folder_1/folder_2/folder_3/
 * </code>
 *
 * @param string separator
 * @param string a
 * @param string b
 * @param string ...N
 */
PHP_METHOD(Dao_Text, concat){

	zval *separator, *a, *b, *args, str = {}, a_trimmed = {}, str_trimmed = {};
	uint32_t i;

	dao_fetch_params(1, 3, 0, &separator, &a, &b);

	if (ZEND_NUM_ARGS() > 3) {
		args = (zval *)safe_emalloc(ZEND_NUM_ARGS(), sizeof(zval), 0);
		if (zend_get_parameters_array_ex(ZEND_NUM_ARGS(), args) == FAILURE) {
			efree(args);
			WRONG_PARAM_COUNT;
		}

		for (i = 2; i < ZEND_NUM_ARGS(); i++) {
			zval trimmed = {};
			ZVAL_STR(&trimmed, dao_trim(&args[i], separator, DAO_TRIM_BOTH));

			DAO_SCONCAT_VV(&str, &trimmed, separator);
			zval_ptr_dtor(&trimmed);
			DAO_MM_ADD_ENTRY(&str);
		}
		efree(args);
	} else {
		DAO_MM_ZVAL_COPY(&str, b);
	}

	ZVAL_STR(&a_trimmed, dao_trim(a, separator, DAO_TRIM_RIGHT));
	ZVAL_STR(&str_trimmed, dao_trim(&str, separator, DAO_TRIM_LEFT));

	DAO_CONCAT_VVV(return_value, &a_trimmed, separator, &str_trimmed)
	zval_ptr_dtor(&a_trimmed);
	zval_ptr_dtor(&str_trimmed);
	RETURN_MM();
}

/**
 * Makes a phrase underscored instead of spaced
 *
 * <code>
 *   echo Dao\Text::underscore('look behind'); // 'look_behind'
 *   echo Dao\Text::underscore('Awesome Dao'); // 'Awesome_Dao'
 * </code>
 */
PHP_METHOD(Dao_Text, underscore)
{
	zval *str, trimmed = {}, pattern = {}, replacement = {};

	dao_fetch_params(0, 1, 0, &str);

	ZVAL_STR(&trimmed, dao_trim(str, NULL, DAO_TRIM_BOTH));
	ZVAL_STRING(&pattern, "#\\s+#");
	ZVAL_STRING(&replacement, "_");

	dao_fast_preg_replace(return_value, &pattern, &replacement, &trimmed);
	zval_ptr_dtor(&pattern);
	zval_ptr_dtor(&replacement);
	zval_ptr_dtor(&trimmed);
}

/**
 * Makes an underscored or dashed phrase human-readable
 *
 * <code>
 *   echo Dao\Text::humanize('start-a-horse'); // 'start a horse'
 *   echo Dao\Text::humanize('five_cats'); // 'five cats'
 * </code>
 */
PHP_METHOD(Dao_Text, humanize)
{
	zval *str, trimmed = {}, pattern = {}, replacement = {};

	dao_fetch_params(0, 1, 0, &str);

	ZVAL_STR(&trimmed, dao_trim(str, NULL, DAO_TRIM_BOTH));
	ZVAL_STRING(&pattern, "#[_-]+#");
	ZVAL_STRING(&replacement, " ");

	dao_fast_preg_replace(return_value, &pattern, &replacement, &trimmed);
	zval_ptr_dtor(&pattern);
	zval_ptr_dtor(&replacement);
	zval_ptr_dtor(&trimmed);
}

/**
 * Limits a phrase to a given number of characters.
 *
 * <code>
 *   $text = Dao\Text::limitChars($text, 30);
 * </code>
 *
 * @param string $str
 * @param int $limit
 * @param string $end
 * @return string
 */
PHP_METHOD(Dao_Text, limitChars)
{
	zval *str, *limit, *end = NULL, length = {}, substr = {};

	dao_fetch_params(1, 2, 1, &str, &limit, &end);

	if (!end) {
		end = &DAO_GLOBAL(z_null);
	}

	if (DAO_IS_EMPTY(str) || DAO_LE_LONG(limit, 0)) {
		RETURN_MM_CTOR(str);
	}

	DAO_MM_CALL_FUNCTION(&length, "mb_strlen", str);

	if (DAO_GE(limit, &length)) {
		RETURN_MM_CTOR(str);
	}

	DAO_MM_CALL_FUNCTION(&substr, "mb_substr", str, &DAO_GLOBAL(z_zero), limit);
	DAO_MM_ADD_ENTRY(&substr);
	DAO_CONCAT_VV(return_value, &substr, end);
	RETURN_MM();
}
