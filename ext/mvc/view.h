
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

#ifndef DAO_MVC_VIEW_H
#define DAO_MVC_VIEW_H

#include "php_dao.h"

/* Render level constraints */
#define DAO_VIEW_LEVEL_NO_RENDER        0
#define DAO_VIEW_LEVEL_ACTION           1
#define DAO_VIEW_LEVEL_BEFORE_TEMPLATE  2
#define DAO_VIEW_LEVEL_CONTROLLER       3
#define DAO_VIEW_LEVEL_NAMESPACE        4
#define DAO_VIEW_LEVEL_AFTER_TEMPLATE   5
#define DAO_VIEW_LEVEL_MAIN             6

extern zend_class_entry *dao_mvc_view_ce;

DAO_INIT_CLASS(Dao_Mvc_View);

#endif /* DAO_MVC_VIEW_H */
