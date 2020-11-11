
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

#include <stdlib.h>

#ifdef __linux__
# include <signal.h>
#endif

#include "kernel/io/support.h"
#include "kernel/io/sockets.h"
#include "kernel/io/threads.h"


dao_io_threads_info *dao_io_create_threads  (void* parent, char* address, int port, char *networks, void (*callback)(dao_io_client_info *ci,int op), int worker_threads)
{
	dao_io_socket_t server_socket;

	if ((server_socket = dao_io_create_server_socket (address, port)) < 0)
		return NULL;
	dao_io_threads_info *ti = calloc (1, sizeof(dao_io_threads_info));
	ti->parent = parent;
	ti->info_type = DAO_IO_TP_THREADS;
	ti->allow_tasks = DAO_IO_FALSE;
	ti->clients_are_non_blocking = DAO_IO_FALSE;
	dao_io_preset_client_config (ti);
	ti->server_socket = server_socket;
	ti->actual_connections = 0;
	dao_io_assign_networks ((void *)ti, networks);
	ti->callback = callback;
	ti->stop = DAO_IO_FALSE;
	ti->first_client = NULL;
	ti->last_client = NULL;
	dao_io_init_clients (ti);

	ti->worker_threads = worker_threads;
	ti->threads = (dao_io_thread_t*) calloc (worker_threads, sizeof(dao_io_thread_t));
	ti->running_threads = 0;

	return ti;
}

int dao_io_start_threads (dao_io_threads_info *ti)
{
	int it;
	for (it = 0; it < ti->worker_threads; ++it)
		if (! dao_io_create_thread (&ti->threads[it], dao_io_worker_thread, ti))
			return DAO_IO_FALSE;
	return DAO_IO_TRUE;
}

// not used for now
void *dao_io_stop_threads (dao_io_threads_info *ti)
{
	int it;

	ti->stop = DAO_IO_TRUE;
	ti->server_socket = dao_io_close_socket (ti->server_socket);

	dao_io_client_info *ci = ti->first_client;
	while (ci != NULL) {
		dao_io_client_info *next = ci->next;
		dao_io_close_socket (ci->socket);
		ci = next;
	}

	for (it = 0; it < ti->worker_threads; it++)
		if (ti->threads[it] != 0) {
#ifdef __linux__
			pthread_kill(ti->threads[it], SIGHUP);
#endif
			pthread_join (ti->threads[it], NULL);
			ti->threads[it] = 0;
		}

	dao_io_clean_clients (ti);
	dao_io_release_networks (ti);

	free (ti);
	return NULL;
}

int dao_io_threads_are_running (dao_io_threads_info *ti)
{
	if (ti->info_type == DAO_IO_TP_THREADS)
		return ti->running_threads > 0;
	else
		return 0;
}

#ifdef __linux__
void threads_signal (int sig, siginfo_t *info, void *ucontext)
{
}
#endif

dao_io_callback_t dao_io_worker_thread (void *tpi)
{
	dao_io_threads_info *ti = (dao_io_threads_info *) tpi;
	int thread_id = atomic_add_uint(&ti->running_threads, 1);

#ifdef __linux__
	// to awake accept if linux
	struct sigaction sa;
	sa.sa_handler = NULL;
	sa.sa_sigaction = threads_signal;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);

	if (sigaction(SIGHUP, &sa, NULL) < 0) {
		dao_io_error_message("error setting sigaction");
		return NULL;
	}
#endif

	while (!ti->stop) {
		dao_io_client_info *ci = dao_io_do_accept (tpi, ti->server_socket);
		if (ci && !ti->stop) {
			++ti->actual_connections;
			ci->thread_id = thread_id;
			while (!ci->error && !ci->read_end) {
				if (dao_io_do_read (ci) > 0)
					dao_io_do_callback (ci, DAO_IO_CLIENT_READ);
			}
			--ti->actual_connections;
			ci->socket = dao_io_close_socket (ci->socket);
			ci = dao_io_delete_client (ci, DAO_IO_TRUE);
		}
	}

	atomic_add_uint(&ti->running_threads, -1);
	return (dao_io_callback_t) 0;
}

int dao_io_create_thread(dao_io_thread_t *pidthread, dao_io_callback_t (*worker_thread)(void *data), void *info)
{
	int rc;
	rc = pthread_create (pidthread, NULL, worker_thread, info);
	if (rc != 0)
		return DAO_IO_FALSE;
	return DAO_IO_TRUE;
}

void dao_io_wait_thread (dao_io_thread_t idthread)
{
	pthread_join (idthread, NULL);
}
