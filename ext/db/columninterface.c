
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

#include "db/columninterface.h"
#include "kernel/main.h"

zend_class_entry *dao_db_columninterface_ce;

static const zend_function_entry dao_db_columninterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Db_ColumnInterface, getSchemaName, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Db_ColumnInterface, getName, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Db_ColumnInterface, getType, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Db_ColumnInterface, getSize, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Db_ColumnInterface, getScale, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Db_ColumnInterface, isUnsigned, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Db_ColumnInterface, isNotNull, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Db_ColumnInterface, isPrimary, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Db_ColumnInterface, isAutoIncrement, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Db_ColumnInterface, isNumeric, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Db_ColumnInterface, isFirst, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Db_ColumnInterface, getAfterPosition, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Db_ColumnInterface, getBindType, arginfo_empty)
	PHP_FE_END
};

/**
 * Dao\Db\ColumnInterface initializer
 */
DAO_INIT_CLASS(Dao_Db_ColumnInterface){

	DAO_REGISTER_INTERFACE(Dao\\Db, ColumnInterface, db_columninterface, dao_db_columninterface_method_entry);

	return SUCCESS;
}

/**
 * Returns schema's table related to column
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Db_ColumnInterface, getSchemaName);

/**
 * Returns column name
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Db_ColumnInterface, getName);

/**
 * Returns column type
 *
 * @return int
 */
DAO_DOC_METHOD(Dao_Db_ColumnInterface, getType);

/**
 * Returns column size
 *
 * @return int
 */
DAO_DOC_METHOD(Dao_Db_ColumnInterface, getSize);

/**
 * Returns column scale
 *
 * @return int
 */
DAO_DOC_METHOD(Dao_Db_ColumnInterface, getScale);

/**
 * Returns true if number column is unsigned
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Db_ColumnInterface, isUnsigned);

/**
 * Not null
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Db_ColumnInterface, isNotNull);

/**
 * Column is part of the primary key?
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Db_ColumnInterface, isPrimary);

/**
 * Auto-Increment
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Db_ColumnInterface, isAutoIncrement);

/**
 * Check whether column have an numeric type
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Db_ColumnInterface, isNumeric);

/**
 * Check whether column have first position in table
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Db_ColumnInterface, isFirst);

/**
 * Check whether field absolute to position in table
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Db_ColumnInterface, getAfterPosition);

/**
 * Returns the type of bind handling
 *
 * @return int
 */
DAO_DOC_METHOD(Dao_Db_ColumnInterface, getBindType);
