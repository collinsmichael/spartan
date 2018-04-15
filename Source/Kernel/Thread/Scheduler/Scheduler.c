#include <Kernel.h>

extern IMonitor *Monitor;
extern IPool    *Pool;

bool InstallScheduler(u8 *base, u32 size) {
    CTask *TaskPool = (CTask*)MMAP_TASKPOOL;
    stosd(TaskPool, 0, sizeof(CTask)/4);

    if (!(TaskPool->Tss     = (CTss*)Monitor->GetBase('TSS '))) return false;
    if (!(TaskPool->Node    = (char*)Monitor->GetBase('TASK'))) return false;
    if (!(TaskPool->ReadyQ  = (int *)Monitor->GetBase('RDYQ'))) return false;
    if (!(TaskPool->BlockQ  = (int *)Monitor->GetBase('BLKQ'))) return false;
    if (!(TaskPool->KillQ   = (int *)Monitor->GetBase('KILL'))) return false;
    if (!(TaskPool->CtxList = (int *)Monitor->GetBase('CTX '))) return false;
    if (!(TaskPool->PcbList = (int *)Monitor->GetBase('PCB '))) return false;
    if (!(TaskPool->PebList = (int *)Monitor->GetBase('PEB '))) return false;

    stosd(TaskPool->ReadyQ,  0, Monitor->GetSize('RDYQ')/4);
    stosd(TaskPool->BlockQ,  0, Monitor->GetSize('BLKQ')/4);
    stosd(TaskPool->KillQ,   0, Monitor->GetSize('KILL')/4);
    stosd(TaskPool->CtxList, 0, Monitor->GetSize('CTX ')/4);
    stosd(TaskPool->PcbList, 0, Monitor->GetSize('PCB ')/4);
    stosd(TaskPool->PebList, 0, Monitor->GetSize('PEB ')/4);

    void *node = TaskPool->Node;
    void *data = TaskPool->CtxList;

    Pool->CreateMeta((CPool*)TaskPool, 4*KB, 4, node, data, "TASK");
    Monitor->Protect('IVT ');
    Monitor->Protect('TASK');
    Monitor->Protect('RDYQ');
    Monitor->Protect('BLKQ');
    Monitor->Protect('KILL');
    Monitor->Protect('CTX ');
    Monitor->Protect('PCB ');
    Monitor->Protect('PEB ');
    return true;
}