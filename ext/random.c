
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

#include "random.h"
#include "exception.h"

#include <ext/standard/php_rand.h>

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/exception.h"
#include "kernel/operators.h"
#include "kernel/fcall.h"
#include "kernel/string.h"
#include "kernel/concat.h"
#include "kernel/array.h"

/**
 * Dao\Random
 *
 *<code>
 *  $random = new \Dao\Random();
 *
 *  // Random alnum string
 *  $alnum = $random->alnum();
 */
zend_class_entry *dao_random_ce;

PHP_METHOD(Dao_Random, alnum);
PHP_METHOD(Dao_Random, alpha);
PHP_METHOD(Dao_Random, hexdec);
PHP_METHOD(Dao_Random, numeric);
PHP_METHOD(Dao_Random, nozero);
PHP_METHOD(Dao_Random, color);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_random_alnum, 0, 0, 0)
	ZEND_ARG_INFO(0, len)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_random_alpha, 0, 0, 0)
	ZEND_ARG_INFO(0, len)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_random_hexdec, 0, 0, 0)
	ZEND_ARG_INFO(0, len)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_random_numeric, 0, 0, 0)
	ZEND_ARG_INFO(0, len)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_random_nozero, 0, 0, 0)
	ZEND_ARG_INFO(0, len)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_random_color, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, type, IS_LONG, 1)
ZEND_END_ARG_INFO()


