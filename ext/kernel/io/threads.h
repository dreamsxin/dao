
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

#ifndef DAO_KERNEL_IO_THREADS_H
#define DAO_KERNEL_IO_THREADS_H


#include "kernel/io/definitions.h"
#include "kernel/io/networks.h"
#include "kernel/io/client.h"

typedef struct {
	void* parent;
	int info_type;
	int allow_tasks;
	int clients_are_non_blocking;
	dao_io_client_config cc;
	dao_io_socket_t server_socket;
	int actual_connections;
	dao_io_network_info *networks_info;
	int networks_size;
	void (*callback)(dao_io_client_info *ci, int can_defer);
	int stop;
	dao_io_client_info *first_client;
	dao_io_client_info *last_client;
	dao_io_mutex_t lock;
	// specific
	int worker_threads;
	dao_io_thread_t *threads;
	unsigned volatile running_threads;
} dao_io_threads_info;

dao_io_threads_info* dao_io_create_threads(void* parent, char* address, int port, char *networks, void (*callback)(dao_io_client_info *ci,int op), int worker_threads);
int dao_io_start_threads(dao_io_threads_info *ti);
void *dao_io_stop_threads(dao_io_threads_info *ti);
int dao_io_threads_are_running(dao_io_threads_info *ti);
dao_io_callback_t dao_io_worker_thread(void *data);
int dao_io_create_thread(dao_io_thread_t *pidthread, dao_io_callback_t (*worker_thread)(void *data), void *info);
void dao_io_wait_thread(dao_io_thread_t idthread);

#endif /* DAO_KERNEL_IO_THREADS_H */
