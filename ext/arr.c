
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

#include "arr.h"
#include "di.h"
#include "filterinterface.h"
#include "exception.h"

#include <ext/standard/php_array.h>
#include <ext/spl/spl_array.h>
#include <ext/pcre/php_pcre.h>

#include <Zend/zend_closures.h>
#include <Zend/zend_hash.h>

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"
#include "kernel/string.h"
#include "kernel/array.h"
#include "kernel/operators.h"
#include "kernel/concat.h"
#include "kernel/object.h"
#include "kernel/hash.h"
#include "kernel/debug.h"

#include "interned-strings.h"

/**
 * Dao\Arr
 *
 * Provides utilities to work with arrs
 */
zend_class_entry *dao_arr_ce;

PHP_METHOD(Dao_Arr, is_assoc);
PHP_METHOD(Dao_Arr, is_array);
PHP_METHOD(Dao_Arr, path);
PHP_METHOD(Dao_Arr, set_path);
PHP_METHOD(Dao_Arr, range);
PHP_METHOD(Dao_Arr, get);
PHP_METHOD(Dao_Arr, first);
PHP_METHOD(Dao_Arr, choice);
PHP_METHOD(Dao_Arr, extract);
PHP_METHOD(Dao_Arr, pluck);
PHP_METHOD(Dao_Arr, unshift);
PHP_METHOD(Dao_Arr, map);
PHP_METHOD(Dao_Arr, merge);
PHP_METHOD(Dao_Arr, overwrite);
PHP_METHOD(Dao_Arr, callback);
PHP_METHOD(Dao_Arr, flatten);
PHP_METHOD(Dao_Arr, arrayobject);
PHP_METHOD(Dao_Arr, key);
PHP_METHOD(Dao_Arr, filter);
PHP_METHOD(Dao_Arr, sum);
PHP_METHOD(Dao_Arr, toArray);
PHP_METHOD(Dao_Arr, aggr);
PHP_METHOD(Dao_Arr, group);
PHP_METHOD(Dao_Arr, flip);
PHP_METHOD(Dao_Arr, getHashKey);
PHP_METHOD(Dao_Arr, getHashValue);
PHP_METHOD(Dao_Arr, sort);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_arr_is_assoc, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_arr_is_array, 0, 0, 1)
	ZEND_ARG_INFO(0, array)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_arr_path, 0, 0, 2)
	ZEND_ARG_INFO(0, array)
	ZEND_ARG_INFO(0, path)
	ZEND_ARG_INFO(0, default_value)
	ZEND_ARG_INFO(0, delimiter)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_arr_set_path, 0, 0, 3)
	ZEND_ARG_TYPE_INFO(1, array, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, path)
	ZEND_ARG_INFO(0, value)
	ZEND_ARG_INFO(0, delimiter)
	ZEND_ARG_INFO(0, flag)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_arr_range, 0, 0, 0)
	ZEND_ARG_INFO(0, step)
	ZEND_ARG_INFO(0, max)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_arr_get, 0, 0, 2)
	ZEND_ARG_INFO(0, array)
	ZEND_ARG_INFO(0, key)
	ZEND_ARG_INFO(0, default_value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_arr_first, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, key)
	ZEND_ARG_INFO(0, default_value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_arr_choice, 0, 0, 3)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, key)
	ZEND_ARG_INFO(0, value1)
	ZEND_ARG_INFO(0, value2)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_arr_extract, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, paths)
	ZEND_ARG_INFO(0, default_value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_arr_pluck, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_arr_unshift, 0, 0, 3)
	ZEND_ARG_TYPE_INFO(1, array, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, key)
	ZEND_ARG_INFO(0, val)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_arr_map, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, callbacks)
	ZEND_ARG_INFO(0, keys)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_arr_merge, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, array1, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, array2, IS_ARRAY, 0)
	ZEND_ARG_VARIADIC_TYPE_INFO(0, arrays, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_arr_overwrite, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, array1, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, array2, IS_ARRAY, 0)
	ZEND_ARG_VARIADIC_TYPE_INFO(0, arrays, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_arr_callback, 0, 0, 1)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_arr_flatten, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_arr_arrayobject, 0, 0, 1)
	ZEND_ARG_INFO(0, array)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_arr_key, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, postion)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_arr_filter, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, callback)
	ZEND_ARG_TYPE_INFO(0, strict, _IS_BOOL, 1)
	ZEND_ARG_TYPE_INFO(0, allowEmpty, _IS_BOOL, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_arr_sum, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_arr_toarray, 0, 0, 1)
	ZEND_ARG_INFO(0, object)
	ZEND_ARG_TYPE_INFO(0, properties, IS_ARRAY, 1)
	ZEND_ARG_TYPE_INFO(0, recursive, _IS_BOOL, 1)
	ZEND_ARG_TYPE_INFO(0, negate, _IS_BOOL, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_arr_aggr, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, rows, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, aggregators, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_arr_group, 0, 0, 3)
	ZEND_ARG_TYPE_INFO(0, rows, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, fields, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, aggregators, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_arr_flip, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_arr_gethashkey, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, pos, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_arr_gethashvalue, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, pos, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_arr_sort, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(1, array, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, sort_field)
	ZEND_ARG_TYPE_INFO(0, sort_order, IS_LONG, 1)
	ZEND_ARG_TYPE_INFO(0, sort_flags, IS_LONG, 1)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_arr_method_entry[] = {
	PHP_ME(Dao_Arr, is_assoc, arginfo_dao_arr_is_assoc, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Arr, is_array, arginfo_dao_arr_is_array, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Arr, path, arginfo_dao_arr_path, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Arr, set_path, arginfo_dao_arr_set_path, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Arr, range, arginfo_dao_arr_range, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Arr, get, arginfo_dao_arr_get, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Arr, first, arginfo_dao_arr_first, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Arr, choice, arginfo_dao_arr_choice, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Arr, extract, arginfo_dao_arr_extract, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Arr, pluck, arginfo_dao_arr_pluck, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Arr, unshift, arginfo_dao_arr_unshift, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Arr, map, arginfo_dao_arr_map, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Arr, merge, arginfo_dao_arr_merge, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Arr, overwrite, arginfo_dao_arr_overwrite, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Arr, callback, arginfo_dao_arr_callback, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Arr, flatten, arginfo_dao_arr_flatten, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Arr, arrayobject, arginfo_dao_arr_arrayobject, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Arr, key, arginfo_dao_arr_key, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Arr, filter, arginfo_dao_arr_filter, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Arr, sum, arginfo_dao_arr_sum, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Arr, toArray, arginfo_dao_arr_toarray, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Arr, aggr, arginfo_dao_arr_aggr, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Arr, group, arginfo_dao_arr_group, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Arr, flip, arginfo_dao_arr_flip, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Arr, getHashKey, arginfo_dao_arr_gethashkey, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Arr, getHashValue, arginfo_dao_arr_gethashvalue, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Arr, sort, arginfo_dao_arr_sort, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};

/**
 * Dao\Arr initializer
 */
DAO_INIT_CLASS(Dao_Arr){

	DAO_REGISTER_CLASS(Dao, Arr, arr, dao_arr_method_entry, 0);

	// zend_declare_class_constant_stringl(dao_arr_ce, SL("delimiter"), SL("."));
	zend_declare_property_string(dao_arr_ce, SL("delimiter"), ".", ZEND_ACC_PUBLIC|ZEND_ACC_STATIC);

	zend_declare_class_constant_long(dao_arr_ce, SL("TYPE_BOOLEAN"),	DAO_ARR_TYPE_BOOLEAN);
	zend_declare_class_constant_long(dao_arr_ce, SL("TYPE_LONG"),		DAO_ARR_TYPE_LONG);
	zend_declare_class_constant_long(dao_arr_ce, SL("TYPE_DOUBLE"),		DAO_ARR_TYPE_DOUBLE);
	zend_declare_class_constant_long(dao_arr_ce, SL("TYPE_STRING"),		DAO_ARR_TYPE_STRING);

	zend_declare_class_constant_long(dao_arr_ce, SL("AGGR_SUM"),		DAO_ARR_AGGR_SUM);
	zend_declare_class_constant_long(dao_arr_ce, SL("AGGR_AVG"),		DAO_ARR_AGGR_AVG);
	zend_declare_class_constant_long(dao_arr_ce, SL("AGGR_COUNT"),	DAO_ARR_AGGR_COUNT);
	zend_declare_class_constant_long(dao_arr_ce, SL("AGGR_FIRST"),	DAO_ARR_AGGR_FIRST);
	zend_declare_class_constant_long(dao_arr_ce, SL("AGGR_LAST"),	DAO_ARR_AGGR_LAST);
	zend_declare_class_constant_long(dao_arr_ce, SL("AGGR_MIN"),		DAO_ARR_AGGR_MIN);
	zend_declare_class_constant_long(dao_arr_ce, SL("AGGR_MAX"),		DAO_ARR_AGGR_MAX);
	zend_declare_class_constant_long(dao_arr_ce, SL("AGGR_GROUP"),	DAO_ARR_AGGR_GROUP);

	return SUCCESS;
}

/**
 * Tests if an array is associative or not.
 *
 *<code>
 *	// Returns TRUE
 *	\Dao\Arr::is_assoc(array('username' => 'john.doe'))
 *</code>
 *
 * @param array $array
 * @return boolean
 */
PHP_METHOD(Dao_Arr, is_assoc){

	zval *array;

	dao_fetch_params(0, 1, 0, &array);

	if (dao_array_is_associative(array, 0)) {
		RETURN_TRUE;
	} else {
		RETURN_FALSE;
	}
}

/**
 * Test if a value is an array with an additional check for array-like objects.
 *
 *<code>
 *	// Returns TRUE
 *	\Dao\Arr::is_array(array());
 *</code>
 *
 * @param mixed $value
 * @return boolean
 */
PHP_METHOD(Dao_Arr, is_array){

	zval *value;

	dao_fetch_params(0, 1, 0, &value);

	if (Z_TYPE_P(value) == IS_ARRAY) {
		RETURN_TRUE;
	}

	RETURN_BOOL(Z_TYPE_P(value) == IS_OBJECT && zend_class_implements_interface(Z_OBJCE_P(value), zend_ce_traversable));
}

/**
 * Gets a value from an array using a dot separated path.
 *
 *<code>
 *	// Get the value of $array['foo']['bar']
 *	$value = \Dao\Arr::path($array, 'foo.bar');
 *</code>
 *
 * Using a wildcard "*" will search intermediate arrays and return an array.
 *
 *<code>
 *	// Get the values of "color" in theme
 *	$colors = \Dao\Arr::path($array, 'theme.*.color');
 *
 *	// Using an array of keys
 *	$colors = \Dao\Arr::path($array, array('theme', '*', 'color'));
 *</code>
 *
 * @param array $array
 * @param mixed $path
 * @param mixed $default
 * @param string $delimiter
 * @return mixed
 */
PHP_METHOD(Dao_Arr, path){

	zval *array, *path, *default_value = NULL, *_delimiter = NULL, delimiter = {}, keys = {};
	zval values = {};

	dao_fetch_params(1, 2, 2, &array, &path, &default_value, &_delimiter);

	if (!default_value) {
		default_value = &DAO_GLOBAL(z_null);
	}

	if (Z_TYPE_P(array) != IS_ARRAY) {
		RETURN_MM_CTOR(default_value);
	}
	DAO_SEPARATE_PARAM(array);

	if (!_delimiter || Z_TYPE_P(_delimiter) == IS_NULL) {
		dao_read_static_property_ce(&delimiter, dao_arr_ce, SL("delimiter"), PH_READONLY);
	} else {
		ZVAL_COPY_VALUE(&delimiter, _delimiter);
	}

	if (Z_TYPE_P(path) == IS_ARRAY) {
		DAO_MM_ZVAL_DUP(&keys, path);
	} else {
		if (dao_array_isset_fetch(return_value, array, path, PH_COPY)) {
			RETURN_MM();
		}

		dao_fast_explode(&keys, &delimiter, path);
		DAO_MM_ADD_ENTRY(&keys);
	}

	do {
		zval key = {};
		ZVAL_MAKE_REF(&keys);
		DAO_MM_CALL_FUNCTION(&key, "array_shift", &keys);
		ZVAL_UNREF(&keys);

		if (Z_TYPE(key) == IS_NULL) {
			break;
		}

		if (dao_array_isset_fetch(&values, array, &key, PH_READONLY)) {
			if (dao_fast_count_ev(&keys) > 0) {
				if (Z_TYPE(values) == IS_ARRAY) {
					array = &values;
				} else {
					// Unable to dig deeper
					zval_ptr_dtor(&key);
					break;
				}
			} else {
				zval_ptr_dtor(&key);
				RETURN_MM_CTOR(&values);
			}
		} else if (DAO_IS_STRING(&key, "*")) {
			zval *arr = NULL;
			array_init(return_value);

			ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(array), arr) {
				zval value = {};

				if (Z_TYPE_P(arr) == IS_ARRAY) {
					DAO_MM_CALL_CE_STATIC(&value, dao_arr_ce, "path", arr, &keys);

					if (Z_TYPE(value) != IS_NULL) {
						dao_array_append(return_value, &value, 0);
					}
				}
			} ZEND_HASH_FOREACH_END();

			if (dao_fast_count_ev(return_value)) {
				zval_ptr_dtor(&key);
				RETURN_MM();
			} else {
				// Unable to dig deeper
				zval_ptr_dtor(&key);
				break;
			}
		} else {
			// Unable to dig deeper
			zval_ptr_dtor(&key);
			break;
		}
		zval_ptr_dtor(&key);
	} while (dao_fast_count_ev(&keys));

	RETURN_MM_CTOR(default_value);
}

/**
 * Set a value on an array by path.
 *
 * Using a wildcard "*" will search intermediate arrays and return an array.
 *
 *<code>
 *	// Set the values of "color" in theme
 *	$array = array('theme' => array('one' => array('color' => 'green'), 'two' => array('size' => 11));
 *	\Dao\Arr::set_path($array, 'theme.*.color', 'red');
 *	// Result: array('theme' => array('one' => array('color' => 'red'), 'two' => array('size' => 11, 'color' => 'red'));
 *</code>
 *
 * @param array $array
 * @param string $path
 * @param mixed $value
 * @param string $delimiter
 */
PHP_METHOD(Dao_Arr, set_path){

	zval *array, *path, *value, *_delimiter = NULL, *flag = NULL, delimiter = {}, keys = {}, cpy_array = {};
	int found = 1;

	dao_fetch_params(0, 3, 2, &array, &path, &value, &_delimiter, &flag);
	ZVAL_DEREF(array);

	if (Z_TYPE_P(path) == IS_ARRAY) {
		ZVAL_DUP(&keys, path);
	} else {
		if (!_delimiter || Z_TYPE_P(_delimiter) == IS_NULL) {
			dao_read_static_property_ce(&delimiter, dao_arr_ce, SL("delimiter"), PH_READONLY);
		} else {
			ZVAL_COPY_VALUE(&delimiter, _delimiter);
		}

		dao_fast_explode(&keys, &delimiter, path);
	}

	if (!flag) {
		flag = &DAO_GLOBAL(z_false);
	}

	ZVAL_COPY_VALUE(&cpy_array, array);

	// Set current $array to inner-most array	path
	while ((int) zend_hash_num_elements(Z_ARRVAL(keys)) > 1) {
		zval key = {};
		ZVAL_MAKE_REF(&keys);
		DAO_CALL_FUNCTION(&key, "array_shift", &keys);
		ZVAL_UNREF(&keys);

		if (DAO_IS_STRING(&key, "*")) {
			zval *arr = NULL;
			ZEND_HASH_FOREACH_VAL(Z_ARRVAL(cpy_array), arr) {
				zval is_array = {};
				DAO_CALL_CE_STATIC(&is_array, dao_arr_ce, "is_array", arr);

				if (zend_is_true(&is_array)) {
					ZVAL_MAKE_REF(arr);
					DAO_CALL_CE_STATIC(NULL, dao_arr_ce, "set_path", arr, &keys, value, &DAO_GLOBAL(z_null), flag);
					ZVAL_UNREF(arr);
				}
			} ZEND_HASH_FOREACH_END();
			found = 0;
			zval_ptr_dtor(&key);
			break;
		} else {
			zval v = {};

			if (dao_is_long(&key)) {
				convert_to_long(&key);
			}

			if (dao_array_isset_fetch(&v, &cpy_array, &key, PH_READONLY)) {
				ZVAL_COPY_VALUE(&cpy_array, &v);
			} else {
				array_init(&v);
				dao_array_update(&cpy_array, &key, &v, 0);
			}
		}
		zval_ptr_dtor(&key);
	}

	if (found) {
		zval key = {};
		ZVAL_MAKE_REF(&keys);
		DAO_CALL_FUNCTION(&key, "array_shift", &keys);
		ZVAL_UNREF(&keys);

		if (zend_is_true(flag)) {
			zval v = {};
			if (dao_array_isset_fetch(&v, &cpy_array, &key, PH_COPY)) {
				if (Z_TYPE(v) != IS_ARRAY) {
					convert_to_array(&v);
				}
			} else {
				array_init(&v);
			}
			dao_array_append(&v, value, PH_COPY);
			dao_array_update(&cpy_array, &key, &v, 0);
		} else {
			dao_array_update(&cpy_array, &key, value, PH_COPY);
		}
		zval_ptr_dtor(&key);
	}
	zval_ptr_dtor(&keys);
}

/**
 * Fill an array with a range of numbers.
 *
 *<code>
 *	// Fill an array with values 5, 10, 15, 20
 *	$values = \Dao\Arr::range(5, 20);
 *</code>
 *
 * @param integer $step
 * @param integer $max
 * @return array
 */
PHP_METHOD(Dao_Arr, range){

	zval *step = NULL, *max = NULL;
	int i, s, m;

	dao_fetch_params(0, 0, 2, &step, &max);

	if (!step) {
		s = 10;
	} else {
		if (Z_TYPE_P(step) != IS_LONG) {
			DAO_SEPARATE_PARAM(step);
			convert_to_long(step);
		}
		s = Z_LVAL_P(step);

		if (s < 1) {
			RETURN_EMPTY_ARRAY();
		}
	}

	if (!max) {
		m = 100;
	} else {
		if (Z_TYPE_P(max) != IS_LONG) {
			DAO_SEPARATE_PARAM(max);
			convert_to_long(max);
		}

		m = Z_LVAL_P(max);
	}

	array_init(return_value);
	for (i = s; i <= m; i += s) {
		dao_array_update_long_long(return_value, i, i, PH_COPY);
	}
}

/**
 * Retrieve a single key from an array. If the key does not exist in the
 * array, the default value will be returned instead.
 *
 *<code>
 *	// Get the value "username" from $_POST, if it exists
 *	$username = \Dao\Arr::get($_POST, 'username');
 *</code>
 *
 * @param array|object $array
 * @param string|array|\Closure $key
 * @param mixed $default_value
 * @return mixed
 */
PHP_METHOD(Dao_Arr, get){

	zval *array, *keys, *default_value = NULL, *key, arguments = {}, value = {};

	dao_fetch_params(0, 2, 1, &array, &keys, &default_value);

	if (!default_value) {
		default_value = &DAO_GLOBAL(z_null);
	}

	if (Z_TYPE_P(keys) == IS_OBJECT && instanceof_function(Z_OBJCE_P(keys), zend_ce_closure)) {
		array_init_size(&arguments, 2);
		dao_array_append(&arguments, array, PH_COPY);
		dao_array_append(&arguments, default_value, PH_COPY);
		DAO_CALL_USER_FUNC_ARRAY(return_value, keys, &arguments);
		zval_ptr_dtor(&arguments);
		return;
	}

	if (Z_TYPE_P(array) == IS_ARRAY) {
		if (Z_TYPE_P(keys) == IS_ARRAY) {
			array_init(return_value);

			ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(keys), key) {
				zval value0 = {};
				if (dao_array_isset_fetch(&value0, array, key, PH_READONLY)) {
					dao_array_update(return_value, key, &value0, PH_COPY);
				}
			} ZEND_HASH_FOREACH_END();
			if (dao_fast_count_ev(return_value)) {
				return;
			}
		} else if (dao_array_isset_fetch(&value, array, keys, PH_READONLY)) {
			RETURN_CTOR(&value);
		}
	} else if (Z_TYPE_P(array) == IS_OBJECT) {
		if (dao_instance_of_ev(array, zend_ce_arrayaccess)) {
			if (Z_TYPE_P(keys) == IS_ARRAY) {
				array_init(return_value);

				ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(keys), key) {
					zval exists = {}, value0 = {};
					DAO_CALL_METHOD(&exists, array, "offsetexists", key);
					if (zend_is_true(&exists)) {
						DAO_CALL_METHOD(&value0, array, "offsetget", key);
						dao_array_update(return_value, key, &value0, 0);
					}
				} ZEND_HASH_FOREACH_END();
				if (dao_fast_count_ev(return_value)) {
					return;
				}
			} else {
				zval exists = {};
				DAO_CALL_METHOD(&exists, array, "offsetexists", keys);
				if (zend_is_true(&exists)) {
					DAO_CALL_METHOD(return_value, array, "offsetget", keys);
					return;
				}
			}
		} else {
			if (Z_TYPE_P(keys) == IS_ARRAY) {
				array_init(return_value);

				ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(keys), key) {
					zval value0 = {};
					if (dao_property_isset_fetch_zval(&value0, array, key, PH_READONLY)) {
						dao_array_update(return_value, key, &value0, PH_COPY);
					}
				} ZEND_HASH_FOREACH_END();
				if (dao_fast_count_ev(return_value)) {
					return;
				}
			} else if (dao_property_isset_fetch_zval(&value, array, keys, PH_READONLY)) {
				RETURN_CTOR(&value);
			}
		}
	}

	RETURN_CTOR(default_value);
}

