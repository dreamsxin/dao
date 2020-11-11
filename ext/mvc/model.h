
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

#ifndef DAO_MVC_MODEL_H
#define DAO_MVC_MODEL_H

#include "php_dao.h"

#define DAO_MODEL_OP_NONE					0
#define DAO_MODEL_OP_CREATE					1
#define DAO_MODEL_OP_UPDATE					2
#define DAO_MODEL_OP_DELETE					3

#define DAO_MODEL_DIRTY_STATE_PERSISTEN		0
#define DAO_MODEL_DIRTY_STATE_TRANSIENT		1
#define DAO_MODEL_DIRTY_STATE_DETACHED		2

extern zend_class_entry *dao_mvc_model_ce;

DAO_INIT_CLASS(Dao_Mvc_Model);

#endif /* DAO_MVC_MODEL_H */
