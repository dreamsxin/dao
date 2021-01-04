
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

#include "mvc/view/modelinterface.h"
#include "kernel/main.h"

zend_class_entry *dao_mvc_view_modelinterface_ce;

static const zend_function_entry dao_mvc_view_modelinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Mvc_View_ModelInterface, setTemplate, arginfo_dao_mvc_view_modelinterface_settemplate)
	PHP_ABSTRACT_ME(Dao_Mvc_View_ModelInterface, getTemplate, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_View_ModelInterface, setVars, arginfo_dao_mvc_view_modelinterface_setvars)
	PHP_ABSTRACT_ME(Dao_Mvc_View_ModelInterface, getVars, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_View_ModelInterface, setVar, arginfo_dao_mvc_view_modelinterface_setvar)
	PHP_ABSTRACT_ME(Dao_Mvc_View_ModelInterface, getVar, arginfo_dao_mvc_view_modelinterface_getvar)
	PHP_ABSTRACT_ME(Dao_Mvc_View_ModelInterface, addChild, arginfo_dao_mvc_view_modelinterface_addchild)
	PHP_ABSTRACT_ME(Dao_Mvc_View_ModelInterface, appendChild, arginfo_dao_mvc_view_modelinterface_appendchild)
	PHP_ABSTRACT_ME(Dao_Mvc_View_ModelInterface, getChild, arginfo_dao_mvc_view_modelinterface_getchild)
	PHP_ABSTRACT_ME(Dao_Mvc_View_ModelInterface, hasChild, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_View_ModelInterface, setCaptureTo, arginfo_dao_mvc_view_modelinterface_setcaptureto)
	PHP_ABSTRACT_ME(Dao_Mvc_View_ModelInterface, getCaptureTo, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_View_ModelInterface, setTerminal, arginfo_dao_mvc_view_modelinterface_setterminal)
	PHP_ABSTRACT_ME(Dao_Mvc_View_ModelInterface, getTerminal, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_View_ModelInterface, setAppend, arginfo_dao_mvc_view_modelinterface_setappend)
	PHP_ABSTRACT_ME(Dao_Mvc_View_ModelInterface, isAppend, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_View_ModelInterface, setView, arginfo_dao_mvc_view_modelinterface_setview)
	PHP_ABSTRACT_ME(Dao_Mvc_View_ModelInterface, getView, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_View_ModelInterface, render, arginfo_empty)
	PHP_FE_END
};

/**
 * Dao\Mvc\View\ModelInterface initializer
 */
DAO_INIT_CLASS(Dao_Mvc_View_ModelInterface){

	DAO_REGISTER_INTERFACE(Dao\\Mvc\\View, ModelInterface, mvc_view_modelinterface, dao_mvc_view_modelinterface_method_entry);

	return SUCCESS;
}

/**
 * Set the template to be used by this model
 *
 * @param  string $template
 * @return Dao\Mvc\View\ModelInterface
 */
DAO_DOC_METHOD(Dao_Mvc_View_ModelInterface, setTemplate);

/**
 * Get the template to be used by this model
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Mvc_View_ModelInterface, getTemplate);

/**
 * Set all the render params
 *
 * @param array $params
 * @param boolean $merge
 * @return Dao\Mvc\View\ModelInterface
 */
DAO_DOC_METHOD(Dao_Mvc_View_ModelInterface, setVars);

/**
 * Set a single view parameter
 *
 * @param string $key
 * @param mixed $value
 * @return Dao\Mvc\View\ModelInterface
 */
DAO_DOC_METHOD(Dao_Mvc_View_ModelInterface, setVar);

/**
 * Add a child model
 *
 * @param Dao\Mvc\View\ModelInterface $child
 * @param null|string $captureTo Optional; if specified, the "capture to" value to set on the child
 * @param null|bool $append Optional; if specified, append to child  with the same capture
 * @return Dao\Mvc\View\ModelInterface
 */
DAO_DOC_METHOD(Dao_Mvc_View_ModelInterface, addChild);

/**
 * Add a child model
 *
 * @param Dao\Mvc\View\ModelInterface $child
 * @param null|string $captureTo Optional; if specified, the "capture to" value to set on the child
 * @return Dao\Mvc\View\ModelInterface
 */
DAO_DOC_METHOD(Dao_Mvc_View_ModelInterface, appendChild);

/**
 * Return a child model or all child model
 *
 * @param null|string $captureTo
 * @return array
 */
DAO_DOC_METHOD(Dao_Mvc_View_ModelInterface, getChild);

/**
 * Does the model have any children?
 *
 * @param null|string $captureTo
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Mvc_View_ModelInterface, hasChild);

/**
 * Set the name of the variable to capture this model to, if it is a child model
 *
 * @param string $capture
 * @return Dao\Mvc\View\ModelInterface
 */
DAO_DOC_METHOD(Dao_Mvc_View_ModelInterface, setCaptureTo);

/**
 * Get the name of the variable to which to capture this model
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Mvc_View_ModelInterface, getCaptureTo);

/**
 * Set flag indicating whether or not this is considered a terminal or standalone model
 *
 * @param boolean $terminate
 * @return Dao\Mvc\View\ModelInterface
 */
DAO_DOC_METHOD(Dao_Mvc_View_ModelInterface, setTerminal);

/**
 * Is this considered a terminal or standalone model?
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Mvc_View_ModelInterface, getTerminal);

/**
 * Set flag indicating whether or not append to child  with the same capture
 *
 * @param boolean $append
 * @return Dao\Mvc\View\ModelInterface
 */
DAO_DOC_METHOD(Dao_Mvc_View_ModelInterface, setAppend);

/**
 * Is this append to child  with the same capture?
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Mvc_View_ModelInterface, isAppend);

/**
 * Set the view
 *
 * @param Dao\Mvc\ViewInterface $view
 * @return Dao\Mvc\View\ModelInterface
 */
DAO_DOC_METHOD(Dao_Mvc_View_ModelInterface, setAppend);

/**
 * Get the view
 *
 * @return Dao\Mvc\ViewInterface
 */
DAO_DOC_METHOD(Dao_Mvc_View_ModelInterface, getView);

/**
 * Renders the view
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Mvc_View_ModelInterface, render);