/**
 * Retrieve a single key from an array, if value an array return the first element.
 *
 * @param array $array
 * @param string|array|\Closure $key
 * @param mixed $default_value
 * @return mixed
 */
PHP_METHOD(Dao_Arr, first){

	zval *array, *keys, *default_value = NULL, *key, value = {};

	dao_fetch_params(0, 2, 1, &array, &keys, &default_value);

	if (!default_value) {
		default_value = &DAO_GLOBAL(z_null);
	}

	if (Z_TYPE_P(keys) == IS_OBJECT && instanceof_function(Z_OBJCE_P(keys), zend_ce_closure)) {
		zval arguments = {};
		array_init_size(&arguments, 2);
		dao_array_append(&arguments, array, PH_COPY);
		dao_array_append(&arguments, default_value, PH_COPY);
		DAO_CALL_USER_FUNC_ARRAY(&value, keys, &arguments);
		zval_ptr_dtor(&arguments);
		if (Z_TYPE(value) != IS_ARRAY) {
			RETURN_ZVAL(&value, 0, 0);
		} else {
			zval real_value = {};
			dao_array_get_current(&real_value, &value);
			RETURN_ZVAL(&real_value, 0, 0);
		}
	}

	if (Z_TYPE_P(array) == IS_ARRAY) {
		if (Z_TYPE_P(keys) == IS_ARRAY) {
			array_init(return_value);

			ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(keys), key) {
				zval value0 = {};
				if (dao_array_isset_fetch(&value0, array, key, PH_READONLY)) {
					if (Z_TYPE(value0) != IS_ARRAY) {
						dao_array_update(return_value, key, &value0, PH_COPY);
					} else {
						zval real_value = {};
						dao_array_get_current(&real_value, &value0);
						dao_array_update(return_value, key, &real_value, 0);
					}
				}
			} ZEND_HASH_FOREACH_END();
			if (dao_fast_count_ev(return_value)) {
				return;
			}
		} else if (dao_array_isset_fetch(&value, array, keys, PH_READONLY)) {
			if (Z_TYPE(value) != IS_ARRAY) {
				RETURN_CTOR(&value);
			} else {
				zval real_value = {};
				dao_array_get_current(&real_value, &value);
				RETURN_ZVAL(&real_value, 0, 0);
			}
		}
	} else if (Z_TYPE_P(array) == IS_OBJECT) {
		if (Z_TYPE_P(keys) == IS_ARRAY) {
			array_init(return_value);

			ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(keys), key) {
				zval value0 = {};
				if (dao_property_isset_fetch_zval(&value0, array, key, PH_READONLY)) {
					if (Z_TYPE(value0) != IS_ARRAY) {
						dao_array_update(return_value, key, &value0, PH_COPY);
					} else {
						zval real_value = {};
						dao_array_get_current(&real_value, &value0);
						dao_array_update(return_value, key, &real_value, 0);
					}
				}
			} ZEND_HASH_FOREACH_END();
			if (dao_fast_count_ev(return_value)) {
				return;
			}
		} else if (dao_property_isset_fetch_zval(&value, array, keys, PH_READONLY)) {
			if (Z_TYPE(value) != IS_ARRAY) {
				RETURN_CTOR(&value);
			} else {
				zval real_value = {};
				dao_array_get_current(&real_value, &value);
				RETURN_ZVAL(&real_value, 0, 0);
			}
		}
	}

	RETURN_CTOR(default_value);
}

