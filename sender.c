#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "lib.h"

int send_data_to_shared_memory(void *newdata, size_t dataSize)
{
    if (dataSize >SHARED_MEM_SIZE)
    {
        printf("The data is too large to transmit!!!\n");
        return -1;
    }
    // 打开或创建信号量
    sem_t *sem_sender = sem_open(SENDER_SEM_NAME, O_CREAT | O_RDWR , 0666, 0);
    sem_t *sem_receiver = sem_open(RECEIVE_SEM_NAME, O_CREAT | O_RDWR , 0666, 0);
    
    // 打开或创建共享内存
    int shm_fd = shm_open("/my_shared_memory", O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, SHARED_MEM_SIZE);

    // 获取属性 
    struct stat buf;
    if (fstat(shm_fd, &buf) == -1) {
        perror("fstat failed:");
        exit(1);
    }
    // printf("the shm object size is %ld\n", buf.st_size);

     // 映射共享内存到进程的地址空间
     SharedData *shared_data = (void*)mmap(NULL, SHARED_MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shared_data == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
        return -1;
    }
    shared_data->data = shared_data + 1;

    shared_data->dataSize = dataSize;
    memcpy(shared_data->data, newdata, shared_data->dataSize);

    // 通知接收进程可以读取了
    sem_post(sem_sender);

    // 等待接收进程读取完毕后再继续发送
    sem_wait(sem_receiver);

    if (munmap(shared_data, SHARED_MEM_SIZE) == -1) {
        perror("munmap");
        return -1;
    }
    
    close(shm_fd);
    sem_close(sem_sender);
    sem_close(sem_receiver);
    
    return 0;

}

int main() {

    // int date[10] = {111, 222, 333, 444, 555, 666, 777, 888, 999, 1010};  
    char date1[60] = {0x78, 0xbf, 0x6c, 0xe5, 0x4b, 0x4a, 0xf4, 0xaf, 0x86, 0x57, \
                        0x78, 0xbf, 0x6c, 0xe5, 0x4b, 0x4a, 0xf4, 0xaf, 0x86, 0x57, \
                        0x78, 0xbf, 0x6c, 0xe5, 0x4b, 0x4a, 0xf4, 0xaf, 0x86, 0x57, \
                        0x78, 0xbf, 0x6c, 0xe5, 0x4b, 0x4a, 0xf4, 0xaf, 0x86, 0x57, \
                        0x78, 0xbf, 0x6c, 0xe5, 0x4b, 0x4a, 0xf4, 0xaf, 0x86, 0x57, \
                        0x78, 0xbf, 0x6c, 0xe5, 0x4b, 0x4a, 0xf4, 0xaf, 0x86, 0x57}; 

    char date2[10] = {0x6c, 0x4b, 0x57, 0xf4, 0x4a, 0x86, 0xaf, 0xbf, 0xe5, 0x78}; 
  
    int res = send_data_to_shared_memory(date1,  sizeof(date1));
    if(res==0){
        printf("Send successfully \n");
    }else{
        printf("Send defeat \n");
    }
        
    //     res = send_data_to_shared_memory(date2,  sizeof(date2));
    //     if(res==0){
    //         printf("Send2 successfully \n");
    //     }else{
    //         printf("Send defeat \n");
    //     }

    // res = send_data_to_shared_memory(date1,  sizeof(date1));
    // if(res==0){
    //     printf("Send3 successfully \n");
    // }else{
    //     printf("Send defeat \n");
    // }
        
    //     res = send_data_to_shared_memory(date2,  sizeof(date2));
    //     if(res==0){
    //         printf("Send4 successfully \n");
    //     }else{
    //         printf("Send defeat \n");
    //     }

    //     res = send_data_to_shared_memory(date1,  sizeof(date1));
    // if(res==0){
    //     printf("Send5 successfully \n");
    // }else{
    //     printf("Send defeat \n");
    // }
        
    //     res = send_data_to_shared_memory(date2,  sizeof(date2));
    //     if(res==0){
    //         printf("Send6 successfully \n");
    //     }else{
    //         printf("Send defeat \n");
    //     }

    SemUnlink();
    return 0;
}
