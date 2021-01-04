
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

#include "mvc/viewinterface.h"
#include "kernel/main.h"

zend_class_entry *dao_mvc_viewinterface_ce;

static const zend_function_entry dao_mvc_viewinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, setViewsDir, arginfo_dao_mvc_viewinterface_setviewsdir)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, getViewsDir, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, setLayoutsDir, arginfo_dao_mvc_viewinterface_setlayoutsdir)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, getLayoutsDir, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, setPartialsDir, arginfo_dao_mvc_viewinterface_setpartialsdir)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, getPartialsDir, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, setBasePath, arginfo_dao_mvc_viewinterface_setbasepath)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, getBasePath, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, getCurrentRenderLevel, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, getRenderLevel, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, setRenderLevel, arginfo_dao_mvc_viewinterface_setrenderlevel)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, disableLevel, arginfo_dao_mvc_viewinterface_disablelevel)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, getDisabledLevels, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, setMainView, arginfo_dao_mvc_viewinterface_setmainview)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, getMainView, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, setLayout, arginfo_dao_mvc_viewinterface_setlayout)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, getLayout, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, setTemplateBefore, arginfo_dao_mvc_viewinterface_settemplatebefore)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, cleanTemplateBefore, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, setTemplateAfter, arginfo_dao_mvc_viewinterface_settemplateafter)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, cleanTemplateAfter, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, setParamToView, arginfo_dao_mvc_viewinterface_setparamtoview)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, getParamsToView, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, setVars, arginfo_dao_mvc_viewinterface_setvars)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, setVar, arginfo_dao_mvc_viewinterface_setvar)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, getVar, arginfo_dao_mvc_viewinterface_getvar)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, setControllerName, arginfo_dao_mvc_viewinterface_setcontrollername)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, getControllerName, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, setActionName, arginfo_dao_mvc_viewinterface_setactionname)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, getActionName, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, setParams, arginfo_dao_mvc_viewinterface_setparams)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, getParams, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, start, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, registerEngines, arginfo_dao_mvc_viewinterface_registerengines)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, getRegisteredEngines, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, getEngines, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, exists, arginfo_dao_mvc_viewinterface_exists)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, render, arginfo_dao_mvc_viewinterface_render)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, pick, arginfo_dao_mvc_viewinterface_pick)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, partial, arginfo_dao_mvc_viewinterface_partial)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, getRender, arginfo_dao_mvc_viewinterface_getrender)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, finish, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, isCaching, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, getCache, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, cache, arginfo_dao_mvc_viewinterface_cache)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, setContent, arginfo_dao_mvc_viewinterface_setcontent)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, getContent, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, startSection, arginfo_dao_mvc_viewinterface_startsection)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, stopSection, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, section, arginfo_dao_mvc_viewinterface_section)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, getActiveRenderPath, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, disable, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, enable, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, isDisabled, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, enableNamespaceView, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, disableNamespaceView, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, enableLowerCase, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, disableLowerCase, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, setConverter, arginfo_dao_mvc_viewinterface_setconverter)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, getConverter, arginfo_dao_mvc_viewinterface_getconverter)
	PHP_ABSTRACT_ME(Dao_Mvc_ViewInterface, reset, arginfo_empty)
	PHP_FE_END
};

/**
 * Dao\Mvc\ViewInterface initializer
 */
DAO_INIT_CLASS(Dao_Mvc_ViewInterface){

	DAO_REGISTER_INTERFACE(Dao\\Mvc, ViewInterface, mvc_viewinterface, dao_mvc_viewinterface_method_entry);

	return SUCCESS;
}