/**
 * Choice one value, If the key does not exist in the array, the value2 will be returned instead.
 *
 *	// Choice the "value1", if exists the value "email" of $_POST
 *	$username = \Dao\Arr::choice($_POST, 'email', 'value1', 'value2');
 *
 * @param array $array
 * @param string $key
 * @param string $value1
 * @param string $value2
 * @return mixed
 */
PHP_METHOD(Dao_Arr, choice){

	zval *array, *key, *value1, *value2 = NULL;

	dao_fetch_params(0, 3, 1, &array, &key, &value1, &value2);

	if (!value2) {
		value2 = &DAO_GLOBAL(z_null);
	}

	if (dao_array_isset(array, key)) {
		RETURN_CTOR(value1);
	}

	RETURN_CTOR(value2);
}

/**
 * Retrieves multiple paths from an array. If the path does not exist in the
 * array, the default value will be added instead.
 *
 *	// Get the values "username", "password" from $_POST
 *	$auth = \Dao\Arr::extract($_POST, array('username', 'password'));
 *
 *	// Get the value "level1.level2a" from $data
 *	$data = array('level1' => array('level2a' => 'value 1', 'level2b' => 'value 2'));
 *	\Dao\Arr::extract($data, array('level1.level2a', 'password'));
 *
 * @param array $array
 * @param array $paths
 * @param mixed $default_value
 * @return array
 */
PHP_METHOD(Dao_Arr, extract){

	zval *array, *paths, *default_value = NULL, *path;

	dao_fetch_params(0, 2, 1, &array, &paths, &default_value);

	if (!default_value) {
		default_value = &DAO_GLOBAL(z_null);
	}

	array_init(return_value);

	ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(paths), path) {
		zval value = {};
		DAO_CALL_CE_STATIC(&value, dao_arr_ce, "path", array, path, default_value);

		ZVAL_MAKE_REF(return_value);
		DAO_CALL_CE_STATIC(NULL, dao_arr_ce, "set_path", return_value, path, &value);
		ZVAL_UNREF(return_value);
		zval_ptr_dtor(&value);
	} ZEND_HASH_FOREACH_END();
}

/**
 * Retrieves muliple single-key values from a list of arrays.
 *
 *	// Get all of the "id" values from a result
 *	$ids = \Dao\Arr::pluck($result, 'id');
 *
 * @param array $array
 * @param string $key
 * @return array
 */
