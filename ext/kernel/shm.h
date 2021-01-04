
/*
  +------------------------------------------------------------------------+
  | Dao Framework                                                          |
  +------------------------------------------------------------------------+
  | Copyright (c) 2011-2015 Phalcon Team (http://www.phalconphp.com)       |
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

#ifndef DAO_KERNEL_SHM_H
#define DAO_KERNEL_SHM_H

#include <stddef.h>
#include <stdint.h>
#include <semaphore.h>

typedef struct _dao_shared_memory {
  int     owner;
  char    name[255];
  size_t  lenght;
  sem_t*  sem;
  int     fd;
  void*   mem;
} dao_shared_memory;

dao_shared_memory* dao_shared_memory_create(char const* name, size_t);
dao_shared_memory* dao_shared_memory_open(char const* name);

void dao_shared_memory_unlink(char const* name);
void dao_shared_memory_cleanup(dao_shared_memory* src);

char const* dao_shared_memory_name(dao_shared_memory const* src);

int dao_shared_memory_trylock(dao_shared_memory* src);
int dao_shared_memory_lock(dao_shared_memory* src);
int dao_shared_memory_unlock(dao_shared_memory* src);
int dao_shared_memory_unlock_force(dao_shared_memory* src);

void* dao_shared_memory_ptr(dao_shared_memory const* src);
size_t dao_shared_memory_size(dao_shared_memory const* src);

#endif /* DAO_KERNEL_SHM_H */
