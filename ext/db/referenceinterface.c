
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

#include "db/referenceinterface.h"
#include "kernel/main.h"

zend_class_entry *dao_db_referenceinterface_ce;

static const zend_function_entry dao_db_referenceinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Db_ReferenceInterface, getName, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Db_ReferenceInterface, getSchemaName, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Db_ReferenceInterface, getReferencedSchema, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Db_ReferenceInterface, getColumns, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Db_ReferenceInterface, getReferencedTable, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Db_ReferenceInterface, getReferencedColumns, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Db_ReferenceInterface, getOnDelete, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Db_ReferenceInterface, getOnUpdate, arginfo_empty)
	PHP_FE_END
};

/**
 * Dao\Db\ReferenceInterface initializer
 */
DAO_INIT_CLASS(Dao_Db_ReferenceInterface){

	DAO_REGISTER_INTERFACE(Dao\\Db, ReferenceInterface, db_referenceinterface, dao_db_referenceinterface_method_entry);

	return SUCCESS;
}

/**
 * Gets the index name
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Db_ReferenceInterface, getName);

/**
 * Gets the schema where referenced table is
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Db_ReferenceInterface, getSchemaName);

/**
 * Gets the schema where referenced table is
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Db_ReferenceInterface, getReferencedSchema);

/**
 * Gets local columns which reference is based
 *
 * @return array
 */
DAO_DOC_METHOD(Dao_Db_ReferenceInterface, getColumns);

/**
 * Gets the referenced table
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Db_ReferenceInterface, getReferencedTable);

/**
 * Gets referenced columns
 *
 * @return array
 */
DAO_DOC_METHOD(Dao_Db_ReferenceInterface, getReferencedColumns);

/**
 * Gets the referenced on delete
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Db_ReferenceInterface, getOnDelete);

/**
 * Gets the referenced on update
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Db_ReferenceInterface, getOnUpdate);
