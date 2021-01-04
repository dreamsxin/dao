
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
  |          Didier Bertrand <diblibre@gmail.com>                          |
  |          ZhuZongXin <dreamsxin@qq.com>                                 |
  +------------------------------------------------------------------------+
*/

#ifndef DAO_KERNEL_IO_TASKS_H
#define DAO_KERNEL_IO_TASKS_H

#include "kernel/io/sockets.h"
#include "kernel/io/client.h"

#define DAO_IO_BLOCK_SIZE 128

typedef struct {
	dao_io_client_info *ci;
} dao_io_task_info;

typedef struct {
	pthread_mutex_t lock;
	pthread_cond_t	cond;
	int must_Exit;
	dao_io_thread_t *threads;
	int max_threads;
	int first_free_thread;
	dao_io_task_info *tasks;
	int max_tasks;
	int first_free_task;
	int first_task;
} dao_io_tasks_data;

void dao_io_init_tasks ();
void dao_io_clean_tasks ();
void dao_io_push_thread (dao_io_thread_t thread);
void dao_io_delete_thread (dao_io_thread_t thread);
int  dao_io_get_running_task_threads ();
int  dao_io_start_one_task_thread ();
void dao_io_stop_one_task_thread ();
int  dao_io_push_task (dao_io_client_info *ci);
dao_io_client_info *dao_io_pop_task ();
int	 dao_io_enqueue_task (dao_io_client_info *ci);
dao_io_callback_t dao_io_tasks_thread (void *data);

#endif /* DAO_KERNEL_IO_TASKS_H */
