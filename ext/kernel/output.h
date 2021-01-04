
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

#ifndef DAO_KERNEL_OUTPUT_H
#define DAO_KERNEL_OUTPUT_H

#include "php_dao.h"

void dao_ob_start();
void dao_ob_get_contents(zval *result);
int dao_ob_end_flush();
int dao_ob_end_clean();
int dao_ob_flush();
int dao_ob_clean();
int dao_ob_get_clean(zval *result);
int dao_ob_get_level();

#endif /* DAO_KERNEL_OUTPUT_H */
