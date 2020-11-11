
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

#ifndef DAO_KERNEL_IO_CLIENT_H
#define DAO_KERNEL_IO_CLIENT_H

#include "kernel/io/definitions.h"

enum // socket operations for IOCP and tasks
{
	DAO_IO_OP_NONE,
	DAO_IO_OP_ACCEPT,
	DAO_IO_OP_READ,
	DAO_IO_OP_WRITE
};

enum // callback operation
{
	DAO_IO_CLIENT_CREATE,
	DAO_IO_CLIENT_DELETE,
	DAO_IO_CLIENT_READ,
	DAO_IO_CLIENT_WRITE,
	DAO_IO_CLIENT_DEFFERED_READ,
	DAO_IO_CLIENT_DEFFERED_WRITE
};

#define DAO_IO_CI_GET_DATA_HEAD(cb)       (cb->buffer + cb->head)
#define DAO_IO_CI_GET_DATA_MARK(cb)       (cb->buffer + cb->mark)
#define DAO_IO_CI_GET_DATA_TAIL(cb)       (cb->buffer + cb->tail)
#define DAO_IO_CI_GET_DATA_SIZE(cb)       (cb->tail - cb->head)
#define DAO_IO_CI_GET_FREE_SPACE(cb)      (cb->allocated - cb->tail)
#define DAO_IO_CI_GET_INCREASED_SIZE(cb)  (cb->tail - cb->mark)
#define DAO_IO_CI_GET_DECREASED_SIZE(cb)  (cb->head - cb->mark)
#define DAO_IO_CI_SLIDE_HEAD(cb,n)        (cb->head += n)
#define DAO_IO_CI_SLIDE_TAIL(cb,n)        (cb->tail += n)
#define DAO_IO_CI_SET_DATA_TAIL(cb,n)     (cb->tail = n)
#define DAO_IO_CI_CLEAR_BUFFER(cb)        (cb->head = cb->mark = cb->tail = 0)
#define DAO_IO_CI_SET_MARK_AT_HEAD(cb)    (cb->mark = cb->head)
#define DAO_IO_CI_SET_MARK_AT_TAIL(cb)    (cb->mark = cb->tail)

typedef struct _dao_io_client_info   dao_io_client_info;
typedef struct _dao_io_client_buffer dao_io_client_buffer;
typedef struct _dao_io_client_config dao_io_client_config;

struct _dao_io_client_buffer {
	char *buffer;	// 1 byte more than allocated
	int allocated;
	int limit;
	int head;
	int mark;		// previous tail after a write
	int tail;
};

// can be adjusted after
struct _dao_io_client_config {
	int use_write_events;		// DAO_IO_TRUE if checking write events. faster if we don't check write events (on kqueue)
	int read_buffer_size;		// default read buffer size
	int write_buffer_size;	// default write buffer size
	int read_buffer_limit;	// maximum read buffer size
	int write_buffer_limit;	// maximum write buffer size
};

// the use_ variables can be set during creation callback by application
struct _dao_io_client_info {
	void* tpi;					// pointer to the server
	dao_io_client_config *cc;		// reference to common clients config (tpi->cc)
	int use_write_events;		// if DAO_IO_FALSE, may be changed to DAO_IO_TRUE if write would block
	dao_io_socket_t socket;		// client socket
	struct in_addr remote_addr;	// client address
	int remote_port;			// client remote port
	struct in_addr local_addr;	// server address
	int local_port;				// server port
	int thread_id;				// reference to the thread number for debugging purpose
	dao_io_client_buffer* rb;		// read buffer
	dao_io_client_buffer* wb;		// write buffer
	dao_io_client_buffer* eb;		// external write buffer
	int use_eb;				// used internally by dao_io_do_write and dao_io_start_writing/dao_io_write_completed
	int read_end;				// DAO_IO_TRUE if no more data
	int error;				// DAO_IO_TRUE if I/O error
	int operation;			// IOCP and tasks operation
	void *overlapped;			// IOCP overlapped buffer
	int callback_has_written;	// I/O management. indication that data was written during callback
	int can_write;			// I/O management. if DAO_IO_FALSE, indicates a write started and waiting for readiness signal
	int write_pending;		// I/O management. indicates that a buffer is ready to be sent on write readiness
	dao_io_client_info* previous;
	dao_io_client_info* next;
	int step;					// set by user to organize read/write sequences
	void *context;				// user defined data
};

void dao_io_preset_client_config(void *tpi);
dao_io_client_info *dao_io_create_client(void *tpi, dao_io_socket_t socket);
dao_io_client_info *dao_io_delete_client(dao_io_client_info *ci, int want_callback);
dao_io_client_buffer *dao_io_create_client_buffer(int size, int limit);
dao_io_client_buffer *dao_io_delete_client_buffer();
int dao_io_trim_buffer(dao_io_client_buffer *cb);
char *dao_io_realloc_buffer(dao_io_client_buffer *cb);
char *dao_io_realloc_buffer_for(dao_io_client_buffer* cb, int size);
void dao_io_append_buffer(dao_io_client_buffer* cb, char* string);
char *dao_io_get_buffer_data(dao_io_client_buffer *cb);

void dao_io_init_clients(void *tpi);
void dao_io_clean_clients(void *tpi);

#endif /* DAO_KERNEL_IO_CLIENT_H */
