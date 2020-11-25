
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

#include "mvc/model/resultset.h"
#include "mvc/model/resultsetinterface.h"
#include "mvc/model/exception.h"
#include "di/injectable.h"

#ifdef DAO_USE_PHP_JSON
#include <ext/json/php_json.h>
#endif

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/operators.h"
#include "kernel/fcall.h"
#include "kernel/array.h"
#include "kernel/exception.h"

/**
 * Dao\Mvc\Model\Resultset
 *
 * This component allows to Dao\Mvc\Model returns large resulsets with the minimum memory consumption
 * Resulsets can be traversed using a standard foreach or a while statement. If a resultset is serialized
 * it will dump all the rows into a big array. Then unserialize will retrieve the rows as they were before
 * serializing.
 *
 * <code>
 *
 * //Using a standard foreach
 * $robots = Robots::find(array("type='virtual'", "order" => "name"));
 * foreach ($robots as $robot) {
 *  echo $robot->name, "\n";
 * }
 *
 * //Using a while
 * $robots = Robots::find(array("type='virtual'", "order" => "name"));
 * $robots->rewind();
 * while ($robots->valid()) {
 *  $robot = $robots->current();
 *  echo $robot->name, "\n";
 *  $robots->next();
 * }
 * </code>
 *
 */
zend_class_entry *dao_mvc_model_resultset_ce;

