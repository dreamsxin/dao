
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

#include "escaper.h"
#include "escaperinterface.h"
#include "escaper/exception.h"

#include <ext/standard/html.h>

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/exception.h"
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/filter.h"
#include "kernel/string.h"
#include "kernel/framework/url.h"

/**
 * Dao\Escaper
 *
 * Escapes different kinds of text securing them. By using this component you may
 * prevent XSS attacks.
 *
 * This component only works with UTF-8. The PREG extension needs to be compiled with UTF-8 support.
 *
 *<code>
 *	$escaper = new Dao\Escaper();
 *	$escaped = $escaper->escapeCss("font-family: <Verdana>");
 *	echo $escaped; // font\2D family\3A \20 \3C Verdana\3E
 *</code>
 */
zend_class_entry *dao_escaper_ce;

PHP_METHOD(Dao_Escaper, setEncoding);
PHP_METHOD(Dao_Escaper, getEncoding);
PHP_METHOD(Dao_Escaper, setHtmlQuoteType);
PHP_METHOD(Dao_Escaper, detectEncoding);
PHP_METHOD(Dao_Escaper, normalizeEncoding);
PHP_METHOD(Dao_Escaper, escapeHtml);
PHP_METHOD(Dao_Escaper, escapeHtmlAttr);
PHP_METHOD(Dao_Escaper, escapeCss);
PHP_METHOD(Dao_Escaper, escapeJs);
PHP_METHOD(Dao_Escaper, escapeUrl);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_escaper_detectencoding, 0, 0, 1)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_escaper_normalizeencoding, 0, 0, 1)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_escaper_method_entry[] = {
	PHP_ME(Dao_Escaper, setEncoding, arginfo_dao_escaperinterface_setencoding, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Escaper, getEncoding, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Escaper, setHtmlQuoteType, arginfo_dao_escaperinterface_sethtmlquotetype, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Escaper, detectEncoding, arginfo_dao_escaper_detectencoding, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Escaper, normalizeEncoding, arginfo_dao_escaper_normalizeencoding, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Escaper, escapeHtml, arginfo_dao_escaperinterface_escapehtml, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Escaper, escapeHtmlAttr, arginfo_dao_escaperinterface_escapehtmlattr, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Escaper, escapeCss, arginfo_dao_escaperinterface_escapecss, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Escaper, escapeJs, arginfo_dao_escaperinterface_escapejs, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Escaper, escapeUrl, arginfo_dao_escaperinterface_escapeurl, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Escaper initializer
 */
DAO_INIT_CLASS(Dao_Escaper){

	DAO_REGISTER_CLASS(Dao, Escaper, escaper, dao_escaper_method_entry, 0);

	zend_declare_property_string(dao_escaper_ce, SL("_encoding"), "utf-8", ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_escaper_ce, SL("_htmlEscapeMap"), ZEND_ACC_PROTECTED);
	zend_declare_property_long(dao_escaper_ce, SL("_htmlQuoteType"), 3, ZEND_ACC_PROTECTED);

	zend_class_implements(dao_escaper_ce, 1, dao_escaperinterface_ce);

	return SUCCESS;
}

/**
 * Sets the encoding to be used by the escaper
 *
 *<code>
 * $escaper->setEncoding('utf-8');
 *</code>
 *
 * @param string $encoding
 */
PHP_METHOD(Dao_Escaper, setEncoding){

	zval *encoding;

	dao_fetch_params(0, 1, 0, &encoding);
	dao_update_property(getThis(), SL("_encoding"), encoding);
}

/**
 * Returns the internal encoding used by the escaper
 *
 * @return string
 */
PHP_METHOD(Dao_Escaper, getEncoding){


	RETURN_MEMBER(getThis(), "_encoding");
}

/**
 * Sets the HTML quoting type for htmlspecialchars
 *
 *<code>
 * $escaper->setHtmlQuoteType(ENT_XHTML);
 *</code>
 *
 * @param int $quoteType
 */
PHP_METHOD(Dao_Escaper, setHtmlQuoteType){

	zval *quote_type;

	dao_fetch_params(0, 1, 0, &quote_type);
	dao_update_property(getThis(), SL("_htmlQuoteType"), quote_type);
}

/**
 * Detect the character encoding of a string to be handled by an encoder
 * Special-handling for chr(172) and chr(128) to chr(159) which fail to be detected by mb_detect_encoding()
 *
 * @param string $str
 * @param string $charset
 * @return string
 */
PHP_METHOD(Dao_Escaper, detectEncoding){

	zval *str, charset = {}, strict_check = {}, detected = {};

	dao_fetch_params(0, 1, 0, &str);

	/**
	 * Check if charset is ASCII or ISO-8859-1
	 */
	dao_is_basic_charset(&charset, str);
	if (Z_TYPE(charset) == IS_STRING) {
		RETURN_CTOR(&charset);
	}

	/**
	 * Strict encoding detection with fallback to non-strict detection.
	 */
	ZVAL_TRUE(&strict_check);

	/**
	 * Check for UTF-32 encoding
	 */
	ZVAL_STRING(&charset, "UTF-32");

#ifdef DAO_USE_PHP_MBSTRING
	dao_detect_encoding(&detected, str, &charset, &strict_check);
#else
	/**
	 * We require mbstring extension here
	 */
	if (dao_function_exists_ex(SL("mb_detect_encoding")) == FAILURE) {
		RETURN_NULL();
	}

	DAO_CALL_FUNCTION(&detected, "mb_detect_encoding", str, &charset, &strict_check);
#endif
	if (zend_is_true(&detected)) {
		RETURN_CTOR(&charset);
	}
	zval_ptr_dtor(&charset);

	/**
	 * Check for UTF-16 encoding
	 */
	ZVAL_STRING(&charset, "UTF-16");

#ifdef DAO_USE_PHP_MBSTRING
	dao_detect_encoding(&detected, str, &charset, &strict_check);
#else
	DAO_CALL_FUNCTION(&detected, "mb_detect_encoding", str, &charset, &strict_check);
#endif
	if (zend_is_true(&detected)) {
		RETURN_NCTOR(&charset);
	}
	zval_ptr_dtor(&charset);

	/**
	 * Check for UTF-8 encoding
	 */
	ZVAL_STRING(&charset, "UTF-8");

#ifdef DAO_USE_PHP_MBSTRING
	dao_detect_encoding(&detected, str, &charset, &strict_check);
#else
	DAO_CALL_FUNCTION(&detected, "mb_detect_encoding", str, &charset, &strict_check);
#endif
	if (zend_is_true(&detected)) {
		RETURN_NCTOR(&charset);
	}
	zval_ptr_dtor(&charset);

	/**
	 * Check for ISO-8859-1 encoding
	 */
	ZVAL_STRING(&charset, "ISO-8859-1");
#ifdef DAO_USE_PHP_MBSTRING
	dao_detect_encoding(&detected, str, &charset, &strict_check);
#else
	DAO_CALL_FUNCTION(&detected, "mb_detect_encoding", str, &charset, &strict_check);
#endif
	if (zend_is_true(&detected)) {
		RETURN_NCTOR(&charset);
	}
	zval_ptr_dtor(&charset);

	/**
	 * Check for ASCII encoding
	 */
	ZVAL_STRING(&charset, "ASCII");
#ifdef DAO_USE_PHP_MBSTRING
	dao_detect_encoding(&detected, str, &charset, &strict_check);
#else
	DAO_CALL_FUNCTION(&detected, "mb_detect_encoding", str, &charset, &strict_check);
#endif
	if (zend_is_true(&detected)) {
		RETURN_NCTOR(&charset);
	}
	zval_ptr_dtor(&charset);

	/**
	 * Fallback to global detection
	 */
	DAO_RETURN_CALL_FUNCTION("mb_detect_encoding", str);
}

/**
 * Utility to normalize a string's encoding to UTF-32.
 *
 * @param string $str
 * @return string
 */
PHP_METHOD(Dao_Escaper, normalizeEncoding){

	zval *str, encoding = {}, charset = {};

	dao_fetch_params(0, 1, 0, &str);

	/**
	 * mbstring is required here
	 */
	if (dao_function_exists_ex(SL("mb_convert_encoding")) == FAILURE) {
		DAO_THROW_EXCEPTION_STR(dao_escaper_exception_ce, "Extension 'mbstring' is required");
		return;
	}

	DAO_CALL_METHOD(&encoding, getThis(), "detectencoding", getThis());

	ZVAL_STRING(&charset, "UTF-32");

	/**
	 * Convert to UTF-32 (4 byte characters, regardless of actual number of bytes in
	 * the character).
	 */
	DAO_RETURN_CALL_FUNCTION("mb_convert_encoding", str, &charset, &encoding);
}

/**
 * Escapes a HTML string. Internally uses htmlspecialchars
 *
 * @param string $text
 * @return string
 */
PHP_METHOD(Dao_Escaper, escapeHtml){

	zval *text, html_quote_type = {}, encoding = {};

	dao_fetch_params(0, 1, 0, &text);

	if (Z_TYPE_P(text) == IS_STRING) {
		dao_read_property(&html_quote_type, getThis(), SL("_htmlQuoteType"), PH_NOISY|PH_READONLY);
		dao_read_property(&encoding, getThis(), SL("_encoding"), PH_NOISY|PH_READONLY);

		dao_htmlspecialchars(return_value, text, &html_quote_type, &encoding);
		return;
	}

	RETURN_CTOR(text);
}

/**
 * Escapes a HTML attribute string
 *
 * @param string $attribute
 * @return string
 */
PHP_METHOD(Dao_Escaper, escapeHtmlAttr){

	zval *attribute, quoting = {}, encoding = {};

	dao_fetch_params(0, 1, 0, &attribute);

	if (Z_TYPE_P(attribute) == IS_STRING && zend_is_true(attribute)) {
		ZVAL_LONG(&quoting, ENT_QUOTES);

		dao_read_property(&encoding, getThis(), SL("_encoding"), PH_NOISY|PH_READONLY);

		dao_htmlspecialchars(return_value, attribute, &quoting, &encoding);
		return;
	}

	RETURN_CTOR(attribute);
}

/**
 * Escape CSS strings by replacing non-alphanumeric chars by their hexadecimal escaped representation
 *
 * @param string $css
 * @return string
 */
PHP_METHOD(Dao_Escaper, escapeCss){

	zval *css, normalized = {};

	dao_fetch_params(0, 1, 0, &css);

	if (Z_TYPE_P(css) == IS_STRING && zend_is_true(css)) {
		/**
		 * Normalize encoding to UTF-32
		 */
		DAO_CALL_METHOD(&normalized, getThis(), "normalizeencoding", css);

		/**
		 * Escape the string
		 */
		dao_escape_css(return_value, &normalized);
		return;
	}

	RETURN_CTOR(css);
}

/**
 * Escape javascript strings by replacing non-alphanumeric chars by their hexadecimal escaped representation
 *
 * @param string $js
 * @return string
 */
PHP_METHOD(Dao_Escaper, escapeJs){

	zval *js, normalized = {};

	dao_fetch_params(0, 1, 0, &js);

	if (Z_TYPE_P(js) == IS_STRING && zend_is_true(js)) {
		/**
		 * Normalize encoding to UTF-32
		 */
		DAO_CALL_METHOD(&normalized, getThis(), "normalizeencoding", js);

		/**
		 * Escape the string
		 */
		dao_escape_js(return_value, &normalized);
		return;
	}

	RETURN_CTOR(js);
}

/**
 * Escapes a URL. Internally uses rawurlencode
 *
 * @param string $url
 * @return string
 */
PHP_METHOD(Dao_Escaper, escapeUrl){

	zval *url;

	dao_fetch_params(0, 1, 0, &url);

	dao_raw_url_encode(return_value, url);
}