PHP_METHOD(Dao_Arr, pluck){

	zval *array, *key, *row;

	dao_fetch_params(0, 2, 0, &array, &key);

	array_init(return_value);

	ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(array), row) {
		zval value = {};
		if (dao_array_isset_fetch(&value, row, key, PH_READONLY)) {
			dao_array_append(return_value, &value, PH_COPY);
		}
	} ZEND_HASH_FOREACH_END();
}

/**
 * Adds a value to the beginning of an associative array.
 *
 *	// Add an empty value to the start of a select list
 *	\Dao\Arr::unshift($array, 'none', 'Select a value');
 *
 * @param array $array
 * @param string $key
 * @param mixed $val
 * @return array
 */
PHP_METHOD(Dao_Arr, unshift){

	zval *array, *key, *val, tmp = {};

	dao_fetch_params(0, 3, 0, &array, &key, &val);

	DAO_CALL_FUNCTION(&tmp, "array_reverse", array, &DAO_GLOBAL(z_true));

	dao_array_update(&tmp, key, val, PH_COPY);

	DAO_RETURN_CALL_FUNCTION("array_reverse", &tmp, &DAO_GLOBAL(z_true));
	zval_ptr_dtor(&tmp);
}

/**
 * Recursive version of [array_map](http://php.net/array_map), applies one or more
 * callbacks to all elements in an array, including sub-arrays.
 *
 *	// Apply "strip_tags" to every element in the array
 *	$array = \Dao\Arr::map($array, 'strip_tags');
 *
 *	// Apply $this->filter to every element in the array
 *	$array = \Dao\Arr::map($array, array(array($this,'filter')));
 *
 * @param array $array
 * @param mixed $callbacks
 * @return array
 */
PHP_METHOD(Dao_Arr, map){

	zval *array, *callbacks, *keys = NULL, *val;
	zend_string *str_key;
	ulong idx;

	dao_fetch_params(0, 2, 1, &array, &callbacks, &keys);
	DAO_SEPARATE_PARAM(array);

	ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(array), idx, str_key, val) {
		zval key = {}, value = {}, *callback, params = {};
		if (str_key) {
			ZVAL_STR(&key, str_key);
		} else {
			ZVAL_LONG(&key, idx);
		}
		if (Z_TYPE_P(val) == IS_ARRAY) {
			DAO_CALL_CE_STATIC(&value, dao_arr_ce, "map", val, callbacks);
			dao_array_update(array, &key, &value, 0);
		} else if (!keys || Z_TYPE_P(keys) != IS_ARRAY || dao_fast_in_array(&key, keys)) {
			if (Z_TYPE_P(callbacks) == IS_ARRAY) {
				ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(callbacks), callback) {
					array_init(&params);
					dao_array_update_long(&params, 0, val, PH_COPY);

					DAO_CALL_USER_FUNC_ARRAY(&value, callback, &params);
					dao_array_update(array, &key, &value, 0);
					zval_ptr_dtor(&params);
				} ZEND_HASH_FOREACH_END();
			} else {
				array_init(&params);
				dao_array_update_long(&params, 0, val, PH_COPY);

				DAO_CALL_USER_FUNC_ARRAY(&value, callbacks, &params);
				dao_array_update(array, &key, &value, 0);
				zval_ptr_dtor(&params);
			}
		}
	} ZEND_HASH_FOREACH_END();

	RETURN_CTOR(array);
}

/**
 * Recursively merge two or more arrays. Values in an associative array
 * overwrite previous values with the same key. Values in an indexed array
 * are appended, but only when they do not already exist in the result.
 *
 * Note that this does not work the same as [array_merge_recursive](http://php.net/array_merge_recursive)!
 *
 *	$john = array('name' => 'john', 'children' => array('fred', 'paul', 'sally', 'jane'));
 *	$mary = array('name' => 'mary', 'children' => array('jane'));
 *
 *	// John and Mary are married, merge them together
 *	$john = \Dao\Arr::merge($john, $mary);
 *
 *	// The output of $john will now be:
 *	array('name' => 'mary', 'children' => array('fred', 'paul', 'sally', 'jane'))
 *
 * @param array $array1
 * @param array $array2,...
 * @return array
 */
PHP_METHOD(Dao_Arr, merge){

	zval *array1, *array2, *value, *args;
	zend_string *str_key;
	ulong idx;
	uint32_t i;

	dao_fetch_params(0, 2, 0, &array1, &array2);
	ZVAL_DUP(return_value, array1);

	if (dao_array_is_associative(array2, 0)) {
		ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(array2), idx, str_key, value) {
			zval tmp = {}, arr = {}, value1 = {};
			if (str_key) {
				ZVAL_STR(&tmp, str_key);
			} else {
				ZVAL_LONG(&tmp, idx);
			}
			if (Z_TYPE_P(value) == IS_ARRAY && dao_array_isset_fetch(&value1, array1, &tmp, PH_READONLY)) {
				if (Z_TYPE(value1) == IS_ARRAY) {
					DAO_CALL_CE_STATIC(&arr, dao_arr_ce, "merge", &value1, value);

					dao_array_update(return_value, &tmp, &arr, 0);
				} else {
					dao_array_update(return_value, &tmp, value, PH_COPY);
				}
			} else {
				dao_array_update(return_value, &tmp, value, PH_COPY);
			}
		} ZEND_HASH_FOREACH_END();
	} else {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(array2), value) {
			if (!dao_fast_in_array(value, array1)) {
				dao_array_append(return_value, value, PH_COPY);
			}
		} ZEND_HASH_FOREACH_END();
	}

	if (ZEND_NUM_ARGS() > 2) {
		args = (zval *)safe_emalloc(ZEND_NUM_ARGS(), sizeof(zval), 0);
		if (zend_get_parameters_array_ex(ZEND_NUM_ARGS(), args) == FAILURE) {
			efree(args);
			WRONG_PARAM_COUNT;
		}

		for (i = 2; i < ZEND_NUM_ARGS(); i++) {
			zval tmp = {};
			DAO_CALL_CE_STATIC(&tmp, dao_arr_ce, "merge", return_value, &args[i]);
			zval_ptr_dtor(return_value);
			ZVAL_COPY_VALUE(return_value, &tmp);
		}
		efree(args);
	}
}

/**
 * Overwrites an array with values from input arrays.
 * Keys that do not exist in the first array will not be added!
 *
 *	$a1 = array('name' => 'john', 'mood' => 'happy', 'food' => 'bacon');
 *	$a2 = array('name' => 'jack', 'food' => 'tacos', 'drink' => 'beer');
 *
 *	// Overwrite the values of $a1 with $a2
 *	$array = \Dao\Arr::overwrite($a1, $a2);
 *
 *	// The output of $array will now be:
 *	array('name' => 'jack', 'mood' => 'happy', 'food' => 'tacos')
 *
 * @param array $array1
 * @param array $array2
 * @return array
 */
PHP_METHOD(Dao_Arr, overwrite){

	zval *array1, *array2, array = {}, *value, *args;
	zend_string *key;
	ulong idx;
	uint32_t i;

	dao_fetch_params(0, 2, 0, &array1, &array2);
	ZVAL_DUP(return_value, array1);

	if (Z_TYPE_P(array2) != IS_ARRAY) {
		DAO_SEPARATE_PARAM(array2);
		convert_to_array(array2);
	}

	DAO_CALL_FUNCTION(&array, "array_intersect_key", array2, return_value);

	ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL(array), idx, key, value) {
		if (key) {
			dao_array_update_string(return_value, key, value, PH_COPY);
		} else {
			dao_array_update_long(return_value, idx, value, PH_COPY);
		}
	} ZEND_HASH_FOREACH_END();
	zval_ptr_dtor(&array);

	if (ZEND_NUM_ARGS() > 2) {
		args = (zval *)safe_emalloc(ZEND_NUM_ARGS(), sizeof(zval), 0);
		if (zend_get_parameters_array_ex(ZEND_NUM_ARGS(), args) == FAILURE) {
			efree(args);
			WRONG_PARAM_COUNT;
		}

		for (i = 2; i < ZEND_NUM_ARGS(); i++) {
			zval tmp = {};
			DAO_CALL_CE_STATIC(&tmp, dao_arr_ce, "overwrite", return_value, &args[i]);
			zval_ptr_dtor(return_value);
			ZVAL_COPY_VALUE(return_value, &tmp);
		}
		efree(args);
	}
}

/**
 * Creates a callable function and parameter list from a string representation.
 * Note that this function does not validate the callback string.
 *
 *	// Get the callback function and parameters
 *	list($func, $params) = \Dao\Arr::callback('Foo::bar(apple,orange)');
 *
 *	// Get the result of the callback
 *	$result = call_user_func_array($func, $params);
 *
 * @param string $str
 * @return array function, params
 */
