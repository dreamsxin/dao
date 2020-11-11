
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

#include "cli/color.h"

#include <main/SAPI.h>

#include "kernel/main.h"
#include "kernel/exception.h"
#include "kernel/fcall.h"
#include "kernel/object.h"
#include "kernel/array.h"
#include "kernel/concat.h"
#include "kernel/operators.h"
#include "kernel/string.h"
#include "kernel/debug.h"

/**
 * Dao\Cli\Color
 *
 *<code>
 *
 *	echo Dao\Cli\Color::head("head");
 *	echo Dao\Cli\Color::error("error");
 *	echo Dao\Cli\Color::success("success");
 *	echo Dao\Cli\Color::info("info");
 *</code>
 */
zend_class_entry *dao_cli_color_ce;

PHP_METHOD(Dao_Cli_Color, isSupportedShell);
PHP_METHOD(Dao_Cli_Color, colorize);
PHP_METHOD(Dao_Cli_Color, head);
PHP_METHOD(Dao_Cli_Color, error);
PHP_METHOD(Dao_Cli_Color, success);
PHP_METHOD(Dao_Cli_Color, info);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_cli_color_colorize, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, fg, IS_LONG, 1)
	ZEND_ARG_TYPE_INFO(0, at, IS_LONG, 1)
	ZEND_ARG_TYPE_INFO(0, bg, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_cli_color_head, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_cli_color_error, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_cli_color_success, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_cli_color_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_cli_color_method_entry[] = {
	PHP_ME(Dao_Cli_Color, isSupportedShell, arginfo_empty, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Cli_Color, colorize, arginfo_dao_cli_color_colorize, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Cli_Color, head, arginfo_dao_cli_color_head, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Cli_Color, error, arginfo_dao_cli_color_error, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Cli_Color, success, arginfo_dao_cli_color_success, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Cli_Color, info, arginfo_dao_cli_color_info, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};

/**
 * Dao\Cli\Color initializer
 */
DAO_INIT_CLASS(Dao_Cli_Color){

	DAO_REGISTER_CLASS(Dao\\Cli, Color, cli_color, dao_cli_color_method_entry, 0);

	zend_declare_property_null(dao_cli_color_ce, SL("_fg"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);
	zend_declare_property_null(dao_cli_color_ce, SL("_bg"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);
	zend_declare_property_null(dao_cli_color_ce, SL("_at"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);

	/* constraints */
	zend_declare_class_constant_long(dao_cli_color_ce, SL("FG_BLACK"),			DAO_CLI_COLOR_FG_BLACK);
	zend_declare_class_constant_long(dao_cli_color_ce, SL("FG_DARK_GRAY"),		DAO_CLI_COLOR_FG_DARK_GRAY);
	zend_declare_class_constant_long(dao_cli_color_ce, SL("FG_RED"),			DAO_CLI_COLOR_FG_RED);
	zend_declare_class_constant_long(dao_cli_color_ce, SL("FG_LIGHT_RED"),		DAO_CLI_COLOR_FG_LIGHT_RED);
	zend_declare_class_constant_long(dao_cli_color_ce, SL("FG_GREEN"),			DAO_CLI_COLOR_FG_GREEN);
	zend_declare_class_constant_long(dao_cli_color_ce, SL("FG_LIGHT_GREEN"),	DAO_CLI_COLOR_FG_LIGHT_GREEN);
	zend_declare_class_constant_long(dao_cli_color_ce, SL("FG_BROWN"),			DAO_CLI_COLOR_FG_BROWN);
	zend_declare_class_constant_long(dao_cli_color_ce, SL("FG_YELLOW"),			DAO_CLI_COLOR_FG_YELLOW);
	zend_declare_class_constant_long(dao_cli_color_ce, SL("FG_BLUE"),			DAO_CLI_COLOR_FG_BLUE);
	zend_declare_class_constant_long(dao_cli_color_ce, SL("FG_LIGHT_BLUE"),		DAO_CLI_COLOR_FG_LIGHT_BLUE);
	zend_declare_class_constant_long(dao_cli_color_ce, SL("FG_PURPLE"),			DAO_CLI_COLOR_FG_PURPLE);
	zend_declare_class_constant_long(dao_cli_color_ce, SL("FG_LIGHT_PURPLE"),	DAO_CLI_COLOR_FG_LIGHT_PURPLE);
	zend_declare_class_constant_long(dao_cli_color_ce, SL("FG_CYAN"),			DAO_CLI_COLOR_FG_CYAN);
	zend_declare_class_constant_long(dao_cli_color_ce, SL("FG_LIGHT_CYAN"),		DAO_CLI_COLOR_FG_LIGHT_CYAN);
	zend_declare_class_constant_long(dao_cli_color_ce, SL("FG_LIGHT_GRAY"),		DAO_CLI_COLOR_FG_LIGHT_GRAY);
	zend_declare_class_constant_long(dao_cli_color_ce, SL("FG_WHITE"),			DAO_CLI_COLOR_FG_WHITE);

	zend_declare_class_constant_long(dao_cli_color_ce, SL("BG_BLACK"),			DAO_CLI_COLOR_BG_BLACK);
	zend_declare_class_constant_long(dao_cli_color_ce, SL("BG_RED"),			DAO_CLI_COLOR_BG_RED);
	zend_declare_class_constant_long(dao_cli_color_ce, SL("BG_GREEN"),			DAO_CLI_COLOR_BG_GREEN);
	zend_declare_class_constant_long(dao_cli_color_ce, SL("BG_YELLOW"),			DAO_CLI_COLOR_BG_YELLOW);
	zend_declare_class_constant_long(dao_cli_color_ce, SL("BG_BLUE"),			DAO_CLI_COLOR_BG_BLUE);
	zend_declare_class_constant_long(dao_cli_color_ce, SL("BG_MAGENTA"),		DAO_CLI_COLOR_BG_MAGENTA);
	zend_declare_class_constant_long(dao_cli_color_ce, SL("BG_CYAN"),			DAO_CLI_COLOR_BG_CYAN);
	zend_declare_class_constant_long(dao_cli_color_ce, SL("BG_LIGHT_GRAY"),		DAO_CLI_COLOR_BG_LIGHT_GRAY);

	zend_declare_class_constant_long(dao_cli_color_ce, SL("AT_NORMAL"),			DAO_CLI_COLOR_AT_NORMAL);
	zend_declare_class_constant_long(dao_cli_color_ce, SL("AT_BOLD"),			DAO_CLI_COLOR_AT_BOLD);
	zend_declare_class_constant_long(dao_cli_color_ce, SL("AT_ITALIC"),			DAO_CLI_COLOR_AT_ITALIC);
	zend_declare_class_constant_long(dao_cli_color_ce, SL("AT_UNDERLINE"),		DAO_CLI_COLOR_AT_UNDERLINE);
	zend_declare_class_constant_long(dao_cli_color_ce, SL("AT_BLINK"),			DAO_CLI_COLOR_AT_BLINK);
	zend_declare_class_constant_long(dao_cli_color_ce, SL("AT_OUTLINE"),		DAO_CLI_COLOR_AT_OUTLINE);
	zend_declare_class_constant_long(dao_cli_color_ce, SL("AT_REVERSE"),		DAO_CLI_COLOR_AT_REVERSE);
	zend_declare_class_constant_long(dao_cli_color_ce, SL("AT_NONDISP"),		DAO_CLI_COLOR_AT_NONDISP);
	zend_declare_class_constant_long(dao_cli_color_ce, SL("AT_STRIKE"),			DAO_CLI_COLOR_AT_STRIKE);

	return SUCCESS;
}

/**
 * Identify if console supports colors
 *
 * @return boolean
 */
PHP_METHOD(Dao_Cli_Color, isSupportedShell){

	zval ret = {};
#ifdef PHP_WIN32
	zval arg = {};

	ZVAL_STRING(&arg, "ANSICON");

	DAO_CALL_FUNCTION(&ret, "getenv", &arg);
	zval_ptr_dtor(&arg);

	if (!DAO_IS_FALSE(&ret)) {
		RETURN_TRUE;
	}
	zval_ptr_dtor(&ret);

	ZVAL_STRING(&arg, "ConEmuANSI");

	DAO_CALL_FUNCTION(&ret, "getenv", &arg);
	zval_ptr_dtor(&arg);

	if (DAO_IS_STRING(&ret, "ON")) {
		zval_ptr_dtor(&ret);
		RETURN_TRUE;
	}
	zval_ptr_dtor(&ret);

	ZVAL_STRING(&arg, "TERM");

	DAO_CALL_FUNCTION(&ret, "getenv", &arg);
	zval_ptr_dtor(&arg);

	if (DAO_IS_STRING(&ret, "xterm")) {
		zval_ptr_dtor(&ret);
		RETURN_TRUE;
	}
	zval_ptr_dtor(&ret);
#endif

	if (dao_has_constant(SL("STDOUT"))) {
		if (dao_function_exists_ex(SL("posix_isatty")) == SUCCESS) {
			zval arg = {};
			ZVAL_STRING(&arg, "STDOUT");
			DAO_CALL_FUNCTION(&ret, "posix_isatty", &arg);
			if (zend_is_true(&ret)) {
				zval_ptr_dtor(&arg);
				RETURN_TRUE;
			}
			zval_ptr_dtor(&arg);
		}
	}

	RETURN_FALSE;
}

static char* fg_map[] = {"0;30","1;30","0;31","1;31","0;32","1;32","0;33","1;33","0;34","1;34","0;35","1;35","0;36","1;36","0;37","1;37"};
static char* at_map[] = {"0","1","3","4","5","6","7","8","9"};
static char* bg_map[] = {"40","41","42","43","44","45","46","47"};

/**
 * Colorizes the string using provided colors.
 *
 * @param string $str
 * @param null|integer $fg
 * @param null|integer $at
 * @param null|integer $bg
 * @return string
 */
PHP_METHOD(Dao_Cli_Color, colorize){

	zval *str, *fg = NULL, *at = NULL, *bg = NULL;
	zval ret = {}, colored = {};
	int i;

	dao_fetch_params(1, 1, 3, &str, &fg, &at, &bg);

	DAO_MM_CALL_CE_STATIC(&ret, dao_cli_color_ce, "issupportedshell");

	if (!zend_is_true(&ret)) {
		RETURN_MM_CTOR(str);
	}

	if (fg && Z_TYPE_P(fg) == IS_LONG) {
		i = Z_LVAL_P(fg);
		if (i >= DAO_CLI_COLOR_FG_BLACK && i <= DAO_CLI_COLOR_FG_WHITE) {
			dao_concat_sss(&colored, SL("\033["), fg_map[i], strlen(fg_map[i]), "m", 1, 0);
			DAO_MM_ADD_ENTRY(&colored);
		}
	}

	if (bg && Z_TYPE_P(bg) == IS_LONG) {
		i = Z_LVAL_P(bg);
		if (i >= DAO_CLI_COLOR_BG_BLACK && i <= DAO_CLI_COLOR_BG_LIGHT_GRAY) {
			dao_concat_sss(&colored, SL("\033["), bg_map[i], strlen(bg_map[i]), "m", 1, 1);
			DAO_MM_ADD_ENTRY(&colored);
		}
	}

	if (at && Z_TYPE_P(at) == IS_LONG) {
		i = Z_LVAL_P(at);
		if (i >= DAO_CLI_COLOR_AT_NORMAL && i <= DAO_CLI_COLOR_AT_STRIKE) {
			dao_concat_sss(&colored, SL("\033["), at_map[i], strlen(at_map[i]), "m", 1, 1);
			DAO_MM_ADD_ENTRY(&colored);
		}
	}

	DAO_CONCAT_VVS(return_value, &colored, str, "\033[0m");
	RETURN_MM();
}

/**
 * Color style for head messages.
 *
 * @param string $str
 * @return string
 */
PHP_METHOD(Dao_Cli_Color, head){

	zval *str, fg = {}, tmp = {};

	dao_fetch_params(0, 1, 0, &str);

	ZVAL_LONG(&fg, DAO_CLI_COLOR_FG_BROWN);

	DAO_CALL_CE_STATIC(&tmp, dao_cli_color_ce, "colorize", str, &fg);
	DAO_CONCAT_VS(return_value, &tmp, PHP_EOL);
	zval_ptr_dtor(&tmp);
}

/**
 * Color style for error messages.
 *
 * @param string $str
 * @return string
 */
PHP_METHOD(Dao_Cli_Color, error){

	zval *str, fg = {}, at= {}, bg= {}, msg = {}, tmp = {};
	zval input = {}, out = {}, line1 = {}, line2 = {};
	int space;

	dao_fetch_params(0, 1, 0, &str);

	ZVAL_LONG(&fg, DAO_CLI_COLOR_FG_WHITE);
	ZVAL_LONG(&at, DAO_CLI_COLOR_AT_BOLD);
	ZVAL_LONG(&bg, DAO_CLI_COLOR_BG_RED);

	DAO_CONCAT_SV(&tmp, "Error: ", str);

	space = Z_STRLEN(tmp) + 4;

	DAO_CONCAT_SVS(&msg, "  ", &tmp, "  ");
	zval_ptr_dtor(&tmp);

	ZVAL_STRING(&input, " ");
	dao_pad_str(&out, &input, space, " ", DAO_PDA_RIGHT);
	zval_ptr_dtor(&input);

	DAO_CALL_CE_STATIC(&line1, dao_cli_color_ce, "colorize", &out, &fg, &at, &bg);
	DAO_CALL_CE_STATIC(&line2, dao_cli_color_ce, "colorize", &msg, &fg, &at, &bg);
	zval_ptr_dtor(&out);
	zval_ptr_dtor(&msg);

	DAO_CONCAT_VSVSVS(return_value, &line1, PHP_EOL, &line2, PHP_EOL, &line1, PHP_EOL);
	zval_ptr_dtor(&line1);
	zval_ptr_dtor(&line2);
}

/**
 * Color style for success messages.
 *
 * @param string $str
 * @return string
 */
PHP_METHOD(Dao_Cli_Color, success){

	zval *str, fg = {}, at= {}, bg= {}, msg = {}, tmp = {};
	zval input = {}, out = {}, line1 = {}, line2 = {};
	int space;

	dao_fetch_params(0, 1, 0, &str);

	ZVAL_LONG(&fg, DAO_CLI_COLOR_FG_WHITE);
	ZVAL_LONG(&at, DAO_CLI_COLOR_AT_BOLD);
	ZVAL_LONG(&bg, DAO_CLI_COLOR_BG_GREEN);

	DAO_CONCAT_SV(&tmp, "Success: ", str);

	space = Z_STRLEN(tmp) + 4;

	DAO_CONCAT_SVS(&msg, "  ", &tmp, "  ");
	zval_ptr_dtor(&tmp);

	ZVAL_STRING(&input, " ");
	dao_pad_str(&out, &input, space, " ", DAO_PDA_RIGHT);
	zval_ptr_dtor(&input);

	DAO_CALL_CE_STATIC(&line1, dao_cli_color_ce, "colorize", &out, &fg, &at, &bg);
	DAO_CALL_CE_STATIC(&line2, dao_cli_color_ce, "colorize", &msg, &fg, &at, &bg);
	zval_ptr_dtor(&out);
	zval_ptr_dtor(&msg);

	DAO_CONCAT_VSVSVS(return_value, &line1, PHP_EOL, &line2, PHP_EOL, &line1, PHP_EOL);
	zval_ptr_dtor(&line1);
	zval_ptr_dtor(&line2);
}

/**
 * Color style for info messages.
 *
 * @param string $str
 * @return string
 */
PHP_METHOD(Dao_Cli_Color, info){

	zval *str, fg = {}, at= {}, bg= {}, msg = {}, tmp = {};
	zval input = {}, out = {}, line1 = {}, line2 = {};
	int space;

	dao_fetch_params(0, 1, 0, &str);

	ZVAL_LONG(&fg, DAO_CLI_COLOR_FG_WHITE);
	ZVAL_LONG(&at, DAO_CLI_COLOR_AT_BOLD);
	ZVAL_LONG(&bg, DAO_CLI_COLOR_BG_BLUE);

	DAO_CONCAT_SV(&tmp, "Info: ", str);

	space = Z_STRLEN(tmp) + 4;

	DAO_CONCAT_SVS(&msg, "  ", &tmp, "  ");
	zval_ptr_dtor(&tmp);

	ZVAL_STRING(&input, " ");
	dao_pad_str(&out, &input, space, " ", DAO_PDA_RIGHT);
	zval_ptr_dtor(&input);

	DAO_CALL_CE_STATIC(&line1, dao_cli_color_ce, "colorize", &out, &fg, &at, &bg);
	DAO_CALL_CE_STATIC(&line2, dao_cli_color_ce, "colorize", &msg, &fg, &at, &bg);
	zval_ptr_dtor(&out);
	zval_ptr_dtor(&msg);

	DAO_CONCAT_VSVSVS(return_value, &line1, PHP_EOL, &line2, PHP_EOL, &line1, PHP_EOL);
	zval_ptr_dtor(&line1);
	zval_ptr_dtor(&line2);
}
