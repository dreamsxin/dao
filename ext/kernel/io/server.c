
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

#include <signal.h>

#include "kernel/io/tasks.h"
#include "kernel/io/sockets.h"
#include "kernel/io/threads.h"
#include "kernel/io/kqueue.h"
#include "kernel/io/epoll.h"
#include "kernel/io/tasks.h"
#include "kernel/io/server.h"
#include "kernel/io/generic.h"

int dao_io_init_servers()
{
	return dao_io_init_sockets();
}

void *dao_io_create_server(void* parent, char* address, int port, char* networks, void (*callback)(dao_io_client_info* ci,int op), int worker_threads)
{
	if (callback==NULL)
		return NULL;

	signal(SIGPIPE, SIG_IGN);

	if (worker_threads > 0)
		return dao_io_create_threads(parent, address, port, networks, callback, worker_threads);
	else
		return dao_io_create_poller(parent, address, port, networks, callback);
}

int dao_io_start_server(void* tpi)
{
	if (((dao_io_threads_info *)tpi)->info_type == DAO_IO_TP_THREADS)
		return dao_io_start_threads(tpi);
	else
		return dao_io_start_poller(tpi);
}

void *dao_io_stop_server(void* tpi)
{
	if (tpi == NULL)
		return NULL;
	if (((dao_io_threads_info *)tpi)->info_type == DAO_IO_TP_THREADS)
		return dao_io_stop_threads((dao_io_threads_info *)tpi);
	else
		return dao_io_stop_poller((dao_io_poller_info *)tpi);
}

int dao_io_server_is_alive(void* tpi)
{
	return dao_io_poller_is_alive((dao_io_poller_info *)tpi) | dao_io_threads_are_running((dao_io_threads_info *)tpi);
}

void dao_io_set_defaults(void* tpi, int use_write_events, int read_buffer_size, int write_buffer_size, int read_buffer_limit, int write_buffer_limit)
{
	dao_io_threads_info *ti = (dao_io_threads_info *)tpi;
	if (use_write_events != DAO_IO_DEFAULT)
		ti->cc.use_write_events = use_write_events;
	if (read_buffer_size != DAO_IO_DEFAULT)
		ti->cc.read_buffer_size = read_buffer_size;
	if (write_buffer_size != DAO_IO_DEFAULT)
		ti->cc.write_buffer_size = write_buffer_size;
	if (read_buffer_limit != DAO_IO_DEFAULT)
		ti->cc.read_buffer_limit = read_buffer_limit;
	if (write_buffer_limit != DAO_IO_DEFAULT)
		ti->cc.write_buffer_limit = write_buffer_limit;
}

int dao_io_enqueue_message(dao_io_client_info* ci, int operation)
{
	ci->operation = operation;
	return dao_io_enqueue_task(ci);
}

int dao_io_write_message(dao_io_client_info* ci, char* message)
{
	dao_io_append_buffer (ci->wb, message);
	return dao_io_write_internal_buffer(ci);
}

int dao_io_write_internal_buffer(dao_io_client_info* ci)
{
	return dao_io_query_write((dao_io_client_info *)ci);
}

int dao_io_write_external_buffer(dao_io_client_info* ci, char* buffer, int size)
{
	ci->eb->buffer = buffer;
	ci->eb->head = 0;
	ci->eb->mark = 0;
	ci->eb->tail = size;
	ci->use_eb = DAO_IO_TRUE;
	return dao_io_query_write ((dao_io_client_info *)ci);
}
