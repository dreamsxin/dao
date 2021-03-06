
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

#include "mvc/micro/collection.h"
#include "mvc/micro/collectioninterface.h"
#include "interned-strings.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/string.h"
#include "kernel/array.h"
#include "kernel/object.h"
#include "kernel/fcall.h"

/**
 * Dao\Mvc\Micro\Collection
 *
 * Groups Micro-Mvc handlers as controllers
 *
 *<code>
 *
 * $app = new Dao\Mvc\Micro();
 *
 * $collection = new Dao\Mvc\Micro\Collection();
 *
 * $collection->setHandler(new PostsController());
 *
 * $collection->get('/posts/edit/{id}', 'edit');
 *
 * $app->mount($collection);
 *
 *</code>
 *
 */
zend_class_entry *dao_mvc_micro_collection_ce;

PHP_METHOD(Dao_Mvc_Micro_Collection, setPrefix);
PHP_METHOD(Dao_Mvc_Micro_Collection, getPrefix);
PHP_METHOD(Dao_Mvc_Micro_Collection, getHandlers);
PHP_METHOD(Dao_Mvc_Micro_Collection, setHandler);
PHP_METHOD(Dao_Mvc_Micro_Collection, setLazy);
PHP_METHOD(Dao_Mvc_Micro_Collection, isLazy);
PHP_METHOD(Dao_Mvc_Micro_Collection, getHandler);
PHP_METHOD(Dao_Mvc_Micro_Collection, map);
PHP_METHOD(Dao_Mvc_Micro_Collection, get);
PHP_METHOD(Dao_Mvc_Micro_Collection, post);
PHP_METHOD(Dao_Mvc_Micro_Collection, put);
PHP_METHOD(Dao_Mvc_Micro_Collection, patch);
PHP_METHOD(Dao_Mvc_Micro_Collection, head);
PHP_METHOD(Dao_Mvc_Micro_Collection, delete);
PHP_METHOD(Dao_Mvc_Micro_Collection, options);

