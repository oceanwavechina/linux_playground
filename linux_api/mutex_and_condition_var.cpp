/*
 * mutex.cpp
 *
 *  Created on: Feb 17, 2017
 *      Author: liuyanan
 */

#include <iostream>
#include <queue>


#include <pthread.h>
#include <unistd.h>

using namespace std;

#define USING_CONDITION_VAR


pthread_mutex_t g_mutex;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

queue<int> g_product;


void* produce(void* ) {
	for(int i=0; i<10; ++i) {
		pthread_mutex_lock(&g_mutex);

		/*
		 * 锁两次也不行，因为这不是可重入锁
		 */
//		pthread_mutex_lock(&g_mutex);
		g_product.push(i);

		//cout << "before cond_signal" << endl;
		pthread_cond_signal(&cond);
		//cout << "after cond_signal" << endl;

		int ret = pthread_mutex_unlock(&g_mutex);

		/*
		 * 当mutex被释放两次时,会产生异常后果(man page)：
		 * If the current thread holds the lock on mutex, then the pthread_mutex_unlock() function unlocks mutex.
		 * Calling pthread_mutex_unlock() with a mutex that the calling thread does not hold will result in undefined behavior.
		 *
		 * 当时面试时遇到问到的这个问题，面试官说“不会有影响，该函数会判断锁的持有者”， 他说的应该不是这种场景
		 */
//		cout << "first unlock with return:" << ret <<  endl;
//		ret = pthread_mutex_unlock(&g_mutex);
//		cout << "second unlock with return:" << ret <<  endl;
	}
}


void* consumer(void* ) {
	for(int i=0; i<10;) {
		int ret = pthread_mutex_lock(&g_mutex);

		if (g_product.empty()) {
			pthread_mutex_unlock(&g_mutex);
			continue;

			cout << "before cond_wait" << endl;
			pthread_cond_wait(&cond,&g_mutex);
			cout << "after cond_wait" << endl;
		}

		++i;

		cout << "consumer" << g_product.front() << endl;
		g_product.pop();
		pthread_mutex_unlock(&g_mutex);
	}

}


int main(int argc, char **argv) {

	pthread_mutex_init(&g_mutex, NULL);

	pthread_t tid1, tid2;
	pthread_create(&tid1, NULL, consumer, NULL);
	sleep(1);		// 让队列是空的，检验 condition_var 的 wait 行为
	pthread_create(&tid2, NULL, produce, NULL);

	void* ret_val;
	pthread_join(tid1, &ret_val);
	pthread_join(tid2, &ret_val);

	return 0;
}



