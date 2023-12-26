#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include "lib.h"

int main() {

    int date[20];  
    char date1[60] = {0x78, 0xbf, 0x6c, 0xe5, 0x4b, 0x4a, 0xf4, 0xaf, 0x86, 0x57, \
                        0x78, 0xbf, 0x6c, 0xe5, 0x4b, 0x4a, 0xf4, 0xaf, 0x86, 0x57, \
                        0x78, 0xbf, 0x6c, 0xe5, 0x4b, 0x4a, 0xf4, 0xaf, 0x86, 0x57, \
                        0x78, 0xbf, 0x6c, 0xe5, 0x4b, 0x4a, 0xf4, 0xaf, 0x86, 0x57, \
                        0x78, 0xbf, 0x6c, 0xe5, 0x4b, 0x4a, 0xf4, 0xaf, 0x86, 0x57, \
                        0x78, 0xbf, 0x6c, 0xe5, 0x4b, 0x4a, 0xf4, 0xaf, 0x86, 0x57}; 

    //char date2[10] = {0x6c, 0x4b, 0x57, 0xf4, 0x4a, 0x86, 0xaf, 0xbf, 0xe5, 0x78}; 

    char date2[100] = {22, 44, 45, 86, 34, 97, 24, 64, 24, 75, \
                        22, 44, 45, 86, 34, 97, 24, 64, 24, 75, \
                        22, 44, 45, 86, 34, 97, 24, 64, 24, 75, \
                        22, 44, 45, 86, 34, 97, 24, 64, 24, 75, \
                        22, 44, 45, 86, 34, 97, 24, 64, 24, 75, \
                        22, 44, 45, 86, 34, 97, 24, 64, 24, 75, \
                        22, 44, 45, 86, 34, 97, 24, 64, 24, 75, \
                        22, 44, 45, 86, 34, 97, 24, 64, 24, 75, \
                        22, 44, 45, 86, 34, 97, 24, 64, 24, 75, \
                        22, 44, 45, 86, 34, 97, 24, 64, 24, 75}; 

    //初始化
    int send_fd = open_client(CHANNLE_NAME);
    if(send_fd>0){
        printf("send_ok:%d\n",send_fd);
    }else{
        printf("send_ERR: %d\n", send_fd);
    }

    //发送
    int ret = send_data(send_fd,date2, sizeof(date2));
    if(ret > 0){
        printf("Send successfully %d \n",ret);
    }else{
        printf("Send defeat \n");
    }

    //接收
    ret = recv_data(send_fd,date, sizeof(date));
    if(ret > 0){
        printf("recv successfully %d \n",ret);
    }else{
        printf("recv defeat %d \n",ret);
    }

    for (int i = 0; i < ret/sizeof(int); ++i) {
        printf("%d ",date[i]);
    }
    printf("\r\n");

    close(send_fd);

  
    return 0;
}
