
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
  +------------------------------------------------------------------------+
*/

#ifndef DAO_KERNEL_MAIN_H
#define DAO_KERNEL_MAIN_H

#include "php_dao.h"

#include <Zend/zend_types.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include <ext/spl/spl_exceptions.h>
#include <ext/spl/spl_iterators.h>

#include "kernel/memory.h"
#include "kernel/backtrace.h"

#include "internal/arginfo.h"


#if PHP_VERSION_ID >= 80000
# ifndef ZEND_ACC_DTOR
#  define ZEND_ACC_DTOR 0
# endif
# ifndef instanceof_function_ex
#  define instanceof_function_ex(class_ce, interface_ce, loss) instanceof_function(class_ce, interface_ce)
# endif

#endif

/** Main macros */
#define PH_DEBUG		0

#define PH_DECLARED		2
#define PH_DYNAMIC		4
#define PH_BOTH			6

#define PH_NOISY		256
#define PH_SILENT		512
#define PH_READONLY		1024

#define PH_SEPARATE		2048
#define PH_COPY			4096
#define PH_CTOR			8192

#define SL(str)   (str), (sizeof(str)-1)
#define SS(str)   (str), (sizeof(str))
#define IS(str)   (dao_interned_##str)
#define ISV(str)  (dao_interned_##str)->val
#define ISL(str)  (dao_interned_##str)->val, (sizeof(#str)-1)
#define ISS(str)  (dao_interned_##str)->val, (sizeof(#str))
#define SSL(str)   zend_string_init(SL(str), 0)
#define SSS(str)   zend_string_init(SS(str), 0)

/* Startup functions */
void php_dao_init_globals(zend_dao_globals *dao_globals);
zend_class_entry *dao_register_internal_interface_ex(zend_class_entry *orig_ce, zend_class_entry *parent_ce);

/* Globals functions */
int dao_read_global_str(zval *return_value, const char *global, unsigned int global_length);
zval* dao_get_global_str(const char *global, unsigned int global_length);
zval* dao_get_global(zend_string *name);

int dao_is_callable(zval *var);
int dao_function_exists_ex(const char *func_name, unsigned int func_len);

DAO_ATTR_NONNULL static inline zend_function *dao_fetch_function_str(const char *function_name, unsigned int function_len)
{
	return zend_hash_str_find_ptr(EG(function_table), function_name, function_len+1);
}

DAO_ATTR_NONNULL static inline zend_function *dao_fetch_function(zend_string *function_name)
{
	return zend_hash_find_ptr(EG(function_table), function_name);
}

/* Fetch Parameters */
int dao_fetch_parameters(int num_args, int required_args, int optional_args, ...);

int dao_has_constant(const char *name, size_t name_len);
int dao_get_constant(zval *retval, const char *name, size_t name_len);

#define DAO_TYPE_P(var)	 (Z_ISREF_P(var) ? Z_TYPE_P(Z_REFVAL_P(var)) : Z_TYPE_P(var))

/* types */
void dao_gettype(zval *return_value, zval *arg);

#define DAO_MM_INIT() zval dao_memory_entry = {}; array_init(&dao_memory_entry);
#define DAO_MM_DEINIT() zval_ptr_dtor(&dao_memory_entry);
#define DAO_MM_ADD_ENTRY(var) dao_array_append(&dao_memory_entry, var, 0);

#define DAO_MM_ZVAL_EMPTY_STRING(z) \
	do { \
		ZVAL_EMPTY_STRING(z); \
		dao_array_append(&dao_memory_entry, z, 0); \
	} while (0)

#define DAO_MM_ZVAL_STRING(z, s) \
	do { \
		ZVAL_STRING(z, s); \
		dao_array_append(&dao_memory_entry, z, 0); \
	} while (0)

#define DAO_MM_ZVAL_STRINGL(z, s, l) \
	do { \
		ZVAL_STRINGL(z, s, l); \
		dao_array_append(&dao_memory_entry, z, 0); \
	} while (0)

