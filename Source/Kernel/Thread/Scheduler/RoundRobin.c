#include <Kernel.h>

extern bool fpu;
extern bool mmx;
extern bool sse;

int RoundRobin(int err, int esp) {
    CTask *TaskPool = (CTask*)MMAP_TASKPOOL;
    int prev = TaskPool->ReadyQ[TaskPool->ReadyHead];
    TaskPool->ReadyHead = (++TaskPool->ReadyHead) % TaskPool->ReadyTail;
    int next = TaskPool->ReadyQ[TaskPool->ReadyHead];
    TaskPool->CtxList[prev] = esp;
    int ctx = TaskPool->CtxList[next];
    TaskPool->Tss->esp0 = ctx + 0x3C;

    /*
    if (fpu|sse) clts();
    if (fpu) {
        fnsave ((float*)(esp + 0x0140));
        frstor ((float*)(ctx + 0x0140));
    }
    if (sse) {
        fxsave ((float*)(esp + 0x0240));
        fxrstor((float*)(ctx + 0x0240));
    }
    */
    return ctx;
}