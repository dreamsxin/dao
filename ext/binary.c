
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

#include "binary.h"
#include "di.h"
#include "filterinterface.h"

#include <ext/standard/php_array.h>
#include <ext/spl/spl_array.h>

#include <Zend/zend_closures.h>

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"
#include "kernel/string.h"
#include "kernel/array.h"
#include "kernel/operators.h"
#include "kernel/concat.h"
#include "kernel/object.h"
#include "kernel/exception.h"

#include "interned-strings.h"

/**
 * Dao\Binary
 *
 * Provides utilities to work with binary data
 */
zend_class_entry *dao_binary_ce;

PHP_METHOD(Dao_Binary, pack);
PHP_METHOD(Dao_Binary, unpack);
PHP_METHOD(Dao_Binary, setbit);
PHP_METHOD(Dao_Binary, getbit);
PHP_METHOD(Dao_Binary, toBitstring);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_binary_pack, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, type, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, endian, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_binary_unpack, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, type, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, endian, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_binary_setbit, 1, 0, 3)
	ZEND_ARG_TYPE_INFO(0, data, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, offset, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_binary_getbit, 1, 0, 2)
	ZEND_ARG_TYPE_INFO(0, data, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, offset, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_binary_tobitstring, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, data, IS_STRING, 0)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_binary_method_entry[] = {
	PHP_ME(Dao_Binary, pack, arginfo_dao_binary_pack, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Binary, unpack, arginfo_dao_binary_unpack, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Binary, setbit, arginfo_dao_binary_setbit, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Binary, getbit, arginfo_dao_binary_getbit, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Binary, toBitstring, arginfo_dao_binary_tobitstring, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};

/**
 * Dao\Binary initializer
 */
DAO_INIT_CLASS(Dao_Binary){

	DAO_REGISTER_CLASS(Dao, Binary, binary, dao_binary_method_entry, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	zend_declare_class_constant_long(dao_binary_ce, SL("TYPE_CHAR"), DAO_BINARY_TYPE_CHAR);
	zend_declare_class_constant_long(dao_binary_ce, SL("TYPE_UNSIGNEDCHAR"), DAO_BINARY_TYPE_UNSIGNEDCHAR);
	zend_declare_class_constant_long(dao_binary_ce, SL("TYPE_INT16"), DAO_BINARY_TYPE_INT16);
	zend_declare_class_constant_long(dao_binary_ce, SL("TYPE_UNSIGNEDINT16"), DAO_BINARY_TYPE_UNSIGNEDINT16);
	zend_declare_class_constant_long(dao_binary_ce, SL("TYPE_INT"), DAO_BINARY_TYPE_INT);
	zend_declare_class_constant_long(dao_binary_ce, SL("TYPE_UNSIGNEDINT"), DAO_BINARY_TYPE_UNSIGNEDINT);
	zend_declare_class_constant_long(dao_binary_ce, SL("TYPE_INT32"), DAO_BINARY_TYPE_INT32);
	zend_declare_class_constant_long(dao_binary_ce, SL("TYPE_UNSIGNEDINT32"), DAO_BINARY_TYPE_UNSIGNEDINT32);
	zend_declare_class_constant_long(dao_binary_ce, SL("TYPE_FLOAT"), DAO_BINARY_TYPE_FLOAT);
	zend_declare_class_constant_long(dao_binary_ce, SL("TYPE_DOUBLE"), DAO_BINARY_TYPE_DOUBLE);
	//zend_declare_class_constant_long(dao_binary_ce, SL("TYPE_STRING"), DAO_BINARY_TYPE_STRING);
	//zend_declare_class_constant_long(dao_binary_ce, SL("TYPE_HEXSTRING"), DAO_BINARY_TYPE_HEXSTRING);

	zend_declare_class_constant_long(dao_binary_ce, SL("ENDIAN_MACHINE"), DAO_BINARY_ENDIAN_MACHINE);
	zend_declare_class_constant_long(dao_binary_ce, SL("ENDIAN_BIG"), DAO_BINARY_ENDIAN_BIG);
	zend_declare_class_constant_long(dao_binary_ce, SL("ENDIAN_LITTLE"), DAO_BINARY_ENDIAN_LITTLE);
	return SUCCESS;
}

/**
 * Pack data into binary string
 *
 * @param int $type
 * @param string $data
 * @param int $endian
 * @return mixed
 **/
PHP_METHOD(Dao_Binary, pack)
{
	zval *type, *data, *endian = NULL, format = {};

	dao_fetch_params(1, 2, 1, &type, &data, &endian);

	if (!endian || Z_TYPE_P(endian) == IS_NULL) {
		endian = &DAO_GLOBAL(z_one);
	}

	switch (Z_LVAL_P(type)) {
		case DAO_BINARY_TYPE_CHAR:
			DAO_MM_ZVAL_STRING(&format, "c");
			break;
		case DAO_BINARY_TYPE_UNSIGNEDCHAR:
			DAO_MM_ZVAL_STRING(&format, "C");
			break;
		case DAO_BINARY_TYPE_INT16:
			DAO_MM_ZVAL_STRING(&format, "s");
			break;
		case DAO_BINARY_TYPE_UNSIGNEDINT16:
			if (Z_LVAL_P(endian) == DAO_BINARY_ENDIAN_LITTLE) {
				DAO_MM_ZVAL_STRING(&format, "v");
			} else if (Z_LVAL_P(endian) == DAO_BINARY_ENDIAN_MACHINE) {
				DAO_MM_ZVAL_STRING(&format, "S");
			} else { // DAO_BINARY_ENDIAN_BIG
				DAO_MM_ZVAL_STRING(&format, "n");
			}
			break;
		case DAO_BINARY_TYPE_INT:
			DAO_MM_ZVAL_STRING(&format, "i");
			break;
		case DAO_BINARY_TYPE_UNSIGNEDINT:
			DAO_MM_ZVAL_STRING(&format, "I");
			break;
		case DAO_BINARY_TYPE_INT32:
			DAO_MM_ZVAL_STRING(&format, "l");
			break;
		case DAO_BINARY_TYPE_UNSIGNEDINT32:
			if (Z_LVAL_P(endian) == DAO_BINARY_ENDIAN_LITTLE) {
				DAO_MM_ZVAL_STRING(&format, "V");
			} else if (Z_LVAL_P(endian) == DAO_BINARY_ENDIAN_MACHINE) {
				DAO_MM_ZVAL_STRING(&format, "L");
			} else { // DAO_BINARY_ENDIAN_BIG
				DAO_MM_ZVAL_STRING(&format, "N");
			}
			break;
		case DAO_BINARY_TYPE_FLOAT:
			DAO_MM_ZVAL_STRING(&format, "f");
			break;
		case DAO_BINARY_TYPE_DOUBLE:
			DAO_MM_ZVAL_STRING(&format, "d");
			break;
		default:
			RETURN_MM_FALSE;
	}

	DAO_MM_CALL_FUNCTION(return_value, "pack", &format, data);
	RETURN_MM();
}

/**
 * Unpack data from binary string
 *
 * @param int $type
 * @param string $data
 * @param int $endian
 * @return mixed
 **/
PHP_METHOD(Dao_Binary, unpack)
{
	zval *type, *data, *endian = NULL, format = {}, result = {};

	dao_fetch_params(1, 2, 1, &type, &data, &endian);

	if (!endian || Z_TYPE_P(endian) == IS_NULL) {
		endian = &DAO_GLOBAL(z_one);
	}

	switch (Z_LVAL_P(type)) {
		case DAO_BINARY_TYPE_CHAR:
			DAO_MM_ZVAL_STRING(&format, "c");
			break;
		case DAO_BINARY_TYPE_UNSIGNEDCHAR:
			DAO_MM_ZVAL_STRING(&format, "C");
			break;
		case DAO_BINARY_TYPE_INT16:
			DAO_MM_ZVAL_STRING(&format, "s");
			break;
		case DAO_BINARY_TYPE_UNSIGNEDINT16:
			if (Z_LVAL_P(endian) == DAO_BINARY_ENDIAN_LITTLE) {
				DAO_MM_ZVAL_STRING(&format, "v");
			} else if (Z_LVAL_P(endian) == DAO_BINARY_ENDIAN_MACHINE) {
				DAO_MM_ZVAL_STRING(&format, "S");
			} else { // DAO_BINARY_ENDIAN_BIG
				DAO_MM_ZVAL_STRING(&format, "n");
			}
			break;
		case DAO_BINARY_TYPE_INT:
			DAO_MM_ZVAL_STRING(&format, "i");
			break;
		case DAO_BINARY_TYPE_UNSIGNEDINT:
			DAO_MM_ZVAL_STRING(&format, "I");
			break;
		case DAO_BINARY_TYPE_INT32:
			DAO_MM_ZVAL_STRING(&format, "l");
			break;
		case DAO_BINARY_TYPE_UNSIGNEDINT32:
			if (Z_LVAL_P(endian) == DAO_BINARY_ENDIAN_LITTLE) {
				DAO_MM_ZVAL_STRING(&format, "V");
			} else if (Z_LVAL_P(endian) == DAO_BINARY_ENDIAN_MACHINE) {
				DAO_MM_ZVAL_STRING(&format, "L");
			} else { // DAO_BINARY_ENDIAN_BIG
				DAO_MM_ZVAL_STRING(&format, "N");
			}
			break;
		case DAO_BINARY_TYPE_FLOAT:
			DAO_MM_ZVAL_STRING(&format, "f");
			break;
		case DAO_BINARY_TYPE_DOUBLE:
			DAO_MM_ZVAL_STRING(&format, "d");
			break;
		default:
			RETURN_MM_FALSE;
	}

	DAO_MM_CALL_FUNCTION(&result, "unpack", &format, data);
	DAO_MM_ADD_ENTRY(&result);

	if (!dao_array_isset_fetch_long(return_value, &result, 1, PH_COPY)) {
		RETURN_MM_NULL();
	}
	RETURN_MM();
}

/**
 * Changes a single bit of a string.
 *
 * @param string $data
 * @param int $offset
 * @param int $value
 * @return bool
 **/
PHP_METHOD(Dao_Binary, setbit)
{
	zval *data, *offset, *value;
	zend_long byte_offset;

	dao_fetch_params(0, 3, 0, &data, &offset, &value);

	if (offset < 0) {
		php_error_docref(NULL, E_NOTICE, "Offset must be greater than or equal to 0");
		RETURN_FALSE;
	}
    byte_offset = Z_LVAL_P(offset) / 8;
	if (byte_offset >= Z_STRLEN_P(data)) {
		zend_string *c;
		c = Z_STR_P(data);
		if (IS_INTERNED(c)) {
			php_error_docref(NULL, E_NOTICE, "Cannot Change interned string");
			RETURN_FALSE;
		}
		zend_string_realloc(c, byte_offset+1, 0);
		ZVAL_STR(data, c);
	}
	if (zend_is_true(value)) {
		DAO_BINARY_SETBIT(Z_STRVAL_P(data), Z_LVAL_P(offset));
	} else {
		DAO_BINARY_UNSETBIT(Z_STRVAL_P(data), Z_LVAL_P(offset));
	}
	RETURN_TRUE;
}

/**
 * Gets a single bit of a string.
 *
 * @param string $data
 * @param int $offset
 * @return int
 **/
PHP_METHOD(Dao_Binary, getbit)
{
	zval *data, *offset;
	zend_long byte_offset;

	dao_fetch_params(0, 2, 0, &data, &offset);

	if (offset < 0) {
		php_error_docref(NULL, E_NOTICE, "Offset must be greater than or equal to 0");
		RETURN_FALSE;
	}
    byte_offset = Z_LVAL_P(offset) / 8;
	if (byte_offset >= Z_STRLEN_P(data)) {
		RETURN_FALSE;
	}
	RETURN_LONG(DAO_BINARY_CHECKBIT(Z_STRVAL_P(data), Z_LVAL_P(offset)) ? 1 : 0);
}

/**
 * Print binary string.
 *
 * @param string $data
 * @return string
 **/
PHP_METHOD(Dao_Binary, toBitstring)
{
	zval *data;
	zend_long byte_length;
	smart_str str = {0};
	int i;

	dao_fetch_params(0, 1, 0, &data);

    byte_length = Z_STRLEN_P(data);
	for (i=0; i<byte_length; i++) {
		int j;
		for (j=0; j<8; j++) {
			if (DAO_BINARY_CHECKBIT(Z_STRVAL_P(data), (i*8)+j)) {
				smart_str_appendc(&str, '1');
			} else {
				smart_str_appendc(&str, '0');
			}
		}
	}

	smart_str_0(&str);

	if (str.s) {
		RETURN_STR(str.s);
	} else {
		smart_str_free(&str);
		RETURN_FALSE;
	}
}
