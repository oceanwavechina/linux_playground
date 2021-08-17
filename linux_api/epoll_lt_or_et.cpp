/*
 * epoll_lt_or_et.cpp
 *
 *  Created on: Aug 17, 2021
 *      Author: liuyanan
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/epoll.h>

void edage_trigger()
{
	/*
	 	运行结果：
			liuyanan@localhost:~/test$ ./a.out
			adfads
			welcome from: edage_trigger
			fadsf
			welcome from: edage_trigger

		分析：
			1. 当用户输入一组字符，这组字符被送入buffer，字符停留在buffer中，又因为buffer由空变为不空，
				所以ET返回读就绪，输出”welcome to epoll's world！”。

			2. 之后程序再次执行epoll_wait，此时虽然buffer中有内容可读，但是根据我们上节的分析，ET并不返回就绪，导致epoll_wait阻塞。
				（底层原因是ET下就绪fd的epitem只被放入rdlist一次）。

			3. 用户再次输入一组字符，导致buffer中的内容增多，根据我们上节的分析这将导致fd状态的改变，是对应的epitem再次加入rdlist，
				从而使epoll_wait返回读就绪，再次输出“Welcome to epoll's world！”。
	 */

	struct epoll_event ev, events[5];
	int epfd = epoll_create(1);
	ev.data.fd = STDIN_FILENO;
	ev.events = EPOLLIN|EPOLLET;	//监听读状态同时设置ET模式

	epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev);

	for(;;) {
		int nfds = epoll_wait(epfd, events, 5, -1);
		for(int i=0; i<nfds; ++i) {
			if(events[i].data.fd == STDIN_FILENO) {
				printf("welcome from: %s\n", __FUNCTION__);
			}
		}
	}
}

void level_trigger()
{
	/*
	 	运行结果：
			liuyanan@localhost:~/test$ ./a.out
			adfads
			welcome from: level_trigger
			welcome from: level_trigger
			welcome from: level_trigger
			welcome from: level_trigger
			welcome from: level_trigger
			welcome from: level_trigger
			welcome from: level_trigger
				......

		分析：
			程序陷入死循环，因为用户输入任意数据后，数据被送入buffer且没有被读出，
			所以LT模式下每次epoll_wait都认为buffer可读返回读就绪。导致每次都会输出”welcome to epoll's world！”。
	 */

	struct epoll_event ev, events[5];
	int epfd = epoll_create(1);
	ev.data.fd = STDIN_FILENO;
	ev.events = EPOLLIN;	//监听读状态， 默认是LT模式

	epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev);

	for(;;) {
		int nfds = epoll_wait(epfd, events, 5, -1);
		for(int i=0; i<nfds; ++i) {
			if(events[i].data.fd == STDIN_FILENO) {
				printf("welcome from: %s\n", __FUNCTION__);
			}
		}
	}
}

void level_trigger_with_readout()
{
	/*
	 	运行结果：
			liuyanan@localhost:~/test$ ./a.out
			fadfa
			welcome from: level_trigger_with_readout
			fadf
			welcome from: level_trigger_with_readout
		分析：
			本程序依然使用LT模式，但是每次epoll_wait返回读就绪的时候我们都将buffer（缓冲）中的内容read出来，所以导致buffer再次清空，
			下次调用epoll_wait就会阻塞。所以能够实现我们所想要的功能——当用户从控制台有任何输入操作时，输出”welcome to epoll's world！”
	 */

	struct epoll_event ev, events[5];
	int epfd = epoll_create(1);
	ev.data.fd = STDIN_FILENO;
	ev.events = EPOLLIN;	//监听读状态， 默认是LT模式

	epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev);

	for(;;) {
		int nfds = epoll_wait(epfd, events, 5, -1);
		for(int i=0; i<nfds; ++i) {
			if(events[i].data.fd == STDIN_FILENO) {
				char buf[1024] = {0};
				read(STDIN_FILENO, buf, sizeof(buf));
				printf("welcome from: %s\n", __FUNCTION__);
			}
		}
	}
}

