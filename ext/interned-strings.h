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
  |          Vladimir Kolesnikov <vladimir@extrememember.com>              |
  |          ZhuZongXin <dreamsxin@qq.com>                                 |
  +------------------------------------------------------------------------+
*/

#ifndef DAO_INTERNED_STRINGS_H
#define DAO_INTERNED_STRINGS_H

#include <TSRM/TSRM.h>

extern zend_string *dao_interned_DELETE;
extern zend_string *dao_interned_GET;
extern zend_string *dao_interned_HEAD;
extern zend_string *dao_interned_OPTIONS;
extern zend_string *dao_interned_PATCH;
extern zend_string *dao_interned_POST;
extern zend_string *dao_interned_PUT;
extern zend_string *dao_interned_default;
extern zend_string *dao_interned_autoLoad;
extern zend_string *dao_interned_di;
extern zend_string *dao_interned_action;
extern zend_string *dao_interned_alias;
extern zend_string *dao_interned_all;
extern zend_string *dao_interned_allowEmpty;
extern zend_string *dao_interned_arguments;
extern zend_string *dao_interned_balias;
extern zend_string *dao_interned_binary_op;
extern zend_string *dao_interned_code;
extern zend_string *dao_interned_column;
extern zend_string *dao_interned_columns;
extern zend_string *dao_interned_conditions;
extern zend_string *dao_interned_controller;
extern zend_string *dao_interned_delete;
extern zend_string *dao_interned_dispatcher;
extern zend_string *dao_interned_distinct;
extern zend_string *dao_interned_domain;
extern zend_string *dao_interned_escaper;
extern zend_string *dao_interned_expr;
extern zend_string *dao_interned_fields;
extern zend_string *dao_interned_file;
extern zend_string *dao_interned_filter;
extern zend_string *dao_interned_functionCall;
extern zend_string *dao_interned_group;
extern zend_string *dao_interned_groupBy;
extern zend_string *dao_interned_having;
extern zend_string *dao_interned_items;
extern zend_string *dao_interned_joins;
extern zend_string *dao_interned_label;
extern zend_string *dao_interned_left;
extern zend_string *dao_interned_limit;
extern zend_string *dao_interned_forupdate;
extern zend_string *dao_interned_line;
extern zend_string *dao_interned_message;
extern zend_string *dao_interned_model;
extern zend_string *dao_interned_models;
extern zend_string *dao_interned_modelsCache;
extern zend_string *dao_interned_modelsManager;
extern zend_string *dao_interned_modelsMetadata;
extern zend_string *dao_interned_modelsQuery;
extern zend_string *dao_interned_modelsQueryBuilderForSelect;
extern zend_string *dao_interned_modelsQueryBuilderForInsert;
extern zend_string *dao_interned_modelsQueryBuilderForUpdate;
extern zend_string *dao_interned_modelsQueryBuilderForDelete;
extern zend_string *dao_interned_modelsCriteria;
extern zend_string *dao_interned_modelsResultsetSimple;
extern zend_string *dao_interned_modelsResultsetComplex;
extern zend_string *dao_interned_modelsRow;
extern zend_string *dao_interned_module;
extern zend_string *dao_interned_name;
extern zend_string *dao_interned_namespace;
extern zend_string *dao_interned_ns_alias;
extern zend_string *dao_interned_number;
extern zend_string *dao_interned_offset;
extern zend_string *dao_interned_op;
extern zend_string *dao_interned_order;
extern zend_string *dao_interned_orderBy;
extern zend_string *dao_interned_params;
extern zend_string *dao_interned_parent;
extern zend_string *dao_interned_paths;
extern zend_string *dao_interned_qualified;
extern zend_string *dao_interned_qualifiedName;
extern zend_string *dao_interned_request;
extern zend_string *dao_interned_response;
extern zend_string *dao_interned_right;
extern zend_string *dao_interned_router;
extern zend_string *dao_interned_select;
extern zend_string *dao_interned_self;
extern zend_string *dao_interned_session;
extern zend_string *dao_interned_sort;
extern zend_string *dao_interned_source;
extern zend_string *dao_interned_sqlAlias;
extern zend_string *dao_interned_static;
extern zend_string *dao_interned_table;
extern zend_string *dao_interned_tables;
extern zend_string *dao_interned_type;
extern zend_string *dao_interned_update;
extern zend_string *dao_interned_url;
extern zend_string *dao_interned_value;
extern zend_string *dao_interned_values;
extern zend_string *dao_interned_where;
extern zend_string *dao_interned_rows;
extern zend_string *dao_interned_cookies;
extern zend_string *dao_interned_annotations;
extern zend_string *dao_interned_security;
extern zend_string *dao_interned_crypt;
extern zend_string *dao_interned_flash;
extern zend_string *dao_interned_flashSession;
extern zend_string *dao_interned_tag;
extern zend_string *dao_interned_sessionBag;
extern zend_string *dao_interned_eventsManager;
extern zend_string *dao_interned_transactionManager;
extern zend_string *dao_interned_assets;
extern zend_string *dao_interned_db;
extern zend_string *dao_interned_view;
extern zend_string *dao_interned_php;
extern zend_string *dao_interned_except;
extern zend_string *dao_interned_app;
extern zend_string *dao_interned_application;
extern zend_string *dao_interned_validation;
extern zend_string *dao_interned_translate;
extern zend_string *dao_interned_logger;

void dao_init_interned_strings();
void dao_release_interned_strings();

#endif /* DAO_INTERNED_STRINGS_H */