/**
 * Sets views directory. Depending of your platform, always add a trailing slash or backslash
 *
 * @param string $viewsDir
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, setViewsDir);

/**
 * Gets views directory
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, getViewsDir);

/**
 * Sets the layouts sub-directory. Must be a directory under the views directory. Depending of your platform, always add a trailing slash or backslash
 *
 * @param string $layoutsDir
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, setLayoutsDir);

/**
 * Gets the current layouts sub-directory
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, getLayoutsDir);

/**
 * Sets a partials sub-directory. Must be a directory under the views directory. Depending of your platform, always add a trailing slash or backslash
 *
 * @param string $partialsDir
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, setPartialsDir);

/**
 * Gets the current partials sub-directory
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, getPartialsDir);

/**
 * Sets base path. Depending of your platform, always add a trailing slash or backslash
 *
 * @param string $basePath
 * @return Dao\Mvc\ViewInterface
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, setBasePath);

/**
 * Gets base path
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, getBasePath);

/**
 * Gets the current render level
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, getCurrentRenderLevel);

/**
 * Gets the render level for the view
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, getRenderLevel);

/**
 * Sets the render level for the view
 *
 * @param string $level
 * @return Dao\Mvc\ViewInterface
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, setRenderLevel);

/**
 * Sets the render level for the view
 *
 * @param string $level
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, disableLevel);

/**
 * Disables a specific level of rendering
 *
 * @param int|array $level
 * @return Dao\Mvc\ViewInterface
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, setMainView);

/**
 * Returns the name of the main view
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, getMainView);

/**
 * Change the layout to be used instead of using the name of the latest controller name
 *
 * @param string $layout
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, setLayout);

/**
 * Returns the name of the main view
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, getLayout);

/**
 * Appends template before controller layout
 *
 * @param string|array $templateBefore
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, setTemplateBefore);

/**
 * Resets any template before layouts
 *
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, cleanTemplateBefore);

/**
 * Appends template after controller layout
 *
 * @param string|array $templateAfter
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, setTemplateAfter);

/**
 * Resets any template before layouts
 *
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, cleanTemplateAfter);

/**
 * Adds parameters to views (alias of setVar)
 *
 * @param string $key
 * @param mixed $value
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, setParamToView);

/**
 * Returns parameters to views
 *
 * @return array
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, getParamsToView);

/**
 * Set all the render params
 *
 * @param array $params
 * @param boolean $merge
 * @return Dao\Mvc\ViewInterface
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, setVars);

/**
 * Adds parameters to views
 *
 * @param string $key
 * @param mixed $value
 * @return Dao\Mvc\ViewInterface
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, setVar);

/**
 * Returns a parameter previously set in the view
 *
 * @param string $key
 * @return mixed
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, getVar);

/**
 * Sets the controller name to be view
 *
 * @param string $controllerName
 * @return Dao\Mvc\ViewInterface
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, setControllerName);

/**
 * Gets the name of the controller rendered
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, getControllerName);

/**
 * Sets the action name to be view
 *
 * @param string $actionName
 * @return Dao\Mvc\ViewInterface
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, setActionName);

/**
 * Gets the name of the action rendered
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, getActionName);

/**
 * Sets the extra parameters to be view
 *
 * @param array $params
 * @return Dao\Mvc\ViewInterface
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, setParams);

/**
 * Gets extra parameters of the action rendered
 *
 * @return array
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, getParams);

/**
 * Starts rendering process enabling the output buffering
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, start);

/**
 * Register templating engines
 *
 * @param array $engines
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, registerEngines);

/**
 * Returns the registered templating engines
 *
 * @return array
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, getRegisterEngines);

/**
 * Returns the registered templating engines, if none is registered it will use Dao\Mvc\View\Engine\Php
 *
 * @return array
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, getEngines);

/**
 * Checks whether a view file exists
 *
 * @param string $view
 * @param boolean $absolutePath
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, exists);

/**
 * Executes render process from dispatching data
 *
 * @param string $controllerName
 * @param string $actionName
 * @param array $params
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, render);

/**
 * Choose a view different to render than last-controller/last-action
 *
 * @param string $renderView
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, pick);

/**
 * Renders a partial view
 *
 * @param string $partialPath
 * @return string
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, partial);

/**
 * Perform the automatic rendering returning the output as a string
 *
 * @param string $controllerName
 * @param string $actionName
 * @param array $params
 * @param mixed $configCallback
 * @return string
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, getRender);

/**
 * Finishes the render process by stopping the output buffering
 *
 * @return Dao\Mvc\ViewInterface
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, finish);

/**
 * Check if the component is currently caching the output content
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, isCaching);

/**
 * Returns the cache instance used to cache
 *
 * @return Dao\Cache\BackendInterface
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, getCache);

/**
 * Cache the actual view render to certain level
 *
 * @param boolean|array $options
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, cache);

/**
 * Externally sets the view content
 *
 * @param string $content
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, setContent);

/**
 * Returns cached ouput from another view stage
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, getContent);

/**
 * Start a new section block
 *
 * @param string $name
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, startSection);

/**
 * Stop the current section block
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, stopSection);

/**
 * Returns the content for a section block
 *
 * @param  string $name Section name
 * @param  string $default Default section content
 * @return string
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, section);

/**
 * Returns the path of the view that is currently rendered
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, getActiveRenderPath);

/**
 * Disables the auto-rendering process
 *
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, disable);

/**
 * Enables the auto-rendering process
 *
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, enable);

/**
 * Whether automatic rendering is enabled
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, isDisabled);

/**
 * Enables namespace view render
 *
 * @return Dao\Mvc\ViewInterface
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, enableNamespaceView);

/**
 * Disables namespace view render
 *
 * @return Dao\Mvc\ViewInterface
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, disableNamespaceView);

/**
 * Enables to lower case view path
 *
 * @return Dao\Mvc\ViewInterface
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, enableLowerCase);

/**
 * Disables to lower case view path
 *
 * @return Dao\Mvc\ViewInterface
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, disableLowerCase);

/**
 * Adds a converter
 *
 * @param string $name
 * @param callable $converter
 * @return Dao\Mvc\ViewInterface
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, setConverter);

/**
 * Returns the router converter
 *
 * @return callable|null
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, getConverter);

/**
 * Resets the view component to its factory default values
 *
 */
DAO_DOC_METHOD(Dao_Mvc_ViewInterface, reset);

