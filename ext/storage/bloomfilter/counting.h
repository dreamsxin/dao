
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

#ifndef DAO_STORAGE_BLOOMFILTER_COUNTING_H
#define DAO_STORAGE_BLOOMFILTER_COUNTING_H

#include "php_dao.h"
#include "kernel/countingbloomfilter.h"

#ifdef ZEND_ENABLE_ZVAL_LONG64
typedef struct _dao_storage_bloomfilter_counting_object {
	counting_bloom_t* bloomfilter;
	zval filename;
	zend_object std;
} dao_storage_bloomfilter_counting_object;

static inline dao_storage_bloomfilter_counting_object *dao_storage_bloomfilter_counting_object_from_obj(zend_object *obj) {
	return (dao_storage_bloomfilter_counting_object*)((char*)(obj) - XtOffsetOf(dao_storage_bloomfilter_counting_object, std));
}

extern zend_class_entry *dao_storage_bloomfilter_counting_ce;

DAO_INIT_CLASS(Dao_Storage_Bloomfilter_Counting);
#endif

#endif /* DAO_STORAGE_BLOOMFILTER_COUNTING_H */