PHP_METHOD(Dao_Arr, callback){

	zval *str, pattern = {}, matches = {}, ret = {}, command = {}, match = {}, split = {}, search = {}, replace = {}, params = {};

	dao_fetch_params(1, 1, 0, &str);

	DAO_MM_ZVAL_STRING(&pattern, "#^([^\\(]*+)\\((.*)\\)$#");

	ZVAL_NULL(&matches);
	RETURN_MM_ON_FAILURE(dao_preg_match(&ret, &pattern, str, &matches, 0, 0));
	DAO_MM_ADD_ENTRY(&matches);

	if (zend_is_true(&ret)) {
		if (!dao_array_isset_fetch_long(&command, &matches, 1, PH_READONLY)) {
			ZVAL_EMPTY_STRING(&command);
			DAO_MM_ADD_ENTRY(&command);
		}

		if (dao_array_isset_fetch_long(&match, &matches, 2, PH_READONLY)) {
			zval regex = {};
			DAO_MM_ZVAL_STRING(&regex, "#(?<!\\\\\\\\),#");
			DAO_MM_CALL_FUNCTION(&split, "preg_split", &regex, &match);
			DAO_MM_ADD_ENTRY(&split);

			DAO_MM_ZVAL_STRING(&search, "\\,");
			DAO_MM_ZVAL_STRING(&replace, ",");

			DAO_STR_REPLACE(&params, &search, &replace, &split);
			DAO_MM_ADD_ENTRY(&params);
		}
	} else {
		ZVAL_COPY_VALUE(&command, str);
	}

	array_init(return_value);

	if (dao_memnstr_str(&command, SL("::"))) {
		zval command_parts = {};
		dao_fast_explode_str(&command_parts, SL("::"), &command);
		dao_array_append(return_value, &command_parts, 0);
	} else {
		dao_array_append(return_value, &command, PH_COPY);
	}

	if (Z_TYPE(params) > IS_NULL) {
		dao_array_append(return_value, &params, PH_COPY);
	}
	RETURN_MM();
}

/**
 * Convert a multi-dimensional array into a single-dimensional array.
 *
 *	$array = array('set' => array('one' => 'something'), 'two' => 'other');
 *
 *	// Flatten the array
 *	$array = \Dao\Arr::flatten($array);
 *
 *	// The array will now be
 *	array('one' => 'something', 'two' => 'other');
 *
 * @param array $array
 * @return array
 */
PHP_METHOD(Dao_Arr, flatten){

	zval *array, is_assoc = {}, *value;
	zend_string *key;
	ulong idx;

	dao_fetch_params(0, 1, 0, &array);

	DAO_CALL_CE_STATIC(&is_assoc, dao_arr_ce, "is_assoc", array);

	array_init(return_value);

	ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(array), idx, key, value) {
		if (Z_TYPE_P(value) == IS_ARRAY) {
			zval arr = {};
			DAO_CALL_CE_STATIC(&arr, dao_arr_ce, "flatten", value);

			php_array_merge(Z_ARRVAL_P(return_value), Z_ARRVAL(arr));
			zval_ptr_dtor(&arr);
		} else {
			if (zend_is_true(&is_assoc)) {
				if (key) {
					dao_array_update_string(return_value, key, value, PH_COPY);
				} else {
					dao_array_update_long(return_value, idx, value, PH_COPY);
				}
			} else {
				dao_array_append(return_value, value, PH_COPY);
			}
		}
	} ZEND_HASH_FOREACH_END();
}

/**
 * Convert a array to a array object.
 *
 *	$array = array('name' => 'Dao7', 'version' => '1.0.x');
 *
 *	$arrayobject = \Dao\Arr::arrayobject($array);
 *
 * @param array $array
 * @return ArrayObject
 */
PHP_METHOD(Dao_Arr, arrayobject){

	zval *array, arrayobject = {};

	dao_fetch_params(0, 1, 0, &array);

	object_init_ex(&arrayobject, spl_ce_ArrayObject);
	DAO_CALL_METHOD(NULL, &arrayobject, "__construct", array);

	RETVAL_ZVAL(&arrayobject, 0, 0);
}

/**
 * Gets array key of the postion
 *
 *	$array = array('name' => 'Dao7', 'version' => '1.0.x');
 *
 *	$key = \Dao\Arr::key($array, 1);
 *
 * @param array $array
 * @param int $postion
 * @return mixed
 */
PHP_METHOD(Dao_Arr, key){

	zval *array, *postion = NULL, arrayobject = {}, arrayiterator = {}, ret = {};

	dao_fetch_params(0, 1, 1, &array, &postion);

	object_init_ex(&arrayobject, spl_ce_ArrayObject);
	DAO_CALL_METHOD(NULL, &arrayobject, "__construct", array);

	DAO_CALL_METHOD(&arrayiterator, &arrayobject, "getIterator");

	DAO_CALL_METHOD(&ret, &arrayiterator, "valid");
	if (!zend_is_true(&ret)) {
		RETURN_NULL();
	}

	if (postion) {
		DAO_CALL_METHOD(NULL, &arrayiterator, "seek", postion);
	}

	DAO_CALL_METHOD(return_value, &arrayiterator, "key");
	zval_ptr_dtor(&arrayiterator);
	zval_ptr_dtor(&arrayobject);
}

/**
 * Filters elements of an array using a the filter
 *
 *	$array = array('name' => 'Dao7', 'version' => '1.0.x');
 *
 *	$key = \Dao\Arr::filter($array, 'int');
 *
 * @param array $array
 * @param mixed $filters
 * @return array
 */
PHP_METHOD(Dao_Arr, filter){

	zval *array, *filters = NULL, *strict = NULL, *allow_empty = NULL, dependency_injector = {}, service = {}, filter = {}, *value;
	zend_string *str_key;
	ulong idx;

	dao_fetch_params(0, 1, 3, &array, &filters, &strict, &allow_empty);

	if (!strict) {
		strict = &DAO_GLOBAL(z_false);
	}

	if (!allow_empty) {
		allow_empty = &DAO_GLOBAL(z_false);
	}

	array_init(return_value);
	if (zend_hash_num_elements(Z_ARRVAL_P(array)) == 0) {
		return;
	}

	if (!filters || Z_TYPE_P(filters) == IS_NULL) {
		ZEND_HASH_FOREACH_KEY_VAL_IND(Z_ARRVAL_P(array), idx, str_key, value) {
			if (!zend_is_true(allow_empty)) {
				if (strict) {
					if (DAO_IS_EMPTY(value)) {
						continue;
					}
				} else if (!zend_is_true(value)) {
					continue;
				}
			}

			if (str_key) {
				value = zend_hash_update(Z_ARRVAL_P(return_value), str_key, value);
			} else {
				value = zend_hash_index_update(Z_ARRVAL_P(return_value), idx, value);
			}
			zval_add_ref(value);
		} ZEND_HASH_FOREACH_END();
		return;
	}

	DAO_CALL_CE_STATIC(&dependency_injector, dao_di_ce, "getdefault");

	ZVAL_STR(&service, IS(filter));

	DAO_CALL_METHOD(&filter, &dependency_injector, "getshared", &service);
	zval_ptr_dtor(&dependency_injector);
	DAO_VERIFY_INTERFACE(&filter, dao_filterinterface_ce);

	ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(array), idx, str_key, value) {
		zval filter_value = {};
		DAO_CALL_METHOD(&filter_value, &filter, "sanitize", value, filters);
		if (!zend_is_true(allow_empty)) {
			if (strict) {
				if (DAO_IS_EMPTY(&filter_value)) {
					continue;
				}
			} else if (!zend_is_true(&filter_value)) {
				continue;
			}
		}
		if (str_key) {
			dao_array_update_string(return_value, str_key, &filter_value, 0);
		} else {
			dao_array_update_long(return_value, idx, &filter_value, 0);
		}
	} ZEND_HASH_FOREACH_END();
	zval_ptr_dtor(&filter);
}

/**
 * Return the sum of all the values in the array using a dot separated path
 *
 * @param array $array
 * @param mixed $path
 * @param mixed $default
 * @param string $delimiter
 * @return number
 */
PHP_METHOD(Dao_Arr, sum){

	zval *array, *path, *default_value = NULL, *_delimiter = NULL, delimiter = {}, values = {};

	dao_fetch_params(0, 2, 2, &array, &path, &default_value, &_delimiter);

	if (!default_value) {
		default_value = &DAO_GLOBAL(z_null);
	}

	if (!_delimiter || Z_TYPE_P(_delimiter) == IS_NULL) {
		dao_read_static_property_ce(&delimiter, dao_arr_ce, SL("delimiter"), PH_READONLY);
	} else {
		ZVAL_COPY_VALUE(&delimiter, _delimiter);
	}

	DAO_CALL_CE_STATIC(&values, dao_arr_ce, "path", array, path, default_value, &delimiter);

	if (Z_TYPE(values) == IS_ARRAY) {
		DAO_RETURN_CALL_FUNCTION("array_sum", &values);
		zval_ptr_dtor(&values);
	} else {
		RETVAL_ZVAL(&values, 0, 0);
	}
}

/**
 * Converts an object or an array of objects into an array
 *
 *<code>
 *	print_r(Dao\Arr::toArray($user);
 *</code>
 *
 * @param object|array|string $object
 * @param array $properties
 * @param bool $recursive
 * @return array
 */
