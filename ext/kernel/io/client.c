
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
#include <string.h>

#include "kernel/io/definitions.h"
#include "kernel/io/support.h"
#include "kernel/io/client.h"
#include "kernel/io/sockets.h"
#include "kernel/io/threads.h"

void dao_io_preset_client_config (void *tpi)
{
	dao_io_threads_info *ti = (dao_io_threads_info *) tpi;

	ti->cc.use_write_events  = DAO_IO_TRUE;		// set DAO_IO_TRUE to check polling events on writes. a little bit faster for kqueues

	ti->cc.read_buffer_size   = 150;			// minimum 32 bytes for windows
	ti->cc.write_buffer_size  = 500;			// minimum 32 bytes for windows
	ti->cc.read_buffer_limit  = DAO_IO_DEFAULT;	// no limit on read buffer size
	ti->cc.write_buffer_limit = DAO_IO_DEFAULT;	// no limit on write buffer size
}

dao_io_client_info *dao_io_create_client (void *tpi, dao_io_socket_t socket)
{
	dao_io_threads_info *ti = (dao_io_threads_info *) tpi;
	dao_io_client_info *ci;
	ci = (dao_io_client_info *) malloc (sizeof(dao_io_client_info));
	if (!ci) {
		dao_io_error_message("No memory\n");
		return NULL;
	}
	ci->tpi = tpi;
	ci->cc = &ti->cc;
	ci->use_write_events = ti->clients_are_non_blocking? ti->cc.use_write_events: DAO_IO_FALSE;
	ci->socket = socket;
	memset (&ci->remote_addr, '\0', sizeof(struct in_addr));
	ci->remote_port = 0;
	memset (&ci->local_addr, '\0', sizeof(struct in_addr));
	ci->thread_id   = 0;
	ci->local_port  = 0;
	ci->read_end    = DAO_IO_FALSE;
	ci->error       = DAO_IO_FALSE;

	// create buffers. write size larger than read size for web applications
	if ((ci->eb = dao_io_create_client_buffer(0,DAO_IO_DEFAULT)) == NULL) {
		free(ci);
		return NULL;
	}
	if ((ci->wb = dao_io_create_client_buffer(ci->cc->write_buffer_size,ci->cc->write_buffer_limit)) == NULL) {
		dao_io_delete_client_buffer (ci->eb);
		free(ci);
		return NULL;
	}
	if ((ci->rb = dao_io_create_client_buffer(ci->cc->read_buffer_size,ci->cc->read_buffer_limit)) == NULL) {
		dao_io_delete_client_buffer (ci->wb);
		dao_io_delete_client_buffer (ci->eb);
		free(ci);
		return NULL;
	}

	ci->use_eb = DAO_IO_FALSE;

	ci->operation = DAO_IO_OP_NONE;
	ci->overlapped = NULL;
	ci->callback_has_written = DAO_IO_FALSE;
	ci->can_write = DAO_IO_TRUE;
	ci->write_pending = DAO_IO_FALSE;

	// add client to the end of clients linked list. used to close connections while stopping server
	dao_io_debug_message(DAO_IO_DEBUG_CLIENT, "ci list +> %08x < %08x > %08x == %08x <> %08x\n",
		ci->previous, ci, ci->next, ti->first_client, ti->last_client);
	dao_io_mutex_lock (&ti->lock);
	ci->previous = NULL;
	ci->next = NULL;
	if (ti->first_client == NULL)
		ti->first_client = ci;
	if (ti->last_client != NULL) {
		ti->last_client->next = ci;
		ci->previous = ti->last_client;
	}
	ti->last_client = ci;
	dao_io_mutex_unlock (&ti->lock);

	ci->step = -1;
	ci->context = NULL;
	dao_io_debug_message(DAO_IO_DEBUG_CLIENT, "ci list +< %08x < %08x > %08x == %08x <> %08x\n",
			ci->previous, ci, ci->next, ti->first_client, ti->last_client);

	return ci;
}

