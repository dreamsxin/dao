
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

#include "mvc/controller.h"
#include "mvc/controllerinterface.h"
#include "di/injectable.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/fcall.h"

/**
 * Dao\Mvc\Controller
 *
 * Every application controller should extend this class that encapsulates all the controller functionality
 *
 * The controllers provide the “flow” between models and views. Controllers are responsible
 * for processing the incoming requests from the web browser, interrogating the models for data,
 * and passing that data on to the views for presentation.
 *
 *<code>
 *
 *class PeopleController extends \Dao\Mvc\Controller
 *{
 *
 *  //This action will be executed by default
 *  public function indexAction()
 *  {
 *
 *  }
 *
 *  public function findAction()
 *  {
 *
 *  }
 *
 *  public function saveAction()
 *  {
 *   //Forwards flow to the index action
 *   return $this->dispatcher->forward(array('controller' => 'people', 'action' => 'index'));
 *  }
 *
 *}
 *
 *</code>
 */
zend_class_entry *dao_mvc_controller_ce;

PHP_METHOD(Dao_Mvc_Controller, __construct);

static const zend_function_entry dao_mvc_controller_method_entry[] = {
	PHP_ME(Dao_Mvc_Controller, __construct, arginfo_empty, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL|ZEND_ACC_CTOR)
	PHP_FE_END
};

/**
 * Dao\Mvc\Controller initializer
 */
DAO_INIT_CLASS(Dao_Mvc_Controller){

	DAO_REGISTER_CLASS_EX(Dao\\Mvc, Controller, mvc_controller, dao_di_injectable_ce, dao_mvc_controller_method_entry, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	zend_class_implements(dao_mvc_controller_ce, 1, dao_mvc_controllerinterface_ce);
	return SUCCESS;
}

/**
 * Dao\Mvc\Controller constructor
 *
 */
PHP_METHOD(Dao_Mvc_Controller, __construct){

	if (dao_method_exists_ex(getThis(), SL("onconstruct")) == SUCCESS) {
		DAO_CALL_METHOD(NULL, getThis(), "onconstruct");
	}
}
