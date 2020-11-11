
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

#include "paginator/adapter/querybuilder.h"
#include "paginator/adapter.h"
#include "paginator/adapterinterface.h"
#include "paginator/exception.h"
#include "mvc/model/query/builderinterface.h"
#include "mvc/model/managerinterface.h"
#include "db/rawvalue.h"
#include "cache/backendinterface.h"
#include "cache/frontendinterface.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/array.h"
#include "kernel/exception.h"
#include "kernel/operators.h"
#include "kernel/fcall.h"
#include "kernel/hash.h"
#include "kernel/concat.h"
#include "kernel/string.h"

#include "interned-strings.h"

/**
 * Dao\Paginator\Adapter\QueryBuilder
 *
 * Pagination using a PHQL query builder as source of data
 *
 *<code>
 *  $builder = $this->modelsManager->createBuilder()
 *                   ->columns('id, name')
 *                   ->from('Robots')
 *                   ->orderBy('name');
 *
 *  $paginator = new Dao\Paginator\Adapter\QueryBuilder(array(
 *      "builder" => $builder,
 *      "limit"=> 20,
 *      "page" => 1
 *  ));
 *</code>
 */
zend_class_entry *dao_paginator_adapter_querybuilder_ce;

PHP_METHOD(Dao_Paginator_Adapter_QueryBuilder, __construct);
PHP_METHOD(Dao_Paginator_Adapter_QueryBuilder, getPaginate);
PHP_METHOD(Dao_Paginator_Adapter_QueryBuilder, setQueryBuilder);
PHP_METHOD(Dao_Paginator_Adapter_QueryBuilder, getQueryBuilder);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_paginator_adapter_querybuilder___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, config)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_paginator_adapter_querybuilder_setquerybuilder, 0, 0, 1)
	ZEND_ARG_INFO(0, queryBuilder)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_paginator_adapter_querybuilder_method_entry[] = {
	PHP_ME(Dao_Paginator_Adapter_QueryBuilder, __construct, arginfo_dao_paginator_adapter_querybuilder___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Paginator_Adapter_QueryBuilder, getPaginate, arginfo_dao_paginator_adapterinterface_getpaginate, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Paginator_Adapter_QueryBuilder, setQueryBuilder, arginfo_dao_paginator_adapter_querybuilder_setquerybuilder, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Paginator_Adapter_QueryBuilder, getQueryBuilder, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Paginator\Adapter\QueryBuilder initializer
 */
DAO_INIT_CLASS(Dao_Paginator_Adapter_QueryBuilder){

	DAO_REGISTER_CLASS_EX(Dao\\Paginator\\Adapter, QueryBuilder, paginator_adapter_querybuilder, dao_paginator_adapter_ce, dao_paginator_adapter_querybuilder_method_entry, 0);

	zend_declare_property_null(dao_paginator_adapter_querybuilder_ce, SL("_builder"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_paginator_adapter_querybuilder_ce, SL("_totalItems"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_paginator_adapter_querybuilder_ce, SL("_limitRows"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_paginator_adapter_querybuilder_ce, SL("_cache"), ZEND_ACC_PROTECTED);

	zend_class_implements(dao_paginator_adapter_querybuilder_ce, 1, dao_paginator_adapterinterface_ce);

	return SUCCESS;
}

/**
 * Dao\Paginator\Adapter\QueryBuilder
 *
 * @param array $config
 */
PHP_METHOD(Dao_Paginator_Adapter_QueryBuilder, __construct){

	zval *config, builder = {}, limit = {}, page = {}, total_items = {}, cache = {};
	long int i_limit;

	dao_fetch_params(0, 1, 0, &config);

	if (!dao_array_isset_fetch_str(&builder, config, SL("builder"), PH_READONLY)) {
		DAO_THROW_EXCEPTION_STR(dao_paginator_exception_ce, "Parameter 'builder' is required");
		return;
	}

	DAO_VERIFY_INTERFACE_EX(&builder, dao_mvc_model_query_builderinterface_ce, dao_paginator_exception_ce);

	dao_update_property(getThis(), SL("_builder"), &builder);

	if (!dao_array_isset_fetch_str(&limit, config, SL("limit"), PH_READONLY)) {
		DAO_THROW_EXCEPTION_STR(dao_paginator_exception_ce, "Parameter 'limit' is required");
		return;
	}

	i_limit = dao_get_intval(&limit);
	if (i_limit < 1) {
		DAO_THROW_EXCEPTION_STR(dao_paginator_exception_ce, "'limit' should be positive");
		return;
	}

	dao_update_property(getThis(), SL("_limitRows"), &limit);

	if (dao_array_isset_fetch_str(&page, config, SL("page"), PH_READONLY)) {
		dao_update_property(getThis(), SL("_page"), &page);
	}

	if (dao_array_isset_fetch_str(&total_items, config, SL("totalItems"), PH_READONLY) && Z_TYPE(total_items) == IS_LONG) {
		dao_update_property(getThis(), SL("_totalItems"), &total_items);
	}

	if (dao_array_isset_fetch_str(&cache, config, SL("cache"), PH_READONLY)) {
		dao_update_property(getThis(), SL("_cache"), &cache);
	}
}

/**
 * Set query builder object
 *
 * @param Dao\Mvc\Model\Query\BuilderInterface $builder
 *
 * @return Dao\Paginator\Adapter\QueryBuilder $this Fluent interface
 */
PHP_METHOD(Dao_Paginator_Adapter_QueryBuilder, setQueryBuilder){

	zval *query_builder;

	dao_fetch_params(0, 1, 0, &query_builder);
	DAO_VERIFY_INTERFACE_EX(query_builder, dao_mvc_model_query_builderinterface_ce, dao_paginator_exception_ce);

	dao_update_property(getThis(), SL("_builder"), query_builder);

	RETURN_THIS();
}

/**
 * Get query builder object
 *
 * @return Dao\Mvc\Model\Query\BuilderInterface $builder
 */
PHP_METHOD(Dao_Paginator_Adapter_QueryBuilder, getQueryBuilder){

	RETURN_MEMBER(getThis(), "_builder");
}

static inline void dao_query_sql_replace(zval *sql, zval *wildcard, zval *value)
{
	zval string_wildcard = {}, fixed_value = {}, sql_tmp = {};
	DAO_CONCAT_SVS(&string_wildcard, ":", wildcard, ",");
	if (!dao_memnstr(sql, &string_wildcard)) {
		zval_ptr_dtor(&string_wildcard);
		DAO_CONCAT_SVS(&string_wildcard, ":", wildcard, " ");
		if (!dao_memnstr(sql, &string_wildcard)) {
			zval_ptr_dtor(&string_wildcard);
			DAO_CONCAT_SVS(&string_wildcard, ":", wildcard, ")");

			if (!dao_memnstr(sql, &string_wildcard)) {
				zval_ptr_dtor(&string_wildcard);
				DAO_CONCAT_SV(&string_wildcard, ":", wildcard);
				ZVAL_COPY(&fixed_value, value);
			} else {
				DAO_CONCAT_VS(&fixed_value, value, ")");
			}
		} else {
			DAO_CONCAT_VS(&fixed_value, value, " ");
		}
	} else {
		DAO_CONCAT_VS(&fixed_value, value, ",");
	}
	dao_fast_str_replace(&sql_tmp, &string_wildcard, &fixed_value, sql);
	zval_ptr_dtor(&string_wildcard);
	zval_ptr_dtor(&fixed_value);
	ZVAL_COPY_VALUE(sql, &sql_tmp);
}

/**
 * Returns a slice of the resultset to show in the pagination
 *
 * @return \stdClass
 */
PHP_METHOD(Dao_Paginator_Adapter_QueryBuilder, getPaginate){

	zval event_name = {}, original_builder = {}, builder = {}, total_builder = {}, limit = {}, number_page = {}, number = {}, query = {}, items = {};
	zval total_query = {}, result = {}, row = {}, rowcount = {}, dependency_injector = {};
	zval service_name = {}, models_manager = {}, models = {}, model_name = {}, model = {}, connection = {}, bind_params = {}, bind_types = {};
	zval processed = {}, *value, processed_types = {};
	zval intermediate = {}, dialect = {}, sql = {}, tmp = {}, page = {};
	zend_string *str_key;
	ulong idx;
	ldiv_t tp;
	long int i_limit, i_number_page, i_number, i_before, i_rowcount;
	long int i_total_pages, i_next;

	DAO_MM_INIT();

	DAO_MM_ZVAL_STRING(&event_name, "pagination:beforeGetPaginate");
	DAO_MM_CALL_METHOD(NULL, getThis(), "fireevent", &event_name);

	dao_read_property(&original_builder, getThis(), SL("_builder"), PH_READONLY);

	/* Make a copy of the original builder to leave it as it is */
	if (dao_clone(&builder, &original_builder) == FAILURE) {
		RETURN_MM();
	}

	/* make a copy of the original builder to count the total of records */
	if (dao_clone(&total_builder, &builder) == FAILURE) {
		RETURN_MM();
	}

	dao_read_property(&limit, getThis(), SL("_limitRows"), PH_READONLY);
	dao_read_property(&number_page, getThis(), SL("_page"), PH_READONLY);
	i_limit       = dao_get_intval(&limit);
	i_number_page = dao_get_intval(&number_page);

	if (i_limit < 1) {
		/* This should never happen unless someone deliberately modified the properties of the object */
		i_limit = 10;
	}

	if (!i_number_page) {
		i_number_page = 1;
	}

	i_number = (i_number_page - 1) * i_limit;
	i_before = (i_number_page == 1) ? 1 : (i_number_page - 1);

	/* Set the limit clause avoiding negative offsets */
	if (i_number < i_limit) {
		DAO_MM_CALL_METHOD(NULL, &builder, "limit", &limit);
	} else {
		ZVAL_LONG(&number, i_number);
		DAO_MM_CALL_METHOD(NULL, &builder, "limit", &limit, &number);
	}

	DAO_MM_CALL_METHOD(&query, &builder, "getquery");
	DAO_MM_ADD_ENTRY(&query);

	/* Execute the query an return the requested slice of data */
	DAO_MM_CALL_METHOD(&items, &query, "execute");
	DAO_MM_ADD_ENTRY(&items);

	dao_read_property(&rowcount, getThis(), SL("_totalItems"), PH_READONLY);
	if (Z_TYPE(rowcount) != IS_LONG) {
		zval cache_options = {}, cache_key = {}, cache_service = {}, cache = {}, lifetime = {};
		dao_read_property(&cache_options, getThis(), SL("_cache"), PH_READONLY);
		if (Z_TYPE(cache_options) == IS_ARRAY) {
			if (!dao_array_isset_fetch_str(&cache_key, &cache_options, SL("key"), PH_READONLY)) {
				DAO_MM_THROW_EXCEPTION_STR(dao_paginator_exception_ce, "A cache key must be provided to identify the cached resultset in the cache backend");
				return;
			}

			if (!dao_array_isset_fetch_str(&cache_service, &cache_options, SL("service"), PH_READONLY)) {
				ZVAL_STR(&cache_service, IS(modelsCache));
			}

			DAO_MM_CALL_METHOD(&dependency_injector, getThis(), "getdi", &DAO_GLOBAL(z_true));
			DAO_MM_ADD_ENTRY(&dependency_injector);

			DAO_MM_CALL_METHOD(&cache, &dependency_injector, "getshared", &cache_service);
			DAO_MM_ADD_ENTRY(&cache);
			if (Z_TYPE(cache) != IS_OBJECT) {
				DAO_MM_THROW_EXCEPTION_STR(dao_paginator_exception_ce, "The cache service must be an object");
				return;
			}

			DAO_MM_VERIFY_INTERFACE(&cache, dao_cache_backendinterface_ce);
			if (!dao_array_isset_fetch_str(&lifetime, &cache_options, SL("lifetime"), PH_READONLY)) {
				zval frontend = {};
				DAO_MM_CALL_METHOD(&frontend, &cache, "getfrontend");
				DAO_MM_ADD_ENTRY(&frontend);

				if (Z_TYPE(frontend) == IS_OBJECT) {
					DAO_MM_VERIFY_INTERFACE_EX(&frontend, dao_cache_frontendinterface_ce, dao_paginator_exception_ce);
					DAO_MM_CALL_METHOD(&lifetime, &frontend, "getlifetime");
				} else {
					ZVAL_LONG(&lifetime, 3600);
				}
			}

			DAO_MM_CALL_METHOD(&row, &cache, "get", &cache_key, &lifetime);
			DAO_MM_ADD_ENTRY(&row);
		}
		
		if (Z_TYPE(row) != IS_ARRAY) {
			/* Remove the 'ORDER BY' clause, PostgreSQL requires this */
			DAO_MM_CALL_METHOD(NULL, &total_builder, "orderby", &DAO_GLOBAL(z_null));

			/* Obtain the PHQL for the total query */
			DAO_MM_CALL_METHOD(&total_query, &total_builder, "getquery");
			DAO_MM_ADD_ENTRY(&total_query);

			DAO_MM_CALL_METHOD(&dependency_injector, &total_query, "getdi");
			DAO_MM_ADD_ENTRY(&dependency_injector);
			if (Z_TYPE(dependency_injector) != IS_OBJECT) {
				DAO_MM_THROW_EXCEPTION_STR(dao_paginator_exception_ce, "A dependency injection object is required to access internal services");
				return;
			}

			/* Get the connection through the model */
			ZVAL_STR(&service_name, IS(modelsManager));

			DAO_MM_CALL_METHOD(&models_manager, &dependency_injector, "getshared", &service_name);
			DAO_MM_ADD_ENTRY(&models_manager);
			if (Z_TYPE(models_manager) != IS_OBJECT) {
				DAO_MM_THROW_EXCEPTION_STR(dao_paginator_exception_ce, "The injected service 'modelsManager' is not object");
				return;
			}

			DAO_MM_VERIFY_INTERFACE(&models_manager, dao_mvc_model_managerinterface_ce);

			DAO_MM_CALL_METHOD(&models, &builder, "getfrom");
			DAO_MM_ADD_ENTRY(&models);

			if (Z_TYPE(models) == IS_ARRAY) {
				dao_array_get_current(&model_name, &models);
			} else {
				ZVAL_COPY(&model_name, &models);
			}
			DAO_MM_ADD_ENTRY(&model_name);

			DAO_MM_CALL_METHOD(&model, &models_manager, "load", &model_name);
			DAO_MM_ADD_ENTRY(&model);
			DAO_MM_CALL_METHOD(&connection, &model, "getreadconnection");
			DAO_MM_ADD_ENTRY(&connection);
			DAO_MM_CALL_METHOD(&intermediate, &total_query, "parse");
			DAO_MM_ADD_ENTRY(&intermediate);
			DAO_MM_SEPARATE(&intermediate);

			DAO_MM_CALL_METHOD(&tmp, &total_query, "getindex");
			DAO_MM_ADD_ENTRY(&tmp);
			if (Z_TYPE(tmp) > IS_NULL) {
				dao_array_update_str(&intermediate, SL("index"), &tmp, PH_COPY);
			}

			DAO_MM_CALL_METHOD(&dialect, &connection, "getdialect");
			DAO_MM_ADD_ENTRY(&dialect);
			DAO_MM_CALL_METHOD(&sql, &dialect, "select", &intermediate, &DAO_GLOBAL(z_true));
			DAO_MM_ADD_ENTRY(&sql);

			DAO_MM_CALL_METHOD(&bind_params, &total_query, "getbindparams");
			DAO_MM_ADD_ENTRY(&bind_params);
			DAO_MM_CALL_METHOD(&bind_types, &total_query, "getbindtypes");
			DAO_MM_ADD_ENTRY(&bind_types);
			DAO_MM_SEPARATE(&bind_types);

			DAO_MM_CALL_METHOD(&connection, &total_query, "getreadconnection", &intermediate, &bind_params, &bind_types);
			DAO_MM_ADD_ENTRY(&connection);

			/**
			 * Replace the placeholders
			 */
			if (Z_TYPE(bind_params) == IS_ARRAY) {
				array_init(&processed);
				DAO_MM_ADD_ENTRY(&processed);

				ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL(bind_params), idx, str_key, value) {
					zval wildcard = {};
					if (str_key) {
						ZVAL_STR(&wildcard, str_key);
					} else {
						ZVAL_LONG(&wildcard, idx);
					}

					if (Z_TYPE_P(value) == IS_OBJECT && instanceof_function(Z_OBJCE_P(value), dao_db_rawvalue_ce)) {
						zval tmp_value = {};
						DAO_MM_CALL_METHOD(&tmp_value, value, "getvalue");
						dao_query_sql_replace(&sql, &wildcard, &tmp_value);
						zval_ptr_dtor(&tmp_value);
						DAO_MM_ADD_ENTRY(&sql);
						dao_array_unset(&bind_types, &wildcard, 0);
					} else if (Z_TYPE_P(value) == IS_ARRAY) {
						zval *v, bind_keys = {}, joined_keys = {}, hidden_param = {};
						array_init(&bind_keys);
						ZVAL_LONG(&hidden_param, 0);
						ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(value), v) {
							zval k = {}, query_key = {};
							/**
							 * Key with auto bind-params
							 */
							DAO_CONCAT_SVV(&k, "phi", &wildcard, &hidden_param);

							DAO_CONCAT_SV(&query_key, ":", &k);
							dao_array_append(&bind_keys, &query_key, 0);
							dao_array_update(&processed, &k, v, PH_COPY);
							zval_ptr_dtor(&k);
							dao_increment(&hidden_param);
						} ZEND_HASH_FOREACH_END();

						dao_fast_join_str(&joined_keys, SL(", "), &bind_keys);
						zval_ptr_dtor(&bind_keys);
						dao_query_sql_replace(&sql, &wildcard, &joined_keys);
						DAO_MM_ADD_ENTRY(&sql);
						zval_ptr_dtor(&joined_keys);
						dao_array_unset(&bind_types, &wildcard, 0);
					} else if (Z_TYPE(wildcard) == IS_LONG) {
						zval string_wildcard = {};
						DAO_CONCAT_SV(&string_wildcard, ":", &wildcard);
						dao_array_update(&processed, &string_wildcard, value, PH_COPY);
						zval_ptr_dtor(&string_wildcard);
					} else {
						dao_array_update(&processed, &wildcard, value, PH_COPY);
					}
				} ZEND_HASH_FOREACH_END();
			} else {
				DAO_MM_ZVAL_DUP(&processed, &bind_params);
			}

			/**
			 * Replace the bind Types
			 */
			if (Z_TYPE(bind_types) == IS_ARRAY) {
				array_init(&processed_types);
				DAO_MM_ADD_ENTRY(&processed_types);
				ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL(bind_types), idx, str_key, value) {
					zval tmp = {}, string_wildcard = {};
					if (str_key) {
						ZVAL_STR(&tmp, str_key);
					} else {
						ZVAL_LONG(&tmp, idx);
					}

					if (Z_TYPE(tmp) == IS_LONG) {
						DAO_CONCAT_SV(&string_wildcard, ":", &tmp);
						dao_array_update(&processed_types, &string_wildcard, value, PH_COPY);
						zval_ptr_dtor(&string_wildcard);
					} else {
						dao_array_update(&processed_types, &tmp, value, PH_COPY);
					}
				} ZEND_HASH_FOREACH_END();

			} else {
				DAO_MM_ZVAL_COPY(&processed_types, &bind_types);
			}

			DAO_MM_CALL_METHOD(&result, &connection, "query", &sql, &processed, &processed_types);
			DAO_MM_ADD_ENTRY(&result);

			DAO_MM_CALL_METHOD(&row, &result, "fetch");
			DAO_MM_ADD_ENTRY(&row);
			if (Z_TYPE(cache) == IS_OBJECT) {
				zval allow_empty = {};
				if (dao_array_isset_fetch_str(&allow_empty, &cache_options, SL("allowEmpty"), PH_READONLY)) {
					if (zend_is_true(&allow_empty)) {
						DAO_MM_CALL_METHOD(NULL, &cache, "save", &cache_key, &row, &lifetime);
					} else {
						if (Z_TYPE(row) == IS_ARRAY) {
							DAO_MM_CALL_METHOD(NULL, &cache, "save", &cache_key, &row, &lifetime);
						}
					}
				} else {
					DAO_MM_CALL_METHOD(NULL, &cache, "save", &cache_key, &row, &lifetime);
				}
			}
		}
		dao_array_fetch_str(&rowcount, &row, SL("rowcount"), PH_NOISY|PH_READONLY);
	}
	i_rowcount    = dao_get_intval(&rowcount);
	tp            = ldiv(i_rowcount, i_limit);
	i_total_pages = tp.quot + (tp.rem ? 1 : 0);
	i_next        = (i_number_page < i_total_pages) ? (i_number_page + 1) : i_total_pages;

	object_init(&page);
	dao_update_property(&page, SL("items"),			   &items);
	dao_update_property_long(&page, SL("before"),      i_before);
	dao_update_property_long(&page, SL("first"),       1);
	dao_update_property_long(&page, SL("next"),        i_next);
	dao_update_property_long(&page, SL("last"),        i_total_pages);
	dao_update_property_long(&page, SL("current"),     i_number_page);
	dao_update_property_long(&page, SL("total_pages"), i_total_pages);
	dao_update_property_long(&page, SL("totalPages"), i_total_pages);
	dao_update_property_long(&page, SL("total_items"), i_rowcount);
	dao_update_property_long(&page, SL("totalItems"), i_rowcount);
	DAO_MM_ADD_ENTRY(&page);

	DAO_MM_ZVAL_STRING(&event_name, "pagination:afterGetPaginate");
	DAO_MM_CALL_METHOD(return_value, getThis(), "fireevent", &event_name, &page);

	if (Z_TYPE_P(return_value) < IS_ARRAY) {
		zval_ptr_dtor(return_value);
		RETURN_MM_CTOR(&page);
	}
	RETURN_MM();
}
