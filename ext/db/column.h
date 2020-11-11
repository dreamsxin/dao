
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

#ifndef DAO_DB_COLUMN_H
#define DAO_DB_COLUMN_H

#include "php_dao.h"

extern zend_class_entry *dao_db_column_ce;

DAO_INIT_CLASS(Dao_Db_Column);

#define DAO_DB_COLUMN_TYPE_INTEGER			0
#define DAO_DB_COLUMN_TYPE_BIGINTEGER		1
#define DAO_DB_COLUMN_TYPE_FLOAT			2
#define DAO_DB_COLUMN_TYPE_DECIMAL			3
#define DAO_DB_COLUMN_TYPE_DATE				4
#define DAO_DB_COLUMN_TYPE_DATETIME			5
#define DAO_DB_COLUMN_TYPE_TIMESTAMP		6
#define DAO_DB_COLUMN_TYPE_CHAR				7
#define DAO_DB_COLUMN_TYPE_VARCHAR			8
#define DAO_DB_COLUMN_TYPE_TEXT				9
#define DAO_DB_COLUMN_TYPE_BOOLEAN			10
#define DAO_DB_COLUMN_TYPE_DOUBLE			11
#define DAO_DB_COLUMN_TYPE_TINYBLOB			12
#define DAO_DB_COLUMN_TYPE_MEDIUMBLOB		13
#define DAO_DB_COLUMN_TYPE_LONGBLOB			14
#define DAO_DB_COLUMN_TYPE_BLOB				15
#define DAO_DB_COLUMN_TYPE_JSON				16
#define DAO_DB_COLUMN_TYPE_JSONB			17
#define DAO_DB_COLUMN_TYPE_ARRAY			18
#define DAO_DB_COLUMN_TYPE_BYTEA			19
#define DAO_DB_COLUMN_TYPE_MONEY			20
#define DAO_DB_COLUMN_TYPE_INT_ARRAY		21
#define DAO_DB_COLUMN_TYPE_OTHER			100

#define DAO_DB_COLUMN_BIND_PARAM_NULL		0
#define DAO_DB_COLUMN_BIND_PARAM_INT		1
#define DAO_DB_COLUMN_BIND_PARAM_STR		2
#define DAO_DB_COLUMN_BIND_PARAM_BLOD		3
#define DAO_DB_COLUMN_BIND_PARAM_BOOL		5
#define DAO_DB_COLUMN_BIND_PARAM_DECIMAL	32
#define DAO_DB_COLUMN_BIND_SKIP				1024

#endif /* DAO_DB_COLUMN_H */
