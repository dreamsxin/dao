
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
  +------------------------------------------------------------------------+
*/

#include "mvc/model/row.h"
#include "mvc/model/exception.h"
#include "mvc/model/resultinterface.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/hash.h"
#include "kernel/object.h"
#include "kernel/exception.h"

/**
 * Dao\Mvc\Model\Row
 *
 * This component allows Dao\Mvc\Model to return rows without an associated entity.
 * This objects implements the ArrayAccess interface to allow access the object as object->x or array[x].
 */
zend_class_entry *dao_mvc_model_row_ce;

PHP_METHOD(Dao_Mvc_Model_Row, setDirtyState);
PHP_METHOD(Dao_Mvc_Model_Row, offsetExists);
PHP_METHOD(Dao_Mvc_Model_Row, offsetGet);
PHP_METHOD(Dao_Mvc_Model_Row, offsetSet);
PHP_METHOD(Dao_Mvc_Model_Row, offsetUnset);
PHP_METHOD(Dao_Mvc_Model_Row, toArray);
PHP_METHOD(Dao_Mvc_Model_Row, count);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_row_setdirtystate, 0, 0, 1)
	ZEND_ARG_INFO(0, dirtyState)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_row_offsetexists, 0, 0, 1)
	ZEND_ARG_INFO(0, index)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_row_offsetget, 0, 0, 1)
	ZEND_ARG_INFO(0, index)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_row_offsetset, 0, 0, 2)
	ZEND_ARG_INFO(0, index)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_row_offsetunset, 0, 0, 1)
	ZEND_ARG_INFO(0, offset)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_mvc_model_row_method_entry[] = {
	PHP_ME(Dao_Mvc_Model_Row, setDirtyState, arginfo_dao_mvc_model_row_setdirtystate, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Row, offsetExists, arginfo_dao_mvc_model_row_offsetexists, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Row, offsetGet, arginfo_dao_mvc_model_row_offsetget, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Row, offsetSet, arginfo_dao_mvc_model_row_offsetset, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Row, offsetUnset, arginfo_dao_mvc_model_row_offsetunset, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Row, toArray, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Row, count, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Mvc\Model\Row initializer
 */
DAO_INIT_CLASS(Dao_Mvc_Model_Row){

	DAO_REGISTER_CLASS(Dao\\Mvc\\Model, Row, mvc_model_row, dao_mvc_model_row_method_entry, 0);

	zend_class_implements(dao_mvc_model_row_ce, 3, zend_ce_arrayaccess, spl_ce_Countable, dao_mvc_model_resultinterface_ce);

	return SUCCESS;
}

/**
 * Set the current object's state
 *
 * @param int $dirtyState
 */
PHP_METHOD(Dao_Mvc_Model_Row, setDirtyState){

	zval *dirty_state;

	dao_fetch_params(0, 1, 0, &dirty_state);

	RETURN_FALSE;
}

/**
 * Checks whether offset exists in the row
 *
 * @param int $index
 * @return boolean
 */
PHP_METHOD(Dao_Mvc_Model_Row, offsetExists){

	zval *index;

	dao_fetch_params(0, 1, 0, &index);

	if (dao_isset_property_zval(getThis(), index)) {
		RETURN_TRUE;
	}
	RETURN_FALSE;
}

/**
 * Gets a record in a specific position of the row
 *
 * @param int $index
 * @return string|\Dao\Mvc\ModelInterface
 */
PHP_METHOD(Dao_Mvc_Model_Row, offsetGet){

	zval *index, value = {};

	dao_fetch_params(0, 1, 0, &index);

	if (dao_property_isset_fetch_zval(&value, getThis(), index, PH_READONLY)) {
		RETURN_CTOR(&value);
	}

	DAO_THROW_EXCEPTION_STR(dao_mvc_model_exception_ce, "The index does not exist in the row");
}

/**
 * Rows cannot be changed. It has only been implemented to meet the definition of the ArrayAccess interface
 *
 * @param int $index
 * @param Dao\Mvc\ModelInterface $value
 */
PHP_METHOD(Dao_Mvc_Model_Row, offsetSet){

	zval *index, *value;

	dao_fetch_params(0, 2, 0, &index, &value);

	dao_update_property_zval_zval(getThis(), index, value);
}

/**
 * Rows cannot be changed. It has only been implemented to meet the definition of the ArrayAccess interface
 *
 * @param int $offset
 */
PHP_METHOD(Dao_Mvc_Model_Row, offsetUnset){

	zval *offset;

	dao_fetch_params(0, 1, 0, &offset);

	DAO_THROW_EXCEPTION_STR(dao_mvc_model_exception_ce, "The index does not exist in the row");
}

/**
 * Returns the instance as an array representation
 *
 * @return array
 */
PHP_METHOD(Dao_Mvc_Model_Row, toArray){

	HashTable *properties;

	properties = Z_OBJ_HT_P(getThis())->get_properties(getThis() ? Z_OBJ_P(getThis()) : NULL);

	if (!properties) {
		RETURN_FALSE;
	}

	array_init_size(return_value, zend_hash_num_elements(properties));
	zend_hash_copy(Z_ARRVAL_P(return_value), properties, (copy_ctor_func_t)zval_add_ref);
}

/**
 * Counts how many properties were added to the row
 *
 * @return int
 */
PHP_METHOD(Dao_Mvc_Model_Row, count)
{
	HashTable *properties;

	properties = Z_OBJ_HT_P(getThis())->get_properties(getThis() ? Z_OBJ_P(getThis()) : NULL);

	if (properties) {
		RETURN_LONG(zend_hash_num_elements(properties));
	}
	RETURN_LONG(0);
}
