
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
  |          ZhuZongXin <dreamsxin@qq.com>                                 |
  |          Julien Salleyron <julien.salleyron@gmail.com>                 |
  |          <pangudashu@gmail.com>                                        |
  +------------------------------------------------------------------------+
*/

#ifndef DAO_AOP_H
#define DAO_AOP_H

#include "php_dao.h"
#include "kernel/debug.h"

#include <ext/pcre/php_pcre.h>

#define DAO_AOP_KIND_AROUND		1
#define DAO_AOP_KIND_BEFORE		2
#define DAO_AOP_KIND_AFTER		4
#define DAO_AOP_KIND_READ		8
#define DAO_AOP_KIND_WRITE 		16
#define DAO_AOP_KIND_PROPERTY	32
#define DAO_AOP_KIND_METHOD		64
#define DAO_AOP_KIND_FUNCTION	128
#define DAO_AOP_KIND_CATCH		256
#define DAO_AOP_KIND_RETURN		512

#define DAO_AOP_KIND_AROUND_READ_PROPERTY  (DAO_AOP_KIND_AROUND + DAO_AOP_KIND_READ + DAO_AOP_KIND_PROPERTY)
#define DAO_AOP_KIND_AROUND_WRITE_PROPERTY (DAO_AOP_KIND_AROUND + DAO_AOP_KIND_WRITE + DAO_AOP_KIND_PROPERTY)
#define DAO_AOP_KIND_BEFORE_READ_PROPERTY  (DAO_AOP_KIND_BEFORE + DAO_AOP_KIND_READ + DAO_AOP_KIND_PROPERTY)
#define DAO_AOP_KIND_BEFORE_WRITE_PROPERTY (DAO_AOP_KIND_BEFORE + DAO_AOP_KIND_WRITE + DAO_AOP_KIND_PROPERTY)
#define DAO_AOP_KIND_AFTER_READ_PROPERTY   (DAO_AOP_KIND_AFTER + DAO_AOP_KIND_READ + DAO_AOP_KIND_PROPERTY)
#define DAO_AOP_KIND_AFTER_WRITE_PROPERTY  (DAO_AOP_KIND_AFTER + DAO_AOP_KIND_WRITE + DAO_AOP_KIND_PROPERTY)

#define DAO_AOP_KIND_AROUND_METHOD   (DAO_AOP_KIND_AROUND + DAO_AOP_KIND_METHOD)
#define DAO_AOP_KIND_AROUND_FUNCTION (DAO_AOP_KIND_AROUND + DAO_AOP_KIND_FUNCTION)
#define DAO_AOP_KIND_BEFORE_METHOD   (DAO_AOP_KIND_BEFORE + DAO_AOP_KIND_METHOD)
#define DAO_AOP_KIND_BEFORE_FUNCTION (DAO_AOP_KIND_BEFORE + DAO_AOP_KIND_FUNCTION)
#define DAO_AOP_KIND_AFTER_METHOD    (DAO_AOP_KIND_AFTER + DAO_AOP_KIND_METHOD)
#define DAO_AOP_KIND_AFTER_FUNCTION  (DAO_AOP_KIND_AFTER + DAO_AOP_KIND_FUNCTION)

typedef struct {
	int scope;
	int static_state;

	zend_string *class_name;
	int class_jok;
	
	zend_string *method;
	int method_jok;
	
	zend_string *selector;
	int kind_of_advice;
	zend_fcall_info fci;
	zend_fcall_info_cache fci_cache;
#if PHP_VERSION_ID < 70300
	pcre *re_method;
	pcre *re_class;
#else
	pcre2_code *re_method;
	pcre2_code *re_class;
#endif
} dao_aop_pointcut;

typedef struct {
    zend_array *ht;
    int version;
    zend_class_entry *ce;
} dao_aop_pointcut_cache;

ZEND_API void dao_aop_execute_internal(zend_execute_data *execute_data, zval *return_value);
ZEND_API void dao_aop_execute_ex(zend_execute_data *execute_data);

void dao_aop_do_func_execute(HashPosition pos, zend_array *pointcut_table, zend_execute_data *ex, zval *object);
void dao_aop_do_read_property(HashPosition pos, zend_array *pointcut_table, zval *aop_object);
void dao_aop_do_write_property(HashPosition pos, zend_array *pointcut_table, zval *aop_object);

zval *dao_aop_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv);
zval *dao_aop_write_property(zval *object, zval *member, zval *value, void **cache_slot);
zval *dao_aop_get_property_ptr_ptr(zend_object *object, zend_string *member, int type, void **cache_slot);

void dao_aop_free_pointcut(zval *elem);
void dao_aop_free_pointcut_cache(zval *elem);

extern zend_object_read_property_t    original_zend_std_read_property;
extern zend_object_write_property_t   original_zend_std_write_property;
extern zend_object_get_property_ptr_ptr_t	original_zend_std_get_property_ptr_ptr;

extern zend_class_entry *dao_aop_ce;

DAO_INIT_CLASS(Dao_Aop);

#endif /* DAO_AOP_H */
