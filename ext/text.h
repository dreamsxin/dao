
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

#ifndef DAO_TEXT_H
#define DAO_TEXT_H

#include "php_dao.h"

#define DAO_TEXT_RANDOM_ALNUM    0
#define DAO_TEXT_RANDOM_ALPHA    1
#define DAO_TEXT_RANDOM_HEXDEC   2
#define DAO_TEXT_RANDOM_NUMERIC  3
#define DAO_TEXT_RANDOM_NOZERO   4

extern zend_class_entry *dao_text_ce;

DAO_INIT_CLASS(Dao_Text);

#endif /* DAO_TEXT_H */
