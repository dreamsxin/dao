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

#include "mvc/model/query/builder.h"
#include "mvc/model/query/builderinterface.h"
#include "mvc/model/query/exception.h"
#include "mvc/model/query/builder/select.h"
#include "mvc/model/query/builder/update.h"
#include "mvc/model/query/builder/insert.h"
#include "mvc/model/query/builder/delete.h"
#include "mvc/model/metadatainterface.h"
#include "mvc/model/metadata/memory.h"
#include "mvc/model/query.h"
#include "mvc/model/query/scanner.h"
#include "di.h"
#include "diinterface.h"
#include "di/injectable.h"
#include "db/rawvalue.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/object.h"
#include "kernel/exception.h"
#include "kernel/fcall.h"
#include "kernel/concat.h"
#include "kernel/operators.h"
#include "kernel/string.h"
#include "kernel/file.h"
#include "kernel/hash.h"
#include "kernel/debug.h"
#include "kernel/framework/orm.h"

#include "interned-strings.h"

/**
 * Dao\Mvc\Model\Query\Builder
 *
 * Helps to create PHQL queries using an OO interface
 *
 *<code>
 *$resultset = Dao\Mvc\Model\Query\Builder::create(Dao\Mvc\Model\Query::TYPE_SELECT)
 *   ->from('Robots')
 *   ->join('RobotsParts')
 *   ->limit(20)
 *   ->orderBy('Robots.name')
 *   ->getQuery()
 *   ->execute();
 *</code>
 */
zend_class_entry *dao_mvc_model_query_builder_ce;

