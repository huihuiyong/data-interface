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


int receive_from_shared_memory(void *recv_data, int32_t size)
{
    // 打开或创建信号量
    sem_t *sem_sender = sem_open(SENDER_SEM_NAME, O_CREAT | O_RDWR , 0666, 0);
    sem_t *sem_receiver = sem_open(RECEIVE_SEM_NAME, O_CREAT | O_RDWR , 0666, 0);

    sem_wait(sem_sender);

    // 打开共享内存
    int shm_fd = shm_open("/my_shared_memory", O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, SHARED_MEM_SIZE);

    //  // 获取属性 
    // struct stat buf;
    // if (fstat(shm_fd, &buf) == -1) {
    //     perror("fstat failed:");
    //     exit(1);
    // }
    // printf("[rec]the shm object size is %ld\n", buf.st_size);

    // 映射共享内存到进程的地址空间
    SharedData *shared_data = (SharedData *)mmap(NULL, SHARED_MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    int datasize = shared_data->dataSize;
    shared_data->data = shared_data + 1;

    memcpy(recv_data, shared_data->data, size);

    // 清理共享内存中的数据
    shared_data->dataSize = 0;
    // sharedData->data = NULL;

    if (munmap(shared_data, SHARED_MEM_SIZE) == -1) {
        perror("munmap");
        return -1;
    }

    sem_post(sem_receiver);

    close(shm_fd);
    sem_close(sem_sender);
    sem_close(sem_receiver);
    return datasize;
}

int main() {
    
    int size=1000;  //SHARED_MEM_SIZE
    void * receive_data = malloc(size);


    while (1)
    {
        int data_size = receive_from_shared_memory(receive_data,size);
        printf("data_size: %d\n", data_size);

        if(data_size != 0){
            printf("Received successfully \n");
        }else{
            printf("Received defeat \n");
        }

        // 打印接收到的数据
        printf("Received Data: ");
        // 假设 shared_data->count 存储了接收到的数据的数量
        for (int i = 0; i < data_size/sizeof(char); ++i) {
            printf("%x ",*((unsigned char*)receive_data+i));
        }
        printf("\r\n");
    }
    
    SemUnlink();

    return 0;
}
