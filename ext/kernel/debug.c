
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

#include "kernel/debug.h"
#include "kernel/string.h"

FILE *dao_log = NULL;
int dao_debug_trace = 0;
dao_debug_entry *start = NULL;
dao_debug_entry *active = NULL;

/**
 * Stars debug on file pipe
 */
int dao_start_debug(){
	if(!dao_log){
		dao_log = stderr;
	}
	return SUCCESS;
}

/**
 * Stops debug process
 */
int dao_stop_debug(){
	dao_debug_entry *ptr = active;
	dao_debug_entry *this_entry = NULL;
	while(ptr){
		this_entry = ptr;
		ptr = ptr->prev;
		efree(this_entry);
	}
	//fclose(dao_log);
	dao_log = NULL;
	return SUCCESS;
}

/**
 * Executes a print_r on an interal zval
 */
int dao_print_r(zval *userval){
	zend_print_zval_r(userval, 0);
	return SUCCESS;
}

/**
 * Executes a print_r on an interal zval
 */
int dao_debug_print_r(zval *message){
	zend_printf("[DEBUG] ");
	zend_print_zval_r(message, 0);
	zend_printf("<br>\r\n");
	return SUCCESS;
}

/**
 * Internal fast zval dump
 */
int dao_vdump(zval *uservar){
	dao_start_debug();
    if(!uservar){
		fprintf(dao_log, "Null pointer\n");
		return SUCCESS;
	}
    switch(Z_TYPE_P(uservar)){
        case IS_NULL:
            fprintf(dao_log, "NULL \n");
            break;
        case IS_TRUE:
            fprintf(dao_log, "Boolean: %s\n", "TRUE");
        case IS_FALSE:
            fprintf(dao_log, "Boolean: %s\n", "FALSE");
            break;
        case IS_LONG:
            fprintf(dao_log, "Long: %ld at %p, refcount=%d\n", (long)Z_LVAL_P(uservar), uservar, Z_REFCOUNT_P(uservar));
            break;
        case IS_DOUBLE:
            fprintf(dao_log, "Double: %f\n", Z_DVAL_P(uservar));
            break;
        case IS_STRING:
			fprintf(dao_log, "String: %s(%zu) at %p, refcount=%d\n", Z_STRVAL_P(uservar), Z_STRLEN_P(uservar), uservar, Z_REFCOUNT_P(uservar));
            break;
        case IS_RESOURCE:
            fprintf(dao_log, "Resource\n");
            break;
        case IS_ARRAY:
            fprintf(dao_log, "Array at %p, refcount=%d\n", uservar, Z_REFCOUNT_P(uservar));
            break;
        case IS_OBJECT:
            fprintf(dao_log, "Object <%s> at %p\n", Z_OBJCE_P(uservar)->name->val, uservar);
            break;
        default:
            fprintf(dao_log, "Unknown\n");
    }
    return SUCCESS;
}

int dao_dump_ce(zend_class_entry *ce){
	char *message = emalloc(sizeof(char *) * 120);
	if(ce){
		snprintf(message, 120, "- ClassType => %d", ce->type);
		dao_step_over(message);
		snprintf(message, 120, "- ClassName => %s", ce->name->val);
		dao_step_over(message);
	} else {
		dao_step_over("- NULL class entry :(");
	}
	return SUCCESS;
}

int dao_class_debug(zval *val){
	char *message = emalloc(sizeof(char *) * 120);
	zend_class_entry *ce;
	if (val) {
		ce = Z_OBJCE_P(val);
		if(ce){
			snprintf(message, 120, "- MemoryAddress => %p", val);
			dao_step_over(message);
			dao_dump_ce(ce);
		} else {
			dao_step_over("- No class entry :(");
		}
	} else {
		dao_step_over("- this_ptr is null :(");
	}
	return SUCCESS;
}

/**
 * Append debug information to file
 */
int dao_debug_str(char *what, char *message){
	fprintf(dao_log, "%s", what);
	fprintf(dao_log, "%s", message);
	fprintf(dao_log, "\n");
	return SUCCESS;
}

