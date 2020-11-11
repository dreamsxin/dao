
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

#include "mvc/model/query/exception.h"
#include "mvc/model/exception.h"
#include "kernel/main.h"

/**
 * Dao\Mvc\Model\Query\Exception
 *
 * Exceptions thrown in Dao\Mvc\Model\Query\* classes will use this class
 *
 */
zend_class_entry *dao_mvc_model_query_exception_ce;

/**
 * Dao\Mvc\Model\Query\Exception initializer
 */
DAO_INIT_CLASS(Dao_Mvc_Model_Query_Exception){

	DAO_REGISTER_CLASS_EX(Dao\\Mvc\\Model\\Query, Exception, mvc_model_query_exception, dao_mvc_model_exception_ce, NULL, 0);

	return SUCCESS;
}
