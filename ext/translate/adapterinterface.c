
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

#include "translate/adapterinterface.h"

#include "kernel/main.h"

zend_class_entry *dao_translate_adapterinterface_ce;

static const zend_function_entry dao_translate_adapterinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Translate_AdapterInterface, query, arginfo_dao_translate_adapterinterface_query)
	PHP_ABSTRACT_ME(Dao_Translate_AdapterInterface, exists, arginfo_dao_translate_adapterinterface_exists)
	PHP_FE_END
};

/**
 * Dao\Translate\AdapterInterface initializer
 */
DAO_INIT_CLASS(Dao_Translate_AdapterInterface){

	DAO_REGISTER_INTERFACE(Dao\\Translate, AdapterInterface, translate_adapterinterface, dao_translate_adapterinterface_method_entry);

	return SUCCESS;
}

/**
 * Returns the translation string of the given key
 *
 * @param string $translateKey
 * @param array $placeholders
 * @return string
 */
DAO_DOC_METHOD(Dao_Translate_AdapterInterface, _);

/**
 * Returns the translation related to the given key
 *
 * @param string $index
 * @param array $placeholders
 * @return string
 */
DAO_DOC_METHOD(Dao_Translate_AdapterInterface, query);

/**
 * Check whether is defined a translation key in the internal array
 *
 * @param string $index
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Translate_AdapterInterface, exists);
