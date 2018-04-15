#ifndef KERNEL_THREAD_H
#define KERNEL_THREAD_H
#include <oop.h>
#pragma pack(push, 1)

typedef struct CContext {
    int cr3;    // 00
    int ds;     // 04
    int edi;    // 08
    int esi;    // 0C
    int ebp;    // 10
    int esp;    // 14
    int ebx;    // 18
    int edx;    // 1C
    int ecx;    // 20
    int eax;    // 24
    int eip;    // 28
    int cs;     // 2C
    int eflags; // 30
    int esp0;   // 34
    int ss;     // 38
    int pid;    // 3C
} CContext;

typedef struct IThread {
    bool      (*Debug  )(void);
    char     *(*Status )(char *buf);
    int       (*GetPid )(void);
    CContext *(*Fork   )(u32 cr3, u32 eip, u32 esp0, u32 esp3);
    CContext *(*Spawn  )(u32 cr3, u32 eip, u32 esp0, u32 esp3);
    bool      (*Run    )(int pid);
    bool      (*Yield  )(void);
    void      (*Exit   )(int code);
    bool      (*Kill   )(int pid);
    bool      (*Block  )(int pid);
    bool      (*Unblock)(int pid);
} IThread;

#pragma pack(pop)
#endif KERNEL_THREAD_H