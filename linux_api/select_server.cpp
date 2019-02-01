/*
 * select.cpp
 *
 *  Created on: Nov 21, 2016
 *      Author: liuyanan
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>

#include <iostream>
using namespace std;


#define MAX_CLIENT	1024
#define PORT 8888

int main(int argc, char* argv[])
{
	int client_fds[MAX_CLIENT] = {0};
	int listen_fd = 0;

	char buf[1024] = {0};
	int opt = 1;

	fd_set read_fds;

	const char* msg = "Msg From Server ! \r\n";

	for(int i=0; i<MAX_CLIENT; ++i) {
		client_fds[i] = 0;
	}

	// create listen socket
	if((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("create socket failed !");
		exit(EXIT_FAILURE);
	}

	// reuse socket address
	if(setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) < 0) {
		perror("setsockopt failed !");
		exit(EXIT_FAILURE);
	}

	// bind socket with address
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	int r =  ::bind(listen_fd, (const struct sockaddr*)&address, sizeof(address));
	if( r < 0) {
		perror("bind failed !");
		exit(EXIT_FAILURE);
	}

	// listen: set pending connection queue
	if(listen(listen_fd, 10) < 0) {
		perror("listen failed !");
		exit(EXIT_FAILURE);
	}

	cout << "listen on port:" << PORT << " ..." << endl;

	// handler event
	int addrlen = sizeof(address);
	int max_fd = -1;
	int new_client_fd;

	while(true) {

		// clear socket set
		FD_ZERO(&read_fds);

		// add listen socket to the set
		FD_SET(listen_fd, &read_fds);
		max_fd = listen_fd;

		// add client fds
		for(int i=0; i<MAX_CLIENT; ++i) {
			if (client_fds[i] > 0) {
				FD_SET(client_fds[i], &read_fds);
			}

			if (client_fds[i] > max_fd) {
				max_fd = client_fds[i];
			}
		}

		//wait for an activity on one of the sockets , timeout is NULL , so wait indefinitely
		int ret = select(max_fd+1, &read_fds, NULL, NULL, NULL);

		if (ret < 0 && errno != EINTR) {
			perror("select error!");
			exit(EXIT_FAILURE);
		}



        //If something happened on the master socket , then its an incoming connection
        if (FD_ISSET(listen_fd, &read_fds)) {

        	if ((new_client_fd = accept(listen_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            //inform user of socket number - used in send and receive commands
            printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_client_fd , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

            //send new connection greeting message
            if( send(new_client_fd, msg, strlen(msg), 0) != strlen(msg) ){
                perror("send");
            }

            //add new socket to array of sockets
            for (int i = 0; i < max_fd; ++i) {

            	//if position is empty
                if( client_fds[i] == 0 ) {
                	client_fds[i] = new_client_fd;
                    printf("Adding to list of sockets as %d\n" , i);
                    break;
                }
            }
        }

        int valread = -1;
        //else its some IO operation on some other socket :)
        for (int i = 0; i < max_fd; i++)
        {
            if (FD_ISSET( client_fds[i], &read_fds))
            {
                //Check if it was for closing , and also read the incoming message
                if ((valread = read( client_fds[i] , buf, 1024)) == 0)
                {
                    //Somebody disconnected , get his details and print
                    getpeername(client_fds[i] , (struct sockaddr*)&address , (socklen_t*)&addrlen);
                    printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

                    //Close the socket and mark as 0 in list for reuse
                    close( client_fds[i]);
                    client_fds[i] = 0;
                }

                //Echo back the message that came in
                else
                {
                    //set the string terminating NULL byte on the end of the data read
                    buf[valread] = '\0';
                    send(client_fds[i] , buf , strlen(buf) , 0 );
                }
            }
        }

	}
}
