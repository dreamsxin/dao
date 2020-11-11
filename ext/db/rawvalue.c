
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

#include "db/rawvalue.h"
#include "kernel/main.h"
#include "kernel/object.h"

/**
 * Dao\Db\RawValue
 *
 * This class allows to insert/update raw data without quoting or formating.
 *
 *The next example shows how to use the MySQL now() function as a field value.
 *
 *<code>
 *	$subscriber = new Subscribers();
 *	$subscriber->email = 'andres@phalconphp.com';
 *	$subscriber->created_at = new Dao\Db\RawValue('now()');
 *	$subscriber->save();
 *</code>
 */
zend_class_entry *dao_db_rawvalue_ce;

PHP_METHOD(Dao_Db_RawValue, __construct);
PHP_METHOD(Dao_Db_RawValue, getValue);
PHP_METHOD(Dao_Db_RawValue, __toString);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_db_rawvalue___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_db_rawvalue_method_entry[] = {
	PHP_ME(Dao_Db_RawValue, __construct, arginfo_dao_db_rawvalue___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Db_RawValue, getValue, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Db_RawValue, __toString, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Db\RawValue initializer
 */
DAO_INIT_CLASS(Dao_Db_RawValue){

	DAO_REGISTER_CLASS(Dao\\Db, RawValue, db_rawvalue, dao_db_rawvalue_method_entry, 0);

	zend_declare_property_null(dao_db_rawvalue_ce, SL("_value"), ZEND_ACC_PROTECTED);

	return SUCCESS;
}

/**
 * Dao\Db\RawValue constructor
 *
 * @param string $value
 */
PHP_METHOD(Dao_Db_RawValue, __construct){

	zval *value;

	dao_fetch_params(0, 1, 0, &value);
	
	dao_update_property(getThis(), SL("_value"), value);
	
}

/**
 * Returns internal raw value without quoting or formating
 *
 * @return string
 */
PHP_METHOD(Dao_Db_RawValue, getValue){


	RETURN_MEMBER(getThis(), "_value");
}

/**
 * Magic method __toString returns raw value without quoting or formating
 */
PHP_METHOD(Dao_Db_RawValue, __toString){


	RETURN_MEMBER(getThis(), "_value");
}
