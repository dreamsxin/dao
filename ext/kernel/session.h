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
  |          Vladimir Kolesnikov <vladimir@extrememember.com>              |
  +------------------------------------------------------------------------+
*/

#ifndef DAO_KERNEL_SESSION_H
#define DAO_KERNEL_SESSION_H

#include "php_dao.h"

#define IF_SESSION_VARS() \
	if (Z_ISREF_P(&PS(http_session_vars)) && Z_TYPE_P(Z_REFVAL(PS(http_session_vars))) == IS_ARRAY)

#define SESSION_CHECK_ACTIVE_STATE	\
	if (PS(session_status) == php_session_active) {	\
		php_error_docref(NULL, E_WARNING, "A session is active. You cannot change the session module's ini settings at this time");	\
		return FAILURE;	\
	}

int dao_session_start() DAO_ATTR_WARN_UNUSED_RESULT;
int dao_session_regenerate_id(zend_bool delete_old_session) DAO_ATTR_WARN_UNUSED_RESULT;
int dao_session_destroy() DAO_ATTR_WARN_UNUSED_RESULT;
int dao_get_session_id(zval *return_value) DAO_ATTR_WARN_UNUSED_RESULT;
int dao_set_session_id(zval *sid) DAO_ATTR_WARN_UNUSED_RESULT;
int dao_session_write_close() DAO_ATTR_WARN_UNUSED_RESULT;
int dao_session_set(zval *name, zval *val);
zval* dao_session_get(zval *name);

#endif /* DAO_KERNEL_SESSION_H */
