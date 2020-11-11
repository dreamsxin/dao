
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

#include "php_dao.h"

#include "kernel/memory.h"
#include "kernel/operators.h"
#include "kernel/string.h"
#include "kernel/concat.h"
#include "kernel/array.h"

#include <Zend/zend_smart_str.h>

/**
 * Destroyes the prepared ASTs
 */
void dao_orm_destroy_cache() {

	zend_dao_globals *dao_globals_ptr = DAO_VGLOBAL;

	if (dao_globals_ptr->orm.ast_cache != NULL) {
		zend_hash_destroy(dao_globals_ptr->orm.ast_cache);
		FREE_HASHTABLE(dao_globals_ptr->orm.ast_cache);
		dao_globals_ptr->orm.ast_cache = NULL;
	}
}

/**
 * Obtains a prepared ast in the phalcon's superglobals
 */
void dao_orm_get_prepared_ast(zval *return_value, zval *unique_id) {

	zend_dao_globals *dao_globals_ptr = DAO_VGLOBAL;
	zval *temp_ast;

	if (Z_TYPE_P(unique_id) == IS_LONG) {
		if (dao_globals_ptr->orm.cache_level >= 0) {
			if (dao_globals_ptr->orm.ast_cache != NULL) {
				if ((temp_ast = zend_hash_index_find(dao_globals_ptr->orm.ast_cache, Z_LVAL_P(unique_id))) != NULL) {
					ZVAL_ZVAL(return_value, temp_ast, 1, 0);
					return;
				}
			}
		}
	}
}

/**
 * Stores a prepared ast in the phalcon's superglobals
 */
void dao_orm_set_prepared_ast(zval *unique_id, zval *prepared_ast) {

	zend_dao_globals *dao_globals_ptr = DAO_VGLOBAL;
	zval copy_ast = {};

	if (Z_TYPE_P(unique_id) == IS_LONG) {
		if (dao_globals_ptr->orm.cache_level >= 0) {

			if (!dao_globals_ptr->orm.ast_cache) {
				ALLOC_HASHTABLE(dao_globals_ptr->orm.ast_cache);
				zend_hash_init(dao_globals_ptr->orm.ast_cache, 0, NULL, ZVAL_PTR_DTOR, 0);
			}

			array_init(&copy_ast);

			zend_hash_copy(Z_ARRVAL(copy_ast), Z_ARRVAL_P(prepared_ast), (copy_ctor_func_t)zval_add_ref);

			zend_hash_index_update(dao_globals_ptr->orm.ast_cache, Z_LVAL_P(unique_id), &copy_ast);
		}
	}

}

/**
 * Escapes single quotes into database single quotes
 */
void dao_orm_singlequotes(zval *return_value, zval *str) {

	int i;
	smart_str  escaped_str = {0};
	char *marker;

	if (Z_TYPE_P(str) != IS_STRING) {
		RETURN_ZVAL(str, 1, 0);
	}

	marker = Z_STRVAL_P(str);

	for (i = 0; i < Z_STRLEN_P(str); i++) {
		if ((*marker) == '\0') {
			break;
		}
		if ((*marker) == '\'') {
			if (i > 0) {
				if (*(marker - 1) != '\\') {
					smart_str_appendc(&escaped_str, '\'');
				}
			} else {
				smart_str_appendc(&escaped_str, '\'');
			}
		}
		smart_str_appendc(&escaped_str, (*marker));
		marker++;
	}

	smart_str_0(&escaped_str);

	if (escaped_str.s) {
		RETURN_STR(escaped_str.s);
	} else {
		smart_str_free(&escaped_str);
		RETURN_EMPTY_STRING();
	}
}

