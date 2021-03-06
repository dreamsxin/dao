
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

#ifndef DAO_BINARY_H
#define DAO_BINARY_H

#include "php_dao.h"

#define DAO_BINARY_ENDIAN_MACHINE		0
#define DAO_BINARY_ENDIAN_BIG			1
#define DAO_BINARY_ENDIAN_LITTLE		2

#define DAO_BINARY_TYPE_CHAR				0
#define DAO_BINARY_TYPE_UNSIGNEDCHAR		1
#define DAO_BINARY_TYPE_INT16				2
#define DAO_BINARY_TYPE_UNSIGNEDINT16		3
#define DAO_BINARY_TYPE_INT					4
#define DAO_BINARY_TYPE_UNSIGNEDINT			5
#define DAO_BINARY_TYPE_INT32				6
#define DAO_BINARY_TYPE_UNSIGNEDINT32		7
#define DAO_BINARY_TYPE_FLOAT				8
#define DAO_BINARY_TYPE_DOUBLE				9
#define DAO_BINARY_TYPE_STRING				10
#define DAO_BINARY_TYPE_HEXSTRING			11


#define DAO_BINARY_SETBIT(data, n)   (data[n/8] |= (0x80 >> (n%8)))
#define DAO_BINARY_UNSETBIT(data, n)   (data[n/8] &= ~(0x80 >> (n%8)))
#define DAO_BINARY_REVBIT(data, n)   (data[n/8] ^= (0x80 >> (n%8)))
#define DAO_BINARY_CHECKBIT(data, n)   (data[n/8] & (0x80 >> (n%8)))

extern zend_class_entry *dao_binary_ce;

DAO_INIT_CLASS(Dao_Binary);

#endif /* DAO_BINARY_H */
