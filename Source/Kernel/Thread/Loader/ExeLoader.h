#ifndef KERNEL_THREAD_PROCESS_EXELOADER_H
#define KERNEL_THREAD_PROCESS_EXELOADER_H
#include <oop.h>
#include <Kernel\Thread\Process\Process.h>
#pragma pack(push, 1)

typedef struct ILoader {
    bool  (*Reloc)(char *path, char *base, int size);
    bool  (*Link )(char *path, char *base, int size);
    void *(*GetEntry)(char *path);
    u32   (*CalcFootPrint)(char *image, CPcb *pcb, int overhead, int alignment);
} ILoader;

#pragma pack(pop)
#endif KERNEL_THREAD_PROCESS_EXELOADER_H