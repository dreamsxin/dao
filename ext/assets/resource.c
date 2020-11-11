
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

#include "assets/resource.h"
#include "assets/exception.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/operators.h"
#include "kernel/concat.h"
#include "kernel/exception.h"
#include "kernel/file.h"
#include "kernel/fcall.h"

/**
 * Dao\Assets\Resource
 *
 * Represents an asset resource
 *
 *<code>
 * $resource = new Dao\Assets\Resource('js', 'javascripts/jquery.js');
 *</code>
 *
 */
zend_class_entry *dao_assets_resource_ce;

PHP_METHOD(Dao_Assets_Resource, __construct);
PHP_METHOD(Dao_Assets_Resource, setType);
PHP_METHOD(Dao_Assets_Resource, getType);
PHP_METHOD(Dao_Assets_Resource, setPath);
PHP_METHOD(Dao_Assets_Resource, getPath);
PHP_METHOD(Dao_Assets_Resource, setLocal);
PHP_METHOD(Dao_Assets_Resource, getLocal);
PHP_METHOD(Dao_Assets_Resource, setFilter);
PHP_METHOD(Dao_Assets_Resource, getFilter);
PHP_METHOD(Dao_Assets_Resource, setAttributes);
PHP_METHOD(Dao_Assets_Resource, getAttributes);
PHP_METHOD(Dao_Assets_Resource, setTargetUri);
PHP_METHOD(Dao_Assets_Resource, getTargetUri);
PHP_METHOD(Dao_Assets_Resource, setSourcePath);
PHP_METHOD(Dao_Assets_Resource, getSourcePath);
PHP_METHOD(Dao_Assets_Resource, setTargetPath);
PHP_METHOD(Dao_Assets_Resource, getTargetPath);
PHP_METHOD(Dao_Assets_Resource, getContent);
PHP_METHOD(Dao_Assets_Resource, getRealTargetUri);
PHP_METHOD(Dao_Assets_Resource, getRealSourcePath);
PHP_METHOD(Dao_Assets_Resource, getRealTargetPath);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_assets_resource___construct, 0, 0, 2)
	ZEND_ARG_INFO(0, type)
	ZEND_ARG_INFO(0, path)
	ZEND_ARG_INFO(0, local)
	ZEND_ARG_INFO(0, filter)
	ZEND_ARG_INFO(0, attributes)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_assets_resource_settype, 0, 0, 1)
	ZEND_ARG_INFO(0, type)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_assets_resource_setpath, 0, 0, 1)
	ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_assets_resource_setlocal, 0, 0, 1)
	ZEND_ARG_INFO(0, local)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_assets_resource_setfilter, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, filter, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_assets_resource_setattributes, 0, 0, 1)
	ZEND_ARG_INFO(0, attributes)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_assets_resource_settargeturi, 0, 0, 1)
	ZEND_ARG_INFO(0, targetUri)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_assets_resource_setsourcepath, 0, 0, 1)
	ZEND_ARG_INFO(0, sourcePath)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_assets_resource_settargetpath, 0, 0, 1)
	ZEND_ARG_INFO(0, targetPath)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_assets_resource_getcontent, 0, 0, 0)
	ZEND_ARG_INFO(0, basePath)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_assets_resource_getrealsourcepath, 0, 0, 0)
	ZEND_ARG_INFO(0, basePath)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_assets_resource_getrealtargetpath, 0, 0, 0)
	ZEND_ARG_INFO(0, basePath)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_assets_resource_method_entry[] = {
	PHP_ME(Dao_Assets_Resource, __construct, arginfo_dao_assets_resource___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Assets_Resource, setType, arginfo_dao_assets_resource_settype, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Assets_Resource, getType, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Assets_Resource, setPath, arginfo_dao_assets_resource_setpath, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Assets_Resource, getPath, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Assets_Resource, setLocal, arginfo_dao_assets_resource_setlocal, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Assets_Resource, getLocal, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Assets_Resource, setFilter, arginfo_dao_assets_resource_setfilter, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Assets_Resource, getFilter, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Assets_Resource, setAttributes, arginfo_dao_assets_resource_setattributes, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Assets_Resource, getAttributes, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Assets_Resource, setTargetUri, arginfo_dao_assets_resource_settargeturi, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Assets_Resource, getTargetUri, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Assets_Resource, setSourcePath, arginfo_dao_assets_resource_setsourcepath, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Assets_Resource, getSourcePath, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Assets_Resource, setTargetPath, arginfo_dao_assets_resource_settargetpath, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Assets_Resource, getTargetPath, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Assets_Resource, getContent, arginfo_dao_assets_resource_getcontent, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Assets_Resource, getRealTargetUri, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Assets_Resource, getRealSourcePath, arginfo_dao_assets_resource_getrealsourcepath, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Assets_Resource, getRealTargetPath, arginfo_dao_assets_resource_getrealtargetpath, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Assets\Resource initializer
 */
DAO_INIT_CLASS(Dao_Assets_Resource){

	DAO_REGISTER_CLASS(Dao\\Assets, Resource, assets_resource, dao_assets_resource_method_entry, 0);

	zend_declare_property_null(dao_assets_resource_ce, SL("_type"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_assets_resource_ce, SL("_path"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_assets_resource_ce, SL("_local"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_assets_resource_ce, SL("_filter"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_assets_resource_ce, SL("_attributes"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_assets_resource_ce, SL("_sourcePath"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_assets_resource_ce, SL("_targetPath"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_assets_resource_ce, SL("_targetUri"), ZEND_ACC_PROTECTED);

	return SUCCESS;
}

/**
 * Dao\Assets\Resource constructor
 *
 * @param string $type
 * @param string $path
 * @param boolean $local
 * @param boolean $filter
 * @param array $attributes
 */
PHP_METHOD(Dao_Assets_Resource, __construct){

	zval *type, *path, *local = NULL, *filter = NULL, *attributes = NULL;

	dao_fetch_params(0, 2, 3, &type, &path, &local, &filter, &attributes);

	if (!local) {
		local = &DAO_GLOBAL(z_true);
	}

	if (!filter) {
		filter = &DAO_GLOBAL(z_true);
	}

	if (!attributes) {
		attributes = &DAO_GLOBAL(z_null);
	}

	dao_update_property(getThis(), SL("_type"), type);
	dao_update_property(getThis(), SL("_path"), path);
	dao_update_property(getThis(), SL("_local"), local);
	dao_update_property(getThis(), SL("_filter"), filter);
	if (Z_TYPE_P(attributes) == IS_ARRAY) {
		dao_update_property(getThis(), SL("_attributes"), attributes);
	}
}

/**
 * Sets the resource's type
 *
 * @param string $type
 * @return Dao\Assets\Resource
 */
PHP_METHOD(Dao_Assets_Resource, setType){

	zval *type;

	dao_fetch_params(0, 1, 0, &type);

	dao_update_property(getThis(), SL("_type"), type);
	RETURN_THIS();
}

/**
 * Returns the type of resource
 *
 * @return string
 */
PHP_METHOD(Dao_Assets_Resource, getType){


	RETURN_MEMBER(getThis(), "_type");
}

/**
 * Sets the resource's path
 *
 * @param string $path
 * @return Dao\Assets\Resource
 */
PHP_METHOD(Dao_Assets_Resource, setPath){

	zval *path;

	dao_fetch_params(0, 1, 0, &path);

	dao_update_property(getThis(), SL("_path"), path);
	RETURN_THIS();
}

/**
 * Returns the URI/URL path to the resource
 *
 * @return string
 */
PHP_METHOD(Dao_Assets_Resource, getPath){


	RETURN_MEMBER(getThis(), "_path");
}

/**
 * Sets if the resource is local or external
 *
 * @param boolean $local
 * @return Dao\Assets\Resource
 */
PHP_METHOD(Dao_Assets_Resource, setLocal){

	zval *local;

	dao_fetch_params(0, 1, 0, &local);

	dao_update_property(getThis(), SL("_local"), local);
	RETURN_THIS();
}

/**
 * Returns whether the resource is local or external
 *
 * @return boolean
 */
PHP_METHOD(Dao_Assets_Resource, getLocal){


	RETURN_MEMBER(getThis(), "_local");
}

/**
 * Sets if the resource must be filtered or not
 *
 * @param boolean $filter
 * @return Dao\Assets\Resource
 */
PHP_METHOD(Dao_Assets_Resource, setFilter){

	zval *filter;

	dao_fetch_params(0, 1, 0, &filter);

	dao_update_property(getThis(), SL("_filter"), filter);
	RETURN_THIS();
}

/**
 * Returns whether the resource must be filtered or not
 *
 * @return boolean
 */
PHP_METHOD(Dao_Assets_Resource, getFilter){


	RETURN_MEMBER(getThis(), "_filter");
}

/**
 * Sets extra HTML attributes
 *
 * @param array $attributes
 * @return Dao\Assets\Resource
 */
PHP_METHOD(Dao_Assets_Resource, setAttributes){

	zval *attributes;

	dao_fetch_params(0, 1, 0, &attributes);

	dao_update_property(getThis(), SL("_attributes"), attributes);
	RETURN_THIS();
}

/**
 * Returns extra HTML attributes set in the resource
 *
 * @return array
 */
PHP_METHOD(Dao_Assets_Resource, getAttributes){


	RETURN_MEMBER(getThis(), "_attributes");
}

/**
 * Sets a target uri for the generated HTML
 *
 * @param string $targetUri
 * @return Dao\Assets\Resource
 */
PHP_METHOD(Dao_Assets_Resource, setTargetUri){

	zval *target_uri;

	dao_fetch_params(0, 1, 0, &target_uri);

	dao_update_property(getThis(), SL("_targetUri"), target_uri);
	RETURN_THIS();
}

/**
 * Returns the target uri for the generated HTML
 *
 * @return string
 */
PHP_METHOD(Dao_Assets_Resource, getTargetUri){


	RETURN_MEMBER(getThis(), "_targetUri");
}

/**
 * Sets the resource's source path
 *
 * @param string $sourcePath
 * @return Dao\Assets\Resource
 */
PHP_METHOD(Dao_Assets_Resource, setSourcePath){

	zval *source_path;

	dao_fetch_params(0, 1, 0, &source_path);

	dao_update_property(getThis(), SL("_sourcePath"), source_path);
	RETURN_THIS();
}

/**
 * Returns the resource's target path
 *
 * @return string
 */
PHP_METHOD(Dao_Assets_Resource, getSourcePath){


	RETURN_MEMBER(getThis(), "_sourcePath");
}

/**
 * Sets the resource's target path
 *
 * @param string $targetPath
 * @return Dao\Assets\Resource
 */
PHP_METHOD(Dao_Assets_Resource, setTargetPath){

	zval *target_path;

	dao_fetch_params(0, 1, 0, &target_path);

	dao_update_property(getThis(), SL("_targetPath"), target_path);
	RETURN_THIS();
}

/**
 * Returns the resource's target path
 *
 * @return string
 */
PHP_METHOD(Dao_Assets_Resource, getTargetPath){


	RETURN_MEMBER(getThis(), "_targetPath");
}

/**
 * Returns the content of the resource as an string
 * Optionally a base path where the resource is located can be set
 *
 * @param string $basePath
 * @return string
 */
PHP_METHOD(Dao_Assets_Resource, getContent){

	zval *base_path = NULL, source_path = {}, complete_path = {}, local = {}, exception_message = {};

	dao_fetch_params(0, 0, 1, &base_path);

	if (!base_path) {
		base_path = &DAO_GLOBAL(z_null);
	}

	dao_read_property(&source_path, getThis(), SL("_sourcePath"), PH_NOISY|PH_READONLY);
	if (DAO_IS_EMPTY(&source_path)) {
		dao_read_property(&source_path, getThis(), SL("_path"), PH_NOISY|PH_READONLY);
	}

	/**
	 * A base path for resources can be set in the assets manager
	 */
	DAO_CONCAT_VV(&complete_path, base_path, &source_path);

	dao_read_property(&local, getThis(), SL("_local"), PH_NOISY|PH_READONLY);

	/**
	 * Local resources are loaded from the local disk
	 */
	if (zend_is_true(&local)) {

		/**
		 * Check first if the file is readable
		 */
		if (dao_file_exists(&complete_path) == FAILURE) {
			DAO_CONCAT_SVS(&exception_message, "Resource's content for \"", &complete_path, "\" cannot be loaded");
			DAO_THROW_EXCEPTION_ZVAL(dao_assets_exception_ce, &exception_message);
			zval_ptr_dtor(&complete_path);
			return;
		}
	}

	/**
	 * Use file_get_contents to respect the openbase_dir. Access urls must be enabled
	 */
	dao_file_get_contents(return_value, &complete_path);
	if (DAO_IS_FALSE(return_value)) {
		DAO_CONCAT_SVS(&exception_message, "Resource's content for \"", &complete_path, "\" cannot be read");
		DAO_THROW_EXCEPTION_ZVAL(dao_assets_exception_ce, &exception_message);
		zval_ptr_dtor(&complete_path);
		zval_ptr_dtor(&exception_message);
		return;
	}
	zval_ptr_dtor(&complete_path);
}

/**
 * Returns the real target uri for the generated HTML
 *
 * @return string
 */
PHP_METHOD(Dao_Assets_Resource, getRealTargetUri){

	zval *base_uri = NULL, target_uri = {};

	dao_fetch_params(0, 0, 1, &base_uri);

	if (!base_uri) {
		base_uri = &DAO_GLOBAL(z_null);
	}

	dao_read_property(&target_uri, getThis(), SL("_targetUri"), PH_NOISY|PH_READONLY);
	if (DAO_IS_EMPTY(&target_uri)) {
		dao_read_property(&target_uri, getThis(), SL("_path"), PH_NOISY|PH_READONLY);
	}

	DAO_CONCAT_VV(return_value, base_uri, &target_uri);
}

/**
 * Returns the complete location where the resource is located
 *
 * @param string $basePath
 * @return string
 */
PHP_METHOD(Dao_Assets_Resource, getRealSourcePath){

	zval *base_path = NULL, source_path = {}, local = {};

	dao_fetch_params(0, 0, 1, &base_path);

	if (!base_path) {
		base_path = &DAO_GLOBAL(z_null);
	}

	dao_read_property(&source_path, getThis(), SL("_sourcePath"), PH_NOISY|PH_READONLY);
	if (DAO_IS_EMPTY(&source_path)) {
		dao_read_property(&source_path, getThis(), SL("_path"), PH_NOISY|PH_READONLY);
	}

	dao_read_property(&local, getThis(), SL("_local"), PH_NOISY|PH_READONLY);
	if (zend_is_true(&local)) {
		zval complete_path = {};
		/**
		 * A base path for resources can be set in the assets manager
		 */
		DAO_CONCAT_VV(&complete_path, base_path, &source_path);

		/**
		 * Get the real template path
		 */
		dao_file_realpath(return_value, &complete_path);
		zval_ptr_dtor(&complete_path);
		return;
	}

	RETURN_CTOR(&source_path);
}

/**
 * Returns the complete location where the resource must be written
 *
 * @param string $basePath
 * @return string
 */
PHP_METHOD(Dao_Assets_Resource, getRealTargetPath){

	zval *base_path = NULL, target_path = {}, local = {};

	dao_fetch_params(0, 0, 1, &base_path);

	if (!base_path) {
		base_path = &DAO_GLOBAL(z_null);
	}

	dao_read_property(&target_path, getThis(), SL("_targetPath"), PH_NOISY|PH_READONLY);
	if (DAO_IS_EMPTY(&target_path)) {
		dao_read_property(&target_path, getThis(), SL("_path"), PH_NOISY|PH_READONLY);
	}

	dao_read_property(&local, getThis(), SL("_local"), PH_NOISY|PH_READONLY);
	if (zend_is_true(&local)) {
		zval complete_path = {};
		/**
		 * A base path for resources can be set in the assets manager
		 */
		DAO_CONCAT_VV(&complete_path, base_path, &target_path);

		/**
		 * Get the real template path, the target path can optionally don't exist
		 */
		if (dao_file_exists(&complete_path) == SUCCESS) {
			dao_file_realpath(return_value, &complete_path);
			zval_ptr_dtor(&complete_path);
			return;
		}

		RETURN_ZVAL(&complete_path, 0, 0);
	}

	RETURN_CTOR(&target_path);
}
