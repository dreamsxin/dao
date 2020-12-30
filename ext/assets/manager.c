
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

#include "assets/manager.h"
#include "assets/collection.h"
#include "assets/exception.h"
#include "assets/resource.h"
#include "assets/resource/css.h"
#include "assets/resource/js.h"

#include <ext/standard/file.h>

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/exception.h"
#include "kernel/fcall.h"
#include "kernel/array.h"
#include "kernel/operators.h"
#include "kernel/concat.h"
#include "kernel/string.h"
#include "kernel/file.h"
#include "kernel/debug.h"

/**
 * Dao\Assets\Manager
 *
 * Manages collections of CSS/Javascript assets
 */
zend_class_entry *dao_assets_manager_ce;

PHP_METHOD(Dao_Assets_Manager, __construct);
PHP_METHOD(Dao_Assets_Manager, setSourceBasePath);
PHP_METHOD(Dao_Assets_Manager, getSourceBasePath);
PHP_METHOD(Dao_Assets_Manager, setTargetBasePath);
PHP_METHOD(Dao_Assets_Manager, getTargetBasePath);
PHP_METHOD(Dao_Assets_Manager, setTargetBaseUri);
PHP_METHOD(Dao_Assets_Manager, getTargetBaseUri);
PHP_METHOD(Dao_Assets_Manager, useImplicitOutput);
PHP_METHOD(Dao_Assets_Manager, addCss);
PHP_METHOD(Dao_Assets_Manager, addJs);
PHP_METHOD(Dao_Assets_Manager, addResourceByType);
PHP_METHOD(Dao_Assets_Manager, addResource);
PHP_METHOD(Dao_Assets_Manager, set);
PHP_METHOD(Dao_Assets_Manager, get);
PHP_METHOD(Dao_Assets_Manager, getCss);
PHP_METHOD(Dao_Assets_Manager, getJs);
PHP_METHOD(Dao_Assets_Manager, collection);
PHP_METHOD(Dao_Assets_Manager, output);
PHP_METHOD(Dao_Assets_Manager, outputCss);
PHP_METHOD(Dao_Assets_Manager, outputJs);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_assets_manager___construct, 0, 0, 0)
	ZEND_ARG_INFO(0, options)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_assets_manager_setsourcebasepath, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, basePath, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_assets_manager_settargetbasepath, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, basePath, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_assets_manager_settargetbaseuri, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, baseUri, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_assets_manager_useimplicitoutput, 0, 0, 1)
	ZEND_ARG_INFO(0, implicitOutput)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_assets_manager_addcss, 0, 0, 1)
	ZEND_ARG_INFO(0, path)
	ZEND_ARG_INFO(0, local)
	ZEND_ARG_INFO(0, filter)
	ZEND_ARG_INFO(0, attributes)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_assets_manager_addjs, 0, 0, 1)
	ZEND_ARG_INFO(0, path)
	ZEND_ARG_INFO(0, local)
	ZEND_ARG_INFO(0, filter)
	ZEND_ARG_INFO(0, attributes)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_assets_manager_addresourcebytype, 0, 0, 2)
	ZEND_ARG_INFO(0, type)
	ZEND_ARG_INFO(0, resource)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_assets_manager_addresource, 0, 0, 1)
	ZEND_ARG_INFO(0, resource)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_assets_manager_set, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, id, IS_STRING, 0)
	ZEND_ARG_INFO(0, collection)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_assets_manager_get, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, id, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_assets_manager_collection, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_assets_manager_output, 0, 0, 2)
	ZEND_ARG_INFO(0, collection)
	ZEND_ARG_INFO(0, callback)
	ZEND_ARG_INFO(0, type)
	ZEND_ARG_INFO(0, args)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_assets_manager_outputcss, 0, 0, 0)
	ZEND_ARG_INFO(0, collectionName)
	ZEND_ARG_INFO(0, args)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_assets_manager_outputjs, 0, 0, 0)
	ZEND_ARG_INFO(0, collectionName)
	ZEND_ARG_INFO(0, args)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_assets_manager_method_entry[] = {
	PHP_ME(Dao_Assets_Manager, __construct, arginfo_dao_assets_manager___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Assets_Manager, setSourceBasePath, arginfo_dao_assets_manager_setsourcebasepath, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Assets_Manager, getSourceBasePath, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Assets_Manager, setTargetBasePath, arginfo_dao_assets_manager_settargetbasepath, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Assets_Manager, getTargetBasePath, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Assets_Manager, setTargetBaseUri, arginfo_dao_assets_manager_settargetbaseuri, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Assets_Manager, getTargetBaseUri, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Assets_Manager, useImplicitOutput, arginfo_dao_assets_manager_useimplicitoutput, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Assets_Manager, addCss, arginfo_dao_assets_manager_addcss, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Assets_Manager, addJs, arginfo_dao_assets_manager_addjs, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Assets_Manager, addResourceByType, arginfo_dao_assets_manager_addresourcebytype, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Assets_Manager, addResource, arginfo_dao_assets_manager_addresource, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Assets_Manager, set, arginfo_dao_assets_manager_set, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Assets_Manager, get, arginfo_dao_assets_manager_get, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Assets_Manager, getCss, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Assets_Manager, getJs, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Assets_Manager, collection, arginfo_dao_assets_manager_collection, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Assets_Manager, output, arginfo_dao_assets_manager_output, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Assets_Manager, outputCss, arginfo_dao_assets_manager_outputcss, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Assets_Manager, outputJs, arginfo_dao_assets_manager_outputjs, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Assets\Manager initializer
 */
DAO_INIT_CLASS(Dao_Assets_Manager){

	DAO_REGISTER_CLASS(Dao\\Assets, Manager, assets_manager, dao_assets_manager_method_entry, 0);

	zend_declare_property_null(dao_assets_manager_ce, SL("_collections"), ZEND_ACC_PROTECTED);
	zend_declare_property_bool(dao_assets_manager_ce, SL("_implicitOutput"), 1, ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_assets_manager_ce, SL("_sourceBasePath"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_assets_manager_ce, SL("_targetBasePath"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_assets_manager_ce, SL("_targetBaseUri"), ZEND_ACC_PROTECTED);

	return SUCCESS;
}

/**
 * Dao\Assets\Manager constructor
 *
 * @param array $options
 */
PHP_METHOD(Dao_Assets_Manager, __construct){

	zval *options = NULL;

	dao_fetch_params(0, 0, 1, &options);

	if (options && Z_TYPE_P(options) == IS_ARRAY) {
		zval base = {};
		zval ds_slash = {};
		char slash[2] = {DEFAULT_SLASH, 0};
		ZVAL_STRING(&ds_slash, slash);
		if (dao_array_isset_fetch_str(&base, options, SL("sourceBasePath"), PH_READONLY) && Z_TYPE(base) == IS_STRING) {
			zval fix = {};
			dao_fix_path(&fix, &base, &ds_slash);
			dao_update_property(getThis(), SL("_sourceBasePath"), &fix);
			zval_ptr_dtor(&fix);
		}
		if (dao_array_isset_fetch_str(&base, options, SL("targetBasePath"), PH_READONLY) && Z_TYPE(base) == IS_STRING) {
			zval fix = {};
			dao_fix_path(&fix, &base, &ds_slash);
			dao_update_property(getThis(), SL("_targetBasePath"), &fix);
			zval_ptr_dtor(&fix);
		}
		if (dao_array_isset_fetch_str(&base, options, SL("targetBaseUri"), PH_READONLY) && Z_TYPE(base) == IS_STRING) {
			dao_update_property(getThis(), SL("_targetBaseUri"), &base);
		}
		zval_ptr_dtor(&ds_slash);
	}
}

/**
 * Sets the source base path
 *
 * @param string $basePath
 * @return Dao\Assets\Manager
 */
PHP_METHOD(Dao_Assets_Manager, setSourceBasePath){

	zval *path;

	dao_fetch_params(0, 1, 0, &path);

	dao_update_property(getThis(), SL("_sourceBasePath"), path);

	RETURN_THIS();
}

/**
 * Returns the source base path
 *
 * @return string
 */
PHP_METHOD(Dao_Assets_Manager, getSourceBasePath){


	RETURN_MEMBER(getThis(), "_sourceBasePath");
}

/**
 * Sets the target base path
 *
 * @param string $basePath
 * @return Dao\Assets\Manager
 */
PHP_METHOD(Dao_Assets_Manager, setTargetBasePath){

	zval *path;

	dao_fetch_params(0, 1, 0, &path);

	dao_update_property(getThis(), SL("_targetBasePath"), path);

	RETURN_THIS();
}

/**
 * Returns the target base path
 *
 * @return string
 */
PHP_METHOD(Dao_Assets_Manager, getTargetBasePath){


	RETURN_MEMBER(getThis(), "_targetBasePath");
}

/**
 * Sets the target base uri
 *
 * @param string $baseUri
 * @return Dao\Assets\Manager
 */
PHP_METHOD(Dao_Assets_Manager, setTargetBaseUri){

	zval *uri;

	dao_fetch_params(0, 1, 0, &uri);

	dao_update_property(getThis(), SL("_targetBaseUri"), uri);

	RETURN_THIS();
}

/**
 * Returns the target base uri
 *
 * @return string
 */
PHP_METHOD(Dao_Assets_Manager, getTargetBaseUri){


	RETURN_MEMBER(getThis(), "_targetBaseUri");
}

/**
 * Returns the manager's options
 *
 * @return array
 */
PHP_METHOD(Dao_Assets_Manager, getOptions){


	RETURN_MEMBER(getThis(), "_options");
}

/**
 * Sets if the HTML generated must be directly printed or returned
 *
 * @param boolean $implicitOutput
 * @return Dao\Assets\Manager
 */
PHP_METHOD(Dao_Assets_Manager, useImplicitOutput){

	zval *implicit_output;

	dao_fetch_params(0, 1, 0, &implicit_output);

	dao_update_property(getThis(), SL("_implicitOutput"), implicit_output);
	RETURN_THIS();
}

/**
 * Adds a Css resource to the 'css' collection
 *
 *<code>
 *	$assets->addCss('css/bootstrap.css');
 *	$assets->addCss('http://bootstrap.my-cdn.com/style.css', false);
 *</code>
 *
 * @param string $path
 * @param boolean $local
 * @param boolean $filter
 * @param array $attributes
 * @return Dao\Assets\Manager
 */
PHP_METHOD(Dao_Assets_Manager, addCss){

	zval *path, *local = NULL, *filter = NULL, *attributes = NULL, type = {}, resource = {};

	dao_fetch_params(0, 1, 3, &path, &local, &filter, &attributes);

	if (!local) {
		local = &DAO_GLOBAL(z_true);
	}

	if (!filter) {
		filter = &DAO_GLOBAL(z_true);
	}

	if (!attributes) {
		attributes = &DAO_GLOBAL(z_null);
	}

	ZVAL_STRING(&type, "css");

	object_init_ex(&resource, dao_assets_resource_css_ce);

	DAO_CALL_METHOD(NULL, &resource, "__construct", path, local, filter, attributes);
	DAO_CALL_METHOD(NULL, getThis(), "addresourcebytype", &type, &resource);
	zval_ptr_dtor(&type);
	zval_ptr_dtor(&resource);

	RETURN_THIS();
}

/**
 * Adds a javascript resource to the 'js' collection
 *
 *<code>
 *	$assets->addJs('scripts/jquery.js');
 *	$assets->addJs('http://jquery.my-cdn.com/jquery.js', true);
 *</code>
 *
 * @param string $path
 * @param boolean $local
 * @param boolean $filter
 * @param array $attributes
 * @return Dao\Assets\Manager
 */
PHP_METHOD(Dao_Assets_Manager, addJs){

	zval *path, *local = NULL, *filter = NULL, *attributes = NULL, type = {}, resource = {};

	dao_fetch_params(0, 1, 3, &path, &local, &filter, &attributes);

	if (!local) {
		local = &DAO_GLOBAL(z_true);
	}

	if (!filter) {
		filter = &DAO_GLOBAL(z_true);
	}

	if (!attributes) {
		attributes = &DAO_GLOBAL(z_null);
	}

	ZVAL_STRING(&type, "js");

	object_init_ex(&resource, dao_assets_resource_js_ce);

	DAO_CALL_METHOD(NULL, &resource, "__construct", path, local, filter, attributes);
	DAO_CALL_METHOD(NULL, getThis(), "addresourcebytype", &type, &resource);
	zval_ptr_dtor(&type);
	zval_ptr_dtor(&resource);

	RETURN_THIS();
}

/**
 * Adds a resource by its type
 *
 *<code>
 *	$assets->addResourceByType('css', new Dao\Assets\Resource\Css('css/style.css'));
 *</code>
 *
 * @param string $type
 * @param Dao\Assets\Resource $resource
 * @return Dao\Assets\Manager
 */
PHP_METHOD(Dao_Assets_Manager, addResourceByType){

	zval *type, *resource, collections = {}, collection = {};

	dao_fetch_params(0, 2, 0, &type, &resource);

	dao_read_property(&collections, getThis(), SL("_collections"), PH_NOISY|PH_READONLY);
	if (!dao_array_isset_fetch(&collection, &collections, type, PH_COPY)) {
		object_init_ex(&collection, dao_assets_collection_ce);
		dao_update_property_array(getThis(), SL("_collections"), type, &collection);
	}

	/**
	 * Add the resource to the collection
	 */
	DAO_CALL_METHOD(NULL, &collection, "add", resource);
	zval_ptr_dtor(&collection);

	RETURN_THIS();
}

/**
 * Adds a raw resource to the manager
 *
 *<code>
 * $assets->addResource(new Dao\Assets\Resource('css', 'css/style.css'));
 *</code>
 *
 * @param Dao\Assets\Resource $resource
 * @return Dao\Assets\Manager
 */
PHP_METHOD(Dao_Assets_Manager, addResource){

	zval *resource, type = {};

	dao_fetch_params(0, 1, 0, &resource);
	DAO_VERIFY_CLASS_EX(resource, dao_assets_resource_ce, dao_assets_exception_ce);

	DAO_CALL_METHOD(&type, resource, "gettype");

	/**
	 * Adds the resource by its type
	 */
	DAO_CALL_METHOD(NULL, getThis(), "addresourcebytype", &type, resource);
	zval_ptr_dtor(&type);

	RETURN_THIS();
}

/**
 * Sets a collection in the Assets Manager
 *
 *<code>
 * $assets->get('js', $collection);
 *</code>
 *
 * @param string $id
 * @param Dao\Assets\Collection $collection
 * @return Dao\Assets\Manager
 */
PHP_METHOD(Dao_Assets_Manager, set){

	zval *id, *collection;

	dao_fetch_params(0, 2, 0, &id, &collection);

	if (unlikely(Z_TYPE_P(collection) != IS_OBJECT)) {
		DAO_THROW_EXCEPTION_STR(dao_assets_exception_ce, "Collection must be an object");
		return;
	}

	dao_update_property_array(getThis(), SL("_collections"), id, collection);

	RETURN_THIS();
}

/**
 * Returns a collection by its id
 *
 *<code>
 * $scripts = $assets->get('js');
 *</code>
 *
 * @param string $id
 * @return Dao\Assets\Collection
 */
PHP_METHOD(Dao_Assets_Manager, get){

	zval *id, collections = {};

	dao_fetch_params(0, 1, 0, &id);

	dao_read_property(&collections, getThis(), SL("_collections"), PH_NOISY|PH_READONLY);
	if (!dao_array_isset_fetch(return_value, &collections, id, PH_COPY)) {
		DAO_THROW_EXCEPTION_STR(dao_assets_exception_ce, "The collection does not exist in the manager");
		return;
	}
}

/**
 * Returns the CSS collection of assets
 *
 * @return Dao\Assets\Collection
 */
PHP_METHOD(Dao_Assets_Manager, getCss){

	zval collections = {};

	dao_read_property(&collections, getThis(), SL("_collections"), PH_NOISY|PH_READONLY);

	/**
	 * Check if the collection does not exist and create an implicit collection
	 */
	if (!dao_array_isset_fetch_str(return_value, &collections, SL("css"), PH_COPY)) {
		object_init_ex(return_value, dao_assets_collection_ce);
	}
}

/**
 * Returns the CSS collection of assets
 *
 * @return Dao\Assets\Collection
 */
PHP_METHOD(Dao_Assets_Manager, getJs){

	zval collections = {};

	dao_read_property(&collections, getThis(), SL("_collections"), PH_NOISY|PH_READONLY);

	/**
	 * Check if the collection does not exist and create an implicit collection
	 */
	if (!dao_array_isset_fetch_str(return_value, &collections, SL("js"), PH_COPY)) {
		object_init_ex(return_value, dao_assets_collection_ce);
	}
}

/**
 * Creates/Returns a collection of resources
 *
 * @param string $name
 * @return Dao\Assets\Collection
 */
PHP_METHOD(Dao_Assets_Manager, collection){

	zval *name, collections = {};

	dao_fetch_params(0, 1, 0, &name);

	dao_read_property(&collections, getThis(), SL("_collections"), PH_NOISY|PH_READONLY);
	if (!dao_array_isset_fetch(return_value, &collections, name, PH_COPY)) {
		object_init_ex(return_value, dao_assets_collection_ce);
		dao_update_property_array(getThis(), SL("_collections"), name, return_value);
	}
}

/**
 * Traverses a collection calling the callback to generate its HTML
 *
 * @param Dao\Assets\Collection $collection
 * @param callback $callback
 * @param string $type
 * @param array $args
 */
PHP_METHOD(Dao_Assets_Manager, output){

	zval *collection, *callback, *z_type = NULL, *args = NULL, type = {}, output = {}, use_implicit_output = {}, exception_message = {};
	zval options = {}, collection_source_path = {}, collection_target_path = {}, base_uri = {}, changed = {};
	zval resources = {}, filters = {}, prefix = {}, type_css = {}, source_base_path = {}, target_base_path = {};
	zval complete_target_base_uri = {}, complete_source_path = {}, complete_target_path = {}, complete_target_dir = {}, join = {};
	zval *resource, filtered_joined_content = {};
	zval ds_slash = {}, tmp = {};
	char slash[2] = {DEFAULT_SLASH, 0};
	zend_string *ret;

	dao_fetch_params(1, 2, 2, &collection, &callback, &z_type, &args);

	if (z_type) {
		ZVAL_COPY_VALUE(&type, z_type);
	}

	if (!args) {
		args = &DAO_GLOBAL(z_null);
	}

	DAO_MM_ZVAL_STRING(&ds_slash, slash);

	dao_read_property(&use_implicit_output, getThis(), SL("_implicitOutput"), PH_READONLY);

	/**
	 * Get the resources as an array
	 */
	DAO_MM_CALL_METHOD(&resources, collection, "getresources", &type);
	DAO_MM_ADD_ENTRY(&resources);

	/**
	 * Get filters in the collection
	 */
	DAO_MM_CALL_METHOD(&filters, collection, "getfilters");
	DAO_MM_ADD_ENTRY(&filters);

	/**
	 * Get the collection's prefix
	 */
	DAO_MM_CALL_METHOD(&prefix, collection, "getprefix");
	DAO_MM_ADD_ENTRY(&prefix);

	DAO_MM_ZVAL_STRING(&type_css, "css");

	dao_read_property(&options, getThis(), SL("_options"), PH_READONLY);

	dao_read_property(&source_base_path, getThis(), SL("_sourceBasePath"), PH_NOISY|PH_READONLY);
	dao_read_property(&target_base_path, getThis(), SL("_targetBasePath"), PH_NOISY|PH_READONLY);
	dao_read_property(&base_uri, getThis(), SL("_targetBaseUri"), PH_NOISY|PH_READONLY);

	DAO_CONCAT_VV(&complete_target_base_uri, &base_uri, &prefix);
	DAO_MM_ADD_ENTRY(&complete_target_base_uri);

	/**
	 * Check if the collection have its own source base path
	 */
	DAO_MM_CALL_METHOD(&collection_source_path, collection, "getsourcepath");
	DAO_MM_ADD_ENTRY(&collection_source_path);

	/**
	 * Concatenate the global base source path with the collection one
	 */
	if (DAO_IS_NOT_EMPTY(&collection_source_path)) {
		DAO_CONCAT_VV(&complete_source_path, &source_base_path, &collection_source_path);
		DAO_MM_ADD_ENTRY(&complete_source_path);
	} else {
		ZVAL_COPY_VALUE(&complete_source_path, &source_base_path);
	}

	/**
	 * Check if the collection have its own target base path
	 */
	DAO_MM_CALL_METHOD(&collection_target_path, collection, "gettargetpath");
	DAO_MM_ADD_ENTRY(&collection_target_path);

	/**
	 * Concatenate the global base source path with the collection one
	 */
	if (DAO_IS_NOT_EMPTY(&collection_target_path)) {
		DAO_CONCAT_VV(&complete_target_path, &target_base_path, &collection_target_path);
		DAO_MM_ADD_ENTRY(&complete_target_path);
	} else {
		ZVAL_COPY_VALUE(&complete_target_path, &target_base_path);
	}

	/**
	 * Check if all the resources in the collection must be joined
	 */
	DAO_MM_CALL_METHOD(&join, collection, "getjoin");

	/**
	 * The targetpath needs to be a valid file
	 */
	if (zend_is_true(&join)) {
		/**
		 * We need a valid final target path
		 */
		if (DAO_IS_EMPTY(&complete_target_path)) {
			DAO_CONCAT_SVS(&exception_message, "Path '", &complete_target_path, "' is not a valid target path (1)");
			DAO_MM_ADD_ENTRY(&exception_message);
			DAO_MM_THROW_EXCEPTION_ZVAL(dao_assets_exception_ce, &exception_message);
			return;
		}

		if (dao_is_dir(&complete_target_path)) {
			DAO_CONCAT_SVS(&exception_message, "Path '", &complete_target_path, "' is not a valid target path (2)");
			DAO_MM_ADD_ENTRY(&exception_message);
			DAO_MM_THROW_EXCEPTION_ZVAL(dao_assets_exception_ce, &exception_message);
			return;
		}

		if (dao_file_exists(&complete_target_path) != SUCCESS) {
			ZVAL_TRUE(&changed);
		}

		ret = zend_string_init(Z_STRVAL(complete_target_path), Z_STRLEN(complete_target_path), 0);
		ZSTR_LEN(ret) = zend_dirname(ZSTR_VAL(ret), ZSTR_LEN(ret));
		ZVAL_STR(&tmp, ret);
		dao_fix_path(&complete_target_dir, &tmp, &ds_slash);
		zval_ptr_dtor(&tmp);
		DAO_MM_ADD_ENTRY(&complete_target_dir);
	} else {
		ZVAL_NULL(&complete_target_dir);
	}

	ZEND_HASH_FOREACH_VAL(Z_ARRVAL(resources), resource) {
		zval filter_needed = {}, local = {}, must_filter = {}, *filter, target_path = {};
		zval content = {}, filtered_content = {};

		ZVAL_FALSE(&filter_needed);

		if (Z_TYPE(type) == IS_NULL) {
			DAO_MM_CALL_METHOD(&type, resource, "gettype");
			DAO_MM_ADD_ENTRY(&type);
		}

		/**
		 * Is the resource local?
		 */
		DAO_MM_CALL_METHOD(&local, resource, "getlocal");

		/**
		 * If the collection must not be joined we must print a HTML for each one
		 */
		if (zend_is_true(&join) || Z_TYPE(filters) == IS_ARRAY) {
			zval source_path = {};
			/**
			 * Get the complete source path
			 */
			DAO_MM_CALL_METHOD(&source_path, resource, "getrealsourcepath", &complete_source_path);
			DAO_MM_ADD_ENTRY(&source_path);
			if (zend_is_true(&local)) {
				/**
				 * We need a valid source path
				 */
				if (!zend_is_true(&source_path)) {
					DAO_MM_CALL_METHOD(&source_path, resource, "getpath");
					DAO_MM_ADD_ENTRY(&source_path);
					DAO_CONCAT_SVS(&exception_message, "Resource '", &source_path, "' does not have a valid source path");
					DAO_MM_ADD_ENTRY(&exception_message);
					DAO_MM_THROW_EXCEPTION_ZVAL(dao_assets_exception_ce, &exception_message);
					return;
				}
			} else {
				/**
				 * resources paths are always filtered
				 */
				ZVAL_TRUE(&changed);
				ZVAL_TRUE(&filter_needed);
			}

			if (zend_is_true(&join)) {
				if (dao_file_exists(&complete_target_path) == SUCCESS) {
					if (!dao_compare_mtime(&complete_target_path, &source_path)) {
						ZVAL_TRUE(&filter_needed);
						ZVAL_TRUE(&changed);
					}
				} else {
					ZVAL_TRUE(&changed);
					ZVAL_TRUE(&filter_needed);
				}
			}

			/**
			 * Get the target path, we need to write the filtered content to a file
			 */
			DAO_MM_CALL_METHOD(&target_path, resource, "getrealtargetpath", &complete_target_dir);
			DAO_MM_ADD_ENTRY(&target_path);

			/**
			 * We need a valid final target path
			 */
			if (!zend_is_true(&target_path)) {
				DAO_CONCAT_SVS(&exception_message, "Resource '", &target_path, "' does not have a valid target path");
				DAO_MM_ADD_ENTRY(&exception_message);
				DAO_MM_THROW_EXCEPTION_ZVAL(dao_assets_exception_ce, &exception_message);
				return;
			}

			if (zend_is_true(&local)) {
				/**
				 * Make sure the target path is not the same source path
				 */
				if (DAO_IS_EQUAL(&target_path, &source_path)) {
					DAO_CONCAT_SVS(&exception_message, "Resource '", &target_path, "' have the same source and target paths");
					DAO_MM_ADD_ENTRY(&exception_message);
					DAO_MM_THROW_EXCEPTION_ZVAL(dao_assets_exception_ce, &exception_message);
					return;
				}
				if (dao_file_exists(&target_path) == SUCCESS) {
					if (!dao_compare_mtime(&target_path, &source_path)) {
						ZVAL_TRUE(&filter_needed);
						ZVAL_TRUE(&changed);
					}
				} else {
					ZVAL_TRUE(&changed);
					ZVAL_TRUE(&filter_needed);
				}
			}
		} else {
			zval prefixed_path = {}, attributes = {}, parameters = {}, html = {};
			/**
			 * If there are no filters, just print/buffer the HTML
			 */
			DAO_MM_CALL_METHOD(&prefixed_path, resource, "getrealtargeturi", &complete_target_base_uri);
			DAO_MM_ADD_ENTRY(&prefixed_path);

			/**
			 * Gets extra HTML attributes in the resource
			 */
			DAO_MM_CALL_METHOD(&attributes, resource, "getattributes");
			DAO_MM_ADD_ENTRY(&attributes);

			/**
			 * Prepare the parameters for the callback
			 */
			array_init_size(&parameters, 3);
			DAO_MM_ADD_ENTRY(&parameters);
			if (Z_TYPE(attributes) == IS_ARRAY) {
				dao_array_update_long(&attributes, 0, &prefixed_path, PH_COPY);

				dao_array_append(&parameters, &attributes, PH_COPY);
			} else {
				dao_array_append(&parameters, &prefixed_path, PH_COPY);
			}

			dao_array_append(&parameters, &local, PH_COPY);
			dao_array_append(&parameters, args, PH_COPY);

			/**
			 * Call the callback to generate the HTML
			 */
			DAO_MM_CALL_USER_FUNC_ARRAY(&html, callback, &parameters);
			DAO_MM_ADD_ENTRY(&html);

			/**
			 * Implicit output prints the content directly
			 */
			if (zend_is_true(&use_implicit_output)) {
				zend_print_zval(&html, 0);
			} else {
				if (Z_TYPE(output) <= IS_NULL) {
					DAO_MM_ZVAL_COPY(&output, &html);
				} else {
					zval tmp = {};
					DAO_CONCAT_VV(&tmp, &output, &html);
					DAO_MM_ADD_ENTRY(&tmp);
					ZVAL_COPY_VALUE(&output, &tmp);
				}
				
			}

			continue;
		}

		if (zend_is_true(&filter_needed)) {
			/**
			 * Get the resource's content
			 */
			DAO_MM_CALL_METHOD(&content, resource, "getcontent", &complete_source_path);
			DAO_MM_ADD_ENTRY(&content);

			DAO_MM_ZVAL_COPY(&filtered_content, &content);

			/**
			 * Check if the resource must be filtered
			 */
			DAO_MM_CALL_METHOD(&must_filter, resource, "getfilter");

			/**
			 * Only filter the resource if it's marked as 'filterable'
			 */
			if (zend_is_true(&must_filter) && Z_TYPE(filters) == IS_ARRAY) {
				ZEND_HASH_FOREACH_VAL(Z_ARRVAL(filters), filter) {
					/**
					 * Filters must be valid objects
					 */
					if (Z_TYPE_P(filter) != IS_OBJECT) {
						DAO_MM_THROW_EXCEPTION_STR(dao_assets_exception_ce, "Filter is invalid");
						return;
					}

					/**
					 * Calls the method 'filter' which must return a filtered version of the content
					 */
					DAO_MM_CALL_METHOD(&filtered_content, filter, "filter", &filtered_content);
					DAO_MM_ADD_ENTRY(&filtered_content);
				} ZEND_HASH_FOREACH_END();
			}
		} else {
			dao_file_get_contents(&filtered_content, &target_path);
			if (DAO_IS_FALSE(&filtered_content)) {
				DAO_CONCAT_SVS(&exception_message, "Resource's content for \"", &target_path, "\" cannot be read");
				DAO_MM_ADD_ENTRY(&exception_message);
				DAO_MM_THROW_EXCEPTION_ZVAL(dao_assets_exception_ce, &exception_message);
				return;
			}
			DAO_MM_ADD_ENTRY(&filtered_content);
		}

		/**
		 * Update the joined filtered content
		 */
		if (zend_is_true(&join)) {
			if (DAO_IS_EQUAL(&type, &type_css)) {
				if (Z_TYPE(filtered_joined_content) <= IS_NULL) {
					DAO_MM_ZVAL_COPY(&filtered_joined_content, &filtered_content);
				} else {
					zval tmp = {};
					DAO_CONCAT_VV(&tmp, &filtered_joined_content, &filtered_content);
					DAO_MM_ADD_ENTRY(&tmp);
					ZVAL_COPY_VALUE(&filtered_joined_content, &tmp);
				}
			} else {
				if (Z_TYPE(filtered_joined_content) <= IS_NULL) {
					DAO_CONCAT_VS(&filtered_joined_content, &filtered_content, ";");
					DAO_MM_ADD_ENTRY(&filtered_joined_content);
				} else {
					zval tmp = {};
					DAO_CONCAT_VVS(&tmp, &filtered_joined_content, &filtered_content, ";");
					DAO_MM_ADD_ENTRY(&tmp);
					ZVAL_COPY_VALUE(&filtered_joined_content, &tmp);
				}
			}
		}

		if (zend_is_true(&filter_needed)) {
			zend_string *ret;
			zval path = {};

			ret = zend_string_init(Z_STRVAL(target_path), Z_STRLEN(target_path), 0);
			ZSTR_LEN(ret) = zend_dirname(ZSTR_VAL(ret), ZSTR_LEN(ret));
			ZVAL_STR(&path, ret);
			if (!dao_is_dir(&path)) {
				zend_long mode = 0777;
				zend_bool recursive = 1;
				php_stream_context *context;
				context = php_stream_context_from_zval(NULL, 0);
				if (!php_stream_mkdir(ZSTR_VAL(ret), (int)mode, 
					(recursive ? PHP_STREAM_MKDIR_RECURSIVE : 0) | REPORT_ERRORS, context)) {
					zend_string_free(ret);
					RETURN_MM();
				}
			}

			zend_string_free(ret);

			/**
			 * Write the file using file-put-contents. This respects the openbase-dir also
			 * writes to streams
			 */
			dao_file_put_contents(NULL, &target_path, &filtered_content);
		}

		if (!zend_is_true(&join)) {
			zval prefixed_path = {}, attributes = {}, parameters = {}, html = {};
			/**
			 * Generate the HTML using the original path in the resource
			 */
			DAO_MM_CALL_METHOD(&prefixed_path, resource, "getrealtargeturi", &complete_target_base_uri);
			DAO_MM_ADD_ENTRY(&prefixed_path);

			/**
			 * Gets extra HTML attributes in the resource
			 */
			DAO_MM_CALL_METHOD(&attributes, resource, "getattributes");
			DAO_MM_ADD_ENTRY(&attributes);

			/**
			 * Filtered resources are always local
			 */
			ZVAL_TRUE(&local);

			/**
			 * Prepare the parameters for the callback
			 */
			array_init_size(&parameters, 3);
			DAO_MM_ADD_ENTRY(&parameters);
			if (Z_TYPE(attributes) == IS_ARRAY) {
				dao_array_update_long(&attributes, 0, &prefixed_path, PH_COPY);

				dao_array_append(&parameters, &attributes, PH_COPY);
			} else {
				dao_array_append(&parameters, &prefixed_path, PH_COPY);
			}

			dao_array_append(&parameters, &local, PH_COPY);
			dao_array_append(&parameters, args, PH_COPY);

			/**
			 * Call the callback to generate the HTML
			 */
			DAO_MM_CALL_USER_FUNC_ARRAY(&html, callback, &parameters);
			DAO_MM_ADD_ENTRY(&html);

			/**
			 * Implicit output prints the content directly
			 */
			if (zend_is_true(&use_implicit_output)) {
				zend_print_zval(&html, 0);
			} else {
				if (Z_TYPE(output) <= IS_NULL) {
					ZVAL_STRINGL(&output, Z_STRVAL(html), Z_STRLEN(html));
				} else {
					dao_concat_self(&output, &html);
				}
				DAO_MM_ADD_ENTRY(&output);
			}
		}
	} ZEND_HASH_FOREACH_END();

	if (zend_is_true(&join)) {
		zval target_uri = {}, prefixed_path = {}, attributes = {}, local = {}, parameters = {}, html = {};
		if (zend_is_true(&changed)) {
			/**
			 * Write the file using file_put_contents. This respects the openbase-dir also
			 * writes to streams
			 */
			dao_file_put_contents(NULL, &complete_target_path, &filtered_joined_content);
		}

		DAO_MM_CALL_METHOD(&target_uri, collection, "gettargeturi");
		DAO_MM_ADD_ENTRY(&target_uri);
		if (DAO_IS_NOT_EMPTY(&type) && DAO_IS_NOT_EMPTY(&target_uri) && !dao_fast_strpos_str(NULL, &target_uri, SL("."))) {
			DAO_SCONCAT_SV(&target_uri, ".", &type);
		}

		/**
		 * Generate the HTML using the original path in the resource
		 */
		DAO_CONCAT_VV(&prefixed_path, &complete_target_base_uri, &target_uri);
		DAO_MM_ADD_ENTRY(&prefixed_path);

		/**
		 * Gets extra HTML attributes in the resource
		 */
		DAO_MM_CALL_METHOD(&attributes, collection, "getattributes");
		DAO_MM_ADD_ENTRY(&attributes);
		DAO_MM_CALL_METHOD(&local, collection, "gettargetlocal");
		DAO_MM_ADD_ENTRY(&local);

		/**
		 * Prepare the parameters for the callback
		 */
		array_init_size(&parameters, 3);
		DAO_MM_ADD_ENTRY(&parameters);
		if (Z_TYPE(attributes) == IS_ARRAY) {
			dao_array_update_long(&attributes, 0, &prefixed_path, PH_COPY);

			dao_array_append(&parameters, &attributes, PH_COPY);
		} else {
			dao_array_append(&parameters, &prefixed_path, PH_COPY);
		}

		dao_array_append(&parameters, &local, PH_COPY);
		dao_array_append(&parameters, args, PH_COPY);

		/**
		 * Call the callback to generate the HTML
		 */
		DAO_MM_CALL_USER_FUNC_ARRAY(&html, callback, &parameters);
		DAO_MM_ADD_ENTRY(&html);

		/**
		 * Implicit output prints the content directly
		 */
		if (zend_is_true(&use_implicit_output)) {
			zend_print_zval(&html, 0);
		} else {
			ZVAL_COPY_VALUE(&output, &html);
		}
	}

	RETURN_MM_CTOR(&output);
}

/**
 * Prints the HTML for CSS resources
 *
 * @param string $collectionName
 * @param array $args
 */
PHP_METHOD(Dao_Assets_Manager, outputCss){

	zval *collection_name = NULL, *args = NULL, collection = {}, callback = {}, type = {};

	dao_fetch_params(1, 0, 2, &collection_name, &args);

	if (!collection_name) {
		collection_name = &DAO_GLOBAL(z_null);
	}

	if (!args) {
		args = &DAO_GLOBAL(z_null);
	}

	if (DAO_IS_EMPTY(collection_name)) {
		DAO_MM_CALL_METHOD(&collection, getThis(), "getcss");
	} else {
		DAO_MM_CALL_METHOD(&collection, getThis(), "get", collection_name);
	}
	DAO_MM_ADD_ENTRY(&collection);

	array_init_size(&callback, 2);
	add_next_index_stringl(&callback, SL("Dao\\Tag"));
	add_next_index_stringl(&callback, SL("stylesheetLink"));

	DAO_MM_ADD_ENTRY(&callback);

	DAO_MM_ZVAL_STRING(&type, "css");

	DAO_MM_CALL_METHOD(return_value, getThis(), "output", &collection, &callback, &type, args);
	RETURN_MM();
}

/**
 * Prints the HTML for JS resources
 *
 * @param string $collectionName
 * @param array $args
 */
PHP_METHOD(Dao_Assets_Manager, outputJs){

	zval *collection_name = NULL, *args = NULL, collection = {}, callback = {}, type = {};

	dao_fetch_params(1, 0, 2, &collection_name, &args);

	if (!collection_name) {
		collection_name = &DAO_GLOBAL(z_null);
	}

	if (!args) {
		args = &DAO_GLOBAL(z_null);
	}

	if (DAO_IS_EMPTY(collection_name)) {
		DAO_MM_CALL_METHOD(&collection, getThis(), "getjs");
	} else {
		DAO_MM_CALL_METHOD(&collection, getThis(), "get", collection_name);
	}
	DAO_MM_ADD_ENTRY(&collection);

	array_init_size(&callback, 2);
	add_next_index_stringl(&callback, SL("Dao\\Tag"));
	add_next_index_stringl(&callback, SL("javascriptInclude"));

	DAO_MM_ADD_ENTRY(&callback);

	DAO_MM_ZVAL_STRING(&type, "js");

	DAO_MM_CALL_METHOD(return_value, getThis(), "output", &collection, &callback, &type, args);
	RETURN_MM();
}
