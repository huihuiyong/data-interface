// lib.h
#ifndef LIB_H
#define LIB_H

#include <sys/sem.h>

#define SHARED_MEM_SIZE 100*1024*1024+2  //100M
#define SENDER_SEM_NAME "/sender"
#define RECEIVE_SEM_NAME "/receive"

typedef struct {
    size_t dataSize;      //数据大小
    void *data;           // 用于存储数据的数组
} SharedData;


void SemUnlink(void);

#endif 
