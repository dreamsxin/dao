
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

#include "binary/reader.h"
#include "binary.h"
#include "binary/exception.h"

#include <main/php_streams.h>
#include <ext/standard/php_array.h>
#include <ext/spl/spl_array.h>

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
 * Dao\Binary\Reader
 *
 * Provides utilities to work with binary data
 *
 *<code>
 *	$fp = fopen('unit-tests/assets/data.bin', 'rb');
 *	$bin = new Dao\Binary\Reader($fp);
 *	$v = $bin->readUnsignedChar();
 *</code>
 */
zend_class_entry *dao_binary_reader_ce;

PHP_METHOD(Dao_Binary_Reader, __construct);
PHP_METHOD(Dao_Binary_Reader, getEndian);
PHP_METHOD(Dao_Binary_Reader, getInput);
PHP_METHOD(Dao_Binary_Reader, getContent);
PHP_METHOD(Dao_Binary_Reader, setPosition);
PHP_METHOD(Dao_Binary_Reader, getPosition);
PHP_METHOD(Dao_Binary_Reader, getEofPosition);
PHP_METHOD(Dao_Binary_Reader, isEof);
PHP_METHOD(Dao_Binary_Reader, read);
PHP_METHOD(Dao_Binary_Reader, readChar);
PHP_METHOD(Dao_Binary_Reader, readUnsignedChar);
PHP_METHOD(Dao_Binary_Reader, readInt16);
PHP_METHOD(Dao_Binary_Reader, readUnsignedInt16);
PHP_METHOD(Dao_Binary_Reader, readInt);
PHP_METHOD(Dao_Binary_Reader, readUnsignedInt);
PHP_METHOD(Dao_Binary_Reader, readInt32);
PHP_METHOD(Dao_Binary_Reader, readUnsignedInt32);
PHP_METHOD(Dao_Binary_Reader, readFloat);
PHP_METHOD(Dao_Binary_Reader, readDouble);
PHP_METHOD(Dao_Binary_Reader, readString);
PHP_METHOD(Dao_Binary_Reader, readHexString);
PHP_METHOD(Dao_Binary_Reader, append);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_binary_reader___construct, 0, 0, 0)
	ZEND_ARG_INFO(0, data)
	ZEND_ARG_INFO(0, endian)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_binary_reader_setposition, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, position, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, whence, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_binary_reader_read, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, length, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_binary_reader_readunsignedint16, 0, 0, 0)
	ZEND_ARG_INFO(0, endian)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_binary_reader_readunsignedint32, 0, 0, 0)
	ZEND_ARG_INFO(0, endian)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_binary_reader_readstring, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, length, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_binary_reader_readhexstring, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, length, IS_LONG, 1)
	ZEND_ARG_INFO(0, lowNibble)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_binary_reader_append, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, data, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, length, IS_LONG, 1)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_binary_reader_method_entry[] = {
	PHP_ME(Dao_Binary_Reader, __construct, arginfo_dao_binary_reader___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Binary_Reader, getEndian, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Binary_Reader, getInput, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Binary_Reader, getContent, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Binary_Reader, setPosition, arginfo_dao_binary_reader_setposition, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Binary_Reader, getPosition, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Binary_Reader, getEofPosition, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Binary_Reader, isEof, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Binary_Reader, read, arginfo_dao_binary_reader_read, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Binary_Reader, readChar, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Binary_Reader, readUnsignedChar, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Binary_Reader, readInt16, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Binary_Reader, readUnsignedInt16, arginfo_dao_binary_reader_readunsignedint16, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Binary_Reader, readInt, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Binary_Reader, readUnsignedInt, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Binary_Reader, readInt32, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Binary_Reader, readUnsignedInt32, arginfo_dao_binary_reader_readunsignedint32, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Binary_Reader, readFloat, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Binary_Reader, readDouble, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Binary_Reader, readString, arginfo_dao_binary_reader_readstring, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Binary_Reader, readHexString, arginfo_dao_binary_reader_readhexstring, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Binary_Reader, append, arginfo_dao_binary_reader_append, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Binary initializer
 */
DAO_INIT_CLASS(Dao_Binary_Reader){

	DAO_REGISTER_CLASS(Dao\\Binary, Reader, binary_reader, dao_binary_reader_method_entry, 0);

	zend_declare_property_long(dao_binary_reader_ce, SL("_endian"), DAO_BINARY_ENDIAN_BIG, ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_binary_reader_ce, SL("_input"), ZEND_ACC_PROTECTED);
	zend_declare_property_long(dao_binary_reader_ce, SL("_position"), 0, ZEND_ACC_PROTECTED);
	zend_declare_property_long(dao_binary_reader_ce, SL("_eofPosition"), 0, ZEND_ACC_PROTECTED);
	return SUCCESS;
}

/**
 * Dao\Binary\Reader constructor
 *
 * @param  string|resource $data
 * @param  int $endian
 * @throws \InvalidArgumentException
 */
PHP_METHOD(Dao_Binary_Reader, __construct){

	zval *data = NULL, *endian = NULL;

	dao_fetch_params(1, 0, 2, &data, &endian);

	if (!data) {
		data = &DAO_GLOBAL(z_null);
	}

	if (Z_TYPE_P(data) == IS_STRING || Z_TYPE_P(data) == IS_NULL) {
		zval filename = {}, mode = {}, handler = {};

		DAO_MM_ZVAL_STRING(&filename, "php://memory");
		DAO_MM_ZVAL_STRING(&mode, "br+");
		DAO_MM_CALL_FUNCTION(&handler, "fopen", &filename, &mode);
		DAO_MM_ADD_ENTRY(&handler);
		if (Z_TYPE_P(data) == IS_STRING) {
			php_stream *stream;
			zend_long size;
			php_stream_from_res(stream, Z_RES(handler));
			
			size = php_stream_write(stream, Z_STRVAL_P(data), Z_STRLEN_P(data));
			if (size > 0) {
				dao_update_property_long(getThis(), SL("_eofPosition"), size);
				php_stream_rewind(stream);
			}
		}

		dao_update_property(getThis(), SL("_input"), &handler);
	} else if (Z_TYPE_P(data) == IS_RESOURCE) {
		zval fstat = {}, size = {}, content = {};
		dao_update_property(getThis(), SL("_input"), data);

		DAO_MM_CALL_FUNCTION(&fstat, "fstat", data);
		DAO_MM_ADD_ENTRY(&fstat);
		if (dao_array_isset_fetch_str(&size, &fstat, SL("size"), PH_READONLY)) {
			dao_update_property(getThis(), SL("_eofPosition"), &size);
		} else {
			ZVAL_LONG(&size, 0);
		}

		DAO_MM_CALL_FUNCTION(NULL, "rewind", data);
		DAO_MM_CALL_FUNCTION(&content, "fread", data, &size);
		DAO_MM_ADD_ENTRY(&content);
		DAO_MM_CALL_FUNCTION(NULL, "rewind", data);
	} else {
		DAO_MM_THROW_EXCEPTION_STR(dao_binary_exception_ce, "Data must be set as string or resource");
		return;
	}

	if (endian && Z_TYPE_P(endian) != IS_NULL) {
		if (Z_TYPE_P(endian) != IS_LONG || Z_LVAL_P(endian) < 0 || Z_LVAL_P(endian) > 2) {
			DAO_MM_THROW_EXCEPTION_STR(dao_binary_exception_ce, "Endian must be set as big or little");
			return;
		}
		dao_update_property(getThis(), SL("_endian"), endian);
	}
	RETURN_MM();
}

/**
 * Gets the endian
 *
 * @return int
 */
PHP_METHOD(Dao_Binary_Reader, getEndian){


	RETURN_MEMBER(getThis(), "_endian");
}

/**
 * Gets the input
 *
 * @return int
 */
PHP_METHOD(Dao_Binary_Reader, getInput){


	RETURN_MEMBER(getThis(), "_input");
}

/**
 * Gets the binary data
 *
 * @return int
 */
PHP_METHOD(Dao_Binary_Reader, getContent){


	zval input = {}, position = {}, eof_position = {};
	php_stream *stream;

	dao_read_property(&input, getThis(), SL("_input"), PH_NOISY|PH_READONLY);
	dao_read_property(&position, getThis(), SL("_position"), PH_NOISY|PH_READONLY);
	dao_read_property(&eof_position, getThis(), SL("_eofPosition"), PH_NOISY|PH_READONLY);

	php_stream_from_res(stream, Z_RES(input));
	php_stream_rewind(stream);

	ZVAL_NEW_STR(return_value, zend_string_alloc(Z_LVAL(eof_position), 0));
	Z_STRLEN_P(return_value) = php_stream_read(stream, Z_STRVAL_P(return_value), Z_LVAL(eof_position));
	Z_STRVAL_P(return_value)[Z_STRLEN_P(return_value)] = 0;

	DAO_CALL_METHOD(NULL, getThis(), "setposition", &position);
}

/**
 * Sets the position in the the file pointer
 *
 * @param integer $position
 * @param integer $whence
 * @return boolean
 */
PHP_METHOD(Dao_Binary_Reader, setPosition){

	zval *position, *whence = NULL, input = {}, current_position = {}, eof_position = {}, pos = {};
	php_stream *stream;
	int seek_type = SEEK_SET, ret;

	dao_fetch_params(0, 1, 1, &position, &whence);

	if (whence && Z_TYPE_P(whence) == IS_LONG) {
		seek_type = Z_LVAL_P(whence);
	}

	dao_read_property(&input, getThis(), SL("_input"), PH_NOISY|PH_READONLY);
	php_stream_from_res(stream, Z_RES(input));
	if (seek_type == SEEK_CUR) {
		ret = php_stream_seek(stream, Z_LVAL_P(position), SEEK_CUR);
		if (ret >= 0) {
			dao_read_property(&current_position, getThis(), SL("_position"), PH_NOISY|PH_READONLY);
			ZVAL_LONG(&pos, Z_LVAL(current_position) + Z_LVAL_P(position));
			dao_update_property(getThis(), SL("_position"), &pos);
			RETURN_TRUE;
		}
	} else if (seek_type == SEEK_END) {
		ret = php_stream_seek(stream, Z_LVAL_P(position), SEEK_END);
		if (ret >= 0) {
			dao_read_property(&eof_position, getThis(), SL("_eofPosition"), PH_NOISY|PH_READONLY);
			ZVAL_LONG(&pos, Z_LVAL(eof_position) - Z_LVAL_P(position));
			dao_update_property(getThis(), SL("_position"), &pos);
			RETURN_TRUE;
		}
	} else {
		ret = php_stream_seek(stream, Z_LVAL_P(position), SEEK_SET);
		if (ret >= 0) {
			dao_update_property(getThis(), SL("_position"), position);
			RETURN_TRUE;
		}
	}


	RETURN_FALSE;
}

/**
 * Gets the current postion in the the file pointer
 *
 * @return int
 */
PHP_METHOD(Dao_Binary_Reader, getPosition){


	RETURN_MEMBER(getThis(), "_position");
}

/**
 * Gets the eof postion the file pointer
 *
 * @return int
 */
PHP_METHOD(Dao_Binary_Reader, getEofPosition){


	RETURN_MEMBER(getThis(), "_eofPosition");
}

/**
 * Checks if end of the file pointer was reached
 *
 * @return boolean
 */
PHP_METHOD(Dao_Binary_Reader, isEof){

	zval position = {}, eof_position = {};

	dao_read_property(&position, getThis(), SL("_position"), PH_NOISY|PH_READONLY);
	dao_read_property(&eof_position, getThis(), SL("_eofPosition"), PH_NOISY|PH_READONLY);

	if (DAO_GE(&position, &eof_position)) {
		RETURN_TRUE;
	}
	RETURN_FALSE;
}

/**
 * Read num bytes from the current position in the file pointer
 *
 * @param integer $length
 * @return string
 */
PHP_METHOD(Dao_Binary_Reader, read){

	zval *length, position = {}, eof_position = {}, result = {}, input = {};
	php_stream *stream;
	zend_long len;

	dao_fetch_params(0, 1, 0, &length);

	dao_read_property(&position, getThis(), SL("_position"), PH_NOISY|PH_READONLY);
	dao_read_property(&eof_position, getThis(), SL("_eofPosition"), PH_NOISY|PH_READONLY);
	dao_add_function(&result, &position, length);
	if (DAO_GT(&result, &eof_position)) {
		DAO_THROW_EXCEPTION_FORMAT(dao_binary_exception_ce, "Outside of input, postion: %d, total length: %d", Z_LVAL(result), Z_LVAL(eof_position));
		return;
	}

	dao_read_property(&input, getThis(), SL("_input"), PH_NOISY|PH_READONLY);
	php_stream_from_res(stream, Z_RES(input));

	len = Z_LVAL_P(length);
	ZVAL_NEW_STR(return_value, zend_string_alloc(len, 0));
	Z_STRLEN_P(return_value) = php_stream_read(stream, Z_STRVAL_P(return_value), len);
	Z_STRVAL_P(return_value)[Z_STRLEN_P(return_value)] = 0;

	dao_update_property(getThis(), SL("_position"), &result);
}

/**
 * Read a signed char from the current position in the file pointer
 *
 * @return string
 */
PHP_METHOD(Dao_Binary_Reader, readChar){

	zval length = {}, bytes = {}, format = {}, result = {};

	DAO_MM_INIT();

	ZVAL_LONG(&length, 1);
	DAO_MM_CALL_METHOD(&bytes, getThis(), "read", &length);
	DAO_MM_ADD_ENTRY(&bytes);

	DAO_MM_ZVAL_STRING(&format, "c");
	DAO_MM_CALL_FUNCTION(&result, "unpack", &format, &bytes);
	DAO_MM_ADD_ENTRY(&result);

	if (!dao_array_isset_fetch_long(return_value, &result, 1, PH_COPY)) {
		RETURN_MM_NULL();
	}
	RETURN_MM();
}

/**
 * Read a unsigned char from the current position in the file pointer
 *
 * @return string
 */
PHP_METHOD(Dao_Binary_Reader, readUnsignedChar){

	zval length = {}, bytes = {}, format = {}, result = {};

	DAO_MM_INIT();

	ZVAL_LONG(&length, 1);
	DAO_MM_CALL_METHOD(&bytes, getThis(), "read", &length);
	DAO_MM_ADD_ENTRY(&bytes);

	DAO_MM_ZVAL_STRING(&format, "C");
	DAO_MM_CALL_FUNCTION(&result, "unpack", &format, &bytes);
	DAO_MM_ADD_ENTRY(&result);

	if (!dao_array_isset_fetch_long(return_value, &result, 1, PH_COPY)) {
		RETURN_MM_NULL();
	}
	RETURN_MM();
}

/**
 * Read a signed short int from the current position in the file pointer
 *
 * @return int
 */
PHP_METHOD(Dao_Binary_Reader, readInt16){

	zval length = {}, bytes = {}, format = {}, result = {};

	DAO_MM_INIT();

	ZVAL_LONG(&length, 2);
	DAO_MM_CALL_METHOD(&bytes, getThis(), "read", &length);
	DAO_MM_ADD_ENTRY(&bytes);

	DAO_MM_ZVAL_STRING(&format, "s");
	DAO_MM_CALL_FUNCTION(&result, "unpack", &format, &bytes);
	DAO_MM_ADD_ENTRY(&result);

	if (!dao_array_isset_fetch_long(return_value, &result, 1, PH_COPY)) {
		RETURN_MM_NULL();
	}
	RETURN_MM();
}

/**
 * Read a unsigned short int from the current position in the file pointer
 *
 * @return int
 */
PHP_METHOD(Dao_Binary_Reader, readUnsignedInt16){

	zval *_endian = NULL, length = {}, bytes = {}, endian = {}, format = {}, result = {};

	dao_fetch_params(1, 0, 1, &_endian);

	if (!_endian || Z_TYPE_P(_endian) == IS_NULL) {
		dao_read_property(&endian, getThis(), SL("_endian"), PH_NOISY|PH_READONLY);
	} else {
		ZVAL_COPY_VALUE(&endian, _endian);
	}

	ZVAL_LONG(&length, 2);
	DAO_MM_CALL_METHOD(&bytes, getThis(), "read", &length);
	DAO_MM_ADD_ENTRY(&bytes);

	if (Z_LVAL(endian) == DAO_BINARY_ENDIAN_BIG) {
		DAO_MM_ZVAL_STRING(&format, "n");
	} else if (Z_LVAL(endian) == DAO_BINARY_ENDIAN_LITTLE) {
		DAO_MM_ZVAL_STRING(&format, "v");
	} else if (Z_LVAL(endian) == DAO_BINARY_ENDIAN_MACHINE) {
		DAO_MM_ZVAL_STRING(&format, "S");
	}

	DAO_MM_CALL_FUNCTION(&result, "unpack", &format, &bytes);
	DAO_MM_ADD_ENTRY(&result);

	if (!dao_array_isset_fetch_long(return_value, &result, 1, PH_COPY)) {
		RETURN_MM_NULL();
	}
	RETURN_MM();
}

/**
 * Read a signed int from the current position in the file pointer
 *
 * @return int
 */
PHP_METHOD(Dao_Binary_Reader, readInt){

	zval length = {}, bytes = {}, format = {}, result = {};

	DAO_MM_INIT();

	ZVAL_LONG(&length, sizeof(int));
	DAO_MM_CALL_METHOD(&bytes, getThis(), "read", &length);
	DAO_MM_ADD_ENTRY(&bytes);

	DAO_MM_ZVAL_STRING(&format, "i");

	DAO_CALL_FUNCTION(&result, "unpack", &format, &bytes);
	DAO_MM_ADD_ENTRY(&result);

	if (!dao_array_isset_fetch_long(return_value, &result, 1, PH_COPY)) {
		RETURN_MM_NULL();
	}
	RETURN_MM();
}

/**
 * Read a unsigned int from the current position in the file pointer
 *
 * @return int
 */
PHP_METHOD(Dao_Binary_Reader, readUnsignedInt){

	zval length = {}, bytes = {}, format = {}, result = {};

	DAO_MM_INIT();

	ZVAL_LONG(&length, sizeof(int));
	DAO_MM_CALL_METHOD(&bytes, getThis(), "read", &length);
	DAO_MM_ADD_ENTRY(&bytes);

	DAO_MM_ZVAL_STRING(&format, "I");

	DAO_MM_CALL_FUNCTION(&result, "unpack", &format, &bytes);
	DAO_MM_ADD_ENTRY(&result);

	if (!dao_array_isset_fetch_long(return_value, &result, 1, PH_COPY)) {
		RETURN_MM_NULL();
	}
	RETURN_MM();
}

/**
 * Read a signed long int from the current position in the file pointer
 *
 * @return int
 */
PHP_METHOD(Dao_Binary_Reader, readInt32){

	zval length = {}, bytes = {}, format = {}, result = {};

	DAO_MM_INIT();

	ZVAL_LONG(&length, 4);
	DAO_MM_CALL_METHOD(&bytes, getThis(), "read", &length);
	DAO_MM_ADD_ENTRY(&bytes);

	DAO_MM_ZVAL_STRING(&format, "l");

	DAO_MM_CALL_FUNCTION(&result, "unpack", &format, &bytes);
	DAO_MM_ADD_ENTRY(&result);

	if (!dao_array_isset_fetch_long(return_value, &result, 1, PH_COPY)) {
		RETURN_MM_NULL();
	}
	RETURN_MM();
}

/**
 * Read a unsigned long int from the current position in the file pointer
 *
 * @return int
 */
PHP_METHOD(Dao_Binary_Reader, readUnsignedInt32){

	zval *_endian = NULL, length = {}, bytes = {}, endian = {}, format = {}, result = {};

	dao_fetch_params(1, 0, 1, &_endian);

	if (!_endian || Z_TYPE_P(_endian) == IS_NULL) {
		dao_read_property(&endian, getThis(), SL("_endian"), PH_NOISY|PH_READONLY);
	} else {
		ZVAL_COPY_VALUE(&endian, _endian);
	}

	ZVAL_LONG(&length, 4);
	DAO_MM_CALL_METHOD(&bytes, getThis(), "read", &length);
	DAO_MM_ADD_ENTRY(&bytes);

	if (Z_LVAL(endian) == DAO_BINARY_ENDIAN_BIG) {
		DAO_MM_ZVAL_STRING(&format, "N");
	} else if (Z_LVAL(endian) == DAO_BINARY_ENDIAN_LITTLE) {
		DAO_MM_ZVAL_STRING(&format, "V");
	} else if (Z_LVAL(endian) == DAO_BINARY_ENDIAN_MACHINE) {
		DAO_MM_ZVAL_STRING(&format, "L");
	}

	DAO_MM_CALL_FUNCTION(&result, "unpack", &format, &bytes);
	DAO_MM_ADD_ENTRY(&result);

	if (!dao_array_isset_fetch_long(return_value, &result, 1, PH_COPY)) {
		RETURN_MM_NULL();
	}
	RETURN_MM();
}

/**
 * Read a float from the current position in the file pointer
 *
 * @return float
 */
PHP_METHOD(Dao_Binary_Reader, readFloat){

	zval length = {}, bytes = {}, format = {}, result = {};

	DAO_MM_INIT();

	ZVAL_LONG(&length, sizeof(float));
	DAO_MM_CALL_METHOD(&bytes, getThis(), "read", &length);
	DAO_MM_ADD_ENTRY(&bytes);

	DAO_MM_ZVAL_STRING(&format, "f");

	DAO_MM_CALL_FUNCTION(&result, "unpack", &format, &bytes);
	DAO_MM_ADD_ENTRY(&result);

	if (!dao_array_isset_fetch_long(return_value, &result, 1, PH_COPY)) {
		RETURN_MM_NULL();
	}
	RETURN_MM();
}

/**
 * Read a double from the current position in the file pointer
 *
 * @return double
 */
PHP_METHOD(Dao_Binary_Reader, readDouble){

	zval length = {}, bytes = {}, format = {}, result = {};

	DAO_MM_INIT();

	ZVAL_LONG(&length, sizeof(double));
	DAO_MM_CALL_METHOD(&bytes, getThis(), "read", &length);
	DAO_MM_ADD_ENTRY(&bytes);

	DAO_MM_ZVAL_STRING(&format, "d");

	DAO_MM_CALL_FUNCTION(&result, "unpack", &format, &bytes);
	DAO_MM_ADD_ENTRY(&result);

	if (!dao_array_isset_fetch_long(return_value, &result, 1, PH_COPY)) {
		RETURN_MM_NULL();
	}
	RETURN_MM();
}

/**
 * Read string from the current position in the file pointer
 *
 * @return string
 */
PHP_METHOD(Dao_Binary_Reader, readString){

	zval *length = NULL, input = {}, position = {}, eof_position = {}, data = {}, format = {}, result = {};
	php_stream *stream;

	dao_fetch_params(1, 0, 1, &length);

	if (length && Z_TYPE_P(length) != IS_NULL) {
		DAO_MM_CALL_METHOD(return_value, getThis(), "read", length);
		RETURN_MM();
	}

	dao_read_property(&input, getThis(), SL("_input"), PH_NOISY|PH_READONLY);
	dao_read_property(&position, getThis(), SL("_position"), PH_NOISY|PH_READONLY);
	dao_read_property(&eof_position, getThis(), SL("_eofPosition"), PH_NOISY|PH_READONLY);
	if (DAO_GE(&position, &eof_position)) {
		DAO_MM_THROW_EXCEPTION_STR(dao_binary_exception_ce, "Not enough input");
		return;
	}

	php_stream_from_res(stream, Z_RES(input));

	ZVAL_NEW_STR(&data, zend_string_alloc(Z_LVAL(eof_position), 0));
	Z_STRLEN(data) = php_stream_read(stream, Z_STRVAL(data), Z_LVAL(eof_position));
	Z_STRVAL(data)[Z_STRLEN(data)] = 0;
	DAO_MM_ADD_ENTRY(&data);

	ZVAL_STRING(&format, "Z*");
	DAO_MM_ADD_ENTRY(&format);

	DAO_MM_CALL_FUNCTION(&result, "unpack", &format, &data);
	DAO_MM_ADD_ENTRY(&result);

	if (!dao_array_isset_fetch_long(return_value, &result, 1, PH_COPY)) {
		RETURN_MM_NULL();
	} else {
		zval len = {}, new_position = {};
		ZVAL_LONG(&len, Z_STRLEN_P(return_value)+1);
		dao_add_function(&new_position, &position, &len);

		DAO_MM_CALL_METHOD(NULL, getThis(), "setposition", &new_position);
	}
	RETURN_MM();
}

/**
 * Read hex string from the current position in the file pointer
 *
 * @return string
 */
PHP_METHOD(Dao_Binary_Reader, readHexString){

	zval *length = NULL, *low_nibble = NULL, endian = {}, position = {}, eof_position = {}, format = {}, data = {}, result = {};

	dao_fetch_params(1, 0, 2, &length, &low_nibble);

	if (!low_nibble || Z_TYPE_P(low_nibble) == IS_NULL) {
		dao_read_property(&endian, getThis(), SL("_endian"), PH_NOISY|PH_READONLY);
		if (Z_LVAL(endian) == DAO_BINARY_ENDIAN_LITTLE) {
			low_nibble = &DAO_GLOBAL(z_true);
		} else {
			low_nibble = &DAO_GLOBAL(z_false);
		}
	}

	if (length && Z_TYPE_P(length) != IS_NULL) {
		DAO_MM_CALL_METHOD(&data, getThis(), "read", length);

		// 低位是否在前半字节
		if (zend_is_true(low_nibble)) {
			DAO_CONCAT_SV(&format, "h", length);
		} else {
			DAO_CONCAT_SV(&format, "H", length);
		}
		DAO_MM_ADD_ENTRY(&format);
		DAO_MM_CALL_FUNCTION(&result, "unpack", &format, &data);
		DAO_MM_ADD_ENTRY(&result);
		if (!dao_array_isset_fetch_long(return_value, &result, 1, PH_COPY)) {
			RETURN_MM_NULL();
		}
	} else {
		zval input = {};
		php_stream *stream;

		dao_read_property(&input, getThis(), SL("_input"), PH_NOISY|PH_READONLY);
		dao_read_property(&position, getThis(), SL("_position"), PH_NOISY|PH_READONLY);
		dao_read_property(&eof_position, getThis(), SL("_eofPosition"), PH_NOISY|PH_READONLY);
		if (DAO_GE(&position, &eof_position)) {
			DAO_MM_THROW_EXCEPTION_STR(dao_binary_exception_ce, "Not enough input");
			return;
		}

		php_stream_from_res(stream, Z_RES(input));

		ZVAL_NEW_STR(&data, zend_string_alloc(Z_LVAL(eof_position), 0));
		Z_STRLEN(data) = php_stream_read(stream, Z_STRVAL(data), Z_LVAL(eof_position));
		Z_STRVAL(data)[Z_STRLEN(data)] = 0;

		DAO_MM_ADD_ENTRY(&data);

		// 低位是否在前半字节
		if (zend_is_true(low_nibble)) {
			ZVAL_STRING(&format, "h*");
		} else {
			ZVAL_STRING(&format, "H*");
		}
		DAO_MM_ADD_ENTRY(&format);

		DAO_MM_CALL_FUNCTION(&result, "unpack", &format, &data);
		if (!dao_array_isset_fetch_long(return_value, &result, 1, PH_COPY)) {
			RETURN_MM_NULL();
		} else {
			zval len = {}, new_position = {};
			ZVAL_LONG(&len, Z_STRLEN_P(return_value)/2);
			dao_add_function(&new_position, &position, &len);

			DAO_MM_CALL_METHOD(NULL, getThis(), "setposition", &new_position);
		}
	}
	RETURN_MM();
}

/**
 * Write bytes to the end position in the file pointer
 *
 * @return boolean
 */
PHP_METHOD(Dao_Binary_Reader, append){

	zval *data, *length = NULL, position = {}, eof_position = {}, input = {};
	php_stream *stream;
	zend_long len, size;
	int ret;

	dao_fetch_params(0, 1, 1, &data, &length);
	
	if (!length || Z_LVAL_P(length) <= 0) {
		len = Z_STRLEN_P(data);
	} else {
		len = Z_LVAL_P(length);
	}

	dao_read_property(&position, getThis(), SL("_position"), PH_NOISY|PH_READONLY);
	dao_read_property(&eof_position, getThis(), SL("_eofPosition"), PH_NOISY|PH_READONLY);

	dao_read_property(&input, getThis(), SL("_input"), PH_NOISY|PH_READONLY);
	php_stream_from_res(stream, Z_RES(input));

	ret = php_stream_seek(stream, 0, SEEK_END);
	if (ret < 0) {
		RETURN_FALSE;
	}

	size = php_stream_write(stream, Z_STRVAL_P(data), len);
	ret = php_stream_seek(stream, Z_LVAL(position), SEEK_SET);
	if (size <= 0) {
		RETURN_FALSE;
	}

	dao_update_property_long(getThis(), SL("_eofPosition"), Z_LVAL(eof_position) + size);

	RETURN_TRUE;
}