void dao_orm_phql_build_group(zval *return_value, zval *group) {

	zval group_items = {}, joined_items = {}, *group_item = NULL;

	if (DAO_IS_NOT_EMPTY(group)) {
		DAO_MM_INIT();
		if (dao_is_numeric(group)) {
			DAO_SCONCAT_SV(return_value, " GROUP BY ", group);
			DAO_MM_ADD_ENTRY(return_value);
		} else if (Z_TYPE_P(group) == IS_ARRAY) {
			array_init(&group_items);
			DAO_MM_ADD_ENTRY(&group_items);
			ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(group), group_item) {
				zval escaped_item = {};
				if (
					dao_is_numeric(group_item)
					|| dao_memnstr_str(group_item, SL("."))
					|| dao_memnstr_str(group_item, SL("["))
					|| dao_memnstr_str(group_item, SL("("))
				) {
					dao_array_append(&group_items, group_item, PH_COPY);
				} else {
					DAO_CONCAT_SVS(&escaped_item, "[", group_item, "]");
					dao_array_append(&group_items, &escaped_item, 0);
				}
			} ZEND_HASH_FOREACH_END();

			dao_fast_join_str(&joined_items, SL(", "), &group_items);
			DAO_SCONCAT_SV(return_value, " GROUP BY ", &joined_items);
			zval_ptr_dtor(&joined_items);
			DAO_MM_ADD_ENTRY(return_value);
		} else if (dao_memnstr_str(group, SL(","))) {
			zval escaped_items = {};
			array_init(&escaped_items);
			DAO_MM_ADD_ENTRY(&escaped_items);

			dao_fast_explode_str(&group_items, SL(", "), group);
			DAO_MM_ADD_ENTRY(&group_items);
			ZEND_HASH_FOREACH_VAL(Z_ARRVAL(group_items), group_item) {
				zval escaped_item = {};
				if (
					dao_is_numeric(group_item)
					|| dao_memnstr_str(group_item, SL("."))
					|| dao_memnstr_str(group_item, SL("["))
					|| dao_memnstr_str(group_item, SL("("))
				) {
					dao_array_append(&escaped_items, group_item, PH_COPY);
				} else {
					DAO_CONCAT_SVS(&escaped_item, "[", group_item, "]");
					dao_array_append(&escaped_items, &escaped_item, 0);
				}
			} ZEND_HASH_FOREACH_END();

			dao_fast_join_str(&joined_items, SL(", "), &escaped_items);
			DAO_SCONCAT_SV(return_value, " GROUP BY ", &joined_items);
			zval_ptr_dtor(&joined_items);
			DAO_MM_ADD_ENTRY(return_value);
		} else if (
			dao_memnstr_str(group, SL("."))
			|| dao_memnstr_str(group, SL("["))
			|| dao_memnstr_str(group, SL("("))
		) {
			DAO_SCONCAT_SV(return_value, " GROUP BY ", group);
			DAO_MM_ADD_ENTRY(return_value);
		} else {
			DAO_SCONCAT_SVS(return_value, " GROUP BY [", group, "]");
			DAO_MM_ADD_ENTRY(return_value);
		}

		Z_TRY_ADDREF_P(return_value);
		RETURN_MM();
	}
	Z_TRY_ADDREF_P(return_value);
}

void dao_orm_phql_build_order(zval *return_value, zval *order) {

	zval order_items = {}, joined_items = {}, *order_item = NULL;

	if (DAO_IS_NOT_EMPTY(order)) {
		DAO_MM_INIT();
		if (Z_TYPE_P(order) == IS_ARRAY) {
			array_init(&order_items);
			DAO_MM_ADD_ENTRY(&order_items);

			ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(order), order_item) {
				zval escaped_item = {};
				if (dao_is_numeric(order_item)) {
					dao_array_append(&order_items, order_item, PH_COPY);
				} else {
					if (dao_memnstr_str(order_item, SL("."))) {
						dao_array_append(&order_items, order_item, PH_COPY);
					} else {
						DAO_CONCAT_SVS(&escaped_item, "[", order_item, "]");
						dao_array_append(&order_items, &escaped_item, 0);
					}
				}
			} ZEND_HASH_FOREACH_END();

			dao_fast_join_str(&joined_items, SL(", "), &order_items);
			DAO_SCONCAT_SV(return_value, " ORDER BY ", &joined_items);
			zval_ptr_dtor(&joined_items);
			DAO_MM_ADD_ENTRY(return_value);
		} else {
			DAO_SCONCAT_SV(return_value, " ORDER BY ", order);
			DAO_MM_ADD_ENTRY(return_value);
		}
		Z_TRY_ADDREF_P(return_value);
		RETURN_MM();
	}
	Z_TRY_ADDREF_P(return_value);
}

void dao_orm_phql_build_limit(zval *return_value, zval *limit) {

	zval number = {}, offset = {};

	if (DAO_IS_NOT_EMPTY(limit)) {
		DAO_MM_INIT();
		if (Z_TYPE_P(limit) == IS_ARRAY) {
			dao_array_fetch_str(&number, limit, SL("number"), PH_NOISY|PH_READONLY);
			if (dao_array_isset_fetch_str(&offset, limit, SL("offset"), PH_READONLY) && Z_TYPE(offset) != IS_NULL) {
				DAO_SCONCAT_SVSV(return_value, " LIMIT ", &number, " OFFSET ", &offset);
				DAO_MM_ADD_ENTRY(return_value);
			} else {
				DAO_SCONCAT_SV(return_value, " LIMIT ", &number);
				DAO_MM_ADD_ENTRY(return_value);
			}
		} else {
			DAO_SCONCAT_SV(return_value, " LIMIT ", limit);
			DAO_MM_ADD_ENTRY(return_value);
		}
		Z_TRY_ADDREF_P(return_value);
		RETURN_MM();
	}
	Z_TRY_ADDREF_P(return_value);
}
