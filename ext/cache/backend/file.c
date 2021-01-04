
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

#include "cache/backend/file.h"
#include "cache/backend.h"
#include "cache/backendinterface.h"
#include "cache/exception.h"

#include <ext/standard/file.h>
#include <ext/spl/spl_directory.h>

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/exception.h"
#include "kernel/fcall.h"
#include "kernel/object.h"
#include "kernel/concat.h"
#include "kernel/file.h"
#include "kernel/operators.h"
#include "kernel/string.h"

/**
 * Dao\Cache\Backend\File
 *
 * Allows to cache output fragments using a file backend
 *
 *<code>
 *	//Cache the file for 2 days
 *	$frontendOptions = array(
 *		'lifetime' => 172800
 *	);
 *
 *  //Create a output cache
 *  $frontCache = \Dao\Cache\Frontend\Output($frontOptions);
 *
 *	//Set the cache directory
 *	$backendOptions = array(
 *		'cacheDir' => '../app/cache/'
 *	);
 *
 *  //Create the File backend
 *  $cache = new \Dao\Cache\Backend\File($frontCache, $backendOptions);
 *
 *	$content = $cache->start('my-cache');
 *	if ($content === null) {
 *  	echo '<h1>', time(), '</h1>';
 *  	$cache->save();
 *	} else {
 *		echo $content;
 *	}
 *</code>
 */
zend_class_entry *dao_cache_backend_file_ce;

