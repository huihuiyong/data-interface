#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>

#include "lib.h"

int open_server(char * channle_name)
{
    int server_fd;
    socklen_t len;
    struct sockaddr_un server;

    // 创建套接字
    if ((server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket error");
        exit(EXIT_FAILURE);
    }

    
    server.sun_family = AF_UNIX;
    strcpy(server.sun_path, channle_name);
    
    unlink(channle_name);  // 确保路径可用

    // 绑定套接字到指定路径
    if (bind(server_fd, (struct sockaddr*)&server, sizeof(server)) == -1) {
        perror("bind error");
        exit(EXIT_FAILURE);
    }

    // 监听连接
    if (listen(server_fd, 5) == -1) {
        perror("listen error");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on socket: %s\n", channle_name);

    // 接受连接
    len = sizeof(server);
    if ((server_fd = accept(server_fd, (struct sockaddr*)&server, &len)) == -1) {
        perror("accept error");
        exit(EXIT_FAILURE);
    }
    return server_fd;
}

int open_client(char * channle_name)
{
    int client_fd;
    struct sockaddr_un server;

    // 创建套接字
    if ((client_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket error");
        exit(EXIT_FAILURE);
    }

    server.sun_family = AF_UNIX;
    strcpy(server.sun_path, channle_name);

    // 连接到服务器
    if (connect(client_fd, (struct sockaddr*)&server, sizeof(server)) == -1) {
        perror("connect error");
        close(client_fd);
        printf("The server is not started, please reconnect!!!\n");
        exit(EXIT_FAILURE);
    }

    return client_fd;
}

void close_channle(int fd, char *channle_name)
{
    // 关闭套接字
    close(fd);
    unlink(channle_name);
}


int send_data(int fd, const void * buf, size_t len)
{
    int ret;

    // 发送消息
    ret = write(fd, buf, len);

    return len;
}

int recv_data(int fd, void * buf, size_t len)
{
    int ret;

    // 接收消息
    ret = recv(fd,buf,len,0);
}