PHP_METHOD(Dao_Mvc_Model_Resultset, next);
PHP_METHOD(Dao_Mvc_Model_Resultset, key);
PHP_METHOD(Dao_Mvc_Model_Resultset, rewind);
PHP_METHOD(Dao_Mvc_Model_Resultset, seek);
PHP_METHOD(Dao_Mvc_Model_Resultset, count);
PHP_METHOD(Dao_Mvc_Model_Resultset, offsetExists);
PHP_METHOD(Dao_Mvc_Model_Resultset, offsetGet);
PHP_METHOD(Dao_Mvc_Model_Resultset, offsetSet);
PHP_METHOD(Dao_Mvc_Model_Resultset, offsetUnset);
PHP_METHOD(Dao_Mvc_Model_Resultset, getType);
PHP_METHOD(Dao_Mvc_Model_Resultset, getFirst);
PHP_METHOD(Dao_Mvc_Model_Resultset, getLast);
PHP_METHOD(Dao_Mvc_Model_Resultset, setIsFresh);
PHP_METHOD(Dao_Mvc_Model_Resultset, isFresh);
PHP_METHOD(Dao_Mvc_Model_Resultset, setHydrateMode);
PHP_METHOD(Dao_Mvc_Model_Resultset, getHydrateMode);
PHP_METHOD(Dao_Mvc_Model_Resultset, getCache);
PHP_METHOD(Dao_Mvc_Model_Resultset, current);
PHP_METHOD(Dao_Mvc_Model_Resultset, getMessages);
PHP_METHOD(Dao_Mvc_Model_Resultset, delete);
PHP_METHOD(Dao_Mvc_Model_Resultset, filter);
PHP_METHOD(Dao_Mvc_Model_Resultset, update);
PHP_METHOD(Dao_Mvc_Model_Resultset, jsonSerialize);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_resultset_sethydratemode, 0, 0, 1)
	ZEND_ARG_INFO(0, hydrateMode)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_resultset_delete, 0, 0, 0)
	ZEND_ARG_INFO(0, conditionCallback)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_resultset_filter, 0, 0, 1)
	ZEND_ARG_INFO(0, filter)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_resultset_update, 0, 0, 1)
	ZEND_ARG_INFO(0, data)
	ZEND_ARG_INFO(0, conditionCallback)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_mvc_model_resultset_method_entry[] = {
	PHP_ME(Dao_Mvc_Model_Resultset, next, arginfo_iterator_next, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Resultset, key, arginfo_iterator_key, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Resultset, rewind, arginfo_iterator_rewind, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Resultset, seek, arginfo_seekableiterator_seek, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Resultset, count, arginfo_countable_count, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Resultset, offsetExists, arginfo_arrayaccess_offsetexists, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Resultset, offsetGet, arginfo_arrayaccess_offsetget, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Resultset, offsetSet, arginfo_arrayaccess_offsetset, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Resultset, offsetUnset, arginfo_arrayaccess_offsetunset, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Resultset, getType, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Resultset, getFirst, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Resultset, getLast, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Resultset, setIsFresh, arginfo_dao_mvc_model_resultsetinterface_setisfresh, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Resultset, isFresh, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Resultset, setHydrateMode, arginfo_dao_mvc_model_resultset_sethydratemode, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Resultset, getHydrateMode, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Resultset, getCache, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Resultset, current, arginfo_iterator_current, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Resultset, getMessages, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Resultset, delete, arginfo_dao_mvc_model_resultset_delete, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Resultset, filter, arginfo_dao_mvc_model_resultset_filter, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Resultset, update, arginfo_dao_mvc_model_resultset_update, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Resultset, jsonSerialize, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Mvc\Model\Resultset initializer
 */
DAO_INIT_CLASS(Dao_Mvc_Model_Resultset){

	DAO_REGISTER_CLASS_EX(Dao\\Mvc\\Model, Resultset, mvc_model_resultset, dao_di_injectable_ce, dao_mvc_model_resultset_method_entry, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	zend_declare_property_long(dao_mvc_model_resultset_ce, SL("_type"), 0, ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_model_resultset_ce, SL("_result"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_model_resultset_ce, SL("_cache"), ZEND_ACC_PROTECTED);
	zend_declare_property_bool(dao_mvc_model_resultset_ce, SL("_isFresh"), 1, ZEND_ACC_PROTECTED);
	zend_declare_property_long(dao_mvc_model_resultset_ce, SL("_pointer"), -1, ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_model_resultset_ce, SL("_count"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_model_resultset_ce, SL("_activeRow"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_model_resultset_ce, SL("_rows"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_model_resultset_ce, SL("_errorMessages"), ZEND_ACC_PROTECTED);
	zend_declare_property_long(dao_mvc_model_resultset_ce, SL("_hydrateMode"), 0, ZEND_ACC_PROTECTED);

	zend_declare_class_constant_long(dao_mvc_model_resultset_ce, SL("TYPE_RESULT_FULL"),    DAO_MVC_MODEL_RESULTSET_TYPE_FULL);
	zend_declare_class_constant_long(dao_mvc_model_resultset_ce, SL("TYPE_RESULT_PARTIAL"), DAO_MVC_MODEL_RESULTSET_TYPE_PARTIAL);
	zend_declare_class_constant_long(dao_mvc_model_resultset_ce, SL("HYDRATE_RECORDS"), 0);
	zend_declare_class_constant_long(dao_mvc_model_resultset_ce, SL("HYDRATE_OBJECTS"), 2);
	zend_declare_class_constant_long(dao_mvc_model_resultset_ce, SL("HYDRATE_ARRAYS"), 1);

	zend_class_implements(dao_mvc_model_resultset_ce, 6, dao_mvc_model_resultsetinterface_ce, zend_ce_iterator, spl_ce_SeekableIterator, spl_ce_Countable, zend_ce_arrayaccess, zend_ce_serializable);

#ifdef DAO_USE_PHP_JSON
	zend_class_implements(dao_mvc_model_resultset_ce, 1, php_json_serializable_ce);
#endif
	return SUCCESS;
}

/**
 * Moves cursor to next row in the resultset
 *
 */
PHP_METHOD(Dao_Mvc_Model_Resultset, next){


	dao_property_incr(getThis(), SL("_pointer"));
	DAO_CALL_METHOD(NULL, getThis(), "valid");

}

/**
 * Gets pointer number of active row in the resultset
 *
 * @return int
 */
PHP_METHOD(Dao_Mvc_Model_Resultset, key){


	RETURN_MEMBER(getThis(), "_pointer");
}

/**
 * Rewinds resultset to its beginning
 *
 */
PHP_METHOD(Dao_Mvc_Model_Resultset, rewind){

	dao_update_property(getThis(), SL("_pointer"), &DAO_GLOBAL(z_zero));
}

/**
 * Changes internal pointer to a specific position in the resultset
 *
 * @param int $position
 */
PHP_METHOD(Dao_Mvc_Model_Resultset, seek){

	zval *position, type = {}, pointer = {}, is_different = {};

	dao_fetch_params(0, 1, 0, &position);

	dao_read_property(&pointer, getThis(), SL("_pointer"), PH_NOISY|PH_READONLY);

	/**
	 * We only seek the records if the current position is diferent than the passed one
	 */
	is_not_equal_function(&is_different, &pointer, position);

	if (DAO_IS_TRUE(&is_different)) {
		convert_to_long(position);
		dao_read_property(&type, getThis(), SL("_type"), PH_NOISY|PH_READONLY);

		if (zend_is_true(&type)) {
			if (DAO_GT(position, &pointer) && !dao_property_array_isset(getThis(), SL("_rows"), position, PH_READONLY)) {
				zend_long pos = Z_LVAL_P(position)- Z_LVAL(pointer);
				while(pos-->0) {
					DAO_CALL_METHOD(NULL, getThis(), "next");
				}
			}
		}
		dao_update_property(getThis(), SL("_pointer"), position);
	}
}

/**
 * Counts how many rows are in the resultset
 *
 * @return int
 */
PHP_METHOD(Dao_Mvc_Model_Resultset, count){

	zval count = {}, type = {}, result = {}, number_rows = {}, rows = {};

	dao_read_property(&count, getThis(), SL("_count"), PH_READONLY);

	/**
	 * We only calculate the row number is it wasn't calculated before
	 */
	if (Z_TYPE(count) == IS_NULL) {
		ZVAL_LONG(&count, 0);

		dao_read_property(&type, getThis(), SL("_type"), PH_NOISY|PH_READONLY);
		if (zend_is_true(&type)) {
			/**
			 * Here, the resultset act as a result that is fetched one by one
			 */
			dao_read_property(&result, getThis(), SL("_result"), PH_NOISY|PH_READONLY);
			if (DAO_IS_NOT_FALSE(&result)) {
				DAO_CALL_METHOD(&number_rows, &result, "numrows");
				ZVAL_LONG(&count, dao_get_intval(&number_rows));
				zval_ptr_dtor(&number_rows);
			}
		} else {
			/**
			 * Here, the resultset act as an array
			 */
			dao_read_property(&rows, getThis(), SL("_rows"), PH_READONLY);
			if (Z_TYPE(rows) == IS_NULL) {
				dao_read_property(&result, getThis(), SL("_result"), PH_NOISY|PH_READONLY);
				if (Z_TYPE(result) == IS_OBJECT) {
					DAO_CALL_METHOD(&rows, &result, "fetchall");
					dao_update_property(getThis(), SL("_rows"), &rows);
					zval_ptr_dtor(&rows);
				}
			}

			dao_fast_count(&count, &rows);
		}

		dao_update_property(getThis(), SL("_count"), &count);
	}

	RETURN_CTOR(&count);
}

/**
 * Checks whether offset exists in the resultset
 *
 * @param int $index
 * @return boolean
 */
PHP_METHOD(Dao_Mvc_Model_Resultset, offsetExists){

	zval *index, count = {};

	dao_fetch_params(0, 1, 0, &index);

	DAO_CALL_METHOD(&count, getThis(), "count");
	is_smaller_function(return_value, index, &count);
	zval_ptr_dtor(&count);
}

/**
 * Gets row in a specific position of the resultset
 *
 * @param int $index
 * @return Dao\Mvc\ModelInterface
 */
PHP_METHOD(Dao_Mvc_Model_Resultset, offsetGet){

	zval *index, count = {}, pointer = {}, valid = {};

	dao_fetch_params(1, 1, 0, &index);

	DAO_MM_CALL_METHOD(&count, getThis(), "count");
	DAO_MM_ADD_ENTRY(&count);
	if (DAO_LT(index, &count)) {
		/**
		 * Check if the last record returned is the current requested
		 */
		dao_read_property(&pointer, getThis(), SL("_pointer"), PH_NOISY|PH_READONLY);
		if (DAO_IS_EQUAL(&pointer, index)) {
			DAO_MM_RETURN_CALL_METHOD(getThis(), "current");
			RETURN_MM();
		}

		/**
		 * Move the cursor to the specific position
		 */
		DAO_MM_CALL_METHOD(NULL, getThis(), "seek", index);

		/**
		 * Check if the last record returned is the requested
		 */
		DAO_MM_CALL_METHOD(&valid, getThis(), "valid");
		if (DAO_IS_NOT_FALSE(&valid)) {
			DAO_MM_RETURN_CALL_METHOD(getThis(), "current");
			RETURN_MM();
		}
		RETURN_MM_FALSE;
	}
	DAO_MM_THROW_EXCEPTION_STR(dao_mvc_model_exception_ce, "The index does not exist in the cursor");
	return;
}

/**
 * Resultsets cannot be changed. It has only been implemented to meet the definition of the ArrayAccess interface
 *
 * @param int $index
 * @param Dao\Mvc\ModelInterface $value
 */
PHP_METHOD(Dao_Mvc_Model_Resultset, offsetSet){

	zval *index, *value;

	dao_fetch_params(0, 2, 0, &index, &value);

	DAO_THROW_EXCEPTION_STR(dao_mvc_model_exception_ce, "Cursor is an immutable ArrayAccess object");
}

/**
 * Resultsets cannot be changed. It has only been implemented to meet the definition of the ArrayAccess interface
 *
 * @param int $offset
 */
PHP_METHOD(Dao_Mvc_Model_Resultset, offsetUnset){

	zval *offset;

	dao_fetch_params(0, 1, 0, &offset);

	DAO_THROW_EXCEPTION_STR(dao_mvc_model_exception_ce, "Cursor is an immutable ArrayAccess object");
}

/**
 * Returns the internal type of data retrieval that the resultset is using
 *
 * @return int
 */
PHP_METHOD(Dao_Mvc_Model_Resultset, getType){


	RETURN_MEMBER(getThis(), "_type");
}

/**
 * Get first row in the resultset
 *
 * @return Dao\Mvc\ModelInterface
 */
PHP_METHOD(Dao_Mvc_Model_Resultset, getFirst){

	zval pointer = {}, valid = {};

	/**
	 * Check if the last record returned is the current requested
	 */
	dao_read_property(&pointer, getThis(), SL("_pointer"), PH_NOISY|PH_READONLY);
	if (DAO_IS_LONG(&pointer, 0)) {
		DAO_RETURN_CALL_METHOD(getThis(), "current");
		return;
	}

	/**
	 * Otherwise re-execute the statement
	 */
	DAO_CALL_METHOD(NULL, getThis(), "rewind");

	DAO_CALL_METHOD(&valid, getThis(), "valid");
	if (DAO_IS_NOT_FALSE(&valid)) {
		DAO_CALL_METHOD(return_value, getThis(), "current");
	} else {
		RETVAL_FALSE;
	}
	zval_ptr_dtor(&valid);
}

/**
 * Get last row in the resultset
 *
 * @return Dao\Mvc\ModelInterface
 */
PHP_METHOD(Dao_Mvc_Model_Resultset, getLast){

	zval count = {}, pre_count = {}, valid = {};

	DAO_CALL_METHOD(&count, getThis(), "count");

	dao_sub_function(&pre_count, &count, &DAO_GLOBAL(z_one));
	DAO_CALL_METHOD(NULL, getThis(), "seek", &pre_count);

	DAO_CALL_METHOD(&valid, getThis(), "valid");
	if (DAO_IS_NOT_FALSE(&valid)) {
		DAO_RETURN_CALL_METHOD(getThis(), "current");
		return;
	}

	RETURN_FALSE;
}

/**
 * Set if the resultset is fresh or an old one cached
 *
 * @param boolean $isFresh
 * @return Dao\Mvc\Model\Resultset
 */
PHP_METHOD(Dao_Mvc_Model_Resultset, setIsFresh){

	zval *is_fresh;

	dao_fetch_params(0, 1, 0, &is_fresh);

	dao_update_property(getThis(), SL("_isFresh"), is_fresh);
	RETURN_THIS();
}

/**
 * Tell if the resultset if fresh or an old one cached
 *
 * @return boolean
 */
PHP_METHOD(Dao_Mvc_Model_Resultset, isFresh){


	RETURN_MEMBER(getThis(), "_isFresh");
}

/**
 * Sets the hydration mode in the resultset
 *
 * @param int $hydrateMode
 * @return Dao\Mvc\Model\Resultset
 */
PHP_METHOD(Dao_Mvc_Model_Resultset, setHydrateMode){

	zval *hydrate_mode;

	dao_fetch_params(0, 1, 0, &hydrate_mode);

	dao_update_property(getThis(), SL("_hydrateMode"), hydrate_mode);
	RETURN_THIS();
}

/**
 * Returns the current hydration mode
 *
 * @return int
 */
PHP_METHOD(Dao_Mvc_Model_Resultset, getHydrateMode){


	RETURN_MEMBER(getThis(), "_hydrateMode");
}

/**
 * Returns the associated cache for the resultset
 *
 * @return Dao\Cache\BackendInterface
 */
PHP_METHOD(Dao_Mvc_Model_Resultset, getCache){


	RETURN_MEMBER(getThis(), "_cache");
}

/**
 * Returns current row in the resultset
 *
 * @return Dao\Mvc\ModelInterface
 */
PHP_METHOD(Dao_Mvc_Model_Resultset, current){


	RETURN_MEMBER(getThis(), "_activeRow");
}

/**
 * Returns the error messages produced by a batch operation
 *
 * @return Dao\Mvc\Model\MessageInterface[]
 */
PHP_METHOD(Dao_Mvc_Model_Resultset, getMessages){


	RETURN_MEMBER(getThis(), "_errorMessages");
}

/**
 * Deletes every record in the resultset
 *
 * @param Closure $conditionCallback
 * @return boolean
 */
PHP_METHOD(Dao_Mvc_Model_Resultset, delete){

	zval *condition_callback = NULL, transaction = {}, connection = {};

	dao_fetch_params(1, 0, 1, &condition_callback);

	if (!condition_callback) {
		condition_callback = &DAO_GLOBAL(z_null);
	}

	ZVAL_FALSE(&transaction);
	DAO_MM_CALL_METHOD(NULL, getThis(), "rewind");

	while (1) {
		zval r0 = {}, record = {}, parameters = {}, status = {}, messages = {};

		DAO_MM_CALL_METHOD(&r0, getThis(), "valid");
		DAO_MM_ADD_ENTRY(&r0);
		if (zend_is_true(&r0)) {
		} else {
			break;
		}

		DAO_MM_CALL_METHOD(&record, getThis(), "current");
		DAO_MM_ADD_ENTRY(&record);
		if (DAO_IS_FALSE(&transaction)) {

			/**
			 * We only can delete resultsets whose every element is a complete object
			 */
			if (dao_method_exists_ex(&record, SL("getwriteconnection")) == FAILURE) {
				DAO_MM_THROW_EXCEPTION_STR(dao_mvc_model_exception_ce, "The returned record is not valid");
				return;
			}

			if (!zend_is_true(&connection)) {
				DAO_MM_CALL_METHOD(&connection, &record, "getwriteconnection");
				DAO_MM_ADD_ENTRY(&connection);
			}
			DAO_MM_CALL_METHOD(NULL, &connection, "begin");

			ZVAL_TRUE(&transaction);
		}

		/**
		 * Perform additional validations
		 */
		if (Z_TYPE_P(condition_callback) == IS_OBJECT) {
			array_init_size(&parameters, 1);
			dao_array_append(&parameters, &record, PH_COPY);
			DAO_MM_ADD_ENTRY(&parameters);
			DAO_MM_CALL_USER_FUNC_ARRAY(&status, condition_callback, &parameters);
			DAO_MM_ADD_ENTRY(&status);
			if (DAO_IS_FALSE(&status)) {
				continue;
			}
		}

		/**
		 * Try to delete the record
		 */
		DAO_MM_CALL_METHOD(&status, &record, "delete");
		DAO_MM_ADD_ENTRY(&status);

		if (!zend_is_true(&status)) {
			/**
			 * Get the messages from the record that produce the error
			 */
			DAO_MM_CALL_METHOD(&messages, &record, "getmessages");
			dao_update_property(getThis(), SL("_errorMessages"), &messages);
			zval_ptr_dtor(&messages);

			/**
			 * Rollback the transaction
			 */
			DAO_MM_CALL_METHOD(NULL, &connection, "rollback");

			ZVAL_BOOL(&transaction, 0);
			break;
		}

		DAO_MM_CALL_METHOD(NULL, getThis(), "next");
	}

	/**
	 * Commit the transaction
	 */
	if (DAO_IS_TRUE(&transaction)) {
		DAO_MM_CALL_METHOD(NULL, &connection, "commit");
	}

	RETURN_MM_TRUE;
}

/**
 * Filters a resultset returning only those the developer requires
 *
 *<code>
 * $filtered = $robots->filter(function($robot){
 *		if ($robot->id < 3) {
 *			return $robot;
 *		}
 *	});
 *</code>
 *
 * @param callback $filter
 * @return Dao\Mvc\Model[]
 */
PHP_METHOD(Dao_Mvc_Model_Resultset, filter){

	zval *filter;

	dao_fetch_params(1, 1, 0, &filter);

	DAO_MM_CALL_METHOD(NULL, getThis(), "rewind");

	array_init(return_value);

	while (1) {
		zval r0 = {}, record = {}, parameters = {}, processed_record = {};

		DAO_MM_CALL_METHOD(&r0, getThis(), "valid");
		if (!zend_is_true(&r0)) {
			break;
		}

		DAO_MM_CALL_METHOD(&record, getThis(), "current");

		array_init(&parameters);
		DAO_MM_ADD_ENTRY(&parameters);
		dao_array_update_long(&parameters, 0, &record, 0);

		DAO_MM_CALL_USER_FUNC_ARRAY(&processed_record, filter, &parameters);
		DAO_MM_ADD_ENTRY(&processed_record);
		DAO_MM_CALL_METHOD(NULL, getThis(), "next");
		
		/**
		 * Only add processed records to 'records' if the returned value is an array/object
		 */
		if (Z_TYPE(processed_record) == IS_OBJECT || Z_TYPE(processed_record) == IS_ARRAY) {
			dao_array_append(return_value, &processed_record, PH_COPY);
		}
	}

	RETURN_MM();
}

/**
 * Updates every record in the resultset
 *
 * @param array $data
 * @param Closure $conditionCallback
 * @return boolean
 */
PHP_METHOD(Dao_Mvc_Model_Resultset, update){

	zval *data, *condition_callback = NULL, transaction = {}, connection = {};

	dao_fetch_params(0, 1, 1, &data, &condition_callback);

	if (!condition_callback) {
		condition_callback = &DAO_GLOBAL(z_null);
	}

	ZVAL_FALSE(&transaction);
	DAO_CALL_METHOD(NULL, getThis(), "rewind");

	while (1) {
		zval r0 = {}, record = {}, parameters = {}, status = {}, messages = {};

		DAO_CALL_METHOD(&r0, getThis(), "valid");
		if (!zend_is_true(&r0)) {
			break;
		}

		DAO_CALL_METHOD(&record, getThis(), "current");
		if (DAO_IS_FALSE(&transaction)) {

			/**
			 * We only can update resultsets whose every element is a complete object
			 */
			if (dao_method_exists_ex(&record, SL("getwriteconnection")) == FAILURE) {
				DAO_THROW_EXCEPTION_STR(dao_mvc_model_exception_ce, "The returned record is not valid");
				zval_ptr_dtor(&record);
				return;
			}

			DAO_CALL_METHOD(&connection, &record, "getwriteconnection");
			DAO_CALL_METHOD(NULL, &connection, "begin");

			ZVAL_TRUE(&transaction);
		}

		/**
		 * Perform additional validations
		 */
		if (Z_TYPE_P(condition_callback) == IS_OBJECT) {
			array_init_size(&parameters, 1);
			dao_array_append(&parameters, &record, PH_COPY);

			DAO_CALL_USER_FUNC_ARRAY(&status, condition_callback, &parameters);
			zval_ptr_dtor(&parameters);
			if (DAO_IS_FALSE(&status)) {
				zval_ptr_dtor(&record);
				continue;
			}
		}

		/**
		 * Try to update the record
		 */
		DAO_CALL_METHOD(&status, &record, "save", data);
		if (!zend_is_true(&status)) {
			/**
			 * Get the messages from the record that produce the error
			 */
			DAO_CALL_METHOD(&messages, &record, "getmessages");
			dao_update_property(getThis(), SL("_errorMessages"), &messages);
			zval_ptr_dtor(&messages);

			/**
			 * Rollback the transaction
			 */
			DAO_CALL_METHOD(NULL, &connection, "rollback");

			ZVAL_FALSE(&transaction);
			zval_ptr_dtor(&record);
			break;
		}

		DAO_CALL_METHOD(NULL, getThis(), "next");
		zval_ptr_dtor(&record);
	}

	/**
	 * Commit the transaction
	 */
	if (DAO_IS_TRUE(&transaction)) {
		DAO_CALL_METHOD(NULL, &connection, "commit");
	}
	if (zend_is_true(&connection)) {
		zval_ptr_dtor(&connection);
	}

	RETURN_TRUE;
}

/**
 * Returns serialised model objects as array for json_encode.
 * Calls jsonSerialize on each object if present
 *
 *<code>
 * $robots = Robots::find();
 * echo json_encode($robots);
 *</code>
 *
 * @return array
 */
PHP_METHOD(Dao_Mvc_Model_Resultset, jsonSerialize) {

	array_init(return_value);

	DAO_CALL_METHOD(NULL, getThis(), "rewind");

	while (1) {
		zval r0, current = {}, jsondata = {};
		zend_bool status;

		DAO_CALL_METHOD(&r0, getThis(), "valid");

		if (!(zend_is_true(&r0))) {
			break;
		}
		DAO_CALL_METHOD(&current, getThis(), "current");

		status = Z_TYPE(current) == IS_OBJECT;
		if (status) {
			status = (dao_method_exists_ex(&current, SS("jsonserialize")) == SUCCESS);
		}

		if (status) {
			DAO_CALL_METHOD(&jsondata, &current, "jsonserialize");

			dao_array_append(return_value, &jsondata, 0);
			zval_ptr_dtor(&current);
		} else {
			dao_array_append(return_value, &current, 0);
		}
		DAO_CALL_METHOD(NULL, getThis(), "next");
	}
}