PHP_METHOD(Dao_Arr, toArray){

	zval *object, *properties = NULL, *recursive = NULL, *negate = NULL, *value;
	zend_string *str_key;
	ulong idx;

	dao_fetch_params(0, 1, 3, &object, &properties, &recursive, &negate);

	if (!properties) {
		properties = &DAO_GLOBAL(z_null);
	}

	if (!recursive) {
		recursive = &DAO_GLOBAL(z_false);
	}

	if (!negate) {
		negate = &DAO_GLOBAL(z_false);
	}

	if (Z_TYPE_P(object) == IS_OBJECT) {
		zval tmp = {};
		dao_get_object_vars(&tmp, object, 1);

		array_init(return_value);
		
		ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL(tmp), idx, str_key, value) {
			zval key = {};
			if (str_key) {
				ZVAL_STR(&key, str_key);
			} else {
				ZVAL_LONG(&key, idx);
			}
			if (unlikely(Z_TYPE_P(properties) == IS_ARRAY)) {
				if (likely(!zend_is_true(negate))) {
					if (dao_fast_in_array(&key, properties)) {
						dao_array_update(return_value, &key, value, PH_COPY);
					}
				} else {
					if (!dao_fast_in_array(&key, properties)) {
						dao_array_update(return_value, &key, value, PH_COPY);
					}
				}
			} else {
				dao_array_update(return_value, &key, value, PH_COPY);
			}
		} ZEND_HASH_FOREACH_END();
		zval_ptr_dtor(&tmp);

		if (zend_is_true(recursive)) {
			ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(return_value), idx, str_key, value) {
				zval key = {}, array_value = {};
				if (str_key) {
					ZVAL_STR(&key, str_key);
				} else {
					ZVAL_LONG(&key, idx);
				}

				if (Z_TYPE_P(value) == IS_OBJECT && Z_TYPE_P(value) == IS_ARRAY) {
					DAO_CALL_CE_STATIC(&array_value, dao_arr_ce, "toarray", value);
					dao_array_update(return_value, &key, &array_value, 0);
				}
			} ZEND_HASH_FOREACH_END();
		}
	} else if (Z_TYPE_P(object) == IS_ARRAY) {
		array_init(return_value);
		ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(object), idx, str_key, value) {
			zval key = {};
			if (str_key) {
				ZVAL_STR(&key, str_key);
			} else {
				ZVAL_LONG(&key, idx);
			}

			if (Z_TYPE_P(properties) == IS_ARRAY) {
				if (likely(!zend_is_true(negate))) {
					if (dao_fast_in_array(&key, properties)) {
						dao_array_update(return_value, &key, value, PH_COPY);
					}
				} else {
					if (!dao_fast_in_array(&key, properties)) {
						dao_array_update(return_value, &key, value, PH_COPY);
					}
				}
			} else {
				dao_array_update(return_value, &key, value, PH_COPY);
			}
		} ZEND_HASH_FOREACH_END();

		if (!recursive || zend_is_true(recursive)) {
			ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(return_value), idx, str_key, value) {
				zval key = {}, array_value = {};
				if (str_key) {
					ZVAL_STR(&key, str_key);
				} else {
					ZVAL_LONG(&key, idx);
				}
				if (Z_TYPE_P(value) == IS_OBJECT && Z_TYPE_P(value) == IS_ARRAY) {
					DAO_CALL_CE_STATIC(&array_value, dao_arr_ce, "toarray", value);
					dao_array_update(return_value, &key, &array_value, 0);
				}
			} ZEND_HASH_FOREACH_END();
		}
	} else {
		array_init_size(return_value, 1);
		dao_array_append(return_value, object, PH_COPY);
	}
}

zend_always_inline static uint isa_cast(dao_arr_aggregator *agt, zval *val)
{
	if ((Z_TYPE_P(val) == IS_LONG && Z_TYPE_P(val) == IS_DOUBLE)) {
		return 1;
	}
	switch (agt->isa) {
		case DAO_ARR_TYPE_LONG:
		convert_to_long(val);
		return 1;

		case DAO_ARR_TYPE_DOUBLE:
		convert_to_double(val);
		return 1;

		case DAO_ARR_TYPE_BOOLEAN:
		convert_to_boolean(val);
		return 1;

		default:
		return 0;
	}
	return 1;
}

zend_always_inline static void dao_arr_compile_aggregator_selector_default(dao_arr_aggregator *agt, zend_long num_alias, zend_string * alias)
{
	if (alias) {
		agt->alias = alias;
		agt->selector = alias;
	} else {
		agt->alias = NULL;
		agt->selector = NULL;
		agt->num_alias = num_alias;
		agt->num_selector = num_alias;
	}
}

zend_always_inline static int dao_arr_compile_aggregator_selector(dao_arr_aggregator *agt, zval * agt_def, zend_long num_alias, zend_string * alias)
{
	zval * sel = zend_hash_str_find(Z_ARRVAL_P(agt_def), "selector", sizeof("selector") -1);
	if (sel) {
		ZVAL_DEREF(sel);

		if (Z_TYPE_P(sel) == IS_STRING) { 

			agt->selector = Z_STR_P(sel);

		} else if (Z_TYPE_P(sel) == IS_LONG) {

			agt->num_selector = Z_LVAL_P(sel);

		} else {
			php_error_docref(NULL, E_USER_ERROR, "Invalid selector type.");
			return -1;
		}

		if (alias) {
			agt->alias = alias;
		} else {
			agt->num_alias = num_alias;
		}
	} else {
		if (alias) {
			agt->selector = alias;
			agt->alias = alias;
		} else {
			agt->selector = NULL;
			agt->num_selector = num_alias;
			agt->alias = NULL;
			agt->num_alias = num_alias;
		}
	}
	return 0;
}

static zend_always_inline int dao_arr_compile_aggregator_isa(dao_arr_aggregator *agt, zval * agt_def)
{
	zval * isa = zend_hash_str_find(Z_ARRVAL_P(agt_def), "isa", sizeof("isa")-1);
	if (!isa) {
		agt->isa = DAO_ARR_TYPE_LONG;
		return 0;
	}

	switch (Z_TYPE_P(isa)) {
	case IS_LONG:
		agt->isa = Z_LVAL_P(isa);
		break;

	case IS_STRING:
		if (strncasecmp(Z_STRVAL_P(isa), "int", sizeof("int")-1) == 0) {
			agt->isa = DAO_ARR_TYPE_LONG;
		} else if (strncasecmp(Z_STRVAL_P(isa), "double", sizeof("double")-1) == 0) {
			agt->isa = DAO_ARR_TYPE_DOUBLE;
		} else if (strncasecmp(Z_STRVAL_P(isa), "boolean", sizeof("boolean")-1) == 0) {
			agt->isa = DAO_ARR_TYPE_BOOLEAN;
		} else {
			php_error_docref(NULL, E_USER_ERROR, "Unsupported aggregator typename.");
			return -1;
		}
		break;

	default:
		php_error_docref(NULL, E_USER_ERROR, "Unsupported aggregator data type.");
		return -1;
	}
	return 0;
}

static zend_always_inline int dao_arr_compile_aggregator_function_callable(dao_arr_aggregator *agt, zval *type)
{
	agt->is_callable = 1;
	agt->type = type;

	// fetch fci
	agt->fci = empty_fcall_info;
	agt->fci_cache = empty_fcall_info_cache;

	char *errstr = NULL;
	if (zend_fcall_info_init(type, 0, &agt->fci, &agt->fci_cache, NULL, &errstr) == FAILURE) {
		php_error_docref(NULL, E_USER_ERROR, "Error setting converter callback: %s", errstr);
		efree(errstr);
		return -1;
	}

	agt->fci.param_count = 2;

	return 0;
}


static zend_always_inline int dao_arr_compile_aggregator_function(dao_arr_aggregator *agt, zval *agt_def)
{
	zval *fun;
	fun = zend_hash_str_find(Z_ARRVAL_P(agt_def), "aggregator", sizeof("aggregator") - 1);
	if (fun == NULL) {
		php_error_docref(NULL, E_USER_ERROR, "Aggregator is not defined.");
		return -1;
	}
	ZVAL_DEREF(fun);

	if (Z_TYPE_P(fun) == IS_LONG) {
		agt->is_callable = 0;
		agt->type = fun;
	} else if (zend_is_callable(fun, 0, NULL)) {
		return dao_arr_compile_aggregator_function_callable(agt, fun);
	} else {
		php_error_docref(NULL, E_USER_ERROR, "Invalid aggregator function.");
		return -1;
	}
	return 0;
}

static zend_always_inline int dao_arr_compile_aggregator(dao_arr_aggregator *agt, zval *agt_def, ulong num_alias, zend_string *alias)
{
	if (Z_TYPE_P(agt_def) == IS_LONG) {

		agt->is_callable = 0;
		agt->type = agt_def;
		agt->isa = DAO_ARR_TYPE_LONG;
		dao_arr_compile_aggregator_selector_default(agt, num_alias, alias);
	} else if (zend_is_callable(agt_def, 0, NULL)) {

		if (dao_arr_compile_aggregator_function_callable(agt, agt_def) != 0) {
			return -1;
		}
		agt->isa = DAO_ARR_TYPE_LONG;
		dao_arr_compile_aggregator_selector_default(agt, num_alias, alias);
	} else if (Z_TYPE_P(agt_def) == IS_ARRAY) {
		if (dao_arr_compile_aggregator_function(agt, agt_def) != 0) {
			return -1;
		}
		if (dao_arr_compile_aggregator_selector(agt, agt_def, num_alias, alias) != 0) {
			return -1;
		}
		if (dao_arr_compile_aggregator_isa(agt, agt_def) != 0) {
			return -1;
		}
	} else {
		php_error_docref(NULL, E_USER_ERROR, "Unsupported aggregator");
		return -1;
	}
	
	return 0;
}

