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

#ifndef DAO_KERNEL_STRING_H
#define DAO_KERNEL_STRING_H

#include "php_dao.h"

#include <Zend/zend_smart_str.h>

#ifdef DAO_USE_PHP_JSON
# include <ext/json/php_json.h>
#endif

#define DAO_TRIM_LEFT  1
#define DAO_TRIM_RIGHT 2
#define DAO_TRIM_BOTH  3

#define DAO_PDA_LEFT  1
#define DAO_PDA_RIGHT 2
#define DAO_PDA_BOTH  3

#define DAO_RANDOM_ALNUM   0
#define DAO_RANDOM_ALPHA   1
#define DAO_RANDOM_HEXDEC  2
#define DAO_RANDOM_NUMERIC 3
#define DAO_RANDOM_NOZERO  4

#define DAO_STR_REPLACE(return_value, search, replace, subject) dao_fast_str_replace(return_value, search, replace, subject)
#define DAO_PREG_REPLACE(return_value, regex, replace, subject) dao_fast_preg_replace(return_value, regex, replace, subject)

#define dao_smart_str_appendl(dest, src, len) \
	smart_str_appendl_ex((dest), (src), (len), 0)

/** Fast char position */
int dao_memnstr(const zval *haystack, const zval *needle);
int dao_memnstr_string(const zval *haystack, const zend_string *needle);
int dao_memnstr_string_string(zend_string *haystack, zend_string *needle);
int dao_memnstr_str(const zval *haystack, char *needle, unsigned int needle_length);
int dao_memnstr_str_str(const char *haystack, unsigned int haystack_length, char *needle, unsigned int needle_length);
int dao_same_name(const char *key, const char *name, uint32_t name_len);

void dao_strtr(zval *return_value, zval *str, zval *str_from, zval *str_to);
void dao_strtr_str(zval *return_value, zval *str, char *str_from, unsigned int str_from_length, char *str_to, unsigned int str_to_length);
void dao_strtr_array(zval *return_value, zval *str, zval *replace_pairs);

/** Function replacement */
void dao_fast_strlen(zval *return_value, zval *str);
void dao_strlen(zval *return_value, zval *str);
int dao_fast_strlen_ev(zval *str);
void dao_fast_strtolower(zval *return_value, zval *str);
void dao_strtolower(zval *return_value, zval *str);
void dao_strtolower_inplace(zval *s);
void dao_fast_strtoupper(zval *return_value, zval *str);
void dao_strtoupper(zval *return_value, zval *str);
void dao_fast_join(zval *result, zval *glue, zval *pieces);
void dao_append_printable_zval(smart_str *implstr, zval *tmp);
void dao_fast_join_str(zval *result, char *glue, unsigned int glue_length, zval *pieces);
void dao_fast_explode(zval *result, zval *delimiter, zval *str);
void dao_fast_explode_str(zval *result, const char *delimiter, unsigned int delimiter_length, zval *str);
void dao_fast_explode_str2(zval *result, const char *delimiter, unsigned int delimiter_length, zval *str, zend_long limit);
void dao_fast_explode_str_str(zval *result, const char *delimiter, unsigned int delimiter_length, const char *str, unsigned int str_length);
int dao_fast_strpos(zval *return_value, const zval *haystack, const zval *needle);
int dao_fast_strpos_str(zval *return_value, const zval *haystack, const char *needle, unsigned int needle_length);
int dao_fast_stripos_str(zval *return_value, const zval *haystack, const char *needle, unsigned int needle_length);
int dao_fast_strrpos(zval *return_value, const zval *haystack, const zval *needle);
int dao_fast_strrpos_str(zval *return_value, const zval *haystack, const char *needle, unsigned int needle_length);
int dao_fast_strripos_str(zval *return_value, const zval *haystack, const char *needle, unsigned int needle_length);
zend_string* dao_trim(zval *str, zval *charlist, int where);
void dao_fast_strip_tags(zval *return_value, zval *str);
void dao_fast_trim(zval *return_value, zval *str, zval *charlist, int where);
static inline void dao_fast_trim_str(zval *return_value, zval *str, const char *charlist, int where)
{
	zval c = {};

	ZVAL_STRING(&c, charlist);

	dao_fast_trim(return_value, str, &c, where);
	zval_ptr_dtor(&c);
}

