
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

#include "paginator/adapter/dbbuilder.h"
#include "paginator/adapter.h"
#include "paginator/adapterinterface.h"
#include "paginator/exception.h"
#include "db/builderinterface.h"
#include "db.h"

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
 * Dao\Paginator\Adapter\DbBuilder
 *
 * Pagination using a PHQL query builder as source of data
 *
 *<code>
 *  $builder = Dao\Db\Builder::select('robots')
 *                   ->columns('id, name')
 *                   ->orderBy('name');
 *
 *  $paginator = new Dao\Paginator\Adapter\DbBuilder(array(
 *      "builder" => $builder,
 *      "limit"=> 20,
 *      "page" => 1
 *  ));
 *</code>
 */
zend_class_entry *dao_paginator_adapter_dbbuilder_ce;

PHP_METHOD(Dao_Paginator_Adapter_DbBuilder, __construct);
PHP_METHOD(Dao_Paginator_Adapter_DbBuilder, getPaginate);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_paginator_adapter_dbbuilder___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, config)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_paginator_adapter_dbbuilder_method_entry[] = {
	PHP_ME(Dao_Paginator_Adapter_DbBuilder, __construct, arginfo_dao_paginator_adapter_dbbuilder___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Paginator_Adapter_DbBuilder, getPaginate, arginfo_dao_paginator_adapterinterface_getpaginate, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Paginator\Adapter\DbBuilder initializer
 */
DAO_INIT_CLASS(Dao_Paginator_Adapter_DbBuilder){

	DAO_REGISTER_CLASS_EX(Dao\\Paginator\\Adapter, DbBuilder, paginator_adapter_dbbuilder, dao_paginator_adapter_ce, dao_paginator_adapter_dbbuilder_method_entry, 0);

	zend_declare_property_null(dao_paginator_adapter_dbbuilder_ce, SL("_builder"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_paginator_adapter_dbbuilder_ce, SL("_totalItems"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_paginator_adapter_dbbuilder_ce, SL("_limitRows"), ZEND_ACC_PROTECTED);

	zend_class_implements(dao_paginator_adapter_dbbuilder_ce, 1, dao_paginator_adapterinterface_ce);

	return SUCCESS;
}

/**
 * Dao\Paginator\Adapter\DbBuilder
 *
 * @param array $config
 */
PHP_METHOD(Dao_Paginator_Adapter_DbBuilder, __construct){

	zval *config, builder = {}, limit = {}, page = {}, total_items = {};
	long int i_limit;

	dao_fetch_params(0, 1, 0, &config);

	if (!dao_array_isset_fetch_str(&builder, config, SL("builder"), PH_READONLY)) {
		DAO_THROW_EXCEPTION_STR(dao_paginator_exception_ce, "Parameter 'builder' is required");
		return;
	}

	DAO_VERIFY_INTERFACE_EX(&builder, dao_db_builderinterface_ce, dao_paginator_exception_ce);

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
}

/**
 * Returns a slice of the resultset to show in the pagination
 *
 * @return \stdClass
 */
PHP_METHOD(Dao_Paginator_Adapter_DbBuilder, getPaginate){

	zval event_name = {}, builder = {}, limit = {}, number_page = {}, number = {}, fetch_mode = {}, items = {};
	zval row = {}, rowcount = {}, result = {}, page = {};
	ldiv_t tp;
	long int i_limit, i_number_page, i_number, i_before, i_rowcount;
	long int i_total_pages, i_next;

	DAO_MM_INIT();

	DAO_MM_ZVAL_STRING(&event_name, "pagination:beforeGetPaginate");
	DAO_MM_CALL_METHOD(NULL, getThis(), "fireevent", &event_name);

	dao_read_property(&builder, getThis(), SL("_builder"), PH_READONLY);

	dao_read_property(&limit, getThis(), SL("_limitRows"), PH_READONLY);
	dao_read_property(&number_page, getThis(), SL("_page"), PH_READONLY);

	dao_read_property(&rowcount, getThis(), SL("_totalItems"), PH_READONLY);
	if (Z_TYPE(rowcount) != IS_LONG) {
		DAO_MM_CALL_METHOD(&result, &builder, "count");
		DAO_MM_ADD_ENTRY(&result);

		DAO_MM_CALL_METHOD(&row, &result, "fetch");
		DAO_MM_ADD_ENTRY(&row);

		dao_array_fetch_str(&rowcount, &row, SL("rowcount"), PH_NOISY|PH_READONLY);
	}

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

	DAO_MM_CALL_METHOD(&result, &builder, "execute");
	DAO_MM_ADD_ENTRY(&result);

	dao_get_class_constant(&fetch_mode, dao_db_ce, SL("FETCH_OBJ"));
	DAO_MM_ADD_ENTRY(&fetch_mode);
	DAO_MM_CALL_METHOD(&items, &result, "fetchall", &fetch_mode);
	DAO_MM_ADD_ENTRY(&items);

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
