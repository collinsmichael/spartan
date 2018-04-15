#ifndef KERNEL_MEMORY_HEAP_POOL_H
#define KERNEL_MEMORY_HEAP_POOL_H
#include <oop.h>
#include <Kernel\BootLoad\Intel\Tss.h>
#pragma pack(push, 1)

typedef struct CPool {
    char  Name[4];
    void *Meta;
    char *Node;
    char *Data;
    int   Gran;
    int   Count;
    int   Next;
    int   Pad;
} CPool;

typedef struct CTask {
    char  Name[4];
    CTss *Tss;
    char *Node;
    char *Data;
    int   Gran;
    int   Count;
    int   Next;

    int  *CtxList;
    int  *PcbList;
    int  *PebList;
    int  *ReadyQ;
    int  *BlockQ;
    int  *KillQ;
    int   ReadyHead;
    int   ReadyTail;
    int   BlockHead;
    int   BlockTail;
    int   KillHead;
    int   KillTail;
} CTask;

typedef struct CEvtPool {
    char  Name[4];
    void *Meta;
    char *Node;
    char *Data;
    int   Gran;
    int   Count;
    int   Next;
    int   Hanging;
    int   Head;
    int   Tail;
} CEvtPool;

typedef struct IPool {
    bool  (*CreateMeta)(CPool *pool, int count, int granularity, char *nodes, char *data, char *name);
    bool  (*Create )(CPool *pool, int size, int granularity);
    int   (*Alloc  )(CPool *pool);
    int   (*Release)(CPool *pool, int index);

    void *(*Insert )(CPool *pool, void *item);
    bool  (*Remove )(CPool *pool, void *item);
    bool  (*Debug  )(CPool *pool);
} IPool;

#pragma pack(pop)
#endif KERNEL_MEMORY_HEAP_POOL_H