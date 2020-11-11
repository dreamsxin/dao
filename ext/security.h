
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

#ifndef DAO_SECURITY_H
#define DAO_SECURITY_H

#include "php_dao.h"

extern zend_class_entry *dao_security_ce;

#define DAO_SECURITY_CRYPT_DEFAULT          0
#define DAO_SECURITY_CRYPT_STD_DES          1
#define DAO_SECURITY_CRYPT_EXT_DES          2
#define DAO_SECURITY_CRYPT_MD5              3
#define DAO_SECURITY_CRYPT_BLOWFISH         4
#define DAO_SECURITY_CRYPT_BLOWFISH_A       5
#define DAO_SECURITY_CRYPT_BLOWFISH_X       6
#define DAO_SECURITY_CRYPT_BLOWFISH_Y       7
#define DAO_SECURITY_CRYPT_SHA256           8
#define DAO_SECURITY_CRYPT_SHA512           9

DAO_INIT_CLASS(Dao_Security);

#endif /* DAO_SECURITY_H */
