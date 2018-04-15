#ifndef KERNEL_THREAD_EXCEPTION_EXCEPTION_H
#define KERNEL_THREAD_EXCEPTION_EXCEPTION_H
#include <oop.h>
#pragma pack(push, 1)

typedef struct IException {
    bool (*Install)(void);
} IException;

#pragma pack(pop)
#endif KERNEL_THREAD_EXCEPTION_EXCEPTION_H