static const zend_function_entry dao_random_method_entry[] = {
	PHP_ME(Dao_Random, alnum, arginfo_dao_random_alnum, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Random, alpha, arginfo_dao_random_alpha, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Random, hexdec, arginfo_dao_random_hexdec, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Random, numeric, arginfo_dao_random_numeric, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Random, nozero, arginfo_dao_random_nozero, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Random, color, arginfo_dao_random_color, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};

/**
 * Dao\Random initializer
 */
DAO_INIT_CLASS(Dao_Random){

	DAO_REGISTER_CLASS(Dao, Random, random, dao_random_method_entry, 0);

	zend_declare_class_constant_long(dao_random_ce, SL("COLOR_RGB"), DAO_RANDOM_COLOR_RGB);
	zend_declare_class_constant_long(dao_random_ce, SL("COLOR_RGBA"), DAO_RANDOM_COLOR_RGBA);
	return SUCCESS;
}

/**
 * Generates a random alnum string
 *
 *<code>
 *  $random = new \Dao\Random();
 *  $alnum = $random->alnum();
 *</code>
 */
PHP_METHOD(Dao_Random, alnum){

	zval *len_param = NULL, len = {}, type = {};
	int l;

	dao_fetch_params(0, 0, 1, &len_param);

	if (!len_param) {
		l = 8;
	} else {
		l = dao_get_intval(len_param);
		if (l <= 0) {
			l = 8;
		}
	}

	ZVAL_LONG(&len, l);
	ZVAL_LONG(&type, DAO_RANDOM_ALNUM);

	dao_random_string(return_value, &type, &len);
}

/**
 * Generates a random alpha string
 *
 *<code>
 *  $random = new \Dao\Random();
 *  $alpha = $random->alpha();
 *</code>
 */
PHP_METHOD(Dao_Random, alpha){

	zval *len_param = NULL, len = {}, type = {};
	int l;

	dao_fetch_params(0, 0, 1, &len_param);

	if (!len_param) {
		l = 8;
	} else {
		l = dao_get_intval(len_param);
		if (l <= 0) {
			l = 8;
		}
	}

	ZVAL_LONG(&len, l);
	ZVAL_LONG(&type, DAO_RANDOM_ALPHA);

	dao_random_string(return_value, &type, &len);
}

/**
 * Generates a random hexdec string
 *
 *<code>
 *  $random = new \Dao\Random();
 *  $hexdec = $random->hexdec();
 *</code>
 */
PHP_METHOD(Dao_Random, hexdec){

	zval *len_param = NULL, len = {}, type = {};
	int l;

	dao_fetch_params(0, 0, 1, &len_param);

	if (!len_param) {
		l = 8;
	} else {
		l = dao_get_intval(len_param);
		if (l <= 0) {
			l = 8;
		}
	}

	ZVAL_LONG(&len, l);
	ZVAL_LONG(&type, DAO_RANDOM_HEXDEC);

	dao_random_string(return_value, &type, &len);
}

/**
 * Generates a random numeric string
 *
 *<code>
 *  $random = new \Dao\Random();
 *  $numeric = $random->numeric();
 *</code>
 */
PHP_METHOD(Dao_Random, numeric){

	zval *len_param = NULL, len = {}, type = {};
	int l;

	dao_fetch_params(0, 0, 1, &len_param);

	if (!len_param) {
		l = 8;
	} else {
		l = dao_get_intval(len_param);
		if (l <= 0) {
			l = 8;
		}
	}

	ZVAL_LONG(&len, l);
	ZVAL_LONG(&type, DAO_RANDOM_NUMERIC);

	dao_random_string(return_value, &type, &len);
}

/**
 * Generates a random nozero numeric string
 *
 *<code>
 *  $random = new \Dao\Random();
 *  $bytes = $random->nozero();
 *</code>
 */
PHP_METHOD(Dao_Random, nozero){

	zval *len_param = NULL, len = {}, type = {};
	int l;

	dao_fetch_params(0, 0, 1, &len_param);

	if (!len_param) {
		l = 8;
	} else {
		l = dao_get_intval(len_param);
		if (l <= 0) {
			l = 8;
		}
	}

	ZVAL_LONG(&len, l);
	ZVAL_LONG(&type, DAO_RANDOM_NOZERO);

	dao_random_string(return_value, &type, &len);
}

/**
 * Generates a random color
 */
PHP_METHOD(Dao_Random, color) {

	zval *type = NULL, r = {}, g = {}, b = {}, a = {}, r_hex = {}, g_hex = {}, b_hex = {}, pad_type = {}, r_str = {}, g_str = {}, b_str = {};
	zend_long color_type = DAO_RANDOM_COLOR_RGBA, rnd_idx = 0;

	dao_fetch_params(0, 0, 1, &type);

	if (type && Z_TYPE_P(type) == IS_LONG) {
		color_type = Z_LVAL_P(type);
	}

#if PHP_VERSION_ID >= 70100
		rnd_idx = php_mt_rand_common(0, 255);
#else
		rnd_idx = php_rand();
		RAND_RANGE(rnd_idx, 0, 255, PHP_RAND_MAX);
#endif
	ZVAL_LONG(&r, rnd_idx);

#if PHP_VERSION_ID >= 70100
		rnd_idx = php_mt_rand_common(0, 255);
#else
		rnd_idx = php_rand();
		RAND_RANGE(rnd_idx, 0, 255, PHP_RAND_MAX);
#endif
	ZVAL_LONG(&g, rnd_idx);

#if PHP_VERSION_ID >= 70100
		rnd_idx = php_mt_rand_common(0, 255);
#else
		rnd_idx = php_rand();
		RAND_RANGE(rnd_idx, 0, 255, PHP_RAND_MAX);
#endif
	ZVAL_LONG(&b, rnd_idx);

	if (color_type == DAO_RANDOM_COLOR_RGBA) {
#if PHP_VERSION_ID >= 70100
		rnd_idx = php_mt_rand_common(0, 127);
#else
		rnd_idx = php_rand();
		RAND_RANGE(rnd_idx, 0, 127, PHP_RAND_MAX);
#endif
		ZVAL_LONG(&a, rnd_idx);
		DAO_CONCAT_SVSVSVSVS(return_value, "rgba(", &r, ",", &g, ",", &b, ",", &a, ")");
	} else {
		DAO_CALL_FUNCTION(&r_hex, "dechex", &r);
		DAO_CALL_FUNCTION(&g_hex, "dechex", &g);
		DAO_CALL_FUNCTION(&b_hex, "dechex", &b);

		if (!dao_get_constant(&pad_type, SL("STR_PAD_LEFT"))) {
			ZVAL_LONG(&pad_type, 0);
		}
		DAO_CALL_FUNCTION(&r_str, "str_pad", &r_hex, &DAO_GLOBAL(z_two), &pad_type);
		DAO_CALL_FUNCTION(&g_str, "str_pad", &g_hex, &DAO_GLOBAL(z_two), &pad_type);
		DAO_CALL_FUNCTION(&b_str, "str_pad", &b_hex, &DAO_GLOBAL(z_two), &pad_type);

		DAO_CONCAT_SVVV(return_value, "#", &r_str, &g_str, &b_str);
	}
}
