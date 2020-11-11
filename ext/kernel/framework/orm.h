
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

void dao_orm_destroy_cache();
void dao_orm_get_prepared_ast(zval *return_value, zval *unique_id);
void dao_orm_set_prepared_ast(zval *unique_id, zval *prepared_ast);
void dao_orm_singlequotes(zval *return_value, zval *str);

void dao_orm_phql_build_group(zval *return_value, zval *group);
void dao_orm_phql_build_order(zval *return_value, zval *order);
void dao_orm_phql_build_limit(zval *return_value, zval *limit);