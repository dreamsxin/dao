
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

#include "escaperinterface.h"
#include "kernel/main.h"

zend_class_entry *dao_escaperinterface_ce;

static const zend_function_entry dao_escaperinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_EscaperInterface, setEncoding, arginfo_dao_escaperinterface_setencoding)
	PHP_ABSTRACT_ME(Dao_EscaperInterface, getEncoding, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_EscaperInterface, setHtmlQuoteType, arginfo_dao_escaperinterface_sethtmlquotetype)
	PHP_ABSTRACT_ME(Dao_EscaperInterface, escapeHtml, arginfo_dao_escaperinterface_escapehtml)
	PHP_ABSTRACT_ME(Dao_EscaperInterface, escapeHtmlAttr, arginfo_dao_escaperinterface_escapehtmlattr)
	PHP_ABSTRACT_ME(Dao_EscaperInterface, escapeCss, arginfo_dao_escaperinterface_escapecss)
	PHP_ABSTRACT_ME(Dao_EscaperInterface, escapeJs, arginfo_dao_escaperinterface_escapejs)
	PHP_ABSTRACT_ME(Dao_EscaperInterface, escapeUrl, arginfo_dao_escaperinterface_escapeurl)
	PHP_FE_END
};

/**
 * Dao\EscaperInterface initializer
 */
DAO_INIT_CLASS(Dao_EscaperInterface){

	DAO_REGISTER_INTERFACE(Dao, EscaperInterface, escaperinterface, dao_escaperinterface_method_entry);

	return SUCCESS;
}

/**
 * Sets the encoding to be used by the escaper
 *
 * @param string $encoding
 */
DAO_DOC_METHOD(Dao_EscaperInterface, setEncoding);

/**
 * Returns the internal encoding used by the escaper
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_EscaperInterface, getEncoding);

/**
 * Sets the HTML quoting type for htmlspecialchars
 *
 * @param int $quoteType
 */
DAO_DOC_METHOD(Dao_EscaperInterface, setHtmlQuoteType);

/**
 * Escapes a HTML string
 *
 * @param string $text
 * @return string
 */
DAO_DOC_METHOD(Dao_EscaperInterface, escapeHtml);

/**
 * Escapes a HTML attribute string
 *
 * @param string $text
 * @return string
 */
DAO_DOC_METHOD(Dao_EscaperInterface, escapeHtmlAttr);

/**
 * Escape CSS strings by replacing non-alphanumeric chars by their hexadecimal representation
 *
 * @param string $css
 * @return string
 */
DAO_DOC_METHOD(Dao_EscaperInterface, escapeCss);

/**
 * Escape Javascript strings by replacing non-alphanumeric chars by their hexadecimal representation
 *
 * @param string $js
 * @return string
 */
DAO_DOC_METHOD(Dao_EscaperInterface, escapeJs);

/**
 * Escapes a URL. Internally uses rawurlencode
 *
 * @param string $url
 * @return string
 */
DAO_DOC_METHOD(Dao_EscaperInterface, escapeUrl);
