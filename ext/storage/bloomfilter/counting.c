
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

#include "storage/bloomfilter/counting.h"
#include "storage/exception.h"

#include "kernel/main.h"
#include "kernel/exception.h"
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"

#ifdef ZEND_ENABLE_ZVAL_LONG64

/**
 * Dao\Storage\Bloomfilter\Counting
 *
 */
zend_class_entry *dao_storage_bloomfilter_counting_ce;

PHP_METHOD(Dao_Storage_Bloomfilter_Counting, __construct);
PHP_METHOD(Dao_Storage_Bloomfilter_Counting, add);
PHP_METHOD(Dao_Storage_Bloomfilter_Counting, remove);
PHP_METHOD(Dao_Storage_Bloomfilter_Counting, check);
PHP_METHOD(Dao_Storage_Bloomfilter_Counting, save);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_bloomfilter_counting___construct, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, filename, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, seed, IS_LONG, 1)
	ZEND_ARG_TYPE_INFO(0, maxItems, IS_LONG, 1)
	ZEND_ARG_TYPE_INFO(0, falsePositive, IS_DOUBLE, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_bloomfilter_counting_add, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, value, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_bloomfilter_counting_remove, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, value, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_bloomfilter_counting_check, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, value, IS_STRING, 0)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_storage_bloomfilter_counting_method_entry[] = {
	PHP_ME(Dao_Storage_Bloomfilter_Counting, __construct, arginfo_dao_storage_bloomfilter_counting___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL|ZEND_ACC_CTOR)
	PHP_ME(Dao_Storage_Bloomfilter_Counting, add, arginfo_dao_storage_bloomfilter_counting_add, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Bloomfilter_Counting, remove, arginfo_dao_storage_bloomfilter_counting_remove, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Bloomfilter_Counting, check, arginfo_dao_storage_bloomfilter_counting_check, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

zend_object_handlers dao_storage_bloomfilter_counting_object_handlers;
zend_object* dao_storage_bloomfilter_counting_object_create_handler(zend_class_entry *ce)
{
	dao_storage_bloomfilter_counting_object *intern = ecalloc(1, sizeof(dao_storage_bloomfilter_counting_object) + zend_object_properties_size(ce));
	intern->std.ce = ce;

	zend_object_std_init(&intern->std, ce);
	object_properties_init(&intern->std, ce);
	intern->std.handlers = &dao_storage_bloomfilter_counting_object_handlers;

	return &intern->std;
}

void dao_storage_bloomfilter_counting_object_free_handler(zend_object *object)
{
	dao_storage_bloomfilter_counting_object *intern = dao_storage_bloomfilter_counting_object_from_obj(object);

	if (intern->bloomfilter) {
		free_counting_bloom(intern->bloomfilter);
	}
}

/**
 * Dao\Storage\Bloomfilter\Counting initializer
 */
DAO_INIT_CLASS(Dao_Storage_Bloomfilter_Counting){

	DAO_REGISTER_CLASS_CREATE_OBJECT(Dao\\Storage\\Bloomfilter, Counting, storage_bloomfilter_counting, dao_storage_bloomfilter_counting_method_entry, 0);

	return SUCCESS;
}

/**
 * Dao\Storage\Bloomfilter\Counting constructor
 */
PHP_METHOD(Dao_Storage_Bloomfilter_Counting, __construct){

	zval *filename, *_capacity = NULL, *_error_rate = NULL;
	dao_storage_bloomfilter_counting_object *intern;
	unsigned int capacity = 100000;
	double error_rate = 0.05;

	dao_fetch_params(0, 1, 4, &filename, &_capacity, &_error_rate);

	if (_capacity && Z_TYPE_P(_capacity) == IS_LONG && Z_LVAL_P(_capacity) > capacity) {
		capacity = Z_LVAL_P(_capacity);
	}

	if (_error_rate && Z_TYPE_P(_error_rate) == IS_LONG && Z_LVAL_P(_error_rate) > 0 &&  Z_LVAL_P(_error_rate) < error_rate) {
		error_rate = Z_LVAL_P(_error_rate);
	}

	intern = dao_storage_bloomfilter_counting_object_from_obj(Z_OBJ_P(getThis()));

	DAO_ZVAL_DUP(&intern->filename, filename);

	intern->bloomfilter = autocreate_counting_bloom_from_file(capacity, error_rate, Z_STRVAL_P(filename));
	if (!intern->bloomfilter) {
		DAO_THROW_EXCEPTION_STR(dao_storage_exception_ce, "Create counting bloom filter failed");
		return;
	}
}

/**
 * Add value
 *
 * @param string value
 * @return boolean
 */
PHP_METHOD(Dao_Storage_Bloomfilter_Counting, add){

	zval *value;
	dao_storage_bloomfilter_counting_object *intern;
	int ret;

	dao_fetch_params(0, 1, 0, &value);

	intern = dao_storage_bloomfilter_counting_object_from_obj(Z_OBJ_P(getThis()));
	ret = counting_bloom_add(intern->bloomfilter, Z_STRVAL_P(value), Z_STRLEN_P(value));
	if (ret < 0) {
		DAO_THROW_EXCEPTION_STR(dao_storage_exception_ce, "Add value error");
		return;
	}
	RETURN_TRUE;
}

/**
 * Remove value
 *
 * @param string value
 * @return boolean
 */
PHP_METHOD(Dao_Storage_Bloomfilter_Counting, remove){

	zval *value;
	dao_storage_bloomfilter_counting_object *intern;
	int ret;

	dao_fetch_params(0, 1, 0, &value);

	intern = dao_storage_bloomfilter_counting_object_from_obj(Z_OBJ_P(getThis()));
	ret = counting_bloom_remove(intern->bloomfilter, Z_STRVAL_P(value), Z_STRLEN_P(value));
	if (ret < 0) {
		DAO_THROW_EXCEPTION_STR(dao_storage_exception_ce, "Add value error");
		return;
	}
	RETURN_TRUE;
}

/**
 * Check value
 *
 * @param string value
 * @return boolean
 */
PHP_METHOD(Dao_Storage_Bloomfilter_Counting, check){

	zval *value;
	dao_storage_bloomfilter_counting_object *intern;

	dao_fetch_params(0, 1, 0, &value);

	intern = dao_storage_bloomfilter_counting_object_from_obj(Z_OBJ_P(getThis()));
	if (!counting_bloom_check(intern->bloomfilter, Z_STRVAL_P(value), Z_STRLEN_P(value))) {
		RETURN_FALSE;
	}
	RETURN_TRUE;
}

#endif