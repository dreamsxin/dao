
/*
  +------------------------------------------------------------------------+
  | Dao Framework                                                          |
  +------------------------------------------------------------------------+
  | Copyright (c) 2011-2013 Phalcon Team (http://www.phalconphp.com)       |
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
#ifndef DAO_HTTP_CLIENT_HEADER_H
#define DAO_HTTP_CLIENT_HEADER_H

#include "php_dao.h"

#define DAO_HTTP_CLIENT_HEADER_BUILD_STATUS    1
#define DAO_HTTP_CLIENT_HEADER_BUILD_FIELDS    2

extern zend_class_entry *dao_http_client_header_ce;

DAO_INIT_CLASS(Dao_Http_Client_Header);

#endif /* DAO_HTTP_CLIENT_HEADER_H */

