
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

#ifndef DAO_HTTP_PARSER_H
#define DAO_HTTP_PARSER_H

#include "php_dao.h"

#include "http/parser/http_parser.h"

#include <Zend/zend_smart_str.h>

int dao_http_parser_on_message_begin(http_parser *);
int dao_http_parser_on_url(http_parser *, const char *at, size_t length);
int dao_http_parser_on_status(http_parser *, const char *at, size_t length);
int dao_http_parser_on_header_field(http_parser *, const char *at, size_t length);
int dao_http_parser_on_header_value(http_parser *, const char *at, size_t length);
int dao_http_parser_on_headers_complete(http_parser *);
int dao_http_parser_on_body(http_parser *, const char *at, size_t length);
int dao_http_parser_on_message_complete(http_parser *);
int dao_http_parser_on_chunk_header(http_parser *);
int dao_http_parser_on_chunk_complete(http_parser *);

typedef enum {
    HTTP_PARSER_STATE_NONE, HTTP_PARSER_STATE_BEGIN, HTTP_PARSER_STATE_URL, HTTP_PARSER_STATE_STATUS, HTTP_PARSER_STATE_FIELD, HTTP_PARSER_STATE_VALUE, HTTP_PARSER_STATE_HEADER_END, HTTP_PARSER_STATE_BODY, HTTP_PARSER_STATE_CHUNK, HTTP_PARSER_STATE_END
} dao_parser_state;

typedef struct {
    struct http_parser *parser;
    struct http_parser_settings *settings;
    dao_parser_state state;
    zval head;
    smart_str url;
    smart_str body;
    zend_string *last_key;
} dao_http_parser_data;

dao_http_parser_data *dao_http_parser_data_new();
void dao_http_parser_data_free(dao_http_parser_data *hp);

typedef struct _dao_http_parser_object {
	dao_http_parser_data *data;
	zend_object std;
} dao_http_parser_object;

static inline dao_http_parser_object *dao_http_parser_object_from_obj(zend_object *obj) {
	return (dao_http_parser_object*)((char*)(obj) - XtOffsetOf(dao_http_parser_object, std));
}

extern zend_class_entry *dao_http_parser_ce;

DAO_INIT_CLASS(Dao_Http_Parser);

#endif /* DAO_HTTP_PARSER_H */
