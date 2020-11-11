
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

#include "mvc/model/query/builderinterface.h"
#include "kernel/main.h"

zend_class_entry *dao_mvc_model_query_builderinterface_ce;

static const zend_function_entry dao_mvc_model_query_builderinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Mvc_Model_Query_BuilderInterface, getType, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_Query_BuilderInterface, compile, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_Query_BuilderInterface, getPhql, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_Query_BuilderInterface, getQuery, arginfo_empty)
	PHP_FE_END
};

/**
 * Dao\Mvc\Model\Query\BuilderInterface initializer
 */
DAO_INIT_CLASS(Dao_Mvc_Model_Query_BuilderInterface){

	DAO_REGISTER_INTERFACE(Dao\\Mvc\\Model\\Query, BuilderInterface, mvc_model_query_builderinterface, dao_mvc_model_query_builderinterface_method_entry);

	return SUCCESS;
}

/**
 * Gets the type of PHQL queries
 *
 * @return int
 */
DAO_DOC_METHOD(Dao_Mvc_Model_Query_BuilderInterface, getType);

/**
 * Compile the PHQL query
 *
 * @return Dao\Mvc\Model\QueryInterface
 */
DAO_DOC_METHOD(Dao_Mvc_Model_Query_BuilderInterface, compile);

/**
 * Returns a PHQL statement built based on the builder parameters
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Mvc_Model_Query_BuilderInterface, getPhql);

/**
 * Returns the query built
 *
 * @return Dao\Mvc\Model\QueryInterface
 */
DAO_DOC_METHOD(Dao_Mvc_Model_Query_BuilderInterface, getQuery);