dao_io_client_info *dao_io_delete_client (dao_io_client_info *ci, int want_callback)
{
	if (!ci)
		return NULL;

	// adjust clients linked list. used to close connections while stopping server
	dao_io_threads_info *ti = (dao_io_threads_info *) ci->tpi;
	dao_io_debug_message(DAO_IO_DEBUG_CLIENT, "ci list -> %08x < %08x > %08x == %08x <> %08x\n",
		ci->previous, ci, ci->next, ti->first_client, ti->last_client);

	if (want_callback)
		dao_io_do_callback (ci, DAO_IO_CLIENT_DELETE);

	dao_io_mutex_lock (&ti->lock);
	if (ci->previous == NULL && ci->next == NULL) {
		ti->first_client = NULL;
		ti->last_client = NULL;
	}
	else if (ci->next == NULL) {
		ci->previous->next = NULL;
		ti->last_client = ci->previous;
	}
	else if (ci->previous == NULL) {
		ci->next->previous = NULL;
		ti->first_client = ci->next;
	}
	else {
		ci->previous->next = ci->next;
		ci->next->previous = ci->previous;
	}
	dao_io_mutex_unlock (&ti->lock);
	dao_io_debug_message(DAO_IO_DEBUG_CLIENT, "ci list -< %08x < %08x > %08x == %08x <> %08x\n",
		ci->previous, ci, ci->next, ti->first_client, ti->last_client);

	// release client's data
	dao_io_delete_client_buffer (ci->eb);
	dao_io_delete_client_buffer (ci->wb);
	dao_io_delete_client_buffer (ci->rb);
	// external buffer ci->eb is not freed. allocated by external program
	if (ci->overlapped)
		free (ci->overlapped);
	free(ci);
	return NULL;
}

dao_io_client_buffer *dao_io_create_client_buffer (int size, int limit)
{
	dao_io_client_buffer* cb;
	cb        = calloc (1, sizeof(dao_io_client_buffer));
	cb->head  = 0;
	cb->mark  = 0;
	cb->tail  = 0;
	cb->allocated = size;
	cb->limit     = limit;
	if (size > 0) {
		cb->buffer  = (char*) malloc(cb->allocated+1);
		if (!cb->buffer) {
			dao_io_error_message("No memory\n");
			return NULL;
		}
		cb->buffer[cb->allocated] = '\0';
	}
	else
		cb->buffer = NULL;
	return cb;
}

dao_io_client_buffer *dao_io_delete_client_buffer (dao_io_client_buffer *cb)
{
	if (cb==NULL)
		return NULL;
	if (cb->allocated>0 && cb->buffer)	// don't free external buffers
		free (cb->buffer);
	if (cb)
		free (cb);
	return NULL;
}

// empty the buffer or shift data to the beginning
int dao_io_trim_buffer (dao_io_client_buffer* cb)
{
	if (cb->head == cb->tail)
		cb->head = cb->tail = 0;
	else if (cb->head>0) {
		memmove(cb->buffer, cb->buffer+cb->head, cb->tail-cb->head+1);
		cb->tail -= cb->head;
		cb->head = 0;
		cb->mark = 0;
	}
	return cb->tail;
}

// grow buffer if full;
char *dao_io_realloc_buffer (dao_io_client_buffer* cb)
{
	if (cb->allocated == cb->tail)
		dao_io_realloc_buffer_for (cb, cb->allocated);	// double buffer
	return cb->buffer;
}

// adjust buffer to have size free space
char *dao_io_realloc_buffer_for (dao_io_client_buffer* cb, int size)
{
	char *buffer;
	if (DAO_IO_CI_GET_FREE_SPACE(cb) > size)
		return cb->buffer;				// enough space
	cb->allocated += size - DAO_IO_CI_GET_FREE_SPACE(cb);
	if (cb->limit>0 && cb->allocated>cb->limit)
		cb->allocated = cb->limit;
	buffer = (char*)realloc(cb->buffer, cb->allocated+1);
	if (!buffer)
		dao_io_error_message("No memory (realloc)\n");
	else
		cb->buffer = buffer;
	return cb->buffer;
}

void dao_io_append_buffer (dao_io_client_buffer* cb, char* string)
{
	int size = strlen(string);
	dao_io_realloc_buffer_for (cb, size);
	if (size > DAO_IO_CI_GET_FREE_SPACE(cb))
		size = cb->allocated-cb->tail;		// was not able to allocate new buffer
	strncpy (DAO_IO_CI_GET_DATA_TAIL(cb), string, size);
	DAO_IO_CI_SLIDE_TAIL (cb, size);
	*DAO_IO_CI_GET_DATA_TAIL(cb) = '\0';				// if string was trimmed
}

char *dao_io_get_buffer_data (dao_io_client_buffer *cb)
{
	return cb->buffer+cb->head;
}


void dao_io_init_clients (void *tpi)
{
	dao_io_threads_info *ti = (dao_io_threads_info *) tpi;
	dao_io_mutex_init(&ti->lock);
}

void dao_io_clean_clients (void *tpi)
{
	dao_io_threads_info *ti = (dao_io_threads_info *) tpi;
	dao_io_mutex_destroy(&ti->lock);
}