void edage_trigger_with_mod_reset()
{
	/*
	 	运行结果：
			liuyanan@localhost:~/test$ ./a.out
			adfads
			welcome from: edage_trigger_with_mod_reset
			welcome from: edage_trigger_with_mod_reset
			welcome from: edage_trigger_with_mod_reset
			welcome from: edage_trigger_with_mod_reset
				......

		分析：
			程序依然使用ET，但是每次读就绪后都主动的再次MOD IN事件，我们发现程序再次出现死循环，也就是每次返回读就绪。
			但是注意，如果我们将MOD改为ADD，将不会产生任何影响。
			别忘了每次ADD一个描述符都会在epitem组成的红黑树中添加一个项，我们之前已经ADD过一次，再次ADD将阻止添加，所以在次调用ADD IN事件不会有任何影响。
	 */

	struct epoll_event ev, events[5];
	int epfd = epoll_create(1);
	ev.data.fd = STDIN_FILENO;
	ev.events = EPOLLIN|EPOLLET;	//监听读状态同时设置ET模式

	epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev);

	for(;;) {
		int nfds = epoll_wait(epfd, events, 5, -1);
		for(int i=0; i<nfds; ++i) {
			if(events[i].data.fd == STDIN_FILENO) {
				printf("welcome from: %s\n", __FUNCTION__);

				ev.data.fd = STDIN_FILENO;
				ev.events = EPOLLIN|EPOLLET;                        //设置ET模式
				epoll_ctl(epfd, EPOLL_CTL_MOD, STDIN_FILENO, &ev);    //重置epoll事件（ADD无效）
			}
		}
	}
}


void edage_trigger_out()
{
	/*
	 	如果 printf 有换行符 运行结果：
			liuyanan@localhost:~/test$ ./a.out
			welcome from: edage_trigger_out
			welcome from: edage_trigger_out
			welcome from: edage_trigger_out
			welcome from: edage_trigger_out
				......

		分析：
			这个程序的功能是只要标准输出写就绪，就输出“welcome to epoll's world”。我们发现这将是一个死循环。下面具体分析一下这个程序的执行过程：

			首先初始buffer为空，buffer中有空间可写，这时无论是ET还是LT都会将对应的epitem加入rdlist，导致epoll_wait就返回写就绪。
			程序想标准输出输出”welcome to epoll's world”和换行符，因为标准输出为控制台的时候缓冲是“行缓冲”,所以换行符导致buffer中的内容清空，
			这就对应第二节中ET模式下写就绪的第二种情况——当有旧数据被发送走时，即buffer中待写的内容变少得时候会触发fd状态的改变。
			所以下次epoll_wait会返回写就绪。如此循环往复。
	 */

	struct epoll_event ev, events[5];
	int epfd = epoll_create(1);
	ev.data.fd = STDOUT_FILENO;
	ev.events = EPOLLOUT|EPOLLET;	//监听 写 状态同时设置ET模式

	epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev);

	for(;;) {
		int nfds = epoll_wait(epfd, events, 5, -1);
		for(int i=0; i<nfds; ++i) {
			if(events[i].data.fd == STDOUT_FILENO) {
				printf("welcome from: %s\n", __FUNCTION__);
			}
		}
	}
}

void edage_trigger_out_without_line_end()
{
	/*
	 	如果 printf 有换行符 运行结果：
			liuyanan@localhost:~/test$ ./a.out


		分析：
			与程序四相比，程序五只是将输出语句的printf的换行符移除。我们看到程序成挂起状态。
			因为第一次epoll_wait返回写就绪后，程序向标准输出的buffer中写入“welcome to epoll's world！”，但是因为没有输出换行，
			所以buffer中的内容一直存在，下次epoll_wait的时候，虽然有写空间但是ET模式下不再返回写就绪。

			回忆第一节关于ET的实现，这种情况原因就是第一次buffer为空，导致epitem加入rdlist，返回一次就绪后移除此epitem，
			之后虽然buffer仍然可写，但是由于对应epitem已经不再rdlist中，就不会对其就绪fd的events的在检测了。
	 */

	struct epoll_event ev, events[5];
	int epfd = epoll_create(1);
	ev.data.fd = STDOUT_FILENO;
	ev.events = EPOLLOUT|EPOLLET;	//监听 写 状态同时设置ET模式

	epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev);

	for(;;) {
		int nfds = epoll_wait(epfd, events, 5, -1);
		for(int i=0; i<nfds; ++i) {
			if(events[i].data.fd == STDOUT_FILENO) {
				printf("welcome from: %s", __FUNCTION__);
			}
		}
	}
}

