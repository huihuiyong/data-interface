// lib.h
#ifndef LIB_H
#define LIB_H

#include <sys/sem.h>

#define SHARED_MEM_SIZE 101*1024*1024  //101M

typedef struct {
    long long data[100*1024*1024/8];  // 用于存储 long long 数据的数组
    int count;            // 数据的数量
} SharedData;

#endif 