int dao_debug_long(char *what, uint vlong){
	fprintf(dao_log, "%s", what);
	fprintf(dao_log, "%u", vlong);
	fprintf(dao_log, "\n");
	return SUCCESS;
}

int dao_debug_screen(char *message){
	dao_debug_space();
	fprintf(dao_log, "%s\n", message);
	return SUCCESS;
}

int dao_debug_method_call(zval *obj, char *method_name){
	if(Z_TYPE_P(obj)==IS_OBJECT){
		dao_debug_space();
	} else {
		dao_error_space();
	}
	if(Z_TYPE_P(obj)==IS_OBJECT){
		fprintf(dao_log, "Calling method %s::%s on Object at %p\n", Z_OBJCE_P(obj)->name->val, method_name, obj);
	} else {
		fprintf(dao_log, "Calling method %s on non object :(\n", method_name);
	}
	return SUCCESS;
}

int dao_error_space(){
	int i;
	fprintf(dao_log, "[ERROR] ");
	for(i=0;i<dao_debug_trace;i++){
		fprintf(dao_log, " ");
	}
	return SUCCESS;
}

int dao_debug_space(){
	int i;
	fprintf(dao_log, "[DEBUG] ");
	for(i=0;i<dao_debug_trace;i++){
		fprintf(dao_log, " ");
	}
	return SUCCESS;
}

int dao_debug_param(zval *param){
	dao_debug_space();
	fprintf(dao_log, "Push method Param > ");
	dao_vdump(param);
	return SUCCESS;
}

int dao_debug_vdump(char *preffix, zval *value){
	dao_debug_space();
	fprintf(dao_log, "%s", preffix);
	dao_vdump(value);
	return SUCCESS;
}

int dao_debug_assign(char *name, zval *value){
	dao_debug_space();
	fprintf(dao_log, "Assign on %s with ", name);
	dao_vdump(value);
	return SUCCESS;
}

int dao_step_over(char *message){
	dao_debug_screen(message);
	return SUCCESS;
}

int dao_step_into(char *message){
	dao_debug_trace++;
	dao_debug_screen(message);
	return SUCCESS;
}

int dao_step_out(char *message){
	dao_debug_screen(message);
	dao_debug_trace--;
	return SUCCESS;
}

/**
 * Prints internal debug backtrace
 */
int dao_debug_backtrace_internal(){
	int step = 0;
	char *message;
	dao_debug_entry *ptr = active;
	while(ptr){
		dao_spprintf(&message, 0, "#%d %s::%s", step, ptr->class_name, ptr->method_name);
		dao_debug_screen(message);
		efree(message);
		ptr = ptr->prev;
		step++;
	}
	return SUCCESS;
}

/**
 * Appends a debug entry to internal execution scope
 */
int dao_step_into_entry(char *class_name, char *method_name, int lineno){

	char *message;
	dao_debug_entry *entry;

	if (!start) {
		start = (dao_debug_entry *) emalloc(sizeof(dao_debug_entry));
		start->class_name = "__main__";
		start->method_name = "__init__";
		start->lineno = 0;
		start->prev = NULL;
		start->next = NULL;
		active = start;
	}

	dao_spprintf(&message, 0, "Step Into %s::%s", class_name, method_name);
	dao_debug_screen(message);
	efree(message);

	entry = emalloc(sizeof(dao_debug_entry));
	entry->class_name = class_name;
	entry->method_name = method_name;
	entry->lineno = lineno;
	entry->prev = active;
	active->next = entry;
	active = entry;
	dao_debug_trace++;

	return SUCCESS;
}

/**
 * Steps out current stack
 */
int dao_step_out_entry(){

	char *message;
	dao_debug_entry *prev;
	if(active){

		dao_debug_trace--;

		dao_spprintf(&message, 0, "Step out %s::%s", active->class_name, active->method_name);
		dao_debug_screen(message);
		efree(message);

		prev = active->prev;
		efree(active);
		active = prev;

	} else {
		fprintf(dao_log, "Problem, stack?");
		return FAILURE;
	}
	return SUCCESS;
}