#define RETURN_MM_LONG(value) RETVAL_LONG(value); RETURN_MM();

#define RETURN_MM_STRING(s) { \
		RETVAL_STRING(s); \
		zval_ptr_dtor(&dao_memory_entry); \
	} \
	return;

/** Return zval with always not ctor */
#define RETURN_NCTOR(var) { \
		RETVAL_ZVAL(var, 0, 0); \
	} \
	return;

#define RETURN_MM_NCTOR(var) { \
		RETVAL_ZVAL(var, 0, 0); \
		zval_ptr_dtor(&dao_memory_entry); \
	} \
	return;

/** Return zval with always ctor */
#define RETURN_CTOR(var) { \
		RETVAL_ZVAL(var, 1, 0); \
	} \
	return;

#define RETURN_MM_CTOR(var) { \
		RETVAL_ZVAL(var, 1, 0); \
		zval_ptr_dtor(&dao_memory_entry); \
	} \
	return;

#define RETURN_CTOR_DTOR(var) { \
		RETVAL_ZVAL(var, 1, 1); \
	} \
	return;

#define RETURN_MM_CTOR_DTOR(var) { \
		RETVAL_ZVAL(var, 1, 1); \
		zval_ptr_dtor(&dao_memory_entry); \
	} \
	return;

/** Return this pointer */
#define RETURN_THIS() { \
		RETVAL_ZVAL(getThis(), 1, 0); \
	} \
	return;

#define RETURN_MM_THIS() { \
		RETVAL_ZVAL(getThis(), 1, 0); \
		zval_ptr_dtor(&dao_memory_entry); \
	} \
	return;

/**
 * Returns a zval in an object member
 */
#define RETURN_MEMBER(object, member_name) \
	dao_read_property(return_value, object, SL(member_name), PH_COPY); \
	return;

#define RETURN_MM_MEMBER(object, member_name) \
	dao_read_property(return_value, object, SL(member_name), PH_COPY); \
	zval_ptr_dtor(&dao_memory_entry); \
	return;

#define RETURN_STATIC_MEMBER(ce, member_name) \
	dao_read_static_property_ce(return_value, ce, SL(member_name), PH_COPY); \
	return;

#define RETURN_MM_STATIC_MEMBER(ce, member_name) \
	dao_read_static_property_ce(return_value, ce, SL(member_name), PH_COPY); \
	zval_ptr_dtor(&dao_memory_entry); \
	return;

#define RETURN_ON_FAILURE(what) \
	if (FAILURE == what) { \
		return; \
	}

#define RETURN_MM_ON_FAILURE(what) \
	if (FAILURE == what) { \
		zval_ptr_dtor(&dao_memory_entry); \
		return; \
	}

/** Return without change return_value */
#define RETURN_MM() zval_ptr_dtor(&dao_memory_entry); return;

/** Return bool restoring memory frame */
#define RETURN_MM_BOOL(value) RETVAL_BOOL(value); RETURN_MM();

/** Return null restoring memory frame */
#define RETURN_MM_NULL() RETVAL_NULL(); RETURN_MM();

/** Return bool restoring memory frame */
#define RETURN_MM_FALSE RETVAL_FALSE; RETURN_MM();
#define RETURN_MM_TRUE RETVAL_TRUE; RETURN_MM();

/** Return empty array */
#if PHP_VERSION_ID >= 70400
#define RETURN_MM_EMPTY_ARRAY() RETVAL_EMPTY_ARRAY(); RETURN_MM();
#else
# define RETURN_EMPTY_ARRAY() array_init(return_value); return;
# define RETURN_MM_EMPTY_ARRAY() array_init(return_value); RETURN_MM();
#endif

