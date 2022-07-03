/*
 * measure_syscall_time.cpp
 *
 *  Created on: Sep 29, 2021
 *      Author: liuyanan
 */

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>

using namespace std;

#if __GLIBC_PREREQ(2, 3)
#if defined FUTEX_WAIT || defined FUTEX_WAKE
#include <linux/futex.h>
#else
#define FUTEX_WAIT      0
#define FUTEX_WAKE      1
#endif

#ifndef __NR_futex
#define __NR_futex     202  /* syscall number */
#endif
#endif


void measure_getpid()
{
	for(int i=0; i<100; ++i) {
		pid_t a = getpid();
	}
}


void* __wait(void* var)
{
	syscall(__NR_futex, var, FUTEX_WAIT, *(int *)var, NULL);

	return NULL;
}

void measure_futex()
{
	static const int NUM_THREADS = 100;
	pthread_t tids[NUM_THREADS];
	for(int i = 0; i < NUM_THREADS; ++i)
	{
		int ret = pthread_create(&tids[i], NULL, __wait, &tids[i]);
		if (ret != 0) {
		   cout << "pthread_create error: error_code=" << ret << endl;
		}
	}

	pthread_exit(NULL);
}

int main(int argc, char **argv) {

	// measure_getpid();
	cout << "test" << endl;

	measure_futex();

	return 0;
}
