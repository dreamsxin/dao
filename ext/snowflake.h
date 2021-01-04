
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

#ifndef DAO_SNOWFLAKE_H
#define DAO_SNOWFLAKE_H

#include "php_dao.h"

#include "kernel/shm.h"

typedef struct {
	uint32_t sequence;
	uint64_t timestamp;
} snowflake_data_t;

typedef struct {
	snowflake_data_t *data;
	dao_shared_memory *shm;
	zend_object std;
} dao_snowflake_object;

static inline dao_snowflake_object *dao_snowflake_object_from_obj(zend_object *obj) {
	return (dao_snowflake_object*)((char*)(obj) - XtOffsetOf(dao_snowflake_object, std));
}

extern zend_class_entry *dao_snowflake_ce;

DAO_INIT_CLASS(Dao_Snowflake);

#endif /* DAO_SNOWFLAKE_H */
