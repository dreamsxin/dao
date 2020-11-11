
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

#include "kernel/io/support.h"
#include "kernel/io/tasks.h"
#include "kernel/io/sockets.h"
#include "kernel/io/threads.h"
#include "kernel/io/kqueue.h"

#if HAVE_KQUEUE

#include <errno.h>
#include <stdlib.h>
#include <string.h>

dao_io_poller_info *dao_io_create_poller (void* parent, char* address, int port, char *networks, void (*callback)(dao_io_client_info *ci,int op))
{
	dao_io_socket_t server_socket;
	if ((server_socket = dao_io_create_server_socket (address, port)) < 0)
		return NULL;

	dao_io_poller_info *pi = calloc (1, sizeof(dao_io_poller_info));
	pi->parent = parent;
	pi->info_type = DAO_IO_TP_POLLER;
	pi->allow_tasks = DAO_IO_TRUE;
	pi->clients_are_non_blocking = DAO_IO_TRUE;
	dao_io_preset_client_config (pi);
	pi->server_socket = server_socket;
	pi->actual_connections = 0;
	dao_io_assign_networks ((void *)pi, networks);
	pi->callback = callback;
	pi->stop = DAO_IO_FALSE;
	dao_io_init_clients (pi);
	dao_io_init_tasks();

	return pi;
}

int dao_io_start_poller(dao_io_poller_info *pi)
{
	pi->accept_ci = dao_io_create_client (pi, pi->server_socket);

	if ((pi->poll_handler = dao_io_create_poll()) < 0) {
		pi->server_socket = dao_io_close_socket (pi->server_socket);
		dao_io_error_message ("Error creating poll queue: %s\n", strerror (errno));
		return DAO_IO_FALSE;
	}

	return dao_io_create_thread (&pi->poll_thread, dao_io_event_loop, pi);
}

void *dao_io_stop_poller(dao_io_poller_info *pi)
{
	pi->stop = DAO_IO_TRUE;

	pi->server_socket = dao_io_close_socket (pi->server_socket);
	pi->accept_ci = dao_io_delete_client (pi->accept_ci, DAO_IO_FALSE);

	dao_io_client_info *ci = pi->first_client;
	while (ci != NULL) {
		dao_io_client_info *next = ci->next;
		dao_io_close_socket (ci->socket);
		dao_io_delete_client (ci, DAO_IO_TRUE);
		ci = next;
	}

	pi->poll_handler = dao_io_close_socket (pi->poll_handler);

	if (pi->poll_thread != 0) {
		dao_io_wait_thread (pi->poll_thread);
		pi->poll_thread = 0;
	}

	dao_io_clean_tasks();
	dao_io_clean_clients (pi);
	dao_io_release_networks (pi);

	free (pi);
	return NULL;
}

int dao_io_poller_is_alive(dao_io_poller_info *pi)
{
	if (pi->info_type == DAO_IO_TP_POLLER)
		return pi->poll_thread != 0;
	else
		return 0;
}

dao_io_callback_t dao_io_event_loop (void *tpi)
{
	dao_io_poller_info *pi = (dao_io_poller_info *)tpi;
	dao_io_poll_event *events_queue = NULL;
	int events_queue_size = 0;

	dao_io_add_server_to_poll (pi->accept_ci);

	while (!pi->stop) {
		int old_events_queue_size = events_queue_size;
		if (events_queue_size==0 || events_queue_size<pi->actual_connections)
			events_queue_size += DAO_IO_EVENTS_QUEUE_SIZE;								// grow
		if (events_queue_size>pi->actual_connections*2 && events_queue_size>DAO_IO_EVENTS_QUEUE_SIZE)
			events_queue_size += DAO_IO_EVENTS_QUEUE_SIZE;								// shrink
		if (old_events_queue_size != events_queue_size)  {
			if (events_queue != NULL)
				free(events_queue);
			events_queue = (dao_io_poll_event *) calloc (events_queue_size, sizeof(dao_io_poll_event));
		}

		int nevents = dao_io_wait_event (pi, events_queue, events_queue_size);
		dao_io_poll_event *pev;
		dao_io_debug_message(DAO_IO_DEBUG_IO, "nevents=%d\n", nevents);

		if (nevents < 0) {
			if (!pi->stop)
				dao_io_error_message ("Error in waiting events: %s\n", strerror (errno));
			break;
		}
		if (nevents == 0) {
			dao_io_error_message ("No events received!\n");
			break;
		}

		int ievent;
		for (ievent = 0; ievent < nevents; ievent++) {
			pev = &events_queue[ievent];
			dao_io_client_info *ci = (dao_io_client_info *) DAO_IO_DATA(pev);
			if (ci==NULL)
				break;
			dao_io_debug_message(DAO_IO_DEBUG_IO, "event socket=%d, ci=%6x, ev=%s, op=%s, flags=%s%s\n",
					ci->socket, ci, dao_io_name_event(DAO_IO_EV(pev)), dao_io_name_operation(DAO_IO_OP(pev)),
					(pev->flags&EV_DELETE)?"DELETE ":"", (pev->flags&EV_ERROR)?"ERROR ":"");
			if (DAO_IO_IGNORE_EV(pev))
				continue;
			if (ci->socket==-1)		// event after closing socket
				continue;
			if (DAO_IO_FALSE_EOF_EV(pev))
				ci->read_end = DAO_IO_TRUE;
			else if (ci == pi->accept_ci)
				dao_io_do_accept (pi, ci->socket);
			else {
				if (DAO_IO_READ_EV(pev)) {
					if (dao_io_do_read (ci) > 0)
						dao_io_do_callback(ci, DAO_IO_CLIENT_READ);
				}
				if (DAO_IO_WRITE_EV(pev)) {		// if use_write_events. in linux, no write event until queue full
					if (ci->write_pending)
						dao_io_do_write (ci);
					else
						ci->can_write = DAO_IO_TRUE;
				}
				while (ci->callback_has_written && ci->can_write)
					if (ci->can_write && ci->step >= 0)
						dao_io_do_callback(ci, DAO_IO_CLIENT_WRITE);
			}
			if (ci->error || ((ci->read_end && DAO_IO_CI_GET_DATA_SIZE(ci->rb)==0) /*&& DAO_IO_CI_GET_DATA_SIZE(ci->wb)==0*/)) {
				dao_io_debug_message(DAO_IO_DEBUG_IO, "close socket=%d, ci=%6x, ev=%s, op=%s\n",
						ci->socket, ci, dao_io_name_event(DAO_IO_EV(pev)), dao_io_name_operation(DAO_IO_OP(pev)));
				dao_io_remove_client_from_poll (ci);
				if (DAO_IO_TRUE_EOF_EV(pev)) {
					dao_io_debug_message(DAO_IO_DEBUG_IO, "dao_io_close_socket xxx(ci=%6x,%d)\n", ci, ci->socket);
					ci->socket = dao_io_close_socket (ci->socket);
					ci = dao_io_delete_client(ci, DAO_IO_TRUE);
				}
			}
		}
	}

	if (events_queue != NULL)
		free(events_queue);
	pi->poll_thread = 0;			// signal end of thread
	return (dao_io_callback_t) 0;
}

