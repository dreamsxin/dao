
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

#include "flash.h"
#include "flashinterface.h"
#include "flash/exception.h"
#include "di/injectable.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/exception.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "kernel/array.h"
#include "kernel/string.h"
#include "kernel/concat.h"

/**
 * Dao\Flash
 *
 * Shows HTML notifications related to different circumstances. Classes can be stylized using CSS
 *
 *<code>
 * $flash->success("The record was successfully deleted");
 * $flash->error("Cannot open the file");
 *</code>
 */
zend_class_entry *dao_flash_ce;

PHP_METHOD(Dao_Flash, __construct);
PHP_METHOD(Dao_Flash, setImplicitFlush);
PHP_METHOD(Dao_Flash, setAutomaticHtml);
PHP_METHOD(Dao_Flash, setCssClasses);
PHP_METHOD(Dao_Flash, error);
PHP_METHOD(Dao_Flash, notice);
PHP_METHOD(Dao_Flash, success);
PHP_METHOD(Dao_Flash, warning);
PHP_METHOD(Dao_Flash, outputMessage);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_flash___construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, cssClasses, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_flash_setimplicitflush, 0, 0, 1)
	ZEND_ARG_INFO(0, implicitFlush)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_flash_setautomatichtml, 0, 0, 1)
	ZEND_ARG_INFO(0, automaticHtml)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_flash_setcssclasses, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, cssClasses, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_flash_method_entry[] = {
	PHP_ME(Dao_Flash, __construct, arginfo_dao_flash___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Flash, setImplicitFlush, arginfo_dao_flash_setimplicitflush, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Flash, setAutomaticHtml, arginfo_dao_flash_setautomatichtml, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Flash, setCssClasses, arginfo_dao_flash_setcssclasses, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Flash, error, arginfo_dao_flashinterface_error, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Flash, notice, arginfo_dao_flashinterface_notice, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Flash, success, arginfo_dao_flashinterface_success, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Flash, warning, arginfo_dao_flashinterface_warning, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Flash, outputMessage, arginfo_dao_flashinterface_outputmessage, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Flash initializer
 */
DAO_INIT_CLASS(Dao_Flash){

	DAO_REGISTER_CLASS_EX(Dao, Flash, flash, dao_di_injectable_ce, dao_flash_method_entry, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	zend_declare_property_null(dao_flash_ce, SL("_cssClasses"), ZEND_ACC_PROTECTED);
	zend_declare_property_bool(dao_flash_ce, SL("_implicitFlush"), 1, ZEND_ACC_PROTECTED);
	zend_declare_property_bool(dao_flash_ce, SL("_automaticHtml"), 1, ZEND_ACC_PROTECTED);

	return SUCCESS;
}

/**
 * Dao\Flash constructor
 *
 * @param array $cssClasses
 */
PHP_METHOD(Dao_Flash, __construct){

	zval *_css_classes = NULL, css_classes = {};

	dao_fetch_params(0, 0, 1, &_css_classes);

	if (!_css_classes || Z_TYPE_P(_css_classes) == IS_NULL) {
		array_init_size(&css_classes, 4);
		dao_array_update_str_str(&css_classes, SL("error"), SL("errorMessage"), 0);
		dao_array_update_str_str(&css_classes, SL("notice"), SL("noticeMessage"), 0);
		dao_array_update_str_str(&css_classes, SL("success"), SL("successMessage"), 0);
		dao_array_update_str_str(&css_classes, SL("warning"), SL("warningMessage"), 0);
	} else {
		ZVAL_COPY_VALUE(&css_classes, _css_classes);
	}

	dao_update_property(getThis(), SL("_cssClasses"), &css_classes);
}

/**
 * Set whether the output must be implictly flushed to the output or returned as string
 *
 * @param boolean $implicitFlush
 * @return Dao\FlashInterface
 */
PHP_METHOD(Dao_Flash, setImplicitFlush){

	zval *implicit_flush;

	dao_fetch_params(0, 1, 0, &implicit_flush);

	dao_update_property(getThis(), SL("_implicitFlush"), implicit_flush);
	RETURN_THIS();
}

/**
 * Set if the output must be implictily formatted with HTML
 *
 * @param boolean $automaticHtml
 * @return Dao\FlashInterface
 */
PHP_METHOD(Dao_Flash, setAutomaticHtml){

	zval *automatic_html;

	dao_fetch_params(0, 1, 0, &automatic_html);

	dao_update_property(getThis(), SL("_automaticHtml"), automatic_html);
	RETURN_THIS();
}

/**
 * Set an array with CSS classes to format the messages
 *
 * @param array $cssClasses
 * @return Dao\FlashInterface
 */
PHP_METHOD(Dao_Flash, setCssClasses){

	zval *css_classes;

	dao_fetch_params(0, 1, 0, &css_classes);

	if (Z_TYPE_P(css_classes) == IS_ARRAY) {
		dao_update_property(getThis(), SL("_cssClasses"), css_classes);
		RETURN_THIS();
	}
	DAO_THROW_EXCEPTION_STR(dao_flash_exception_ce, "CSS classes must be an Array");
	return;
}

static void dao_flash_message_helper(INTERNAL_FUNCTION_PARAMETERS, const char *stype)
{
	zval *msg, type = {};

	dao_fetch_params(0, 1, 0, &msg);

	ZVAL_STRING(&type, stype);

	DAO_RETURN_CALL_METHOD(getThis(), "message", &type, msg);
}

/**
 * Shows a HTML error message
 *
 *<code>
 * $flash->error('This is an error');
 *</code>
 *
 * @param string $message
 * @return string
 */
PHP_METHOD(Dao_Flash, error)
{
	dao_flash_message_helper(INTERNAL_FUNCTION_PARAM_PASSTHRU, "error");
}

/**
 * Shows a HTML notice/information message
 *
 *<code>
 * $flash->notice('This is an information');
 *</code>
 *
 * @param string $message
 * @return string
 */
PHP_METHOD(Dao_Flash, notice)
{
	dao_flash_message_helper(INTERNAL_FUNCTION_PARAM_PASSTHRU, "notice");
}

/**
 * Shows a HTML success message
 *
 *<code>
 * $flash->success('The process was finished successfully');
 *</code>
 *
 * @param string $message
 * @return string
 */
PHP_METHOD(Dao_Flash, success)
{
	dao_flash_message_helper(INTERNAL_FUNCTION_PARAM_PASSTHRU, "success");
}

/**
 * Shows a HTML warning message
 *
 *<code>
 * $flash->warning('Hey, this is important');
 *</code>
 *
 * @param string $message
 * @return string
 */
PHP_METHOD(Dao_Flash, warning)
{
	dao_flash_message_helper(INTERNAL_FUNCTION_PARAM_PASSTHRU, "warning");
}

/**
 * Outputs a message formatting it with HTML
 *
 *<code>
 * $flash->outputMessage('error', $message);
 *</code>
 *
 * @param string $type
 * @param string $message
 */
PHP_METHOD(Dao_Flash, outputMessage){

	zval *type, *message, automatic_html = {}, classes = {}, type_classes = {}, joined_classes = {}, css_classes = {}, implicit_flush = {}, content = {}, *msg, html_message = {};
	int flag_automatic_html;
	int flag_implicit_flush;

	dao_fetch_params(1, 2, 0, &type, &message);

	dao_read_property(&automatic_html, getThis(), SL("_automaticHtml"), PH_READONLY);
	flag_automatic_html = zend_is_true(&automatic_html);
	if (flag_automatic_html) {
		dao_read_property(&classes, getThis(), SL("_cssClasses"), PH_READONLY);

		if (dao_array_isset_fetch(&type_classes, &classes, type, PH_READONLY)) {
			if (Z_TYPE(type_classes) == IS_ARRAY) {
				dao_fast_join_str(&joined_classes, SL(" "), &type_classes);

				DAO_CONCAT_SVS(&css_classes, " class=\"", &joined_classes, "\"");
				zval_ptr_dtor(&joined_classes);
			} else {
				DAO_CONCAT_SVS(&css_classes, " class=\"", &type_classes, "\"");
			}
		} else {
			ZVAL_EMPTY_STRING(&css_classes);
		}
	}
	DAO_MM_ADD_ENTRY(&css_classes);

	dao_read_property(&implicit_flush, getThis(), SL("_implicitFlush"), PH_READONLY);
	flag_implicit_flush = zend_is_true(&implicit_flush);
	if (Z_TYPE_P(message) == IS_ARRAY) {
		/**
		 * We create the message with implicit flush or other
		 */
		if (!flag_implicit_flush) {
			DAO_MM_ZVAL_EMPTY_STRING(&content);
		}

		/**
		 * We create the message with implicit flush or other
		 */
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(message), msg) {
			zval html_message0 = {};
			/**
			 * We create the applying formatting or not
			 */
			if (flag_automatic_html) {
				DAO_CONCAT_SVSVS(&html_message0, "<div", &css_classes, ">", msg, "</div>" PHP_EOL);
				DAO_MM_ADD_ENTRY(&html_message0);
			} else {
				ZVAL_COPY_VALUE(&html_message0, msg);
			}

			if (flag_implicit_flush) {
				zend_print_zval(&html_message0, 0);
			} else {
				dao_concat_self(&content, &html_message0);
				DAO_MM_ADD_ENTRY(&content);
			}
		} ZEND_HASH_FOREACH_END();

		/**
		 * We return the message as string if the implicit_flush is turned off
		 */
		if (!flag_implicit_flush) {
			RETURN_MM_CTOR(&content);
		}
	} else {
		/**
		 * We create the applying formatting or not
		 */
		if (flag_automatic_html) {
			DAO_CONCAT_SVSVS(&html_message, "<div", &css_classes, ">", message, "</div>" PHP_EOL);
			DAO_MM_ADD_ENTRY(&html_message);
		} else {
			ZVAL_COPY_VALUE(&html_message, message);
		}

		/**
		 * We return the message as string if the implicit_flush is turned off
		 */
		if (flag_implicit_flush) {
			zend_print_zval(&html_message, 0);
			RETURN_MM();
		}
		RETURN_MM_CTOR(&html_message);

	}
	RETURN_MM();
}
