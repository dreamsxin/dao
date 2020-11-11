
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
  |          Nikolaos Dimopoulos <nikos@phalconphp.com>                    |
  +------------------------------------------------------------------------+
*/

#ifndef DAO_TAG_H
#define DAO_TAG_H

#include "php_dao.h"

void dao_tag_render_attributes(zval *code, zval *attributes);

extern zend_class_entry *dao_tag_ce;

DAO_INIT_CLASS(Dao_Tag);

#endif /* DAO_TAG_H */
