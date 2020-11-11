
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

#ifndef DAO_KERNEL_IO_SOCKET_H
#define DAO_KERNEL_IO_SOCKET_H

#include "kernel/io/definitions.h"
#include "kernel/io/client.h"

int dao_io_init_sockets(void);
dao_io_socket_t dao_io_create_server_socket(char* address, int port);
dao_io_socket_t dao_io_create_socket();
dao_io_socket_t dao_io_close_socket(dao_io_socket_t a_socket);
void dao_io_block_socket(dao_io_socket_t a_socket, int block);
int dao_io_bind_socket(dao_io_socket_t server_socket, char* address, int port);
int dao_io_start_listening(dao_io_socket_t server_socket);
dao_io_client_info *dao_io_do_accept(void *tpi, dao_io_socket_t server_socket);
int dao_io_do_read(dao_io_client_info *ci);
int dao_io_do_write(dao_io_client_info *ci);
int dao_io_query_write(dao_io_client_info *ci);
void dao_io_do_callback(dao_io_client_info *ci, int op);
void dao_io_get_hostname(char *hostname, int namesize);

#endif /* DAO_KERNEL_IO_SOCKET_H */
