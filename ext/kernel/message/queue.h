
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

#ifndef KERNEL_MESSAGE_QUEUE_H
#define KERNEL_MESSAGE_QUEUE_H

#define KERNEL_MESSAGE_QUEUE_CACHE_LINE_SIZE 64

#include <semaphore.h>

struct dao_message_queue {
	unsigned int message_size;
	unsigned int max_depth;
	void *memory;
	void **freelist;
	void **queue_data;
	struct {
		sem_t unnamed_sem;
		sem_t *sem;
		unsigned int blocked_readers;
		int free_blocks;
		unsigned int allocpos __attribute__((aligned(KERNEL_MESSAGE_QUEUE_CACHE_LINE_SIZE)));
		unsigned int freepos __attribute__((aligned(KERNEL_MESSAGE_QUEUE_CACHE_LINE_SIZE)));
	} allocator __attribute__((aligned(KERNEL_MESSAGE_QUEUE_CACHE_LINE_SIZE)));
	struct {
		sem_t unnamed_sem;
		sem_t *sem;
		unsigned int blocked_readers;
		int entries;
		unsigned int readpos __attribute__((aligned(KERNEL_MESSAGE_QUEUE_CACHE_LINE_SIZE)));
		unsigned int writepos __attribute__((aligned(KERNEL_MESSAGE_QUEUE_CACHE_LINE_SIZE)));
	} queue __attribute__((aligned(KERNEL_MESSAGE_QUEUE_CACHE_LINE_SIZE)));
};

int dao_message_queue_init(struct dao_message_queue *queue, int message_size, int max_depth);
void *dao_message_queue_message_alloc(struct dao_message_queue *queue);
void *dao_message_queue_message_alloc_blocking(struct dao_message_queue *queue);
void dao_message_queue_message_free(struct dao_message_queue *queue, void *message);
void dao_message_queue_write(struct dao_message_queue *queue, void *message);
void *dao_message_queue_tryread(struct dao_message_queue *queue);
void *dao_message_queue_read(struct dao_message_queue *queue);
void dao_message_queue_destroy(struct dao_message_queue *queue);

#endif
