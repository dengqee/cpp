#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>

#define MYPORT 7000
#define BUFFER_SIZE 1024

int client_connect()
{
    ///定义sockfd
    int sock_cli = socket(AF_INET, SOCK_STREAM, 0); //1. 创建流套接字
    // AF_INET 表示使用TCP/IP协议族, SOCK_STREAM 表示使用TCP协议, SOCK_DGRAM 表示使用UDP协议

    ///定义sockaddr_in
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(MYPORT);                 ///服务器端口
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); ///服务器ip

    //连接服务器，成功返回0，错误返回-1
    if (connect(sock_cli, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) // 2. 建立到达服务器的链接(三次握手)
    {
        perror("connect");
        exit(1);
    }

    char sendbuf[BUFFER_SIZE];
    char recvbuf[BUFFER_SIZE];
    while (fgets(sendbuf, sizeof(sendbuf), stdin) != NULL)
    {
        send(sock_cli, sendbuf, strlen(sendbuf), 0); // 3.1 用套接字与服务器进行通信
        if (strcmp(sendbuf, "exit\n") == 0)
            break;
        recv(sock_cli, recvbuf, sizeof(recvbuf), 0); // 3.2 用套接字与服务器进行通信
        fputs(recvbuf, stdout);

        memset(sendbuf, 0, sizeof(sendbuf));
        memset(recvbuf, 0, sizeof(recvbuf));
    }

    close(sock_cli); // 4. 关闭套接字
    return 0;
}

int client_select()
{

}

int main(int argc, char*argv[])
{
    client_connect();
}