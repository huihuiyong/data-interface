#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include "lib.h"


int main() {
    
    char buff[100];
    memset(buff, 0, sizeof(buff));
    int date[10] = {111, 222, 333, 444, 555, 666, 777, 888, 999, 1010};  

    //初始化
    int recv_fd = open_server(CHANNLE_NAME);
    if(recv_fd>0){
        printf("recv_ok:%d\n",recv_fd);
    }else{
        printf("send_ERR: %d\n", recv_fd);
    }

    //接收
    int ret = recv_data(recv_fd,buff, sizeof(buff));
    if(ret > 0){
        printf("recv1 successfully %d \n",ret);
    }else{
        printf("recv1 defeat %d \n",ret);
    }

    for (int i = 0; i < sizeof(buff); ++i) {
        printf("%d ",buff[i]);
    }
    printf("\r\n");

    //发送
    ret = send_data(recv_fd,date, sizeof(date));
    if(ret > 0){
        printf("Send1 successfully %d \n",ret);
    }else{
        printf("Send1 defeat \n");
    }

    close(recv_fd);

    // while (1)
    // {
    //     // 打印接收到的数据
    //     printf("Received Data: ");
    // }
    

    return 0;
}
