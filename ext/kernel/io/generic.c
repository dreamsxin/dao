
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

#include "kernel/io/generic.h"

#if !HAVE_EPOLL && !HAVE_KQUEUE

dao_io_poller_info *dao_io_create_poller(void* parent, char* address, int port, char *networks, void (*callback)(dao_io_client_info *ci,int op))
{
	return NULL;
}

int dao_io_start_poller(dao_io_poller_info *pi)
{
	return DAO_IO_FALSE;
}

void *dao_io_stop_poller(dao_io_poller_info *pi)
{
	pi->stop = DAO_IO_TRUE;
	return NULL;
}

int dao_io_poller_is_alive(dao_io_poller_info *pi)
{
	return 0;
}

#endif
