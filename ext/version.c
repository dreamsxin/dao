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
  |          Nikolaos Dimopoulos <nikos@niden.net>                         |
  |          ZhuZongXin <dreamsxin@qq.com>                                 |
  +------------------------------------------------------------------------+
*/

#include "version.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"
#include "kernel/array.h"
#include "kernel/concat.h"
#include "kernel/string.h"
#include "kernel/operators.h"

/**
 * Dao\Version
 *
 * This class allows to get the installed version of the framework
 */
zend_class_entry *dao_version_ce;

PHP_METHOD(Dao_Version, _getVersion);
PHP_METHOD(Dao_Version, get);
PHP_METHOD(Dao_Version, getId);

static const zend_function_entry dao_version_method_entry[] = {
	PHP_ME(Dao_Version, _getVersion, arginfo_empty, ZEND_ACC_PROTECTED|ZEND_ACC_STATIC)
	PHP_ME(Dao_Version, get, arginfo_empty, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Dao_Version, getId, arginfo_empty, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};

/**
 * Dao\Version initializer
 */
DAO_INIT_CLASS(Dao_Version){

	DAO_REGISTER_CLASS(Dao, Version, version, dao_version_method_entry, 0);

	return SUCCESS;
}

/**
 * Area where the version number is set. The format is as follows:
 * ABBCCDE
 *
 * A - Major version
 * B - Med version (two digits)
 * C - Min version (two digits)
 * D - Special release: 1 = Alpha, 2 = Beta, 3 = RC, 4 = Stable
 * E - Special release version i.e. RC1, Beta2 etc.
 */
PHP_METHOD(Dao_Version, _getVersion){

	array_init_size(return_value, 5);
	add_next_index_long(return_value, PHP_DAO_VERSION_MAJOR);
	add_next_index_long(return_value, PHP_DAO_VERSION_MED);
	add_next_index_long(return_value, PHP_DAO_VERSION_MIN);
	add_next_index_long(return_value, PHP_DAO_VERSION_RELEASE);
	add_next_index_long(return_value, PHP_DAO_VERSION_RELEASE_VERSION);
}

/**
 * Returns the active version (string)
 *
 * <code>
 * echo Dao\Version::get();
 * </code>
 *
 * @return string
 */
PHP_METHOD(Dao_Version, get){

	zval version = {}, major = {}, medium = {}, minor = {}, special = {}, special_number = {}, result = {}, suffix = {};

	DAO_MM_INIT();
	DAO_MM_CALL_CE_STATIC(&version, dao_version_ce, "_getversion");
	DAO_MM_ADD_ENTRY(&version);

	dao_array_fetch_long(&major, &version, 0, PH_NOISY|PH_READONLY);
	dao_array_fetch_long(&medium, &version, 1, PH_NOISY|PH_READONLY);
	dao_array_fetch_long(&minor, &version, 2, PH_NOISY|PH_READONLY);
	dao_array_fetch_long(&special, &version, 3, PH_NOISY|PH_READONLY);
	dao_array_fetch_long(&special_number, &version, 4, PH_NOISY|PH_READONLY);

	DAO_CONCAT_VSVSVS(&result, &major, ".", &medium, ".", &minor, " ");
	DAO_MM_ADD_ENTRY(&result);
	switch (dao_get_intval(&special)) {

		case DAO_VERSION_ALPHA:
			DAO_CONCAT_SV(&suffix, "ALPHA ", &special_number);
			break;

		case DAO_VERSION_BETA:
			DAO_CONCAT_SV(&suffix, "BETA ", &special_number);
			break;

		case DAO_VERSION_RC:
			DAO_CONCAT_SV(&suffix, "RC ", &special_number);
			break;

		default:
			ZVAL_EMPTY_STRING(&suffix);
			break;

	}
	DAO_MM_ADD_ENTRY(&suffix);
	dao_concat_self(&result, &suffix);
	DAO_MM_ADD_ENTRY(&result);
	ZVAL_STR(return_value, dao_trim(&result, NULL, DAO_TRIM_BOTH));
	RETURN_MM();
}

/**
 * Returns the numeric active version
 *
 * <code>
 * echo Dao\Version::getId();
 * </code>
 *
 * @return int
 */
PHP_METHOD(Dao_Version, getId){

	zval version = {}, major = {}, medium = {}, minor = {}, special = {}, special_number = {}, format = {}, real_medium = {}, real_minor = {};

	DAO_CALL_CE_STATIC(&version, dao_version_ce, "_getversion");
	dao_array_fetch_long(&major, &version, 0, PH_NOISY|PH_READONLY);
	dao_array_fetch_long(&medium, &version, 1, PH_NOISY|PH_READONLY);
	dao_array_fetch_long(&minor, &version, 2, PH_NOISY|PH_READONLY);
	dao_array_fetch_long(&special, &version, 3, PH_NOISY|PH_READONLY);
	dao_array_fetch_long(&special_number, &version, 4, PH_NOISY|PH_READONLY);

	ZVAL_STRING(&format, "%02s");

	DAO_CALL_FUNCTION(&real_medium, "sprintf", &format, &medium);
	DAO_CALL_FUNCTION(&real_minor, "sprintf", &format, &minor);
	zval_ptr_dtor(&format);

	DAO_CONCAT_VVVVV(return_value, &major, &real_medium, &real_minor, &special, &special_number);
	zval_ptr_dtor(&real_medium);
	zval_ptr_dtor(&real_minor);
	zval_ptr_dtor(&version);
}
