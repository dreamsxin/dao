
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

#ifndef DAO_FORMS_FORM_H
#define DAO_FORMS_FORM_H

#include "php_dao.h"

#define DAO_FROM_VALUES_RAW	1
#define DAO_FROM_VALUES_AS_ARRAY	2

extern zend_class_entry *dao_forms_form_ce;

DAO_INIT_CLASS(Dao_Forms_Form);

#endif /* DAO_FORMS_FORM_H */