static zend_always_inline int dao_arr_compile_aggregators(dao_arr_aggregator *agts, zval *aggregators)
{
	uint agt_idx = 0;
	ulong num_alias;
	zend_string *alias;
	zval *arg;

	ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(aggregators), num_alias, alias, arg) {
		if (dao_arr_compile_aggregator(&agts[agt_idx++], arg, num_alias, alias) != 0) {
			return -1;
		}
	} ZEND_HASH_FOREACH_END();
	return 0;
}


static zend_always_inline int dao_arr_aggregate(zval *result, zval* rows, zval* fields, dao_arr_aggregator* agts, uint agts_cnt)
{
	zval *row, *carry_val, *current_val, *first, *field;

	uint agt_idx;
	dao_arr_aggregator *current_agt;

	array_init(result);

	HashTable *ht = Z_ARRVAL_P(rows);
	HashTable *result_ht = Z_ARRVAL_P(result);
	zend_long cnt = zend_array_count(ht);

	zend_hash_internal_pointer_reset(ht);

	if (fields && (first = zend_hash_get_current_data(ht)) != NULL) {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(fields), field) {

			if (Z_TYPE_P(field) == IS_STRING) {
				if ((carry_val = zend_hash_find(Z_ARRVAL_P(first), Z_STR_P(field))) != NULL) {
					zend_hash_add_new(result_ht, Z_STR_P(field), carry_val);
				}
			} else if (Z_TYPE_P(field) == IS_LONG) {
				if ((carry_val = zend_hash_index_find(Z_ARRVAL_P(first), Z_LVAL_P(field))) != NULL) {
					zend_hash_index_add_new(result_ht, Z_LVAL_P(field), carry_val);
				}
			} else {
				php_error_docref(NULL, E_USER_ERROR, "Unsupported field value type.");
				return -1;
			}

		} ZEND_HASH_FOREACH_END();

		// TODO: Aggregate value from the first row.
		for (agt_idx = 0; agt_idx < agts_cnt ; agt_idx++) {
			current_agt = &agts[agt_idx];
			if (Z_TYPE_P(current_agt->type) == IS_LONG) {
				carry_val = DAO_ARR_HASH_FIND(result_ht, current_agt->num_alias, current_agt->alias);
			}
		}
	}

	// Iterate the rows and aggregate the result.
	ZEND_HASH_FOREACH_VAL(ht, row) {
		if (Z_TYPE_P(row) != IS_ARRAY) {
			php_error_docref(NULL, E_USER_ERROR, "input row is not an array.");
			return -1;
		}

		for (agt_idx = 0; agt_idx < agts_cnt ; agt_idx++) {
			current_agt = &agts[agt_idx];

			// get the carried value, and then use aggregator to reduce the values.
			current_val = DAO_ARR_HASH_FIND(Z_ARRVAL_P(row), current_agt->num_selector, current_agt->selector);

			if (current_val == NULL) {
				continue;
			}

			carry_val = DAO_ARR_HASH_FIND(result_ht, current_agt->num_alias, current_agt->alias);

			if (current_agt->is_callable) {
				zval retval;
				zval args[2];

				if (carry_val == NULL) {
					zval tmp;
					ZVAL_LONG(&tmp, 0);
					carry_val = DAO_ARR_HASH_ADD_NEW(result_ht, current_agt->num_alias, current_agt->alias, &tmp);
				}

				ZVAL_COPY_VALUE(&args[0], carry_val);
				ZVAL_COPY_VALUE(&args[1], current_val);

				current_agt->fci.retval = &retval;
				current_agt->fci.params = args;

				zend_call_function(&current_agt->fci, &current_agt->fci_cache);
				ZVAL_COPY_VALUE(carry_val, &retval);

			} else if (Z_TYPE_P(current_agt->type) == IS_LONG) {

				switch (Z_LVAL_P(current_agt->type)) {
					case DAO_ARR_AGGR_MIN:

						if (isa_cast(current_agt, current_val) == 0) {
							continue;
						}

						if (carry_val == NULL) {
							zval tmp;
							ZVAL_DEREF(current_val);
							ZVAL_COPY(&tmp, current_val);
							carry_val = DAO_ARR_HASH_ADD_NEW(result_ht, current_agt->num_alias, current_agt->alias, &tmp);

						} else {
							switch (Z_TYPE_P(carry_val) ) {
								case IS_LONG:
									if (Z_LVAL_P(current_val) < Z_LVAL_P(carry_val)) {
										Z_LVAL_P(carry_val) = Z_LVAL_P(current_val);
									}
								break;
								case IS_DOUBLE:
									if (Z_DVAL_P(current_val) < Z_DVAL_P(carry_val)) {
										Z_DVAL_P(carry_val) = Z_DVAL_P(current_val);
									}
								break;
							}
						}
						break;
					case DAO_ARR_AGGR_MAX:
						if ((Z_TYPE_P(current_val) != IS_LONG && Z_TYPE_P(current_val) != IS_DOUBLE)) {
							continue;
						}
						if (carry_val == NULL) {
							zval tmp;
							ZVAL_DEREF(current_val);
							ZVAL_COPY(&tmp, current_val);
							carry_val = DAO_ARR_HASH_ADD_NEW(result_ht, current_agt->num_alias, current_agt->alias, &tmp);
						} else {
							switch (Z_TYPE_P(carry_val) ) {
								case IS_LONG:
									if (Z_LVAL_P(current_val) > Z_LVAL_P(carry_val)) {
										Z_LVAL_P(carry_val) = Z_LVAL_P(current_val);
									}
								break;
								case IS_DOUBLE:
									if (Z_DVAL_P(current_val) > Z_DVAL_P(carry_val)) {
										Z_DVAL_P(carry_val) = Z_DVAL_P(current_val);
									}
								break;
							}
						}
						break;
					case DAO_ARR_AGGR_AVG:
					case DAO_ARR_AGGR_SUM:
						if (isa_cast(current_agt, current_val) == 0) {
							continue;
						}
						if (carry_val == NULL) {
							zval tmp;
							ZVAL_DEREF(current_val);
							ZVAL_COPY(&tmp, current_val);
							carry_val = DAO_ARR_HASH_ADD_NEW(result_ht, current_agt->num_alias, current_agt->alias, &tmp);
						} else {
							switch (Z_TYPE_P(carry_val)) {
								case IS_LONG:
									Z_LVAL_P(carry_val) += Z_LVAL_P(current_val);
								break;
								case IS_DOUBLE:
									Z_DVAL_P(carry_val) += Z_DVAL_P(current_val);
								break;
							}
						}
						break;
					case DAO_ARR_AGGR_COUNT:
						if (carry_val == NULL) {
							zval tmp;
							ZVAL_LONG(&tmp, 1);
							carry_val = DAO_ARR_HASH_ADD_NEW(result_ht, current_agt->num_alias, current_agt->alias, &tmp);
						} else {
							Z_LVAL_P(carry_val)++;
						}
						break;
					case DAO_ARR_AGGR_LAST:
						carry_val = DAO_ARR_HASH_UPDATE(result_ht, current_agt->num_alias, current_agt->alias, current_val);
						break;
					case DAO_ARR_AGGR_FIRST:
						if (carry_val == NULL) {
							carry_val = DAO_ARR_HASH_ADD_NEW(result_ht, current_agt->num_alias, current_agt->alias, current_val);
						}
						break;
					case DAO_ARR_AGGR_GROUP:
						if (carry_val == NULL) {
							zval group;
							array_init(&group);
							add_next_index_zval(&group, current_val);
							carry_val = DAO_ARR_HASH_ADD_NEW(result_ht, current_agt->num_alias, current_agt->alias, &group);
						} else {
							add_next_index_zval(carry_val, current_val);
						}
						break;
				}
			}
		}

	} ZEND_HASH_FOREACH_END();

	for (agt_idx = 0; agt_idx < agts_cnt ; agt_idx++) {
		current_agt = &agts[agt_idx];
		if (Z_TYPE_P(current_agt->type) == IS_LONG) {
			carry_val = DAO_ARR_HASH_FIND(result_ht, current_agt->num_alias, current_agt->alias);
			if (carry_val != NULL) {
				switch (Z_LVAL_P(current_agt->type)) {
				case DAO_ARR_AGGR_AVG:
					convert_to_double(carry_val);
					Z_DVAL_P(carry_val) /= cnt;
					break;
				}
			}
		}
	}
	return 0;
}

