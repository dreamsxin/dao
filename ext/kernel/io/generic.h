
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

#ifndef DAO_KERNEL_IO_GENERIC_H
#define DAO_KERNEL_IO_GENERIC_H

#include "kernel/io/definitions.h"
#include "kernel/io/networks.h"
#include "kernel/io/client.h"

#if !HAVE_EPOLL && !HAVE_KQUEUE

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
} dao_io_poller_info;

dao_io_poller_info *dao_io_create_poller(void* parent, char* address, int port, char *networks, void (*callback)(dao_io_client_info *ci,int op));
int dao_io_start_poller(dao_io_poller_info *pi);
void *dao_io_stop_poller(dao_io_poller_info *pi);
int dao_io_poller_is_alive(dao_io_poller_info *pi);
dao_io_callback_t dao_io_event_loop(void *tpi);

#endif

#endif /* DAO_KERNEL_IO_GENERIC_H */