PHP_METHOD(Dao_Cache_Backend_File, __construct);
PHP_METHOD(Dao_Cache_Backend_File, get);
PHP_METHOD(Dao_Cache_Backend_File, save);
PHP_METHOD(Dao_Cache_Backend_File, delete);
PHP_METHOD(Dao_Cache_Backend_File, queryKeys);
PHP_METHOD(Dao_Cache_Backend_File, exists);
PHP_METHOD(Dao_Cache_Backend_File, increment);
PHP_METHOD(Dao_Cache_Backend_File, decrement);
PHP_METHOD(Dao_Cache_Backend_File, flush);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_cache_backend_file___construct, 0, 0, 2)
	ZEND_ARG_INFO(0, frontend)
	ZEND_ARG_TYPE_INFO(0, options, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_cache_backend_file_method_entry[] = {
	PHP_ME(Dao_Cache_Backend_File, __construct, arginfo_dao_cache_backend_file___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Cache_Backend_File, get, arginfo_dao_cache_backendinterface_get, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Backend_File, save, arginfo_dao_cache_backendinterface_save, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Backend_File, delete, arginfo_dao_cache_backendinterface_delete, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Backend_File, queryKeys, arginfo_dao_cache_backendinterface_querykeys, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Backend_File, exists, arginfo_dao_cache_backendinterface_exists, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Backend_File, increment, arginfo_dao_cache_backendinterface_increment, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Backend_File, decrement, arginfo_dao_cache_backendinterface_decrement, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Cache_Backend_File, flush, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Cache\Backend\File initializer
 */
DAO_INIT_CLASS(Dao_Cache_Backend_File){

	DAO_REGISTER_CLASS_EX(Dao\\Cache\\Backend, File, cache_backend_file, dao_cache_backend_ce, dao_cache_backend_file_method_entry, 0);

	zend_declare_property_null(dao_cache_backend_file_ce, SL("_cacheDir"), ZEND_ACC_PROTECTED);

	zend_class_implements(dao_cache_backend_file_ce, 1, dao_cache_backendinterface_ce);

	return SUCCESS;
}

/**
 * Dao\Cache\Backend\File constructor
 *
 * @param Dao\Cache\FrontendInterface $frontend
 * @param array $options
 */
PHP_METHOD(Dao_Cache_Backend_File, __construct){

	zval *frontend, *options, cache_dir = {};
	zend_long mode = 0777;

	dao_fetch_params(0, 2, 0, &frontend, &options);

	if (unlikely(!dao_array_isset_fetch_str(&cache_dir, options, SL("cacheDir"), PH_READONLY))) {
		DAO_THROW_EXCEPTION_STR(dao_cache_exception_ce, "Cache directory must be specified with the option cacheDir");
		return;
	}
	dao_update_property(getThis(), SL("_cacheDir"), &cache_dir);

	DAO_CALL_PARENT(NULL, dao_cache_backend_file_ce, getThis(), "__construct", frontend, options);

	if (dao_file_exists(&cache_dir) == FAILURE) {
		php_stream_mkdir(Z_STRVAL(cache_dir), (int)mode, PHP_STREAM_MKDIR_RECURSIVE | REPORT_ERRORS, php_stream_context_from_zval(NULL, 0));
	}
}

/**
 * Returns a cached content
 *
 * @param string $keyName
 * @param int $lifetime
 * @return mixed
 */
PHP_METHOD(Dao_Cache_Backend_File, get){

	zval *key_name, *lifetime = NULL, prefix = {}, prefixed_key = {}, cache_dir = {}, cache_file = {}, frontend = {};
	zval modified_time = {}, cached_content = {}, exception_message = {};
	long int now, ttl, mtime, diff;
	int expired;

	dao_fetch_params(0, 1, 1, &key_name, &lifetime);

	dao_read_property(&prefix, getThis(), SL("_prefix"), PH_NOISY|PH_READONLY);
	dao_read_property(&cache_dir, getThis(), SL("_cacheDir"), PH_NOISY|PH_READONLY);

	DAO_CONCAT_VV(&prefixed_key, &prefix, key_name);
	DAO_CONCAT_VV(&cache_file, &cache_dir, &prefixed_key);
	zval_ptr_dtor(&prefixed_key);

	if (dao_file_exists(&cache_file) == SUCCESS) {
		dao_read_property(&frontend, getThis(), SL("_frontend"), PH_READONLY);

		/**
		 * Check if the file has expired
		 */
		now = (long int)time(NULL);

		dao_filemtime(&modified_time, &cache_file);
		if (unlikely(Z_TYPE(modified_time) != IS_LONG)) {
			convert_to_long(&modified_time);
		}

		if (!lifetime || Z_TYPE_P(lifetime) != IS_LONG) {
			zval tmp = {};
			DAO_CALL_METHOD(&tmp, getThis(), "getlifetime");
			ttl = dao_get_intval(&tmp);
			zval_ptr_dtor(&tmp);
		} else {
			ttl = dao_get_intval(lifetime);
		}

		mtime   = Z_LVAL(modified_time);
		diff    = now - ttl;
		expired = diff > mtime;

		/**
		 * The content is only retrieved if the content has not expired
		 */
		if (!expired) {
			/**
			 * Use file-get-contents to control that the openbase_dir can't be skipped
			 */
			dao_file_get_contents(&cached_content, &cache_file);
			if (DAO_IS_FALSE(&cached_content)) {
				DAO_CONCAT_SVS(&exception_message, "Cache file ", &cache_file, " could not be opened");
				DAO_THROW_EXCEPTION_ZVAL(dao_cache_exception_ce, &exception_message);
				goto end;
			}

			if (dao_is_numeric(&cached_content)) {
				ZVAL_COPY(return_value, &cached_content);
			} else {
				/**
				 * Use the frontend to process the content of the cache
				 */
				DAO_RETURN_CALL_METHOD(&frontend, "afterretrieve", &cached_content);
			}
			zval_ptr_dtor(&cached_content);
		} else {
			dao_unlink(return_value, &cache_file);
			ZVAL_NULL(return_value);
		}
	} else {
		ZVAL_NULL(return_value);
	}
end:
	zval_ptr_dtor(&cache_file);
}

/**
 * Stores cached content into the file backend and stops the frontend
 *
 * @param string $keyName
 * @param string $content
 * @param long $lifetime
 * @param boolean $stopBuffer
 */
PHP_METHOD(Dao_Cache_Backend_File, save){

	zval *key_name = NULL, *content = NULL, *lifetime = NULL, *stop_buffer = NULL, frontend = {};
	zval key = {}, prefix = {}, prefixed_key = {}, cache_dir = {}, cache_file = {}, cached_content = {}, prepared_content = {}, status = {}, is_buffering = {};

	dao_fetch_params(0, 0, 4, &key_name, &content, &lifetime, &stop_buffer);

	dao_read_property(&prefix, getThis(), SL("_prefix"), PH_READONLY);

	if (!key_name || Z_TYPE_P(key_name) == IS_NULL) {
		dao_read_property(&key, getThis(), SL("_lastKey"), PH_READONLY);
		key_name = &key;
	}

	if (!zend_is_true(key_name)) {
		DAO_THROW_EXCEPTION_STR(dao_cache_exception_ce, "The cache must be started first");
		return;
	}

	DAO_CONCAT_VV(&prefixed_key, &prefix, key_name);

	dao_read_property(&frontend, getThis(), SL("_frontend"), PH_NOISY|PH_READONLY);
	dao_read_property(&cache_dir, getThis(), SL("_cacheDir"), PH_NOISY|PH_READONLY);

	DAO_CONCAT_VV(&cache_file, &cache_dir, &prefixed_key);
	zval_ptr_dtor(&prefixed_key);

	if (!content || !zend_is_true(content)) {
		DAO_CALL_METHOD(&cached_content, &frontend, "getcontent");
	} else {
		ZVAL_COPY(&cached_content, content);
	}

	/**
	 * We use file_put_contents to respect open-base-dir directive
	 */
	if (!dao_is_numeric(&cached_content)) {
		DAO_CALL_METHOD(&prepared_content, &frontend, "beforestore", &cached_content);
		dao_file_put_contents(&status, &cache_file, &prepared_content);
		zval_ptr_dtor(&prepared_content);
	} else {
		dao_file_put_contents(&status, &cache_file, &cached_content);
	}

	if (DAO_IS_FALSE(&status)) {
		DAO_THROW_EXCEPTION_FORMAT(dao_cache_exception_ce, "Cache directory is not writable: %s", &cache_file);
		zval_ptr_dtor(&cache_file);
		zval_ptr_dtor(&cached_content);
		return;
	}
	zval_ptr_dtor(&cache_file);

	DAO_CALL_METHOD(&is_buffering, &frontend, "isbuffering");
	if (!stop_buffer || DAO_IS_TRUE(stop_buffer)) {
		DAO_CALL_METHOD(NULL, &frontend, "stop");
	}

	if (DAO_IS_TRUE(&is_buffering)) {
		zend_print_zval(&cached_content, 0);
	}
	zval_ptr_dtor(&cached_content);

	dao_update_property_bool(getThis(), SL("_started"), 0);
}

/**
 * Deletes a value from the cache by its key
 *
 * @param string $keyName
 * @return boolean
 */
PHP_METHOD(Dao_Cache_Backend_File, delete){

	zval *key_name, prefix = {}, prefixed_key = {}, cache_dir = {}, cache_file = {};

	dao_fetch_params(0, 1, 0, &key_name);

	dao_read_property(&prefix, getThis(), SL("_prefix"), PH_NOISY|PH_READONLY);
	dao_read_property(&cache_dir, getThis(), SL("_cacheDir"), PH_NOISY|PH_READONLY);

	DAO_CONCAT_VV(&prefixed_key, &prefix, key_name);

	DAO_CONCAT_VV(&cache_file, &cache_dir, &prefixed_key);
	zval_ptr_dtor(&prefixed_key);

	if (dao_file_exists(&cache_file) == SUCCESS) {
		dao_unlink(return_value, &cache_file);
		zval_ptr_dtor(&cache_file);
		return;
	}
	zval_ptr_dtor(&cache_file);

	RETURN_FALSE;
}

/**
 * Query the existing cached keys
 *
 * @param string $prefix
 * @return array
 */
PHP_METHOD(Dao_Cache_Backend_File, queryKeys){

	zval *prefix = NULL, cache_dir = {}, iterator = {};
	zend_object_iterator *it;

	dao_fetch_params(0, 0, 1, &prefix);

	array_init(return_value);

	dao_read_property(&cache_dir, getThis(), SL("_cacheDir"), PH_NOISY|PH_READONLY);

	/**
	 * We use a directory iterator to traverse the cache dir directory
	 */
	object_init_ex(&iterator, spl_ce_DirectoryIterator);
	DAO_CALL_METHOD(NULL, &iterator, "__construct", &cache_dir);

	/* DirectoryIterator implements Iterator */
	assert(instanceof_function(spl_ce_DirectoryIterator, zend_ce_iterator));

	it = spl_ce_DirectoryIterator->get_iterator(spl_ce_DirectoryIterator, &iterator, 0);

	/* DirectoryIterator is an iterator */
	assert(it != NULL);

	/* DirectoryIterator has rewind() method */
	assert(it->funcs->rewind != NULL);

	it->funcs->rewind(it);
	while (it->funcs->valid(it) == SUCCESS && !EG(exception)) {
		zval *item, is_directory = {}, key = {};

		item = it->funcs->get_current_data(it);

		if (FAILURE == dao_call_method(&is_directory, item, "isdir", 0, NULL)) {
			break;
		}

		if (!EG(exception) && DAO_IS_FALSE(&is_directory)) {
			if (FAILURE == dao_call_method(&key, item, "getfilename", 0, NULL)) {
				break;
			}

			if (!EG(exception) && (!prefix || dao_start_with(&key, prefix, NULL))) {
				dao_array_append(return_value, &key, PH_COPY);
			}
			zval_ptr_dtor(&key);
		}

		it->funcs->move_forward(it);
	}

	if (it) {
		zend_iterator_dtor(it);
	}
	zval_ptr_dtor(&iterator);
}

/**
 * Checks if cache exists and it isn't expired
 *
 * @param string $keyName
 * @param  long $lifetime
 * @return boolean
 */
PHP_METHOD(Dao_Cache_Backend_File, exists){

	zval *key_name, prefix = {}, prefixed_key = {}, cache_dir = {}, cache_file = {}, frontend = {}, lifetime = {}, modified_time = {};
	long int mtime, ttl;

	dao_fetch_params(0, 1, 0, &key_name);

	dao_read_property(&prefix, getThis(), SL("_prefix"), PH_READONLY);
	dao_read_property(&cache_dir, getThis(), SL("_cacheDir"), PH_NOISY|PH_READONLY);

	DAO_CONCAT_VV(&prefixed_key, &prefix, key_name);
	DAO_CONCAT_VV(&cache_file, &cache_dir, &prefixed_key);
	zval_ptr_dtor(&prefixed_key);

	if (dao_file_exists(&cache_file) == SUCCESS) {
		dao_read_property(&frontend, getThis(), SL("_frontend"), PH_READONLY);

		/**
		 * Check if the file has expired
		 */
		DAO_CALL_METHOD(&lifetime, &frontend, "getlifetime");

		ttl = Z_LVAL(lifetime) ;
		zval_ptr_dtor(&lifetime);

		dao_filemtime(&modified_time, &cache_file);
		mtime = likely(Z_TYPE(modified_time) == IS_LONG) ? Z_LVAL(modified_time) : dao_get_intval(&modified_time);
		zval_ptr_dtor(&modified_time);

		if (mtime + ttl > (long int)time(NULL)) {
			RETVAL_TRUE;
		}
	} else {
		RETVAL_FALSE;
	}
	zval_ptr_dtor(&cache_file);
}

/**
 * Increment of a given key, by number $value
 *
 * @param string $keyName
 * @param long $value
 * @return mixed
 */
PHP_METHOD(Dao_Cache_Backend_File, increment){

	zval *key_name, *value = NULL, prefix = {}, prefixed_key = {}, status = {};
	zval cache_dir = {}, cache_file = {}, frontend = {}, lifetime = {}, modified_time = {}, cached_content = {};
	long int now, ttl, mtime, diff;
	int expired;

	dao_fetch_params(0, 1, 1, &key_name, &value);

	if (!value) {
		value = &DAO_GLOBAL(z_one);
	}

	dao_read_property(&prefix, getThis(), SL("_prefix"), PH_NOISY|PH_READONLY);
	dao_read_property(&cache_dir, getThis(), SL("_cacheDir"), PH_NOISY|PH_READONLY);

	DAO_CONCAT_VV(&prefixed_key, &prefix, key_name);

	DAO_CONCAT_VV(&cache_file, &cache_dir, &prefixed_key);
	zval_ptr_dtor(&prefixed_key);
	assert(Z_TYPE(cache_file) == IS_STRING);

	if (dao_file_exists(&cache_file) == SUCCESS) {
		dao_read_property(&frontend, getThis(), SL("_frontend"), PH_NOISY|PH_READONLY);
		/**
		 * Check if the file has expired
		 */
		now = (long int)time(NULL);

		/**
		 * Take the lifetime from the frontend or read it from the set in start()
		 */
		DAO_CALL_METHOD(&lifetime, getThis(), "getlifetime");
		ttl = dao_get_intval(&lifetime);
		zval_ptr_dtor(&lifetime);

		dao_filemtime(&modified_time, &cache_file);
		if (unlikely(Z_TYPE(modified_time) != IS_LONG)) {
			convert_to_long(&modified_time);
		}

		mtime   = Z_LVAL(modified_time);
		diff    = now - ttl;
		expired = diff > mtime;
		zval_ptr_dtor(&modified_time);

		/**
		 * The content is only retrieved if the content has not expired
		 */
		if (!expired) {
			/**
			 * Use file-get-contents to control that the openbase_dir can't be skipped
			 */
			dao_file_get_contents(&cached_content, &cache_file);
			if (DAO_IS_FALSE(&cached_content)) {
				zend_throw_exception_ex(dao_cache_exception_ce, 0, "Failed to open cache file %s", Z_STRVAL(cache_file));
				zval_ptr_dtor(&cache_file);
				return;
			}

			dao_add_function(return_value, &cached_content, value);
			zval_ptr_dtor(&cached_content);

			dao_file_put_contents(&status, &cache_file, return_value);

			if (DAO_IS_FALSE(&status)) {
				DAO_THROW_EXCEPTION_STR(dao_cache_exception_ce, "Cache directory can't be written");
			}
		}
	} else {
		RETVAL_FALSE;
	}
	zval_ptr_dtor(&cache_file);
}

/**
 * Decrement of a given key, by number $value
 *
 * @param string $keyName
 * @param long $value
 * @return mixed
 */
PHP_METHOD(Dao_Cache_Backend_File, decrement){

	zval *key_name, *value = NULL, prefix = {}, prefixed_key = {}, status = {};
	zval cache_dir = {}, cache_file = {}, frontend = {}, lifetime = {}, modified_time = {}, cached_content = {};
	long int now, ttl, mtime, diff;
	int expired;

	dao_fetch_params(0, 1, 1, &key_name, &value);
	if (!value) {
		value = &DAO_GLOBAL(z_one);
	}

	dao_read_property(&prefix, getThis(), SL("_prefix"), PH_NOISY|PH_READONLY);
	dao_read_property(&cache_dir, getThis(), SL("_cacheDir"), PH_NOISY|PH_READONLY);

	DAO_CONCAT_VV(&prefixed_key, &prefix, key_name);
	DAO_CONCAT_VV(&cache_file, &cache_dir, &prefixed_key);
	zval_ptr_dtor(&prefixed_key);
	assert(Z_TYPE(cache_file) == IS_STRING);

	if (dao_file_exists(&cache_file) == SUCCESS) {
		dao_read_property(&frontend, getThis(), SL("_frontend"), PH_NOISY|PH_READONLY);
		/**
		 * Check if the file has expired
		 */
		now = (long int)time(NULL);

		/**
		 * Take the lifetime from the frontend or read it from the set in start()
		 */
		DAO_CALL_METHOD(&lifetime, getThis(), "getlifetime");
		ttl = dao_get_intval(&lifetime);
		zval_ptr_dtor(&lifetime);

		dao_filemtime(&modified_time, &cache_file);
		if (unlikely(Z_TYPE(modified_time) != IS_LONG)) {
			convert_to_long(&modified_time);
		}

		mtime   = Z_LVAL(modified_time);
		diff    = now - ttl;
		expired = diff > mtime;
		zval_ptr_dtor(&modified_time);

		/**
		 * The content is only retrieved if the content has not expired
		 */
		if (!expired) {
			/**
			 * Use file-get-contents to control that the openbase_dir can't be skipped
			 */
			dao_file_get_contents(&cached_content, &cache_file);
			if (DAO_IS_FALSE(&cached_content)) {
				zend_throw_exception_ex(dao_cache_exception_ce, 0, "Failed to open cache file %s", Z_STRVAL(cache_file));
				zval_ptr_dtor(&cache_file);
				return;
			}

			dao_sub_function(return_value, &cached_content, value);
			zval_ptr_dtor(&cached_content);

			dao_file_put_contents(&status, &cache_file, return_value);

			if (DAO_IS_FALSE(&status)) {
				DAO_THROW_EXCEPTION_STR(dao_cache_exception_ce, "Cache directory can't be written");
			}
		}
	} else {
		RETVAL_FALSE;
	}
	zval_ptr_dtor(&cache_file);
}

/**
 * Immediately invalidates all existing items.
 *
 * @return boolean
 */
PHP_METHOD(Dao_Cache_Backend_File, flush){

	zval prefix = {}, cache_dir = {}, iterator = {};
	zend_object_iterator *it;

	dao_read_property(&prefix, getThis(), SL("_prefix"), PH_NOISY|PH_READONLY);
	dao_read_property(&cache_dir, getThis(), SL("_cacheDir"), PH_NOISY|PH_READONLY);

	object_init_ex(&iterator, spl_ce_DirectoryIterator);
	assert(dao_has_constructor(&iterator));
	DAO_CALL_METHOD(NULL, &iterator, "__construct", &cache_dir);

	/* DirectoryIterator implements Iterator */
	assert(instanceof_function(spl_ce_DirectoryIterator, zend_ce_iterator));

	it = spl_ce_DirectoryIterator->get_iterator(spl_ce_DirectoryIterator, &iterator, 0);

	/* DirectoryIterator is an iterator */
	assert(it != NULL);

	/* DirectoryIterator has rewind() method */
	assert(it->funcs->rewind != NULL);

	it->funcs->rewind(it);
	while (it->funcs->valid(it) == SUCCESS && !EG(exception)) {
		zval *item, is_file = {}, key = {}, cache_file = {};
		item = it->funcs->get_current_data(it);

		if (FAILURE == dao_call_method(&is_file, item, "isfile", 0, NULL)) {
			break;
		}

		if (DAO_IS_TRUE(&is_file)) {
			if (FAILURE == dao_call_method(&key, item, "getfilename", 0, NULL)) {
				break;
			}

			if (FAILURE == dao_call_method(&cache_file, item, "getpathname", 0, NULL)) {
				zval_ptr_dtor(&key);
				break;
			}

			if (DAO_IS_EMPTY(&prefix) || dao_start_with(&key, &prefix, NULL)) {
				dao_unlink(return_value, &cache_file);
			}
			zval_ptr_dtor(&key);
			zval_ptr_dtor(&cache_file);
		}

		it->funcs->move_forward(it);
	}

	if (it) {
		zend_iterator_dtor(it);
	}
	zval_ptr_dtor(&iterator);

	RETURN_TRUE;
}