PHP_METHOD(Dao_Mvc_Model_Query_Builder, create);
PHP_METHOD(Dao_Mvc_Model_Query_Builder, createSelectBuilder);
PHP_METHOD(Dao_Mvc_Model_Query_Builder, createInsertBuilder);
PHP_METHOD(Dao_Mvc_Model_Query_Builder, createUpdateBuilder);
PHP_METHOD(Dao_Mvc_Model_Query_Builder, createDeleteBuilder);
PHP_METHOD(Dao_Mvc_Model_Query_Builder, getType);
PHP_METHOD(Dao_Mvc_Model_Query_Builder, setBindParams);
PHP_METHOD(Dao_Mvc_Model_Query_Builder, getBindParams);
PHP_METHOD(Dao_Mvc_Model_Query_Builder, getMergeBindParams);
PHP_METHOD(Dao_Mvc_Model_Query_Builder, setBindTypes);
PHP_METHOD(Dao_Mvc_Model_Query_Builder, getBindTypes);
PHP_METHOD(Dao_Mvc_Model_Query_Builder, getMergeBindTypes);
PHP_METHOD(Dao_Mvc_Model_Query_Builder, setIndex);
PHP_METHOD(Dao_Mvc_Model_Query_Builder, getIndex);
PHP_METHOD(Dao_Mvc_Model_Query_Builder, compile);
PHP_METHOD(Dao_Mvc_Model_Query_Builder, getPhql);
PHP_METHOD(Dao_Mvc_Model_Query_Builder, getQuery);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_query_builder_create, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, type, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_query_builder_createselectbuilder, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, params, IS_ARRAY, 1)
	ZEND_ARG_OBJ_INFO(0, dependencyInjector, Dao\\DiInterface, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_query_builder_setbindparams, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, bindparams, IS_ARRAY, 1)
	ZEND_ARG_TYPE_INFO(0, merge, _IS_BOOL, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_query_builder_setbindtypes, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, bindtypes, IS_ARRAY, 1)
	ZEND_ARG_TYPE_INFO(0, merge, _IS_BOOL, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_model_query_builder_setindex, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, index, IS_STRING, 0)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_mvc_model_query_builder_method_entry[] = {
	PHP_ME(Dao_Mvc_Model_Query_Builder, create, arginfo_dao_mvc_model_query_builder_create, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Mvc_Model_Query_Builder, createSelectBuilder, arginfo_dao_mvc_model_query_builder_createselectbuilder, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Mvc_Model_Query_Builder, createInsertBuilder, arginfo_dao_mvc_model_query_builder_createselectbuilder, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Mvc_Model_Query_Builder, createUpdateBuilder, arginfo_dao_mvc_model_query_builder_createselectbuilder, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Mvc_Model_Query_Builder, createDeleteBuilder, arginfo_dao_mvc_model_query_builder_createselectbuilder, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Mvc_Model_Query_Builder, getType, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Query_Builder, setBindParams, arginfo_dao_mvc_model_query_builder_setbindparams, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Query_Builder, getBindParams, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Query_Builder, getMergeBindParams, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Query_Builder, setBindTypes, arginfo_dao_mvc_model_query_builder_setbindtypes, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Query_Builder, getBindTypes, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Query_Builder, getMergeBindTypes, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Query_Builder, setIndex, arginfo_dao_mvc_model_query_builder_setindex, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Query_Builder, getIndex, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Query_Builder, compile, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Query_Builder, getPhql, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Model_Query_Builder, getQuery, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Mvc\Model\Query\Builder initializer
 */
DAO_INIT_CLASS(Dao_Mvc_Model_Query_Builder){

	DAO_REGISTER_CLASS_EX(Dao\\Mvc\\Model\\Query, Builder, mvc_model_query_builder, dao_di_injectable_ce, dao_mvc_model_query_builder_method_entry, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	zend_declare_property_null(dao_mvc_model_query_builder_ce, SL("_type"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_model_query_builder_ce, SL("_bindParams"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_model_query_builder_ce, SL("_bindTypes"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_model_query_builder_ce, SL("_mergeBindParams"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_model_query_builder_ce, SL("_mergeBindTypes"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_model_query_builder_ce, SL("_phql"), ZEND_ACC_PROTECTED);
	zend_declare_property_long(dao_mvc_model_query_builder_ce, SL("_hiddenParamNumber"), 0, ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_model_query_builder_ce, SL("_index"), ZEND_ACC_PROTECTED);

	zend_class_implements(dao_mvc_model_query_builder_ce, 1, dao_mvc_model_query_builderinterface_ce);

	return SUCCESS;
}

/**
 * Create a new Query Builder of the given type.
 *
 *<code>
 *	Dao\Mvc\Model\Query\Builder::create(Dao\Mvc\Model\Query::TYPE_SELECT);
 *</code>
 *
 * @return Dao\Mvc\Model\Query\Builder
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder, create){

	zval *type, *params = NULL, *_di = NULL, di = {}, service_name = {}, has = {}, exception_message = {};

	dao_fetch_params(1, 1, 2, &type, &params, &_di);

	if (!params) {
		params = &DAO_GLOBAL(z_null);
	}

	if (!_di) {
		_di = &DAO_GLOBAL(z_null);
	}

	if (Z_TYPE_P(_di) == IS_OBJECT) {
		DAO_MM_VERIFY_INTERFACE(_di, dao_diinterface_ce);
		ZVAL_COPY_VALUE(&di, _di);
	} else {
		DAO_MM_CALL_CE_STATIC(&di, dao_di_ce, "getdefault", _di);
		DAO_MM_ADD_ENTRY(&di);
	}

	switch (dao_get_intval(type)) {
		case PHQL_T_SELECT:
			ZVAL_STR(&service_name, IS(modelsQueryBuilderForSelect));
			DAO_MM_CALL_METHOD(&has, &di, "has", &service_name);
			if (zend_is_true(&has)) {
				DAO_MM_CALL_METHOD(return_value, &di, "get", &service_name);
			} else {
				object_init_ex(return_value, dao_mvc_model_query_builder_select_ce);
			}
			DAO_MM_CALL_METHOD(NULL, return_value, "__construct", params);
			break;

		case PHQL_T_INSERT:
			ZVAL_STR(&service_name, IS(modelsQueryBuilderForInsert));
			DAO_MM_CALL_METHOD(&has, &di, "has", &service_name);
			if (zend_is_true(&has)) {
				DAO_MM_CALL_METHOD(return_value, &di, "get", &service_name, params);
			} else {
				object_init_ex(return_value, dao_mvc_model_query_builder_insert_ce);
			}
			DAO_MM_CALL_METHOD(NULL, return_value, "__construct", params);
			break;

		case PHQL_T_UPDATE:
			ZVAL_STR(&service_name, IS(modelsQueryBuilderForUpdate));
			DAO_MM_CALL_METHOD(&has, &di, "has", &service_name);
			if (zend_is_true(&has)) {
				DAO_MM_CALL_METHOD(return_value, &di, "get", &service_name, params);
			} else {
				object_init_ex(return_value, dao_mvc_model_query_builder_delete_ce);
			}
			DAO_MM_CALL_METHOD(NULL, return_value, "__construct", params);
			break;

		case PHQL_T_DELETE:
			ZVAL_STR(&service_name, IS(modelsQueryBuilderForDelete));
			DAO_MM_CALL_METHOD(&has, &di, "has", &service_name);
			if (zend_is_true(&has)) {
				DAO_MM_CALL_METHOD(return_value, &di, "get", &service_name, params);
			} else {
				object_init_ex(return_value, dao_mvc_model_query_builder_delete_ce);
			}
			DAO_MM_CALL_METHOD(NULL, return_value, "__construct", params);
			break;

		default:
			DAO_CONCAT_SV(&exception_message, "Not found builder: ", type);
			DAO_MM_ADD_ENTRY(&exception_message);
			DAO_MM_THROW_EXCEPTION_ZVAL(dao_mvc_model_query_exception_ce, &exception_message);
			return;
	}

	DAO_MM_VERIFY_INTERFACE(return_value, dao_mvc_model_query_builderinterface_ce);
	RETURN_MM();
}

/**
 * Create a new Query Builder for Select
 *
 *
 * @return Dao\Mvc\Model\Query\Builder\Select
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder, createSelectBuilder){

	zval *params = NULL, *_di = NULL, di = {}, service_name = {};

	dao_fetch_params(1, 0, 2, &params, &_di);

	if (!params) {
		params = &DAO_GLOBAL(z_null);
	}

	if (!_di || Z_TYPE_P(_di) == IS_NULL) {
		DAO_MM_CALL_CE_STATIC(&di, dao_di_ce, "getdefault");
		DAO_MM_ADD_ENTRY(&di);
	} else if (Z_TYPE_P(_di) == IS_STRING) {
		DAO_MM_CALL_CE_STATIC(&di, dao_di_ce, "getdefault", _di);
		DAO_MM_ADD_ENTRY(&di);
	} else {
		DAO_MM_VERIFY_INTERFACE(_di, dao_diinterface_ce);
		ZVAL_COPY_VALUE(&di, _di);
	}

	ZVAL_STR(&service_name, IS(modelsQueryBuilderForSelect));
	DAO_MM_CALL_METHOD(return_value, &di, "get", &service_name, &DAO_GLOBAL(z_null), &DAO_GLOBAL(z_true));
	if (Z_TYPE_P(return_value) != IS_OBJECT) {
		zval_ptr_dtor(return_value);
		object_init_ex(return_value, dao_mvc_model_query_builder_select_ce);
		DAO_MM_CALL_METHOD(NULL, return_value, "__construct", params);
	}

	DAO_MM_VERIFY_INTERFACE(return_value, dao_mvc_model_query_builderinterface_ce);
	RETURN_MM();
}

/**
 * Create a new Query Builder for Insert
 *
 *
 * @return Dao\Mvc\Model\Query\Builder\Insert
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder, createInsertBuilder){
	zval *params = NULL, *_di = NULL, di = {}, service_name = {};

	dao_fetch_params(1, 0, 2, &params, &_di);

	if (!params) {
		params = &DAO_GLOBAL(z_null);
	}

	if (!_di || Z_TYPE_P(_di) == IS_NULL) {
		DAO_MM_CALL_CE_STATIC(&di, dao_di_ce, "getdefault");
		DAO_MM_ADD_ENTRY(&di);
	} else if (Z_TYPE_P(_di) == IS_STRING) {
		DAO_MM_CALL_CE_STATIC(&di, dao_di_ce, "getdefault", _di);
		DAO_MM_ADD_ENTRY(&di);
	} else {
		DAO_MM_VERIFY_INTERFACE(_di, dao_diinterface_ce);
		ZVAL_COPY_VALUE(&di, _di);
	}

	ZVAL_STR(&service_name, IS(modelsQueryBuilderForInsert));
	DAO_MM_CALL_METHOD(return_value, &di, "get", &service_name, &DAO_GLOBAL(z_null), &DAO_GLOBAL(z_true));
	if (Z_TYPE_P(return_value) != IS_OBJECT) {
		zval_ptr_dtor(return_value);
		object_init_ex(return_value, dao_mvc_model_query_builder_insert_ce);
		DAO_MM_CALL_METHOD(NULL, return_value, "__construct", params);
	}

	DAO_MM_VERIFY_INTERFACE(return_value, dao_mvc_model_query_builderinterface_ce);
	RETURN_MM();
}

/**
 * Create a new Query Builder for Update
 *
 *
 * @return Dao\Mvc\Model\Query\Builder\Update
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder, createUpdateBuilder){
	zval *params = NULL, *_di = NULL, di = {}, service_name = {};

	dao_fetch_params(1, 0, 2, &params, &_di);

	if (!params) {
		params = &DAO_GLOBAL(z_null);
	}

	if (!_di || Z_TYPE_P(_di) == IS_NULL) {
		DAO_MM_CALL_CE_STATIC(&di, dao_di_ce, "getdefault");
		DAO_MM_ADD_ENTRY(&di);
	} else if (Z_TYPE_P(_di) == IS_STRING) {
		DAO_MM_CALL_CE_STATIC(&di, dao_di_ce, "getdefault", _di);
		DAO_MM_ADD_ENTRY(&di);
	} else {
		DAO_MM_VERIFY_INTERFACE(_di, dao_diinterface_ce);
		ZVAL_COPY_VALUE(&di, _di);
	}

	ZVAL_STR(&service_name, IS(modelsQueryBuilderForUpdate));
	DAO_MM_CALL_METHOD(return_value, &di, "get", &service_name, &DAO_GLOBAL(z_null), &DAO_GLOBAL(z_true));
	if (Z_TYPE_P(return_value) != IS_OBJECT) {
		zval_ptr_dtor(return_value);
		object_init_ex(return_value, dao_mvc_model_query_builder_update_ce);
		DAO_MM_CALL_METHOD(NULL, return_value, "__construct", params);
	}

	DAO_MM_VERIFY_INTERFACE(return_value, dao_mvc_model_query_builderinterface_ce);
	RETURN_MM();
}

/**
 * Create a new Query Builder for Delete
 *
 *
 * @return Dao\Mvc\Model\Query\Builder\Delete
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder, createDeleteBuilder){
	zval *params = NULL, *_di = NULL, di = {}, service_name = {};

	dao_fetch_params(1, 0, 2, &params, &_di);

	if (!params) {
		params = &DAO_GLOBAL(z_null);
	}

	if (!_di || Z_TYPE_P(_di) == IS_NULL) {
		DAO_MM_CALL_CE_STATIC(&di, dao_di_ce, "getdefault");
		DAO_MM_ADD_ENTRY(&di);
	} else if (Z_TYPE_P(_di) == IS_STRING) {
		DAO_MM_CALL_CE_STATIC(&di, dao_di_ce, "getdefault", _di);
		DAO_MM_ADD_ENTRY(&di);
	} else {
		DAO_MM_VERIFY_INTERFACE(_di, dao_diinterface_ce);
		ZVAL_COPY_VALUE(&di, _di);
	}

	ZVAL_STR(&service_name, IS(modelsQueryBuilderForDelete));
	DAO_MM_CALL_METHOD(return_value, &di, "get", &service_name, &DAO_GLOBAL(z_null), &DAO_GLOBAL(z_true));
	if (Z_TYPE_P(return_value) != IS_OBJECT) {
		zval_ptr_dtor(return_value);
		object_init_ex(return_value, dao_mvc_model_query_builder_delete_ce);
		DAO_MM_CALL_METHOD(NULL, return_value, "__construct", params);
	}

	DAO_MM_VERIFY_INTERFACE(return_value, dao_mvc_model_query_builderinterface_ce);
	RETURN_MM();
}

/**
 * Gets the type of PHQL queries
 *
 *
 * @return int
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder, getType){

	RETURN_MEMBER(getThis(), "_type");
}

/**
 * Sets the bind parameters
 *
 * @param array $bindParams
 * @return Dao\Mvc\Model\Query\Builder
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder, setBindParams){

	zval *bind_params, *merge = NULL, current_bind_params = {}, merged_params = {};

	dao_fetch_params(0, 1, 1, &bind_params, &merge);

	if (!merge) {
		merge = &DAO_GLOBAL(z_true);
	}

	if (zend_is_true(merge)) {
		if (Z_TYPE_P(bind_params) == IS_ARRAY) {
			dao_read_property(&current_bind_params, getThis(), SL("_bindParams"), PH_NOISY|PH_READONLY);
			if (Z_TYPE(current_bind_params) == IS_ARRAY) {
				dao_add_function(&merged_params, bind_params, &current_bind_params);
			} else {
				ZVAL_COPY(&merged_params, bind_params);
			}

			dao_update_property(getThis(), SL("_bindParams"), &merged_params);
			zval_ptr_dtor(&merged_params);
		}
	} else {
		dao_update_property(getThis(), SL("_bindParams"), bind_params);
	}

	RETURN_THIS();
}

/**
 * Gets the bind parameters
 *
 * @return Dao\Mvc\Model\Query\Builder
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder, getBindParams){


	RETURN_MEMBER(getThis(), "_bindParams");
}

/**
 * Gets the merge bind parameters
 *
 * @return Dao\Mvc\Model\Query\Builder
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder, getMergeBindParams){


	RETURN_MEMBER(getThis(), "_mergeBindParams");
}

/**
 * Sets the bind types
 *
 * @param array $bindTypes
 * @return Dao\Mvc\Model\Query\Builder
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder, setBindTypes){

	zval *bind_types, *merge = NULL, current_bind_types = {}, merged_types = {};

	dao_fetch_params(0, 1, 1, &bind_types, &merge);

	if (!merge) {
		merge = &DAO_GLOBAL(z_true);
	}

	if (zend_is_true(merge)) {
		if (Z_TYPE_P(bind_types) == IS_ARRAY) {
			dao_read_property(&current_bind_types, getThis(), SL("_bindTypes"), PH_NOISY|PH_READONLY);
			if (Z_TYPE(current_bind_types) == IS_ARRAY) {
				dao_add_function(&merged_types, bind_types, &current_bind_types);
			} else {
				ZVAL_COPY(&merged_types, bind_types);
			}

			dao_update_property(getThis(), SL("_bindTypes"), &merged_types);
			zval_ptr_dtor(&merged_types);
		}
	} else {
		dao_update_property(getThis(), SL("_bindTypes"), bind_types);
	}

	RETURN_THIS();
}

/**
 * Gets the bind types
 *
 * @return Dao\Mvc\Model\Query\Builder
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder, getBindTypes){


	RETURN_MEMBER(getThis(), "_bindTypes");
}

/**
 * Gets the merge bind types
 *
 * @return Dao\Mvc\Model\Query\Builder
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder, getMergeBindTypes){


	RETURN_MEMBER(getThis(), "_mergeBindTypes");
}

/**
 * Adds the index
 *
 * @param string $index
 * @return Dao\Mvc\Model\Query\Builder
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder, setIndex) {

	zval *index;

	dao_fetch_params(0, 1, 0, &index);

	dao_update_property(getThis(), SL("_index"), index);
	RETURN_THIS();
}

/**
 * Gets the index
 *
 * @return string
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder, getIndex){


	RETURN_MEMBER(getThis(), "_index");
}

/**
 * Compile the PHQL query
 *
 * @return Dao\Mvc\Model\Query\Builder
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder, compile){

	DAO_CALL_METHOD(NULL, getThis(), "_compile");

	RETURN_THIS();
}

/**
 * Returns a PHQL statement built based on the builder parameters
 *
 * @return string
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder, getPhql){

	DAO_CALL_METHOD(NULL, getThis(), "compile");
	RETURN_MEMBER(getThis(), "_phql");
}

/**
 * Returns the query built
 *
 * @return Dao\Mvc\Model\Query
 */
PHP_METHOD(Dao_Mvc_Model_Query_Builder, getQuery){

	zval phql = {}, bind_params = {}, bind_types = {}, index = {}, dependency_injector = {}, service_name = {};
	zval cache = {}, has = {}, args = {};

	DAO_MM_INIT();
	/**
	 * Process the PHQL
	 */
	DAO_MM_CALL_METHOD(&phql, getThis(), "getphql");
	DAO_MM_ADD_ENTRY(&phql);
	DAO_MM_CALL_METHOD(&bind_params, getThis(), "getmergebindparams");
	DAO_MM_ADD_ENTRY(&bind_params);
	DAO_MM_CALL_METHOD(&bind_types, getThis(), "getmergebindtypes");
	DAO_MM_ADD_ENTRY(&bind_types);
	DAO_MM_CALL_METHOD(&index, getThis(), "getindex");
	DAO_MM_ADD_ENTRY(&index);

	DAO_MM_CALL_METHOD(&dependency_injector, getThis(), "getdi", &DAO_GLOBAL(z_true));
	DAO_MM_ADD_ENTRY(&dependency_injector);

	ZVAL_STR(&service_name, IS(modelsQuery));

	DAO_MM_CALL_METHOD(&has, &dependency_injector, "has", &service_name);

	if (zend_is_true(&has)) {
		array_init(&args);
		dao_array_append(&args, &phql, PH_COPY);
		dao_array_append(&args, &dependency_injector, PH_COPY);
		DAO_MM_ADD_ENTRY(&args);

		DAO_MM_CALL_METHOD(return_value, &dependency_injector, "get", &service_name, &args);
	} else {
		object_init_ex(return_value, dao_mvc_model_query_ce);
		DAO_MM_CALL_METHOD(NULL, return_value, "__construct", &phql, &dependency_injector);
	}

	if (dao_property_isset_fetch(&cache, getThis(), SL("_cache"), PH_READONLY)) {
		DAO_MM_CALL_METHOD(NULL, return_value, "cache", &cache);
	}

	/**
	 * Set default bind params
	 */
	if (Z_TYPE(bind_params) > IS_NULL) {
		DAO_MM_CALL_METHOD(NULL, return_value, "setbindparams", &bind_params);
	}

	/**
	 * Set default bind params
	 */
	if (Z_TYPE(bind_types) > IS_NULL) {
		DAO_MM_CALL_METHOD(NULL, return_value, "setbindtypes", &bind_types);
	}

	/**
	 * Set index
	 */
	if (Z_TYPE(index) > IS_NULL) {
		DAO_MM_CALL_METHOD(NULL, return_value, "setindex", &index);
	}
	RETURN_MM();
}
