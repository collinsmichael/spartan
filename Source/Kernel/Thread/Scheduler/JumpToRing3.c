#include <Kernel.h>
#include <UnitTest.h>

extern IMonitor *Monitor;
extern IThread  *Thread;
extern bool FromRing0;

CPcb *IProcess_Load(char *path);
CContext *IProcess_Enqueue(CPcb *pcb);

bool JumpToRing3(char *path) {
  //if (!TestRunner(3)) for (;;);
    FromRing0 = true;

    CPcb *pcb = IProcess_Load(path);
    if (!pcb) return false;
    CPeb *peb = pcb->Peb;
    if (!peb) return false;

    CContext *ctx = IProcess_Enqueue(pcb);
    if (!ctx) return false;
    movsd(peb->Pages, Monitor->GetBase('PAGE'), 4*KB/4);

    CTask *TaskPool = (CTask*)MMAP_TASKPOOL;
    TaskPool->Tss->esp0 = (int)&ctx->pid;

    FromRing0 = false;
    Thread->Run(ctx->pid);
    ring3(ctx);
    return true;
}