
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

#ifndef DAO_CLI_COLOR_H
#define DAO_CLI_COLOR_H

#include "php_dao.h"

enum dao_cli_color_fgs {
	DAO_CLI_COLOR_FG_BLACK,
	DAO_CLI_COLOR_FG_DARK_GRAY,
	DAO_CLI_COLOR_FG_RED,
	DAO_CLI_COLOR_FG_LIGHT_RED,
	DAO_CLI_COLOR_FG_GREEN,
	DAO_CLI_COLOR_FG_LIGHT_GREEN,
	DAO_CLI_COLOR_FG_BROWN,
	DAO_CLI_COLOR_FG_YELLOW,
	DAO_CLI_COLOR_FG_BLUE,
	DAO_CLI_COLOR_FG_LIGHT_BLUE,
	DAO_CLI_COLOR_FG_PURPLE,
	DAO_CLI_COLOR_FG_LIGHT_PURPLE,
	DAO_CLI_COLOR_FG_CYAN,
	DAO_CLI_COLOR_FG_LIGHT_CYAN,
	DAO_CLI_COLOR_FG_LIGHT_GRAY,
	DAO_CLI_COLOR_FG_WHITE
};

enum dao_cli_color_bgs {
	DAO_CLI_COLOR_BG_BLACK,
	DAO_CLI_COLOR_BG_RED,
	DAO_CLI_COLOR_BG_GREEN,
	DAO_CLI_COLOR_BG_YELLOW,
	DAO_CLI_COLOR_BG_BLUE,
	DAO_CLI_COLOR_BG_MAGENTA,
	DAO_CLI_COLOR_BG_CYAN,
	DAO_CLI_COLOR_BG_LIGHT_GRAY
};

enum dao_cli_color_ats {
	DAO_CLI_COLOR_AT_NORMAL,
	DAO_CLI_COLOR_AT_BOLD,
	DAO_CLI_COLOR_AT_ITALIC,
	DAO_CLI_COLOR_AT_UNDERLINE,
	DAO_CLI_COLOR_AT_BLINK,
	DAO_CLI_COLOR_AT_OUTLINE,
	DAO_CLI_COLOR_AT_REVERSE,
	DAO_CLI_COLOR_AT_NONDISP,
	DAO_CLI_COLOR_AT_STRIKE
};


extern zend_class_entry *dao_cli_color_ce;

DAO_INIT_CLASS(Dao_Cli_Color);

#endif /* DAO_CLI_COLOR_H */
