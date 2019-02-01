/*
 * socket_client.cpp
 *
 *  Created on: Nov 21, 2016
 *      Author: liuyanan
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

void error_handling(char* message);

void error_handling(char* message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char **argv) {
    int sock;
    struct sockaddr_in serv_addr;
    char message[30];
    int str_len;
    memset(&serv_addr, 0, sizeof(serv_addr));

    if (argc != 3)
    {
        printf("Usage: /%s <IP> <Port>\n",argv[0]);
        exit(1);
    }

    //创建用于internet的流协议(TCP)socket
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        error_handling("socket() error");
    }

    printf("1\n");

    //设置一个socket地址结构client_addr,代表客户机internet地址, 端口
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    //把socket和socket地址结构联系起来
    if( connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) == -1) {
        error_handling("connect() error");
    }

    printf("2\n");

    const char* msg = "abcd";
    send(sock, msg, strlen(msg)-1 , 0 );

    printf("3\n");

    str_len = read(sock, message, sizeof(message) - 1);
    if (str_len == -1) {
        error_handling("read() error");
    }

    printf("3\n");

    printf("Message from server : %s \n", message);
    close(sock);
    return 0;
}
