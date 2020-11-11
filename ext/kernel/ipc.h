
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
  |          ZhuZongXin <dreamsxin@qq.com>                                 |
  +------------------------------------------------------------------------+
*/


#ifndef DAO_KERNEL_IPC_H
#define DAO_KERNEL_IPC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

/*
 * Inter Process Communication server
 */
typedef struct {
	unsigned int s, s2;
	struct sockaddr_un local, remote;
	unsigned int msglen;
} dao_ipc_server;

/* create unix domain socket inter process communication server */
dao_ipc_server dao_ipc_create(const char *path) {
	dao_ipc_server srv;
	srv.s = socket(AF_UNIX, SOCK_STREAM, 0);
	
	/* bind */
	srv.local.sun_family = AF_UNIX;
	strcpy(srv.local.sun_path, path);
	unlink(srv.local.sun_path);
	srv.msglen = strlen(srv.local.sun_path) + sizeof(srv.local.sun_family);
	
	bind(srv.s, (struct sockaddr *)&srv.local, srv.msglen);

	/* listen */
	listen(srv.s, 5);
	return srv;
}

/* accept ipc client */
void dao_ipc_server_accept(dao_ipc_server *srv) {
	srv->msglen = sizeof(srv->remote);
	srv->s2 = accept(srv->s, (struct sockaddr *)&(srv->remote), &srv->msglen);
}

/* read from ipc server */
int dao_ipc_server_read(dao_ipc_server *srv, char *buf, int buflen) {
	srv->msglen = recv(srv->s2, buf, buflen, 0);
	return srv->msglen > 0;
}

/* write to ipc client */
void dao_ipc_server_send(dao_ipc_server *srv, char *buf, int buflen) {
	send(srv->s2, buf, buflen, 0);
}

/* close srv */
void dao_ipc_server_close(dao_ipc_server srv) {
	close(srv.s2);
}

/*
 * Inter Process Communication client
 */
typedef enum {
	DAO_IPC_OK = 0,
	DAO_IPC_UNKNOWN,
	DAO_IPC_SOCKET_ERROR,
	DAO_IPC_CONNECT_FAILED
} dao_ipc_status;

typedef struct {
	int s, t, len;
	struct sockaddr_un remote;

	/* status */
	dao_ipc_status error;
} dao_ipc_client;

dao_ipc_client dao_ipc_connect(const char *path) {
	dao_ipc_client cli;
	cli.error = IPC_UNKNOWN;

	cli.s = socket(AF_UNIX, SOCK_STREAM, 0);
	if (cli.s == -1) {
		cli.error = IPC_SOCKET_ERROR;
		return cli;
	}
	
	cli.remote.sun_family = AF_UNIX;
	strcpy(cli.remote.sun_path, path);
	cli.len = strlen(cli.remote.sun_path) + sizeof(cli.remote.sun_family);
	if (connect(cli.s, (struct sockaddr *)&cli.remote, cli.len) == -1) {
		cli.error = IPC_CONNECT_FAILED;
		return cli;
	}
	
	cli.error = IPC_OK;
	return cli;
}

/* sends ipc data */
int dao_ipc_client_send(dao_ipc_client *cli, char *buf, int buflen) {
	return send(cli->s, buf, buflen, 0);
}

/* read ipc data */
int dao_ipc_client_read(dao_ipc_client *cli, char *buf, int buflen) {
	cli->t = recv(cli->s, buf, buflen, 0);
	return cli->t > 0;
}

/* close ipc socket */
void dao_ipc_client_close(dao_ipc_client cli) {
	close(cli.s);
}

#endif /* DAO_KERNEL_IPC_H */
