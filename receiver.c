#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <sys/stat.h>
#include "lib.h"

long long *receive_from_shared_memory()
{
    long long *recv_data;
     // 打开共享内存
    int shm_fd = shm_open("/my_shared_memory", O_RDWR, 0666);
    ftruncate(shm_fd, SHARED_MEM_SIZE);

     /* 获取属性 */
    struct stat buf;
    if (fstat(shm_fd, &buf) == -1) {
        perror("fstat failed:");
        exit(1);
    }
    printf("[rec]the shm object size is %ld\n", buf.st_size);

    // 映射共享内存到进程的地址空间
    SharedData *shared_data = (SharedData *)mmap(NULL, SHARED_MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    recv_data = (long long *)malloc(shared_data->count * sizeof(long long));

    // 打开或创建信号量
    sem_t *sem = sem_open("/my_semaphore", O_CREAT, 0666, 0);

    // 你的数据接收逻辑，这里只是一个示例
    // 假设你要处理接收到的 long long 数据
    for (int i = 0; i < shared_data->count; ++i) {
        *(recv_data + i) =shared_data->data[i]; 
        // printf("Rec data: %lld\n", *(recv_data + i));
        printf("Rec data: %lld\n", shared_data->data[i]);
    }

    // 清理共享内存中的数据
    shared_data->count = 0;

    // 通知发送进程可以继续发送
    sem_post(sem);
    return recv_data;
}

int main() {
    
    long long *receive_data;
    receive_data = receive_from_shared_memory();

    // 打印接收到的数据
    printf("Received Data:\n");
    // 假设 shared_data->count 存储了接收到的数据的数量
    for (int i = 0; i < 11; ++i) {
        printf("%lld ", *(receive_data+i));
    }
    // // 清理资源
    // munmap(shared_data, SHARED_MEM_SIZE);
    // close(shm_fd);
    // sem_close(sem);
    sem_unlink("/my_semaphore");

    return 0;
}
