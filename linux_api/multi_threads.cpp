/*
 * muti_threads.cpp
 *
 *  Created on: Aug 23, 2021
 *      Author: liuyanan
 */

#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

#define ERROR -1
#define OK 0


void *thread_function(void *arg)
{
	long i = reinterpret_cast<long>( arg);
	printf("sub thread %ld info: ppid=%d, pid=%d, tid=%lu\n", i, getppid(), getpid(), pthread_self());

	sleep(1000);

	return NULL;
}

int main(void)
{
    pthread_t tids[5];
    int ret[5];

    printf("main thread info: ppid=%d, pid=%d, tid=%lu\n", getppid(), getpid(), pthread_self());

    for(int i=0; i<5; ++i) {
    	pthread_create(&tids[i], NULL, thread_function, reinterpret_cast<void*>(i));
    }

    for(int i=0; i<5; ++i) {
    	pthread_join(tids[i], (void**)&ret[i]);
    }

    sleep(1000);

    return OK;
}
