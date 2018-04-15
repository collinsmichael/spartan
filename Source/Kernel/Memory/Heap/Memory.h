#ifndef KERNEL_MEMORY_H
#define KERNEL_MEMORY_H
#include <oop.h>
#pragma pack(push, 1)

typedef struct IMemory {
    void *(*Alloc)(int len);
    bool  (*Free )(void *buf);
} IMemory;

#pragma pack(pop)
#endif KERNEL_MEMORY_H