void level_trigger_out_without_line_end()
{
	/*
	 	如果 printf 有换行符 运行结果：
			liuyanan@localhost:~/test$ ./a.out
			welcome from: level_trigger_out_without_line_endwelcome from: level_trigger_out_without_line_end....

		分析：
			程序六相对程序五仅仅是修改ET模式为默认的LT模式，我们发现程序再次死循环。
			这时候原因已经很清楚了，因为当向buffer写入”welcome to epoll's world！”后，虽然buffer没有输出清空，
			但是LT模式下只有buffer有写空间就返回写就绪，所以会一直输出”welcome to epoll's world！”,
			当buffer满的时候，buffer会自动刷清输出，同样会造成epoll_wait返回写就绪。
	 */

	struct epoll_event ev, events[5];
	int epfd = epoll_create(1);
	ev.data.fd = STDOUT_FILENO;
	ev.events = EPOLLOUT;	//监听 写 状态同时设置LT模式

	epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev);

	for(;;) {
		int nfds = epoll_wait(epfd, events, 5, -1);
		for(int i=0; i<nfds; ++i) {
			if(events[i].data.fd == STDOUT_FILENO) {
				printf("welcome from: %s", __FUNCTION__);
			}
		}
	}
}

void edage_trigger_out_without_line_end_with_readout()
{
	/*
	 	如果 printf 有换行符 运行结果：
			liuyanan@localhost:~/test$ ./a.out
			welcome from: level_trigger_out_without_line_endwelcome from: level_trigger_out_without_line_end....


		分析：
			程序七相对于程序五在每次向标准输出的buffer输出”welcome to epoll's world！”后，重新MOD OUT事件。
			所以相当于每次都会返回就绪，导致程序循环输出。
	 */

	struct epoll_event ev, events[5];
	int epfd = epoll_create(1);
	ev.data.fd = STDOUT_FILENO;
	ev.events = EPOLLOUT|EPOLLET;	//监听 写 状态同时设置ET模式

	epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev);

	for(;;) {
		int nfds = epoll_wait(epfd, events, 5, -1);
		for(int i=0; i<nfds; ++i) {
			if(events[i].data.fd == STDOUT_FILENO) {
				printf("welcome from: %s", __FUNCTION__);
				ev.data.fd = STDOUT_FILENO;
				ev.events = EPOLLOUT|EPOLLET;                        //设置ET模式
				epoll_ctl(epfd, EPOLL_CTL_MOD, STDOUT_FILENO, &ev);    //重置epoll事件（ADD无效）
			}
		}
	}
}

/*
     经过前面的案例分析，我们已经了解到，当epoll工作在ET模式下时，对于读操作，如果read一次没有读尽buffer中的数据，那么下次将得不到读就绪的通知，
     造成buffer中已有的数据无机会读出，除非有新的数据再次到达。对于写操作，主要是因为ET模式下fd通常为非阻塞造成的一个问题——如何保证将用户要求写的数据写完。

     要解决上述两个ET模式下的读写问题，我们必须实现：
		对于读，只要buffer中还有数据就一直读；
		对于写，只要buffer还有空间且用户请求写的数据还未写完，就一直写。

 	 ET模式下的accept问题
      请思考以下一种场景：在某一时刻，有多个连接同时到达，服务器的 TCP 就绪队列瞬间积累多个就绪连接，
      由于是边缘触发模式，epoll 只会通知一次，accept 只处理一个连接，导致 TCP 就绪队列中剩下的连接都得不到处理。在这种情形下，我们应该如何有效的处理呢？

      解决的方法是：
      	  解决办法是用 while 循环抱住 accept 调用，处理完 TCP 就绪队列中的所有连接后再退出循环。
      	  如何知道是否处理完就绪队列中的所有连接呢？
      	  	  accept  返回 -1 并且 errno 设置为 EAGAIN 就表示所有连接都处理完。


 */

int main(int argc, char **argv) {

	// edage_trigger();

	// level_trigger();

	// level_trigger_with_readout();

	// edage_trigger_with_mod_reset();

	// edage_trigger_out_without_line_end();

	// level_trigger_out_without_line_end();

	edage_trigger_out_without_line_end_with_readout();
}