int dao_io_create_poll () {
	return kqueue();
}

int dao_io_wait_event (dao_io_poller_info *pi, dao_io_poll_event *events_queue, int events_queue_size) {
	return kevent(pi->poll_handler, NULL, 0, events_queue, events_queue_size, NULL);
}

void dao_io_add_server_to_poll (dao_io_client_info *ci) {
	dao_io_change_poll (ci, EV_ADD | EV_ENABLE, EVFILT_READ);
}

void dao_io_add_client_to_poll (dao_io_client_info *ci) {
	((dao_io_poller_info *)ci->tpi)->actual_connections++;
	dao_io_change_poll (ci, EV_ADD|EV_ENABLE|EV_CLEAR, EVFILT_READ);
	if (ci->use_write_events)
		dao_io_change_poll(ci, EV_ADD|EV_ENABLE|EV_CLEAR, EVFILT_WRITE);
}

void dao_io_remove_client_from_poll (dao_io_client_info *ci) {
	((dao_io_poller_info *)ci->tpi)->actual_connections--;
	dao_io_change_poll (ci, EV_DELETE, EVFILT_READ);
	if (ci->use_write_events)
		dao_io_change_poll (ci, EV_DELETE, EVFILT_WRITE);
}

void dao_io_change_poll (dao_io_client_info *ci, int operation, int events)
{
	if (ci->socket<0) {
		dao_io_error_message ("Socket (-1) is invalid in dao_io_change_poll (ci=%6x)\n", ci);
		return;
	}
	dao_io_poll_event pe;
	pe.ident  = ci->socket;
	pe.filter = events;
	pe.flags  = operation;
	pe.fflags = 0;
	pe.data   = 0;
	pe.udata  = ci;
  	dao_io_debug_message(DAO_IO_DEBUG_IO, "dao_io_change_poll socket=%d, ci=%6x, ev=%s, op=%s\n",
  		ci->socket, ci, dao_io_name_event(DAO_IO_EV((&pe))), dao_io_name_operation(DAO_IO_OP((&pe))));
	int nevents = kevent (((dao_io_poller_info *)ci->tpi)->poll_handler, &pe, 1, NULL, 0, NULL);
	if (nevents < 0)
		dao_io_error_message ("Error in dao_io_change_poll: %s\n", strerror (errno));
}

const char *dao_io_name_event (int event)
{
	switch (event) {
	case EVFILT_READ:				// -1
		return "EVFILT_READ ";
	case EVFILT_WRITE:				// -2
		return "EVFILT_WRITE";
	default:
		return "EVFILT_???  ";
	}
}

const char *dao_io_name_operation (int operation)
{
	static char string[51];
	char *sep="", *ps=string;
	int bit=0x0001;
	do {
		switch (operation&bit) {
		case EV_ADD:		// 0001
			strcpy(ps,sep); ps+=strlen(sep); strcpy(ps,"EV_ADD");     ps+=6; break;
		case EV_DELETE:		// 0002
			strcpy(ps,sep); ps+=strlen(sep); strcpy(ps,"EV_DELETE");  ps+=9; break;
		case EV_ENABLE:		// 0004
			strcpy(ps,sep); ps+=strlen(sep); strcpy(ps,"EV_ENABLE");  ps+=9; break;
		case EV_DISABLE:	// 0008
			strcpy(ps,sep); ps+=strlen(sep); strcpy(ps,"EV_DISABLE"); ps+=10; break;
		case EV_CLEAR:		// 0020
			strcpy(ps,sep); ps+=strlen(sep); strcpy(ps,"EV_CLEAR");   ps+=8; break;
		case EV_EOF:		// 8000
			strcpy(ps,sep); ps+=strlen(sep); strcpy(ps,"EV_EOF");     ps+=6; break;
		case EV_ERROR:		// 4000
			strcpy(ps,sep); ps+=strlen(sep); strcpy(ps,"EV_ERROR");   ps+=8; break;
		default:
			break;
		}
		if (ps!=string)
			sep = "|";
		bit <<= 1;
		if (ps-string >= 40)
			break;
	} while (bit < 0x10000);
	*ps = '\0';
	return string;
}

#endif /* HAVE_KQUEUE */
