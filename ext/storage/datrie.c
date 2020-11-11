
/*
  +------------------------------------------------------------------------+
  | Dao Framework    													  |
  +------------------------------------------------------------------------+
  | Copyright (c) 2011-2014 Phalcon Team (http://www.phalconphp.com)	   |
  +------------------------------------------------------------------------+
  | This source file is subject to the New BSD License that is bundled	 |
  | with this package in the file docs/LICENSE.txt.						|
  |																		|
  | If you did not receive a copy of the license and are unable to		 |
  | obtain it through the world-wide-web, please send an email			 |
  | to license@phalconphp.com so we can send you a copy immediately.	   |
  +------------------------------------------------------------------------+
  | Authors: Andres Gutierrez <andres@phalconphp.com>					  |
  |		  Eduar Carvajal <eduar@phalconphp.com>						 |
  |		  ZhuZongXin <dreamsxin@qq.com>								 |
  +------------------------------------------------------------------------+
*/

#include "storage/datrie.h"
#include "storage/exception.h"

#include "zend_smart_str.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "kernel/file.h"
#include "kernel/exception.h"

/**
 * Dao\Storage\Datrie
 *
 */
zend_class_entry *dao_storage_datrie_ce;

PHP_METHOD(Dao_Storage_Datrie, __construct);
PHP_METHOD(Dao_Storage_Datrie, search);
PHP_METHOD(Dao_Storage_Datrie, add);
PHP_METHOD(Dao_Storage_Datrie, query);
PHP_METHOD(Dao_Storage_Datrie, delete);
PHP_METHOD(Dao_Storage_Datrie, save);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_datrie___construct, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, filename, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_datrie_search, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, all, _IS_BOOL, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_datrie_add, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, keyword, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_datrie_query, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, keyword, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_storage_datrie_delete, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, keyword, IS_STRING, 0)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_storage_datrie_method_entry[] = {
	PHP_ME(Dao_Storage_Datrie, __construct, arginfo_dao_storage_datrie___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Storage_Datrie, search, arginfo_dao_storage_datrie_search, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Datrie, add, arginfo_dao_storage_datrie_add, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Datrie, query, arginfo_dao_storage_datrie_query, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Datrie, delete, arginfo_dao_storage_datrie_delete, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Storage_Datrie, save, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_MALIAS(Dao_Storage_Datrie, get, query, arginfo_dao_storage_datrie_query, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

zend_object_handlers dao_storage_datrie_object_handlers;
zend_object* dao_storage_datrie_object_create_handler(zend_class_entry *ce)
{
	dao_storage_datrie_object *intern = ecalloc(1, sizeof(dao_storage_datrie_object) + zend_object_properties_size(ce));
	intern->std.ce = ce;

	zend_object_std_init(&intern->std, ce);
	object_properties_init(&intern->std, ce);
	intern->std.handlers = &dao_storage_datrie_object_handlers;

	return &intern->std;
}

void dao_storage_datrie_object_free_handler(zend_object *object)
{
	dao_storage_datrie_object *intern = dao_storage_datrie_object_from_obj(object);
	if (intern->trie) trie_free(intern->trie);
}

/**
 * Dao\Storage\Datrie initializer
 */
DAO_INIT_CLASS(Dao_Storage_Datrie){

	DAO_REGISTER_CLASS_CREATE_OBJECT(Dao\\Storage, Datrie, storage_datrie, dao_storage_datrie_method_entry, 0);

	zend_declare_property_null(dao_storage_datrie_ce, SL("_filename"), ZEND_ACC_PROTECTED);

	return SUCCESS;
}

/**
 * Dao\Storage\Datrie constructor
 *
 * @param string $filename
 */
PHP_METHOD(Dao_Storage_Datrie, __construct)
{
	zval *filename;
	dao_storage_datrie_object *intern;

	dao_fetch_params(0, 1, 0, &filename);

	dao_update_property(getThis(), SL("_filename"), filename);

	intern = dao_storage_datrie_object_from_obj(Z_OBJ_P(getThis()));
	if (dao_file_exists(filename) == SUCCESS) {
		intern->trie = trie_new_from_file(Z_STRVAL_P(filename));
		if (!intern->trie) {
			DAO_THROW_EXCEPTION_FORMAT(dao_storage_exception_ce, "Unable to load %s", Z_STRVAL_P(filename));
			return;
		}
	} else {
		AlphaMap *alpha_map = alpha_map_new();
		if (!alpha_map) {
			DAO_THROW_EXCEPTION_STR(dao_storage_exception_ce, "Unable to create alpha map");
			return;
		}
		/*
		// a-z
		if (alpha_map_add_range (alpha_map, 0x0061, 0x007a) != 0) {
			alpha_map_free (alpha_map);
			DAO_THROW_EXCEPTION_STR(dao_storage_exception_ce, "Unable to create alpha map");
			return;
		}
		// A-Z
		if (alpha_map_add_range (alpha_map, 0x0041, 0x005a) != 0) {
			alpha_map_free (alpha_map);
			DAO_THROW_EXCEPTION_STR(dao_storage_exception_ce, "Unable to create alpha map");
			return;
		}
		*/
		if (alpha_map_add_range(alpha_map, 0x00000000, 0xffffffff) != 0) {
			alpha_map_free(alpha_map);
			DAO_THROW_EXCEPTION_STR(dao_storage_exception_ce, "Unable to create alpha map");
			return;
		}
		
		intern->trie = trie_new(alpha_map);
		alpha_map_free(alpha_map);
		if (!intern->trie) {
			DAO_THROW_EXCEPTION_STR(dao_storage_exception_ce, "Unable to create new trie");
			return;
		}
	}
}

static int dao_storage_datrie_search(zval *return_value, Trie *trie, const AlphaChar *text, zend_bool all)
{
	TrieState *s;
	const AlphaChar *p;
	const AlphaChar *base;
	int ret = 0;

	base = text;
	if (! (s = trie_root(trie))) {
		return -1;
	}

	while (*text) {   
		p = text;
		if(! trie_state_is_walkable(s, *p)) {
			trie_state_rewind(s);
			text++;
			continue;
		}

		while(*p && trie_state_is_walkable(s, *p) && ! trie_state_is_leaf(s)) {
			trie_state_walk(s, *p++);  
			if (trie_state_is_terminal(s)) {
				zval word = {};
				array_init(&word);
				add_next_index_long(&word, text - base);
				add_next_index_long(&word, p - text);
				add_next_index_zval(return_value, &word);
				ret++;
				if (!all) {
					return ret;
				}
			}		
		}
		trie_state_rewind(s);
		text++;
	}
	trie_state_free(s);

	return ret;
}

/**
 * Search
 *
 * @param string $str
 * @param boolean $all
 * @return array|boolean
 */
PHP_METHOD(Dao_Storage_Datrie, search)
{
	zval *str, *all = NULL;
	AlphaChar *alpha_text;
	dao_storage_datrie_object *intern;
	int i, ret;

	dao_fetch_params(0, 1, 1, &str, &all);

	if (!all) {
		all = &DAO_GLOBAL(z_false);
	}

	if (DAO_IS_EMPTY(str)) {
		RETURN_FALSE;
	}

	array_init(return_value);

	intern = dao_storage_datrie_object_from_obj(Z_OBJ_P(getThis()));

	alpha_text = emalloc(sizeof(AlphaChar) * (Z_STRLEN_P(str) + 1));

	for (i = 0; i < Z_STRLEN_P(str); i++) {
		alpha_text[i] = (AlphaChar) Z_STRVAL_P(str)[i];
	}

	alpha_text[Z_STRLEN_P(str)] = TRIE_CHAR_TERM;

	ret = dao_storage_datrie_search(return_value, intern->trie, alpha_text, zend_is_true(all));
	efree(alpha_text);
	if (ret <= 0) {
		zval_ptr_dtor(return_value);
		RETURN_FALSE;
	}
}

/**
 * Add
 *
 * @param string $keyword
 */
PHP_METHOD(Dao_Storage_Datrie, add)
{
	zval *keyword, *value = NULL;
	AlphaChar *alpha_key;
	TrieData data;
	dao_storage_datrie_object *intern;
	int i;

	dao_fetch_params(0, 1, 1, &keyword, &value);

	if (DAO_IS_EMPTY(keyword)) {
		RETURN_FALSE;
	}
	data = value ? Z_LVAL_P(value) : TRIE_DATA_ERROR;

	intern = dao_storage_datrie_object_from_obj(Z_OBJ_P(getThis()));

	alpha_key = emalloc(sizeof(AlphaChar) * (Z_STRLEN_P(keyword) + 1));

	for (i = 0; i < Z_STRLEN_P(keyword); i++) {
		alpha_key[i] = (AlphaChar) Z_STRVAL_P(keyword)[i];
	}

	alpha_key[Z_STRLEN_P(keyword)] = TRIE_CHAR_TERM;

	if (!trie_store(intern->trie, alpha_key, data)) {
        RETVAL_FALSE;
    }
    RETVAL_TRUE;
	efree(alpha_key);
}

/**
 * Query
 *
 * @param string $keyword
 * @return int|boolean
 */
PHP_METHOD(Dao_Storage_Datrie, query)
{
	zval *keyword;
	AlphaChar *alpha_key;
	TrieData data;
	dao_storage_datrie_object *intern;
	int i;

	dao_fetch_params(0, 1, 0, &keyword);

	if (DAO_IS_EMPTY(keyword)) {
		RETURN_FALSE;
	}

	intern = dao_storage_datrie_object_from_obj(Z_OBJ_P(getThis()));

	alpha_key = emalloc(sizeof(AlphaChar) * (Z_STRLEN_P(keyword) + 1));

	for (i = 0; i < Z_STRLEN_P(keyword); i++) {
		alpha_key[i] = (AlphaChar) Z_STRVAL_P(keyword)[i];
	}

	alpha_key[Z_STRLEN_P(keyword)] = TRIE_CHAR_TERM;

	if (trie_retrieve(intern->trie, alpha_key, &data)) {
		RETVAL_LONG(data);
	} else {
		RETVAL_FALSE;
	}
	efree(alpha_key);
}

/**
 * Delete
 *
 * @param string $keyword
 * @return boolean
 */
PHP_METHOD(Dao_Storage_Datrie, delete)
{
	zval *keyword;
	AlphaChar *alpha_key;
	dao_storage_datrie_object *intern;
	int i;

	dao_fetch_params(0, 1, 0, &keyword);

	if (DAO_IS_EMPTY(keyword)) {
		RETURN_FALSE;
	}

	intern = dao_storage_datrie_object_from_obj(Z_OBJ_P(getThis()));

	alpha_key = emalloc(sizeof(AlphaChar) * (Z_STRLEN_P(keyword) + 1));

	for (i = 0; i < Z_STRLEN_P(keyword); i++) {
		alpha_key[i] = (AlphaChar) Z_STRVAL_P(keyword)[i];
	}

	alpha_key[Z_STRLEN_P(keyword)] = TRIE_CHAR_TERM;

	if (!trie_delete(intern->trie, alpha_key)) {
		RETVAL_FALSE;
	} else {
		RETVAL_TRUE;
	}
	efree(alpha_key);
}

/**
 * Save
 *
 * @return boolean
 */
PHP_METHOD(Dao_Storage_Datrie, save)
{
	zval file = {};
	dao_storage_datrie_object *intern;

	dao_read_property(&file, getThis(), SL("_filename"), PH_NOISY|PH_READONLY);

	intern = dao_storage_datrie_object_from_obj(Z_OBJ_P(getThis()));

	if (trie_save(intern->trie, Z_STRVAL(file)) != 0) {
		RETVAL_FALSE;
	} else {
		RETVAL_TRUE;
	}
}
