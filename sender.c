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

void send_data_to_shared_memory(int count, long long *date)
{
    // 打开或创建共享内存
    int shm_fd = shm_open("/my_shared_memory", O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, SHARED_MEM_SIZE);

    // 获取属性 
    struct stat buf;
    if (fstat(shm_fd, &buf) == -1) {
        perror("fstat failed:");
        exit(1);
    }
    printf("the shm object size is %ld\n", buf.st_size);

    // 映射共享内存到进程的地址空间
    SharedData *shared_data = (SharedData *)mmap(NULL, SHARED_MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    // 打开或创建信号量
    sem_t *sem = sem_open("/my_semaphore", O_CREAT, 0666, 0);

    // 假设你有一些 long long 数据要发送
    for (int i = 0; i < count; ++i) {
        shared_data->data[i] = *(date + i);
        printf("[send]%lld\n", shared_data->data[i]);
    }
    shared_data->count = count;

    close(shm_fd);
    // 通知接收进程可以读取了
    sem_post(sem);

    // 等待接收进程读取完毕后再继续发送
    sem_wait(sem);
}

int main() {
    long long date[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // for (int i = 0; i < 10; i++)
	// {
	// 	printf("array1D[%d] = %lld\n", i, *(date+i));
	// }
    send_data_to_shared_memory(10, date);
    // close(shm_fd);
    // sem_close(sem);
    sem_unlink("/my_semaphore");

    return 0;
}
