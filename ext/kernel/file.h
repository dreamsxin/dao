
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

#ifndef DAO_KERNEL_FILE_H
#define DAO_KERNEL_FILE_H

#include "php_dao.h"

int dao_file_exists_str(char *filename);
int dao_file_exists(zval *filename);
int dao_is_file_str(char *filename);
int dao_is_file(zval *filename);
int dao_compare_mtime(zval *filename1, zval *filename2);
void dao_fix_path(zval *return_value, zval *path, zval *directory_separator);
void dao_prepare_virtual_path(zval *return_value, zval *path, zval *virtual_separator);
void dao_unique_path_key(zval *return_value, zval *path);
void dao_file_realpath(zval *return_value, zval *filename);
void dao_file_get_contents(zval *return_value, zval *filename);
void dao_file_put_contents(zval *return_value, zval *filename, zval *data);
void dao_possible_autoload_filepath(zval *return_value, zval *prefix, zval *class_name, zval *virtual_separator, zval *separator);
void dao_possible_autoload_filepath2(zval *return_value, zval *class_name, zval *virtual_separator, zval *separator);

int dao_is_dir(zval *path);
int dao_is_dir_str(char *path);
void dao_unlink(zval *return_value, zval *path);
void dao_filemtime(zval *return_value, zval *path);
void dao_basename(zval *return_value, zval *path);

void dao_prepare_virtual_path_ex(zval *return_value, const char *path, size_t path_len, char virtual_separator);

void dao_fwrite(zval *return_value, zval *stream_zval, zval *data);
int dao_feof(zval *stream_zval);
int dao_fclose(zval *stream_zval);

int dao_is_absolutepath(zval *path);

void dao_path_concat(zval* return_value, char* path1, int len1, char * path2, int len2);

#ifdef TSRM_WIN32
#define DAO_DIRECTORY_SEPARATOR "\\"
#else
#define DAO_DIRECTORY_SEPARATOR "/"
#endif

#endif /* DAO_KERNEL_FILE_H */
