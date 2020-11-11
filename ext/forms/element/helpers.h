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

#ifndef DAO_FORMS_ELEMENT_HELPERS_H
#define DAO_FORMS_ELEMENT_HELPERS_H

#include "php_dao.h"
#include "kernel/main.h"
#include "kernel/fcall.h"
#include "tag.h"

static inline void dao_forms_element_render_helper(const char *method, int use_checked, INTERNAL_FUNCTION_PARAMETERS)
{
	zval *attributes = NULL, widget_attributes = {}, *uc, *params[1];

	dao_fetch_params(0, 0, 1, &attributes);

	if (!attributes) {
		attributes = &DAO_GLOBAL(z_null);
	}

	uc = use_checked ? &DAO_GLOBAL(z_true) : &DAO_GLOBAL(z_false);

	DAO_CALL_METHOD(&widget_attributes, getThis(), "prepareattributes", attributes, uc);

	params[0] = &widget_attributes;
	if (FAILURE == dao_call_ce(return_value, dao_tag_ce, method, 1, params)) {
		;
	}
	zval_ptr_dtor(&widget_attributes);
}

#endif /* DAO_FORMS_ELEMENT_HELPERS_H */
