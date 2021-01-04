
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

#include "kernel/io/tasks.h"

#include <stdlib.h>		// calloc, free, realloc
#include <string.h>		// memmove, memset
#include <unistd.h>		// sleep
#include <stdio.h>		// fprintf

#include "kernel/io/support.h"
#include "kernel/io/sockets.h"
#include "kernel/io/threads.h"

static dao_io_tasks_data * td;

void dao_io_init_tasks()
{
	if (td != NULL)
		return;
	td = (dao_io_tasks_data *) calloc (1, sizeof(dao_io_tasks_data));
	td->threads = (dao_io_thread_t *) calloc(DAO_IO_BLOCK_SIZE, sizeof(dao_io_thread_t));
	td->max_threads = 0;
	td->first_free_thread = 0;
	td->tasks = (dao_io_task_info *) calloc(DAO_IO_BLOCK_SIZE, sizeof(dao_io_task_info));
	td->max_tasks = 0;
	td->first_free_task = 0;
	td->first_task = 0;
	pthread_mutex_init(&td->lock,NULL);
	pthread_cond_init(&td->cond,NULL);
	td->must_Exit = 0;
}

void dao_io_clean_tasks()
{
	int it;
	td->must_Exit = 1;
	pthread_cond_broadcast(&td->cond);
    for (it=0; it < td->first_free_thread; it++) {
    	if (td->threads[it]!=(dao_io_thread_t)0)
    		pthread_join(td->threads[it], NULL);
    	td->threads[it] = (dao_io_thread_t)0;
    }
    if (td)
    	free(td);
    td = NULL;
}

void dao_io_push_thread (dao_io_thread_t thread)
{
	if (td->first_free_thread >= td->max_threads) {
		td->max_threads += DAO_IO_BLOCK_SIZE;
		if (realloc ((void *)td->threads, td->max_threads) == NULL)
			dao_io_error_message("No memory (realloc)\n");

	}
	td->threads[td->first_free_thread++] = thread;
}

// remove a thread from the list
void dao_io_delete_thread (dao_io_thread_t thread)
{
	int it;
    for (it=0; it < td->first_free_thread; it++) {
    	if (td->threads[it]==thread) {
    		int nthreads = td->first_free_thread - it - 1;
    		if (nthreads > 0)
    			memmove (&td->threads[it], &td->threads[it+1], nthreads*sizeof(dao_io_thread_t));
    		--td->first_free_thread;
    		break;
    	}
     }
}

int dao_io_get_running_task_threads()
{
	return td? td->first_free_thread: 0;
}

int dao_io_start_one_task_thread()
{
	if (td==NULL)
		dao_io_init_tasks ();
	dao_io_thread_t pid;
	int ret = pthread_create(&pid, 0, dao_io_tasks_thread, td);
	dao_io_push_thread(pid);
	return ret;
}

void dao_io_stop_one_task_thread()
{
	fprintf (stderr, "dao_io_stop_one_task_thread\n");
	// push an empty task to signal the end
	if (dao_io_get_running_task_threads() > 0)
		dao_io_enqueue_task (NULL);
}

// return DAO_IO_ERROR if error or number of tasks pushed
int dao_io_push_task (dao_io_client_info *ci)
{
	if (td->first_free_task >= td->max_tasks) {
		td->max_tasks += DAO_IO_BLOCK_SIZE;
		if (realloc ((void *)td->tasks, td->max_tasks) == NULL) {
			dao_io_error_message("No memory (realloc)\n");
			return DAO_IO_ERROR;
		}
	}
	td->tasks[td->first_free_task++].ci = ci;
	return td->first_free_task - td->first_task;
}

dao_io_client_info *dao_io_pop_task ()
{
	dao_io_client_info *ci = NULL;
	if (td->first_task >= 0)
		ci = td->tasks[td->first_task++].ci;

	if (td->first_task >= td->first_free_task)
		td->first_task = td->first_free_task = 0;		// queue empty; reset pointers
	else if (td->first_task >= td->max_tasks/2) {		// half empty; shift data at the beginning
		int ntasks = td->first_free_task-td->first_task;
		memmove (&td->tasks[0], &td->tasks[td->first_task], ntasks*sizeof(dao_io_task_info));
		td->first_free_task -= td->first_task;
		td->first_task = 0;
	}
	return ci;
}

// return DAO_IO_ERROR if error or number of tasks enqueued
int dao_io_enqueue_task (dao_io_client_info *ci)
{
	int ntasks;
	dao_io_threads_info *ti = (dao_io_threads_info *)ci->tpi;
	if (!ti->allow_tasks)
		return DAO_IO_ERROR;
	dao_io_debug_message(DAO_IO_DEBUG_CLIENT, "Task push\n");

	if (dao_io_get_running_task_threads() < 1)
		dao_io_start_one_task_thread();
	pthread_mutex_lock(&td->lock);
	ntasks = dao_io_push_task(ci);
	pthread_mutex_unlock(&td->lock);
	if (ntasks < 0)
		return DAO_IO_ERROR;
	pthread_cond_signal(&td->cond);
	return ntasks;
}

dao_io_callback_t dao_io_tasks_thread(void *data)
{
	dao_io_client_info *ci;

	while (!td->must_Exit) {
		pthread_mutex_lock(&td->lock);
		if (td->first_free_task <= 0) {
			while (td->first_free_task==0 && !td->must_Exit)
				pthread_cond_wait(&td->cond, &td->lock);
			if (td->must_Exit) {
				pthread_mutex_unlock(&td->lock);
				break;
			}
		}
		ci = dao_io_pop_task();
		if (ci == NULL)					// request to stop thread
			dao_io_delete_thread (pthread_self());
		pthread_mutex_unlock(&td->lock);
		if (ci == NULL) {				// request to stop thread
			dao_io_delete_thread (pthread_self());
			pthread_detach(pthread_self());
			break;
		}

		dao_io_debug_message(DAO_IO_DEBUG_CLIENT, "Task pop\n");
		if (ci->operation == DAO_IO_OP_READ)
			dao_io_do_callback(ci, DAO_IO_CLIENT_DEFFERED_READ);
		else if (ci->operation == DAO_IO_OP_WRITE)
			dao_io_do_callback(ci, DAO_IO_CLIENT_DEFFERED_WRITE);
	}
	return (dao_io_callback_t) 0;
}