static const zend_function_entry dao_mvc_micro_collection_method_entry[] = {
	PHP_ME(Dao_Mvc_Micro_Collection, setPrefix, arginfo_dao_mvc_micro_collectioninterface_setprefix, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Micro_Collection, getPrefix, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Micro_Collection, getHandlers, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Micro_Collection, setHandler, arginfo_dao_mvc_micro_collectioninterface_sethandler, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Micro_Collection, setLazy, arginfo_dao_mvc_micro_collectioninterface_setlazy, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Micro_Collection, isLazy, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Micro_Collection, getHandler, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Micro_Collection, map, arginfo_dao_mvc_micro_collectioninterface_map, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Micro_Collection, get, arginfo_dao_mvc_micro_collectioninterface_get, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Micro_Collection, post, arginfo_dao_mvc_micro_collectioninterface_post, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Micro_Collection, put, arginfo_dao_mvc_micro_collectioninterface_put, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Micro_Collection, patch, arginfo_dao_mvc_micro_collectioninterface_patch, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Micro_Collection, head, arginfo_dao_mvc_micro_collectioninterface_head, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Micro_Collection, delete, arginfo_dao_mvc_micro_collectioninterface_delete, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_Micro_Collection, options, arginfo_dao_mvc_micro_collectioninterface_options, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Mvc\Micro\Collection initializer
 */
DAO_INIT_CLASS(Dao_Mvc_Micro_Collection){

	DAO_REGISTER_CLASS(Dao\\Mvc\\Micro, Collection, mvc_micro_collection, dao_mvc_micro_collection_method_entry, 0);

	zend_declare_property_null(dao_mvc_micro_collection_ce, SL("_prefix"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_micro_collection_ce, SL("_lazy"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_micro_collection_ce, SL("_handler"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_micro_collection_ce, SL("_handlers"), ZEND_ACC_PROTECTED);

	zend_class_implements(dao_mvc_micro_collection_ce, 1, dao_mvc_micro_collectioninterface_ce);

	return SUCCESS;
}

/**
 * Internal function to add a handler to the group
 *
 * @param string|null $method
 * @param string $routePattern
 * @param mixed $handler
 * @param string $name
 */
void dao_mvc_collection_addmap(zval *this_ptr, zend_string *method, zval *route_pattern, zval *handler, zval *name)
{
	zval handler_definition = {}, zmethod = {};

	Z_TRY_ADDREF_P(route_pattern);
	Z_TRY_ADDREF_P(handler);

	array_init_size(&handler_definition, 3 + (name != NULL ? 1 : 0));
	if (method) {
		ZVAL_STR(&zmethod, method);
		Z_ADDREF(zmethod);
		add_next_index_zval(&handler_definition, &zmethod);
	} else {
		add_next_index_null(&handler_definition);
	}
	add_next_index_zval(&handler_definition, route_pattern);
	add_next_index_zval(&handler_definition, handler);
	if (name) {
		Z_TRY_ADDREF_P(name);
		add_next_index_zval(&handler_definition, name);
	} else {
		add_next_index_null(&handler_definition);
	}

	Z_ADDREF(handler_definition);

	dao_update_property_array_append(this_ptr, SL("_handlers"), &handler_definition);
}

/**
 * Sets a prefix for all routes added to the collection
 *
 * @param string $prefix
 * @return Dao\Mvc\Micro\CollectionInterface
 */
PHP_METHOD(Dao_Mvc_Micro_Collection, setPrefix){

	zval *prefix;

	dao_fetch_params(0, 1, 0, &prefix);
	
	dao_update_property(getThis(), SL("_prefix"), prefix);
	RETURN_THIS();
}

/**
 * Returns the collection prefix if any
 *
 * @return string
 */
PHP_METHOD(Dao_Mvc_Micro_Collection, getPrefix){


	RETURN_MEMBER(getThis(), "_prefix");
}

/**
 * Returns the registered handlers
 *
 * @return array
 */
PHP_METHOD(Dao_Mvc_Micro_Collection, getHandlers){


	RETURN_MEMBER(getThis(), "_handlers");
}

/**
 * Sets the main handler
 *
 * @param mixed $handler
 * @param boolean $lazy
 * @return Dao\Mvc\Micro\CollectionInterface
 */
PHP_METHOD(Dao_Mvc_Micro_Collection, setHandler){

	zval *handler, *lazy = NULL;

	dao_fetch_params(0, 1, 1, &handler, &lazy);
	
	if (!lazy) {
		lazy = &DAO_GLOBAL(z_false);
	}
	
	dao_update_property(getThis(), SL("_handler"), handler);
	dao_update_property(getThis(), SL("_lazy"), lazy);
	RETURN_THIS();
}

/**
 * Sets if the main handler must be lazy loaded
 *
 * @param boolean $lazy
 * @return Dao\Mvc\Micro\CollectionInterface
 */
PHP_METHOD(Dao_Mvc_Micro_Collection, setLazy){

	zval *lazy;

	dao_fetch_params(0, 1, 0, &lazy);
	
	dao_update_property(getThis(), SL("_lazy"), lazy);
	RETURN_THIS();
}

/**
 * Returns if the main handler must be lazy loaded
 *
 * @return boolean
 */
PHP_METHOD(Dao_Mvc_Micro_Collection, isLazy){


	RETURN_MEMBER(getThis(), "_lazy");
}

/**
 * Returns the main handler
 *
 * @return mixed
 */
PHP_METHOD(Dao_Mvc_Micro_Collection, getHandler){


	RETURN_MEMBER(getThis(), "_handler");
}

/**
 * Maps a route to a handler
 *
 * @param string $routePattern
 * @param callable $handler
 * @param string $name
 * @return Dao\Mvc\Micro\CollectionInterface
 */
PHP_METHOD(Dao_Mvc_Micro_Collection, map){

	zval *route_pattern, *handler, *name = NULL;

	dao_fetch_params(0, 2, 1, &route_pattern, &handler, &name);
	
	dao_mvc_collection_addmap(getThis(), NULL, route_pattern, handler, name);
	RETURN_ZVAL(getThis(), 1, 0);
}

/**
 * Maps a route to a handler that only matches if the HTTP method is GET
 *
 * @param string $routePattern
 * @param callable $handler
 * @param string $name
 * @return Dao\Mvc\Micro\CollectionInterface
 */
PHP_METHOD(Dao_Mvc_Micro_Collection, get){

	zval *route_pattern, *handler, *name = NULL;

	dao_fetch_params(0, 2, 1, &route_pattern, &handler, &name);
	
	dao_mvc_collection_addmap(getThis(), dao_interned_GET, route_pattern, handler, name);
	RETURN_ZVAL(getThis(), 1, 0);
}

/**
 * Maps a route to a handler that only matches if the HTTP method is POST
 *
 * @param string $routePattern
 * @param callable $handler
 * @param string $name
 * @return Dao\Mvc\Micro\CollectionInterface
 */
PHP_METHOD(Dao_Mvc_Micro_Collection, post){

	zval *route_pattern, *handler, *name = NULL;

	dao_fetch_params(0, 2, 0, &route_pattern, &handler, &name);
	
	dao_mvc_collection_addmap(getThis(), dao_interned_POST, route_pattern, handler, name);
	RETURN_ZVAL(getThis(), 1, 0);
}

/**
 * Maps a route to a handler that only matches if the HTTP method is PUT
 *
 * @param string $routePattern
 * @param callable $handler
 * @param string $name
 * @return Dao\Mvc\Micro\CollectionInterface
 */
PHP_METHOD(Dao_Mvc_Micro_Collection, put){

	zval *route_pattern, *handler, *name = NULL;

	dao_fetch_params(0, 2, 0, &route_pattern, &handler, &name);
	
	dao_mvc_collection_addmap(getThis(), dao_interned_PUT, route_pattern, handler, name);
	RETURN_ZVAL(getThis(), 1, 0);
}

/**
 * Maps a route to a handler that only matches if the HTTP method is PATCH
 *
 * @param string $routePattern
 * @param callable $handler
 * @param string $name
 * @return Dao\Mvc\Micro\CollectionInterface
 */
PHP_METHOD(Dao_Mvc_Micro_Collection, patch){

	zval *route_pattern, *handler, *name = NULL;

	dao_fetch_params(0, 2, 0, &route_pattern, &handler, &name);
	
	dao_mvc_collection_addmap(getThis(), dao_interned_PATCH, route_pattern, handler, name);
	RETURN_ZVAL(getThis(), 1, 0);
}

/**
 * Maps a route to a handler that only matches if the HTTP method is HEAD
 *
 * @param string $routePattern
 * @param callable $handler
 * @param string $name
 * @return Dao\Mvc\Micro\CollectionInterface
 */
PHP_METHOD(Dao_Mvc_Micro_Collection, head){

	zval *route_pattern, *handler, *name = NULL;

	dao_fetch_params(0, 2, 0, &route_pattern, &handler, &name);
	
	dao_mvc_collection_addmap(getThis(), dao_interned_HEAD, route_pattern, handler, name);
	RETURN_ZVAL(getThis(), 1, 0);
}

/**
 * Maps a route to a handler that only matches if the HTTP method is DELETE
 *
 * @param string $routePattern
 * @param callable $handler
 * @param string $name
 * @return Dao\Mvc\Micro\CollectionInterface
 */
PHP_METHOD(Dao_Mvc_Micro_Collection, delete){

	zval *route_pattern, *handler, *name = NULL;

	dao_fetch_params(0, 2, 0, &route_pattern, &handler, &name);
	
	dao_mvc_collection_addmap(getThis(), dao_interned_DELETE, route_pattern, handler, name);
	RETURN_ZVAL(getThis(), 1, 0);
}

/**
 * Maps a route to a handler that only matches if the HTTP method is OPTIONS
 *
 * @param string $routePattern
 * @param callable $handler
 * @param string $name
 * @return Dao\Mvc\Micro\CollectionInterface
 */
PHP_METHOD(Dao_Mvc_Micro_Collection, options){

	zval *route_pattern, *handler, *name = NULL;

	dao_fetch_params(0, 2, 0, &route_pattern, &handler, &name);
	
	dao_mvc_collection_addmap(getThis(), dao_interned_OPTIONS, route_pattern, handler, name);
	RETURN_ZVAL(getThis(), 1, 0);
}