void dao_fast_str_replace(zval *return_value, zval *search, zval *replace, zval *subject);
void dao_fast_preg_replace(zval *return_value, zval *search, zval *replace, zval *subject);

static inline void dao_fast_str_replace_str(zval *return_value, const char *search, const char *replace, zval *subject)
{
	zval s = {}, r = {};

	ZVAL_STRING(&s, search);
	ZVAL_STRING(&r, replace);

	dao_fast_str_replace(return_value, &s, &r, subject);
	zval_ptr_dtor(&s);
	zval_ptr_dtor(&r);
}

void dao_pad_str(zval *return_value, zval *input, int pad_length, const char *pad_str, int pad_type);

/** Camelize/Uncamelize */
void dao_camelize(zval *return_value, const zval *str);
void dao_uncamelize(zval *return_value, const zval *str);

void dao_camelize_delim(zval *return_value, const zval *str, const zval *delimiter);
void dao_uncamelize_delim(zval *return_value, const zval *str, const zval *delimiter);

/** Starts/Ends with */
int dao_start_with(const zval *str, const zval *compared, zval *case_sensitive);
int dao_start_with_str(const zval *str, char *compared, unsigned int compared_length);
int dao_start_with_str_str(char *str, unsigned int str_length, char *compared, unsigned int compared_length);
int dao_end_with(const zval *str, const zval *compared, zval *case_sensitive);
int dao_end_with_str(const zval *str, char *compared, unsigned int compared_length);

/** Compare */
int dao_comparestr(const zval *str, const zval *compared, zval *case_sensitive);
int dao_comparestr_str(const zval *str, char *compared, unsigned int compared_length, zval *case_sensitive);

/** Random string */
void dao_random_string(zval *return_value, const zval *type, const zval *length);

/* Strips extra slashes */
void dao_remove_extra_slashes(zval *return_value, const zval *str);

/** Generates a unique key for an array/object */
void dao_unique_key(zval *return_value, zval *prefix, zval *value);

/** spprintf */
int dao_spprintf(char **message, int max_len, char *format, ...);

/* Substr */
void dao_substr(zval *return_value, zval *str, unsigned long from, long length);
void dao_substr_string(zval *return_value, zend_string *str, unsigned long from, long length);

/** Preg-Match */
int dao_preg_match(zval *return_value, zval *regex, zval *subject, zval *matches, zend_long flags, int global);

/** Base64 */
void dao_base64_encode(zval *return_value, zval *data);
void dao_base64_decode(zval *return_value, zval *data);

/** Hash */
void dao_md5(zval *return_value, zval *str);
void dao_crc32(zval *return_value, zval *str);

/** JSON */
int dao_json_encode(zval *return_value, zval *v, int opts);
int dao_json_decode(zval *return_value, zval *v, zend_bool assoc);

/***/
void dao_lcfirst(zval *return_value, zval *s);
void dao_ucfirst(zval *return_value, zval *s);
int dao_http_build_query(zval *return_value, zval *params, char *sep);
void dao_htmlspecialchars(zval *return_value, zval *string, zval *quoting, zval *charset);
void dao_htmlentities(zval *return_value, zval *string, zval *quoting, zval *charset);
void dao_strval(zval *return_value, zval *v);
void dao_date(zval *return_value, zval *format, zval *timestamp);
void dao_addslashes(zval *return_value, zval *str);
void dao_add_trailing_slash(zval *return_value, zval* v);
void dao_stripslashes(zval *return_value, zval *str);
void dao_stripcslashes(zval *return_value, zval *str);

#ifdef DAO_USE_PHP_MBSTRING
void dao_detect_encoding(zval *return_value, zval *str, zval *charset, zend_bool strict);
void dao_convert_encoding(zval *return_value, zval *str, zval *to, zval *from);
#endif

#endif /* DAO_KERNEL_STRING_H */