/** class/interface registering */
#define DAO_REGISTER_CLASS(ns, class_name, name, methods, flags) \
	{ \
		zend_class_entry ce; \
		INIT_NS_CLASS_ENTRY(ce, #ns, #class_name, methods); \
		dao_ ##name## _ce = zend_register_internal_class(&ce); \
		dao_ ##name## _ce->ce_flags |= flags;  \
	}

#define DAO_REGISTER_CLASS_CREATE_OBJECT(ns, class_name, name, methods, flags) \
	{ \
		zend_class_entry ce; \
		memcpy(&dao_ ##name## _object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers)); \
		dao_ ##name## _object_handlers.offset = XtOffsetOf(dao_ ##name## _object, std); \
		dao_ ##name## _object_handlers.free_obj = (zend_object_free_obj_t) dao_ ##name## _object_free_handler; \
		INIT_NS_CLASS_ENTRY(ce, #ns, #class_name, methods); \
		dao_ ##name## _ce = zend_register_internal_class(&ce); \
		dao_ ##name## _ce->create_object = dao_ ##name## _object_create_handler; \
		dao_ ##name## _ce->ce_flags |= flags;  \
	}

#define DAO_REGISTER_CLASS_EX(ns, class_name, lcname, parent_ce, methods, flags) \
	{ \
		zend_class_entry ce; \
		INIT_NS_CLASS_ENTRY(ce, #ns, #class_name, methods); \
		dao_ ##lcname## _ce = zend_register_internal_class_ex(&ce, parent_ce); \
		if (!dao_ ##lcname## _ce) { \
			fprintf(stderr, "Dao Error: Class to extend '%s' was not found when registering class '%s'\n", (parent_ce ? parent_ce->name->val : "(null)"), ZEND_NS_NAME(#ns, #class_name)); \
			return FAILURE; \
		} \
		dao_ ##lcname## _ce->ce_flags |= flags;  \
	}

#define DAO_REGISTER_CLASS_CREATE_OBJECT_EX(ns, class_name, lcname, parent_ce, methods, flags) \
	{ \
		zend_class_entry ce; \
		memcpy(&dao_ ##lcname## _object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers)); \
		dao_ ##lcname## _object_handlers.offset = XtOffsetOf(dao_ ##lcname## _object, std); \
		dao_ ##lcname## _object_handlers.free_obj = (zend_object_free_obj_t) dao_ ##lcname## _object_free_handler; \
		INIT_NS_CLASS_ENTRY(ce, #ns, #class_name, methods); \
		dao_ ##lcname## _ce = zend_register_internal_class_ex(&ce, parent_ce); \
		if (!dao_ ##lcname## _ce) { \
			fprintf(stderr, "Dao Error: Class to extend '%s' was not found when registering class '%s'\n", (parent_ce ? parent_ce->name->val : "(null)"), ZEND_NS_NAME(#ns, #class_name)); \
			return FAILURE; \
		} \
		dao_ ##lcname## _ce->create_object = dao_ ##lcname## _object_create_handler; \
		dao_ ##lcname## _ce->ce_flags |= flags;  \
	}

#define DAO_REGISTER_INTERFACE(ns, classname, name, methods) \
	{ \
		zend_class_entry ce; \
		INIT_NS_CLASS_ENTRY(ce, #ns, #classname, methods); \
		dao_ ##name## _ce = zend_register_internal_interface(&ce); \
	}

#define DAO_REGISTER_INTERFACE_EX(ns, classname, lcname, parent_ce, methods) \
	{ \
		zend_class_entry ce; \
		INIT_NS_CLASS_ENTRY(ce, #ns, #classname, methods); \
		dao_ ##lcname## _ce = dao_register_internal_interface_ex(&ce, parent_ce); \
		if (!dao_ ##lcname## _ce) { \
			fprintf(stderr, "Can't register interface %s with parent %s\n", ZEND_NS_NAME(#ns, #classname), (parent_ce ? parent_ce->name->val : "(null)")); \
			return FAILURE; \
		} \
	}

/** Method declaration for API generation */
#define DAO_DOC_METHOD(class_name, method)

/** Low overhead parse/fetch parameters */
#define dao_fetch_params(memory_grow, required_params, optional_params, ...) \
	zval dao_memory_entry = {}; \
	if (dao_fetch_parameters(ZEND_NUM_ARGS(), required_params, optional_params, __VA_ARGS__) == FAILURE) { \
		RETURN_NULL(); \
	} else if (memory_grow) { \
		array_init(&dao_memory_entry); \
	} else { \
		ZVAL_NULL(&dao_memory_entry); \
	}

#if PHP_VERSION_ID >= 80000
#define DAO_VERIFY_INTERFACE_EX(instance, interface_ce, exception_ce) \
	if (Z_TYPE_P(instance) != IS_OBJECT) { \
		zend_throw_exception_ex(exception_ce, 0, "Unexpected value type: expected object implementing %s, %s given", interface_ce->name->val, zend_zval_type_name(instance)); \
		return; \
	} else if (!zend_class_implements_interface(Z_OBJCE_P(instance), interface_ce)) { \
		zend_throw_exception_ex(exception_ce, 0, "Unexpected value type: expected object implementing %s, object of type %s given", interface_ce->name->val, Z_OBJCE_P(instance)->name->val); \
		return; \
	}
#else
#define DAO_VERIFY_INTERFACE_EX(instance, interface_ce, exception_ce) \
	if (Z_TYPE_P(instance) != IS_OBJECT) { \
		zend_throw_exception_ex(exception_ce, 0, "Unexpected value type: expected object implementing %s, %s given", interface_ce->name->val, zend_zval_type_name(instance)); \
		return; \
	} else if (!instanceof_function_ex(Z_OBJCE_P(instance), interface_ce, 1)) { \
		zend_throw_exception_ex(exception_ce, 0, "Unexpected value type: expected object implementing %s, object of type %s given", interface_ce->name->val, Z_OBJCE_P(instance)->name->val); \
		return; \
	}
#endif

#if PHP_VERSION_ID >= 80000
#define DAO_MM_VERIFY_INTERFACE_EX(instance, interface_ce, exception_ce) \
	if (Z_TYPE_P(instance) != IS_OBJECT) { \
		zend_throw_exception_ex(exception_ce, 0, "Unexpected value type: expected object implementing %s, %s given", interface_ce->name->val, zend_zval_type_name(instance)); \
		RETURN_MM(); \
	} else if (!zend_class_implements_interface(Z_OBJCE_P(instance), interface_ce)) { \
		zend_throw_exception_ex(exception_ce, 0, "Unexpected value type: expected object implementing %s, object of type %s given", interface_ce->name->val, Z_OBJCE_P(instance)->name->val); \
		RETURN_MM(); \
	}
#else
#define DAO_MM_VERIFY_INTERFACE_EX(instance, interface_ce, exception_ce) \
	if (Z_TYPE_P(instance) != IS_OBJECT) { \
		zend_throw_exception_ex(exception_ce, 0, "Unexpected value type: expected object implementing %s, %s given", interface_ce->name->val, zend_zval_type_name(instance)); \
		RETURN_MM(); \
	} else if (!instanceof_function_ex(Z_OBJCE_P(instance), interface_ce, 1)) { \
		zend_throw_exception_ex(exception_ce, 0, "Unexpected value type: expected object implementing %s, object of type %s given", interface_ce->name->val, Z_OBJCE_P(instance)->name->val); \
		RETURN_MM(); \
	}
#endif

#if PHP_VERSION_ID >= 80000
#define DAO_VERIFY_INTERFACE_CE_EX(instance_ce, interface_ce, exception_ce) \
	if (!zend_class_implements_interface(instance_ce, interface_ce)) { \
		zend_throw_exception_ex(exception_ce, 0, "Unexpected value type: expected object implementing %s, object of type %s given", interface_ce->name->val, instance_ce->name->val); \
		return; \
	}

#define DAO_MM_VERIFY_INTERFACE_CE_EX(instance_ce, interface_ce, exception_ce) \
	if (!zend_class_implements_interface(instance_ce, interface_ce)) { \
		zend_throw_exception_ex(exception_ce, 0, "Unexpected value type: expected object implementing %s, object of type %s given", interface_ce->name->val, instance_ce->name->val); \
		RETURN_MM(); \
	}
#else
#define DAO_VERIFY_INTERFACE_CE_EX(instance_ce, interface_ce, exception_ce) \
	if (!instanceof_function_ex(instance_ce, interface_ce, 1)) { \
		zend_throw_exception_ex(exception_ce, 0, "Unexpected value type: expected object implementing %s, object of type %s given", interface_ce->name->val, instance_ce->name->val); \
		return; \
	}

#define DAO_MM_VERIFY_INTERFACE_CE_EX(instance_ce, interface_ce, exception_ce) \
	if (!instanceof_function_ex(instance_ce, interface_ce, 1)) { \
		zend_throw_exception_ex(exception_ce, 0, "Unexpected value type: expected object implementing %s, object of type %s given", interface_ce->name->val, instance_ce->name->val); \
		RETURN_MM(); \
	}
#endif

#define DAO_VERIFY_INTERFACE_OR_NULL_EX(pzv, interface_ce, exception_ce) \
	if (Z_TYPE_P(pzv) != IS_NULL){ \
		DAO_VERIFY_INTERFACE_EX(pzv, interface_ce, exception_ce); \
	}

#define DAO_MM_VERIFY_INTERFACE_OR_NULL_EX(pzv, interface_ce, exception_ce) \
	if (Z_TYPE_P(pzv) != IS_NULL){ \
		DAO_MM_VERIFY_INTERFACE_EX(pzv, interface_ce, exception_ce); \
	}

#define DAO_VERIFY_CLASS_EX(instance, class_ce, exception_ce) \
	if (Z_TYPE_P(instance) != IS_OBJECT || !instanceof_function(Z_OBJCE_P(instance), class_ce)) { \
		if (Z_TYPE_P(instance) != IS_OBJECT) { \
			zend_throw_exception_ex(exception_ce, 0, "Unexpected value type: expected object of type %s, %s given", class_ce->name->val, zend_zval_type_name(instance)); \
		} else { \
			zend_throw_exception_ex(exception_ce, 0, "Unexpected value type: expected object of type %s, object of type %s given", class_ce->name->val, Z_OBJCE_P(instance)->name->val); \
		} \
		return; \
	}

#define DAO_MM_VERIFY_CLASS_EX(instance, class_ce, exception_ce) \
	if (Z_TYPE_P(instance) != IS_OBJECT || !instanceof_function(Z_OBJCE_P(instance), class_ce)) { \
		if (Z_TYPE_P(instance) != IS_OBJECT) { \
			zend_throw_exception_ex(exception_ce, 0, "Unexpected value type: expected object of type %s, %s given", class_ce->name->val, zend_zval_type_name(instance)); \
		} else { \
			zend_throw_exception_ex(exception_ce, 0, "Unexpected value type: expected object of type %s, object of type %s given", class_ce->name->val, Z_OBJCE_P(instance)->name->val); \
		} \
		RETURN_MM(); \
	}

#define DAO_VERIFY_CLASS_CE_EX(instance_ce, interface_ce, exception_ce) \
	if (!instanceof_function_ex(instance_ce, interface_ce)) { \
		zend_throw_exception_ex(exception_ce, 0, "Unexpected value type: expected object of type %s, %s given", interface_ce->name->val, instance_ce->name->val); \
		return; \
	}

#define DAO_MM_VERIFY_CLASS_CE_EX(instance_ce, interface_ce, exception_ce) \
	if (!instanceof_function_ex(instance_ce, interface_ce)) { \
		zend_throw_exception_ex(exception_ce, 0, "Unexpected value type: expected object of type %s, %s given", interface_ce->name->val, instance_ce->name->val); \
		RETURN_MM(); \
	}

#define DAO_VERIFY_CLASS_OR_NULL_EX(pzv, class_ce, exception_ce) \
	if (Z_TYPE_P(pzv) != IS_NULL) { \
		DAO_VERIFY_CLASS_EX(pzv, class_ce, exception_ce); \
	}

#define DAO_MM_VERIFY_CLASS_OR_NULL_EX(pzv, class_ce, exception_ce) \
	if (Z_TYPE_P(pzv) != IS_NULL) { \
		DAO_MM_VERIFY_CLASS_EX(pzv, class_ce, exception_ce); \
	}

#define DAO_VERIFY_INTERFACE(instance, interface_ce) \
	DAO_VERIFY_INTERFACE_EX(instance, interface_ce, spl_ce_LogicException)
#define DAO_MM_VERIFY_INTERFACE(instance, interface_ce) \
	DAO_MM_VERIFY_INTERFACE_EX(instance, interface_ce, spl_ce_LogicException)

#define DAO_VERIFY_INTERFACE_OR_NULL(pzv, interface_ce) \
	DAO_VERIFY_INTERFACE_OR_NULL_EX(pzv, interface_ce, spl_ce_LogicException)
#define DAO_MM_VERIFY_INTERFACE_OR_NULL(pzv, interface_ce) \
	DAO_MM_VERIFY_INTERFACE_OR_NULL_EX(pzv, interface_ce, spl_ce_LogicException)

#define DAO_VERIFY_CLASS(instance, class_ce)		DAO_VERIFY_CLASS_EX(instance, class_ce, spl_ce_LogicException)
#define DAO_MM_VERIFY_CLASS(instance, class_ce)		DAO_MM_VERIFY_CLASS_EX(instance, class_ce, spl_ce_LogicException)
#define DAO_VERIFY_CLASS(instance, class_ce)		DAO_VERIFY_CLASS_EX(instance, class_ce, spl_ce_LogicException)
#define DAO_MM_VERIFY_CLASS(instance, class_ce)		DAO_MM_VERIFY_CLASS_EX(instance, class_ce, spl_ce_LogicException)
#define DAO_VERIFY_CLASS_OR_NULL(pzv, class_ce)		DAO_VERIFY_CLASS_OR_NULL_EX(pzv, class_ce, spl_ce_LogicException)
#define DAO_MM_VERIFY_CLASS_OR_NULL(pzv, class_ce)	DAO_MM_VERIFY_CLASS_OR_NULL_EX(pzv, class_ce, spl_ce_LogicException)

#define DAO_ENSURE_IS_STRING(pzv)    convert_to_string_ex(pzv)
#define DAO_ENSURE_IS_LONG(pzv)      convert_to_long_ex(pzv)
#define DAO_ENSURE_IS_DOUBLE(pzv)    convert_to_double_ex(pzv)
#define DAO_ENSURE_IS_BOOL(pzv)      convert_to_boolean_ex(pzv)
#define DAO_ENSURE_IS_ARRAY(pzv)     convert_to_array_ex(pzv)
#define DAO_ENSURE_IS_OBJECT(pzv)    convert_to_object_ex(pzv)
#define DAO_ENSURE_IS_NULL(pzv)      convert_to_null_ex(pzv)

void dao_clean_and_cache_symbol_table(zend_array *symbol_table);

#define DAO_CHECK_POINTER(v) if (!v) fprintf(stderr, "%s:%d\n", __PRETTY_FUNCTION__, __LINE__)
#define DAO_DEBUG_POINTER() fprintf(stderr, "%s:%d\n", __PRETTY_FUNCTION__, __LINE__)

#define DAO_GET_OBJECT_FROM_OBJ(object, object_struct) \
	((object_struct *) ((char *) (object) - XtOffsetOf(object_struct, obj)))

#define DAO_GET_OBJECT_FROM_ZVAL(zv, object_struct) \
	DAO_GET_OBJECT_FROM_OBJ(Z_OBJ_P(zv), object_struct)

#define dao_is_php_version(id) (PHP_VERSION_ID / 10 == id / 10 ?  1 : 0)

#endif /* DAO_KERNEL_MAIN_H */
