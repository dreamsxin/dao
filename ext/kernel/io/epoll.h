
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

#ifndef DAO_KERNEL_IO_EPOLL_H
#define DAO_KERNEL_IO_EPOLL_H

#include "kernel/io/definitions.h"
#include "kernel/io/networks.h"
#include "kernel/io/client.h"

#if HAVE_EPOLL

#include <sys/epoll.h>

typedef struct epoll_event dao_io_poll_event;

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
	int poll_handler;
	dao_io_thread_t poll_thread;
	dao_io_client_info *accept_ci;
} dao_io_poller_info;

#define DAO_IO_OP(pev)				(0)
#define DAO_IO_EV(pev)				(pev->events)
#define DAO_IO_DATA(pev)			(pev->data.ptr)
#define DAO_IO_READ_EV(pev)			(pev->events & EPOLLIN)
#define DAO_IO_WRITE_EV(pev)		(pev->events & EPOLLOUT)
#define DAO_IO_IGNORE_EV(pev)		(0)
#define DAO_IO_TRUE_EOF_EV(pev)		(1)
#define DAO_IO_FALSE_EOF_EV(pev)	(0)

// common to epoll, kqueue and iocp
dao_io_poller_info* dao_io_create_poller(void* parent, char* address, int port, char *networks, void (*callback)(dao_io_client_info *ci,int op));
int dao_io_start_poller(dao_io_poller_info *pi);
void *dao_io_stop_poller(dao_io_poller_info *pi);
int dao_io_poller_is_alive(dao_io_poller_info *pi);
dao_io_callback_t dao_io_event_loop (void *tpi);

int  dao_io_create_poll();
int  dao_io_wait_event(dao_io_poller_info *pi, dao_io_poll_event *events_queue, int events_queue_size);
void dao_io_add_server_to_poll(dao_io_client_info *ci);
void dao_io_add_client_to_poll(dao_io_client_info *ci);
void dao_io_remove_client_from_poll(dao_io_client_info *ci);
void dao_io_change_poll(dao_io_client_info *ci, int operation, int events);
const char *dao_io_name_event(int event);
const char *dao_io_name_operation(int operation);

#endif /* HAVE_EPOLL */

#endif /* DAO_KERNEL_IO_EPOLL_H */
