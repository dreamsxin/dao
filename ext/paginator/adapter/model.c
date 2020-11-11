
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

#include "paginator/adapter/model.h"
#include "paginator/adapter.h"
#include "paginator/adapterinterface.h"
#include "paginator/exception.h"

#include <math.h>

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/array.h"
#include "kernel/operators.h"
#include "kernel/exception.h"
#include "kernel/fcall.h"

/**
 * Dao\Paginator\Adapter\Model
 *
 * This adapter allows to paginate data using a Dao\Mvc\Model resultset as base
 */
zend_class_entry *dao_paginator_adapter_model_ce;

PHP_METHOD(Dao_Paginator_Adapter_Model, __construct);
PHP_METHOD(Dao_Paginator_Adapter_Model, setCurrentPage);
PHP_METHOD(Dao_Paginator_Adapter_Model, getPaginate);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_paginator_adapter_model___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, config)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_paginator_adapter_model_method_entry[] = {
	PHP_ME(Dao_Paginator_Adapter_Model, __construct, arginfo_dao_paginator_adapter_model___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Paginator_Adapter_Model, getPaginate, arginfo_dao_paginator_adapterinterface_getpaginate, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Paginator\Adapter\Model initializer
 */
DAO_INIT_CLASS(Dao_Paginator_Adapter_Model){

	DAO_REGISTER_CLASS_EX(Dao\\Paginator\\Adapter, Model, paginator_adapter_model, dao_paginator_adapter_ce, dao_paginator_adapter_model_method_entry, 0);

	zend_declare_property_null(dao_paginator_adapter_model_ce, SL("_config"), ZEND_ACC_PROTECTED);

	zend_class_implements(dao_paginator_adapter_model_ce, 1, dao_paginator_adapterinterface_ce);

	return SUCCESS;
}

/**
 * Dao\Paginator\Adapter\Model constructor
 *
 * @param array $config
 */
PHP_METHOD(Dao_Paginator_Adapter_Model, __construct)
{
	zval *config, limit = {}, page = {};

	dao_fetch_params(0, 1, 0, &config);

	dao_update_property(getThis(), SL("_config"), config);

	if (dao_array_isset_fetch_str(&limit, config, SL("limit"), PH_READONLY)) {
		if (Z_TYPE(limit) != IS_LONG) {
			convert_to_long_ex(&limit);
		}
		dao_update_property(getThis(), SL("_limitRows"), &limit);
	}

	if (dao_array_isset_fetch_str(&page, config, SL("page"), PH_READONLY)) {
		if (Z_TYPE(page) != IS_LONG) {
			convert_to_long_ex(&page);
		}
		dao_update_property(getThis(), SL("_page"), &page);
	}
}

/**
 * Returns a slice of the resultset to show in the pagination
 *
 * @return \stdClass
 */
PHP_METHOD(Dao_Paginator_Adapter_Model, getPaginate){

	zval event_name = {}, show = {}, config = {}, items = {}, page_number = {}, rowcount = {}, page = {}, last_show_page = {}, start = {};
	zval possible_pages = {}, total_pages = {}, page_items = {}, maximum_pages = {}, next = {}, additional_page = {}, before = {}, remainder = {};
	zval pages_total = {};
	long int i, i_show;

	ZVAL_STRING(&event_name, "pagination:beforeGetPaginate");
	DAO_CALL_METHOD(NULL, getThis(), "fireevent", &event_name);
	zval_ptr_dtor(&event_name);

	dao_read_property(&show, getThis(), SL("_limitRows"), PH_READONLY);
	dao_read_property(&config, getThis(), SL("_config"), PH_READONLY);
	dao_read_property(&page_number, getThis(), SL("_page"), PH_READONLY);

	i_show = dao_get_intval(&show);

	dao_array_fetch_str(&items, &config, SL("data"), PH_NOISY|PH_READONLY);

	if (Z_TYPE(page_number) != IS_LONG || DAO_LT(&show, &DAO_GLOBAL(z_zero))) {
		ZVAL_COPY_VALUE(&page_number, &DAO_GLOBAL(z_one));
	}

	dao_fast_count(&rowcount, &items);

	object_init(&page);

	dao_sub_function(&last_show_page, &page_number, &DAO_GLOBAL(z_one));

	mul_function(&start, &show, &last_show_page);
	dao_div_function(&possible_pages, &rowcount, &show);

	if (unlikely(Z_TYPE(possible_pages)) != IS_DOUBLE) {
		convert_to_double(&possible_pages);
	}

	ZVAL_LONG(&total_pages, (long int)ceil(Z_DVAL(possible_pages)));
	if (Z_TYPE(items) != IS_OBJECT) {
		DAO_THROW_EXCEPTION_STR(dao_paginator_exception_ce, "Invalid data for paginator");
		return;
	}

	array_init(&page_items);
	if (DAO_GT(&rowcount, &DAO_GLOBAL(z_zero))) {
		/**
		 * Seek to the desired position
		 */
		if (DAO_LT(&start, &rowcount)) {
			DAO_CALL_METHOD(NULL, &items, "seek", &start);
		} else {
			DAO_CALL_METHOD(NULL, &items, "rewind");
			ZVAL_COPY_VALUE(&page_number, &DAO_GLOBAL(z_one));
			ZVAL_COPY_VALUE(&start, &DAO_GLOBAL(z_zero));
		}

		/**
		 * The record must be iterable
		 */
		for (i=1; ; ++i) {
			zval valid = {}, current = {};
			DAO_CALL_METHOD(&valid, &items, "valid");
			if (!DAO_IS_NOT_FALSE(&valid)) {
				break;
			}

			DAO_CALL_METHOD(&current, &items, "current");
			dao_array_append(&page_items, &current, 0);

			if (i >= i_show) {
				break;
			}
			DAO_CALL_METHOD(NULL, &items, "next");
		}
	}

	dao_update_property(&page, SL("items"), &page_items);
	zval_ptr_dtor(&page_items);

	dao_add_function(&maximum_pages, &start, &show);
	if (DAO_LT(&maximum_pages, &rowcount)) {
		dao_add_function(&next, &page_number, &DAO_GLOBAL(z_one));
	} else if (DAO_IS_EQUAL(&maximum_pages, &rowcount)) {
			ZVAL_COPY_VALUE(&next, &rowcount);
	} else {
		dao_div_function(&possible_pages, &rowcount, &show);

		dao_add_function(&additional_page, &possible_pages, &DAO_GLOBAL(z_one));

		ZVAL_LONG(&next, dao_get_intval(&additional_page));
	}

	if (DAO_GT(&next, &total_pages)) {
		ZVAL_COPY(&next, &total_pages);
	}

	dao_update_property(&page, SL("next"), &next);
	if (DAO_GT(&page_number, &DAO_GLOBAL(z_one))) {
		dao_sub_function(&before, &page_number, &DAO_GLOBAL(z_one));
	} else {
		ZVAL_COPY(&before, &DAO_GLOBAL(z_one));
	}

	dao_update_property(&page, SL("first"), &DAO_GLOBAL(z_one));
	dao_update_property(&page, SL("before"), &before);
	dao_update_property(&page, SL("current"), &page_number);

	mod_function(&remainder, &rowcount, &show);

	dao_div_function(&possible_pages, &rowcount, &show);
	if (!DAO_IS_LONG(&remainder, 0)) {
		dao_add_function(&next, &possible_pages, &DAO_GLOBAL(z_one));

		ZVAL_LONG(&pages_total, dao_get_intval(&next));
	} else {
		ZVAL_COPY(&pages_total, &possible_pages);
	}

	dao_update_property(&page, SL("last"), &pages_total);
	dao_update_property(&page, SL("total_pages"), &pages_total);
	dao_update_property(&page, SL("total_items"), &rowcount);

	ZVAL_STRING(&event_name, "pagination:afterGetPaginate");
	DAO_CALL_METHOD(return_value, getThis(), "fireevent", &event_name, &page);
	zval_ptr_dtor(&event_name);

	if (Z_TYPE_P(return_value) >= IS_ARRAY) {
		zval_ptr_dtor(&page);
	} else {
		zval_ptr_dtor(return_value);
		RETURN_NCTOR(&page);
	}
}
