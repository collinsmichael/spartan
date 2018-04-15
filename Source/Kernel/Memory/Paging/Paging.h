#ifndef KERNEL_MEMORY_PAGING_H
#define KERNEL_MEMORY_PAGING_H
#include <oop.h>
#pragma pack(push, 1)

typedef struct IPaging {
    bool (*AppPages)(void *pcb);
} IPaging;

#define PG_ACCESSED 0x20
#define PG_DIRTY    0x40

#pragma pack(pop)
#endif KERNEL_MEMORY_PAGING_H