static zend_always_inline void dao_arr_group_items(zval *groups_array, zval* rows, zend_string* field)
{
	zval *row, *category_key, *group = NULL;
	HashTable * groups_ht;

	// Allocate the group array for the current aggregation
	array_init(groups_array);
	groups_ht = Z_ARRVAL_P(groups_array);

	ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(rows), row) {
		if (Z_TYPE_P(row) == IS_ARRAY && (category_key = zend_hash_find(Z_ARRVAL_P(row), field)) != NULL) {
			if (Z_TYPE_P(category_key) != IS_LONG) {
				if (UNEXPECTED(Z_TYPE_P(category_key) != IS_STRING)) {
					convert_to_string_ex(category_key);
				}
				if ((group = zend_hash_find(groups_ht, Z_STR_P(category_key))) == NULL) {
					// Allocate a new group array
					zval new_group = {};
					array_init(&new_group);

					// Set the group array into the groups array.
					group = zend_hash_update(groups_ht, Z_STR_P(category_key), &new_group);
				}
			} else {
				if ((group = zend_hash_index_find(groups_ht, Z_LVAL_P(category_key))) == NULL) {
					// Allocate a new group array
					zval new_group = {};
					array_init(&new_group);

					// Set the group array into the groups array.
					group = zend_hash_index_update(groups_ht, Z_LVAL_P(category_key), &new_group);
				}
			}

			// Append the row into the group array
			if (Z_REFCOUNTED_P(row)) {
				Z_ADDREF_P(row);
			}
			add_next_index_zval(group, row);
		}
	} ZEND_HASH_FOREACH_END();
}

static zend_always_inline void dao_arr_group_groups(zval *groups, zval* fields) {

	zval *field, *tmp_group, *group;

	ZEND_HASH_FOREACH_VAL(HASH_OF(fields), field) {

		if (!field) {
			continue;
		}
		zval tmp_collection = {};
		array_init(&tmp_collection);

		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(groups), group) {
			zval tmp_groups = {};
			dao_arr_group_items(&tmp_groups, group, Z_STR_P(field));

			ZEND_HASH_FOREACH_VAL(Z_ARRVAL(tmp_groups), tmp_group) {
				Z_ADDREF_P(tmp_group);
				add_next_index_zval(&tmp_collection, tmp_group);
			} ZEND_HASH_FOREACH_END();
			zval_ptr_dtor(&tmp_groups);

		} ZEND_HASH_FOREACH_END();

		zval_ptr_dtor(groups);

		ZVAL_COPY_VALUE(groups, &tmp_collection);

	} ZEND_HASH_FOREACH_END();
}

/**
 * group input rows into group arrays.
 */
static zend_always_inline void dao_arr_group_rows(zval *groups, zval* rows, zval* fields) {
	array_init(groups);

	Z_ADDREF_P(rows);
	add_next_index_zval(groups, rows);
	dao_arr_group_groups(groups, fields);
}

/**
 * Aggregating
 *
 * @param array $rows
 * @param array $aggregators
 * @return array
 */
PHP_METHOD(Dao_Arr, aggr){

	zval *rows, *aggregators;
	dao_arr_aggregator *agts;
	zend_long agts_cnt;

	dao_fetch_params(0, 2, 0, &rows, &aggregators);
	DAO_SEPARATE_PARAM(rows);

	agts_cnt = zend_array_count(Z_ARRVAL_P(aggregators));
	agts = (dao_arr_aggregator *)safe_emalloc(agts_cnt, sizeof(dao_arr_aggregator), 0);

	if (dao_arr_compile_aggregators(agts, aggregators) == 0) {
		if (dao_arr_aggregate(return_value, rows, NULL, agts, agts_cnt) != 0) {
			zval_ptr_dtor(return_value);
			RETVAL_FALSE;
		}
	} else {
		RETVAL_FALSE;
	}

	efree(agts);
}

/**
 * Aggregating
 *
 * @param array $rows
 * @param array $fields
 * @param array $aggregators
 * @return array
 */
PHP_METHOD(Dao_Arr, group){

	zval *rows, *fields, *aggregators, groups = {}, *group;
	dao_arr_aggregator *agts;
	zend_long agts_cnt;

	dao_fetch_params(0, 3, 0, &rows, &fields, &aggregators);

	dao_arr_group_rows(&groups, rows, fields);

	agts_cnt = zend_array_count(Z_ARRVAL_P(aggregators));
	agts = (dao_arr_aggregator *)safe_emalloc(agts_cnt, sizeof(dao_arr_aggregator), 0);
	if (dao_arr_compile_aggregators(agts, aggregators) == 0) {
		// push folded result into return_value array.
		array_init(return_value);
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL(groups), group) {
			zval res = {};
			if (dao_arr_aggregate(&res, group, fields, agts, agts_cnt) != 0) {
				zval_ptr_dtor(return_value);
				RETVAL_FALSE;
				break;
			}
			add_next_index_zval(return_value, &res);
		} ZEND_HASH_FOREACH_END();
	} else {
		RETVAL_FALSE;
	}

	zval_ptr_dtor(&groups);
	efree(agts);
}

/**
 * Exchanges all keys with their associated values in an array
 *
 * @param array $array
 * @param string $key
 * @param string $value
 * @return array
 */
PHP_METHOD(Dao_Arr, flip){

	zval *array, *key, *value = NULL, *item;

	dao_fetch_params(0, 2, 1, &array, &key, &value);

	if (!value) {
		value = &DAO_GLOBAL(z_null);
	}

	array_init(return_value);
	ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(array), item) {
		zval k = {};
		DAO_CALL_CE_STATIC(&k, dao_arr_ce, "path", item, key);
		if (Z_TYPE(k) == IS_NULL) {
			zval_ptr_dtor(&k);
			RETURN_FALSE;
		}
		if (Z_TYPE_P(value) == IS_STRING) {
			zval v = {};
			DAO_CALL_CE_STATIC(&v, dao_arr_ce, "path", item, value);
			dao_array_update(return_value, &k, &v, 0);
		} else if (dao_is_callable(value)) {
			zval params = {}, v = {};
			array_init(&params);
			dao_array_update_long(&params, 0, item, PH_COPY);

			DAO_CALL_USER_FUNC_ARRAY(&v, value, &params);
			dao_array_update(return_value, &k, &v, 0);
			zval_ptr_dtor(&params);
		} else {
			dao_array_update(return_value, &k, item, PH_COPY);
		}
	} ZEND_HASH_FOREACH_END();
}

/**
 * Gets the postion key of an array
 *
 * @param array $array
 * @param int $pos
 * @return mixed
 */
PHP_METHOD(Dao_Arr, getHashKey){

	zval *array, *_pos = NULL;
	HashTable *target_hash;
	HashPosition pos = 0;
	int32_t idx;

	dao_fetch_params(0, 1, 1, &array, &_pos);

	if (_pos && Z_TYPE_P(_pos) == IS_LONG) {
		pos = Z_LVAL_P(_pos);
	}

	idx = pos;
	if (idx < 0) {
		idx = zend_hash_num_elements(Z_ARRVAL_P(array)) + idx;
		if (idx < 0) {
			RETURN_NULL();
		}
		pos = idx;
	}

	target_hash = Z_ARRVAL_P (array);
	zend_hash_get_current_key_zval_ex(target_hash, return_value, &pos);
}

/**
 * Gets the postion value of an array
 *
 * @param array $array
 * @param int $pos
 * @return mixed
 */
PHP_METHOD(Dao_Arr, getHashValue){

	zval *array, *_pos = NULL, *item = NULL;
	HashTable *target_hash;
	HashPosition pos = 0;
	int32_t idx;

	dao_fetch_params(0, 1, 1, &array, &_pos);

	if (_pos && Z_TYPE_P(_pos) == IS_LONG) {
		pos = Z_LVAL_P(_pos);
	}
	idx = pos;
	if (idx < 0) {
		idx = zend_hash_num_elements(Z_ARRVAL_P(array)) + idx;
		if (idx < 0) {
			RETURN_NULL();
		}
		pos = idx;
	}

	target_hash = Z_ARRVAL_P (array);
	item = zend_hash_get_current_data_ex(target_hash, &pos);
	if (item) {
		RETURN_CTOR(item);
	}
	RETURN_NULL();
}

/**
 * Sort an array
 * 
 *<code>
 * $arr = [
 * 	'2x' => ['sort' => 3],
 * 	'1x' =>['sort' => 4],
 * 	'3x' =>['sort' => 1],
 * 	'4x' =>['sort' => 2],
 * ];
 * \Dao\Arr::sort($arr, '*.sort'));
 *</code>
 * 
 * @param array $array
 * @param string|array $field
 * @param int $order
 * @param int $flag
 * @return bool
 */
PHP_METHOD(Dao_Arr, sort){

	zval *array, *field, *_order = NULL, *_flag = NULL, order = {}, flag = {}, map = {};
	int sort_order = PHP_SORT_ASC;
	int sort_type  = PHP_SORT_REGULAR;

	dao_fetch_params(1, 2, 2, &array, &field, &_order, &_flag);

	if (_order && Z_TYPE_P(_order) == IS_LONG) {
		sort_order = Z_LVAL_P(_order);
	}
	if (_flag && Z_TYPE_P(_flag) == IS_LONG) {
		sort_type = Z_LVAL_P(_flag);
	}
	ZVAL_LONG(&order, sort_order);
	ZVAL_LONG(&flag, sort_type);

	DAO_MM_CALL_CE_STATIC(&map, dao_arr_ce, "path", array, field);
	if (Z_TYPE(map) != IS_ARRAY) {
		RETURN_MM_FALSE;
	}

	//ZVAL_MAKE_REF(&map);
	DAO_MM_CALL_FUNCTION(return_value, "array_multisort", &map, &order, &flag, array);
	//ZVAL_UNREF(&map);

	RETURN_MM();
}