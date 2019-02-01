/*
 * test.cpp
 *
 *  Created on: Nov 19, 2016
 *      Author: liuyanan
 */

#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

using namespace std;

int __main()
{
	int x = 1;

	pid_t pid;

	if ((pid = fork()) < 0 ) {
		cerr << "fork error:" << strerror(errno);
	}

	if (pid == 0) { // child
//		cout << "child get pid:" <<getpid() << endl;
//		cout << "child get p_pid:" <<getppid() << endl;

		cout << "child :x = " << ++x << endl;
		int count = sleep(2);
		cout << "child exit, with cout:" << count << ", address of x: " << &x << endl;
		exit(100);

	} else {
//		cout << "parent get pid:" <<getpid() << endl;
//		cout << "parent get p_pid:" <<getppid() << endl;

		cout << "parent :x = " << --x << ", address of x: " << &x << endl;

		int status;
		waitpid(pid, &status, 0);
		cout << "child exit with code:" << WEXITSTATUS(status) << endl;
	}

	return 0;
}


void doit()
{
	fork();
	fork();
	cout << "hello" << endl;
	return ;
}


int _main()
{
	doit();
	cout << "hello" << endl;
	exit(0);
}

int main_1()
{
	pid_t pid;

	if (fork() == 0) {
		cout << "child born" << endl;
		pause();
		cout << "nerver reach here" << endl;
		exit(0);
	}

	kill(pid, SIGKILL);

	return 0;
}


void handler(int sig)
{
	static int beeps = 0;

	cout << "beep" << endl;
	if (++beeps < 5)
		alarm(1);
	else {
		cout << "boom" << endl;
		exit(0);
	}
}


int main()
{
	signal(SIGALRM, handler);
	alarm(1);

	while(1)
		;

	exit(0);
}

















