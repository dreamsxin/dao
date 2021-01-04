
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

#ifndef DAO_ARR_H
#define DAO_ARR_H

#include "php_dao.h"
             
#define DAO_ARR_AGGR_FIRST		1
#define DAO_ARR_AGGR_LAST		2
#define DAO_ARR_AGGR_COUNT		3  
#define DAO_ARR_AGGR_AVG		4
#define DAO_ARR_AGGR_SUM		5
#define DAO_ARR_AGGR_GROUP		6
#define DAO_ARR_AGGR_MIN		7
#define DAO_ARR_AGGR_MAX		8

#define DAO_ARR_TYPE_BOOLEAN	1
#define DAO_ARR_TYPE_LONG		2
#define DAO_ARR_TYPE_DOUBLE		3
#define DAO_ARR_TYPE_STRING		4

#define DAO_ARR_HASH_ADD_NEW(ht, num_idx, str_idx, val) \
      (str_idx) \
      ? zend_hash_add_new(ht, str_idx, val) \
      : zend_hash_index_add_new(ht, num_idx, val)

#define DAO_ARR_HASH_UPDATE(ht, num_idx, str_idx, val) \
      (str_idx) \
      ? zend_hash_update(ht, str_idx, val) \
      : zend_hash_index_update(ht, num_idx, val)

#define DAO_ARR_HASH_FIND(ht, num_idx, str_idx) \
      (str_idx) \
      ? zend_hash_find(ht, str_idx) \
: zend_hash_index_find(ht, num_idx)

typedef struct _dao_arr_aggregator {

  zend_string *alias;
  zend_string *selector;
  zend_ulong num_alias;
  zend_ulong num_selector;

  zend_bool is_callable;
  uint isa;

  zval *type; // could be a constant or a function call.
  zend_fcall_info fci;
  zend_fcall_info_cache fci_cache;

} dao_arr_aggregator;

extern zend_class_entry *dao_arr_ce;

DAO_INIT_CLASS(Dao_Arr);

#endif /* DAO_ARR_H */
